#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "MyBMP.h"
//#include "MyBMP.cpp"	//Comment out this if using project setting

using namespace std;


void muti(double mat[3][3],double vec[3]){
    double t[3];
    for(int i=0; i<3; ++i){
        t[i] =
            mat[i][0]*vec[0] +
            mat[i][1]*vec[1] +
            mat[i][2]*vec[2] ;
    }
    //printf("%f\t%f\t%f\n",t[0],t[1],t[2]);
    vec[0] = t[0];
    vec[1] = t[1];
    vec[2] = t[2];
}

void r2x(double rgb[]){
    static double m[3][3] = {
        {0.5141,    0.3239, 0.1604},
        {0.2651,    0.6702, 0.0641},
        {0.0241,    0.1228, 0.8444}
    };
    muti(m,rgb);
}
void x2l(double xyz[]){
    static double m[3][3] = {
        {0.3897,    0.6890, -0.0787},
        {-0.2298,   1.1834, 0.0464},
        {.0,        .0,     1.0}
    };
    muti(m,xyz);
}
void l2r(double lms[]){
    static double m[3][3] = {
        {4.4679,-3.5873,0.1193},
        {-1.2186,2.3809,-0.1624},
        {0.0497,-0.2439,1.2045}
    };
    muti(m,lms);
}
void l2L(double lms[]){
    //adjust to what when L <= 0 ?
    if(lms[0] <0 || lms[1]<0 || lms[2]<0) cout << "notice: log10\n";
    lms[0] = lms[0]<=0 ? 0 : log10(lms[0]);
    lms[1] = lms[1]<=0 ? 0 : log10(lms[1]);
    lms[2] = lms[2]<=0 ? 0 : log10(lms[2]);
}
void L2l(double LMS[]){
	LMS[0] = pow(10, LMS[0]);
    LMS[1] = pow(10, LMS[1]);
    LMS[2] = pow(10, LMS[2]);
}
//to l alpha beta
void L2lab(double LMS[]){
    static double m1[3][3] = {
        {1/sqrt(3),0,0},
        {0,1/sqrt(6),0},
        {0,0,1/sqrt(2)}
    },  m2[3][3] = {
        {1,1,1},
        {1,1,-2},
        {1,-1,0}
    };
    muti(m2,LMS);    muti(m1,LMS);
}
void lab2L(double lab[]){
    static double m1[3][3] = {
        {1,1,1},
        {1,1,-1},
        {1,-2,0}
    },  m2[3][3] = {
        {sqrt(3)/3,0,0},
        {0,sqrt(6)/6,0},
        {0,0,sqrt(2)/2}
    };
    muti(m2,lab);   muti(m1,lab);
}
void r2lab(double rgb[3]){
    r2x(rgb);    x2l(rgb);    l2L(rgb);    L2lab(rgb);
}
void lab2r(double lab[]){
    lab2L(lab); L2l(lab);   l2r(lab);
}
//pass in lab
#define INFO(mat_lab,w,h,mean,deviation)\
    tot = w*h;\
    sum[0] = sum[1] = sum[2] = 0;\
    for(int i=0; i<h; ++i)\
    for(int j=0; j<w; ++j){\
        sum[0] += mat_lab[i][j][0];\
        sum[1] += mat_lab[i][j][1];\
        sum[2] += mat_lab[i][j][2];\
    }\
    mean[0] = sum[0]/tot;\
    mean[1] = sum[1]/tot;\
    mean[2] = sum[2]/tot;\
    dsum[0] = dsum[1] = dsum[2] = 0;\
    for(int i=0; i<h; ++i)\
    for(int j=0; j<w; ++j){\
        dsum[0] += pow(mat_lab[i][j][0]-mean[0], 2);\
        dsum[1] += pow(mat_lab[i][j][1]-mean[1], 2);\
        dsum[2] += pow(mat_lab[i][j][2]-mean[2], 2);\
    }\
    deviation[0] = sqrt( dsum[0]/tot);\
    deviation[1] = sqrt( dsum[1]/tot);\
    deviation[2] = sqrt( dsum[2]/tot);

void process(string mode, string srcName, string tarName, bool print_statistic){
        BMP src, tar, Output;
        src.ReadFromFile(srcName.c_str());
        tar.ReadFromFile(tarName.c_str());
        int sw = src.TellWidth(),
            sh = src.TellHeight(),
            tw = tar.TellWidth(),
            th = tar.TellHeight();
        Output.SetSize(sw ,sh);
        Output.SetBitDepth(24);

        double msrc[sh][sw][3];

        for(int i=0; i<sh; i++)
        {
            for(int j=0; j<sw; j++)
            {
                RGBApixel NewPixel = src.GetPixel(i, j);
                msrc[i][j][0] = NewPixel.Red / 255.0;
                if(NewPixel.Red     ==0) msrc[i][j][0] = 1e-9;
                msrc[i][j][1] = NewPixel.Green / 255.0;
                if(NewPixel.Green   ==0) msrc[i][j][0] = 1e-9;
                msrc[i][j][2] = NewPixel.Blue / 255.0;
                if(NewPixel.Blue    ==0) msrc[i][j][0] = 1e-9;
                if(mode == "lab")
                    r2lab(msrc[i][j]);
            }
        }

        double mtar[th][tw][3];
        for(int i=0; i<th; i++)
        {
            for(int j=0; j<tw; j++)
            {
                RGBApixel NewPixel = tar.GetPixel(i, j);

                mtar[i][j][0] = NewPixel.Red / 255.0;
                if(mtar[i][j][0] <=0) mtar[i][j][0] = 1e-9;
                mtar[i][j][1] = NewPixel.Green / 255.0;
                if(mtar[i][j][1] <=0) mtar[i][j][1] = 1e-9;
                mtar[i][j][2] = NewPixel.Blue / 255.0;
                if(mtar[i][j][2] <=0) mtar[i][j][2] = 1e-9;
                if(mode == "lab")
                    r2lab(mtar[i][j]);
            }
        }

        //Retrieve statistics
        double smean[3],sdev[3],tmean[3],tdev[3];
        int tot;    double sum[3],dsum[3];  //'local' variabies in info
        INFO(msrc,sw,sh,smean,sdev); //-.-
        INFO(mtar,tw,th,tmean,tdev);
        if (print_statistic){
       	    ofstream ofs("statistics_log.txt", ofstream::app);

        	char buf[100];
		    ofs << "\n" << srcName << endl;
		    ofs << "mean.rgb = ";
			for(int i=0; i<3; ++i ) ofs << setw(10) << smean[i];
			ofs << "\ndevi.rgb = ";
			for(int i=0; i<3; ++i ) ofs << setw(10) << sdev[i];
			ofs << "\n\n" << tarName << endl;
		    ofs << "mean.rgb = ";
			for(int i=0; i<3; ++i ) ofs << setw(10) << tmean[i];
			ofs << "\ndevi.rgb = ";
			for(int i=0; i<3; ++i ) ofs << setw(10) << tdev[i];
			ofs << "\n" << srcName << tarName << " = ";
			ofs << smean[0] << setw(10) << smean[1] << setw(10) << smean[2] << setw(10) << sdev[0] << setw(10) << sdev[1] << setw(10) << sdev[2] << setw(10) << tmean[0] << setw(10) << tmean[1] << setw(10) << tmean[2] << setw(10) << tdev[0] << setw(10) << tdev[1] << setw(10) << tdev[2] << endl;
        }
        
        //Now, we are ready to perform transformation
        for(int i=0; i<th; i++)
        {
            for(int j=0; j<tw; j++)
            {
                //Since matrix use so many memory,
                //I'm not gonna declare a new result matrix
                msrc[i][j][0] =
                    (tdev[0]/sdev[0]) * (msrc[i][j][0]-smean[0]) + tmean[0];
                msrc[i][j][1] =
                    (tdev[1]/sdev[1]) * (msrc[i][j][1]-smean[1]) + tmean[1];
                msrc[i][j][2] =
                    (tdev[2]/sdev[2]) * (msrc[i][j][2]-smean[2]) + tmean[2];

                //Reverse color space
                if(mode == "lab")
                    lab2r(msrc[i][j]);

                RGBApixel pix;
                int r,g,b;
                r = floor(msrc[i][j][0] * 255.0);
                pix.Red     = min( 255, max(0,r) );
                g = floor(msrc[i][j][1] * 255.0);
                pix.Green   = min( 255, max(0,g) );
                b = floor(msrc[i][j][2] * 255.0);
                pix.Blue    = min( 255, max(0,b) );
                Output.SetPixel(i, j, pix);
            }
        }

        Output.WriteToFile(
            (mode + "_" + srcName.substr(0, srcName.length() - 4) + "_" + tarName)
                .c_str() );
}

int main(int argc, char** argv)
{
    ifstream file("config.txt");
    string mode, srcName, tarName;
    file >> srcName >> tarName;
    process("rgb",srcName,tarName, true);
    process("lab",srcName,tarName, false);
//    int p = 128; double pp = 1.0/p; cout<<setprecision(3);
//    double x=-999;
//    for(int i=0; i<=p; i++)
//    for(int j=0; j<=p; j++)
//    for(int k=0; k<=p; k++){
//        //cout << setw(8) << i*pp << setw(8) << j*pp << setw(8) << k*pp << endl;
//        double m[3] = {i*pp, j*pp, k*pp};
//        r2lab(m);
//        if(x<m[0]) x=m[0];
//        //cout << setw(8) << m[0] << setw(8) << m[1] << setw(8) << m[2] << endl;
//    }cout<<x<<endl;
//    return 0;

	return 0;
}
