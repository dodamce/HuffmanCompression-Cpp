//������ѹ��˼·:1�����ʾ���� 2 ���ִ���������,���ִ����ٱ��볤
/*
* ѹ������
1.������ѹ���ļ�,��ȡ�ļ��г��ֵ��ַ����ַ����ִ���.����Щ������֯���ֵ�
2.�����ֵ䴴���������������ģ�
3.���ݹ���������ȡ���������벢д���ֵ���
4.�ѹ���������д���ļ���
*/

/*
��ѹ����
1.���ļ��е��ֵ��ȡ����,�����ֵ����ɹ�������
2.�����ֽڶ�ȡ�ļ�,���ն�����λ����������Ӧ���ַ�д�뵽��ѹ�ļ�
3.�ر��ļ�
*/
//�����ֵ䴴����������
//���������ڵ�

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
	//����ѹ�������������д���ļ���
	void ZipFile(const std::string& destPath) {
		//��������ϳ�8λ��λ����,�����벻��8λ��0����
		//ѭ����ȡԴ�ļ��ֽڣ��ҵ�HuffCode������8λд��destPath�ļ���

		std::fstream fpSrc(_encode._srcFile, std::ios::in | std::ios::binary);
		if (!fpSrc.is_open()) {
			printf("DEAD:��ѹ���ļ���ʧ");
			return;
		}
		std::ofstream fpDst(destPath, std::ios::out | std::ios::app | std::ios::binary);
		if (!fpDst.is_open()) {
			printf("DEAD:Ŀ��ѹ��·����ʧ");
			return;
		}

		//���ֵ�д���ļ���
		for (int i = 0; i < _encode._dict._cout; i++) {
			Char msg = _encode._dict._CharArray[i];
			fpDst << msg._character << ";" << msg._code << ";" << msg._index << ";" << msg._times << ";\n";
		}
		fpDst << "-\n";//�ֵ�����ı�־

		char ch = 0;

		//�ж��Ƿ��ȡ���
		bool isEnd = false;
		//��¼����Ĺ���������λ��
		size_t indexHufCode = 0;
		//��¼�����ڼ�λ��
		size_t indexWrite = 0;
		//��¼�ַ��Ĺ���������
		std::string HufCode;

		while (true) {
			if (indexHufCode == HufCode.size()) {
				fpSrc.read(&ch, 1);
				//��ȡĳ���ַ��Ĺ���������
				//std::cout << ch << ":" << _getHufCharCode(ch) << std::endl;
				HufCode = _encode._getHufCharCode(ch);
				indexHufCode = 0;
			}

			indexWrite = 0;

			//8�ֽ�д��һ��
			int bite = 0;

			//����Ҫд�������
			char write = 0;

			while (bite < 8) {
				if (indexHufCode < HufCode.size()) {
					if (HufCode[indexHufCode] == '0') {
						//���ö�ӦλΪ0
						write &= ~(1 << (7 - indexWrite));
					}
					else if (HufCode[indexHufCode] == '1') {
						//���ö�ӦλΪ1
						write |= (1 << (7 - indexWrite));
					}
					bite++;
					indexWrite++;
					indexHufCode++;
				}
				else {
					//����ַ��Ѿ���ȡ�����
					if (fpSrc.read(&ch, 1)) {
						HufCode = _encode._getHufCharCode(ch);
						indexHufCode = 0;
					}
					else {
						//�ļ��Ѿ���ȡ�����
						isEnd = true;
						break;
					}
				}
			}
			fpDst << write;

			//����д��������Ƿ���ȷ,һ���ֽ�8����λ,��ӡÿ������λ
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
	//��ѹ���ļ�
	void UnzipFile(const std::string& destPath) {
		/*_decode._anylizeFile(destPath);
		_decode._TestDict();
		_decode._TestHuffCode();*/
		//��λ�����Ĳ���
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
		//�ѻ��з���ȡ��
		readNum = fread(&ch, 1, 1, fp);
		assert(readNum == 1);


		//��λ��������,��ȡ�����Ľ���
		HufTree tree = _decode._tree;
		TreeNode* node = tree._root;

		//��ԭ�ļ�
		FILE* redict_fp = fopen("��ԭ�ļ�.txt", "w");

		std::string key;
		bool isFinish = false;//�ж��Ƿ��ȡ���

		int allAlph = _decode.allAlph();
		while (true) {
			readNum = fread(&ch, 1, 1, fp);
			if (readNum == 0) {
				break;
			}
			int num = ch;
			for (int i = 0; i < 8; i++) {
				//��ȡÿһλ
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