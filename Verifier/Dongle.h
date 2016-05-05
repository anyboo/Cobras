#pragma once
#include <string>
#include "Dongle_API.h"

typedef
enum Algorithm{
	DES,
	RAS,
} Algorithm;

class CDongle
{
public:

	CDongle(DONGLE_INFO& info);
	~CDongle();
	
	std::string Encode(const std::string& encodeString);
	std::string Decode(const std::string& decodeString);
	void Execute(const std::string& encoded);
	std::string id()const;
private:
	CDongle();
	void Init(DONGLE_INFO& info);
	void Open();
	void Close();
	void Select(Algorithm Algorithm);
	void InteralEncodeAndDecode(const std::string& in, const int op_type, std::string& out);
	operator DONGLE_HANDLE()const;

	DONGLE_HANDLE hDongle;

	unsigned short  version;				 //Version of Device COS
	unsigned long   ProductID;               //Product ID
	unsigned long   UserID;					 //User ID

	std::string     DateOfmanufacture;       //Date of manufacture
	std::string     HardwareID;				 //8 bytes of Hardware ID

	const int execute_id = 0x4321;

	const std::size_t default_ukey = 0;
	static const std::size_t max_caclute_chararter = 512;

	Algorithm AlgType;
	friend class CRSA;
	friend class CDES;
};

