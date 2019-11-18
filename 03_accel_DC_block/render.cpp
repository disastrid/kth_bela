#include <Bela.h>
#include <libraries/Scope/Scope.h>

Scope scope;

float dcIn[3] = { 0 };
float dcOut[3] = { 0 };


bool setup(BelaContext *context, void *userData)
{
	scope.setup(5, context->audioSampleRate);
	return true;
}

void render(BelaContext *context, void *userData)
{
	
	float accelOut[3] = { 0 };
	for (int n = 0; n < context->audioFrames; n++) {
		
		if (!(n % 2)) {
			for (int m = 0; m < 3; m++) {
				float in = analogRead(context, n/2, m);
				accelOut[m] = (in - dcIn[m] + 0.9998 * dcOut[m]);
				dcIn[m] = in;
				dcOut[m] = accelOut[m];
			}
			
			
			
		
		}
	scope.log(accelOut[0], accelOut[1], accelOut[2]);
	}

}

void cleanup(BelaContext *context, void *userData)
{

}