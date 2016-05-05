#pragma once
#include <string>

class CRSA
{
public:
	CRSA(CDongle& dongle);
	~CRSA();
	void Calcute(const std::string& in, const int op_type, std::string& out);

private:
	CRSA();
	void LoadPublicKey(const std::string& rsa_file);
	
	RSA_PUBLIC_KEY  rsaPub;
	CDongle& dev;
};

