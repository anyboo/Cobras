#ifndef REGISTER_H
#define REGISTER_H

#include <iostream>

using namespace std;

//ע���ļ���
#define AuthorFile "AuthorFile"			

class __declspec(dllexport) Register
{
public:

	Register();
	~Register();

	//ע�ἤ��(��������Ҫ�ȶԵ�������/����/�Ǽ����룩
	int codeRegOK(const string &);		
		// 0 : ע��ʧ��
		// 1 ������ע������ע��ɹ�
		// 2 ���ļ�ע��ɹ�
		// 3 ���ļ�ע�᲻�ɹ�

	//ȡ��һ����̫����MAC��ַ
	string getMac();

	//ȡ�����루������MAC��ַ��
	string getACode(const string &);					

	//ȡע�ἤ���루������������ACode��
	string getRCode(const string &);					

};

#endif //REGISTER_H