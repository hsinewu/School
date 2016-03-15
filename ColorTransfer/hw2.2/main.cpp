#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

int printTable(const int h, const int v, const int n){
	cout << "\tn\tK\tTHC\tOPT\tL\tmxc\tCL" << endl;
	int bestk = 0,goodk=0;
	int mxc = 0;
	int Uc = h*v/n;
	double thc = log2(2*n+1);
	double opt = thc * Uc;
	for(int k=1,maxk=h*v/n; k<=maxk; ++k){
		int l = k*thc;
		int c = floor(Uc/k)*l;
		// cout << "\t\t" << k << "\t\t\t" << l << "\t" << c << endl;
		if(c>mxc){
			mxc = c;
			bestk = k;
			if(l<=64)
				goodk = k;
		}
	}
	cout << "\t" << n << "\t" << bestk << "\t" << thc << "\t" << opt << "\t" << floor(bestk*thc) << "\t" << mxc << "\t" << 1-mxc/opt << endl;
	cout << "optimal k " << bestk << ", affordable k " << goodk << endl;
	return bestk;
}

int main(){
	int h=512,v=512,n;
	while(cin){
		cout << "\nInput h,v,n" << endl;
		cin >> h >> v >> n;
		printTable(h,v,n);
	}
	return 0;
}