#include <Bela.h>
#include <cmath>
#include <libraries/Scope/Scope.h>

Scope scope;

float gFrequency = 220.0;
float gPhase;
float gInverseSampleRate;


bool setup(BelaContext *context, void *userData)
{
	scope.setup(5, context->audioSampleRate);
	gInverseSampleRate = 1 / context->audioSampleRate;
	gPhase = 0;
	return true;
}

void render(BelaContext *context, void *userData)
{
	for (int n = 0; n < context->audioFrames; n++) {
		float out = 0.8 * sinf(gPhase);
		gPhase += 2.0f * (float)M_PI * gFrequency * gInverseSampleRate;
		if(gPhase > M_PI)
			gPhase -= 2.0f * (float)M_PI;
		
		for(unsigned int channel = 0; channel < context->audioOutChannels; channel++) {
			audioWrite(context, n, channel, out);
		}
		
		scope.log(out);
	}
}

void cleanup(BelaContext *context, void *userData)
{

}