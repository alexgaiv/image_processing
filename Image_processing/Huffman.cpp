#include "Huffman.h"

BinaryTreeNode::BinaryTreeNode(double value) : value(value)
{
	index = label = 0;
	left = right = NULL;
}

BinaryTreeNode::~BinaryTreeNode()
{
	// not using recursion here, since tree height can be big

	if (!left && !right) return;

	stack<BinaryTreeNode*> st;
	st.push(this);

	while (!st.empty())
	{
		BinaryTreeNode *node = st.top();
		if (node->left) {
			st.push(node->left);
			node->left = NULL;
		}
		else if (node->right) {
			st.push(node->right);
			node->right = NULL;
		}
		else {
			if (node != this) delete node;
			st.pop();
		}
	}
}

void Huffman::BuildCode(const vector<double> &freqs, vector<CodeWord> &codewords)
{
	codewords.resize(freqs.size());
	dfs(buildTree(freqs), codewords);
}

BinaryTree Huffman::buildTree(vector<double> freqs)
{
	sort(freqs.begin(), freqs.end(),
		[](double a, double b) -> bool {
			return a > b;
		});

	vector<BinaryTreeNode *> nodes(freqs.size());
	for (int i = 0; i < freqs.size(); i++) {
		nodes[i] = new BinaryTreeNode(freqs[i]);
		nodes[i]->index = i;
	}

	while (nodes.size() != 1)
	{
		size_t s = nodes.size() - 1;
		BinaryTreeNode *node = new BinaryTreeNode;
		node->left = nodes[s];
		node->right = nodes[s - 1];
		node->value = node->left->value + node->right->value;

		nodes.pop_back();
		nodes.pop_back();

		bool found = false;
		for (int i = (int)nodes.size() - 1; i >= 0; i--) {
			if (node->value < nodes[i]->value) {
				nodes.insert(nodes.begin() + i + 1, node);
				found = true;
				break;
			}
		}
		if (!found)
			nodes.insert(nodes.begin(), node);
	}

	return BinaryTree(*nodes[0]);
}

void Huffman::dfs(BinaryTree &tree, vector<CodeWord> &codewords)
{
	stack<BinaryTreeNode*> st;
	st.push(&tree.root);

	CodeWord curPath;
	do
	{
		BinaryTreeNode *node = st.top();
		node->label = 1;

		if (node->left)
		{
			if (node->left->label == 0) {
				st.push(node->left);
				curPath.push_back(false);
			}
			else if (node->right->label == 0) {
				st.push(node->right);
				curPath.push_back(true);
			}
			else {
				if (curPath.size() == 0) break;
				curPath.pop_back();
				st.pop();
			}
		}
		else {
			// node is a leaf
			codewords[node->index] = curPath;
			curPath.pop_back();
			st.pop();
		}
	} while (!st.empty());
}