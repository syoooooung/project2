#include "Manager.h"
//암거나 수정
//수정 2
void Manager::run(const char* command)
{
	fin.open(command);
	char cmd[1000] ="";
	if(!fin)
	{
		flog << "File Open Error" << endl;
		return;
	}

	while (!fin.eof())
	{
		/* You must fill here */
		fin.getline(cmd,550);
		char* command = strtok(cmd, "	");
		if (command == NULL) { cout<< "No command "; return;}
		if(strcmp(command, "LOAD")==0){ //if command==load
			if(!LOAD()){  //do load
				//printerror(load);
				flog<<"=========LOAD========"<<endl;
				printErrorCode(100); //error code
			}
			else{
				flog<<"=========LOAD========"<<endl;
				printSuccessCode();
			}
		}
		else if (strcmp(command, "PRINT_ITEMLIST")==0){ //if command ==print itemlist
			if(!PRINT_ITEMLIST()){
				//printerror
				flog<<"======PRINT_ITEMLIST======"<<endl;
				printErrorCode(300);
			}
			
		}
		else if(strcmp(command,"PRINT_FPTREE")==0){ //command==print fptree
			if(!PRINT_FPTREE()){
				flog<<"=====PRINT_FPTREE====="<<endl;
				printErrorCode(400);
			}
		}
		else if(strcmp(command,"SAVE")==0){ //command= save
			if(!SAVE()){
				
			}
		}
		else if(strcmp(command,"BTLOAD")==0){ //command = btload
			if(!BTLOAD()){
				flog<<"=========BTLOAD========"<<endl;
				printErrorCode(200);
			}
			else{
				flog<<"=========BTLOAD========"<<endl;
				printSuccessCode();
			}
		}
		else if(strcmp(command ,"PRINT_BPTREE")==0){ //command ==bptree
			command=strtok(NULL,"	");
			char* temp = command;
			command=strtok(NULL,"	");
			string wait_St=command;
			int brn_num = atoi(wait_St.c_str());
			if(!PRINT_BPTREE(temp, brn_num)){
				flog<<"=========PRINT_BPTREE========"<<endl;
				printErrorCode(500);
			}
		}
		else if(strcmp(command,"PRINT_CONFIDENCE")==0){ //command==printconfidence
			command=strtok(NULL,"	");
			char* temp = command;
			command=strtok(NULL,"	");
			const char* double_be = command;
			double con_nm = atof(double_be);
			if(!PRINT_CONFIDENCE(temp, con_nm)){
				flog<<"======PRINT_CONFIDENCE====="<<endl;
				printErrorCode(600);
			}

		}
		else if(strcmp(command,"PRINT_RANGE")==0){ //command==print_range
			command=strtok(NULL,"	");
			char* temp = command;
			command = strtok(NULL,"	");
			string small=command;
			int min_num = atoi(small.c_str());
			command = strtok(NULL," ");
			string max= command;
			int max_num= atoi(max.c_str());
			if(!PRINT_RANGE(temp,min_num, max_num)){

			}
			
		}
		else if(strcmp(command,"EXIT")==0){
			if(EXIT()){
				flog<<"===========EXIT=========="<<endl;
				printSuccessCode();
			}
			//delete fpgrowth;
			//delete bptree;
			break;
		}
	}
	fin.close();
	return;
}

bool Manager::LOAD()
{
	ifstream market_fin;
	char mk[500]="";
	market_fin.open("market.txt"); //market.txt open
	if(!market_fin){
		cout<<"market.txt not open!!"<<endl; //error print
		return 0;
	}
	//make table
	list<string> try_list;
	string endline ="end"; //mark end
	string special_end = "real_end"; //mark endline
	//	int item_frequency(string item) {return table->find_frequency(item);}
	while(!market_fin.eof()){ //index table
		market_fin.getline(mk,500);

		char* market_infor = strtok(mk,"\t");

		while(market_infor != NULL){
			//string str_market_item(market_infor);
			fpgrowth->createTable(market_infor, fpgrowth->item_frequency(market_infor));
			try_list.push_back(market_infor);
			market_infor = strtok(NULL,"\t");
		}
		try_list.push_back(endline);
	}
	try_list.push_back(special_end);
	market_fin.close();
	fpgrowth->sort_descending_index(); //sort indext table for set map
	
	int num_for_make_dataTable=0;
	while(1){
		if(fpgrowth->get_index_item(num_for_make_dataTable) == "") break;
		
		fpgrowth->create_dataTable(fpgrowth->get_index_item(num_for_make_dataTable));
		num_for_make_dataTable++;
	} //data table

	list<string> manager_item_list;
	list<pair<int, string> > temp_index = fpgrowth->getHeaderTable()->getindexTable();
	list<string>::iterator iter_try = try_list.begin();
	string check_empty = "";
	int break_num = 0; //break num
	int iam_cont = 0;
	while (*try_list.begin() != special_end) {
		for (list<pair<int, string>>::iterator itertemp = temp_index.begin(); itertemp != temp_index.end(); itertemp++) {
			if(iam_cont){iam_cont=0;}
			else{itertemp=temp_index.begin();}//here
			//for(; iter_try != try_list.end() ; iter_try++){
			while (*try_list.begin() != "end") { //if end approach => escape
				if (itertemp->second == *iter_try) {
					if (itertemp->first < fpgrowth->get_threshold()) { 
						iter_try=try_list.erase(iter_try); break_num = 1; continue; } //break->continue;
					manager_item_list.push_back(*iter_try);
					iter_try=try_list.erase(iter_try); //delete
					iter_try = try_list.begin();
					continue;
				}
				iter_try++;
				if (*iter_try == "end") { iter_try = try_list.begin(); iam_cont = 1; break; }
			}
			if (iam_cont) { continue; }
			iter_try=try_list.erase(iter_try);
			iter_try = try_list.begin();
			itertemp= temp_index.begin();
			manager_item_list.push_back(endline);
			//if(itertemp->first < fpgrowth->get_threshold()){ break;}
			if (*try_list.begin() == special_end) { break; }
		}
		//if(break_num){break;}
	}
	//manager_item_list.push_back(market_infor);

	fpgrowth->createFPtree(fpgrowth->getTree(), fpgrowth->getHeaderTable(), manager_item_list, 1);

	return true;
}
bool Manager::SAVE(){
	fpgrowth->saveFrequentPatterns();
	fpgrowth->printPatern();
	return 1;
}

bool Manager::BTLOAD()
{	ifstream result_fin;
	char rs[10000]="";
	result_fin.open("result.txt");
	if(!result_fin){
		cout<<"result.txt not open!!"<<endl;
		return 0;
	}

	while(!result_fin.eof()){ //index table
      set<string> result_list;    //set new
      result_fin.getline(rs,9000);
      char* command1 = strtok(rs, "\t");
	  char*command= strtok(NULL,"\t");
      //string wait_St=command;
      //int save_num = atoi(wait_St.c_str());
      while(command != NULL){
         string put_re=command;
         result_list.insert(put_re);
		 command=strtok(NULL,"\t");
      }
	  int o_num=atoi(command1);
	 
    bptree->Insert(o_num, result_list);
   }
	result_fin.close();
	return true;
}

bool Manager::PRINT_ITEMLIST() {
	if(fpgrowth->getHeaderTable()==NULL){ return 0;} //error code
	
	flog<<"========PRINT_ITEMLIST========"<<endl; //print itemlist
	flog<<"Item"<<"\t"<<"Frequency"<<endl;
	fpgrowth->printList(); 
	flog<<"=============================="<<endl;
	return 1;
}

bool Manager::PRINT_FPTREE() {
	flog<<"=========PRINT_FPTREE========"<<endl; //print fp-tree
	flog<<"{StandardItem,Frequency}, (Path_Item,Frequency)"<<endl;
	fpgrowth->printTree();
	flog<<"============================="<<endl;
	return 1;
}

bool Manager::PRINT_BPTREE(char* item, int min_frequency) { //print bptree
	string ttem(item); //char* to string
	flog<<"========PRINT_BPTREE==========="<<endl;
	flog<<"FrequentPattern"<<"\t \t"<<"Frequency"<<endl;
	bptree->printFrequency(ttem, min_frequency);
	flog<<"==============================="<<endl;
	return 1;
}

bool Manager::PRINT_CONFIDENCE(char* item, double rate) { //print confidence
	list<pair<int, string>> find_fre = fpgrowth->getHeaderTable()->getindexTable();
	double save_hownum = 0;
	for(list<pair<int, string>>::iterator i_ter= find_fre.begin(); i_ter != find_fre.end(); i_ter++ ){
		if(i_ter->second == item){save_hownum=i_ter->first;} //find item in indextable
	}
	if(save_hownum==0 || bptree->getRoot()==NULL){ return 0;} //if no freq and dont exist bptree=>error
	string r_tem(item);
	flog<<"========PRINT_CONFIDENCE==========="<<endl;
	flog<<"FrequentPattern"<<"  "<<"Frequency"<<"\t"<<"Confidence"<<endl;
	if(!bptree->printConfidence(r_tem, save_hownum, rate)){ return 0;}
	flog<<"==================================="<<endl;
	return 1;
}
	

bool Manager::PRINT_RANGE(char* item, int start, int end) { //print range
	string pss_item(item); //char* to string
	flog<<"========PRINT_RANGE==========="<<endl;
	flog<<"FrequentPattern"<<"  "<<"Frequency"<<endl;
	bptree->printRange(pss_item, start, end);
	flog<<"==================================="<<endl;
	return 1;
}

bool Manager::EXIT()
{
   map<string, FPNode*> datatable = fpgrowth->getHeaderTable()->getdataTable(); //get data table
  
   for(map<string,FPNode*>::iterator iter = datatable.begin();iter!=datatable.end();iter++) { //all data delete
      FPNode* currNode = iter->second; //get node
      while(1){
         if(currNode == NULL){
			break;
		 }
         FPNode* temp = currNode;
         currNode = currNode->getNext();
         delete temp;
       }
   }
   FPNode* rootnode = fpgrowth->getTree(); //root
   delete rootnode; //delete root

   HeaderTable* gethead = fpgrowth->getHeaderTable(); 
   delete gethead; //delete headertable

   return 1;
}

void Manager::printErrorCode(int n) {	//ERROR CODE PRINT
	flog << "ERROR  " << n << endl;
	flog << "=======================" << endl;
	return;
}

void Manager::printSuccessCode() {//SUCCESS CODE PRINT 
	flog << "Success" << endl;
	flog << "=======================" << endl ;
	return;
}