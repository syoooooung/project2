#include "FPGrowth.h"


FPGrowth::~FPGrowth() {

}

void FPGrowth::createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency) {
	FPNode*currnode = root;
	list<string>::iterator iter2;
	for(iter2=item_array.begin(); iter2!=item_array.end();iter2++){
		if(*iter2=="end"){
				//plus pass
			currnode=root;
			continue;
		}
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
		set<string> set88; set88.insert(item);
      set<string>::iterator origin_s = set88.begin();
		for (int i = 0; i < data.size(); i++) { if (ptr[i] == 1) set88.insert(data[i]); }

      for(map<set<string>, int>::iterator apap = FrequentPattern->begin();apap != FrequentPattern->end(); apap++){
         origin_s = set88.begin();
         for(set<string>::iterator find_i = apap->first.begin(); find_i !=apap->first.end() ; ){
            if(*find_i != *origin_s){ break;}
            find_i++;
            origin_s++;
            if(find_i ==apap->first.end()){
            map<set<string>, int>::iterator remove_mp = apap;
            int tmep_save = apap->second + frequency;
            FrequentPattern->erase(remove_mp);
            FrequentPattern->insert(make_pair(set88, tmep_save)); return;
            }
         }
      
      }
		FrequentPattern->insert(make_pair(set88, frequency)); return;
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

map<set<string>, int> FPGrowth::make_list(map<string, int> t_index, list<pair<int,set<string>>>pre_list){
	map<set<string>, int> new_map;
	
	for(map<string,int>::iterator index_it=t_index.begin(); index_it!=t_index.end(); index_it++){
		if(index_it->second <= threshold){// 인덱스의수가 트레숄드보다 작으면
			//리스트에서 해당 아이템을 삭제해버린다
			for(list<pair<int,set<string>>>::iterator list_it=pre_list.begin(); list_it != pre_list.end(); list_it++){
				if(list_it->second.find(index_it->first)!= list_it->second.end()){ list_it->second.erase(index_it->first);}
			}
		}
	}
	for(list<pair<int,set<string>>>::iterator sav_it = pre_list.begin(); sav_it != pre_list.end(); sav_it++){
		new_map.insert(make_pair(sav_it->second, sav_it->first));
	}
	return new_map;
}

void FPGrowth::saveFrequentPatterns(){ 


   table->ascendingIndexTable();
   list<pair<int, string>> temp_index1 = table->getindexTable();

   list<pair<int, string>> temp_index; //treshold보다 작은 빈도수인 아이템은 index 테이블에서 제거 결과가 뭔가 이상하다면 얠 지워봐
   for (list<pair<int, string>>::iterator iter6 = temp_index1.begin(); iter6 != temp_index1.end(); iter6++) {
      if (iter6->first < threshold) {}
      else { temp_index.push_back(make_pair(iter6->first, iter6->second)); }
   }

   map<string, FPNode*> Print_dataTable = table->getdataTable(); //데이터테이블 불러옴
   for (list<pair<int, string>>::iterator iter = temp_index.begin(); iter != temp_index.end(); iter++) {
      //temp_index = inti_index;
      list<pair<int, set<string>>> inti_index;

      string ag_tp = iter->second;
      map<string, FPNode*>::iterator iter4 = Print_dataTable.find(ag_tp);
      int save_freq = 0;
      FPNode* currnode = iter4->second;

      FPNode* savenode = currnode;
      map<string, int> new_tpindex; //각각의 인덱스 저장할 리스트
      list<int> each_frq; //파워셋에 넘길 애 frq로 ~
      while (1) { //이렇게 크게가 단어 하나당!
         currnode = savenode->getNext();
         savenode = savenode->getNext();
         if (currnode == NULL) { break; }
         set<string> ttt;
         save_freq = currnode->getFrequency();
         each_frq.push_back(save_freq);
         while (currnode->getParent() != NULL) { //루트까지 페스 찍는다 벡터에 넣어주자 inti_index=>vector
            //for(list<pair<int,string>>::iterator iter_nu=temp_index.begin(); iter_nu != temp_index.end(); iter_nu++){ //
               //if(iter_nu->second == currnode->getitem()){ // 얘왜있는거임?
            if (new_tpindex.find(currnode->getitem()) == new_tpindex.end()) { new_tpindex.insert(make_pair(currnode->getitem(), currnode->getFrequency())); } //근데 빈 맵인데 파인드해도되나?
            else {
               int sav_prenum = new_tpindex.find(currnode->getitem())->second; //해당 단어가 있다면 전의 수를 저장한 다음
               new_tpindex.erase(currnode->getitem()); //일단 지우고새로운수를더해다시저장
               sav_prenum = sav_prenum + (currnode->getFrequency());
               new_tpindex.insert(make_pair(currnode->getitem(), sav_prenum));
            }
            ttt.insert(currnode->getitem()); //페스set<string>에저장과정
            //break;
         //}
      //}
            currnode = currnode->getParent();
         }
         inti_index.push_back(make_pair(save_freq, ttt)); //하나의 페스를 저장!
      }
      //그 여기서 init_index에서 threshold보다 작은애들을 뺀 list을 다시 만들어주는 함수를,,만들까,,
      //map<set<string>, int> minus_smal;
      //minus_smal = make_list(new_tpindex, inti_index);
      map<set<string>, int>* new_map= new map<set<string>, int>;
      vector<string> path_word;
      for (map<string, int>::iterator index_it = new_tpindex.begin(); index_it != new_tpindex.end(); index_it++) {
         if (index_it->second < threshold) {// 인덱스의수가 트레숄드보다 작으면
            //리스트에서 해당 아이템을 삭제해버린다
            for (list<pair<int, set<string>>>::iterator list_it = inti_index.begin(); list_it != inti_index.end(); list_it++) {
               if (list_it->second.find(index_it->first) != list_it->second.end()) { list_it->second.erase(index_it->first); }
            }
         }
         else {
            path_word.push_back(index_it->first); //파워셋에 넣어줄거
         }
      }

      for (list<pair<int, set<string>>>::iterator sav_it = inti_index.begin(); sav_it != inti_index.end(); sav_it++) {
         //new_map->insert(make_pair(sav_it->second, sav_it->first));  //파워셋에 넣어줄 map 만들기 그냥 순서바꿔주는거임
         if(frequenctPatterns.find(sav_it->second)== frequenctPatterns.end()){ //그 스트링이 없다면
         push_freqPattern(sav_it->second, sav_it->first);
         }
         else{ set<string> st_tmp = sav_it->second;
            int tmep_save = frequenctPatterns.find(st_tmp)->second;
            map<set<string>, int>::iterator remove_mp = frequenctPatterns.find(st_tmp);
          frequenctPatterns.erase(remove_mp);
          tmep_save=tmep_save + sav_it->first;
          push_freqPattern(st_tmp, tmep_save); 
          } //안된다면 삭제후 넣어주기
      }
      //이제 파워셋 불러준다~
      for (list<int>::iterator sib = each_frq.begin(); sib != each_frq.end(); sib++) {
         int* ptr;
         
         powerSet(&frequenctPatterns, path_word, ag_tp, *sib, ptr = new int(path_word.size()), 0);
         delete[] ptr;
      }
   }

   return;
}

void FPGrowth::once_doit(list<pair<int, string>> save_freque1, string currentitem){ 
   set<string> save_pattern;
    map<string,FPNode*> save_node;
    HeaderTable* save_table = new HeaderTable;

    int get_save_freq=0;

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
 
   return;// push_freqPattern(map<set<string>, int> hihi)
}

void FPGrowth::printPatern(){
   map<set<string>, int>::iterator fre_iter= frequenctPatterns.begin();
   for( ; fre_iter != frequenctPatterns.end(); fre_iter++){
	  if(fre_iter->second < threshold){continue;}
      
      //*fout<<fre_iter->second<<"\t";
      set<string> temp_set = fre_iter->first;
	  set<string>::iterator itt = temp_set.begin();
	  itt++; if(itt == temp_set.end()){continue;}
	  flog<<fre_iter->second<<"\t";
      cout<<fre_iter->second<<"\t";
      for(set<string>::iterator itt = temp_set.begin(); itt != temp_set.end(); itt++){
         flog<<*itt<<"\t";
         cout<<*itt<<"\t";
         //*fout<<*itt<<"\t";
      }
      flog<<endl;
      cout<<endl;
      //*fout<<endl;
   }
}