#ifndef _BpTree_H_
#define _BpTree_H_

#include "BpTreeDataNode.h"
#include "BpTreeIndexNode.h"
#include <fstream>
#include <iostream>

class BpTree {
private:
	BpTreeNode* root;
	BpTreeNode * da_root = NULL;
	BpTreeNode* in_root = NULL;

	int			order;		// m children
	ofstream* fout;
public:
	BpTree(ofstream *fout, int order = 3) {
		root = NULL;
		this->order = order;
		this->fout = fout;
		BpTreeNode * da_root = new BpTreeDataNode; //지울것
		BpTreeNode * in_root = new BpTreeIndexNode; //지울것
	}
	~BpTree();
	/* essential */
	bool		Insert(int key, set<string> set);
	bool		excessDataNode(BpTreeNode* pDataNode);
	bool		excessIndexNode(BpTreeNode* pIndexNode);
	void		splitDataNode(BpTreeNode* pDataNode);
	void		splitIndexNode(BpTreeNode* pIndexNode);
	BpTreeNode* getRoot() { return root; }
	BpTreeNode* searchDataNode(int n);

	void	printFrequentPatterns(set<string> pFrequentPattern, string item);
	bool	printFrequency(string item, int min_frequency);
	bool	printConfidence(string item, double item_frequency, double min_confidence);
	bool 	printRange(string item, int min, int max);

};

#endif
