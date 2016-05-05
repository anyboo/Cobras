#pragma once

#include <list>

class CDongle;
class CDongleMgr
{
public:
	~CDongleMgr();

	static CDongleMgr& Instance();
	void EnumDevices();
	void ClearDevices();
	CDongle& GetDongleFromHead();
protected:
	void AddDevice(DONGLE_INFO& DongleInfo);
	void RemoveDevice();
private:
	CDongleMgr();
	CDongleMgr(CDongleMgr&);
	CDongleMgr& operator=(CDongleMgr&);
	bool IsDuplicate(CDongle* dev);
	std::list<CDongle*> dongles;
	typedef std::list<CDongle*>::iterator Iter;

	//static bool bInstance;
};