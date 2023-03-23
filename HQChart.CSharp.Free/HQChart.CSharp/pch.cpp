// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"
#include <locale>
#include <codecvt>

std::string WStringToUTF8(const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}

std::wstring UTF8ToWString(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.from_bytes(str);
}

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
