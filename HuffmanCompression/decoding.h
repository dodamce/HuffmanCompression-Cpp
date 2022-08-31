#pragma once

#include"hufftree.h"
#include"charactor.h"

class Decode {
	friend class UnZip;
public:
	Decode(std::string& srcFile) {
		_anylizeFile(srcFile);
		_tree.creatHufTree(_dict);
	}

	//ͨ��key�ҵ���Ӧ�ַ�
	char findchar(std::string& key) {
		for (int i = 0; i < _dict._cout; i++) {
			if (_dict._CharArray[i]._code == key) {
				return _dict._CharArray[i]._character;
			}
		}
		return -1;
	}

	//�����ܹ����ַ���
	int allAlph() {
		int ret = 0;
		for (int i = 0; i < _dict._cout; i++) {
			ret += _dict._CharArray[i]._times;
		}
		return ret;
	}
private:
	CharDict _dict;
	std::string _srcFile;//��ѹ����ļ�����·��
	HufTree _tree;

	//����ѹ���ļ�
	void _redictDict(CharDict& dict, std::string& readLine) {
		assert(readLine.size() > 0);
		Char msg;
		int begin = 0;
		int flag = 0;//��ǻ�ԭ���Ǹ�����
		for (int i = 0; i < readLine.size(); i++) {
			if (readLine[i] != ';') {
				continue;
			}
			else {
				if (flag == 0) {
					msg._character = readLine[0];
				}
				else if (flag == 1) {
					msg._code = readLine.substr(begin, i - begin);
				}
				else if (flag == 2) {
					msg._index = atoi(readLine.substr(begin, i - begin).c_str());
				}
				else {
					msg._times = atoi(readLine.substr(begin, i - begin).c_str());
				}
				begin = i + 1;
				flag++;
			}
		}
		_dict._CharArray.push_back(msg);
		_dict._cout++;
	}
	void _anylizeFile(std::string& srcFile) {
		_srcFile = srcFile;
		std::fstream fpSrc(_srcFile, std::ios::in | std::ios::binary);
		if (!fpSrc.is_open()) {
			printf("DEAD:ѹ���ļ���ʧ");
			return;
		}
		std::string readLine;
		//��ԭ�ֵ�
		while (true) {
			std::getline(fpSrc, readLine);
			if (readLine=="-") {
				//�ֵ��Ѿ�ȫ����ȡ�����
				break;
			}
			_redictDict(_dict,readLine);
		}
		fpSrc.close();
		_TestDict();
		_TestHuffCode();
	}
public:

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
			printf("�ַ�%c�ı���Ϊ%s\n", _dict._CharArray[i]._character, _dict._CharArray[i]._code.c_str());
		}
	}
};