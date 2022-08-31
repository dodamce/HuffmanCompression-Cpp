#pragma once

#include"common.h"
//字符结构体
struct Char {
	char _character;//字符
	unsigned int _times;//字符出现次数
	std::string _code;//哈夫曼编码
	int _index;//这个字符在字符字典中的位置
	Char() = default;
	Char(char ch, int times, int index = -1)
		:_character(ch), _times(times), _index(index)
	{}
};

//字符索引，保存的是每个字符的编码，出现的字符
struct CharDict {
	int _cout = 0;//出现过的字符个数,最大MAXCHAR个
	std::vector<Char>_CharArray;//每个字符数组
};