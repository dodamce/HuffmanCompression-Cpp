#pragma once

#include"common.h"
//�ַ��ṹ��
struct Char {
	char _character;//�ַ�
	unsigned int _times;//�ַ����ִ���
	std::string _code;//����������
	int _index;//����ַ����ַ��ֵ��е�λ��
	Char() = default;
	Char(char ch, int times, int index = -1)
		:_character(ch), _times(times), _index(index)
	{}
};

//�ַ��������������ÿ���ַ��ı��룬���ֵ��ַ�
struct CharDict {
	int _cout = 0;//���ֹ����ַ�����,���MAXCHAR��
	std::vector<Char>_CharArray;//ÿ���ַ�����
};