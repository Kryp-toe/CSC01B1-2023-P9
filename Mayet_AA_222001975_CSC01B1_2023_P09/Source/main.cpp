#include "IIoTMonitor.h"
#include <iostream>

using namespace std;

int main()
{
    IIoTMonitor objMonitor("data/sensor_readings.cleartext");
    cout << "Before Cahnges: " << endl << endl << objMonitor;
    
    objMonitor.exportAverageReadingToTXT("data/sensor_averages.cleartext");
    
    int row = 0;
    int col = 0;
    cout << "What row and col do you want to change??" << endl;
    cin >> row;
    cin >> col;
    
    objMonitor.updateSensor(row, col, "data/sensor_binary.cleartext");
    
    cout << "After Cahnges: " << endl << endl << objMonitor;
    
    return 0;
}