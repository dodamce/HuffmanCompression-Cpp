#include"Zip.h"

void Test() {
	//��ѹ��·��
	std::string srcFile = "1.txt";
	//ѹ�����ļ���
	std::string detFile = "1-ѹ��.txt";
	GZip test(srcFile);
	//test._anylizeFile();
	/*test._TestDict();*/ 
	/*HufTree tree = test._getTreeTest();
	CharDict dict = test._getDict_Test();
	test._TestHuffCode();*/
	test.ZipFile(detFile);
	UnZip test2(detFile);
	test2.UnzipFile(detFile);
}

//���Է����ļ�
int main() {
	Test();
}