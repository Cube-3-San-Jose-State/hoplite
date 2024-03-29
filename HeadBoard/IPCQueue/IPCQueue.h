#ifndef IPCQUEUE_H
#define IPCQUEUE_H

#include <string>
#include <filesystem>


class IPCQueue
{
	public:
		IPCQueue(const std::string IPCName);
		~IPCQueue();
		std::string read() const;
		void write(std::string command);
		// compile time constant, adjust if needed
		constexpr static long MAX_MESSAGE_LENGTH = 4096;

	protected:
		// for sending/receiving messages
		struct QueuedMessage
		{
			long type;
			char message[MAX_MESSAGE_LENGTH];
		};

		const std::filesystem::path QUEUES_DIR = "/tmp/hoplite/message_queues/";
		std::filesystem::path _queueFile;
		int _in_qid;
		int _out_qid;
		bool _first_instance = true;
};

#endif
