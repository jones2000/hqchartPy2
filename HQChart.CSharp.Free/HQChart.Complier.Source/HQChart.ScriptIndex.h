/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	技术指标信息 (c++)
*/
#pragma once

#include <string>
#include <map>
#include <vector>

namespace HQChart { namespace Complier {

struct ARGUMENT_ITEM
{
	ARGUMENT_ITEM() :_dValue(0)
	{
	}

	ARGUMENT_ITEM(const std::wstring& strName, double dValue)
		:_strName(strName), _dValue(dValue)
	{

	}

	std::wstring	_strName;
	double			_dValue;
};


class ScriptIndex
{
public:
	typedef std::vector<ARGUMENT_ITEM>	ARRAY_ARGUMENT;

	ScriptIndex();
	ScriptIndex(const std::wstring& strName, const std::wstring& strCode, const std::wstring& strDescription);
	ScriptIndex(const std::wstring& strName, const std::wstring& strCode, const ARRAY_ARGUMENT& argArgs, const std::wstring& strDescription);

	void SetName(const std::wstring& strValue) { m_strName = strValue; }
	void SetCode(const std::wstring& strValue) { m_strCode = strValue; }
	void SetDescription(const std::wstring& strValue) { m_strDescription = strValue; }
	void SetArgument(const ARRAY_ARGUMENT& argArgs) { m_aryArgument = argArgs; }
	void SetID(const std::wstring& strValue) { m_strID = strValue; }

	const std::wstring& GetName() const { return m_strName; }
	const std::wstring& GetCode() const { return m_strCode; }
	const ARRAY_ARGUMENT& GetArgument() const { return m_aryArgument; }
	const std::wstring& GetDescription() const { return m_strDescription; }
	const std::wstring& GetID() const { return m_strID; }

	void CopyTo(ScriptIndex& dest) const;
	void Clear();	//清空指标信息

private:

	std::wstring m_strName;			//名称
	std::wstring m_strCode;			//代码
	std::wstring m_strDescription;	//描述信息
	std::wstring m_strID;			//指标唯一ID
	ARRAY_ARGUMENT m_aryArgument;	//参数
};


}
}