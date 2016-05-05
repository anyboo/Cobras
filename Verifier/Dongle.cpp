#include "stdafx.h"
#include "Dongle.h"
#include <exception>
#include <cassert>
#include "DES.h"
#include "RSA.h"

CDongle::CDongle(DONGLE_INFO& info) :
 hDongle(nullptr)
{
	Open();
	Init(info);
}

void CDongle::Init(DONGLE_INFO& info)
{
	version = info.m_Ver;
	ProductID = info.m_PID;
	UserID = info.m_UserID;

	DateOfmanufacture.assign((char*)info.m_BirthDay, sizeof(info.m_BirthDay));
	HardwareID.assign((char*)info.m_HID, sizeof(info.m_HID));
}

CDongle::~CDongle()
{
	Close();
}

void CDongle::Open()
{
	unsigned long result = 0;
	result = Dongle_Open(&hDongle, default_ukey);
	assert(result == DONGLE_SUCCESS);
	//Adjust Algorithm of the UKEY then Initizal it
	Select(Algorithm::DES);
}

void CDongle::Select(Algorithm Algorithm)
{
	AlgType = Algorithm;
}

void CDongle::Close()
{
	unsigned long result = 0;
	result = Dongle_Close(&hDongle);
}

void CDongle::Execute(const std::string& encoded)
{
	/*execute usb key bin to calculate some thing*/
	unsigned long result = 0;

	assert(encoded.length() <= max_caclute_chararter);
	result = Dongle_RunExeFile(hDongle, execute_id, (BYTE*)const_cast<char*>(encoded.c_str()), encoded.length(), NULL);
	assert(result == DONGLE_SUCCESS);
}

std::string CDongle::Encode(const std::string& encodeString)
{
	//if length > 117 ,must be spilt it.
	assert(encodeString.length() > 0 && encodeString.length() <= max_caclute_chararter);

	std::string outString;
	InteralEncodeAndDecode(encodeString, FLAG_ENCODE, outString);
	return outString;
}

std::string CDongle::Decode(const std::string& decodeString)
{
	//if length > 117 ,must be spilt it.
	assert(decodeString.length() > 0 && decodeString.length() <= max_caclute_chararter);

	std::string outString;
	InteralEncodeAndDecode(decodeString, FLAG_DECODE, outString);
	return outString;
}

void CDongle::InteralEncodeAndDecode(const std::string& in, const int op_type, std::string& out)
{
	if (AlgType == Algorithm::DES){
		CDES alg(*this);
		alg.Calcute(in, op_type, out);
	}
	if (AlgType == Algorithm::RAS){
		CRSA alg(*this);
		alg.Calcute(in, op_type, out);
	}
}

CDongle::operator DONGLE_HANDLE()const
{
	return hDongle;
}

std::string CDongle::id()const
{
	return HardwareID;
}