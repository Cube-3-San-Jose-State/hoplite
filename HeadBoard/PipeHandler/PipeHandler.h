#ifndef PIPE_HANDLER_H
#define PIPE_HANDLER_H

#include <filesystem>
#include <string>


class PipeHandler
{
	public:
		// TODO automatically create pipe if it doesn't exist
		// first instance is used for testing at the moment
		PipeHandler(const std::string pipeName, bool firstInstance);
		~PipeHandler();
		std::string read() const;
		void write(std::string command);

	private:
		const std::filesystem::path _hoplitePipesDir = "/tmp/hoplite/";
		const std::string _pipeName;
		const bool _firstInstance;
		std::filesystem::path _inPipe;
		std::filesystem::path _outPipe;

		bool createPipes();
};

#endif
