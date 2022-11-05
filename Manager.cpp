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
		char* command = strtok(cmd, " ");
		if (command == NULL) { cout<< "No command "; return;}
		if(strcmp(command, "LOAD")==0){
			if(!LOAD()){
				//printerror(load);
				flog<<"=========LOAD========"<<endl;
				printErrorCode(100);
			}
			else{
				flog<<"=========LOAD========"<<endl;
				printSuccessCode();
			}
		}
		else if (strcmp(command, "PRINT_ITEMLIST")==0){
			if(!PRINT_ITEMLIST()){
				//printerror
				flog<<"======PRINT_ITEMLIST======"<<endl;
				printErrorCode(300);
			}
			
		}
		else if(strcmp(command,"PRINT_FPTREE")==0){
			if(!PRINT_FPTREE()){
				flog<<"=====PRINT_FPTREE====="<<endl;
				printErrorCode(400);
			}
		}
	
	}
	fin.close();
	return;
}

bool Manager::LOAD()
{
	ifstream market_fin;
	char mk[500]="";
	market_fin.open("market.txt");
	if(!market_fin){
		cout<<"market.txt not open!!"<<endl;
		return 0;
	}
	
	list<string> try_list;
	string endline ="end";
	string special_end = "real_end";
	//	int item_frequency(string item) {return table->find_frequency(item);}
	while(!market_fin.eof()){ //index table
		market_fin.getline(mk,500);

		char* market_infor = strtok(mk,"\t");

		while(market_infor != NULL){
			//string str_market_item(market_infor);
			//cout<<market_infor<<" / "<<fpgrowth->item_frequency(market_infor)<<endl;
			fpgrowth->createTable(market_infor, fpgrowth->item_frequency(market_infor));
			try_list.push_back(market_infor);
			market_infor = strtok(NULL,"\t");
		}
		try_list.push_back(endline);
	}
	try_list.push_back(special_end);
	market_fin.close();
	//fpgrowth->createTable(??,fpgrowth->item_frequenct())
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
	int break_num = 0;
	int iam_cont = 0;
	while (*try_list.begin() != special_end) {
		for (list<pair<int, string>>::iterator itertemp = temp_index.begin(); itertemp != temp_index.end(); itertemp++) {
			if(iam_cont){iam_cont=0;}
			else{itertemp=temp_index.begin();}//여기여기여깅,~~
			//for(; iter_try != try_list.end() ; iter_try++){
			while (*try_list.begin() != "end") {
				if (itertemp->second == *iter_try) {
					if (itertemp->first < fpgrowth->get_threshold()) { 
						iter_try=try_list.erase(iter_try); break_num = 1; continue; } //얘 원래 브레이큰ㄷ ㅔ콘티뉴로 바꿈!!
					manager_item_list.push_back(*iter_try);
					iter_try=try_list.erase(iter_try);
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
	for(list<string>::iterator iter_just=manager_item_list.begin(); iter_just != manager_item_list.end(); iter_just++){
		cout<<*iter_just<<" ";
		if(*iter_just=="end"){cout<<endl;	
		}
	}

	fpgrowth->createFPtree(fpgrowth->getTree(), fpgrowth->getHeaderTable(), manager_item_list, 1);

	return true;
}

bool Manager::BTLOAD()
{	
	return true;
}

bool Manager::PRINT_ITEMLIST() {
	if(fpgrowth->getHeaderTable()==NULL){ return 0;}
	
	flog<<"========PRINT_ITEMLIST========"<<endl;
	flog<<"Item"<<"\t"<<"Frequency"<<endl;
	fpgrowth->printList(); 
	flog<<"=============================="<<endl;
	return 1;
}

bool Manager::PRINT_FPTREE() {
	flog<<"=========PRINT_FPTREE========"<<endl;
	flog<<"{StandardItem,Frequency}, (Path_Item,Frequency)"<<endl;
	fpgrowth->printTree();
	flog<<"============================="<<endl;
	return 1;
}

bool Manager::PRINT_BPTREE(char* item, int min_frequency) {
	
}

bool Manager::PRINT_CONFIDENCE(char* item, double rate) {
	
}

bool Manager::PRINT_RANGE(char* item, int start, int end) {
	
}

void Manager::printErrorCode(int n) {				//ERROR CODE PRINT
	flog << "ERROR  " << n << endl;
	flog << "=======================" << endl << endl;
	return;
}

void Manager::printSuccessCode() {//SUCCESS CODE PRINT 
	flog << "Success" << endl;
	flog << "=======================" << endl ;
	return;
}

