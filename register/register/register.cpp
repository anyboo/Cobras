//register.cpp
#include "register.h"
#include "md5.h"

#include <winsock2.h> 
#include "iphlpapi.h" 

#pragma comment(lib, "iphlpapi.lib") 

using namespace std;	  

Register::Register()
{
	m_thisDynasty = XIA;		//0��һ����Ȩ��ʽ
	m_strKey[m_thisDynasty] = "XIA";

}

Register::~Register()
{

}

//����ȡ�뺯����----------------------------------------------
//20160622���������ǵ��ͻ�ע��ʱ�ṩACODE����Ҫ����RCODE������ACODE��RCODE������Ϊ������

string Register::getACode(const string &strMac)
{
	return MD5(strMac);	 
}

string Register::getRCode(const string &aCode)
{
	string strMD5;
	switch (m_thisDynasty)
	{
		//��һ����Ȩ��ʽ
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

//�ж�ע���ļ��е�ע�ἤ�����Ƿ���ȷ
bool Register::fileRegOK()
{
	bool fSucess = false;

	//��֤�ļ���¼��ͼ�����������Ƿ�һ�£�
	string m_strRCode = readRCodeFile();
	string scode = getRCode(getACode(getMac()));

	if (scode == m_strRCode)
	{
			fSucess = true;
	}

	return fSucess;
}

//��ȡע���ļ��е�ע�ἤ����
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

//���������ȷע�ἤ����д��ע���ļ�
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

//ע�ἤ��
int Register::codeRegOK(const string &strAuthy)
{
	string strMac = getMac();
	if (strMac.empty())
		return 0;;

	int rSuccess = 0;

	string scode = getRCode(getACode(strMac));

	//�ж�����ע�ἤ�����Ƿ���ȷ
	if (scode == strAuthy)
	{
		rSuccess = 1;
		writeRCodeFile(strAuthy);
	}
	else
	//�����벻��ȷ�����Ŷ�ȡע���ļ��е�ע�ἤ����
	{
		//��ȷ2 ����3
		rSuccess = (fileRegOK()) ? 2 : 3;

	}

	return rSuccess;
}

//��ȡMAC��ַ�����ȡ��һ����̫����MAC��ַ
string Register::getMac()
{
	//PIP_ADAPTER_INFO�ṹ��ָ��洢����������Ϣ
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//�õ��ṹ���С,����GetAdaptersInfo����
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//����GetAdaptersInfo����,���pIpAdapterInfoָ�����;����stSize��������һ��������Ҳ��һ�������
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);

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

	int iMacN = 0;				//��������������
	bool ErrorETHERNET;			//��ETHERNET��������Bluetooth������
	string MacAddress[10];
	char buffer[20] = "";
	string sMacA;		
	string strDynastyAdd;

	if (ERROR_SUCCESS == nRel)
	{
		//���������Ϣ
		//�����ж�����,���ͨ��ѭ��ȥ�ж�	   		
		while (pIpAdapterInfo)
		{
			switch (pIpAdapterInfo->Type)
			{
			case MIB_IF_TYPE_ETHERNET:
				//20160622����������Bluetooth����
				ErrorETHERNET = false;
				if (!strstr(pIpAdapterInfo->Description, "PCI") || strstr(pIpAdapterInfo->Description, "Bluetooth"))
				{
					ErrorETHERNET = true;
					break;
				}
				//��������
				MacAddress[iMacN].clear();
				sMacA = "";

				//������Mac��ַ�μ������ַ������硰-����."��:����
				switch (m_thisDynasty)
				{
					case XIA:					  //��һ����Ȩ��ʽ
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
			if (iMacN < 9 && !ErrorETHERNET) iMacN++;					//ֻȡ���������10
		}

	}
	//�ͷ��ڴ�ռ�
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}
	//���ص�һ����̫����MAC��ַ
	return MacAddress[0];
}

//-------------------------------------------------
//MD5�㷨
string Register::MD5(const string &INPUT_STRING)
{
	return MD5::MD5(INPUT_STRING).toString();

}
//XOR�򵥸������ܽ����㷨
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



