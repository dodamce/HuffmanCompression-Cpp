#pragma once
#include"common.h"
#include"charactor.h"

struct TreeNode {
	Char _data;
	TreeNode* _left = nullptr;
	TreeNode* _right = nullptr;
	TreeNode* _parent = nullptr;
	TreeNode(const Char&msg):
		_data(msg)
	{}
	bool _isLeaf() {
		return (_left == nullptr) && (_right == nullptr);
	}
};

class HufTree {
	typedef TreeNode Node;
	friend class Encode;
	friend class UnZip;
private:
	Node* _root = nullptr;
public:

	void creatHufTree(CharDict& dict) {
		//储存所有字符节点
		std::vector<Node*>NodeArray(dict._cout);
		Node* node = nullptr;
		for (int i = 0; i < dict._cout; i++) {
			NodeArray[i] = new Node(dict._CharArray[i]);
			//设置字符在字典中的下标
			NodeArray[i]->_data._index = i;
			dict._CharArray[i]._index = i;
		}
		
		//循环创建哈夫曼树节点 
		//如果只有一个节点
		if (dict._cout == 1) {
			_root = NodeArray[0];
		}
		else {

			//找出现次数最小的和第二小的节点
			for (int time = 0; time < dict._cout - 1; time++) {
				int minIndex = 0;
				int minSecIndex = 0;
				while (NodeArray[minIndex] == nullptr) {
					minIndex++;
				}
				for (int i = 0; i < dict._cout; i++) {
					//使用父节点结合两个节点，替换最小的节点，删除第二小的节点
					if (NodeArray[i] != nullptr && NodeArray[i]->_data._times < NodeArray[minIndex]->_data._times) {
						minIndex = i;
					}
				}
				//找次小值
				while (NodeArray[minSecIndex] == nullptr || minIndex == minSecIndex) {
					minSecIndex++;
				}
				for (int i = 0; i < dict._cout; i++) {
					if (i != minIndex) {
						if (NodeArray[i] != nullptr && NodeArray[i]->_data._times < NodeArray[minSecIndex]->_data._times) {
							minSecIndex = i;
						}
					}
				}
				/*printf("出现次数最小的字符是%c,出现次数%d\n",NodeArray[minIndex]->_data._character, NodeArray[minIndex]->_data._times);
				printf("出现次数次少的字符是%c,出现次数为%d\n", NodeArray[minSecIndex]->_data._character, NodeArray[minSecIndex]->_data._times);
				printf("============\n");*/
				//创建空节点
				_root = new Node(Char(0, NodeArray[minIndex]->_data._times + NodeArray[minSecIndex]->_data._times));

				//链接节点
				NodeArray[minIndex]->_parent = _root;
				NodeArray[minSecIndex]->_parent = _root;
				_root->_left = NodeArray[minIndex];
				_root->_right = NodeArray[minSecIndex];

				//覆盖数组最小位置，第二小位置制空
				NodeArray[minIndex] = _root;
				NodeArray[minSecIndex] = nullptr;
			}
		}
	}
};