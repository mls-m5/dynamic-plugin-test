
#include "common.h"
#include "iengine.h"
#include "ielement.h"
#include <portaudio.h>
#include <list>
#include <string.h> //for memcpy

class PortAudioEngine: public IEngine {
	public:
	
	
	int SampleRate = 44100; //Temporary value
	int BufferSize = 100; //Temporary value
	
	std::list<IElement*> elementList;

	bool canCapture = true;
	sample_t *dummyInputSample = 0;
	double masterVolume = 1;
	
	
	PaStream *stream = 0;

	struct callbackData{
		const sample_t *in;
		sample_t *out;
		unsigned int channels;
	};

	static int paCallback (void const * inputBuffer, void * outputBuffer, unsigned long int framesPerBuffer, PaStreamCallbackTimeInfo const * timeInfo, PaStreamCallbackFlags statusFlags, void * userData)
	{
		
		sample_t *out = (sample_t*)outputBuffer;
		const sample_t *in = (const sample_t*)inputBuffer;

		callbackData data = {in, out, 1};

		auto &engine = *static_cast<PortAudioEngine*>(userData);
		engine.process(framesPerBuffer, (void*)&data);

		return paContinue;

	}

	PortAudioEngine() {
	
	}

	virtual ~PortAudioEngine() {}
	
	
	
	bool init(std::string name = "") override {
		if (stream) return 0;

		PaStreamParameters inputParameters, outputParameters;
		PaError err;
		err = Pa_Initialize();
		if( err != paNoError ) return true;



		inputParameters.device = Pa_GetDefaultInputDevice(); // default input device

		if (inputParameters.device == paNoDevice) {
			fprintf(stderr,"Error: No default input device.\n");
			return true;
		}
		else{
			//Skriver ut information för enhet som används
			const PaDeviceInfo * deviceInfo = Pa_GetDeviceInfo(inputParameters.device);
			printf("Device %d: %s\n", inputParameters.device, deviceInfo->name);

			//printf("Allt valbart ljud \n");
			//printInfoList();
			//printf("Standard låg fördröjning: %s\n", deviceInfo->defaultLowInputLatency);
		}

		{
			auto deviceInfo = Pa_GetDeviceInfo(inputParameters.device);

			inputParameters.channelCount = 1;       // mono input
			inputParameters.sampleFormat = paFloat32;
			inputParameters.suggestedLatency = deviceInfo->defaultLowInputLatency;
			inputParameters.hostApiSpecificStreamInfo = 0; //NULL

			outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
			//outputParameters.device = 14;
			if (outputParameters.device == paNoDevice) {
				fprintf(stderr,"Error: No default output device.\n");
				return true;
			}

			outputParameters.channelCount = 1;       // mono output
			outputParameters.sampleFormat = paFloat32;
			outputParameters.suggestedLatency = deviceInfo->defaultLowOutputLatency;
			outputParameters.hostApiSpecificStreamInfo = 0; //NULL
		}

		err = Pa_OpenStream(
				&stream,
				&inputParameters,
				&outputParameters,
				SampleRate,
				BufferSize,
				0, /* paClipOff, */  /* we won't output out of range samples so don't bother clipping them */
				paCallback,
				this ); //NULL
		if( err != paNoError ) {
			return false;
		}

		return true;
	}
	
	
	
	int process (unsigned int nframes, void *arg) {
		
		sample_t *out;
		auto data = (callbackData*)arg;

		if (dummyInputSample == 0){
			//Create empty dummy buffer
			dummyInputSample = new sample_t[nframes];
		}

		auto in = dummyInputSample;

		if (canCapture){
			for (int i = 0; i < nframes; ++i){
				in[i] = data->in[i * data->channels];
			}
		}
		else{
			for (int i = 0; i < nframes; ++i){
				dummyInputSample[i] = 0;
			}
		}
		out = data->out;

		bool firstElement = true;
		for (auto element: elementList){
			if (firstElement){
				firstElement = 0;
			}
			else{
				memcpy(in, out,
						sizeof (sample_t) * nframes);
			}
			element->process(in, out, nframes);
		}
		if (masterVolume != 1){
			const auto volume = masterVolume;
			for (int i = 0; i < nframes; ++i){
				out[i] *= volume;
			}
		}

		for (auto element: elementList){
			element->controlSignal();
		}
		return 0;
	}
	
	
	
	
	bool activate() override {
		PaError err;

		err = Pa_StartStream( stream );
		if( err == paNoError ) return true;

		return false;
	}
	
	
	
	
	bool close() override {
		printf("Finished. Shutting down portaudio" );
		if (stream){
			Pa_StopStream(stream);
			Pa_CloseStream(stream);
			stream = 0;
		}
		Pa_Terminate();

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
	return new PortAudioEngine();
}

