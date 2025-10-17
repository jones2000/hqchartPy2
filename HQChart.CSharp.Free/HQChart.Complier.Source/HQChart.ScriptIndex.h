/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	��Դ��Ŀ https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	����ָ����Ϣ (c++)
*/
#pragma once

#include <string>
#include <map>
#include <vector>

namespace HQChart { namespace Complier {

struct ARGUMENT_ITEM
{
	ARGUMENT_ITEM() :_dValue(0), _lValueType(0)
	{

	}

	ARGUMENT_ITEM(const std::wstring& strName, double dValue)
		:_strName(strName), _dValue(dValue), _lValueType(0)
	{

	}

	ARGUMENT_ITEM(const std::wstring& strName, const std::wstring& strValue)
		:_strName(strName), _dValue(0), _strValue(strValue), _lValueType(1)
	{

	}

	std::wstring	_strName;
	double			_dValue;
	std::wstring	_strValue;	
	long			_lValueType;	//0=double 1=string
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
	void Clear();	//���ָ����Ϣ

private:

	std::wstring m_strName;			//����
	std::wstring m_strCode;			//����
	std::wstring m_strDescription;	//������Ϣ
	std::wstring m_strID;			//ָ��ΨһID
	ARRAY_ARGUMENT m_aryArgument;	//����
};


}
}