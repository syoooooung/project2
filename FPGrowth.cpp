#include "FPGrowth.h"


FPGrowth::~FPGrowth() {

}

void FPGrowth::createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency) {
	
	list<string> smaller_treshold;
	int set_continue = 0;
	/*
	iter=table->getindexTable().begin();
	while(1){
		if(iter->first < threshold){
			smaller_treshold.push_back(iter->second);
		}
		if(iter == table->getindexTable().end()) break;
		iter++;
	}*/
	list<pair<int, string>> temp_index = table->getindexTable();
	for (iter = temp_index.begin(); iter != temp_index.end(); iter++) {
		if (iter->first < threshold) {
			smaller_treshold.push_back(iter->second);
		}
	}
	FPNode*currnode = root;
	list<string>::iterator iter2;
	for(iter2=item_array.begin(); iter2!=item_array.end();iter2++){
		if(*iter2=="end"){
				//넘어가기 ㅇ추가
			currnode=root;
			continue;
		}
		for(list<string>::iterator iter3=smaller_treshold.begin(); iter3 != smaller_treshold.end(); iter3++){
			if(*iter2 == *iter3){
				//넘어가기 추가
				set_continue=1;
				break;
			}
		}
		if(set_continue){
			set_continue=0;
			continue;
		}
		FPNode* check_children = currnode->getChildrenNode(*iter2);
		if(check_children == NULL){
			FPNode* newnode = new FPNode;
			//table->getdataTable()
			newnode->setParent(currnode);
			newnode->updateFrequency(1);
			currnode->pushchildren(*iter2, newnode);
			connectNode(table,*iter2,newnode);
		}
		else{
			currnode->getChildrenNode(*iter2)->updateFrequency(1);
		}
		//children.insert(make_pair(*iter2,newnode))
		currnode=currnode->getChildrenNode(*iter2);

	}
	return;
}

void FPGrowth::connectNode(HeaderTable* table, string item, FPNode* node) {
	FPNode* currnode = (table->getdataTable().find(item))->second;
	while(1){
		if(currnode->getNext()==NULL){currnode->setNext(node); break;}
		currnode=currnode->getNext();
	}
	return;
}

bool FPGrowth::contains_single_path(FPNode* pNode) {
	if (pNode->getChildren().size() == 0) return true;
	else if (pNode->getChildren().size() > 1) return false;
	return contains_single_path(pNode->getChildren().begin()->second);
}

map<set<string>, int> FPGrowth::getFrequentPatterns(HeaderTable* pTable, FPNode* pTree) {

	return {};
}



void FPGrowth::powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* ptr, int depth) {
	if (data.size() == depth) {
		set<string> set; set.insert(item);
		for (int i = 0; i < data.size(); i++) { if (ptr[i] == 1) set.insert(data[i]); }
		FrequentPattern->insert(make_pair(set, frequency)); return;
	}
	ptr[depth] = 1;
	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
	ptr[depth] = 0;
	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
}

bool FPGrowth::printList() {
	list<pair<int,string>> index_table_list = table->getindexTable();
	for(iter= index_table_list.begin();iter != index_table_list.end(); iter++){
		*fout<<iter->second<<"\t"<<iter->first<<endl;
	}
	return true;
}
bool FPGrowth::printTree() {
	return true;
}
void FPGrowth::saveFrequentPatterns(){

}
