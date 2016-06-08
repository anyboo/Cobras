#define AUTHOR __declspec(dllexport)

#include <iostream>

using namespace std;

#define AuthorFile "AuthorFile"

class AUTHOR CrypDll
{
public:

	CrypDll();

	bool codeRegOK(const string &);
	string getApllyCode();

	string getMac();       //ȡMAC��ַ�������ã�����ʱȡ����
	string getRCode();     //ȡע���루�����ã�����ʱȡ����

private:

	string m_strApply;
	string m_strMacAddr;

	bool hasKey();
	string CrypDll::readACodeFile();
	void CrypDll::writeACodeFile(const string &);

	string MD5(const string&);

};