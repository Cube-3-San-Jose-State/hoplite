#include "PipeHandler.h"
#include <filesystem>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
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
	ifstream readPipe(_inPipe);

	if(!readPipe.is_open())
	{
		cerr << "Could not read from pipe, fs error" << endl;
		return "";
	}

	stringstream command;
	command << readPipe.rdbuf();
	readPipe.close();
	return command.str();
}


void PipeHandler::write(std::string command)
{
	ofstream writePipe(_outPipe);

	if(!writePipe.is_open())
	{
		cerr << "Could write to pipe, fs error" << endl;
	}

	writePipe << command << endl;
	writePipe.close();
}


/****************************************************************************************************
* Private Methods
****************************************************************************************************/
bool PipeHandler::createPipes()
{
	// move constructor code here later?
	// so it can be called mutliple times if it fails the first time
}

