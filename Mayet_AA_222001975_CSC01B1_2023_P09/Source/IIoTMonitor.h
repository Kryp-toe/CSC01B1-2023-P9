#ifndef IIOTMONITOR_H
#define IIOTMONITOR_H

#include <string>

using namespace std;

/********************************************************************************************************

Remember to disable alignment padding for the
IIoTSensor record structure. Only active sensors must be exported to the binary
file.

*********************************************************************************************************/

#pragma pack(push) //Data allignment setting placed into stack
#pragma pack(2) //Set allignment setting to 2

struct IIoTSensors
{
    // AB12CDEF 1 12.34 Y
    string ID; //Extra char for tab
    int DATA_FORMAT;
    double READING;
    bool ACTIVE;
};

#pragma pack(pop) //Remove allignment so it compiles

class IIoTMonitor
{
public:
    IIoTMonitor(string strFileName);
    
    friend std::ostream& operator<<(std::ostream& osLHS, IIoTMonitor& objRHS);
    
    //saves all the IIoTSensors to a binary file
    void exportSensorsToBinary(string strFileName) const;
    
    //computes the average sensor reading
    // The calculated value must be appended to the end of sensor_readings.cleartext.
    void exportAverageReadingToTXT(string strFileName) const;
    
    //returns the number of sensors in the binary file you created earlier
    int getNumSensorsInBinary(string strFileName) const;
    
    //After updating the 2D array, all active sensors must be rewritten to the binary file in truncate mode
    void updateSensor(int row, int col, string strFileName);
    
    static const int ROWS = 5;
    static const int COLS = 5;
    static const int ID_LENGTH = 9;
    
private:
    int NUM_ROWS;
    int NUM_COLS;
    IIoTSensors** DATA;
    
    void dealloc();
    void alloc(int rows, int cols);
    
    //initialise an IIoTMonitor object from the sensors in the provided file
    void readSensorsFromTXT(string strFileName);
    
};

#endif