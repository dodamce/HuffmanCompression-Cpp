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
	//����ַ��ǳ��ֹ�,�����ַ������ֵ��λ��
public:
	Encode(std::string&srcFile)
		:_srcFile(srcFile)
	{
		_anylizeFile();
		_tree.creatHufTree(_dict);
		_creatHuffCode();
	}
private:
	//�����ֵ��Ƿ���ȷ
	void _TestDict() {
		printf("����%d���ַ�\n", _dict._cout);
		for (int i = 0; i < _dict._cout; i++) {
			printf("�ַ�%c������%d��\n", _dict._CharArray[i]._character, _dict._CharArray[i]._times);
		}
	}
	//�����ַ��Ĺ���������
	void _TestHuffCode() {
		for (int i = 0; i < _dict._cout; i++) {
			printf("�ַ�%c�ı���Ϊ%s\n", _dict._CharArray[i]._character,_dict._CharArray[i]._code.c_str());
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
	//�����ļ�
	void _anylizeFile() {
		FILE* fp = fopen(_srcFile.c_str(), "rb");
		if (fp == nullptr) {
			printf("%s�ļ���ʧ��", _srcFile.c_str());
		}
		assert(fp != nullptr);
		//ѭ�����ֽڶ�ȡ�ļ�����
		char ch = ' ';
		int readNum = 0;
		while (true) {
			readNum = fread(&ch, 1, 1, fp);
			if (readNum != 1) {//��ȡ���
				break;
			}
			//ͳ���ַ����ִ���
			int pos = _findChar(ch);
			if (pos == -1) {
				//û�г��ֹ�
				_dict._cout++;
				_dict._CharArray.push_back(Char(ch, 1));
			}
			else {
				//���ֹ�
				_dict._CharArray[pos]._times++;
			}
		}
		fclose(fp);
	}

	//��������������,��������������������Ҷ�ӽڵ㣬����������Ϊ1������������Ϊ0
	void _setHuffCode(TreeNode* root) {
		//������,�ж��������ӻ����Һ���,���ӱ���Ϊ1���Һ��ӱ���Ϊ0������ٷ�תһ�����鼴��
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
		//�޸��ֵ�ֵ
		_dict._CharArray[root->_data._index]._code = code;
	}
	void _creatHuffCode() {
		_creatHuffCode(_tree._root);
	}
	void _creatHuffCode(TreeNode* root) {
		if (root != nullptr) {
			if (root->_isLeaf()) {
				//������Ҷ�ӽڵ㣬���ù���������
				_setHuffCode(root);
			}
			else {
				//����������
				_creatHuffCode(root->_left);
				//����������
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
