#include "BpTree.h"

int from_data = 0;
bool BpTree::Insert(int key, set<string> set2) {
    FrequentPatternNode* newnode = new FrequentPatternNode;
    set<string> set1 = set2;
    int cnt = 0;
    for (set<string>::iterator count_it = set1.begin(); count_it != set1.end(); count_it++) { cnt++; }
    newnode->setFrequency(cnt);
    newnode->InsertList(set1);
    if (root == NULL) {
        BpTreeNode* newdata = new BpTreeDataNode;
        newdata->insertDataMap(key, newnode);
        root = newdata;
        return 1;
    }


    BpTreeNode* currnode = searchDataNode(key);
    if (currnode->getDataMap()->find(key) == currnode->getDataMap()->end()) { //같은 key의 데이터없음
        currnode->insertDataMap(key, newnode);
        if (excessDataNode(currnode)) {
            splitDataNode(currnode);
        }
    }
    else {
        currnode->getDataMap()->find(key)->second->InsertList(set1);
    }

    return true;
}

BpTreeNode* BpTree::searchDataNode(int n) {
    BpTreeNode* pCur = root;
    
        while (pCur->getMostLeftChild() != NULL) { pCur = pCur->getMostLeftChild(); }

        map<int, FrequentPatternNode*>::iterator iter_search = pCur->getDataMap()->begin();
        while (pCur!=NULL) { //datanode의 끝이면 end에 1을 넣어주며 break걸것
            iter_search = pCur->getDataMap()->begin();
            if (n > iter_search->first) { //다음으로넘어가
                if (pCur->getNext() == NULL) { return pCur; }
                pCur = pCur->getNext();
                iter_search = pCur->getDataMap()->begin();
                if (n < iter_search->first) { //넘어갔는데 걔보단 또 작아
                    pCur = pCur->getPrev();
                    return pCur;
                }
                for (iter_search = pCur->getDataMap()->begin(); iter_search != pCur->getDataMap()->end(); iter_search++) { //넘어가서 나보다 크거나 같은애를 찾았어
                    if (n <= iter_search->first) { return pCur; }
                }
                if (pCur->getNext() == NULL) { return pCur; } //마지막인데 내가 없어
            }
            else if (n <= iter_search->first) { return pCur; } //돌아가는 함수에서 pCur에 frqpatternode를 인서트해주면 되네!
        }
    
    return pCur;
}
//오류가 발생한다면!!!!!!!split진행시에 index가 그냥 밑이 아닌 본인이 들어가있는 것을 찾은 후 그걸 가리키게끔
void BpTree::splitDataNode(BpTreeNode* pDataNode) {
    set<int> temp_array;
    map <int, FrequentPatternNode*>::iterator itt = pDataNode->getDataMap()->begin();
    for (; itt != pDataNode->getDataMap()->end(); itt++) {
        temp_array.insert(itt->first);
    }
    int sp_location = ceil((order - 1) / 2.0) + 1;
    set<int>::iterator it = temp_array.begin();
    int curr_num = 0, whats_num = 0;
    while (1) {
        curr_num++;
        if (curr_num == sp_location) { whats_num = *it; break; } //그게 index새로 해줄 map의 위치
        it++;
    }
    map <int, FrequentPatternNode*>::iterator save_index_it = pDataNode->getDataMap()->find(whats_num);
    //여기부턴 그냥 떼어주기 젤 앞!!
    it = temp_array.begin();
    itt = pDataNode->getDataMap()->find(*it); //이 떼어준 애가 index노드에서 연결을 해줘야할 애인지 판별 그건 index추가함수에서 해주나?
    BpTreeNode* newnode = new BpTreeDataNode;
    newnode->insertDataMap(itt->first, itt->second);
    pDataNode->deleteMap(itt->first);

    if (pDataNode->getPrev() != NULL) {
        BpTreeNode* prevnode = pDataNode->getPrev();
        newnode->setPrev(prevnode);
        prevnode->setNext(newnode);
        newnode->setNext(pDataNode);
        pDataNode->setPrev(newnode);
    }
    else {
        newnode->setNext(pDataNode);
        pDataNode->setPrev(newnode);
    }

    if (pDataNode->getParent() == NULL) {
        BpTreeNode* new_indexnode = new BpTreeIndexNode;
        new_indexnode->insertIndexMap(save_index_it->first, pDataNode);
        new_indexnode->setMostLeftChild(newnode);
        newnode->setParent(new_indexnode);
        pDataNode->setParent(new_indexnode);
        root = new_indexnode;
    }
    else {
        pDataNode->getParent()->insertIndexMap(save_index_it->first, pDataNode);
        newnode->setParent(pDataNode->getParent());
        if (excessIndexNode(pDataNode->getParent())) {
            from_data = 1;
            splitIndexNode(pDataNode->getParent());
            from_data = 0;
        }
    }
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
    set<int> temp_array;
    map <int, BpTreeNode*>::iterator it2 = pIndexNode->getIndexMap()->begin();
    for (; it2 != pIndexNode->getIndexMap()->end(); it2++) {
        temp_array.insert(it2->first);
    }
    int sp_location = ceil((order - 1) / 2.0) + 1;
    set<int>::iterator it = temp_array.begin();
    int curr_num = 0, whats_num = 0;
    while (1) {
        curr_num++;
        if (curr_num == sp_location) { whats_num = *it; break; } //그게 index새로 해줄 map의 위치
        it++;
    }
    map <int, BpTreeNode*>::iterator save_index_new = pIndexNode->getIndexMap()->find(whats_num); //위로올라갈index


    BpTreeNode* split_newnode = new BpTreeIndexNode; //pIndexNode 위로올라갈 애를 기준으로 끊어내는 과정
    it2 = pIndexNode->getIndexMap()->begin();
    for (it = temp_array.begin(); *it < save_index_new->first;) {
        it2 = pIndexNode->getIndexMap()->find(*it);
        split_newnode->insertIndexMap(it2->first, it2->second);
        pIndexNode->getIndexMap()->erase(*it); ///////////////////////////////segmentfault 주의!!!!!!!!!!!!!!!!
        it++;
    }//올라간애의 leftchild는 위의 splitnnewnode 새로운 node임~
    it--;
    int already_left = 0;
    if (from_data) {
        BpTreeNode* curr = root;
        while (curr->getMostLeftChild() != NULL) { curr = curr->getMostLeftChild(); }
        while (curr->getParent() != pIndexNode) {
            curr = curr->getNext();
        }
        while (1) {
            map<int, FrequentPatternNode*>::iterator data_search = curr->getDataMap()->begin();
            if (data_search->first <= *it) {
                curr->setParent(split_newnode);
                if (!already_left) {
                    split_newnode->setMostLeftChild(curr);
                    already_left = 1;
                }
            }
            else { already_left = 0; pIndexNode->setMostLeftChild(curr); break; }
            curr = curr->getNext();
        }
    }
    from_data = 0;


    if (pIndexNode->getParent() == NULL) {
        BpTreeNode* new_indnode = new BpTreeIndexNode;
        new_indnode->insertIndexMap(save_index_new->first, save_index_new->second);
        new_indnode->setMostLeftChild(split_newnode);
        split_newnode->setParent(new_indnode);
        pIndexNode->setParent(new_indnode);
        root = new_indnode;
    }
    else {
        pIndexNode->getParent()->insertIndexMap(save_index_new->first, save_index_new->second);
        pIndexNode->getParent()->setMostLeftChild(split_newnode);
        split_newnode->setParent(pIndexNode->getParent());
        if (excessIndexNode(pIndexNode->getParent())) {
            splitIndexNode(pIndexNode->getParent());
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
	
    return true;
}
bool BpTree::printFrequency(string item, int min_frequency)//print winratio in ascending order
{
    BpTreeNode* currnode = searchDataNode(min_frequency);

    map<int, FrequentPatternNode*>::iterator iter_print = currnode->getDataMap()->begin();
    for (iter_print = currnode->getDataMap()->begin(); iter_print != currnode->getDataMap()->end(); iter_print++) {
        if (iter_print->first >= min_frequency) {
            FrequentPatternNode* F_curr = iter_print->second;
            multimap<int, set<string> > tmp_amp = F_curr->getList();
            multimap<int, set<string> >::iterator F_iter =tmp_amp.begin();
            for (F_iter = tmp_amp.begin(); F_iter != tmp_amp.end(); F_iter++) {
                set<string> che_set = F_iter->second;
                if (che_set.find(item) == che_set.end()) { continue; } //입력받은 글자가 있는 지 확인
                *fout << "{";
                for (set<string>::iterator plz = F_iter->second.begin(); plz != F_iter->second.end(); ) {
                    *fout << *plz; plz++; if (plz != F_iter->second.end()) { *fout << ", ";  }
                }
                *fout << "}" << "  " << iter_print->first << endl;
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
                    if (che_set.find(item) == che_set.end()) { continue; } //입력받은 글자가 있는 지 확인
                    *fout << "{";
                    for (set<string>::iterator plz = F_iter->second.begin(); plz != F_iter->second.end(); ) {
                        *fout << *plz; plz++; if (plz != F_iter->second.end()) { *fout << ", "; }
                    }
                    *fout << "}" << "  " << iter_print->first << endl;
                }
        }
    }
    return true;
}
bool BpTree::printRange(string item, int min, int max) {

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
            *fout << "} ";
            break;
        }
        *fout << ", ";
    }
}
