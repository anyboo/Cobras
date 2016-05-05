#include "stdafx.h"
#include "Dongle.h"
#include "DES.h"
#include <cassert>
#include <stdexcept>

#define CDES_KEY_ID 0x1122

CDES::CDES(CDongle& dongle)
:dev(dongle)
{
}


CDES::~CDES()
{
}

void CDES::Calcute(const std::string& in, const int op_type, std::string& out)
{
	//max char is 512 bytes
	assert(in.length() <= CDongle::max_caclute_chararter);
	if (in.length() > CDongle::max_caclute_chararter)
		throw std::overflow_error("max char is 512 bytes!");
	
	char output[512] = { 0 }; // limitation of size decide by the device of usbkey
	int len = sizeof(output) / sizeof(char);

	unsigned long result = Dongle_TDES(dev, CDES_KEY_ID, op_type, (BYTE*)const_cast<char*>(in.c_str()),
		(BYTE*)output, len);
	assert(result == DONGLE_SUCCESS);
	out = std::string(output, len);
}