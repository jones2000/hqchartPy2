/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com
*/

#pragma once
#include <vector>
#include <string>


namespace HQChart {namespace Complier { namespace Py {

//用户授权信息
class AuthorizeInfo
{
public:
	AuthorizeInfo();
	~AuthorizeInfo();

	static AuthorizeInfo& GetInstance();

	void Load(const std::string& strFile);
	const std::wstring& GetInfo() const;
	const std::wstring& GetError() const;

	std::wstring ToString() const;

	bool IsVaild() const;

protected:
	bool					m_bVaild = true;
	int						m_nExpired = 0;
	std::wstring			m_strInfo = L"HQChartPy2社区版本";
	mutable std::wstring	m_strError;
};



}
}
}
