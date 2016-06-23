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

private:

	enum DYNASTY
	{
		XIA,							//0��һ����Ȩ��ʽ
		SHANG,
		ZHOU,
		QIN,
		HAN,
		SANGUO,
		JING,
		SUI,
		TANG,
		SONG,
		YUAN,
		MING,
		QING,
	};
	//�ڼ�����Ȩ��ʽ
	int m_thisDynasty;	
	//����ڼ���XOR Key
	char *m_strKey[12]; 				

private:
	//�ж�ע���ļ��е�ע�ἤ�����Ƿ���ȷ
	bool fileRegOK();		
	//��ȡע���ļ��е�ע�ἤ����
	string readRCodeFile();							
	//���������ȷע�ἤ����д��ע���ļ�
	void writeRCodeFile(const string &);					
	//MD5�㷨
	string MD5(const string &);		
	//XOR�򵥼����㷨
	string SXOR(const string &info, const string &key);		

};

#endif //REGISTER_H