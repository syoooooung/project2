#include "FPGrowth.h"


FPGrowth::~FPGrowth() {

}

void FPGrowth::createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency) {
	/*여기서부터
	list<string> smaller_treshold;
	int set_continue = 0;
	// 난 여기딸랑
	//iter=table->getindexTable().begin();
	//while(1){
		//if(iter->first < threshold){
			smaller_treshold.push_back(iter->second);
		}
		if(iter == table->getindexTable().end()) break;
		iter++;
	//}여기 딸랑
	list<pair<int, string>> temp_index = table->getindexTable();
	for (iter = temp_index.begin(); iter != temp_index.end(); iter++) {
		if (iter->first < threshold) {
			smaller_treshold.push_back(iter->second);
		}
	}여기까지*/
	FPNode*currnode = root;
	list<string>::iterator iter2;
	for(iter2=item_array.begin(); iter2!=item_array.end();iter2++){
		if(*iter2=="end"){
				//넘어가기 ㅇ추가
			currnode=root;
			continue;
		}/*
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
		}*/
		FPNode* check_children = currnode->getChildrenNode(*iter2);
		if(check_children == NULL){
			FPNode* newnode = new FPNode;
			//table->getdataTable()
			newnode->setParent(currnode);
			newnode->updateFrequency(1);
			string temp_str = *iter2;
			newnode->setItem(temp_str);
			//newnode->setItem((char*)(*iter2).c_str());
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

void FPGrowth::connectNode(HeaderTable* table, string item, FPNode* node) { /*
	FPNode* currnode = (table->getdataTable().find(item))->second;
	while(1){
		if(currnode->getNext()==NULL){currnode->setNext(node); break;}
		currnode=currnode->getNext();
	}*/
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
	table->ascendingIndexTable();
	list<pair<int, string>> temp_index = table->getindexTable();
	
	for (iter = temp_index.begin(); iter != temp_index.end(); iter++) {
		if (iter->first < threshold) {
			continue;
		}
		map<string,FPNode*> Print_dataTable =table->getdataTable();
		map<string,FPNode*>::iterator iter4 =Print_dataTable.find(iter->second);
		*fout<<"{"<<iter4->first<<", "<<table->find_frequency(iter4->first)<<"}"<<endl;
		FPNode* currnode=iter4->second;
		FPNode* savenode=currnode;
		while(1){
			currnode=savenode->getNext();
			savenode=savenode->getNext();
			if(currnode==NULL){break;}
			while(currnode->getParent() != NULL){
				string for_item_temp=currnode->getitem();
				*fout<<"("<<for_item_temp<<", "<<currnode->getFrequency()<<") ";
				currnode=currnode->getParent();
			}
			*fout<<endl;
		}
	}
	return true;
}
void FPGrowth::saveFrequentPatterns(){ /*

//	map<string, FPNode*> remove_small = table->
	table->ascendingIndexTable();
	list<pair<int, string>> temp_index1 = table->getindexTable();
	
	list<pair<int, string>> temp_index;
	for(iter = temp_index1.begin(); iter != temp_index1.end(); iter++){
		if(iter->first< threshold){ }
		else{ temp_index.push_back(make_pair(iter->first, iter->second));}
	}
	
	map<string,FPNode*> Print_dataTable =table->getdataTable();
	for (iter = temp_index.begin(); iter != temp_index.end(); iter++) { //indextable 업데이트를 시키기위함이다!!
		//temp_index = inti_index;
		list<pair<int,string>> inti_index;
		string ag_tp= iter->second;
		map<string,FPNode*>::iterator iter4 =Print_dataTable.find(ag_tp); //찾을게
		int save_freq =0;
		FPNode* currnode=iter4->second;
		FPNode* savenode=currnode;
		while(1){
			currnode=savenode->getNext();
			savenode=savenode->getNext();
			if(currnode==NULL){break;}
			
			while(currnode->getParent() != NULL){
				save_freq = currnode->getFrequency();
				for(list<pair<int,string>>::iterator iter_nu=temp_index.begin(); iter_nu != temp_index.end(); iter_nu++){ //마지막 노드 fre에 맞게모두 set해주기
					if(iter_nu->second == currnode->getitem()){ //여기 abort 주의!!!!!!!!!!!!!!!!!!!!!!!!
					//iter_nu->first = save_freq; break;
					string ttt= currnode->getitem();
					inti_index.push_back(make_pair(save_freq,ttt));
					break;
					}
				}
				currnode=currnode->getParent();
			}
			
		}
		//와~ 그 인덱스의 item은 끝났다 모두 초기화 해줄것
		//다른 트리 만드는 함수 호출해줄것 거기서 private에 넣어주기 모두 넣어주면
		string putput= iter4->first;
		cout<<"============================="<<endl;
		once_doit(inti_index,putput);
		
	} */
	return ;
}

void FPGrowth::once_doit(list<pair<int, string>> save_freque1, string currentitem){ /*
	set<string> save_pattern;
    map<string,FPNode*> save_node;
    HeaderTable* save_table = new HeaderTable;

    int get_save_freq=0;
//여기가 각각의 fptree를 만드는 함수인데 바꾸자
//currentitem 단위로 끊으면서 다시 만들어줘야할듯 list 그렇게 만들고
//받아온 indextable에서 같은 item이면 fre 수를 합쳐줘 그다음 treshold 보다 큰것들로만해서 다시 datatable을 만들고
//그 table로 fptree만든다. 그 다음은??
    list<pair<int,string>> save_freque;
    for(list<pair<int, string>>::iterator itera=save_freque1.begin(); itera!= save_freque1.end(); itera++){
    	if(itera->first< threshold){continue;}
    	else{
         string ghost2 = itera->second;
         save_freque.push_back(make_pair(itera->first, ghost2));
         save_table->make_dataTable(ghost2);
      }
   }
	for(list<pair<int,string>>::iterator ddd=save_freque.begin(); ddd!=save_freque.end(); ddd++){
		cout<<ddd->first<<ddd->second<<"\t";
	} cout<<endl;
 */
	return;// push_freqPattern(map<set<string>, int> hihi)
}

void FPGrowth::printPatern(){
	map<set<string>, int>::iterator fre_iter= frequenctPatterns.begin();
	for( ; fre_iter != frequenctPatterns.end(); fre_iter++){
		flog<<fre_iter->second<<"\t";
		*fout<<fre_iter->second<<"\t";
		set<string> temp_set = fre_iter->first;
		for(set<string>::iterator itt = temp_set.begin(); itt != temp_set.end(); itt++){
			flog<<*itt<<"\t";
			*fout<<*itt<<"\t";
		}
		flog<<endl;
		*fout<<endl;
	}
}
