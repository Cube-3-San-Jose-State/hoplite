#include "IPCQueue.h"
#include <cstddef>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

namespace fs = std::filesystem;
using std::cout;
using std::endl;
using std::string;


/****************************************************************************************************
* Constructor/Destructor
****************************************************************************************************/

IPCQueue::IPCQueue(const std::string IPCName)
{
	fs::create_directories(QUEUES_DIR);
	_queueFile = QUEUES_DIR / IPCName;
	// write/read proj_ids
	int outID;
	int inID;

	// if queue already created
	if(fs::exists(_queueFile))
	{
		// cout << "queue file already exists" << endl;
		_first_instance = false;
		outID = 1;
		inID = 0;
	}

	else
	{
		_first_instance = true;
		// create queue file
		// cout << "queue file does not exist, creating" << endl;
		std::ofstream qf(_queueFile);
		qf << "queue created!" << endl;
		qf.close();
		// order swapped compared to existing queue
		outID = 0;
		inID = 1;
	}
	
	// create message queues
	key_t outKey = ftok(_queueFile.c_str(), outID);
	key_t inKey = ftok(_queueFile.c_str(), inID);
	_out_qid = msgget(outKey, 0666 | IPC_CREAT);
	_in_qid = msgget(inKey, 0666 | IPC_CREAT);
}


IPCQueue::~IPCQueue()
{
	// first instance gets to clean up
	if(_first_instance)
	{
		cout << "IPCQueue: first instance destroyed, cleaning up" << endl;
		// destroy queues
		int out = msgctl(_out_qid, IPC_RMID, NULL);
		int in = msgctl(_in_qid, IPC_RMID, NULL);
		// cout << "removed out: " << out << "\nremoved in: " << in << endl;
		// delete queue file
		fs::remove(_queueFile);
	}
}


/****************************************************************************************************
* Member functions
****************************************************************************************************/

std::string IPCQueue::read() const
{
	QueuedMessage msg;
	int isMsg = msgrcv(_in_qid, &msg, sizeof(msg), 0, MSG_NOERROR | IPC_NOWAIT);

	// return empty string if there is nothing to read
	if(isMsg < 0)
	{
		return "";
	}

	return string(msg.message);
}


void IPCQueue::write(std::string command)
{
	// prepare message
	QueuedMessage msg;
	msg.type = 1;
	strcpy(msg.message, command.c_str());
	// send it
	msgsnd(_out_qid, &msg, sizeof(msg), IPC_NOWAIT);
}


