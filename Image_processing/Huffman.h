#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

typedef vector<bool> CodeWord;

class BinaryTreeNode
{
public:
	double value;
	int index;
	int label;
	BinaryTreeNode *left;
	BinaryTreeNode *right;

	BinaryTreeNode(double value = 0.0);
	~BinaryTreeNode();
};

class BinaryTree
{
public:
	BinaryTree(const BinaryTreeNode &root) : root(root) { }
	BinaryTreeNode root;
};

class Huffman
{
public:
	void BuildCode(const vector<double> &frequenses, vector<CodeWord> &out_codewords);
private:
	BinaryTree buildTree(vector<double> frequenses); // pass by value to make local copy
	void dfs(BinaryTree &tree, vector<CodeWord> &codewords);
};

#endif // _HUFFMAN_H_