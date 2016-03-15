#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "MyBMP.h"
//Maybe because I'm not using Dev-cpp so I have to include it here
#include "MyBMP.cpp"

using namespace std;
void compare(string srcName, string tarName){
    BMP src, tar;
    src.ReadFromFile(srcName.c_str());
    tar.ReadFromFile(tarName.c_str());
    int sw = src.TellWidth(),
        sh = src.TellHeight(),
        tw = tar.TellWidth(),
        th = tar.TellHeight();

    double mse[3]={.0,.0,.0};
    for(int i=0; i<sh; i++)
    {
        for(int j=0; j<sw; j++)
        {
            RGBApixel p1 = src.GetPixel(i, j);
            RGBApixel p2 = tar.GetPixel(i, j);
            // cout << pow(p1.Red - p2.Red, 2) / i*j << endl;
            mse[0] += pow(p1.Red - p2.Red, 2)/(sh*sw);
            mse[1] += pow(p1.Green - p2.Green, 2)/(sh*sw);
            mse[2] += pow(p1.Blue - p2.Blue, 2)/(sh*sw);
        }
    }
    double psnr[3];
    psnr[0] = 10*log10(255*255/mse[0]);
    psnr[1] = 10*log10(255*255/mse[1]);
    psnr[2] = 10*log10(255*255/mse[2]);

    char mbuf[100], pbuf[100];
    sprintf(mbuf,"MSE.rgb:  %.3f,\t%.3f,\t%.3f\n",mse[0],mse[1],mse[2]);
    sprintf(pbuf,"PSNR.rgb: %.3f,\t%.3f,\t%.3f\n",psnr[0],psnr[1],psnr[2]);
    cout << mbuf << pbuf;

    // ofstream ofs("compare_log.txt", ofstream::app);
    // ofs << srcName << " vs " << tarName << '\n' << mbuf << pbuf << endl;
}

int main(int argc, char** argv)
{
    string src,tar;
    cout << "input file name: ";
    while(cin >> src >> tar){
        compare(src,tar);
        cout << "input file name: ";
    }
	return 0;
}
