#include "stdafx.h"
#include "Dongle.h"
#include "RSA.h"
#include <cassert>

CRSA::CRSA(CDongle& dongle)
:dev(dongle)
{
	std::string rsa_file;
	LoadPublicKey(rsa_file);
}


CRSA::~CRSA()
{
}

void CRSA::LoadPublicKey(const std::string& rsa_file)
{

}

void CRSA::Calcute(const std::string& in, const int op_type, std::string& out)
{
	char output[128] = { 0 }; // limitation of size decide by the device of usbkey
	int len = sizeof(output) / sizeof(char);
	unsigned long result;
	result = Dongle_RsaPub(dev, op_type, &rsaPub, (BYTE*)const_cast<char*>(in.c_str()), in.length(), (BYTE*)output, &len);
	assert(result == DONGLE_SUCCESS);
	out = std::string(output, len);
}
