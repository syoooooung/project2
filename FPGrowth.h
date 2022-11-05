#pragma once
#include "HeaderTable.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <set>

class FPGrowth
{
private:
	int threshold;
	FPNode* fpTree;
	HeaderTable* table;
	map<set<string>, int> frequenctPatterns;
	ofstream* fout;
	ofstream flog;
	list<pair<int, string>>::iterator iter;
public:
	FPGrowth(ofstream *fout, int threshold) {
		this->threshold = threshold;
		flog.open("result.txt", ios::app);
		flog.setf(ios::fixed);
		fpTree = new FPNode;  table = new HeaderTable;
		this->fout = fout;
	}
	~FPGrowth();
	void createTable(char* item, int frequency) {table->insertTable(item, frequency); }
	void create_dataTable(string item){ table->make_dataTable(item);}
	void createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency);
	void connectNode(HeaderTable* table, string item, FPNode* node);
	void sort_descending_index(){table->descendingIndexTable();}
	
	void frequenctPatternSetting() {
		table->ascendingIndexTable();
		frequenctPatterns = getFrequentPatterns(table, fpTree);
	}

	map<set<string>, int> getFrequentPatterns(HeaderTable* pTable, FPNode* pTree);
	void powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* flag, int depth);
	bool contains_single_path(FPNode* pNode);
	int item_frequency(string item) {return table->find_frequency(item);}
	FPNode* getTree() { return fpTree; }
	HeaderTable* getHeaderTable() { return table; }
	string get_index_item(int how_num){ return table->get_index_item_name(how_num);}
	int get_threshold(){return threshold;}

	bool printList();
	bool printTree();
	void saveFrequentPatterns();

	};


