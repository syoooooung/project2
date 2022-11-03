#include "HeaderTable.h"

HeaderTable::~HeaderTable() {

}


void HeaderTable::insertTable(char* item, int frequency) {
	string str_item(item);
	if(frequency==0){
		frequency++;
		indexTable.push_back(make_pair(frequency, str_item));
	}
	else{
		for(iter = indexTable.begin(); iter != indexTable.end(); iter++){
			if(iter->second==item){
				iter->first++;
			}
		}
	}
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
