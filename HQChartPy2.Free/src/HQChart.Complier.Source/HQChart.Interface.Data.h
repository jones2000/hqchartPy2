/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	对外数据结构 (c++)
*/

#pragma once

namespace HQChart { namespace Complier {

#pragma pack(push,1)

struct HISTORY_ITEM
{
	int _nDate = 0;		//日期  yyyymmdd
	int _nTime = 0;		//时间  hhmmss
	double _dYClose = 0;	//前收盘价	分时-均价
	double _dOpen = 0;		//开
	double _dHigh = 0;		//高
	double _dLow = 0;		//低
	double _dClose = 0;		//收
	double _dVol = 0;		//成交量
	double _dAmount = 0;	//成交金额
	int	_nAdvance = 0;		//周期上涨家数. 分笔 B/S
	int _nDecline = 0;		//周期下跌家数.

	//期货
	double _dPosition = 0;		//持仓
	double _dSettle = 0;		//结算价

	char	_exData[32] = { 0 };	//预留数据
};

//K线信息
struct KDATA_INFO
{
	long _lPeriod;	//周期
	long _lRight;	//复权

	//起始日期
	long _lStartDate;
	long _lStartTime;

	//结束日期
	long _lEndDate;
	long _lEndTime;

	long _lCount;	//K线个数
};

struct SNAPSHOT_ITEM
{
	wchar_t _szSymbol[32];
	int _nDate;
	int _nTime;

	double _dYClose;	//前收盘价
	double _dOpen;		//开
	double _dHigh;		//高
	double _dLow;		//低
	double _dClose;		//收
	double _dVol;		//成交量
	double _dAmount;	//成交金额

	int	_nAdvance;		//周期上涨家数. 分笔 B/S
	int _nDecline;		//周期下跌家数.

	//五档买卖
	double _dSellPrice[10];
	double _dSellVol[10];
	double _dBuyPrice[10];
	double _dBuyVol[10];
};

struct MINUTE_KLINE_ITEM
{
	int _nDate;
	int _nTime;

	double _dYClose;	//前收盘价
	double _dOpen;		//开
	double _dHigh;		//高
	double _dLow;		//低
	double _dClose;		//收
	double _dVol;		//成交量
	double _dAmount;	//成交金额
};

//当天分钟数据
struct SNAPSHOT_MINUTE_ITEM
{
	wchar_t _szSymbol[32];
	double _dYClose;
	int _nDate;
	int _nCount;

	HISTORY_ITEM _Minute[250];
};

#pragma pack(pop)


}
}