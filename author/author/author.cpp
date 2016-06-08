#include "author.h"

#include <stdio.h> 
#include <winsock2.h> 
#include "iphlpapi.h" 

#include <fstream>  

#include <iostream>  

#pragma comment(lib, "iphlpapi.lib") 

using namespace std;	  

#include <sstream>
#include "Poco/MD5Engine.h"
#include "Poco/DigestStream.h"
#include "Poco/StreamCopier.h"

using Poco::DigestEngine;
using Poco::MD5Engine;
using Poco::DigestOutputStream;
using Poco::StreamCopier;

CrypDll::CrypDll()
{
	m_strMacAddr = getMac();
	m_strApply = getApllyCode();

}

//两个取码函数，----------------------------------------------

string CrypDll::getApllyCode()
{
	return MD5(m_strMacAddr);	 
}

string CrypDll::getRCode()
{
	string md5 = "dwsfdajfaiierueiur";
	md5.append(m_strApply);
	md5.append("adkfjaofdlasdfl");

	return MD5(md5);
}

bool CrypDll::hasKey()
{
	bool fSucess = false;

	//验证文件记录码和计算出来的码是否一致？
	string m_strRCode = readACodeFile();
	string scode = getRCode();

	cout << "scode = " << scode << endl;
	cout << "MD5   = " << m_strRCode << endl;

	if (scode == m_strRCode)
	{
			fSucess = true;
	}

	return fSucess;
}

string CrypDll::readACodeFile()
{
	//string fstr;
	char buffer[256] = "";
	fstream fin;
	fin.open(AuthorFile, ios::in);

	//cout << "Obuffer = " << buffer << endl;

	if (fin.is_open())
	{
		//cout << "File open error!\n";
		fin.getline(buffer, 100);
		//cout << "Rbuffer = " << buffer << endl;
		fin.close();
	}
	//cout << buffer;
	string fstr(buffer);

	cout << "Read = " << fstr << endl;
	return fstr;
}


void CrypDll::writeACodeFile(const string &strAuthy)
{
	fstream fout;
	fout.open(AuthorFile, ios::out);

	if (fout.is_open())
	{
		//cout << "File open error!\n";	  	
		fout << strAuthy;
		fout.close();
		cout << "Write AFile OK!" << endl;
	}
}


bool CrypDll::codeRegOK(const string &strAuthy)
{
	bool rSuccess = false;

	string scode = getRCode();
	if (scode == strAuthy)
	{
		rSuccess = true;
		writeACodeFile(strAuthy);
	}
	else
	{
		cout << "From File!" << endl;

		if (hasKey())
		{
			rSuccess = true;
		}
	}

	return rSuccess;
}


string CrypDll::getMac()
{
	//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//得到结构体大小,用于GetAdaptersInfo参数
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	//记录网卡数量
	int netCardNum = 0;
	//记录每张网卡上的IP地址数量
	int IPnumPerNetCard = 0;
	string MacAddress;
	char buffer[20] = "";
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		//如果函数返回的是ERROR_BUFFER_OVERFLOW
		//则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
		//这也是说明为什么stSize既是一个输入量也是一个输出量
		//释放原来的内存空间
		delete pIpAdapterInfo;
		//重新申请内存空间用来存储所有网卡信息
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	if (ERROR_SUCCESS == nRel)
	{
		//输出网卡信息
		//可能有多网卡,因此通过循环去判断
		while (pIpAdapterInfo)
		{
			//cout<<"网卡数量："<<++netCardNum<<endl;
			//cout<<"网卡名称："<<pIpAdapterInfo->AdapterName<<endl;
			//cout<<"网卡描述："<<pIpAdapterInfo->Description<<endl;

			switch (pIpAdapterInfo->Type)
			{
			case MIB_IF_TYPE_OTHER:
				//cout<<"网卡类型："<<"OTHER"<<endl;
				break;

			case MIB_IF_TYPE_ETHERNET:
				//cout<<"网卡类型："<<"ETHERNET"<<endl;
				//cout << "网卡MAC地址：";

				MacAddress = "";

				for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++)
				{
					sprintf_s(buffer, "%02X", pIpAdapterInfo->Address[i]);
					string str(buffer);
					MacAddress.append(str);
					/*
					if (i < pIpAdapterInfo->AddressLength - 1)
					{
						//MacAddress.append("-");
						printf("%02X-", pIpAdapterInfo->Address[i]);
					}
					else
					{
						printf("%02X\n", pIpAdapterInfo->Address[i]);
					}
					*/
				}
				break;

			case MIB_IF_TYPE_TOKENRING:
				//cout<<"网卡类型："<<"TOKENRING"<<endl;
				break;

			case MIB_IF_TYPE_FDDI:
				//cout<<"网卡类型："<<"FDDI"<<endl;
				break;

			case MIB_IF_TYPE_PPP:
				//printf("PP\n");
				//cout<<"网卡类型："<<"PPP"<<endl;
				break;

			case MIB_IF_TYPE_LOOPBACK:
				//cout<<"网卡类型："<<"LOOPBACK"<<endl;
				break;

			case MIB_IF_TYPE_SLIP:
				//cout<<"网卡类型："<<"SLIP"<<endl;
				break;

			default:
				break;
			}

			pIpAdapterInfo = pIpAdapterInfo->Next;
			//printf("%s\n", buffer);
			//cout << MacAddress;
		}

	}
	//释放内存空间
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}

	return MacAddress;
}

//-------------------------------------------------
//POCO库的MD5算法
string CrypDll::MD5(const string &INPUT_STRING)
{
	MD5Engine omd5;
	DigestOutputStream dos(omd5);

	istringstream istr(INPUT_STRING);

	StreamCopier::copyStream(istr, dos);
	dos.close();

	return DigestEngine::digestToHex(omd5.digest());

}



