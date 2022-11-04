#include "HeaderTable.h"

HeaderTable::~HeaderTable() {

}


void HeaderTable::insertTable(char* item, int frequency) {
	string str_item(item);
	if(frequency==0){
		frequency++;
		indexTable.push_back(make_pair(frequency, str_item));
		return;
	}
	else{
		for(iter = indexTable.begin(); iter != indexTable.end(); iter++){
			if(iter->second==item){
				iter->first++;
				return;
			}
		}
	}
}
void HeaderTable::make_dataTable(string item){
	FPNode* newnode= new FPNode;
	dataTable.insert(make_pair(item,newnode));
	return;
}
string HeaderTable::get_index_item_name(int num){
	iter=indexTable.begin();
	string temp="";
	for(int i=0; i<= num;i++){
		if(i==num){return iter->second;}
		iter++;
		if(iter == indexTable.end()) return temp;
	}
	return temp;
}
int HeaderTable::find_frequency(string item){
	int frequency = 0;
	for(iter = indexTable.begin(); iter != indexTable.end(); iter++){
		if(iter->second == item){
			frequency = iter->first;
			return frequency;
		}
	}
	return 0; //no item
}
