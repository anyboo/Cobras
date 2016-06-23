//register.cpp
#include "register.h"
#include "md5.h"

#include <winsock2.h> 
#include "iphlpapi.h" 

#pragma comment(lib, "iphlpapi.lib") 

using namespace std;	  

Register::Register()
{
	m_thisDynasty = XIA;		//0第一代授权方式
	m_strKey[m_thisDynasty] = "XIA";

}

Register::~Register()
{

}

//两个取码函数，----------------------------------------------
//20160622修正：考虑到客户注册时提供ACODE来需要生成RCODE，所以ACODE和RCODE都修正为带参数

string Register::getACode(const string &strMac)
{
	return MD5(strMac);	 
}

string Register::getRCode(const string &aCode)
{
	string strMD5;
	switch (m_thisDynasty)
	{
		//第一代授权方式
		case XIA:
			strMD5 = "dwsfdajfaiierueiur";
			strMD5.append(aCode);
			strMD5.append("adkfjaofdlasdfl");
			break;
		
		default:
			strMD5 = "dwsfdajfaiierueiur";
			strMD5.append(aCode);
			strMD5.append("adkfjaofdlasdfl");
			break;

	}
	
	return MD5(strMD5);
}

//判断注册文件中的注册激活码是否正确
bool Register::fileRegOK()
{
	bool fSucess = false;

	//验证文件记录码和计算出来的码是否一致？
	string m_strRCode = readRCodeFile();
	string scode = getRCode(getACode(getMac()));

	if (scode == m_strRCode)
	{
			fSucess = true;
	}

	return fSucess;
}

//读取注册文件中的注册激活码
string Register::readRCodeFile()
{
	char buffer[256] = "";
	fstream fin;
	fin.open(AuthorFile, ios::in);

	if (fin.is_open())
	{
		fin.getline(buffer, 100);
		fin.close();
	}
	string fstr(buffer);
	string str = SXOR(fstr, m_strKey[m_thisDynasty]);

	return str;
}

//将输入的正确注册激活码写入注册文件
void Register::writeRCodeFile(const string &strAuthy)
{
	fstream fout;
	fout.open(AuthorFile, ios::out);

	if (fout.is_open())
	{ 	
		string str = SXOR(strAuthy, m_strKey[m_thisDynasty]);
		fout << str;
		fout.close();

	}
}

//注册激活
int Register::codeRegOK(const string &strAuthy)
{
	string strMac = getMac();
	if (strMac.empty())
		return 0;;

	int rSuccess = 0;

	string scode = getRCode(getACode(strMac));

	//判断输入注册激活码是否正确
	if (scode == strAuthy)
	{
		rSuccess = 1;
		writeRCodeFile(strAuthy);
	}
	else
	//输入码不正确则试着读取注册文件中的注册激活码
	{
		//正确2 错误3
		rSuccess = (fileRegOK()) ? 2 : 3;

	}

	return rSuccess;
}

//获取MAC地址，最后取第一个以太网卡MAC地址
string Register::getMac()
{
	//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//得到结构体大小,用于GetAdaptersInfo参数
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);

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

	int iMacN = 0;				//网卡数量计数器
	bool ErrorETHERNET;			//非ETHERNET网卡（如Bluetooth）过滤
	string MacAddress[10];
	char buffer[20] = "";
	string sMacA;		
	string strDynastyAdd;

	if (ERROR_SUCCESS == nRel)
	{
		//输出网卡信息
		//可能有多网卡,因此通过循环去判断	   		
		while (pIpAdapterInfo)
		{
			switch (pIpAdapterInfo->Type)
			{
			case MIB_IF_TYPE_ETHERNET:
				//20160622修正：过滤Bluetooth网卡
				ErrorETHERNET = false;
				if (!strstr(pIpAdapterInfo->Description, "PCI") || strstr(pIpAdapterInfo->Description, "Bluetooth"))
				{
					ErrorETHERNET = true;
					break;
				}
				//修正结束
				MacAddress[iMacN].clear();
				sMacA = "";

				//可以在Mac地址段间增加字符，比如“-”“."“:”等
				switch (m_thisDynasty)
				{
					case XIA:					  //第一代授权方式
						strDynastyAdd = "";
						break;

					default:
						strDynastyAdd = "-";
						break;
				}

				for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++)
				{
					sprintf_s(buffer, "%02X", pIpAdapterInfo->Address[i]);
					string str(buffer);
					sMacA.append(str);

					if (i < (pIpAdapterInfo->AddressLength - 1)) 
						sMacA.append(strDynastyAdd);				
					
				}

				MacAddress[iMacN].append(sMacA);

				break;

			case MIB_IF_TYPE_OTHER:
			case MIB_IF_TYPE_TOKENRING:
			case MIB_IF_TYPE_FDDI:
			case MIB_IF_TYPE_PPP:
			case MIB_IF_TYPE_LOOPBACK:
			case MIB_IF_TYPE_SLIP:
			default:
				break;
			}

			pIpAdapterInfo = pIpAdapterInfo->Next;
			if (iMacN < 9 && !ErrorETHERNET) iMacN++;					//只取最大网卡数10
		}

	}
	//释放内存空间
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}
	//返回第一个以太网卡MAC地址
	return MacAddress[0];
}

//-------------------------------------------------
//MD5算法
string Register::MD5(const string &INPUT_STRING)
{
	return MD5::MD5(INPUT_STRING).toString();

}
//XOR简单跟斗加密解密算法
string Register::SXOR(const string &info, const string &key)
{

	if (info.empty() || key.empty())
		return "";

	string result;

	_Uint32t i = 0;
	_Uint32t j = 0;

	for (; i < info.size(); ++i)
	{
		result.push_back(static_cast<unsigned char>(info[i] ^ key[j]));
		j = (++j) % (key.length());
	}

	return result;

}



