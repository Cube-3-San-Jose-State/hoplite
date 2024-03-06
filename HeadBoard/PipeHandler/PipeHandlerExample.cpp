#include "PipeHandler.h"
#include <iostream>
#include <unistd.h>

using namespace std;


int main()
{
	PipeHandler pipe("yes", true);
	sleep(4);

	return 0;
}
