/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

内置函数类 (c++)
*/

#include "HQChart.data.h"
#include "HQChart.Execute.h"


///////////////////////////////////////////////////////////////////////////////
//
// 四则运算
//
//////////////////////////////////////////////////////////////////////////////

namespace HQChart { namespace Complier {


Variant* VariantOperator::GT(const Variant& left, const Variant& right)
{
	Variant* pResult =  Create();
	if (left.m_nType == Variant::ARRAY_DOUBLE_TYPE && right.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		GT(pResult->m_aryValue, left.m_aryValue, right.m_aryValue);
	}
	else if (left.m_nType == Variant::DOUBLE_TYPE && right.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		if (left.m_aryValue.size() == 1 && left.m_aryValue[0].IsVaild())
		GT(pResult->m_aryValue, left.m_aryValue[0]._dValue, right.m_aryValue);
	}
	else if (left.m_nType == Variant::ARRAY_DOUBLE_TYPE && right.m_nType == Variant::DOUBLE_TYPE)
	{
		if (right.IsVaildDoulbe())
		{
			GT(pResult->m_aryValue, left.m_aryValue, right.GetDoubleValue());
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		}
	}
	else if (left.m_nType == Variant::DOUBLE_TYPE && right.m_nType == Variant::DOUBLE_TYPE)
	{
		if (left.IsVaildDoulbe() && right.IsVaildDoulbe())
		{
			double dValue = DoubleGT(left.GetDoubleValue() , right.GetDoubleValue()) ? 1 : 0;
			pResult->SetDoubleValue(dValue);
		}
	}
	return pResult;
}

void VariantOperator::GT(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right)
{
	size_t nLeftCount = left.size(), nRightCount = right.size();
	size_t nCount = std::max(nLeftCount, nRightCount);
	for (size_t i = 0; i < nCount; ++i)
	{
		VARIANT_ITEM item;
		if (i<nLeftCount || i<nRightCount)
		{
			const auto& leftItem = left[i];
			const auto& rightItem = right[i];
			if (leftItem._sType == VARIANT_ITEM::VAILD_ID && rightItem._sType == VARIANT_ITEM::VAILD_ID)
			{
				item.SetValue(DoubleGT(leftItem._dValue , rightItem._dValue) ? 1 : 0);

			}
		}
		dest.push_back(item);
	}
}

void VariantOperator::GT(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right)
{
	for (auto rightItem : right)
	{
		VARIANT_ITEM item;
		if (rightItem._sType == VARIANT_ITEM::VAILD_ID)
		{
			item.SetValue(DoubleGT(dLeft , rightItem._dValue) ? 1 : 0);
		}
		dest.push_back(item);
	}
}

void VariantOperator::GT(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, double dRight)
{
	dest.reserve(left.size());
	for (auto leftItem : left)
	{
		VARIANT_ITEM item;
		if (leftItem._sType == VARIANT_ITEM::VAILD_ID)
		{
			item.SetValue(DoubleGT(leftItem._dValue , dRight) ? 1 : 0);
		}
		dest.push_back(item);
	}
}

/*
根据条件执行不同的语句, 可中止(根据序列的最后一个数值来判断).
用法:
IFC(X, A, B)若X不为0则执行A, 否则执行B.IFC与IF函数的区别 : 根据X的值来选择性执行A、B表达式.
例如 :
	IFC(CLOSE > OPEN, HIGH, TESTSKIP(1)); L; 表示当日收阳则返回最高值, 并执行下一句"L;", 否则退出公式计算
*/
bool VariantOperator::IFC(const Variant& data)
{
	if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		if (!data.IsVaildDoulbe()) return false;

		return data.GetDoubleValue() > 0;
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		if (data.m_aryValue.empty()) return false;
		auto iter= data.m_aryValue.cbegin();
		if (iter->IsVaild()) return iter->_dValue > 0;
		return false;
	}

	return false;
}

/*
TESTSKIP(A):满足A则直接返回.
用法:
TESTSKIP(A)
表示如果满足条件A则该公式直接返回,不再计算接下来的表达式 注意:A为非序列数据,只取最后一个数据
*/
bool VariantOperator::TESTSKIP(const Variant& data)
{
	if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		if (!data.IsVaildDoulbe()) return false;

		return data.GetDoubleValue() > 0;
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		if (data.m_aryValue.empty()) return false;
		auto iter = data.m_aryValue.cbegin();
		if (iter->IsVaild()) return iter->_dValue > 0;
		return false;
	}

	return false;
}


}
}
