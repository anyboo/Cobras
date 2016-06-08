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

	string getMac();       //取MAC地址（测试用，发布时取消）
	string getRCode();     //取注册码（测试用，发布时取消）

private:

	string m_strApply;
	string m_strMacAddr;

	bool hasKey();
	string CrypDll::readACodeFile();
	void CrypDll::writeACodeFile(const string &);

	string MD5(const string&);

};