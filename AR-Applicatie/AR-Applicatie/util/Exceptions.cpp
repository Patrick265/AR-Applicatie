#include <iostream>
#include "Exceptions.h"

void Exceptions::noCameraDetected() 
{
	throw(Exceptions("The camera is off, try turning on your camera"));
}

void Exceptions::invalidFrames() 
{
	throw(Exceptions("The frames are invalid, try turning on your camera"));
}
