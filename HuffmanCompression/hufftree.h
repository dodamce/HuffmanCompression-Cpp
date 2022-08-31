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
		//���������ַ��ڵ�
		std::vector<Node*>NodeArray(dict._cout);
		Node* node = nullptr;
		for (int i = 0; i < dict._cout; i++) {
			NodeArray[i] = new Node(dict._CharArray[i]);
			//�����ַ����ֵ��е��±�
			NodeArray[i]->_data._index = i;
			dict._CharArray[i]._index = i;
		}
		
		//ѭ���������������ڵ� 
		//���ֻ��һ���ڵ�
		if (dict._cout == 1) {
			_root = NodeArray[0];
		}
		else {

			//�ҳ��ִ�����С�ĺ͵ڶ�С�Ľڵ�
			for (int time = 0; time < dict._cout - 1; time++) {
				int minIndex = 0;
				int minSecIndex = 0;
				while (NodeArray[minIndex] == nullptr) {
					minIndex++;
				}
				for (int i = 0; i < dict._cout; i++) {
					//ʹ�ø��ڵ��������ڵ㣬�滻��С�Ľڵ㣬ɾ���ڶ�С�Ľڵ�
					if (NodeArray[i] != nullptr && NodeArray[i]->_data._times < NodeArray[minIndex]->_data._times) {
						minIndex = i;
					}
				}
				//�Ҵ�Сֵ
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
				/*printf("���ִ�����С���ַ���%c,���ִ���%d\n",NodeArray[minIndex]->_data._character, NodeArray[minIndex]->_data._times);
				printf("���ִ������ٵ��ַ���%c,���ִ���Ϊ%d\n", NodeArray[minSecIndex]->_data._character, NodeArray[minSecIndex]->_data._times);
				printf("============\n");*/
				//�����սڵ�
				_root = new Node(Char(0, NodeArray[minIndex]->_data._times + NodeArray[minSecIndex]->_data._times));

				//���ӽڵ�
				NodeArray[minIndex]->_parent = _root;
				NodeArray[minSecIndex]->_parent = _root;
				_root->_left = NodeArray[minIndex];
				_root->_right = NodeArray[minSecIndex];

				//����������Сλ�ã��ڶ�Сλ���ƿ�
				NodeArray[minIndex] = _root;
				NodeArray[minSecIndex] = nullptr;
			}
		}
	}
};