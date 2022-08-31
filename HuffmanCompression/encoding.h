#pragma once

#include"common.h"
#include"charactor.h"
#include"hufftree.h"
#include"Zip.h"

class Encode {
	friend class GZip;
private:
	HufTree _tree;
	std::string _srcFile;
	CharDict _dict;
	//检测字符是出现过,返回字符所在字典的位置
public:
	Encode(std::string&srcFile)
		:_srcFile(srcFile)
	{
		_anylizeFile();
		_tree.creatHufTree(_dict);
		_creatHuffCode();
	}
private:
	//测试字典是否正确
	void _TestDict() {
		printf("共有%d个字符\n", _dict._cout);
		for (int i = 0; i < _dict._cout; i++) {
			printf("字符%c出现了%d次\n", _dict._CharArray[i]._character, _dict._CharArray[i]._times);
		}
	}
	//测试字符的哈夫曼编码
	void _TestHuffCode() {
		for (int i = 0; i < _dict._cout; i++) {
			printf("字符%c的编码为%s\n", _dict._CharArray[i]._character,_dict._CharArray[i]._code.c_str());
		}
	}
	int _findChar(const char ch) {
		for (int i = 0; i < _dict._cout; i++) {
			if (_dict._CharArray[i]._character == ch) {
				return i;
			}
		}
		return -1;
	}
	//分析文件
	void _anylizeFile() {
		FILE* fp = fopen(_srcFile.c_str(), "rb");
		if (fp == nullptr) {
			printf("%s文件打开失败", _srcFile.c_str());
		}
		assert(fp != nullptr);
		//循环按字节读取文件数据
		char ch = ' ';
		int readNum = 0;
		while (true) {
			readNum = fread(&ch, 1, 1, fp);
			if (readNum != 1) {//读取完毕
				break;
			}
			//统计字符出现次数
			int pos = _findChar(ch);
			if (pos == -1) {
				//没有出现过
				_dict._cout++;
				_dict._CharArray.push_back(Char(ch, 1));
			}
			else {
				//出现过
				_dict._CharArray[pos]._times++;
			}
		}
		fclose(fp);
	}

	//创建哈夫曼编码,遍历哈夫曼树，遍历到叶子节点，左子树设置为1，右子树设置为0
	void _setHuffCode(TreeNode* root) {
		//向上走,判断其是左孩子还是右孩子,左孩子编码为1，右孩子编码为0。最后再反转一下数组即可
		TreeNode* node = root;
		std::string& code = root->_data._code;
		while (node->_parent != nullptr) {
			if (node == node->_parent->_left) {
				code.push_back('1');
			}
			else {
				code.push_back('0');
			}
			node = node->_parent;
		}
		std::reverse(code.begin(), code.end());
		//修改字典值
		_dict._CharArray[root->_data._index]._code = code;
	}
	void _creatHuffCode() {
		_creatHuffCode(_tree._root);
	}
	void _creatHuffCode(TreeNode* root) {
		if (root != nullptr) {
			if (root->_isLeaf()) {
				//遍历到叶子节点，设置哈夫曼编码
				_setHuffCode(root);
			}
			else {
				//遍历左子树
				_creatHuffCode(root->_left);
				//遍历右子树
				_creatHuffCode(root->_right);
			}
		}
	}
	std::string _getHufCharCode(char ch) {
		for (int i = 0; i < _dict._cout; i++) {
			Char msg = _dict._CharArray[i];
			if (msg._character== ch) {
				return msg._code;
			}
		}
		return "-1";
	}
};
