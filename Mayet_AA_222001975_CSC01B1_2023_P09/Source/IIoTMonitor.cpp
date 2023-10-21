#include "IIoTMonitor.h"
#include <fstream>
#include <iostream>

using namespace std;

IIoTMonitor::IIoTMonitor(string strFileName)
{
    readSensorsFromTXT(strFileName);
}

//initialise an IIoTMonitor object from the sensors in the provided file
void IIoTMonitor::readSensorsFromTXT(string strFileName)
{
    //Open file
    ifstream ifStreamFile;
    ifStreamFile.open(strFileName);
    
    if(ifStreamFile.fail())
    {
        cerr << "Could not open file " << strFileName << endl;
        exit(-1);
    }
    
    //Read rows and cols
    ifStreamFile >> NUM_ROWS >> NUM_COLS;
    
    if(NUM_COLS == 0 || NUM_ROWS == 0)
    {
        cerr << "Failed to read Rows and Cols" << endl;
        exit(-2);
    }
    
    //alloc mem
    alloc(NUM_ROWS, NUM_COLS);
    
    string strID;
    int intFormat;
    double dblReading;
    char boolActive;
    
    int r = 0;
    int c = 0;
    
    //Read data
    while(ifStreamFile >> strID >> intFormat >> dblReading >> boolActive)
    {
        if(r <= NUM_ROWS && c <= NUM_COLS)
        {
            DATA[r][c].ID = strID; // Every string in C++ must be terminated by a null character constant.
            DATA[r][c].DATA_FORMAT = intFormat;
            DATA[r][c].READING = dblReading;
            
            if(boolActive == 'N')
            {
                DATA[r][c].ACTIVE = false;
            }
            else if(boolActive == 'Y')
            {
                DATA[r][c].ACTIVE = true;
            }
            
            c++;
            
            if(c == NUM_COLS)
            {
                r++;
                c = 0;
            }
        }
        else
        {
            break;
        }
        
    }
    
    ifStreamFile.close();
}

//Cout
ostream& operator<<(ostream& osLHS, IIoTMonitor& objRHS)
{
    for(int r=0; r < objRHS.NUM_ROWS; r++)
    {
        for(int c=0; c < objRHS.NUM_COLS; c++)
        {
            osLHS << objRHS.DATA[r][c].ID << ' ';
            osLHS << objRHS.DATA[r][c].DATA_FORMAT << ' ';
            osLHS << objRHS.DATA[r][c].READING << ' ';
            osLHS << objRHS.DATA[r][c].ACTIVE << ' ';
        }
    }
    
    return osLHS;
}

//saves all the IIoTSensors to a binary file
void IIoTMonitor::exportSensorsToBinary(string strFileName) const
{
    //Open file
    fstream fWriter;
    fWriter.open(strFileName, ios::binary | ios::out);
    if(fWriter.fail())
    {
        cerr << "Failed to open file" << endl;
        exit(-3);
    }
    
    //save each sensor to binary
    for(int r = 0; r < NUM_ROWS; r++)
    {
        for(int c = 0; c < NUM_COLS; c++)
        {
            if(DATA[r][c].ACTIVE)
            {
                fWriter.write(reinterpret_cast<char*>(&DATA[r][c]), sizeof(IIoTSensors));
            }
        }
        
    }
    
    fWriter.close();
}

//computes the average sensor reading
// The calculated value must be appended to the end of sensor_readings.cleartext.
void IIoTMonitor::exportAverageReadingToTXT(string strFileName) const
{
    double dblAvg = 0;
    double dblTotal = 0;
    int intNumRec = getNumSensorsInBinary(strFileName);
    
    for(int r = 0; r < NUM_ROWS; r++)
    {
        for(int c = 0; c < NUM_COLS; c++)
        {
            dblTotal += DATA[r][c].READING;
        }
        
    }
    
    dblAvg = dblTotal / intNumRec;
    
    ofstream ofWriter;
    ofWriter.open(strFileName, ios::app);
    if(ofWriter.fail())
    {
        cerr << "Could not open file" << endl;
        exit(-4);
    }
    
    ofWriter << "Average Reading: " << dblAvg << endl;
    
    ofWriter.close();
}

//returns the number of sensors in the binary file you created earlier
int IIoTMonitor::getNumSensorsInBinary(string strFileName) const
{
    //open file
    fstream fReader;
    fReader.open(strFileName, ios::binary | ios::in);
    if(fReader.fail())
    {
        cerr << "Could not open file " << strFileName << endl;
        exit(-1);
    }
    
    //go to begining of file
    fReader.seekg(4, ios::end);
    
    //calc size of file
    int FileSize = fReader.tellg();
    int intNumSensors = FileSize / sizeof(IIoTSensors);
    
    fReader.close();
    return intNumSensors;
}

//After updating the 2D array, all active sensors must be rewritten to the binary file in truncate mode
void IIoTMonitor::updateSensor(int row, int col, string strFileName)
{
    fstream fStreamFile;
    fStreamFile.open(strFileName, ios::trunc | ios::out);
    
    if(!(row <= NUM_ROWS && col <= NUM_COLS))
    {
        cerr << "Invalid Row / Col " << endl;
        exit(-4);
    }
    
    double newVal;
    cout << "Please enter a new value for ID: " << DATA[row][col].ID << endl;
    cin >> newVal;
    
    DATA[row][col].READING = newVal;
    
    exportSensorsToBinary(strFileName);
    
    fStreamFile.close();
}

//alloc memory
void IIoTMonitor::alloc(int rows, int cols)
{
    DATA = new IIoTSensors*[rows];
    for(int r=0; r<rows; r++)
    {
        DATA[r] = new IIoTSensors[cols];
    }
}





