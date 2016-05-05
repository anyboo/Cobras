// Verifier.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Verifier.h"
#include <stdio.h>
#include <tchar.h>
#include "Dongle.h"
#include "DongleMgr.h"
#include <iostream>

bool IsPresent()
{
	try
	{
		CDongleMgr::Instance().EnumDevices();
		return true;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what();
		return false;
	}
}

bool Verifiy(unsigned long PID)
{
	if (!IsPresent()) return false;

	bool result = false;
	
	CDongle& dongle = CDongleMgr::Instance().GetDongleFromHead();
	
	std::string input("verifiy");
	std::string encoded = dongle.Encode(input);
	dongle.Execute(encoded);
	std::string output = dongle.Decode(encoded);
	
	result = (output.find("sisec") != std::string::npos);	
	return result;
}
