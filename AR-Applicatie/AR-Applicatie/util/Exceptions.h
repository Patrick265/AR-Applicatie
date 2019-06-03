#pragma once

#include <iostream>
#include <string>

class Exceptions : std::exception
{
	
public:
	Exceptions(const std::string& msg) : exceptionmsg(msg) {}
	Exceptions() {}
	~Exceptions() {}

	std::string getExceptionMessage() const {return(exceptionmsg);}

	static void noCameraDetected();
	static void invalidFrames();

private:
	std::string exceptionmsg;
};
