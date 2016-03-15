#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <string>
#include "MyBMP.cpp"

using namespace std;

void stego(string srcName, const int k)
{
    const int k2 = pow(2,k);
    BMP src, Output;
    src.ReadFromFile(srcName.c_str());
    int sw = src.TellWidth(),
            sh = src.TellHeight();
    Output.SetSize(sw ,sh);
    Output.SetBitDepth(24);

    double mse_lsb = 0;
    double mse_opap = 0;
    for(int i=0; i<sh; i++)
    {
        for(int j=0; j<sw; j++)
        {
            RGBApixel pix = src.GetPixel(i, j);
            int p0 = pix.Red;
            int s = rand() % k2;
            assert(s>=0 && s<k2);

            int p[3];
            p[1] = p0 - (p0 % k2) + s;
            p[0] = p[1] + k2;
            p[2] = p[1] - k2;

            int e[3];
            for(int ix=0; ix<3; ++ix)
                e[ix] = abs(p0 - p[ix]);

            int minIx = 0;
            for(int ix=0; ix<3; ++ix)
                if(e[ix]<e[minIx]) minIx = ix;

            pix.Red = pix.Green = pix.Blue = p[minIx];
			mse_lsb += pow(p0 - p[1], 2)/(sh*sw);
			mse_opap += pow(p0 - p[minIx], 2)/(sh*sw);
//			assert(pow(p0 - pix.Red, 2)<=pow(p0 - p[1], 2));
			assert( abs(p0 - p[minIx]) <= abs(p0 - p[1] ));
            Output.SetPixel(i, j, pix);
        }
    }

    assert(mse_opap <= mse_lsb);
    double psnr_opap = 10*log10(255*255/mse_opap);
    double psnr_lsb  = 10*log10(255*255/mse_lsb);
    char buf[50], buf_lsb[100], buf_opap[100];
    sprintf(buf_lsb ,"LSB  MSE = %.4f\t PSNR = %.4f",mse_lsb ,psnr_lsb );
    sprintf(buf_opap,"OPAP MSE = %.4f\t PSNR = %.4f",mse_opap,psnr_opap);
    sprintf(buf,"%s_k%d.bmp",
            srcName.substr(0, srcName.length() - 4).c_str(),k);
    cout << buf_lsb << endl << buf_opap << endl << endl;
//    Output.WriteToFile(buf);
}

int main(int argc, char** argv)
{

    string fileName;
    int k, seed;
    cout << "Input file name, k, random seed" << endl;
    while(cin >> fileName >> k >> seed){
	    srand(seed);
        stego(fileName,k);
        cout << "Input file name and k" << endl;
    }
	return 0;
}
