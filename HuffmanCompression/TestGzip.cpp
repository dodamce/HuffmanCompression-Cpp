#include"Zip.h"

void Test() {
	//带压缩路径
	std::string srcFile = "1.txt";
	//压缩后文件名
	std::string detFile = "1-压缩.txt";
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

//测试分析文件
int main() {
	Test();
}