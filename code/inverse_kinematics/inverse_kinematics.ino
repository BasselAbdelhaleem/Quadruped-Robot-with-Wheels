#include "Math.h"

int upperL = 265; // upper Leg Length
int lowerL = 400; // lower Leg Length
int inHeight;
int hipAngRad; // hip motor angle
int kneeAngRad; // knee motor angle
int hipAngDeg; // hip motor angle
int kneeAngDeg; // knee motor angle

void setup()
{
  // put your setup code here, to run once:

}

void loop()
{
  hipAngRad = acos((sq(lowerL)-sq(inHeight)-sq(upperL))/(2*inHeight*upperL));
  hipAngDeg = hipAngRad*(180/PI);
  kneeAngRad = acos((sq(inHeight)-sq(lowerL)-sq(upperL))/(2*lowerL*upperL));
  kneeAngDeg = kneeAngRad*(180/PI);
}
