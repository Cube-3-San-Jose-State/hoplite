#include "PipeHandler.h"
#include <iostream>

using namespace std;


PipeHandler::PipeHandler(const std::string pipeName, bool firstInstance)
: _pipeName(pipeName)
{

}


std::string PipeHandler::read() const
{

}


void PipeHandler::write(std::string command)
{

}


/****************************************************************************************************
* Private Methods
****************************************************************************************************/
bool PipeHandler::createPipes()
{

}

