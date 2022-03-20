/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

指标调用配置类 (c++)
*/

#include "HQChart.OutVarToJson.h"
#include <locale>
#include <codecvt>

namespace HQChart { namespace Complier {


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

void OutVarToJsonHelper::ArrayToJson(const std::string& strKey, const Variant* pValue, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter)
{
	jsonWriter.Key(strKey.c_str());
	jsonWriter.StartArray();
	if (pValue)
	{
		if (pValue->GetType() == HQChart::Complier::Variant::ARRAY_DOUBLE_TYPE)
		{
			bool bNull = true;
			for (int i = nStart; i < nKCount; ++i)
			{
				bNull = true;
				if (i < (long)pValue->GetArrayValue().size())
				{
					const auto& varItem = pValue->GetArrayValue()[i];
					if (varItem.IsVaild())
					{
						jsonWriter.Double(varItem._dValue);
						bNull = false;
					}
				}

				if (bNull) 
					jsonWriter.Null();
			}
		}
		else if (pValue->GetType() == HQChart::Complier::Variant::DOUBLE_TYPE)
		{
			if (pValue->IsVaildDoulbe())
			{
				double dValue = pValue->GetDoubleValue();
				for (int i = nStart; i < nKCount; ++i)
				{
					jsonWriter.Double(dValue);
				}
			}
		}
		else if (pValue->GetType() == HQChart::Complier::Variant::STRING_TYPE)
		{
			if (pValue->IsVaildString())
			{
				std::wstring strValue = pValue->GetStringValue();
				std::string strUtf8Value = WStringToUTF8(strValue);
				for (int i = nStart; i < nKCount; ++i)
				{
					jsonWriter.String(strUtf8Value.c_str());
				}
			}
		}
		else if (pValue->GetType() == HQChart::Complier::Variant::ARRAY_STRING_TYPE)
		{
			bool bNull = true;
			for (int i = nStart; i < nKCount; ++i)
			{
				bNull = true;
				if (i < (long)pValue->GetArrayValue().size())
				{
					const auto& varItem = pValue->GetArrayValue()[i];
					if (varItem.IsVaild())
					{
						jsonWriter.String(WStringToUTF8(*varItem._pString).c_str());
						bNull = false;
					}
				}

				if (bNull) jsonWriter.Null();
			}
		}
	}
	jsonWriter.EndArray();
}

void OutVarToJsonHelper::VariantDataToJson(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter)
{
	jsonWriter.Key("Data");
	jsonWriter.StartArray();

	bool bNull = true;
	for (int i = nStart; i < nKCount; ++i)
	{
		bNull = true;
		if (pVariant->GetType() == HQChart::Complier::Variant::ARRAY_DOUBLE_TYPE)	//数组类型
		{
			if (i < (long)pVariant->GetArrayValue().size())
			{
				const auto& varItem = pVariant->GetArrayValue()[i];
				if (varItem.IsVaild())
				{
					jsonWriter.Double(varItem._dValue);
					bNull = false;
				}
			}
		}
		else if (pVariant->GetType() == HQChart::Complier::Variant::DOUBLE_TYPE)		//单数值类型
		{
			if (pVariant->IsVaildDoulbe())
			{
				jsonWriter.Double(pVariant->GetDoubleValue());
				bNull = false;
			}
		}
		else if (pVariant->GetType() == HQChart::Complier::Variant::ARRAY_STRING_TYPE)	//字符串数组
		{
			const auto& varItem = pVariant->GetArrayValue()[i];
			if (varItem.IsVaild())
			{
				jsonWriter.String(WStringToUTF8(*varItem._pString).c_str());
				bNull = false;
			}
		}
		else if (pVariant->GetType() == HQChart::Complier::Variant::STRING_TYPE)
		{
			if (pVariant->IsVaildString())
			{
				jsonWriter.String(WStringToUTF8(pVariant->GetStringValue()).c_str());
				bNull = false;
			}
		}
		else if (pVariant->GetType() == HQChart::Complier::Variant::MEMBER_TYPE)
		{
			bNull = false;
		}

		if (bNull) jsonWriter.Null();
	}

	jsonWriter.EndArray();
}

void OutVarToJsonHelper::DRAWKLINE(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter)
{
	const auto pDrawData = pVariant->GetDrawData();
	if (!pDrawData) return;

	const auto& mapVariant = pDrawData->_mapVaraint;
	const Variant* pHigh = NULL;
	const Variant* pLow = NULL;
	const Variant* pOpen = NULL;
	const Variant* pClose = NULL;

	auto find = mapVariant.find(L"High");
	if (find != mapVariant.end()) pHigh = find->second;

	find = mapVariant.find(L"Low");
	if (find != mapVariant.end()) pLow = find->second;

	find = mapVariant.find(L"Open");
	if (find != mapVariant.end()) pOpen = find->second;

	find = mapVariant.find(L"Close");
	if (find != mapVariant.end()) pClose = find->second;

	jsonWriter.Key("Draw");
	jsonWriter.StartObject();
		ArrayToJson("High", pHigh, nStart, nKCount, jsonWriter);
		ArrayToJson("Low", pLow, nStart, nKCount, jsonWriter);
		ArrayToJson("Open", pOpen, nStart, nKCount, jsonWriter);
		ArrayToJson("Close", pClose, nStart, nKCount, jsonWriter);
	jsonWriter.EndObject();
}

void OutVarToJsonHelper::DRAWCOLORKLINE(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter)
{
	const auto pDrawData = pVariant->GetDrawData();
	if (!pDrawData) return;

	const auto& mapVariant = pDrawData->_mapVaraint;
	const Variant* pHigh = NULL;
	const Variant* pLow = NULL;
	const Variant* pOpen = NULL;
	const Variant* pClose = NULL;
	const Variant* pColor = NULL;
	long lEmpty = 0;
	std::wstring strColor;

	auto find = mapVariant.find(L"High");
	if (find != mapVariant.end()) pHigh = find->second;

	find = mapVariant.find(L"Low");
	if (find != mapVariant.end()) pLow = find->second;

	find = mapVariant.find(L"Open");
	if (find != mapVariant.end()) pOpen = find->second;

	find = mapVariant.find(L"Close");
	if (find != mapVariant.end()) pClose = find->second;

	find = mapVariant.find(L"Color");
	if (find != mapVariant.end()) find->second->TryGetStringValue(strColor, L"");

	find = mapVariant.find(L"Empty");
	if (find != mapVariant.end()) find->second->TryGetLongValue(lEmpty, 0);

	jsonWriter.Key("Draw");
	jsonWriter.StartObject();
		ArrayToJson("High", pHigh, nStart, nKCount, jsonWriter);
		ArrayToJson("Low", pLow, nStart, nKCount, jsonWriter);
		ArrayToJson("Open", pOpen, nStart, nKCount, jsonWriter);
		ArrayToJson("Close", pClose, nStart, nKCount, jsonWriter);

		jsonWriter.Key("Empty");
		jsonWriter.Int(lEmpty);

		jsonWriter.Key("Color");
		jsonWriter.String(WStringToUTF8(strColor).c_str());
		
	jsonWriter.EndObject();
}

void OutVarToJsonHelper::STICKLINE(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter)
{
	const auto pDrawData = pVariant->GetDrawData();
	if (!pDrawData) return;

	double dWidth = 1;
	long lEmpty = 0;
	const auto& mapVariant = pDrawData->_mapVaraint;

	auto find = mapVariant.find(L"Width");
	if (find != mapVariant.end()) find->second->TryGetDobuleValue(dWidth, 1);
	if (dWidth < 0) dWidth = 0;

	find = mapVariant.find(L"Empty");
	if (find != mapVariant.end()) find->second->TryGetLongValue(lEmpty, 0);

	const Variant* pPrice = NULL;
	const Variant* pPrice2 = NULL;
	find = mapVariant.find(L"Price");
	if (find != mapVariant.end()) pPrice = find->second;

	find = mapVariant.find(L"Price2");
	if (find != mapVariant.end()) pPrice2 = find->second;

	jsonWriter.Key("Draw");
	jsonWriter.StartObject();

		jsonWriter.Key("Width");
		jsonWriter.Double(dWidth);

		jsonWriter.Key("Empty");
		jsonWriter.Int(lEmpty);

		ArrayToJson("Price", pPrice, nStart, nKCount, jsonWriter);
		ArrayToJson("Price2", pPrice2, nStart, nKCount, jsonWriter);

	jsonWriter.EndObject();
}

void OutVarToJsonHelper::DRAWICON(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter)
{
	const auto pDrawData = pVariant->GetDrawData();
	if (!pDrawData) return;

	const auto& mapVariant = pDrawData->_mapVaraint;
	const Variant* pIcon = NULL;
	const Variant* pPrice = NULL;

	auto find = mapVariant.find(L"Type");
	if (find != mapVariant.end()) pIcon = find->second;
	
	find = mapVariant.find(L"Price");
	if (find != mapVariant.end()) pPrice = find->second;

	jsonWriter.Key("Draw");
	jsonWriter.StartObject();

		jsonWriter.Key("Type");
		
		if (pIcon && pIcon->IsVaildDoulbe())
		{
			int nValue= (int)pIcon->GetDoubleValue();
			jsonWriter.Int(nValue);
		}
		else if (pIcon && pIcon->IsVaildString())
		{
			std::string strValue = WStringToUTF8(pIcon->GetStringValue());
			jsonWriter.String(strValue.c_str());
		}
		else
		{
			jsonWriter.Int(0);
		}

		ArrayToJson("Price", pPrice, nStart, nKCount, jsonWriter);

	jsonWriter.EndObject();
}

void OutVarToJsonHelper::DRAWTEXT(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter)
{
	const auto pDrawData = pVariant->GetDrawData();
	if (!pDrawData) return;

	const auto& mapVariant = pDrawData->_mapVaraint;

	const Variant* pPrice = NULL;
	const Variant* pText = NULL;
	auto find = mapVariant.find(L"Price");
	if (find != mapVariant.end()) pPrice = find->second;

	find = mapVariant.find(L"Text");
	if (find != mapVariant.end()) pText = find->second;

	jsonWriter.Key("Draw");
	jsonWriter.StartObject();
		
		std::string strText;
		if (pText)
		{
			if (pText->GetType() == HQChart::Complier::Variant::STRING_TYPE)
			{
				strText = WStringToUTF8(pText->GetStringValue());
				jsonWriter.Key("Text");
				jsonWriter.String(strText.c_str());
			}
			else if (pText->GetType() == HQChart::Complier::Variant::ARRAY_STRING_TYPE)
			{
				ArrayToJson("Text", pText, nStart, nKCount, jsonWriter);
			}
		}
		else
		{
			jsonWriter.Key("Text");
			jsonWriter.String(strText.c_str());
		}

		ArrayToJson("Price", pPrice, nStart, nKCount, jsonWriter);

	jsonWriter.EndObject();
}

void OutVarToJsonHelper::DRAWNUMBER(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter)
{
	const auto pDrawData = pVariant->GetDrawData();
	if (!pDrawData) return;

	const auto& mapVariant = pDrawData->_mapVaraint;

	const Variant* pPrice = NULL;
	const Variant* pNumber = NULL;
	auto find = mapVariant.find(L"Price");
	if (find != mapVariant.end()) pPrice = find->second;

	find = mapVariant.find(L"Number");
	if (find != mapVariant.end()) pNumber = find->second;

	jsonWriter.Key("Draw");
	jsonWriter.StartObject();

	ArrayToJson("Number", pNumber, nStart, nKCount, jsonWriter);
	ArrayToJson("Price", pPrice, nStart, nKCount, jsonWriter);

	jsonWriter.EndObject();
}

void OutVarToJsonHelper::DRAWBAND(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter)
{
	const auto pDrawData = pVariant->GetDrawData();
	if (!pDrawData) return;

	const auto& mapVariant = pDrawData->_mapVaraint;

	const Variant* pVal1 = NULL;
	const Variant* pVal2 = NULL;
	const Variant* pColor1 = NULL;
	const Variant* pColor2 = NULL;
	auto find = mapVariant.find(L"Val1");
	if (find != mapVariant.end()) pVal1 = find->second;

	find = mapVariant.find(L"Val2");
	if (find != mapVariant.end()) pVal2 = find->second;

	find = mapVariant.find(L"Color1");
	if (find != mapVariant.end()) pColor1 = find->second;

	find = mapVariant.find(L"Color2");
	if (find != mapVariant.end()) pColor2 = find->second;

	jsonWriter.Key("Draw");
	jsonWriter.StartObject();

	ArrayToJson("Val1", pVal1, nStart, nKCount, jsonWriter);
	ArrayToJson("Val2", pVal2, nStart, nKCount, jsonWriter);

	std::string strColor;
	if (pColor1 && pColor1->GetType() == HQChart::Complier::Variant::STRING_TYPE)
		strColor = WStringToUTF8(pColor1->GetStringValue());

	jsonWriter.Key("Color1");
	jsonWriter.String(strColor.c_str());

	strColor.clear();
	if (pColor2 && pColor2->GetType() == HQChart::Complier::Variant::STRING_TYPE)
		strColor = WStringToUTF8(pColor2->GetStringValue());
	jsonWriter.Key("Color2");
	jsonWriter.String(strColor.c_str());

	jsonWriter.EndObject();
}

void OutVarToJsonHelper::DRAWTEXT_FIX(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter)
{
	const auto pDrawData = pVariant->GetDrawData();
	if (!pDrawData) return;

	const auto& mapVariant = pDrawData->_mapVaraint;

	double dX = 0, dY = 0;
	long lType = 0;
	const Variant* pY = NULL;
	const Variant* pType = NULL;
	const Variant* pCond = NULL;
	const Variant* pText = NULL;

	auto find = mapVariant.find(L"Cond");
	if (find != mapVariant.end()) pCond = find->second;

	find = mapVariant.find(L"Text");
	if (find != mapVariant.end()) pText = find->second;

	find = mapVariant.find(L"X");
	if (find != mapVariant.end()) find->second->TryGetDobuleValue(dX, 0);

	find = mapVariant.find(L"Y");
	if (find != mapVariant.end()) find->second->TryGetDobuleValue(dY, 0);

	find = mapVariant.find(L"Type");
	if (find != mapVariant.end()) find->second->TryGetLongValue(lType, 0);

	jsonWriter.Key("Draw");
	jsonWriter.StartObject();

		ArrayToJson("Cond", pCond, nStart, nKCount, jsonWriter);

		std::string strText;
		if (pText && pText->GetType() == HQChart::Complier::Variant::STRING_TYPE)
			strText = WStringToUTF8(pText->GetStringValue());

		jsonWriter.Key("Text");
		jsonWriter.String(strText.c_str());

		jsonWriter.Key("X");
		jsonWriter.Double(dX);

		jsonWriter.Key("Y");
		jsonWriter.Double(dY);

		jsonWriter.Key("Type");
		jsonWriter.Int(lType);

	jsonWriter.EndObject();
}

void OutVarToJsonHelper::DRAWNUMBER_FIX(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter)
{
	const auto pDrawData = pVariant->GetDrawData();
	if (!pDrawData) return;

	const auto& mapVariant = pDrawData->_mapVaraint;

	double dX = 0, dY = 0;
	long lType = 0;
	const Variant* pY = NULL;
	const Variant* pType = NULL;
	const Variant* pNumber= NULL;

	auto find = mapVariant.find(L"Number");
	if (find != mapVariant.end()) pNumber = find->second;

	find = mapVariant.find(L"X");
	if (find != mapVariant.end()) find->second->TryGetDobuleValue(dX, 0);

	find = mapVariant.find(L"Y");
	if (find != mapVariant.end()) find->second->TryGetDobuleValue(dY, 0);

	find = mapVariant.find(L"Type");
	if (find != mapVariant.end()) find->second->TryGetLongValue(lType, 0);

	jsonWriter.Key("Draw");
	jsonWriter.StartObject();

		ArrayToJson("Number", pNumber, nStart, nKCount, jsonWriter);
		
		jsonWriter.Key("X");
		jsonWriter.Double(dX);

		jsonWriter.Key("Y");
		jsonWriter.Double(dY);

		jsonWriter.Key("Type");
		jsonWriter.Int(lType);

	jsonWriter.EndObject();
}

void OutVarToJsonHelper::FILLRGN(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter)
{
	const auto pDrawData = pVariant->GetDrawData();
	if (!pDrawData) return;

	const auto& mapVariant = pDrawData->_mapVaraint;

	double dX = 0, dY = 0;
	long lType = 0;
	const Variant* pPrice = NULL;
	const Variant* pPrice2 = NULL;
	const Variant* pColor = NULL;

	auto find = mapVariant.find(L"Price");
	if (find != mapVariant.end()) pPrice = find->second;

	find = mapVariant.find(L"Price2");
	if (find != mapVariant.end()) pPrice2 = find->second;

	find = mapVariant.find(L"Color");
	if (find != mapVariant.end()) pColor = find->second;

	jsonWriter.Key("Draw");
	jsonWriter.StartObject();

		ArrayToJson("Price", pPrice, nStart, nKCount, jsonWriter);
		ArrayToJson("Price2", pPrice2, nStart, nKCount, jsonWriter);
		ArrayToJson("Color", pColor, nStart, nKCount, jsonWriter);

	jsonWriter.EndObject();
}

void OutVarToJsonHelper::PARTLINE(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter)
{
	const auto pDrawData = pVariant->GetDrawData();
	if (!pDrawData) return;

	const auto& mapVariant = pDrawData->_mapVaraint;

	double dX = 0, dY = 0;
	long lType = 0;
	const Variant* pPrice = NULL;
	const Variant* pColor = NULL;

	auto find = mapVariant.find(L"Price");
	if (find != mapVariant.end()) pPrice = find->second;

	find = mapVariant.find(L"Color");
	if (find != mapVariant.end()) pColor = find->second;

	jsonWriter.Key("Draw");
	jsonWriter.StartObject();

		ArrayToJson("Price", pPrice, nStart, nKCount, jsonWriter);
		ArrayToJson("Color", pColor, nStart, nKCount, jsonWriter);

	jsonWriter.EndObject();
}

void OutVarToJsonHelper::EXPLAIN(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter)
{
	const auto pDrawData = pVariant->GetDrawData();
	if (!pDrawData) return;

	const auto& mapVariant = pDrawData->_mapVaraint;

	const Variant* pCond = NULL;
	const Variant* pText = NULL;

	auto find = mapVariant.find(L"Cond");
	if (find != mapVariant.end()) pCond = find->second;

	find = mapVariant.find(L"Text");
	if (find != mapVariant.end()) pText = find->second;

	jsonWriter.Key("Draw");
	jsonWriter.StartObject();

		ArrayToJson("Cond", pCond, nStart, nKCount, jsonWriter);

		std::string strText;
		if (pText && pText->GetType() == HQChart::Complier::Variant::STRING_TYPE)
			strText = WStringToUTF8(pText->GetStringValue());

		jsonWriter.Key("Text");
		jsonWriter.String(strText.c_str());

	jsonWriter.EndObject();
}

bool OutVarToJsonHelper::ToJson(const Execute::ARRAY_OUTVAR_INFO& aryOutVar, const HistoryDataCache* pHistoryData, std::string& strJson, long lOutCount,long lOutStartIndex, long lOutEndIndex, const Execute::ARRAY_ARGUMENT* pAryArgs)
{
	StringBuffer strBuffer;
	Writer<StringBuffer> jsonWriter(strBuffer);
	jsonWriter.StartObject();               // Between StartObject()/EndObject(), 

	int nKCount = pHistoryData->GetKCount();
	int nStart = 0;
	if (lOutStartIndex >= 0 && lOutEndIndex >= 0 && lOutStartIndex<= nKCount && lOutEndIndex<= nKCount)
	{
		nStart = lOutStartIndex;
		nKCount = lOutEndIndex;
	}
	else
	{
		if (lOutCount != -1) nStart = nKCount - lOutCount;
	}
	
	if (nStart < 0) nStart = 0;
	long lType = 0;
	long lPeriod = pHistoryData->GetPeriod();

	jsonWriter.Key("Period");
	jsonWriter.Int(lPeriod);
	jsonWriter.Key("Right");
	jsonWriter.Int(pHistoryData->GetRight());
	jsonWriter.Key("Symbol");
	jsonWriter.String(WStringToUTF8(pHistoryData->GetSymbolString()).c_str());

	if (pAryArgs && !pAryArgs->empty())
	{
		jsonWriter.Key("Args");
		jsonWriter.StartArray();
		for (const auto& item : *pAryArgs)
		{
			jsonWriter.StartObject();
			jsonWriter.Key(WStringToUTF8(item._strName).c_str());
			jsonWriter.Double(item._dValue);
			jsonWriter.EndObject();
		}

		jsonWriter.EndArray();
	}


	jsonWriter.Key("Date");
	jsonWriter.StartArray();
	for (int i = nStart; i < nKCount; ++i)
	{
		auto pKItem = pHistoryData->GetKItem(i);
		jsonWriter.Int(pKItem->_nDate);
	}
	jsonWriter.EndArray();

	if (pHistoryData->IsMinutePeriod() || pHistoryData->IsMinuteChartPeriod())
	{
		jsonWriter.Key("Time");
		jsonWriter.StartArray();
		for (int i = nStart; i < nKCount; ++i)
		{
			auto pKItem = pHistoryData->GetKItem(i);
			jsonWriter.Int(pKItem->_nTime);
		}
		jsonWriter.EndArray();
	}


	jsonWriter.Key("OutVar");
	jsonWriter.StartArray();

	for (const auto& item : aryOutVar)
	{
		jsonWriter.StartObject();

		std::wstring strTitle = item.GetName();
		jsonWriter.Key("Name");
		jsonWriter.String(WStringToUTF8(strTitle).c_str());

		lType = 0;
		const Variant* pVariant = item.GetOutValue();
		if (pVariant)
		{
			switch (pVariant->GetType())
			{
			case HQChart::Complier::Variant::DRAWKLINE_TYPE:
				jsonWriter.Key("DrawType");
				jsonWriter.String("DRAWKLINE");
				DRAWKLINE(pVariant, nStart, nKCount, jsonWriter);
				lType = 1;
				break;
			case HQChart::Complier::Variant::DRAWCOLORKLINE_TYPE:
				jsonWriter.Key("DrawType");
				jsonWriter.String("DRAWCOLORKLINE");
				DRAWCOLORKLINE(pVariant, nStart, nKCount, jsonWriter);
				lType = 1;
				break;
			case HQChart::Complier::Variant::STICKLINE_TYPE:
				jsonWriter.Key("DrawType");
				jsonWriter.String("STICKLINE");
				STICKLINE(pVariant, nStart, nKCount, jsonWriter);
				lType = 1;
				break;
			case HQChart::Complier::Variant::DRAWICON_TYPE:
				jsonWriter.Key("DrawType");
				jsonWriter.String("DRAWICON");
				DRAWICON(pVariant, nStart, nKCount, jsonWriter);
				lType = 1;
				break;
			case HQChart::Complier::Variant::DRAWICON2_TYPE:
				jsonWriter.Key("DrawType");
				jsonWriter.String("ICON");
				DRAWICON(pVariant, nStart, nKCount, jsonWriter);
				lType = 1;
				break;
			case HQChart::Complier::Variant::DRAWTEXT_TYPE:
				jsonWriter.Key("DrawType");
				jsonWriter.String("DRAWTEXT");
				DRAWTEXT(pVariant, nStart, nKCount, jsonWriter);
				lType = 1;
				break;
			case HQChart::Complier::Variant::DRAWNUMBER_TYPE:
				jsonWriter.Key("DrawType");
				jsonWriter.String("DRAWNUMBER");
				DRAWNUMBER(pVariant, nStart, nKCount, jsonWriter);
				lType = 1;
				break;
			case HQChart::Complier::Variant::DRAWBAND_TYPE:
				jsonWriter.Key("DrawType");
				jsonWriter.String("DRAWBAND");
				DRAWBAND(pVariant, nStart, nKCount, jsonWriter);
				lType = 1;
				break;
			case HQChart::Complier::Variant::DRAWTEXT_FIX_TYPE:
				jsonWriter.Key("DrawType");
				jsonWriter.String("DRAWTEXT_FIX");
				DRAWTEXT_FIX(pVariant, nStart, nKCount, jsonWriter);
				lType = 1;
				break;
			case HQChart::Complier::Variant::DRAWNUMBER_FIX_TYPE:
				jsonWriter.Key("DrawType");
				jsonWriter.String("DRAWNUMBER_FIX");
				OutVarToJsonHelper::DRAWNUMBER_FIX(pVariant, nStart, nKCount, jsonWriter);
				lType = 1;
				break;
			case HQChart::Complier::Variant::FILLRGN_TYPE:
				jsonWriter.Key("DrawType");
				jsonWriter.String("FILLRGN");
				OutVarToJsonHelper::FILLRGN(pVariant, nStart, nKCount, jsonWriter);
				lType = 1;
				break;
			case HQChart::Complier::Variant::PARTLINE_TYPE:
				jsonWriter.Key("DrawType");
				jsonWriter.String("PARTLINE");
				OutVarToJsonHelper::PARTLINE(pVariant, nStart, nKCount, jsonWriter);
				lType = 1;
				break;
			case HQChart::Complier::Variant::EXPLAIN_TYPE:
				jsonWriter.Key("DrawType");
				jsonWriter.String("EXPLAIN");
				OutVarToJsonHelper::EXPLAIN(pVariant, nStart, nKCount, jsonWriter);
				lType = 1;
				break;
			default:
				VariantDataToJson(pVariant, nStart, nKCount, jsonWriter);
				lType = 0;
				break;
			}
		}


		jsonWriter.Key("Type");
		jsonWriter.Int(lType);

		const auto& aryAttr = item.GetAttributes();
		if (!aryAttr.empty())
		{
			jsonWriter.Key("Attribute");
			jsonWriter.StartArray();
			std::string strAttribute;
			for (const auto& item : aryAttr)
			{
				strAttribute = WStringToUTF8(item);
				jsonWriter.String(strAttribute.c_str());
			}

			jsonWriter.EndArray();
		}

		const auto& mapAttr = item.GetMapAttributes();
		if (!mapAttr.empty())
		{
			jsonWriter.Key("Attribute2");
			jsonWriter.StartObject();
			for (const auto& item : mapAttr)
			{
				jsonWriter.Key(WStringToUTF8(item.first).c_str());
				jsonWriter.String(WStringToUTF8(item.second).c_str());
			}

			jsonWriter.EndObject();
		}

		jsonWriter.EndObject();
	}

	jsonWriter.EndArray();

	jsonWriter.EndObject();

	strJson = strBuffer.GetString();

	return true;
}

}
}