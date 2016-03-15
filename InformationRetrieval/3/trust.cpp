#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <set>
#include <map>
#include <bitset>

#include <typeinfo>
#include <cassert>

using namespace std;

// define type information
#define MAXIMUM_ID 75887	// for Ep.txt
typedef unsigned int Id;
typedef set<Id> SparseList;
typedef bitset<MAXIMUM_ID> LongList;

map<Id, SparseList> trustTable;

// set up trustTable
void readFile(const string& fileName){
	ifstream file(fileName);
	Id from,to;
	while(file >> from >> to){
		assert(from <= MAXIMUM_ID);
		trustTable[from].insert(to);
	}
}

void expand(LongList start, int times){
	assert(times >= 0);
	cout << "start from: ";
	for(int i=0; i<MAXIMUM_ID; i++){
		if(start[i])
			cout << i << ", ";
	}
	cout << "\ntimes: " << times << endl;

	LongList done, updateQueue=start;
	while(times--){
		for(int i=0; i<MAXIMUM_ID; i++){
			if(updateQueue[i]){		// if this bit is just set previous turn
				for(Id id : trustTable[i])
					start.set(id);
			}
		}
		done = done | updateQueue;
		updateQueue = start &~ done;
		if(updateQueue.none())	// saturation test
			break;
	}

	for(int i=0; i<MAXIMUM_ID; i++){
		if(start[i]){
			cout << i << "\t";
		}
	}
}

int main(int argc, char *argv[]){
	// check if enough arguments are supplied
	if(argc < 4){
		cerr << "Require arguments.\nEx: $ main Ep.txt \"777 888\" 0\n";
		return 0;
	}

	// read argv
	readFile(argv[1]);	// set up trustTable
	stringstream tokenizer(argv[2]);
	int times = atoi(argv[3]);

	// init list of trusted people
	LongList startList;
	int i;
	while(tokenizer >> i)
		startList.set(i);

	// people in startList are trusted, expand this list for <times> times
	expand(startList, times);

	return 0;
}