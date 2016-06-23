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

};

#endif //REGISTER_H