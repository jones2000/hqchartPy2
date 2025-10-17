/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	内置函数类-脚本调用 (c++)
*/

#include "HQChart.data.h"
#include "HQChart.Execute.h"
#include "HQChart.Log.h"
#include "HQChart.KLineData.h"
#include "HQChart.BlockScriptResultCache.h"
#include <sstream>
#include <iterator>
#include <regex>

////////////////////////////////////////////////////////////////////////////////
//	板块排名,统计等批量计算
//
///////////////////////////////////////////////////////////////////////////////


namespace HQChart { namespace Complier {

//orderType 0=降序 1=升序
Variant* VariantOperator::RANK(const Variant& block, const Variant& code, const Variant& orderType, const IHistoryData* pHistoryData, Node* pNode)
{
	if (!block.IsVaildString() || !code.IsVaildString())
	{
		std::wstringstream strError;
		strError << L"板块排名板块ID或指标脚本无效";
		ExecuteExcept error(strError.str(), L"VariantOperator::RANK", pNode);
		throw error;
	}

	std::wstring strBlockID, strCode;
	long lOrderType = (long)orderType.GetDoubleValue();
	strBlockID = block.GetStringValue();
	strCode = code.GetStringValue();

	Variant* pResult = Create();
	BlockScriptResult* pBlockData = CallBlockScriptV2(strCode, strBlockID, pHistoryData->GetSymbolString(), pHistoryData, pNode);
	if (!pBlockData) return pResult;

	pBlockData->GetRankResult(pHistoryData->GetSymbolString(), lOrderType, pResult);

	return pResult;
}

//dataType 0-累加,1-平均值,2-最大值,3-最小值,4-中位数 5、最大值所处的品种序号,6-最小值所处的品种序号 7-中位数所处的品种代码
Variant* VariantOperator::RANK2(const Variant& block, const Variant& code, const Variant& dataType, const IHistoryData* pHistoryData, Node* pNode)
{
	if (!block.IsVaildString() || !code.IsVaildString())
	{
		std::wstringstream strError;
		strError << L"板块排名板块ID或指标脚本无效";
		ExecuteExcept error(strError.str(), L"VariantOperator::RANK", pNode);
		throw error;
	}

	std::wstring strBlockID, strCode;
	long lDataType = (long)dataType.GetDoubleValue();
	strBlockID = block.GetStringValue();
	strCode = code.GetStringValue();

	Variant* pResult = Create();
	BlockScriptResult* pBlockData = CallBlockScriptV2(strCode, strBlockID, pHistoryData->GetSymbolString(), pHistoryData, pNode);
	if (!pBlockData) return pResult;

	switch (lDataType)
	{
	case 0:
		pBlockData->GetSumResult(pHistoryData->GetSymbolString(), pResult);
		break;
	case 1:
		pBlockData->GetAverageResult(pHistoryData->GetSymbolString(), pResult);
		break;
	case 2:
		pBlockData->GetMaxResult(pHistoryData->GetSymbolString(), pResult);
		break;
	case 3:
		pBlockData->GetMinResult(pHistoryData->GetSymbolString(), pResult);
		break;
	case 4:
		pBlockData->GetMedianResult(pHistoryData->GetSymbolString(), pResult);
		break;
	}

	return pResult;
}


// 废弃!!!!!
bool VariantOperator::CallBlockScript(const std::wstring& strCode, const std::wstring& strBlockID, const std::wstring& strSymbol, BlockScriptResult& result, const IHistoryData* pHistoryData, Node* pNode)
{
	InvokeIndex IndexInfo;
	IndexInfo.SetIndex(L"BlockScript__", strCode);
	IndexInfo.SetPeriod(pHistoryData->GetPeriod());
	IndexInfo.SetRight(pHistoryData->GetRight());
	IndexInfo.SetDataClassName(pHistoryData->GetClassName());

	const auto& aryKData=pHistoryData->GetData();
	long lDate = aryKData.back()._nDate;
	std::vector<std::wstring> arySymbol;
	if (strBlockID.find(L"|") != std::wstring::npos)
	{
		if (StringSplitV2(strBlockID, L"|", arySymbol) <= 0) return false;
	}
	else
	{
		if (!pHistoryData->GetBlockMember(strBlockID, lDate, arySymbol, pNode)) return false;
	}
	

	result.Inital(arySymbol, pHistoryData->GetKCount());

	for (int i = 0; i < (int)arySymbol.size(); ++i)
	{
		const auto& item = arySymbol[i];
		IndexInfo.SetSymbol(item);

		HQChart::Complier::AutoPtrHistoryData ptrHistoryData(IndexInfo.GetDataClassName());
		HQChart::Complier::IHistoryData* pTempHistoryData = ptrHistoryData.Create(IndexInfo.GetSymbol(), IndexInfo.GetPeriod(), IndexInfo.GetRight());
		pTempHistoryData->Initialization((void*)pHistoryData);
		Execute exec;
		try
		{
			if (ExecuteIndex(IndexInfo, &exec, pTempHistoryData, pNode))
			{
				const Execute::MAP_VARIANT& outVar = exec.GetOutVar();	//输出变量
				if (outVar.empty()) continue;

				//const auto& aryOutVarName = exec.GetOutVarInfo();
				auto finder = outVar.find(L"OUT");
				if (finder == outVar.end()) continue;
				if (!finder->second) continue;

				if (finder->second->m_nType != Variant::ARRAY_DOUBLE_TYPE) continue;

				IHistoryData::ARRAY_FIT_DATETIME_DATA aryDateTime;
				IHistoryData::FitDateTime(pTempHistoryData, pHistoryData, aryDateTime);	//合并数据

				long lCount = (long)aryDateTime.size();
				const auto& aryValue = finder->second->m_aryValue;
				if (aryValue.empty()) continue;
				long lDataCount = (long)aryValue.size();
				auto pRefRow = result.GetRow(i);
				if (!pRefRow) continue;

				for (long j = 0; j < lCount; ++j)
				{
					const auto& item = aryDateTime[j];
					if (item._lIndex < 0 || item._lIndex >= lDataCount) continue;
					const auto& dataItem = aryValue[item._lIndex];
					if (!dataItem.IsVaild()) continue;

					pRefRow->_aryData[j]._bVaild = true;
					pRefRow->_aryData[j]._dValue = dataItem._dValue;
				}
			}
		}
		catch (...)
		{

		}
	}

	return true;
}


void VariantOperator::BuildBlockScriptKey(const InvokeIndex& indexInfo, std::vector<std::wstring>& arySymbol, std::wstring& strOutKey)
{
	std::wstringstream strKey;

	strKey << L"BlockScript--Key--";

	strKey << indexInfo.GetCode().c_str();
	strKey << L"--";

	strKey << indexInfo.GetPeriod();
	strKey << L"--";

	strKey << indexInfo.GetPeriod();
	strKey << L"--";

	for (int i = 0; i < (int)arySymbol.size(); ++i)
	{
		if (i > 0) strKey << L"|";
		strKey << arySymbol[i];
	}

	strOutKey = strKey.str();
}

BlockScriptResult* VariantOperator::CallBlockScriptV2(const std::wstring& strCode, const std::wstring& strBlockID, const std::wstring& strSymbol, const IHistoryData* pHistoryData, Node* pNode)
{
	BlockScriptResult* pResult = NULL;

	InvokeIndex IndexInfo;
	IndexInfo.SetIndex(L"BlockScript__", strCode);
	IndexInfo.SetPeriod(pHistoryData->GetPeriod());
	IndexInfo.SetRight(pHistoryData->GetRight());
	IndexInfo.SetDataClassName(pHistoryData->GetClassName());

	const auto& aryKData = pHistoryData->GetData();
	long lDate = aryKData.back()._nDate;
	std::vector<std::wstring> arySymbol;
	if (strBlockID.find(L"|") != std::wstring::npos)
	{
		if (StringSplitV2(strBlockID, L"|", arySymbol) <= 0) return pResult;
	}
	else
	{
		if (!pHistoryData->GetBlockMember(strBlockID, lDate, arySymbol, pNode)) return pResult;
	}

	std::wstring strKey;
	BuildBlockScriptKey(IndexInfo, arySymbol, strKey);

	BlockScriptResultCache& cache = BlockScriptResultCache::GetInstance();
	pResult = cache.Get(strKey);	//检测缓存

	if (pResult)
	{
		return pResult;
	}

	pResult = new BlockScriptResult();
	pResult->Inital(arySymbol, pHistoryData->GetKCount());

	for (int i = 0; i < (int)arySymbol.size(); ++i)
	{
		const auto& item = arySymbol[i];
		IndexInfo.SetSymbol(item);

		HQChart::Complier::AutoPtrHistoryData ptrHistoryData(IndexInfo.GetDataClassName());
		HQChart::Complier::IHistoryData* pTempHistoryData = ptrHistoryData.Create(IndexInfo.GetSymbol(), IndexInfo.GetPeriod(), IndexInfo.GetRight());
		pTempHistoryData->Initialization((void*)pHistoryData);
		Execute exec;
		try
		{
			if (ExecuteIndex(IndexInfo, &exec, pTempHistoryData, pNode))
			{
				const Execute::MAP_VARIANT& outVar = exec.GetOutVar();	//输出变量
				if (outVar.empty()) continue;

				//const auto& aryOutVarName = exec.GetOutVarInfo();
				auto finder = outVar.find(L"OUT");
				if (finder == outVar.end()) continue;
				if (!finder->second) continue;

				if (finder->second->m_nType != Variant::ARRAY_DOUBLE_TYPE) continue;

				IHistoryData::ARRAY_FIT_DATETIME_DATA aryDateTime;
				IHistoryData::FitDateTime(pTempHistoryData, pHistoryData, aryDateTime);	//合并数据

				long lCount = (long)aryDateTime.size();
				const auto& aryValue = finder->second->m_aryValue;
				if (aryValue.empty()) continue;
				long lDataCount = (long)aryValue.size();
				auto pRefRow = pResult->GetRow(i);
				if (!pRefRow) continue;

				for (long j = 0; j < lCount; ++j)
				{
					const auto& item = aryDateTime[j];
					if (item._lIndex < 0 || item._lIndex >= lDataCount) continue;
					const auto& dataItem = aryValue[item._lIndex];
					if (!dataItem.IsVaild()) continue;

					pRefRow->_aryData[j]._bVaild = true;
					pRefRow->_aryData[j]._dValue = dataItem._dValue;
				}
			}
		}
		catch (...)
		{
			DELETE_PTR(pResult);
			return NULL;
		}
	}

	cache.Add(strKey, pResult);

	return pResult;
}

/////////////////////////////////////////////////////////////////////////////////////
//  板块统计结果类
//
//
////////////////////////////////////////////////////////////////////////////////////
BlockScriptResult::BlockScriptResult()
{

}


BlockScriptResult::~BlockScriptResult()
{

}


bool BlockScriptResult::Inital(const std::vector<std::wstring>& arySymbol, long lDataCount)
{
	m_aryData.clear();
	BLOCK_RESULT_CELL DEFAULT_CELL;
	DEFAULT_CELL._bVaild = false;
	DEFAULT_CELL._dValue = -1;
	m_lKDataCount = lDataCount;

	for (int i = 0; i < (int)arySymbol.size(); ++i)
	{
		BLOCK_RESULT_ROW::REF_PTR pRef = BLOCK_RESULT_ROW::REF_PTR(new BLOCK_RESULT_ROW);

		pRef->_strSymbol = arySymbol[i];
		pRef->_aryData.resize(lDataCount, DEFAULT_CELL);

		m_aryData.push_back(pRef);
	}

	return true;
}

BLOCK_RESULT_ROW::REF_PTR BlockScriptResult::GetRow(long lRow)
{
	if (lRow < 0 && lRow >= m_aryData.size()) return NULL;

	return m_aryData[lRow];
}

bool BlockScriptResult::GetRankResult(const std::wstring& strSymbol, long lOrderType, Variant* pVarOut)
{
	BLOCK_RESULT_ROW::REF_PTR pRefFinder = NULL;
	long lFinderIndex = -1;
	for (int i = 0; i < m_aryData.size(); ++i)
	{
		const auto& item = m_aryData[i];
		if (item->_strSymbol == strSymbol)
		{
			pRefFinder = item;
			lFinderIndex = i;
			break;
		}
	}

	if (!pRefFinder) return false;

	pVarOut->SetType(Variant::ARRAY_DOUBLE_TYPE);
	auto& aryValue=pVarOut->GetArrayValue();
	aryValue.resize(m_lKDataCount, VARIANT_ITEM());

	long lCount = (int)m_aryData.size();
	long lRank = 0;
	for (int i = 0, j=0; i < m_lKDataCount; ++i)
	{
		const auto& item=pRefFinder->_aryData[i];
		if (!item._bVaild)
		{
			lRank = -1;
		}
		else
		{
			lRank = 1;
			for (j = 0; j < lCount; ++j)
			{
				if (j == lFinderIndex) continue;
				const auto& pRefSymbol = m_aryData[j];

				const auto& subItem=pRefSymbol->_aryData[i];
				if (!subItem._bVaild) continue;
				if (lOrderType == 0)
				{
					if (subItem._dValue > item._dValue) ++lRank;
				}
				else
				{
					if (subItem._dValue < item._dValue) ++lRank;
				}
			}
		}

		if (lRank > 0) aryValue[i].SetValue(lRank);
	}

	return true;
}

bool BlockScriptResult::GetMaxResult(const std::wstring& strSymbol, Variant* pVarOut)
{
	long lCount = (int)m_aryData.size();
	pVarOut->SetType(Variant::ARRAY_DOUBLE_TYPE);
	auto& aryValue = pVarOut->GetArrayValue();
	aryValue.resize(m_lKDataCount, VARIANT_ITEM());

	double dMax = 0;
	bool bVaild = false;
	for (int i = 0, j = 0; i < m_lKDataCount; ++i)
	{
		dMax = 0;
		bVaild = false;
		for (j = 0; j < lCount; ++j)
		{
			const auto& pRefSymbol = m_aryData[j];

			const auto& item = pRefSymbol->_aryData[i];
			if (!item._bVaild) continue;

			if (!bVaild)
			{
				dMax = item._dValue;
				bVaild = true;
			}
			else
			{
				if (dMax < item._dValue) dMax = item._dValue;
			}
		}
		
		if (bVaild) aryValue[i].SetValue(dMax);
	}

	return true;

}

bool BlockScriptResult::GetMinResult(const std::wstring& strSymbol, Variant* pVarOut)
{
	long lCount = (int)m_aryData.size();
	pVarOut->SetType(Variant::ARRAY_DOUBLE_TYPE);
	auto& aryValue = pVarOut->GetArrayValue();
	aryValue.resize(m_lKDataCount, VARIANT_ITEM());

	double dMin = 0;
	bool bVaild = false;
	for (int i = 0, j = 0; i < m_lKDataCount; ++i)
	{
		dMin = 0;
		bVaild = false;
		for (j = 0; j < lCount; ++j)
		{
			const auto& pRefSymbol = m_aryData[j];

			const auto& item = pRefSymbol->_aryData[i];
			if (!item._bVaild) continue;

			if (!bVaild)
			{
				dMin = item._dValue;
				bVaild = true;
			}
			else
			{
				if (dMin > item._dValue) dMin = item._dValue;
			}
		}

		if (bVaild) aryValue[i].SetValue(dMin);
	}

	return true;
}


bool BlockScriptResult::GetSumResult(const std::wstring& strSymbol, Variant* pVarOut)
{
	long lCount = (int)m_aryData.size();
	pVarOut->SetType(Variant::ARRAY_DOUBLE_TYPE);
	auto& aryValue = pVarOut->GetArrayValue();
	aryValue.resize(m_lKDataCount, VARIANT_ITEM());

	double dTotal = 0;
	bool bVaild = false;
	for (int i = 0, j = 0; i < m_lKDataCount; ++i)
	{
		dTotal = 0;
		bVaild = false;
		for (j = 0; j < lCount; ++j)
		{
			const auto& pRefSymbol = m_aryData[j];

			const auto& item = pRefSymbol->_aryData[i];
			if (!item._bVaild) continue;

			if (!bVaild)
			{
				dTotal = item._dValue;
				bVaild = true;
			}
			else
			{
				dTotal += item._dValue;
			}
		}

		if (bVaild) aryValue[i].SetValue(dTotal);
	}

	return true;
}

bool BlockScriptResult::GetAverageResult(const std::wstring& strSymbol, Variant* pVarOut)
{
	long lCount = (int)m_aryData.size();
	pVarOut->SetType(Variant::ARRAY_DOUBLE_TYPE);
	auto& aryValue = pVarOut->GetArrayValue();
	aryValue.resize(m_lKDataCount, VARIANT_ITEM());

	double dTotal = 0;
	bool bVaild = false;
	for (int i = 0, j = 0; i < m_lKDataCount; ++i)
	{
		dTotal = 0;
		bVaild = false;
		for (j = 0; j < lCount; ++j)
		{
			const auto& pRefSymbol = m_aryData[j];

			const auto& item = pRefSymbol->_aryData[i];
			if (!item._bVaild) continue;

			if (!bVaild)
			{
				dTotal = item._dValue;
				bVaild = true;
			}
			else
			{
				dTotal += item._dValue;
			}
		}

		if (bVaild && lCount!=0) aryValue[i].SetValue(dTotal/ lCount);
	}

	return true;
}

bool BlockScriptResult::GetMedianResult(const std::wstring& strSymbol, Variant* pVarOut)
{
	long lCount = (int)m_aryData.size();
	pVarOut->SetType(Variant::ARRAY_DOUBLE_TYPE);
	auto& aryValue = pVarOut->GetArrayValue();
	aryValue.resize(m_lKDataCount, VARIANT_ITEM());

	std::vector<double> aryCol;
	long lRowCount = 0, lIndex = 0;
	double dValue = 0;
	for (int i = 0, j = 0; i < m_lKDataCount; ++i)
	{
		aryCol.clear();
		for (j = 0; j < lCount; ++j)
		{
			const auto& pRefSymbol = m_aryData[j];

			const auto& item = pRefSymbol->_aryData[i];
			if (!item._bVaild) continue;

			aryCol.push_back(item._dValue);
		}

		if (!aryCol.empty())
		{
			std::sort(aryCol.begin(), aryCol.end());
			lRowCount = (long)aryCol.size();
			if (lRowCount % 2 == 0)
			{
				lIndex = lRowCount / 2;
				dValue = (aryCol[lIndex]+aryCol[lIndex-2]) / 2;
			}
			else
			{
				lIndex = lRowCount / 2 + 1;
				dValue = aryCol[lIndex];
			}

			aryValue[i].SetValue(dValue);
		}
	}

	return true;
}




}
}