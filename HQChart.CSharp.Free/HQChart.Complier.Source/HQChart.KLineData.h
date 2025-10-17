/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	��Դ��Ŀ https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	��Ʊ�����༰����ӿ��� (c++)
*/

#pragma once
#include "HQChart.Define.h"
#include <vector>
#include <list>
#include <map>
#include <string>
#include <set>
#include "HQChart.Interface.Data.h"

#ifdef _WIN32
#define REGISTER_HISTORYDATA(__class__)\
	HQChart::Complier::IHistoryData* Create##__class__(const std::wstring& strSymbol, long lPeriod, long lRight)\
	{\
		return new __class__(strSymbol, lPeriod, lRight);\
	}\
	void Delete##__class__(HQChart::Complier::IHistoryData*& pHistoryData)\
	{\
		if (pHistoryData) { delete pHistoryData; pHistoryData = NULL; }\
	}\
	const bool bRegisteredCreate##__class__ = HQChart::Complier::DataCreateFactory::GetInstance().RegisterHistoryData(L#__class__, Create##__class__, Delete##__class__);\


#else
#define REGISTER_HISTORYDATA(__class__)\
	HQChart::Complier::IHistoryData* Create__class__(const std::wstring& strSymbol, long lPeriod, long lRight)\
	{\
		return new __class__(strSymbol, lPeriod, lRight);\
	}\
	void Delete__class__(HQChart::Complier::IHistoryData*& pHistoryData)\
	{\
		if (pHistoryData) { delete pHistoryData; pHistoryData = NULL; }\
	}\
	const bool bRegisteredCreate__class__ = HQChart::Complier::DataCreateFactory::GetInstance().RegisterHistoryData(L"_class__", Create__class__, Delete__class__);\
	
#endif

namespace HQChart { namespace Complier {

//����
enum PERIOD_TYPE_ID
{
	PERIOD_DAY_ID = 0,
	PERIOD_WEEK_ID = 1,
	PERIOD_MONTH_ID = 2,
	PERIOD_YEAR_ID = 3,
	PERIOD_QUARTER_ID = 9,		//��
	PERIOD_TWO_WEEK_ID = 21,	//˫��
	PERIOD_HALF_YEAR_ID=22,		//����

	PERIOD_MIN1_ID = 4,
	PERIOD_MIN5_ID = 5,
	PERIOD_MIN15_ID = 6,
	PERIOD_MIN30_ID = 7,
	PERIOD_MIN60_ID = 8,

	PERIOD_TICK_ID = 10,	//�ֱ�

	PERIOD_MIN120_ID = 11,
	PERIOD_MIN240_ID = 12,

	//�Զ�����������
	PERIOD_DAY2_ID = 40002,
	PERIOD_DAY3_ID = 40003,
	PERIOD_DAY4_ID = 40004,
	PERIOD_DAY5_ID = 40005,
	PERIOD_DAY6_ID = 40006,
	PERIOD_DAY7_ID = 40007,
	PERIOD_DAY8_ID = 40008,
	PERIOD_DAY9_ID = 40009,
	PERIOD_DAY10_ID = 40010,
	PERIOD_DAY11_ID = 40011,
	PERIOD_DAY12_ID = 40012,
	PERIOD_DAY13_ID = 40013,
	PERIOD_DAY14_ID = 40014,
	PERIOD_DAY15_ID = 40015,

	//����ͼ
	DAY_MINUTE_ID= 128787,		//����
	MULTIDAY_MINUTE_ID= 128788,	//����
};

//��Ȩ
enum RIGHT_TYPE_ID
{
	RIGHT_NONE_ID = 0, //����Ȩ
	RIGHT_BEFORE_ID = 1,//ǰ��Ȩ
	RIGHT_AFTER_ID= 2,  //��Ȩ
};

class Variant;
class Node;
struct FIT_DATETIME_ITEM;

class VariantCacheManage;
class ScriptIndex;
class IHistoryData
{
public:
	IHistoryData();
	virtual ~IHistoryData();

	typedef std::vector<Variant*> ARRAY_VARIANT;
	typedef std::list<Variant*> LIST_VARIANT;
	typedef std::vector<Variant* > ARRAY_CALL_ARGUMENT;
	typedef std::vector<HISTORY_ITEM> ARRAY_KDATA;
	typedef std::vector<FIT_DATETIME_ITEM> ARRAY_FIT_DATETIME_DATA;

	virtual void Initialization(void* pData) = 0;	//��ʼ��
	virtual bool LoadKData() = 0;	//����K��
	virtual bool LoadKData(const std::wstring& strSymbol, long lPeriod, long lRight, Node* pNode, const std::wstring& strCallInfo) =0;	//K������

	virtual Variant* GetClose() const =0;	//���̼�
	virtual Variant* GetOpen() const =0;	//����
	virtual Variant* GetHigh() const =0;	//���
	virtual Variant* GetLow() const =0;		//���
	virtual Variant* GetVol() const =0;		//��
	virtual Variant* GetAmount() const =0;	//���
	virtual Variant* GetAvPrice() const=0;	//����
	virtual Variant* GetSettle() const=0;	//�����
	virtual Variant* GetPosition() const=0;	//�ֲ�
	virtual Variant* GetDate() const = 0;	//����
	virtual Variant* GetTime() const =0;	//ʱ��
	virtual Variant* GetMonth() const=0;	//��
	virtual Variant* GetDay() const = 0;	//��
	virtual Variant* GetYear() const=0;		//��
	virtual Variant* GetWeekDay() const=0;	//���ڼ�
	virtual Variant* GetHour() const=0;		//Сʱ
	virtual Variant* GetMinute() const=0;	//����
	virtual Variant* GetCurrBarsCount() const = 0;	//��������յ�������
	virtual Variant* GetTotalBarsCount() const = 0;
	virtual Variant* GetBarPos() const = 0;
	virtual Variant* GetFromOpen(Node* pNode) const = 0;
	virtual Variant* GetAdvance() const = 0;
	virtual Variant* GetDecline() const = 0;
	virtual const ARRAY_KDATA* GetKData(const std::wstring& strSymbol, long lPeriod, long lRight) const = 0;
	virtual Variant* GetKDataItem(const ARRAY_KDATA& aryData, const std::wstring& strVarName) const = 0;

	virtual Variant* GetCustomValue(const std::wstring& strName, Node* pNode) const = 0;
	virtual Variant* CallCustomFunction(const std::wstring& strName, const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode) const = 0;

	virtual const HISTORY_ITEM* GetKItem(int nIndex) const = 0;	//��ȡ������Ӧ��K������
	virtual long GetKCount() const = 0;
	virtual const bool GetKMaxMin(double& dMax, double& dMin) const = 0;		// ��ȡK�������Сֵ
	virtual const bool GetKMaxMin(long lStart, long lEnd, double& dMax, double& dMin) const = 0;

	virtual long FindByDate(long lDate) const { return -1; }	//����ĳһ��K�����ڵ�����

	virtual Variant* GetBlockMemberCount(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const = 0;	//����Ʊ����
	virtual Variant* GetBlockCalculate(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const =0 ;		//���ͳ��
	virtual bool GetBlockMember(const std::wstring& strBlockID, long lDate, std::vector<std::wstring>& arySymbol, Node* pNode) const = 0;	//��ȡ����Ա
	

	//������������
	virtual Variant* GetFinance(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const = 0;	//��������
	virtual Variant* GetCapital(Node* pNode) const = 0;			//CAPITAL ��ǰ��ͨ�ɱ� ��
	virtual Variant* GetTotalCaptialAll(Node* pNode) const = 0;	//TOTALCAPITAL  ��ǰ�ܹɱ� ��
	virtual Variant* GetHisCapital(Node* pNode) const = 0;	//��ʷ��ͨ�ɱ� ��
	virtual Variant* GetExchange(Node* pNode) const = 0;	//������
	virtual Variant* GetDynaInfo(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const = 0;	//������������
	virtual Variant* GetIndexData(const std::wstring& strName, Node* pNode) const = 0;	//��ȡ��������

	virtual Variant* GetName() const = 0;		//��Ʊ����
	virtual Variant* GetSymbol(long lType) const = 0;		//��Ʊ����
	virtual const std::wstring& GetSymbolString() const = 0;//��Ʊ����
	virtual Variant* GetCategroyName(long lType) const = 0;	//�������
	virtual Variant* IsInCategroy(long lType, const std::wstring& strName) const = 0; //�Ƿ����Ա
	virtual Variant* GetMarketName() const = 0;	//�г�����
	virtual Variant* GetMarketCNName() const = 0;	//�г�����
	virtual Variant* GetPeriodID() const = 0; //��ʱ��=1,�ֱʳɽ�=2,1������=3,5������=4,15������=5,30������=6,60������=7,����=8,����=9,����=10,������=11,����=12,����=13,������=14��
	virtual Variant* GetSymbolTypeID() const = 0;
	virtual Variant* GetDrawNULL() const = 0;

	virtual bool GetIndexScript(const std::wstring& strName, const std::wstring& strCallInfo, ScriptIndex& script, Node* pNode) const = 0;	//��ȡϵͳָ��ű�
	
	virtual Variant* RPCCall(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const = 0;

	virtual long GetPeriod() const = 0;
	virtual long GetRight() const = 0;

	virtual ARRAY_KDATA& GetData() = 0;
	virtual const ARRAY_KDATA& GetData() const = 0;

	virtual bool IsMinutePeriod() const=0;
	virtual bool IsDayPeriod() const=0;
	virtual bool IsTickPeriod() const = 0;
	virtual bool IsMinuteChartPeriod() const = 0;

	virtual bool ConvertToDayPeriod(const ARRAY_KDATA& aryDay, ARRAY_KDATA& dest, long lPeriod) const;
	virtual bool ConvertToMinutePeriod(const ARRAY_KDATA& aryOneMinute, ARRAY_KDATA& dest, long lPeriod) const;
	virtual bool ConvertToRight(ARRAY_KDATA& aryData, long lRight) const;	//��Ȩ����
	virtual void Reset();	//�����������

	inline const std::wstring& GetClassName() const { return m_strClassName; }

	void ClearCache();
	
	static void FitDateTime(const IHistoryData* pDestHistoryData, const IHistoryData* pHistoryData, ARRAY_FIT_DATETIME_DATA& aryDateTime);		//������� (pDestHistoryData ��Ҫת����K
	static bool IsMinutePeriod(long lPeriod);
	static bool IsDayPeriod(long lPeriod);
	static bool IsTickPeriod(long lPeriod);
	static bool IsMinuteChartPeriod(long lPeriod);	//����ͼ
	static bool IsSZSHStock(const std::wstring& strSymbol);	//�Ƿ�Ϊ�Ϻ�/��֤��Ʊ
	static long GetFriday(long lDate);
	static long TransDayToSeaon(long lMonth);

protected:
	Variant* Create() const;
	
	mutable ARRAY_VARIANT m_VariantUsedCache;		//�������д����ı�������
	mutable LIST_VARIANT m_VariantFreeCache;		//�ͷ��Ժ���뻺��.
	mutable ARRAY_VARIANT m_VariantCache;			//���еĻ���
	std::wstring m_strClassName = L"IHistoryData";
};

class VariantCacheManage
{
public:
	typedef std::vector<Variant*> ARRAY_VARIANT;

	VariantCacheManage();
	~VariantCacheManage();

	Variant* Create();

	void FreeVariant();
	void Clear();

private:
	ARRAY_VARIANT m_aryUsedVariant;
	ARRAY_VARIANT m_aryFreeVariant;
};


#pragma pack(push,1)

struct FINANCE_ITEM
{
	int _nDate;			//����
	double _dValue;		//ĳһ������ָ��
};

enum FINANCE_ID		//��������ID
{
	TOTAL_EQUITY_ID = 0, //�ܹɱ�
	FLOW_EQUITY_ID=7,	 //��ͨ�ɱ�
	SHAREHOLDER_ID=8,	 //�ɶ�����
	AL_RATIO=9,			 //�ʲ���ծ��(asset-liability ratio)
};

struct HISTORY_EXTEND_ITEM
{
	int _nDate;		//K������
	int _nTime = 0;	//K��ʱ��
	double _dValue;	//����

	int _nExDate;	//��Ӧ��չ���ݵ�����ʱ�� ��������)
	int _nExTime;
};

struct OVERLAY_HISTORY_ITEM
{
	int _nDate;
	int _nTime = 0;
	double _dYClose=0;	//Ŀǰû�õ�
	double _dOpen;
	double _dHigh;
	double _dLow;
	double _dClose;
	double _dVol;
	double _dAmount;

	int	_nAdvance = 0;	//�������Ǽ���.
	int _nDecline = 0;	//�����µ�����.

	int _nExDate;	//��Ӧ��չ���ݵ�����ʱ�� ��������)
	int _nExTime;
};

enum DYNAINFO_ID
{
	DYNAINFO_YCLOSE_ID=3,	//ǰ���̼�
	DYNAINFO_OPEN_ID=4,		//���̼�
	DYNAINFO_HIGH_ID=5,		//��߼�
	DYNAINFO_LOW_ID=6,		//��ͼ�
	DYNAINFO_PRICE_ID=7,	//�ּ�
	DYNAINFO_VOL_ID=8,		//����
	DYNAINFO_CVOL_ID=9,		//����
	DYNAINFO_AMOUNT_ID=10,	//���
	DYNAINFO_APRICE_ID=11,  //����
};


#pragma pack(pop)

//��ҵ����
struct CATEGROY_ITEM
{
	long _lType;			//��ҵ����
	std::wstring _strName;	//��ҵ����
	std::wstring _strSymbol;//��ҵ����
};

struct FIT_DATETIME_ITEM
{
	long _lDate[2];
	long _lTime[2];
	long _lIndex = -1;

	FIT_DATETIME_ITEM()
		:_lIndex(-1)
	{
		_lDate[0] = _lDate[1] = 0;
		_lTime[0] = _lTime[1] = 0;
	}
};


class HistoryDataCache : public IHistoryData
{
public:
	typedef std::vector<HISTORY_ITEM> ARRAY_KDATA;
	typedef std::vector<FINANCE_ITEM> ARRAY_FINANCE_DATA;
	typedef std::vector<HISTORY_EXTEND_ITEM> ARRAY_EXTEND_DATA;
	typedef std::map<long, double> MAP_DYNAINFO_DATA;
	typedef std::vector<OVERLAY_HISTORY_ITEM> ARRAY_OVERLAY_DATA;	//��������
	typedef std::vector<CATEGROY_ITEM> ARRAY_CATEGORY_DATA;			//��ҵ����

	HistoryDataCache(const std::wstring& strSymbol, long lPeriod, long lRight);
	virtual ~HistoryDataCache();

	virtual void Initialization(void* pData);
	virtual bool LoadKData();							//����K��
	virtual bool LoadKData(const std::wstring& strSymbol, long lPeriod, long lRight, Node* pNode, const std::wstring& strCallInfo);

	ARRAY_KDATA& GetData() { return m_aryData; };
	const ARRAY_KDATA& GetData() const { return m_aryData; }

	void SetName(const std::wstring& strName) { m_strName = strName; }	//���ù�Ʊ����

	virtual const HISTORY_ITEM* GetKItem(int nIndex) const;
	virtual long GetKCount() const;
	virtual const bool GetKMaxMin(double& dMax, double& dMin) const;
	virtual const bool GetKMaxMin(long lStart, long lEnd, double& dMax, double& dMin) const;
	virtual long FindByDate(long lDate) const;

	virtual Variant* GetClose() const;
	virtual Variant* GetOpen() const;
	virtual Variant* GetHigh() const;
	virtual Variant* GetLow() const;
	virtual Variant* GetVol() const;
	virtual Variant* GetAmount() const;
	virtual Variant* GetAvPrice() const;	//����
	virtual Variant* GetSettle() const;		//�����
	virtual Variant* GetPosition() const;	//�ֲ�
	virtual Variant* GetDate() const;
	virtual Variant* GetTime() const;
	virtual Variant* GetMonth() const;
	virtual Variant* GetYear() const;
	virtual Variant* GetDay() const;
	virtual Variant* GetWeekDay() const;
	virtual Variant* GetHour() const;
	virtual Variant* GetMinute() const;
	virtual Variant* GetCurrBarsCount() const;
	virtual Variant* GetTotalBarsCount() const;
	virtual Variant* GetBarPos() const;
	virtual Variant* GetFromOpen(Node* pNode) const;
	virtual Variant* GetAdvance() const;
	virtual Variant* GetDecline() const;
	virtual Variant* GetCustomValue(const std::wstring& strName, Node* pNode) const;
	virtual Variant* CallCustomFunction(const std::wstring& strName, const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode) const;
	virtual const ARRAY_KDATA* GetKData(const std::wstring& strSymbol, long lPeriod, long lRight) const;
	virtual Variant* GetKDataItem(const ARRAY_KDATA& aryData, const std::wstring& strVarName) const;

	virtual Variant* GetFinance(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;
	virtual Variant* GetExchange(Node* pNode) const;	//������
	virtual Variant* GetCapital(Node* pNode) const;		//��ȡ��ǰ����ͨ�ɱ� ��λ��
	virtual Variant* GetTotalCaptialAll(Node* pNode) const;	//TOTALCAPITAL  ��ǰ�ܹɱ� ��
	virtual Variant* GetHisCapital(Node* pNode) const;	//��ȡ��ʷ���е���ͨ�ɱ� ��λ��
	virtual Variant* GetDynaInfo(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;
	virtual Variant* GetIndexData(const std::wstring& strName, Node* pNode) const;

	virtual Variant* GetBlockMemberCount(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;	//����Ʊ����
	virtual Variant* GetBlockCalculate(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;		//���ͳ��

	virtual Variant* GetName() const;		//��Ʊ����
	virtual Variant* GetSymbol(long lType) const;		//��Ʊ����
	virtual Variant* GetCategroyName(long lType) const;	//������� lType=-1 ȡ������ҵ
	virtual Variant* IsInCategroy(long lType, const std::wstring& strName) const;
	virtual Variant* GetMarketName() const;
	virtual Variant* GetMarketCNName() const;
	virtual Variant* GetPeriodID() const; //��ʱ��=1,�ֱʳɽ�=2,1������=3,5������=4,15������=5,30������=6,60������=7,����=8,����=9,����=10,������=11,����=12,����=13,������=14��
	virtual Variant* GetSymbolTypeID() const;
	virtual Variant* GetDrawNULL() const;
	

	virtual bool GetIndexScript(const std::wstring& strName, const std::wstring& strCallInfo, ScriptIndex& script, Node* pNode) const;	//��ȡϵͳָ��ű�

	virtual Variant* RPCCall(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;

	virtual long GetPeriod() const { return m_lPeriod; }
	virtual long GetRight() const { return m_lRight; }
	virtual bool IsMinutePeriod() const;
	virtual bool IsDayPeriod() const;
	virtual bool IsTickPeriod() const;
	virtual bool IsMinuteChartPeriod() const;

	bool IsSH() const;		//�Ϻ�������Ʒ��
	bool IsSZ() const;		//���ڽ�����Ʒ��
	bool IsSHSZA() const;		//�Ϻ�,����A��
	bool IsSHSZIndex() const; //�Ϻ�,����ָ��

	virtual const std::wstring& GetSymbolString() const { return m_strSymbol; }

	

protected:
	virtual bool LoadFinance(long lType) const;			//���ز������� (�õ��ż��أ�
	virtual bool LoadDynainfo(long lType) const;		//���ؼ�ʱ�������� (�õ��ż��أ�
	virtual bool LoadIndexData(const std::wstring& strName, Node* pNode) const;		//���ش������� (�õ��ż���)
	virtual bool LoadCategoryData() const;				//������ҵ��������(�õ��ż���)

	Variant* GetIsPriceDown() const;
	Variant* GetIsPriceUp() const;
	Variant* GetIsPriceEqual() const;
	Variant* GetIsLastBar() const;
	Variant* GetBarStatus() const;

	static void FitExtendData(ARRAY_EXTEND_DATA& dest, const ARRAY_KDATA& kData, const ARRAY_FINANCE_DATA& fData);			//��������K���������
	static void FitExtendData(ARRAY_EXTEND_DATA& dest, const ARRAY_KDATA& kData, long lPeriod, const ARRAY_FINANCE_DATA& fData, double dNUll, bool bExactMatch=true);	///���ݺ�K���������,����ƽ������ bExactMatch ��ȷƥ��

	static void FitOveralyDayData(ARRAY_OVERLAY_DATA& dest, const ARRAY_KDATA& kData, const ARRAY_KDATA& overalyData);		//K�ߵ������
	static void FitOveralyMinuteData(ARRAY_OVERLAY_DATA& dest, const ARRAY_KDATA& kData, const ARRAY_KDATA& overalyData);	//K�ߵ������
	static int GetWeek(int nDate);

	mutable ARRAY_FINANCE_DATA m_aryFinance;	//�������ݶ�ȡ����
	mutable MAP_DYNAINFO_DATA m_mapDynainfo;	//��ʱ��������
	mutable ARRAY_OVERLAY_DATA m_aryIndex;		//��������
	mutable ARRAY_CATEGORY_DATA m_aryCategory;	//��ҵ��������

	std::wstring m_strSymbol;					//��Ʊ����
	std::wstring m_strName;						//��Ʊ����

	long m_lPeriod = 0;							//���� 0=���� 1=���� 2=���� 3=���� 4=1���� 5=5���� 6=15���� 7=30���� 8=60���� 9=���� 10=�ֱ�
	long m_lRight = 0;							//��Ȩ 0=����Ȩ 1=ǰ��Ȩ 2=��Ȩ
	ARRAY_KDATA m_aryData;						//K������
	KDATA_INFO m_KDataInfo;						//K����Ϣ
	std::map<std::wstring, ARRAY_KDATA>	m_mapKData;	//������K������
	XINT64 m_lUpdateTime=0;						//������ʱ��
};



typedef IHistoryData* (*pCreateHistoryData)(const std::wstring& strSymbol, long lPeriod, long lRight);	//lPeriod=���� lRight=��Ȩ
typedef void(*pDeleteHistoryData)(IHistoryData*& pHistoryData);

struct CREATE_ITEM
{
	std::wstring _strName;
	pCreateHistoryData _pProcCreate = NULL;
	pDeleteHistoryData _pProcDelete = NULL;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
// �����๤��
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

class DataCreateFactory
{
public:
	static DataCreateFactory& GetInstance();

	bool RegisterHistoryData(const std::wstring& strClassName, pCreateHistoryData pProcCreate, pDeleteHistoryData pProcDelete);

	IHistoryData* CreateHistoryData(const std::wstring& strClassName, const std::wstring& strSymbol, long lPeriod, long lRight);
	bool DeleteHistoryData(const std::wstring& strClassName, IHistoryData*& pHistoryData);

private:
	DataCreateFactory();
	~DataCreateFactory();

	typedef std::map<std::wstring, CREATE_ITEM> MAP_CREATE_DATA;

	MAP_CREATE_DATA m_mapCreateData;

};

class AutoPtrHistoryData
{
public:
	AutoPtrHistoryData(const std::wstring& strClassName);
	~AutoPtrHistoryData();

	IHistoryData* Create(const std::wstring& strSymbol, long lPeriod, long lRight);
	void Release();

private:
	std::wstring m_strClassName;
	IHistoryData* m_pHistoryData=NULL;
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  �Զ������,�����ӿ�
//
//
/////////////////////////////////////////////////////////////////////////////////////////////


struct CUSTOM_FUNCTION_ITEM
{
	std::wstring _strName;		//������
	int			_nArgCount;	//��������
};

class CustomFunction
{
public:
	static CustomFunction& GetInstance();

	bool Get(const std::wstring& strName, CUSTOM_FUNCTION_ITEM& item);
	void Add(const std::wstring& strName, long lArgCount);

private:
	CustomFunction();
	~CustomFunction();

	std::map<std::wstring, CUSTOM_FUNCTION_ITEM> m_aryCustomFunc;	//�Զ��庯��


};

class CustomVariant
{
public:
	static CustomVariant& GetInstance();

	void Add(const std::wstring& strName);
	bool IsExist(const std::wstring& strName) const;

private:
	CustomVariant();
	~CustomVariant();

	std::set<std::wstring> m_setVariant;
};


}
}