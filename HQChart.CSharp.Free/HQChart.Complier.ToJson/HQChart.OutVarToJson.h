/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

指标结果转json (c++)
*/

#pragma once

#include "HQChart.KLineData.h"
#include "HQChart.Execute.h"

//json 读取
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "filereadstream.h"
using namespace rapidjson;


namespace HQChart { namespace Complier {

std::string WStringToUTF8(const std::wstring& str);
std::wstring UTF8ToWString(const std::string& str);

class OutVarToJsonHelper
{
public:
	static bool ToJson(const Execute::ARRAY_OUTVAR_INFO& aryOutVar, const HistoryDataCache* pHistoryData, std::string& strJson, long lOutCount, long lOutStartIndex, long lOutEndIndex, const Execute::ARRAY_ARGUMENT* pAryArgs);


	static void ArrayToJson(const std::string& strKey, const Variant* pValue, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter);
	static void VariantDataToJson(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter);

	static void DRAWKLINE(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter);
	static void DRAWCOLORKLINE(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter);
	static void STICKLINE(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter);
	static void DRAWICON(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter);
	static void DRAWTEXT(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter);
	static void DRAWNUMBER(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter);
	static void DRAWBAND(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter);
	static void DRAWTEXT_FIX(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter);
	static void DRAWNUMBER_FIX(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter);
	static void FILLRGN(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter);
	static void PARTLINE(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter);
	static void EXPLAIN(const Variant* pVariant, int nStart, int nKCount, Writer<StringBuffer>& jsonWriter);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
};



}
}
