#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cassert>
#include "MyBMP.cpp"

#define OVERFLOW_UP (256)
using std::string;

BMP img;
int sw,sh;

void initBMP(string srcName){
    img.ReadFromFile(srcName.c_str());
    sw = img.TellWidth();
    sh = img.TellHeight();

    printf("sw, sh = %d, %d\n",sw,sh);
    char buf[50];
    sprintf(buf,"emd_%s.bmp",
            srcName.substr(0, srcName.length() - 4).c_str());
//    Output.WriteToFile(buf);
}

int getPixel(int v){
	// printf("access %d\n",v);
	assert(v<=sw*sh);
	return img.GetPixel(v/sh,v%sh).Red;
}

void setPixel(int v, int c){
	assert(v<sw*sh);
	RGBApixel pix = {c,c,c};
	// pix.Red     = 	pix.Green   = 	pix.Blue    = 
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
	int d2[s2], d1[s2*s1];	// a d2 split to s1 d1
	for(int i=0; i<s2; i++){
		d2[i] = rand() % (1 << c2);		// to store real data, change this
		// printf("%d\n",d2[i]);
		// getchar();
		assert(d2[i]<pow(2,c2));
	}
	int _sequential;
	_sequential = 0;
	for(int i=0, symbol; i<l2; i++){
		symbol = d2[i];
		if(i<3) printf("d2[%d]=%d\n",i,d2[i]);
		for(int j=0; j<s1; j++){
			assert(_sequential==s1*i+j);
			d1[s1*i+j] = symbol % base;
			// if(_sequential<3*s1) printf("d1[%d]=%d\n",j,d1[s1*i+j]);
			symbol /= base;
			++_sequential;
		}
	}
	
	_sequential = 0;
	for(int i=0,d,f,pn,v; i<s2*s1; i++){	// for each digits
		d = d1[i];
		f = 0;

		for(int j=0; j<s0; j++){	// for each pixels
			// printf("%d %d %d %d: %d\n",i,s2,s1,j,i*s2*s1+j);
			assert(_sequential == i*s0+j);
			++_sequential;
			f += getPixel(i*s0+j) * (j+1);
			// if(i<3)printf("getPixel(%d)=%d\n",i*s0+j,getPixel(i*s0+j));
		}
		// if(i<3) printf("f1=%d (%d)\n",f,f%base);
		f = ((d-f) % base + base) % base;
		// if(i<3) printf("f2=%d\n",f);
		assert(f>=0 && f<base);
		if(f==0){
			// if(i<3) printf("plus 0\n");
		}
		else if(f<=s0){
			pn = s0*i+f-1;
			// if(i<3) printf("plus 1 at %d\n",pn);
			v = getPixel(pn); 
			if(v == OVERFLOW_UP)
				v -= base;
			setPixel(pn,v+1);
			// these assertions only works when n = 3?
			assert(f!= 1 || getPixel( s0*i ) - v ==1);
			assert(f!= 2 || getPixel(s0*i+1) - v ==1);
			assert(f!= 3 || getPixel(s0*i+2) - v ==1);
		}else{
			pn = s0*i+base-f-1;
			// if(i<3) printf("minus 1 at %d\n",pn);
			v = getPixel(pn);
			if(v == 0)
				v += base;
			setPixel(pn,v-1);
			// these assertions only works when n = 3?
			assert(f!=6 || getPixel( s0*i ) - v==-1);
			assert(f!=5 || getPixel(s0*i+1) - v==-1);
			assert(f!=4 || getPixel(s0*i+2) - v==-1);
		}
	}
	printf("Encryption done.\n");

	int r2[s2];
	int last;
	_sequential = 0;
	for(int i=0,r1; i<s2; i++){	// for each symbol
		r2[i] = 0;
		for(int j=0,f; j<s1; j++){	// for each digit
			f = 0;
			for(int k=0; k<s0; k++){	// for each pixel
				assert(_sequential == (i*s1+j)*s0+k);
				// if(i<3) printf("getPixel(%d)=%d\n",_sequential,getPixel(_sequential));
				++_sequential;
				last = (i*s1+j)*s0+k;
				f += getPixel((i*s1+j)*s0+k) * (k+1);
				f %= base;
			}
			r1 = f;
			// + r1, +base*r1, +base^2 *r1
			int b = pow(base,j);
			// printf("r1=%d inc=%d ",r1,r1*b);
			r2[i] += r1*b;
		}
		// printf(":actual=%d expect=%d\n",r2[i],d2[i]);
		// getchar();
		assert(r2[i]==d2[i]);	// If got error, decryption fail on this point.
	}
	// printf("%d\n",last);
	printf("Decryption done, pass assertion.\n");
}

void testBMP(){
	initBMP("Lena.bmp");
	int bound = sw*sh;
	for(int i=0,ix,v; i<1000; i++){
		ix = rand() % bound;
		v = rand() % 256;
		setPixel(ix,v);
		assert(getPixel(ix) == v);
	}
	printf("Pass testBMP.\n");
}

int main(){
	testBMP();
	initBMP("Lena.bmp");
	int n=3,k=10;
	emd(n, k, sw*sh/k/n);
	return 0;
}