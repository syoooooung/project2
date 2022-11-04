#pragma once
#include "FPNode.h"
#include <list>
#include <iostream>
using namespace std;

class HeaderTable
{
private:
	list<pair<int, string> > indexTable;
	map<string, FPNode*> dataTable;
	list<pair<int, string>>::iterator iter;
public:
	HeaderTable() { }
	~HeaderTable();
	void insertTable(char* item, int frequency);
	void make_dataTable(string item);
	list<pair<int, string>> getindexTable() { return indexTable; }
	map<string, FPNode*> getdataTable() { return dataTable; }
	FPNode* getNode(string item) { return dataTable.find(item)->second; }
	void descendingIndexTable() { indexTable.sort(greater<pair<int, string>>()); }
	void ascendingIndexTable() { indexTable.sort(); }
	int find_frequency(string item);
	string get_index_item_name(int num);
};