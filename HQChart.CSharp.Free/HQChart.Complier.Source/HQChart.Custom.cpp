/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	麦语法自定义函数,变量 (c++)
*/


#include "HQChart.KLineData.h"
#include "HQChart.Log.h"
#include <list>
#include <sstream>


namespace HQChart { namespace Complier {

CustomFunction::CustomFunction()
{
	CUSTOM_FUNCTION_ITEM FUNCTION_LIST[] =
	{
		//引用专业财务数据 
		//FINVALUE(ID),ID为数据编号
		{L"FINVALUE",1},

		//引用指定年和月日的某类型的财务数据
		//FINONE(ID,Y,MMDD),ID为数据编号,Y和MMDD表示年和月日.
		{L"FINONE",3},	

		//引用股票交易类数据
		//GPJYVALUE(ID,N,TYPE),ID为数据编号,N表示第几个数据,TYPE:为1表示做平滑处理,没有数据的周期返回上一周期的值; 为0表示不做平滑处理
		{L"GPJYVALUE", 3},

		//引用某天某种类型的股票交易类数据
		//GPJYONE(ID,N,Y,MMDD),ID为数据编号,N表示第几个数据,Y和MMDD表示年和月日.
		//如果Y为0,MMDD为0,表示最新数据,MMDD为1,2,3...,表示倒数第2,3,4...个数据
		{L"GPJYONE", 4},

		//引用市场总的交易类数据
		//SCJYVALUE(ID,N,TYPE),ID为数据编号,N表示第几个数据,TYPE:为1表示做平滑处理,没有数据的周期返回上一周期的值; 为0表示不做平滑处理
		{L"SCJYVALUE",3},

		//引用某天某种类型的市场总的交易类数据.如果指标支持云数据函数,则不需要[专业财务数据]下载.
		//SCJYONE(ID,N,Y,MMDD),ID为数据编号,N表示第几个数据,Y和MMDD表示年和月日.
		//如果Y为0,MMDD为0,表示最新数据,MMDD为1,2,3...,表示倒数第2,3,4...个数据
		{L"SCJYONE",4},

		//引用股票的某种数据.
		//GPONEDAT(ID),ID为数据编号
		{L"GPONEDAT",1},
	};

	for (const auto& item : FUNCTION_LIST)
	{
		m_aryCustomFunc[item._strName] = item;
	}
}

CustomFunction::~CustomFunction()
{

}

CustomFunction& CustomFunction::GetInstance()
{
	static CustomFunction instance;
	return instance;
}

bool CustomFunction::Get(const std::wstring& strName, CUSTOM_FUNCTION_ITEM& item)
{
	auto find = m_aryCustomFunc.find(strName);
	if (find == m_aryCustomFunc.end()) return false;

	item = find->second;

	return true;
}

void CustomFunction::Add(const std::wstring& strName, long lArgCount)
{
	CUSTOM_FUNCTION_ITEM item;
	item._strName = strName;
	item._nArgCount = lArgCount;

	m_aryCustomFunc[strName] = item;
}



//////////////////////////////////////////////////////////////////////////////////////
CustomVariant::CustomVariant()
{

}


CustomVariant::~CustomVariant()
{

}


CustomVariant& CustomVariant::GetInstance()
{
	static CustomVariant instance;
	return instance;
}

void CustomVariant::Add(const std::wstring& strName)
{
	auto find = m_setVariant.find(strName);
	if (find == m_setVariant.end())
		m_setVariant.insert(strName);
}

bool CustomVariant::IsExist(const std::wstring& strName) const
{
	if (m_setVariant.empty()) return false;

	auto find = m_setVariant.find(strName);
	
	if (find == m_setVariant.end()) return false;

	return true;
}

}
}