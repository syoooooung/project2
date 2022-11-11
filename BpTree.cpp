#include "BpTree.h"

int from_data = 0;
bool BpTree::Insert(int key, set<string> set2) { //insert func
    FrequentPatternNode* newnode = new FrequentPatternNode;
    set<string> set1 = set2;
    int cnt = 0; //how freq
    for (set<string>::iterator count_it = set1.begin(); count_it != set1.end(); count_it++) { cnt++; } 
    newnode->setFrequency(cnt);
    newnode->InsertList(set1);
    if (root == NULL) { //if bptree is empty
        BpTreeNode* newdata = new BpTreeDataNode;
        newdata->insertDataMap(key, newnode);
        root = newdata;
        return 1;
    }


    BpTreeNode* currnode = searchDataNode(key);
    if (currnode->getDataMap()->find(key) == currnode->getDataMap()->end()) { //no same key data
        currnode->insertDataMap(key, newnode);
        if (excessDataNode(currnode)) { //if excessdatanode order
            splitDataNode(currnode); // than split
        }
    }
    else { //same key exist
        currnode->getDataMap()->find(key)->second->InsertList(set1);
    }

    return true;
}

BpTreeNode* BpTree::searchDataNode(int n) {
    BpTreeNode* pCur = root;
    
        while (pCur->getMostLeftChild() != NULL) { pCur = pCur->getMostLeftChild(); }

        map<int, FrequentPatternNode*>::iterator iter_search = pCur->getDataMap()->begin();
        while (pCur!=NULL) { //if end of datanode=> put end 1 and break 
            iter_search = pCur->getDataMap()->begin();
            if (n > iter_search->first) {   //get next
                if (pCur->getNext() == NULL) { return pCur; }
                pCur = pCur->getNext();
                iter_search = pCur->getDataMap()->begin();
                if (n < iter_search->first) { //get next but smaller than that
                    pCur = pCur->getPrev();
                    return pCur;
                }
                for (iter_search = pCur->getDataMap()->begin(); iter_search != pCur->getDataMap()->end(); iter_search++) { //next and find me
                    if (n <= iter_search->first) { return pCur; }
                }
                if (pCur->getNext() == NULL) { return pCur; } //final but i am not exist
            }
            else if (n <= iter_search->first) { return pCur; } //recur func and put frepatternode inset at pCUr 
        }
    
    return pCur;
}
//if error occur=> split indexdown x => that node find and put & point
void BpTree::splitDataNode(BpTreeNode* pDataNode) {
    set<int> temp_array;
    map <int, FrequentPatternNode*>::iterator itt = pDataNode->getDataMap()->begin();
    for (; itt != pDataNode->getDataMap()->end(); itt++) {
        temp_array.insert(itt->first); //copy datamap for sort order
    }
    int sp_location = ceil((order - 1) / 2.0) + 1; //calculation split location
    set<int>::iterator it = temp_array.begin();
    int curr_num = 0, whats_num = 0;
    while (1) {
        curr_num++;
        if (curr_num == sp_location) { whats_num = *it; break; } //that is new index location
        it++;
    }
    map <int, FrequentPatternNode*>::iterator save_index_it = pDataNode->getDataMap()->find(whats_num);
    // just split front
    it = temp_array.begin();
    itt = pDataNode->getDataMap()->find(*it); //index connect?
    BpTreeNode* newnode = new BpTreeDataNode;
    newnode->insertDataMap(itt->first, itt->second);
    pDataNode->deleteMap(itt->first);

    if (pDataNode->getPrev() != NULL) { //if datanode is not first node
        BpTreeNode* prevnode = pDataNode->getPrev();
        newnode->setPrev(prevnode);
        prevnode->setNext(newnode); //link pre node
        newnode->setNext(pDataNode);
        pDataNode->setPrev(newnode);
    }
    else { //if datanode is first node
        newnode->setNext(pDataNode);
        pDataNode->setPrev(newnode);
    }

    if (pDataNode->getParent() == NULL) { //if datanode doesnt have parentnode
        BpTreeNode* new_indexnode = new BpTreeIndexNode; //new node that is parent
        new_indexnode->insertIndexMap(save_index_it->first, pDataNode);
        new_indexnode->setMostLeftChild(newnode);
        newnode->setParent(new_indexnode);
        pDataNode->setParent(new_indexnode);
        root = new_indexnode;
    }
    else { //if datanode has parent node
        pDataNode->getParent()->insertIndexMap(save_index_it->first, pDataNode);
        newnode->setParent(pDataNode->getParent());
        if (excessIndexNode(pDataNode->getParent())) {
            from_data = 1;
            splitIndexNode(pDataNode->getParent());
            from_data = 0;
        }
    }
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) { //split index node func
    set<int> temp_array;
    map <int, BpTreeNode*>::iterator it2 = pIndexNode->getIndexMap()->begin();
    for (; it2 != pIndexNode->getIndexMap()->end(); it2++) { //for abs array using set
        temp_array.insert(it2->first);
    }
    int sp_location = ceil((order - 1) / 2.0) + 1; //calculation split location
    set<int>::iterator it = temp_array.begin();
    int curr_num = 0, whats_num = 0;
    while (1) {
        curr_num++;
        if (curr_num == sp_location) { whats_num = *it; break; } //that is new index map location
        it++;
    }
    map <int, BpTreeNode*>::iterator save_index_new = pIndexNode->getIndexMap()->find(whats_num); //that is move up index


    BpTreeNode* split_newnode = new BpTreeIndexNode; // move up index split pIndexNode proecess
    it2 = pIndexNode->getIndexMap()->begin();
    for (it = temp_array.begin(); *it < save_index_new->first;) {
        it2 = pIndexNode->getIndexMap()->find(*it);
        split_newnode->insertIndexMap(it2->first, it2->second);
        pIndexNode->getIndexMap()->erase(*it); ///segmentfault cuation
        it++;
    }//move up node is tha new node of splitnodenode's leftchild
    it--;
    int already_left = 0; //have left child or not
    if (from_data) { //if this fund is call from datasplit
        BpTreeNode* curr = root;
        while (curr->getMostLeftChild() != NULL) { curr = curr->getMostLeftChild(); }
        while (curr->getParent() != pIndexNode) {
            curr = curr->getNext();
        }
        while (1) {
            map<int, FrequentPatternNode*>::iterator data_search = curr->getDataMap()->begin();
            if (data_search->first <= *it) {
                curr->setParent(split_newnode); //set parent
                if (!already_left) {
                    split_newnode->setMostLeftChild(curr);
                    already_left = 1;
                }
            }
            else { already_left = 0; pIndexNode->setMostLeftChild(curr); break; } //set leftchild
            curr = curr->getNext();
        }
    }
    from_data = 0;


    if (pIndexNode->getParent() == NULL) { //if indexnode doesnt have parent node
        BpTreeNode* new_indnode = new BpTreeIndexNode;
        new_indnode->insertIndexMap(save_index_new->first, save_index_new->second);
        new_indnode->setMostLeftChild(split_newnode); //new parent node
        split_newnode->setParent(new_indnode);
        pIndexNode->setParent(new_indnode);
        root = new_indnode;
    }
    else { //if indexnode have parent node
        pIndexNode->getParent()->insertIndexMap(save_index_new->first, save_index_new->second);
        pIndexNode->getParent()->setMostLeftChild(split_newnode);
        split_newnode->setParent(pIndexNode->getParent()); //just move up and
        if (excessIndexNode(pIndexNode->getParent())) { //if excess indexnode
            splitIndexNode(pIndexNode->getParent()); //split indexnode
        }

    }
}
bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
    if (pDataNode->getDataMap()->size() > order - 1) return true;//order is equal to the number of elements 
    else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
    if (pIndexNode->getIndexMap()->size() > order - 1)return true;//order is equal to the number of elements 
    else return false;
}


bool BpTree::printConfidence(string item, double item_frequency, double min_confidence)
{
	int search_num = (item_frequency)*(min_confidence); //tiem_Freq * min_conf
	
	double print_con = 0;
	BpTreeNode* currnode = searchDataNode(search_num); //search that fre num
    map<int, FrequentPatternNode*>::iterator iter_print = currnode->getDataMap()->begin();
    for (iter_print = currnode->getDataMap()->begin(); iter_print != currnode->getDataMap()->end(); iter_print++) {
		double temp_chnum = iter_print->first;
		double check_search_num = (item_frequency)*(min_confidence);
        if (temp_chnum > check_search_num) { //print when larger than check_searchnum 
            FrequentPatternNode* F_curr = iter_print->second;
            multimap<int, set<string> > tmp_amp = F_curr->getList();
            multimap<int, set<string> >::iterator F_iter =tmp_amp.begin();
            for (F_iter = tmp_amp.begin(); F_iter != tmp_amp.end(); F_iter++) {
                set<string> che_set = F_iter->second;
                if (che_set.find(item) == che_set.end()) { continue; } //check input word data exist
                printFrequentPatterns(che_set, item);
				print_con=iter_print->first / item_frequency; //calculation comfidence
                *fout << iter_print->first <<"  "<<print_con<<endl;
            }
        }
    }
    while (currnode->getNext() != NULL) { //loop until final node
        currnode = currnode->getNext();
        iter_print = currnode->getDataMap()->begin();
        for (iter_print = currnode->getDataMap()->begin(); iter_print != currnode->getDataMap()->end(); iter_print++) {
   
                FrequentPatternNode* F_curr = iter_print->second;
                multimap<int, set<string> > tmp_amp = F_curr->getList(); //set
                multimap<int, set<string> >::iterator F_iter = tmp_amp.begin(); //set prevent warning
                for (F_iter = tmp_amp.begin(); F_iter != tmp_amp.end(); F_iter++) {
                    set<string> che_set = F_iter->second;
                    if (che_set.find(item) == che_set.end()) { continue; } //check word data exist
                    printFrequentPatterns(che_set, item);
					print_con=iter_print->first / item_frequency;
                    *fout << iter_print->first <<" "<<print_con<< endl;
                }
        }
    }
	if(print_con==0){return 0;} //there's no print pattern => returnfalse

    return true;
}
bool BpTree::printFrequency(string item, int min_frequency)//print winratio in ascending order
{
    BpTreeNode* currnode = searchDataNode(min_frequency); //find min_freq

    map<int, FrequentPatternNode*>::iterator iter_print = currnode->getDataMap()->begin();
    for (iter_print = currnode->getDataMap()->begin(); iter_print != currnode->getDataMap()->end(); iter_print++) {
        if (iter_print->first >= min_frequency) {
            FrequentPatternNode* F_curr = iter_print->second;
            multimap<int, set<string> > tmp_amp = F_curr->getList(); //set multimap
            multimap<int, set<string> >::iterator F_iter =tmp_amp.begin(); //set iterator
            for (F_iter = tmp_amp.begin(); F_iter != tmp_amp.end(); F_iter++) {
                set<string> che_set = F_iter->second;
                if (che_set.find(item) == che_set.end()) { continue; } //check input data exist
                printFrequentPatterns(che_set,  item);
                *fout << iter_print->first << endl;
            }
        }
    }
    while (currnode->getNext() != NULL) {
        currnode = currnode->getNext();
        iter_print = currnode->getDataMap()->begin();
        for (iter_print = currnode->getDataMap()->begin(); iter_print != currnode->getDataMap()->end(); iter_print++) {
   
                FrequentPatternNode* F_curr = iter_print->second;
                multimap<int, set<string> > tmp_amp = F_curr->getList();
                multimap<int, set<string> >::iterator F_iter = tmp_amp.begin();
                for (F_iter = tmp_amp.begin(); F_iter != tmp_amp.end(); F_iter++) {
                    set<string> che_set = F_iter->second;
                    if (che_set.find(item) == che_set.end()) { continue; } //check input data exist
                    printFrequentPatterns(che_set, item);
                    *fout  << iter_print->first << endl;
                }
        }
    }
    return true;
}
bool BpTree::printRange(string item, int min, int max) {
	 BpTreeNode* currnode = searchDataNode(min); //min_num search

    map<int, FrequentPatternNode*>::iterator iter_print = currnode->getDataMap()->begin();
    for (iter_print = currnode->getDataMap()->begin(); iter_print != currnode->getDataMap()->end(); iter_print++) {
        if (iter_print->first >= min || iter_print->first <= max) { //lareger or same min_num
            FrequentPatternNode* F_curr = iter_print->second;
            multimap<int, set<string> > tmp_amp = F_curr->getList();
            multimap<int, set<string> >::iterator F_iter =tmp_amp.begin();
            for (F_iter = tmp_amp.begin(); F_iter != tmp_amp.end(); F_iter++) {
                set<string> che_set = F_iter->second;
                if (che_set.find(item) == che_set.end()) { continue; } //check input data exist
                printFrequentPatterns(che_set,  item); //freque print
                *fout << iter_print->first << endl;
            }
        }
    }
    while (currnode->getNext() != NULL) {
        currnode = currnode->getNext();
        iter_print = currnode->getDataMap()->begin();
        for (iter_print = currnode->getDataMap()->begin(); iter_print != currnode->getDataMap()->end(); iter_print++) {
			if (iter_print->first >= min || iter_print->first <= max) { //smaller or same max_num => print
                FrequentPatternNode* F_curr = iter_print->second;
                multimap<int, set<string> > tmp_amp = F_curr->getList();
                multimap<int, set<string> >::iterator F_iter = tmp_amp.begin();
                for (F_iter = tmp_amp.begin(); F_iter != tmp_amp.end(); F_iter++) {
                    set<string> che_set = F_iter->second;
                    if (che_set.find(item) == che_set.end()) { continue; } //check input data exist
                    printFrequentPatterns(che_set, item); //freqpattern print
                    *fout  << iter_print->first << endl;
                }
			}
        }
    }

    return true;
}
void BpTree::printFrequentPatterns(set<string> pFrequentPattern, string item) {
    *fout << "{";
    set<string> curPattern = pFrequentPattern;
    curPattern.erase(item);
    for (set<string>::iterator it = curPattern.begin(); it != curPattern.end();) {
        string temp = *it++;
        if (temp != item) *fout << temp;
        if (it == curPattern.end()) {
			*fout<<", "<<item; //print that item
            *fout << "} ";
            break;
        }
        *fout << ", ";
    }
}
