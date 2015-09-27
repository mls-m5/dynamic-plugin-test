
#include "iengine.h"
#include "ielement.h"
#include <jack/jack.h>
#include <list>
#include <string.h> //for memcpy


typedef float sample_t;


/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 */
void
jack_shutdown (void *arg)
{
	exit (1);
}



class JackEngine: public IEngine {
public:
	
	bool initialized = false;

	std::list<class IElement*> elementList;
	jack_port_t *inputPorts = 0;
	jack_port_t *outputPorts = 0;
	jack_client_t *client;
	
	
	bool canCapture = true;
	sample_t *dummyInputSample = 0; //This sample is used if input not is possible
	double masterVolume = 1;
	std::string clientName;
	
	int BufferSize;
	int SampleRate;
	
	JackEngine() {}

	virtual ~JackEngine() {}
	
	bool init(std::string name = "") override {
		if (initialized == true) {
			return true;
		}

		const char **ports = 0;
		const char *server_name = NULL;
		clientName = name;
		jack_options_t options = JackNullOption;
		jack_status_t status;

		// open a client connection to the JACK server */

		client = jack_client_open (clientName.c_str(), options, &status, server_name);
		if (client == NULL) {
			fprintf (stderr, "jack_client_open() failed, "
					"status = 0x%2.0x\n", status);
			if (status & JackServerFailed) {
				fprintf (stderr, "Unable to connect to JACK server\n");
			}
			exit (1);
		}
		if (status & JackServerStarted) {
			fprintf (stderr, "JACK server started\n");
		}
		if (status & JackNameNotUnique) {
			clientName = jack_get_client_name(client);
			fprintf (stderr, "unique name `%s' assigned\n", clientName.c_str());
		}

		// tell the JACK server to call `process()' whenever
		//   there is work to be done.

		jack_set_process_callback (client, process, this);

		// tell the JACK server to call `jack_shutdown()' if
		//   it ever shuts down, either entirely, or if it
		//   just decides to stop calling us.

		jack_on_shutdown (client, jack_shutdown, this);

		// display the current sample rate.

		//	printf ("engine sample rate: %" PRIu32 "\n",
		//		jack_get_sample_rate (client));

		/* create two ports */

		inputPorts = jack_port_register (client, "input",
				JACK_DEFAULT_AUDIO_TYPE,
				JackPortIsInput, 0);
		inputPorts = jack_port_register (client, "input2",
				JACK_DEFAULT_AUDIO_TYPE,
				JackPortIsInput, 0);
		outputPorts = jack_port_register (client, "output",
				JACK_DEFAULT_AUDIO_TYPE,
				JackPortIsOutput, 0);
		outputPorts = jack_port_register (client, "output2",
				JACK_DEFAULT_AUDIO_TYPE,
				JackPortIsOutput, 0);

		if ((inputPorts == NULL) || (outputPorts == NULL)) {
			fprintf(stderr, "no more JACK ports available\n");
			exit (1);
		}

		SampleRate = jack_get_sample_rate(client);
		BufferSize = jack_get_buffer_size(client);

		initialized = true;

		return true;
	}
	
	
	
	static int process (unsigned int nframes, void *arg) {
		auto &engine = *static_cast<JackEngine*>(arg);
		jack_default_audio_sample_t *in, *out;

		if (engine.canCapture){
			in = (jack_default_audio_sample_t*)jack_port_get_buffer (engine.inputPorts, nframes);
		}
		else{
			if (engine.dummyInputSample == 0){
				//Create empty dummy buffer
				engine.dummyInputSample = new jack_default_audio_sample_t[nframes];
			}
			in = engine.dummyInputSample;
			for (int i = 0; i < nframes; ++i){
				in[i] = 0;
			}
		}
		out = (jack_default_audio_sample_t*)jack_port_get_buffer (engine.outputPorts, nframes);

		bool firstElement = true;
		for (auto element: engine.elementList){
			if (firstElement){
				firstElement = 0;
			}
			else{
				memcpy(in, out,
						sizeof (jack_default_audio_sample_t) *  nframes);
			}
			element->process(in, out, nframes);
		}
		if (engine.masterVolume != 1){
			const auto volume = engine.masterVolume;
			for (int i = 0; i < nframes; ++i){
				out[i] *= volume;
			}
		}

		for (auto element: engine.elementList){
			element->controlSignal();
		}
		
		return 0;
	}

	
	
	bool activate() override {
		//* Tell the JACK server that we are ready to roll.  Our
		// * process() callback will start running now. */

	//	jack_set_buffer_size(client, 256);

		if (jack_activate (client)) {
			fprintf (stderr, "cannot activate client");
			exit (1);
		}

		// Connect the ports.  You can't do this before the client is
		// * activated, because we can't make connections to clients
		// * that aren't running.  Note the confusing (but necessary)
		// * orientation of the driver backend ports: playback ports are
		// * "input" to the backend, and capture ports are "output" from
		// * it.
		// 

		auto ports = jack_get_ports (client, NULL, NULL,
				JackPortIsPhysical|JackPortIsOutput);
		if (ports == NULL) {
			fprintf(stderr, "no physical capture ports\n");
			canCapture = false;
	//		exit (1);
		}
		else{
			if (jack_connect (client, ports[0], jack_port_name (inputPorts))) {
				fprintf (stderr, "cannot connect input port 1\n");
			}
			free (ports);
		}

		ports = jack_get_ports (client, NULL, NULL,
				JackPortIsPhysical|JackPortIsInput);
		if (ports == NULL) {
			fprintf(stderr, "no physical playback ports\n");
			exit (1);
		}

		if (jack_connect (client, jack_port_name (outputPorts), ports[0])) {
			fprintf (stderr, "cannot connect output port 1\n");
		}

		if (jack_connect (client, jack_port_name (outputPorts), ports[1])) {
			fprintf (stderr, "cannot connect output port 2\n");
		}

		free (ports);

		return true;
		/* keep running until stopped by the user */
	}
	
	
	
	
	bool close() override {
		/* this is never reached but if the program
		   had some other way to exit besides being killed,
		   they would be important to call.
		 */
		jack_client_close (client);

		if (dummyInputSample){
			delete [] dummyInputSample;
		}

		return true;
	}
	
	
	
	
	void addElement(class IElement *e) override {
		elementList.push_back(e);
	}
	
	
	
	
	void removeElement(class IElement *e) override {
		elementList.remove(e);
	}
	
	
	
	void setVolume(double v) override {
		masterVolume = v;
	}
	
	
	
	double getVolume() override {
		return masterVolume;
	}
};


IEngine *createEngine() {
	return new JackEngine();
}

