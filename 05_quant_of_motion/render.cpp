#include <Bela.h>
#include <cmath>
#include <libraries/Scope/Scope.h>

Scope scope;

// Vars for creating sine tone and calculating phase
float gInverseSampleRate;
float gPhase = 0;
float gCalcFreq = 50.0;

// Keep track of 3 axes of DC blocking:
double dc_in[3] = { 0 };
double dc_out[3] = { 0 };

// Vars for keeping track of motion over time: Peak and total motion

// Peak is an envelope follower that follows the highest point of the total motion, and rolls off.
float gPeak = 0;
float gRolloff = 0.000029; // You can mess with this

// Total motion on all three combined axes:
double gTotalMotion;
float thresh = 0.00001; // adjust this, originally 0.0000001 but you can adjust it to change the response

bool setup(BelaContext *context, void *userData)
{
	scope.setup(5, context->audioSampleRate);
	gInverseSampleRate = 1.0 / context->audioSampleRate;
	
	return true;
}

void render(BelaContext *context, void *userData)
{
	for (int n = 0; n < context->audioFrames; n++) {
		double all_motion = 0;
		
		// On each analog frame, look at the accelerometer readings:
		if(!(n % 2)) { 
			double axis_motion[3] = { 0 };
			
			// Get the motion on all 3 axes
			for (int j = 0; j < 3; j++) {
				double dataIn = analogRead(context, n/2, j);
				double axisDCBlocked = (dataIn - dc_in[j] + 0.9998 * dc_out[j]);
				axis_motion[j] = axisDCBlocked * axisDCBlocked;
				all_motion += axis_motion[j];
				dc_in[j] = dataIn;
		    	dc_out[j] = axisDCBlocked;
			}
			
			gTotalMotion = sqrt(all_motion);
			
			// Use the total motion to control frequency
			gCalcFreq = map(gTotalMotion, 0, .5, 50, 800);
			
			gTotalMotion -= thresh;
			if (gTotalMotion < 0) {
				gTotalMotion = 0;
			}
			
	    	if (gTotalMotion > gPeak) {
	    		gPeak = gTotalMotion;
	    	}
	    	
		    gPeak -= gRolloff;
		    if (gPeak < 0) {
		    	gPeak = 0;
		    }
		    
		}// end if for analog read
		
		// Generate sine tone with frequency gCalcFreq, based on the quantity of motion
		float out = 0.8f * sinf(gPhase);
		gPhase += 2.0f * (float)M_PI * gCalcFreq * gInverseSampleRate;
		if(gPhase > M_PI) {
			gPhase -= 2.0f * (float)M_PI;
		}

		for(unsigned int channel = 0; channel < context->audioOutChannels; channel++) {
			audioWrite(context, n, channel, out);
		}
		scope.log(gTotalMotion, out);
		
	}// end block for loop
}





void cleanup(BelaContext *context, void *userData)
{

}