#ifndef REGISTER_H
#define REGISTER_H

#include <iostream>

using namespace std;

//注册文件名
#define AuthorFile "AuthorFile"			

class __declspec(dllexport) Register
{
public:

	Register();
	~Register();

	//注册激活(参数：需要比对的输入码/明码/非加密码）
	int codeRegOK(const string &);		
		// 0 : 注册失败
		// 1 ：输入注册码码注册成功
		// 2 ：文件注册成功
		// 3 ：文件注册不成功

	//取第一个以太网卡MAC地址
	string getMac();

	//取机器码（参数：MAC地址）
	string getACode(const string &);					

	//取注册激活码（参数：机器码ACode）
	string getRCode(const string &);					

private:

	enum DYNASTY
	{
		XIA,							//0第一代授权方式
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
	//第几代授权方式
	int m_thisDynasty;	
	//保存第几代XOR Key
	char *m_strKey[12]; 				

private:
	//判断注册文件中的注册激活码是否正确
	bool fileRegOK();		
	//读取注册文件中的注册激活码
	string readRCodeFile();							
	//将输入的正确注册激活码写入注册文件
	void writeRCodeFile(const string &);					
	//MD5算法
	string MD5(const string &);		
	//XOR简单加密算法
	string SXOR(const string &info, const string &key);		

};

#endif //REGISTER_H