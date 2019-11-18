#include <Bela.h>
#include <libraries/Scope/Scope.h>

Scope scope;

float dcIn = 0;
float dcOut = 0;

float gInverseSampleRate;
float gCalcFreq = 50.0;
float gPhase;


bool setup(BelaContext *context, void *userData)
{
	scope.setup(5, context->audioSampleRate);
	gInverseSampleRate = 1 / context->audioSampleRate;
	gPhase = 0.0;
	return true;
}

void render(BelaContext *context, void *userData)
{
	
	float accelOut = 0;
	for (int n = 0; n < context->audioFrames; n++) {
		
		if (!(n % 2)) {
			float in = analogRead(context, n/2, 0);
			accelOut = (in - dcIn + 0.9998 * dcOut);
			dcIn = in;
			dcOut = accelOut;
			gCalcFreq = map(accelOut, 0, 0.05, 50, 800);

		}
		float out = 0.8f * sinf(gPhase);
		gPhase += 2.0f * (float)M_PI * gCalcFreq * gInverseSampleRate;
		if(gPhase > M_PI)
			gPhase -= 2.0f * (float)M_PI;
		for(unsigned int channel = 0; channel < context->audioOutChannels; channel++) {
			audioWrite(context, n, channel, out);
		}
		
	scope.log(accelOut, out);
	}

}

void cleanup(BelaContext *context, void *userData)
{

}