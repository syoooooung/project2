#pragma once
#include "BpTree.h"
#include<iostream>
#include<fstream>
#include "FPGrowth.h"
using namespace std;
enum Result{ //Enum fo errorcode and successcode
    Fail = 0,
    Success=1,
    Load = 100,
    Btload=200,
    Printitemlist=300,
    Printfptree=400,
    Printbptree=500,
    Printconfidence=600,
    Printrange=700,
    Save=800
};
class Manager
{
private:
	char* cmd;
	FPGrowth* fpgrowth;
	BpTree* bptree;
public:
	Manager(int threshold, int bpOrder)	//constructor
	{
		/* You must fill here */
		//log.txt 열기
		flog.open("log.txt",ios::app);
		fpgrowth = new FPGrowth(&flog,threshold);
		bptree= new BpTree(&flog, 3);
	}


	~Manager()//destructor
	{
		/* You must fill here */
		flog.close();
	}
	//stream 하나
	ifstream fin;
	ofstream flog;
	

	void run(const char* command);
	bool LOAD();
	bool BTLOAD();
	
	bool PRINT_ITEMLIST();
	bool PRINT_FPTREE();

	bool PRINT_RANGE(char* item, int start, int end);
	bool PRINT_CONFIDENCE(char* item, double rate);
	bool PRINT_BPTREE(char* item, int min_frequency);
	bool SAVE();


	void printErrorCode(int n);
	void printSuccessCode();
	

};

