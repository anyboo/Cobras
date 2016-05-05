// gCatch.cpp : Defines the entry point for the console application.
//
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <string>
#include <Windows.h>
#include <tchar.h>

TEST_CASE("This is a sample", "[sample]")
{
	using namespace Catch;
	SECTION("print string")
	{
		std::string echo("print echo");
		CHECK( "print"== echo);
	}
}

typedef bool (*Func1)();
typedef bool (*Func2)(unsigned long PID);

TEST_CASE("Load verifier.dll library", "[loadlibrary]")
{
#if _DEBUG	
	HMODULE moudule = ::LoadLibrary(_T("verifier_d.dll"));
#else
	HMODULE moudule = ::LoadLibrary(_T("verifier.dll"));
#endif
	REQUIRE(moudule != NULL);

	SECTION("Function IsHaveUKEY is work","[IsPresent]")
	{
		Func1 IsPresent = (Func1)::GetProcAddress(moudule, "IsPresent");
		CAPTURE(moudule);
		REQUIRE(IsPresent != NULL);
		CAPTURE(IsPresent);

		INFO("CALL IsPresent");
		(*IsPresent)() ;

		SECTION("NO DEVICE", "[NODEVICE]")
		{
			bool present = (*IsPresent)();
			REQUIRE(present == false);
		}

		SECTION("HAVE A DEVICE", "[DEVICE]")
		{
			bool present = (*IsPresent)();
			REQUIRE(present == true);
		}
	}
	
	SECTION("Function VerifiyUKEY is work", "[Verifiy]")
	{
		Func2 Verifiy = (Func2)::GetProcAddress(moudule, "Verifiy");

		REQUIRE(Verifiy != NULL);
		CAPTURE(Verifiy);

		INFO("CALL Verifiy");
		(*Verifiy)(::GetCurrentProcessId());

		SECTION("TEST Verify in no devices", "[verifynodev]"){
			bool verify = (*Verifiy)(::GetCurrentProcessId());
			REQUIRE(verify == false);
		}

		SECTION("TEST Verify in a devices", "[verifydev]"){
			bool verify = (*Verifiy)(::GetCurrentProcessId());
			REQUIRE(verify == true);
		}
	}
}