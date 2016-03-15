#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "MyBMP.h"
#include "MyBMP.cpp"

using namespace std;

int main(int argc, char** argv)
{
    ifstream ifs("config.txt");
	string line;
	
    while(line!="!REVERSE")
        if(!(ifs>>line)){
            cout << "Oops, specify file name in config plz.";
            return 0;
        }
    string srcName;
    double sm[3], sd[3], tm[3], td[3];
    ifs >> srcName;
    ifs >> sm[0] >> sm[1] >> sm[2] >> sd[0] >> sd[1] >> sd[2] >> tm[0] >> tm[1] >> tm[2] >> td[0] >> td[1] >> td[2];

	BMP src, output;
    src.ReadFromFile(srcName.c_str());
    // tar.ReadFromFile(tarName.c_str());
    int sw = src.TellWidth(),
        sh = src.TellHeight();

    output.SetSize(sw ,sh);
    output.SetBitDepth(24);

    double mse[3]={.0,.0,.0};
    for(int i=0; i<sh; i++)
    {
        for(int j=0; j<sw; j++)
        {
            RGBApixel p1 = src.GetPixel(i, j);
            RGBApixel pix;
            double d[3];
            d[0] = p1.Red / 255.0;
            d[1] = p1.Green / 255.0;
            d[2] = p1.Blue / 255.0;
            int r,g,b;
            r = ceil( ( (d[0]-tm[0]) *sd[0]/td[0] +sm[0]) * 255);
            pix.Red     = min( 255, max(0,r) );
            g = ceil( ( (d[1]-tm[1]) *sd[1]/td[1] +sm[1]) * 255);
            pix.Green   = min( 255, max(1,g) );
            b = ceil( ( (d[2]-tm[2]) *sd[2]/td[2] +sm[2]) * 255);
            pix.Blue    = min( 255, max(2,b) );
            output.SetPixel(i, j, pix);
        }
    }

    char buf[100];
    sprintf(buf,"reverse_%s",srcName.c_str());
    output.WriteToFile(buf);
	return 0;
}
