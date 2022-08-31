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

	//通过key找到对应字符
	char findchar(std::string& key) {
		for (int i = 0; i < _dict._cout; i++) {
			if (_dict._CharArray[i]._code == key) {
				return _dict._CharArray[i]._character;
			}
		}
		return -1;
	}

	//返回总共的字符数
	int allAlph() {
		int ret = 0;
		for (int i = 0; i < _dict._cout; i++) {
			ret += _dict._CharArray[i]._times;
		}
		return ret;
	}
private:
	CharDict _dict;
	std::string _srcFile;//解压后的文件保存路径
	HufTree _tree;

	//分析压缩文件
	void _redictDict(CharDict& dict, std::string& readLine) {
		assert(readLine.size() > 0);
		Char msg;
		int begin = 0;
		int flag = 0;//标记还原到那个属性
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
			printf("DEAD:压缩文件丢失");
			return;
		}
		std::string readLine;
		//还原字典
		while (true) {
			std::getline(fpSrc, readLine);
			if (readLine=="-") {
				//字典已经全部读取完毕了
				break;
			}
			_redictDict(_dict,readLine);
		}
		fpSrc.close();
		_TestDict();
		_TestHuffCode();
	}
public:

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
			printf("字符%c的编码为%s\n", _dict._CharArray[i]._character, _dict._CharArray[i]._code.c_str());
		}
	}
};