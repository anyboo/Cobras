#pragma once
#include <string>

class CDES
{
public:
	CDES(CDongle& dongle);
	~CDES();

	void Calcute(const std::string& in, const int op_type, std::string& out);

private:
	CDES();

	CDongle& dev;
};

