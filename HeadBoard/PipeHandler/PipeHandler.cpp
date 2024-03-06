#include "PipeHandler.h"
#include <filesystem>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;
namespace fs = std::filesystem;


PipeHandler::PipeHandler(const std::string pipeName, bool firstInstance)
: _pipeName(pipeName), _firstInstance(firstInstance)
{
	// create input and output pipes
	if(_firstInstance)
	{
		fs::path pipeDir = _hoplitePipesDir / pipeName;
		fs::create_directories(pipeDir);
		_outPipe = pipeDir / "0";
		_inPipe = pipeDir / "1";
		// create pipes
		mkfifo(_outPipe.c_str(), 0666);
		mkfifo(_inPipe.c_str(), 0666);
	}

	else
	{
		// pipes were created by another object, thus order is swapped
		_outPipe = _hoplitePipesDir / "1";
		_inPipe = _hoplitePipesDir / "0";
	}
}


PipeHandler::~PipeHandler()
{
	// remove pipes if this object created them
	if(_firstInstance)
	{
		unlink(_outPipe.c_str());
		unlink(_inPipe.c_str());
		fs::remove(_hoplitePipesDir / _pipeName);
	}
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
	// move constructor code here later?
	// so it can be called mutliple times if it fails the first time
}

