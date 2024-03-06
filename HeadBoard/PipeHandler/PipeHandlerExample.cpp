#include "PipeHandler.h"
#include <iostream>
#include <unistd.h>

using namespace std;


int main()
{
	PipeHandler pipe("yes", true);
	sleep(4);
	pipe.write("test command");
	cout << pipe.read() << endl;

	return 0;
}
