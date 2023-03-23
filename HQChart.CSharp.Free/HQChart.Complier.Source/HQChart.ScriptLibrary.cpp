/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	自定义脚本函数库 (c++)
*/


#include "HQChart.data.h"



namespace HQChart { namespace Complier {


ScriptLibrary::ScriptLibrary()
{
	Load();
}

ScriptLibrary::~ScriptLibrary()
{

}


ScriptLibrary& ScriptLibrary::GetInstance()
{
	static ScriptLibrary instance;
	return instance;
}

const ScriptIndex* ScriptLibrary::Get(const std::wstring& strName) const
{
	auto find = m_mapFunction.find(strName);
	if (find == m_mapFunction.end()) return NULL;

	return &(find->second);
}

void ScriptLibrary::Add(std::wstring& strName, std::wstring& strCode, std::wstring& strDescription)
{
	m_mapFunction[strName] = ScriptIndex(strName, strCode, strDescription);
}

void ScriptLibrary::Add(std::wstring& strName, std::wstring& strCode, const ARRAY_ARGUMENT& argArgs, std::wstring& strDescription)
{
	m_mapFunction[strName] = ScriptIndex(strName, strCode, argArgs, strDescription);
}

bool ScriptLibrary::Load()
{
	MACD();
	KDJ();
	MA();
	WR();
	BOLL();
	BBI();
	DKX();
	MIKE();

	return true;
}

void ScriptLibrary::MACD()
{
	std::wstring strName = L"MACD";

	std::wstring strCode = 
L"DIF:EMA(CLOSE, SHORT) - EMA(CLOSE, LONG);\n\
DEA:EMA(DIF, MID); \n\
MACD:(DIF - DEA) * 2, COLORSTICK;";

	ScriptIndex::ARRAY_ARGUMENT aryArgument = 
	{ 
		ARGUMENT_ITEM(L"SHORT", 12), 
		ARGUMENT_ITEM(L"LONG", 26), 
		ARGUMENT_ITEM(L"MID",9) 
	};

	m_mapFunction[strName] = ScriptIndex(strName, strCode, aryArgument, L"MACD");
}

void ScriptLibrary::KDJ()
{
	std::wstring strName = L"KDJ";

	std::wstring strCode = 
L"RSV:=(CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100;\n\
K:SMA(RSV, M1, 1);\n\
D:SMA(K, M2, 1);\n\
J:3 * K - 2 * D;";

	ScriptIndex::ARRAY_ARGUMENT aryArgument = 
	{ 
		ARGUMENT_ITEM(L"N", 9), 
		ARGUMENT_ITEM(L"M1", 3), 
		ARGUMENT_ITEM(L"M2",3) 
	};

	m_mapFunction[strName] = ScriptIndex(strName, strCode, aryArgument, L"KDJ");
}


void ScriptLibrary::MA()
{
	std::wstring strName = L"MA";

	std::wstring strCode = 
L"MA1:MA(CLOSE,M1);\n\
MA2:MA(CLOSE, M2);\n\
MA3:MA(CLOSE, M3);\n\
MA4:MA(CLOSE, M4);\n\
MA5:MA(CLOSE, M5);\n\
MA6:MA(CLOSE, M6);\n\
MA7:MA(CLOSE, M7);\n\
MA8:MA(CLOSE, M8); ";

	ScriptIndex::ARRAY_ARGUMENT aryArgument = 
	{ 
		ARGUMENT_ITEM(L"M1", 5), 
		ARGUMENT_ITEM(L"M2",10), 
		ARGUMENT_ITEM(L"M3",20), 
		ARGUMENT_ITEM(L"M4",60),
		ARGUMENT_ITEM(L"M5",0),
		ARGUMENT_ITEM(L"M6",0),
		ARGUMENT_ITEM(L"M7",0),
		ARGUMENT_ITEM(L"M8",0),
	};

	m_mapFunction[strName] = ScriptIndex(strName, strCode, aryArgument, L"均线");
}

void ScriptLibrary::WR()
{
	std::wstring strName = L"WR";

	std::wstring strCode =
L"WR1:100*(HHV(HIGH,N)-CLOSE)/(HHV(HIGH,N)-LLV(LOW,N));\n\
WR2:100 * (HHV(HIGH, N1) - CLOSE) / (HHV(HIGH, N1) - LLV(LOW, N1)); ";

	ScriptIndex::ARRAY_ARGUMENT aryArgument =
	{
		ARGUMENT_ITEM(L"N", 10),
		ARGUMENT_ITEM(L"N1",6),
	};

	m_mapFunction[strName] = ScriptIndex(strName, strCode, aryArgument, L"威廉指标");
}

void ScriptLibrary::BOLL()
{
	std::wstring strName = L"BOLL";

	std::wstring strCode =
L"BOLL:MA(CLOSE,M);\n\
UB:BOLL+2*STD(CLOSE,M);\n\
LB:BOLL-2*STD(CLOSE,M);";

	ScriptIndex::ARRAY_ARGUMENT aryArgument =
	{
		ARGUMENT_ITEM(L"M", 20),
	};

	m_mapFunction[strName] = ScriptIndex(strName, strCode, aryArgument, L"布林线");
}

void ScriptLibrary::BBI()
{
	std::wstring strName = L"BBI";

	std::wstring strCode =
		L"BBI:(MA(CLOSE,M1)+MA(CLOSE,M2)+MA(CLOSE,M3)+MA(CLOSE,M4))/4;";

	ScriptIndex::ARRAY_ARGUMENT aryArgument =
	{
		ARGUMENT_ITEM(L"M1", 3),
		ARGUMENT_ITEM(L"M2", 6),
		ARGUMENT_ITEM(L"M3", 12),
		ARGUMENT_ITEM(L"M4", 24),
	};

	m_mapFunction[strName] = ScriptIndex(strName, strCode, aryArgument, L"多空均线");
}

void ScriptLibrary::DKX()
{
	std::wstring strName = L"DKX";

	std::wstring strCode =
L"MID:=(3*CLOSE+LOW+OPEN+HIGH)/6;\n\
DKX:(20*MID+19*REF(MID,1)+18*REF(MID,2)+17*REF(MID,3)+\n\
16*REF(MID,4)+15*REF(MID,5)+14*REF(MID,6)+\n\
13*REF(MID,7)+12*REF(MID,8)+11*REF(MID,9)+\n\
10*REF(MID,10)+9*REF(MID,11)+8*REF(MID,12)+\n\
7*REF(MID,13)+6*REF(MID,14)+5*REF(MID,15)+\n\
4*REF(MID,16)+3*REF(MID,17)+2*REF(MID,18)+REF(MID,20))/210;\n\
MADKX:MA(DKX,M);";

	ScriptIndex::ARRAY_ARGUMENT aryArgument =
	{
		ARGUMENT_ITEM(L"M", 10)
	};

	m_mapFunction[strName] = ScriptIndex(strName, strCode, aryArgument, L"多空线");
}

void ScriptLibrary::MIKE()
{
	std::wstring strName = L"MIKE";

	std::wstring strCode =
		L"HLC:=REF(MA((HIGH+LOW+CLOSE)/3,N),1);\n\
HV:=EMA(HHV(HIGH,N),3);\n\
LV:=EMA(LLV(LOW,N),3);\n\
STOR:EMA(2*HV-LV,3);\n\
MIDR:EMA(HLC+HV-LV,3);\n\
WEKR:EMA(HLC*2-LV,3);\n\
WEKS:EMA(HLC*2-HV,3);\n\
MIDS:EMA(HLC-HV+LV,3);\n\
STOS:EMA(2*LV-HV,3);";

	ScriptIndex::ARRAY_ARGUMENT aryArgument =
	{
		ARGUMENT_ITEM(L"N", 10)
	};

	m_mapFunction[strName] = ScriptIndex(strName, strCode, aryArgument, L"麦克支撑压力");
}

}}