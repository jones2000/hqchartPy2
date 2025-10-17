/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	��Դ��Ŀ https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	���ú�����-�ű����� (c++)
*/

#include "HQChart.data.h"
#include "HQChart.Execute.h"
#include "HQChart.Log.h"
#include "HQChart.KLineData.h"
#include <sstream>
#include <iterator>
#include <regex>

////////////////////////////////////////////////////////////////////////////////
//	�Զ���ϵͳ�ű�����
//
///////////////////////////////////////////////////////////////////////////////


namespace HQChart { namespace Complier {

Variant* VariantOperator::CallScript(const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode)
{
	long lCount = (long)args.size();
	if (lCount < 1)
	{
		ExecuteExcept error(L"ָ�����ֲ���Ϊ��", L"VariantOperator::CallScript", pNode);
		throw error;
	}

	const Variant* pFunctionName = args[0];
	if (!pFunctionName->IsVaildString())
	{
		ExecuteExcept error(L"ָ��������Ч", L"[VariantOperator::CallScript]", pNode);
		throw error;
	}

	std::wstring strFunctionName = pFunctionName->GetStringValue();	//��������
	std::vector<double> aryValue;	//����
	double dValue = 0;
	for (long i = 1; i < lCount; ++i)
	{
		auto item = args[i];
		if (!item->IsVaildDoulbe())
		{
			std::wstringstream strError;
			strError << strFunctionName << L"������Ч";
			ExecuteExcept error(strError.str(), L"[VariantOperator::CallScript", pNode);
			throw error;
		}

		dValue = item->GetDoubleValue();
		aryValue.push_back(dValue);
	}

	const ScriptLibrary& scriptLib = ScriptLibrary::GetInstance();
	auto find=scriptLib.Get(strFunctionName);
	if (!find)
	{
		std::wstringstream strError;
		strError << L"ָ��'" << strFunctionName << L"'������.";
		ExecuteExcept error(strError.str(), L"[VariantOperator::CallScript", pNode);
		throw error;
	}

	const ScriptIndex::ARRAY_ARGUMENT& aryDefaultArgs = find->GetArgument();
	Execute::ARRAY_ARGUMENT aryArgument;
	for (long i = 0; i < (long)aryDefaultArgs.size(); ++i)
	{
		auto item = aryDefaultArgs[i];
		if (i < (long)aryValue.size())
		{
			item._dValue = aryValue[i];
		}
		aryArgument.push_back(item);
	}
	
	//ִ��
	try
	{
		Pool pool;
		Program* pProgram = HQChart::Complier::Parse(pool, find->GetCode());	//�ű�����

		Execute exec;
		exec.SetProgram(pProgram);
		exec.SetArguments(aryArgument);
		exec.SetHistoryData(pHistoryData);
		exec.Run();	//ִ�нű�

		const Execute::MAP_VARIANT& outVar = exec.GetOutVar();	//�������
		Variant* pResult = Create();
		for (auto item : outVar)
		{
			pResult->SetMemberValue(item.first, Clone(item.second));
		}

		return pResult;
	}
	catch (const HQChart::Complier::ParseExcept& e)	//����ʧ��
	{
		std::wstringstream strError;
		strError << L"ָ��'" << strFunctionName << L"'�������,������Ϣ:" << e.GetDescription();
		ExecuteExcept error(strError.str(), L"[VariantOperator::CallScript", pNode);
		throw error;
	}
	catch (const HQChart::Complier::ExecuteExcept& e)	//ִ��ʧ��
	{
		std::wstringstream strError;
		strError << L"ָ��'" << strFunctionName << L"'ִ�д���, ������Ϣ" << e.GetDescription();
		ExecuteExcept error(strError.str(), L"[VariantOperator::CallScript", pNode);
		throw error;
	}
	catch (...)
	{
		std::wstringstream strError;
		strError << L"ָ��'" << strFunctionName << L"'ִ���쳣.";
		ExecuteExcept error(strError.str(), L"[VariantOperator::CallScript", pNode);
		throw error;
	}

	return NULL;
}

INVOKE_INDEX_ITEM VariantOperator::GetFunctionInfo(const std::wstring& str, const IHistoryData* pHistoryData)
{
	//MIN1,MIN5,MIN15,MIN30,MIN60,DAY,WEEK,MONTH,SEASON,YEAR
	const std::map<std::wstring, long> MAP_PERIOD = 
	{
		{ L"DAY",0 },
		{ L"WEEK", 1 },
		{ L"WEEK2", 21},
		{ L"MONTH",2 },
		{ L"SEASON",9 },
		{ L"HALFYEAR", 22},
		{ L"YEAR", 3 },
		{ L"MIN1", 4},
		{ L"MIN5", 5 },
		{ L"MIN15", 6 },
		{ L"MIN30",7 },
		{ L"MIN60", 8 },

		{L"DAY2", 40002},
		{L"MULTIDAY", 40002},
		{L"DAY3", 40003},
		{L"DAY4", 40004},
		{L"DAY5", 40005},
		{L"DAY6", 40006},
		{L"DAY7", 40007},
		{L"DAY8", 40008},
		{L"DAY9", 40009},
		{L"DAY10", 40010},
		{L"DAY11", 40011},
		{L"DAY12", 40012},
		{L"DAY13", 40013},
		{L"DAY14", 40014},
		{L"DAY15", 40015},
	};

	//��ʽ MA.MA1#week
	INVOKE_INDEX_ITEM item;
	item._lPeriod = pHistoryData->GetPeriod();
	item._lRight = pHistoryData->GetRight();

	std::vector<std::wstring> aryMain = StringSplit(str, L"#");
	std::vector<std::wstring> arySub = StringSplit(aryMain[0], L"\\.");

	item._strName = arySub[0];
	if (arySub.size() >= 2) item._strReturn = arySub[1];
	if (aryMain.size() >= 2)
	{
		std::wstring strPeriod = aryMain[1];
		std::transform(strPeriod.begin(), strPeriod.end(), strPeriod.begin(), ::toupper);
		auto find = MAP_PERIOD.find(strPeriod);
		if (find != MAP_PERIOD.end())
		{
			item._lPeriod = find->second;
			item._strPeriod = strPeriod;
		}
	}

	return item;
}

bool VariantOperator::IsVaildPeriod(long lPeriod, long lIndexPeriod) const
{
	const std::map<long, long> MAP_PERIOD_ORDER =
	{
		{ PERIOD_TYPE_ID::PERIOD_DAY_ID, 1000 },
		{ PERIOD_TYPE_ID::PERIOD_DAY2_ID, 1002 },
		{ PERIOD_TYPE_ID::PERIOD_DAY3_ID, 1003 },
		{ PERIOD_TYPE_ID::PERIOD_DAY4_ID, 1004 },
		{ PERIOD_TYPE_ID::PERIOD_DAY5_ID, 1005 },
		{ PERIOD_TYPE_ID::PERIOD_WEEK_ID, 1005 },

		{ PERIOD_TYPE_ID::PERIOD_DAY6_ID, 1006 },
		{ PERIOD_TYPE_ID::PERIOD_DAY7_ID, 1007 },
		{ PERIOD_TYPE_ID::PERIOD_DAY8_ID, 1008 },
		{ PERIOD_TYPE_ID::PERIOD_DAY9_ID, 1009 },
		{ PERIOD_TYPE_ID::PERIOD_TWO_WEEK_ID, 1010 },
		{ PERIOD_TYPE_ID::PERIOD_DAY10_ID, 1010 },

		{ PERIOD_TYPE_ID::PERIOD_DAY11_ID, 1011 },
		{ PERIOD_TYPE_ID::PERIOD_DAY12_ID, 1012 },
		{ PERIOD_TYPE_ID::PERIOD_DAY13_ID, 1013 },
		{ PERIOD_TYPE_ID::PERIOD_DAY14_ID, 1014 },
		{ PERIOD_TYPE_ID::PERIOD_DAY15_ID, 1015 },

		{ PERIOD_TYPE_ID::PERIOD_MONTH_ID, 1030 },
		{ PERIOD_TYPE_ID::PERIOD_QUARTER_ID, 1090 },
		{ PERIOD_TYPE_ID::PERIOD_HALF_YEAR_ID, 1180 },
		{ PERIOD_TYPE_ID::PERIOD_YEAR_ID, 1365 },

		{ PERIOD_TYPE_ID::PERIOD_MIN1_ID, 10 },
		{ PERIOD_TYPE_ID::PERIOD_MIN5_ID, 11 },
		{ PERIOD_TYPE_ID::PERIOD_MIN15_ID, 12 },
		{ PERIOD_TYPE_ID::PERIOD_MIN30_ID, 13 },
		{ PERIOD_TYPE_ID::PERIOD_MIN60_ID, 14 },
	};

	if (lPeriod == lIndexPeriod) return true;

	//�������ڱ���Ҫ���ڵ�ǰ����
	auto find = MAP_PERIOD_ORDER.find(lPeriod);
	auto find2 = MAP_PERIOD_ORDER.find(lIndexPeriod);
	if (find == MAP_PERIOD_ORDER.end() || find2 == MAP_PERIOD_ORDER.end()) return false;
	if (find2->second < find->second) return false;

	return true;
}


std::vector<std::wstring>  VariantOperator::StringSplit(const std::wstring& str, const std::wstring& strPattern)
{
	std::wregex re{ strPattern };
	return std::vector<std::wstring> {
		std::wsregex_token_iterator(str.begin(), str.end(), re, -1),
			std::wsregex_token_iterator()
	};
}

long VariantOperator::StringSplitV2(const std::wstring& strSrc, const std::wstring& strSplit, std::vector<std::wstring>& aryValue)
{
	aryValue.clear();
	if (strSplit.empty()) return 0;

	std::wstring str = strSrc + strSplit;
	//���ַ���ĩβҲ����ָ����������ȡ���һ��
	size_t pos = str.find(strSplit);
	std::wstring strTemp;
	while (pos != std::wstring::npos)
	{
		strTemp = str.substr(0, pos);
		aryValue.push_back(strTemp);

		//ȥ���ѷָ���ַ���,��ʣ�µ��ַ����н��зָ�
		str = str.substr(pos + 1, str.size());
		pos = str.find(strSplit);
	}

	return (long)aryValue.size();
}


//ָ������.
//�÷�:CALCSTOCKINDEX(Ʒ�ִ���, ָ������, ָ����), ���ظ�ָ����Ӧ����ļ���ֵ.
//Ʒ�ִ���Ϊ��""ʹ�õ�ǰƷ�ִ���
//���� :
//	CALCSTOCKINDEX('SH600000', 'KDJ', 3)��ʾ��֤600000��KDJָ���3�������Jֵ֮, ��һ����������ǰ���SZ(����), SH(����), ���г�_, ,
Variant* VariantOperator::CALCSTOCKINDEX(const Variant& varSymbol, const Variant& varIndexName, const Variant& varOutID, const IHistoryData* pHistoryData, Node* pNode)
{
	if (!varSymbol.IsVaildString())
	{
		ExecuteExcept error(L"CALCSTOCKINDEX() Ʒ�ִ�����Ч", L"VariantOperator::CALCSTOCKINDEX", pNode);
		throw error;
	}

	if (!varIndexName.IsVaildString())
	{
		ExecuteExcept error(L"CALCSTOCKINDEX() ָ��������Ч", L"VariantOperator::CALCSTOCKINDEX", pNode);
		throw error;
	}

	long lOutID = 0;
	if (!varOutID.IsVaildDoulbe())
	{
		ExecuteExcept error(L"CALCSTOCKINDEX() ���������Ч", L"VariantOperator::CALCSTOCKINDEX", pNode);
		throw error;
	}
	else
	{
		lOutID = (long)varOutID.GetDoubleValue();
		if (lOutID < 0)
		{
			ExecuteExcept error(L"CALCSTOCKINDEX() ���������������>=0", L"VariantOperator::CALCSTOCKINDEX", pNode);
			throw error;
		}
	}

	std::wstring strSymbol = varSymbol.GetStringValue();		//��Ʊ
	if (strSymbol.empty())	//�մ��� ʹ��Ŀǰ�Ĺ�Ʊ
		strSymbol = pHistoryData->GetSymbolString();

	if (strSymbol.find(L"SH") == 0) strSymbol = strSymbol.substr(2) + L".SH";
	else if (strSymbol.find(L"SZ") == 0) strSymbol = strSymbol.substr(2) + L".SZ";

	std::wstring strIndexName = varIndexName.GetStringValue();	//ָ������
	std::wstringstream strBuffer;
	strBuffer << L"CALCSTOCKINDEX('" << varSymbol.GetStringValue() << "','" << strIndexName << L"'," << (long)(varOutID.GetDoubleValue()) << L")";
	std::wstring strCallInfo = strBuffer.str();

	std::wstring strKey = strSymbol + L"-" + strIndexName;
	auto find = m_mapCALCSTOCKINDEX.find(strKey);
	if (find != m_mapCALCSTOCKINDEX.end())
	{
		int nOutVarCount=(int)find->second.size();
		if (lOutID >= nOutVarCount)
		{
			std::wstringstream strError;
			strError << L"ָ�� " << strCallInfo << L" �������������." << L"lOutID=" << lOutID << L", nOutVarCount=" << nOutVarCount;
			ExecuteExcept error(strError.str(), L"VariantOperator::CALCSTOCKINDEX", pNode);
			throw error;
		}

		TRACE_DEBUG(L"[VariantOperator::CALCSTOCKINDEX] %ls read in cache m_mapCALCSTOCKINDEX. key=%ls", strCallInfo.c_str(), strKey.c_str());
		return find->second[lOutID];
	}


	ScriptIndex script;
	if (!pHistoryData->GetIndexScript(strIndexName, strCallInfo, script, pNode))
	{
		std::wstringstream strError;
		strError << L"ָ��" << strCallInfo << L"������. ";
		ExecuteExcept error(strError.str(), L"VariantOperator::CALCSTOCKINDEX", pNode);
		throw error;
	}

	InvokeIndex IndexInfo;
	IndexInfo.SetIndex(strIndexName, script.GetCode());
	IndexInfo.SetArguments(script.GetArgument());
	IndexInfo.SetSymbol(strSymbol);
	IndexInfo.SetPeriod(pHistoryData->GetPeriod());
	IndexInfo.SetRight(pHistoryData->GetRight());
	IndexInfo.SetDataClassName(pHistoryData->GetClassName());

	HQChart::Complier::AutoPtrHistoryData ptrHistoryData(IndexInfo.GetDataClassName());
	HQChart::Complier::IHistoryData* pTempHistoryData = ptrHistoryData.Create(IndexInfo.GetSymbol(), IndexInfo.GetPeriod(), IndexInfo.GetRight());
	pTempHistoryData->Initialization((void*)pHistoryData);
	Execute exec;
	ExecuteIndex(IndexInfo, &exec, pTempHistoryData, pNode);

	const Execute::MAP_VARIANT& outVar = exec.GetOutVar();	//�������
	const auto& aryOutVarName = exec.GetOutVarInfo();
	if (aryOutVarName.empty())
	{
		std::wstringstream strError;
		strError << L"ָ��" << strCallInfo << L"ָ���������Ϊ0.";
		ExecuteExcept error(strError.str(), L"VariantOperator::CALCSTOCKINDEX", pNode);
		throw error;
	}

	int nVarCount = (int)aryOutVarName.size();
	if (lOutID >= nVarCount)
	{
		std::wstringstream strError;
		strError << L"ָ��" << strCallInfo << L" �������������." << L"lOutID=" << lOutID << L", nVarCount=" << nVarCount;
		ExecuteExcept error(strError.str(), L"VariantOperator::CALCSTOCKINDEX", pNode);
		throw error;
	}

	m_mapCALCSTOCKINDEX[strKey] = ARRAY_INDEX_VARIANT();
	find = m_mapCALCSTOCKINDEX.find(strKey);

	IHistoryData::ARRAY_FIT_DATETIME_DATA aryDateTime;
	IHistoryData::FitDateTime(pTempHistoryData, pHistoryData, aryDateTime);

	Variant* pResult = NULL;
	for (int i = 0; i < nVarCount; ++i)
	{
		const auto& item = aryOutVarName[i];
		Variant* pVariant = Clone(item.GetOutValue(), aryDateTime);
		find->second.push_back(pVariant);
		if (i == lOutID) pResult = pVariant;
	}

	TRACE_DEBUG(L"[VariantOperator::CALCSTOCKINDEX] %ls save in cache m_mapCALCSTOCKINDEX.key=%ls, Variant count=%d", strCallInfo.c_str(), strKey.c_str(), find->second.size());

	return pResult;
}

/*
�÷�:STKINDI('STKNAME','INDINAME',PARAMs)��STKNAME�ǹ�Ʊ���룬INDINAME��ָ�꼰��ָ��������,PARAM�ǿ�ѡ������������Ӧ�����ڱ�Ӧ��ָ��Ĳ�������������ʹ��ȱʡ������ֵ��
����:STKINDI('SZ300059','MA.MA1',5,10,15,20)��ʾ���ö����Ƹ���MAָ���MA1ָ���ߣ��������5��10��15��20��STKINDI('sz300059','KDJ.K#WEEK',9,4,4)��ʾ���ö����Ƹ�������KDJָ���Kָ���ߣ��������9,4,4��
*/
Variant* VariantOperator::STKINDI(const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode)
{
	long lCount = (long)args.size();
	if (lCount < 2)
	{
		ExecuteExcept error(L"STKINDI() ������������", L"VariantOperator::STKINDI", pNode);
		throw error;
	}

	const Variant* pSymbol = args[0];
	const Variant* pFunction = args[1];
	if (!pSymbol->IsVaildString())
	{
		ExecuteExcept error(L"STKINDI() Ʒ�ִ�����Ч", L"VariantOperator::STKINDI", pNode);
		throw error;
	}

	if (!pFunction->IsVaildString())
	{
		ExecuteExcept error(L"STKINDI() ָ������Ч", L"VariantOperator::STKINDI", pNode);
		throw error;
	}

	std::wstring strSymbol = pSymbol->GetStringValue();		//��Ʊ
	if (strSymbol.empty())	//�մ��� ʹ��Ŀǰ�Ĺ�Ʊ
		strSymbol = pHistoryData->GetSymbolString();
	else
		std::transform(strSymbol.begin(), strSymbol.end(), strSymbol.begin(), ::toupper);


	if (strSymbol.find(L"SH") == 0) strSymbol = strSymbol.substr(2) + L".SH";
	else if (strSymbol.find(L"SZ") == 0) strSymbol = strSymbol.substr(2) + L".SZ";
	auto funcInfo = GetFunctionInfo(pFunction->GetStringValue(), pHistoryData);

	std::wstring strCallInfo = L"STKINDI('" + pSymbol->GetStringValue() + L"','" + pFunction->GetStringValue() + L"', ...)";

	//�������ڱ���Ҫ���ڵ�ǰ����
	if (!IsVaildPeriod(pHistoryData->GetPeriod(), funcInfo._lPeriod))
	{
		std::wstringstream strError;
		strError << strCallInfo<< L" ���ڴ���";
		ExecuteExcept error(strError.str(), L"VariantOperator::STKINDI", pNode);
		throw error;
	}

	std::wstring strParam, strIndexKey, strKey;
	std::wstringstream buffer;
	std::vector<double> aryValue;	//����
	double dValue = 0;
	for (long i = 2; i < lCount; ++i)
	{
		auto item = args[i];
		if (!item->IsVaildDoulbe())
		{
			std::wstringstream strError;
			strError << strCallInfo<< L"ָ�������Ч.";
			ExecuteExcept error(strError.str(), L"VariantOperator::STKINDI", pNode);
			throw error;
		}

		
		dValue = item->GetDoubleValue();
		if (!aryValue.empty()) buffer << L"," << dValue;
		else buffer << dValue;

		aryValue.push_back(dValue);
	}
	strParam = buffer.str();
	strIndexKey = funcInfo._strName + L"(" + strParam + L")";

	auto find = m_mapSTKINDI.find(strIndexKey);
	if (find != m_mapSTKINDI.end())
	{
		strKey = funcInfo._strReturn;
		if (!funcInfo._strPeriod.empty()) strKey += L"#" + funcInfo._strPeriod;
		auto findVar = find->second.find(strKey);
		if (findVar != find->second.end())
		{
			return findVar->second;
		}
	}

	ScriptIndex script;
	if (!pHistoryData->GetIndexScript(funcInfo._strName, strCallInfo, script, pNode))
	{
		std::wstringstream strError;
		strError << strCallInfo << L"ָ�겻����. ";
		ExecuteExcept error(strError.str(), L"VariantOperator::STKINDI", pNode);
		throw error;
	}

	const auto& aryDefaultArgs = script.GetArgument();
	Execute::ARRAY_ARGUMENT aryArgument;
	for (long i = 0; i < (long)aryDefaultArgs.size(); ++i)
	{
		auto item = aryDefaultArgs[i];
		if (i < (long)aryValue.size())
		{
			item._dValue = aryValue[i];
		}
		aryArgument.push_back(item);
	}

	InvokeIndex IndexInfo;
	IndexInfo.SetIndex(funcInfo._strName, script.GetCode());
	IndexInfo.SetArguments(aryArgument);
	IndexInfo.SetSymbol(strSymbol);
	IndexInfo.SetPeriod(funcInfo._lPeriod);
	IndexInfo.SetRight(funcInfo._lRight);
	IndexInfo.SetDataClassName(pHistoryData->GetClassName());

	HQChart::Complier::AutoPtrHistoryData ptrHistoryData(IndexInfo.GetDataClassName());
	HQChart::Complier::IHistoryData* pTempHistoryData = ptrHistoryData.Create(IndexInfo.GetSymbol(), IndexInfo.GetPeriod(), IndexInfo.GetRight());
	pTempHistoryData->Initialization((void*)pHistoryData);
	Execute exec;
	ExecuteIndex(IndexInfo, &exec, pTempHistoryData, pNode);

	IHistoryData::ARRAY_FIT_DATETIME_DATA aryDateTime;
	IHistoryData::FitDateTime(pTempHistoryData, pHistoryData, aryDateTime);

	const Execute::MAP_VARIANT& outVar = exec.GetOutVar();	//�������
	const auto varFind = outVar.find(funcInfo._strReturn);

	find = m_mapSTKINDI.find(strIndexKey);
	if (find == m_mapSTKINDI.end())
	{
		m_mapSTKINDI[strIndexKey] = MAP_INDEX_VARIANT();
		find = m_mapSTKINDI.find(strIndexKey);
	}

	Variant* pResult = NULL;
	for (auto item : outVar)
	{
		Variant* pVariant = Clone(item.second, aryDateTime);
		strKey = item.first;
		if (!funcInfo._strPeriod.empty()) strKey += L"#" + funcInfo._strPeriod;
		find->second[strKey] = pVariant;

		if (item.first == funcInfo._strReturn) pResult = pVariant;
	}

	if (pResult == NULL)
	{
		pResult = Create();
		TRACE_DEBUG(L"[VariantOperator::STKINDI]  %ls can't find out '%ls' ", strIndexKey.c_str(), strKey.c_str());
	}

	return pResult;
}

Variant* VariantOperator::CallIndex(const std::wstring& strCallInfo, const IHistoryData* pHistoryData, Node* pNode)
{
	INVOKE_INDEX_ITEM funcInfo = GetFunctionInfo(strCallInfo, pHistoryData);
	if (funcInfo._strReturn.empty())
	{
		std::wstringstream strError;
		strError << strCallInfo << L",ָ���������Ϊ��";
		ExecuteExcept error(strError.str(), L"VariantOperator::CallIndex", pNode);
		throw error;
	}

	//�������ڱ���Ҫ���ڵ�ǰ����
	if (!IsVaildPeriod(pHistoryData->GetPeriod(), funcInfo._lPeriod))
	{
		std::wstringstream strError;
		strError << strCallInfo << L" ���ڴ���";
		ExecuteExcept error(strError.str(), L"VariantOperator::CallIndex", pNode);
		throw error;
	}

	//������
	std::wstring strKey;
	auto find = m_mapIndex.find(funcInfo._strName);
	if (find != m_mapIndex.end())
	{
		strKey = funcInfo._strReturn;
		if (!funcInfo._strPeriod.empty()) strKey += L"#" + funcInfo._strPeriod;
		auto findVar=find->second.find(strKey);
		if (findVar != find->second.end())
		{
			return findVar->second;
		}
	}

	ScriptIndex script;
	if (!pHistoryData->GetIndexScript(funcInfo._strName, strCallInfo, script, pNode))
	{
		std::wstringstream strError;
		strError << strCallInfo << L"ָ�겻����. ";
		ExecuteExcept error(strError.str(), L"VariantOperator::CallIndex", pNode);
		throw error;
	}

	InvokeIndex IndexInfo;
	IndexInfo.SetIndex(funcInfo._strName, script.GetCode());
	IndexInfo.SetArguments(script.GetArgument());
	IndexInfo.SetSymbol(pHistoryData->GetSymbolString());
	IndexInfo.SetPeriod(funcInfo._lPeriod);
	IndexInfo.SetRight(funcInfo._lRight);
	IndexInfo.SetDataClassName(pHistoryData->GetClassName());

	HQChart::Complier::AutoPtrHistoryData ptrHistoryData(IndexInfo.GetDataClassName());
	HQChart::Complier::IHistoryData* pTempHistoryData = ptrHistoryData.Create(IndexInfo.GetSymbol(), IndexInfo.GetPeriod(), IndexInfo.GetRight());
	if (!pTempHistoryData)
	{
		std::wstringstream strError;
		strError << strCallInfo << L"create data class failed. ClassName=" << IndexInfo.GetDataClassName();
		ExecuteExcept error(strError.str(), L"VariantOperator::CallIndex", pNode);
		throw error;
	}

	pTempHistoryData->Initialization((void*)pHistoryData);
	Execute exec;
	ExecuteIndex(IndexInfo, &exec, pTempHistoryData, pNode);

	IHistoryData::ARRAY_FIT_DATETIME_DATA aryDateTime;
	IHistoryData::FitDateTime(pTempHistoryData, pHistoryData, aryDateTime);

	const Execute::MAP_VARIANT& outVar = exec.GetOutVar();	//�������

	const auto varFind = outVar.find(funcInfo._strReturn);
	if (varFind == outVar.end())
	{
		std::wstringstream strError;
		strError << strCallInfo << L"ָ���������'" << funcInfo._strReturn << L"'������.";
		ExecuteExcept error(strError.str(), L"VariantOperator::CallIndex", pNode);
		throw error;
	}

	find = m_mapIndex.find(funcInfo._strName);
	if (find == m_mapIndex.end())
	{
		m_mapIndex[funcInfo._strName] = MAP_INDEX_VARIANT();
		find = m_mapIndex.find(funcInfo._strName);
	}
		
	Variant* pResult = NULL;
	for (auto item : outVar)
	{
		Variant* pVariant = Clone(item.second, aryDateTime);
		strKey = item.first;
		if (!funcInfo._strPeriod.empty()) strKey += L"#" + funcInfo._strPeriod;
		find->second[strKey] = pVariant;

		if (item.first == funcInfo._strReturn) pResult = pVariant;
	}
	
	//Variant* pResult = Create();
	//varFind->second->CopyTo(*pResult, aryDateTime, this);
	
	return pResult;
}

bool VariantOperator::ExecuteIndex(const InvokeIndex& index, Execute* pExec, HQChart::Complier::IHistoryData* pTempHistoryData, Node* pNode)
{
	//ִ��
	try
	{
		if (!pTempHistoryData || !pTempHistoryData->LoadKData())
		{
			std::wstringstream strError;
			strError << L"K�߶�ȡʧ��. symbol= " << index.GetSymbol() << L" Period=" << index.GetPeriod();
			ExecuteExcept error(strError.str(), L"VariantOperator::ExecuteIndex", pNode);
			throw error;
		}

		Pool pool;
		Program* pProgram = HQChart::Complier::Parse(pool, index.GetCode());	//�ű�����

		pExec->SetProgram(pProgram);
		pExec->SetArguments(index.GetArguments());
		pExec->SetHistoryData(pTempHistoryData);
		pExec->Run();	//ִ�нű�

		return true;
	}
	catch (const HQChart::Complier::ParseExcept& e)	//����ʧ��
	{
		std::wstringstream strError;
		strError << L"ָ��'" << index.GetIndexName() << L"'����ʧ��,������Ϣ:" << e.GetDescription();
		ExecuteExcept error(strError.str(), L"VariantOperator::ExecuteIndex", pNode);
		throw error;
	}
	catch (const HQChart::Complier::ExecuteExcept& e)	//ִ��ʧ��
	{
		std::wstringstream strError;
		strError << L"ָ��'" << index.GetIndexName() << L"'ִ��ʧ��,������Ϣ:" << e.GetDescription();
		ExecuteExcept error(strError.str(), L"VariantOperator::ExecuteIndex", pNode);
		throw error;
	}
	catch (...)
	{
		std::wstringstream strError;
		strError << L"ָ��'" << index.GetIndexName() << L"'ִ���쳣.";
		ExecuteExcept error(strError.str(), L"VariantOperator::ExecuteIndex", pNode);
		throw error;
	}

	return false;
}

Variant* VariantOperator::RPCCall(const Variant::ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode)
{
	Variant* pResult = pHistoryData->RPCCall(args, pNode);

	if (pResult == NULL)
	{
		pResult = Create();
		TRACE_DEBUG(L"[VariantOperator::RPCCall] RPC call %s, failed. ");
	}

	return pResult;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//

void InvokeIndex::SetSymbol(const std::wstring& strValue)
{
	m_strSymbol = strValue;
}

void InvokeIndex::SetIndex(const std::wstring& strName, const std::wstring& strCode)
{
	m_strIndexName = strName;
	m_strCode = strCode;
}

void InvokeIndex::SetPeriod(long lValue)
{
	m_lPeriod = lValue;
}

void InvokeIndex::SetRight(long lValue)
{
	m_lRight = lValue;
}

void InvokeIndex::SetArguments(const ARRAY_ARGUMENT& args)
{
	m_args = args;
}

void InvokeIndex::SetDataClassName(const std::wstring& strValue)
{
	m_strDataClassName = strValue;
}

const std::wstring& InvokeIndex::GetDataClassName() const
{
	return m_strDataClassName;
}

const std::wstring& InvokeIndex::GetSymbol() const
{
	return m_strSymbol;
}

const std::wstring& InvokeIndex::GetCode() const
{
	return m_strCode;
}

const std::wstring& InvokeIndex::GetIndexName() const
{
	return m_strIndexName;
}

long InvokeIndex::GetPeriod() const
{
	return m_lPeriod;
}

long InvokeIndex::GetRight() const
{
	return m_lRight;
}

InvokeIndex::ARRAY_ARGUMENT& InvokeIndex::GetArguments()
{
	return m_args;
}

const InvokeIndex::ARRAY_ARGUMENT& InvokeIndex::GetArguments() const
{
	return m_args;
}


} }