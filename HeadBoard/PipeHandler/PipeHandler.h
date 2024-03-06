#ifndef PIPE_HANDLER_H
#define PIPE_HANDLER_H

#include <string>


class PipeHandler
{
	public:
		// TODO automatically create pipe if it doesn't exist
		// first instance is used for testing at the moment
		PipeHandler(const std::string pipeName, bool firstInstance);
		std::string read() const;
		void write(std::string command);

	private:
		const std::string _pipeName;

		bool createPipes();
};

#endif
