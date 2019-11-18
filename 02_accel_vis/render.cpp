#include <Bela.h>
#include <libraries/Scope/Scope.h>

Scope scope;

bool setup(BelaContext *context, void *userData)
{
	scope.setup(5, context->audioSampleRate);
	return true;
}

void render(BelaContext *context, void *userData)
{
	float x, y, z;
	for (int n = 0; n < context->audioFrames; n++) {
		
		if (!(n % 2)) {
			x = analogRead(context, n/2, 0);
			y = analogRead(context, n/2, 1);
			z = analogRead(context, n/2, 2);
		}
	scope.log(x, y, z);
	}

}

void cleanup(BelaContext *context, void *userData)
{

}