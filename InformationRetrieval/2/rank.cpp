#include <ctime>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

map<string, map<int, double> > parseNews(string file, map<int,int>& ret2){
	// initialize
	ifstream fs(file);
	unsigned int lines = 0;
	char buf[1024];
	map<string,int> wc;
	map<string, map<int,int> > ii;	// lookup words for list id & occurence
	map<string, map<int,double> > ret;

	// read file by lines
	while(fs.getline(buf,1024)){
		++lines;
		char* p = strtok(buf," ,");
		while(p != NULL){
			// printf("%s\n",p);
			string word(p);
			++wc[word];
			++ii[word][lines/8];
			++ret2[lines/8];
			p = strtok(NULL," ,");	// ....
		}
	}
	printf("%d lines read from news.\nBuilding rank index\n",lines);
	for (auto &elem : ii){
		// double N = lines/8;
		double N = lines;
		for(auto &ee: elem.second){
			string word = elem.first;
			int doc = ee.first, freq = ee.second;
			double tf = 1.0 + log10(freq)
				,idf = log10(N/wc[word]);
			ret[word][doc] = tf*idf;
		}
	}
	return ret;
}

string timing(){
	static clock_t tick;
	char buf[256];
	sprintf(buf, "%d ms passed.", (clock() - tick) * 1000 / CLOCKS_PER_SEC);
	tick = clock();
	return string(buf);
}

void search(string query, map<string, map<int, double> >& ri, map<int,int>& dl){
	double weight;
	string word;
	timing();
	stringstream tokens(query);
	map<int, double> scoreKeep;

	// extract query term, summing scores for each doc
	while(tokens >> word >> weight){
		for(auto &elem : ri[word])
			scoreKeep[elem.first] += elem.second * weight;
	}

	if(scoreKeep.size()==0){
		printf("No result found. (Did you specify query weight?)\n> ");
		return;
	}
	// get k hightest docs
	multimap<double, int> inverse;
	for(auto &elem : scoreKeep)
		inverse.insert(make_pair(elem.second/dl[elem.first],elem.first));
	
	// printf("size of keep, inverse = %4d, %4d\n", scoreKeep.size(), inverse.size());
	cout << timing() << endl;
	int k=0;
	printf("Rank\tID\tScore\n");
	for(auto it=inverse.crbegin(); k<10 && it!=inverse.crend(); k++, it++)
		printf("%d\t%d\t%f\n", k+1, it->second, it->first);
	printf("> ");
}

int main(){
	cout << "Parsing news...\n";
	timing();
	map<int,int> doc_len;
	auto rankIndex = parseNews("en09062011.news", doc_len);
	// auto rankIndex = parseNews("debug.news");
	cout << timing() << "\n> ";
	string query;
	while(getline(cin,query)){
		search(query, rankIndex, doc_len);
	}
	return 0;
}