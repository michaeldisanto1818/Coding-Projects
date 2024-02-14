#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
using namespace std;

float **createDataMatrix(int numElement, int numSample)
{
    // Create a 2D data matrix of size numElement and numSample
    float **RFData;

    //allocate float pointer with numElement elements
    RFData = new float*[numElement];

    //loop through & allocate new float array of length numSample to create numElement X numSample matrix 
    for (int i = 0; i < numElement; i++)
    {
        RFData[i] = new float[numSample];
    }

    return RFData;
}

int loadRFData(float **RFData, const char *fileName, int numElement, int numSample)
{
    // Open the text file fileName, read the data and store into RFData
    
    //open the desired file
    ifstream infile(fileName);

    //if we fail to open the correct file, end the program
    if (infile.fail())
    {
        return -1;
    }

    //read file values into 2D matrix created usign createDataMatrix function
    for (int i = 0; i < numElement; i++)
    {
        for (int k = 0; k < numSample; k++)
        {
            infile >> RFData[i][k];
        }
    };

    //close the file
    infile.close();

    return 0;

    // You can use the getline() command to extract the data lines from the txt files
}

// Create an array containing the depth location (in z-direction) for each pixel on the scanline
float *genScanlineLocation(int &numPixel)
{
    //initialize imaging depth variable
    float iDepth = 0;
    
    //ask user for desired imaging depth
    cout << "Please input the desired imaging depth: " << endl;
    cin >> iDepth;

    //ask user for desired pixel depth
    cout << "Please input the desired number of pixels: " << endl;
    cin >> numPixel;

    //create array scanlineLocation
    float *scanlineLocation = new float[numPixel];

    //allocate values to scanline location array
    for (int i = 0; i < numPixel; i++)
    {
        //ensure first value is 0
        if (i == 0)
        {
            scanlineLocation[i] = 0;
        }
        //ensure last value is imaging depth
        else if (i == numPixel - 1)
        {
            scanlineLocation[i] = iDepth;
        }
        //ensure values are evenly spaced btwn first and last value
        else 
        {
            scanlineLocation[i] = (iDepth/(numPixel-1))*i;
        }

        //print out array to ensure the correct values have been stored
        cout << scanlineLocation[i] << endl;
    }

    return scanlineLocation;
}

float *genElementLocation(int numElement, float PITCH)
{
    //create array eleLocation with numElement (128) elements
    float *eleLocation = new float[numElement];

    //transform int numElement to a float to ensure all calculations performed return floats
    float N = numElement;

    //calculate and store values into eleLocation array
    for (int n = 0; n < numElement; n++)
    {
        eleLocation[n] = (n - ((N - 1)/2))*PITCH;
    }
    
    return eleLocation;
}

float *createScanline(int numPixel)
{
    //create 1D scanline array
    float *scanline = new float[numPixel];

    return scanline;
}

// Beamform the A-mode scanline
void beamform(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel, float FS, float SoS)
{
    //create 1D array to store tForward values
    float *tForward = new float[numPixel];

    //calculate & store tForward values
    for (int i = 0; i < numPixel; i++)
    {
        tForward[i] = scanlinePosition[i]/SoS;
    }

    //create 2D array to store tBackward values
    float **tBackward = new float *[numPixel];

    for (int i = 0; i < numPixel; i++)
    {
        tBackward[i] = new float[numElement];
    }

    //calculate & store tBackward values
    for (int i = 0; i < numPixel; i++)
    {
        for (int k = 0; k < numElement; k++)
        {
            float scan2  = scanlinePosition[i] * scanlinePosition[i];
            float ele2 = elementPosition[k] * elementPosition[k];

            tBackward[i][k] = (sqrt(scan2 + ele2))/SoS;
        }
    }

    //create 2D array to store tTotal values
    float **tTotal = new float *[numPixel];

    for (int i = 0; i < numPixel; i++)
    {
        tTotal[i] = new float[numElement];
    }

    //calculate & store tTotal values
    for (int i = 0; i < numPixel; i++)
    {
        for (int k = 0; k < numElement; k++)
        {
            tTotal[i][k] = tForward[i] + tBackward[i][k];
        }
    }

    //create 2D array to store s values
    int **s = new int *[numPixel];

    for (int i = 0; i < numPixel; i++)
    {
        s[i] = new int[numElement];
    }

    //calculate & store s values
    for (int i = 0; i < numPixel; i++)
    {
        for (int k = 0; k < numElement; k++)
        {
            s[i][k] = floor(tTotal[i][k]*FS);
        }
    }

    //create 1D array fpr PReal & PImag
    float *PReal = new float[numPixel];

    float *PImag = new float[numPixel];

    //calculate & store PReal & PImag
    for (int i = 0; i < numPixel; i++)
    {
        for (int k = 0; k < numElement; k++)
        {
            //ensure PReal & PImag are initialized correctly
            if (i == 0)
            {
                PReal[i] = realRFData[k][s[i][k]];
                PImag[i] = imagRFData[k][s[i][k]];
            }
            else 
            {
                PReal[i] += realRFData[k][s[i][k]];
                PImag[i] += imagRFData[k][s[i][k]];
            }
        }
    }

    //create 1D array to store mag values
    float *mag = new float[numPixel];

    //calculate & store mag values
    for (int i = 0; i < numPixel; i++)
    {
        mag[i] = sqrt((PReal[i]*PReal[i]) + (PImag[i]*PImag[i]));
        //store mag values to scanline array
        scanline[i] = mag[i];
    }

    
}

// Write the scanline to a csv file
int outputScanline(const char *fileName, float *scanlinePosition, float *scanline, int numPixel)
{
    //create new output csv file
    ofstream outfile(fileName);

    //end program if file could not be created
    if (outfile.fail())
    {
        return -1;
    }

    //output values to csv file in correct order
    for (int i = 0; i < numPixel; i++)
    {
        outfile << scanlinePosition[i] << "," << scanline[i] << endl;
    }
    
    return 0;
}

// Destroy all the allocated memory
void destroyAllArrays(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel)
{
   delete[] scanline;
   delete[] scanlinePosition;
   delete[] elementPosition;

   for (int i = 0; i < numElement; i++)
   {
        delete[] realRFData[i];
        delete[] imagRFData[i];
   }

   delete[] realRFData;
   delete[] imagRFData;
}
