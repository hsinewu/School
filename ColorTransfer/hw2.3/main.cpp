#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cassert>
#include "MyBMP.cpp"

#define OVERFLOW_UP (256)
using std::string;
using std::cin;

BMP img;
int sw,sh;

void initBMP(string srcName){
    img.ReadFromFile(srcName.c_str());
    sw = img.TellWidth();
    sh = img.TellHeight();
}

int getPixel(int v){
	assert(v<=sw*sh);
	return img.GetPixel(v/sh,v%sh).Red;
}

void setPixel(int v, int c){
	assert(v<sw*sh);
	RGBApixel pix = {c,c,c};
	img.SetPixel(v/sh,v%sh,pix);
}
//					n		k	h*v/k/n
void emd(int s0, int s1, int s2){
	int base = 2*s0 + 1,
		c1 = log2(base),
		// c2 = s1 * c1,	// lose precise
		c2 = s1 * log2(base),	// l
		c3 = s2 * c2,	// mxc
		l2 = s2,
		l1 = l2 * s1,
		l0 = l1 * s0;
	printf("n, k, mxc = %d, %d, %d\n",s0,s1,c3);
	int d2[s2],		// d2 holds orginal random number
		d1[s2*s1];	// a d2 split to s1 d1
	for(int i=0; i<s2; i++)
		d2[i] = rand() % (1 << c2);		// to store real data, change this
	
	for(int i=0, symbol; i<l2; i++){
		symbol = d2[i];
		for(int j=0; j<s1; j++){
			d1[s1*i+j] = symbol % base;
			symbol /= base;
		}
	}
	
	for(int i=0,f,pn,v; i<s2*s1; i++){	// for each digits
		f = 0;
		for(int j=0; j<s0; j++)	// for each pixels
			f += getPixel(i*s0+j) * (j+1);
		f = ((d1[i]-f) % base + base) % base;
		if(f==0)
			;
		else if(f<=s0){
			pn = s0*i+f-1;
			v = getPixel(pn); 
			if(v == OVERFLOW_UP)
				v -= base;
			setPixel(pn,v+1);
		}else{
			pn = s0*i+base-f-1;
			v = getPixel(pn);
			if(v == 0)
				v += base;
			setPixel(pn,v-1);
		}
	}
	printf("Encryption done.\n");

	int r2[s2];
	for(int i=0,r1; i<s2; i++){	// for each symbol
		r2[i] = 0;
		for(int j=0,f; j<s1; j++){	// for each digit
			f = 0;
			for(int k=0; k<s0; k++){	// for each pixel
				f += getPixel((i*s1+j)*s0+k) * (k+1);
				f %= base;
			}
			r1 = f;
			int b = pow(base,j);
			r2[i] += r1*b;
		}
		// test if decrypted message is equal to orginal message
		assert(r2[i]==d2[i]);	// error = decryption failed
	}
	printf("Decryption done, pass assertion.\n");
	img.WriteToFile("OUTPUT.bmp");
}

int affordableK(const int h, const int v, const int n){
	int goodk=0, mxc=0, k=1, l,Uc = h*v/n;
	while((l=k*log2(2*n+1))<=64){
		int c = Uc/k*l;
		if(c>mxc){
			mxc = c;
			goodk = k;
		}
		k++;
	}
	return goodk;
}

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
    printf("%s%s",mbuf,pbuf);
}

int main(){
	string fileName;
	int n,k;

	printf("Input file name:\n");
	cin >> fileName;
	printf("Input n:\n");
	cin >> n;

	initBMP(fileName);
    printf("sw, sh = %d, %d\n",sw,sh);
	k = affordableK(sw,sh,n);
	emd(n, k, sw*sh/k/n);
	compare(fileName,"OUTPUT.bmp");
	return 0;
}