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

//����ȡ�뺯����----------------------------------------------

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

	//��֤�ļ���¼��ͼ�����������Ƿ�һ�£�
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
	//PIP_ADAPTER_INFO�ṹ��ָ��洢����������Ϣ
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//�õ��ṹ���С,����GetAdaptersInfo����
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//����GetAdaptersInfo����,���pIpAdapterInfoָ�����;����stSize��������һ��������Ҳ��һ�������
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	//��¼��������
	int netCardNum = 0;
	//��¼ÿ�������ϵ�IP��ַ����
	int IPnumPerNetCard = 0;
	string MacAddress;
	char buffer[20] = "";
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		//����������ص���ERROR_BUFFER_OVERFLOW
		//��˵��GetAdaptersInfo�������ݵ��ڴ�ռ䲻��,ͬʱ�䴫��stSize,��ʾ��Ҫ�Ŀռ��С
		//��Ҳ��˵��ΪʲôstSize����һ��������Ҳ��һ�������
		//�ͷ�ԭ�����ڴ�ռ�
		delete pIpAdapterInfo;
		//���������ڴ�ռ������洢����������Ϣ
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//�ٴε���GetAdaptersInfo����,���pIpAdapterInfoָ�����
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	if (ERROR_SUCCESS == nRel)
	{
		//���������Ϣ
		//�����ж�����,���ͨ��ѭ��ȥ�ж�
		while (pIpAdapterInfo)
		{
			//cout<<"����������"<<++netCardNum<<endl;
			//cout<<"�������ƣ�"<<pIpAdapterInfo->AdapterName<<endl;
			//cout<<"����������"<<pIpAdapterInfo->Description<<endl;

			switch (pIpAdapterInfo->Type)
			{
			case MIB_IF_TYPE_OTHER:
				//cout<<"�������ͣ�"<<"OTHER"<<endl;
				break;

			case MIB_IF_TYPE_ETHERNET:
				//cout<<"�������ͣ�"<<"ETHERNET"<<endl;
				//cout << "����MAC��ַ��";

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
				//cout<<"�������ͣ�"<<"TOKENRING"<<endl;
				break;

			case MIB_IF_TYPE_FDDI:
				//cout<<"�������ͣ�"<<"FDDI"<<endl;
				break;

			case MIB_IF_TYPE_PPP:
				//printf("PP\n");
				//cout<<"�������ͣ�"<<"PPP"<<endl;
				break;

			case MIB_IF_TYPE_LOOPBACK:
				//cout<<"�������ͣ�"<<"LOOPBACK"<<endl;
				break;

			case MIB_IF_TYPE_SLIP:
				//cout<<"�������ͣ�"<<"SLIP"<<endl;
				break;

			default:
				break;
			}

			pIpAdapterInfo = pIpAdapterInfo->Next;
			//printf("%s\n", buffer);
			//cout << MacAddress;
		}

	}
	//�ͷ��ڴ�ռ�
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}

	return MacAddress;
}

//-------------------------------------------------
//POCO���MD5�㷨
string CrypDll::MD5(const string &INPUT_STRING)
{
	MD5Engine omd5;
	DigestOutputStream dos(omd5);

	istringstream istr(INPUT_STRING);

	StreamCopier::copyStream(istr, dos);
	dos.close();

	return DigestEngine::digestToHex(omd5.digest());

}



