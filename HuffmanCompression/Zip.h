//哈夫曼压缩思路:1编码表示数据 2 出现次数多编码短,出现次数少编码长
/*
* 压缩流程
1.分析待压缩文件,获取文件中出现的字符，字符出现次数.将这些数据组织成字典
2.根据字典创建哈夫曼树（核心）
3.根据哈夫曼树获取哈夫曼编码并写入字典中
4.把哈夫曼编码写入文件中
*/

/*
解压过程
1.将文件中的字典读取出来,根据字典生成哈夫曼树
2.按照字节读取文件,按照二进制位分析，将对应的字符写入到解压文件
3.关闭文件
*/
//根据字典创建哈夫曼树
//哈夫曼树节点

#pragma once
#include"encoding.h"
#include"decoding.h"

class GZip {
private:
	Encode _encode;
public:
	GZip(std::string& srcFile)
		:_encode(srcFile)
	{}
	//保存压缩后哈夫曼编码写到文件中
	void ZipFile(const std::string& destPath) {
		//将编码组合成8位的位运算,最后编码不足8位填0即可
		//循环读取源文件字节，找到HuffCode，凑齐8位写入destPath文件中

		std::fstream fpSrc(_encode._srcFile, std::ios::in | std::ios::binary);
		if (!fpSrc.is_open()) {
			printf("DEAD:待压缩文件丢失");
			return;
		}
		std::ofstream fpDst(destPath, std::ios::out | std::ios::app | std::ios::binary);
		if (!fpDst.is_open()) {
			printf("DEAD:目标压缩路径丢失");
			return;
		}

		//将字典写入文件中
		for (int i = 0; i < _encode._dict._cout; i++) {
			Char msg = _encode._dict._CharArray[i];
			fpDst << msg._character << ";" << msg._code << ";" << msg._index << ";" << msg._times << ";\n";
		}
		fpDst << "-\n";//字典结束的标志

		char ch = 0;

		//判断是否读取完毕
		bool isEnd = false;
		//记录处理的哈夫曼编码位置
		size_t indexHufCode = 0;
		//记录处理到第几位了
		size_t indexWrite = 0;
		//记录字符的哈夫曼编码
		std::string HufCode;

		while (true) {
			if (indexHufCode == HufCode.size()) {
				fpSrc.read(&ch, 1);
				//获取某个字符的哈夫曼编码
				//std::cout << ch << ":" << _getHufCharCode(ch) << std::endl;
				HufCode = _encode._getHufCharCode(ch);
				indexHufCode = 0;
			}

			indexWrite = 0;

			//8字节写入一次
			int bite = 0;

			//储存要写入的数据
			char write = 0;

			while (bite < 8) {
				if (indexHufCode < HufCode.size()) {
					if (HufCode[indexHufCode] == '0') {
						//设置对应位为0
						write &= ~(1 << (7 - indexWrite));
					}
					else if (HufCode[indexHufCode] == '1') {
						//设置对应位为1
						write |= (1 << (7 - indexWrite));
					}
					bite++;
					indexWrite++;
					indexHufCode++;
				}
				else {
					//这个字符已经读取完毕了
					if (fpSrc.read(&ch, 1)) {
						HufCode = _encode._getHufCharCode(ch);
						indexHufCode = 0;
					}
					else {
						//文件已经读取完毕了
						isEnd = true;
						break;
					}
				}
			}
			fpDst << write;

			//测试写入的数据是否正确,一个字节8比特位,打印每个比特位
			char test_write = write;
			for (int i = 0; i < 8; i++) {
				//0x80 :1000 0000
				if ((test_write & 0x80) == 0x80) {
					printf("%d", 1);
				}
				else {
					printf("%d", 0);
				}
				test_write <<= 1;
			}

			printf("\n");

			if (isEnd == true) {
				break;
			}
		}
		fpSrc.close();
		fpDst.close();
	}
};

class UnZip {
private:
	Decode _decode;
public:
	UnZip(std::string& destPath)
		:_decode(destPath)
	{}
	//解压缩文件
	void UnzipFile(const std::string& destPath) {
		/*_decode._anylizeFile(destPath);
		_decode._TestDict();
		_decode._TestHuffCode();*/
		//定位到正文部分
		FILE* fp = fopen(destPath.c_str(), "rb");
		char ch = 0;
		int readNum = 0;
		while (true) {
			readNum = fread(&ch, 1, 1, fp);
			assert(readNum != 0);
			if (ch == '-') {
				break;
			}
		}
		//把换行符读取掉
		readNum = fread(&ch, 1, 1, fp);
		assert(readNum == 1);


		//定位到正文了,读取到正文结束
		HufTree tree = _decode._tree;
		TreeNode* node = tree._root;

		//还原文件
		FILE* redict_fp = fopen("还原文件.txt", "w");

		std::string key;
		bool isFinish = false;//判断是否读取完毕

		int allAlph = _decode.allAlph();
		while (true) {
			readNum = fread(&ch, 1, 1, fp);
			if (readNum == 0) {
				break;
			}
			int num = ch;
			for (int i = 0; i < 8; i++) {
				//获取每一位
				if ((ch & 0x80) == 0x80) {
					node = node->_left;
					key.push_back('1');
				}
				else if ((ch & 0x80) == 0) {
					node = node->_right;
					key.push_back('0');
				}
				ch <<= 1;
				if (node->_isLeaf()) {
					char write = _decode.findchar(key);
					if (write == -1) {
						break;
					}
					fwrite(&write, 1, 1, redict_fp);
					node = tree._root;
					key.clear();
					allAlph -= 1;
					if (allAlph == 0) {
						isFinish = true;
						break;
					}
				}
			}
			if (isFinish == true) {
				break;
			}
		}
		fclose(redict_fp);
	}
};