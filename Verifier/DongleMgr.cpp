#include "stdafx.h"
#include "Dongle.h"
#include "DongleMgr.h"
#include <cassert>
#include <exception>

static CDongleMgr* Inst = nullptr;

CDongleMgr::CDongleMgr()
{

}

CDongleMgr& CDongleMgr::Instance()
{
	if (!Inst)
	{
		Inst = new CDongleMgr();
	}
	assert(Inst != nullptr);//maybe new Operator was failed.
	return *Inst;
}

CDongleMgr::~CDongleMgr()
{
	ClearDevices();
}

void CDongleMgr::ClearDevices()
{
	for (Iter it = dongles.begin(); it != dongles.end(); it++)
	{
		CDongle* ptr = *it;
		if (ptr){
			delete ptr;
			ptr = nullptr;
		}
	}
	dongles.clear();
}

void CDongleMgr::EnumDevices()
{
	unsigned long result = 0;
	int count = 0;
	DONGLE_INFO DongleInfo = { 0 };

	result = Dongle_Enum(&DongleInfo, &count);
	if (result != DONGLE_SUCCESS || count <= 0){
		//no devices or enumrate error must throw an exception;
		throw std::exception("no devices");
	}
	
	for (int i = 0; i < count; ++i)
	{
		AddDevice(DongleInfo);
	}
}

void CDongleMgr::AddDevice(DONGLE_INFO& DongleInfo)
{
	CDongle* pDongle = new CDongle(DongleInfo);

	assert(pDongle != nullptr);
	if (!IsDuplicate(pDongle))
		dongles.push_back(pDongle);
}

bool CDongleMgr::IsDuplicate(CDongle* dev)
{
	assert(dev);
	bool result = false;
	for (Iter it = dongles.begin(); it != dongles.end(); it++){
		result = (dev->id() == (*it)->id());
	}
	return result;
}

void CDongleMgr::RemoveDevice()
{
	//Remove a Dongle Device from Device List
}

CDongle& CDongleMgr::GetDongleFromHead()
{
	assert(!dongles.empty());

	if (dongles.empty())
		throw std::out_of_range("id is out of dongles'size!");

	return *dongles.front();
}