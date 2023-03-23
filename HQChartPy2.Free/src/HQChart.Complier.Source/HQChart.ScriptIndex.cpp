/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	自定义脚本函数库 (c++)
*/


#include "HQChart.ScriptIndex.h"
#include "HQChart.Variant.h"



namespace HQChart { namespace Complier {

ScriptIndex::ScriptIndex()
{

}

ScriptIndex::ScriptIndex(const std::wstring& strName, const std::wstring& strCode, const std::wstring& strDescription)
	:m_strName(strName), m_strCode(strCode), m_strDescription(strDescription)
{

}

ScriptIndex::ScriptIndex(const std::wstring& strName, const std::wstring& strCode, const ARRAY_ARGUMENT& argArgs, const std::wstring& strDescription)
	: m_strName(strName), m_strCode(strCode), m_aryArgument(argArgs), m_strDescription(strDescription)
{

}

void ScriptIndex::CopyTo(ScriptIndex& dest) const
{
	dest.m_strName = m_strName;
	dest.m_strCode = m_strCode;
	dest.m_strDescription = m_strDescription;
	dest.m_aryArgument = m_aryArgument;
}

void ScriptIndex::Clear()
{
	m_strName.clear();
	m_strCode.clear();
	m_strID.clear();
	m_strDescription.clear();
	m_aryArgument.clear();
}

	

}
}