#include <iostream>
#include "BatterySensor.h"

/**
 * Collects and manages data from the power board (battery voltage, solar panel)
 * 
 * Responsibilities: Detect when devices need to be power cycled, shut off to cool, 
 * power needs to be conserved, etc. Requirements are subject to change.
*/

using namespace std;

int main(int argc, char const *argv[])
{
    /* code */
    string outputFromLibrary = hello();
    string outputFromSelf = "hello from BatterySensor.cpp";
    cout << outputFromLibrary << endl;
    cout << outputFromSelf;
    return 0;
}
