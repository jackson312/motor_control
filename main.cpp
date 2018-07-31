#include <iostream>
#include <cstdint>
#include "Motor_Monitor.hpp"

using namespace std;

int main()
{
    string filename;

    Motor_Monitor *monitor;
    monitor = new( Motor_Monitor );
#if 0
    cout << "Please Enter in filename of simulation file (including path"\
            " if not in current directory: ";
    cin >> filename;
    cout << endl;
#endif
    //filename = "perfect.txt";
    //filename = "normal.txt";
    filename = "error.txt";

    if ( monitor->open_file( filename ) )
    {
        cout << "Simulation File is open" << endl;
    }
    else
    {
        cout << "Error, invalid file name/path: " << filename << endl;
    }

    monitor->calibrate_motor_monitor();
    monitor->read_input();
    monitor->print_largest_delta();

    delete monitor;

    return 0;
}