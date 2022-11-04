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

	//	int item_frequency(string item) {return table->find_frequency(item);}
	while(!market_fin.eof()){ //index table
		market_fin.getline(mk,500);

		char* market_infor = strtok(mk,"\t");

		while(market_infor != NULL){
			//string str_market_item(market_infor);
			//cout<<market_infor<<" / "<<fpgrowth->item_frequency(market_infor)<<endl;
			fpgrowth->createTable(market_infor, fpgrowth->item_frequency(market_infor));
			market_infor = strtok(NULL,"\t");
		}
	}
	market_fin.close();
	//fpgrowth->createTable(??,fpgrowth->item_frequenct())
	fpgrowth->sort_descending_index(); //sort indext table for set map

	int num_for_make_dataTable=0;
	while(1){
		if(fpgrowth->get_index_item(num_for_make_dataTable) == "") break;
		fpgrowth->create_dataTable(fpgrowth->get_index_item(num_for_make_dataTable));
		num_for_make_dataTable++;
	} //data table
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

