/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	麦语法执行器 (c++)
*/

#pragma once

#include "HQChart.Complier.h"
#include "HQChart.data.h"
#include <map>
#include <set>

namespace HQChart { namespace Complier {


/////////////////////////////////////////////////////////////////////////
// 指标执行器
//
////////////////////////////////////////////////////////////////////////

class ISendMessage;
class ExecuteExcept : public std::exception
{
public:
	ExecuteExcept();
	ExecuteExcept(const std::wstring& strMessage, const std::wstring& strFunctionInfo, Node* pNode);
	virtual ~ExecuteExcept();

	const std::wstring& GetDescription() const { return m_strDescription; }
	void ToJson(std::wstring& strOut) const;

protected:
	std::wstring m_strDescription;

	//详细信息
	std::wstring m_strMessage;
	std::wstring m_strFunctionInfo;	//函数信息
	int m_nLine = -1;
	int m_nColumn = -1;
};


class OutVarInfo
{
public:
	OutVarInfo();
	OutVarInfo(const std::wstring& strName);
	~OutVarInfo();

	void SetName(const std::wstring& strName);
	void SetOutValue(Variant* pVar);
	void AddAttribute(const std::wstring& strAttribute);
	void AddAttribute(const std::wstring& strKey, const std::wstring& strValue);

	const std::wstring GetName() const { return m_strName; }
	const std::vector<std::wstring>& GetAttributes() const { return m_aryAttributes; }
	const std::map<std::wstring, std::wstring>& GetMapAttributes() const { return m_mapAttributes; }
	Variant* GetOutValue() const { return m_pVar; }

protected:
	std::wstring m_strName;	//变量名
	std::vector<std::wstring> m_aryAttributes;	//属性
	std::map<std::wstring, std::wstring> m_mapAttributes;	//扩展属性
	Variant* m_pVar=NULL;	//变量值
};

//输出变量属性
namespace OutVarAttribute
{
	static const wchar_t NONE_VAR_NAME[] = L"NONE_OUT_NAME";
	static const wchar_t NODRAW[] = L"NODRAW";
}

//中断信息
struct InterruptInfo
{
	bool _bExit = false;	//是否退出
	int _nLine = -1;
	int _nColumn = -1;
};


class IHistoryData;
class Execute
{
public:
	typedef std::pair<std::wstring, Variant*> PARI_VARIANT;
	typedef std::map<std::wstring, Variant*> MAP_VARIANT;
	typedef std::vector<ARGUMENT_ITEM>	ARRAY_ARGUMENT;
	typedef std::set<std::wstring> SET_VARIANT_NAME;
	typedef std::vector<OutVarInfo> ARRAY_OUTVAR_INFO;

	Execute();

	void SetProgram(const Program* program);
	void SetArguments(const ARRAY_ARGUMENT& aryArgs);
	void SetHistoryData(const IHistoryData* pData);
	void SetSendMessageCallback(ISendMessage* pSendMessage);

	void Run();

	bool ToJson(std::wstring& strOut) const;

	const MAP_VARIANT& GetOutVar() const { return m_mapOutVarTable; }	//获取所有输出变量
	const MAP_VARIANT& GetAllVar() const { return m_mapVarTable; }		//获取所有变量
	const ARRAY_OUTVAR_INFO& GetOutVarInfo() const { return m_aryOutVarInfo; }

protected:

	void InitalConstVarTable();	//初始化只读变量
	void InitalVarTable();

	VariantOperator m_VariantOperator;
	const IHistoryData*	m_pHistoryData=NULL;	//行情数据

	Variant* ReadVariable(const std::wstring& strName, Identifier* pNode);
	Variant* ReadMemberVariable(MemberExpression* pNode);
	Variant* ReadSymbolData(const std::wstring& strName, Identifier* pNode);
	Variant* ReadPeriodData(const std::wstring& strName, Identifier* pNode);	//跨周期数据
	Variant* ReadStringData(StringLiteral* pNode);
	Variant* ReadOtherSymbolData(const std::wstring& strParam, Node* pNode);
	Variant* ReadOtherSymbolData(const Variant& varSymbol, const std::wstring& strVarName, Node* pNode);
	Variant* ReadMachineDateTime(const std::wstring& strName);
	Variant* ReadColorData(const std::wstring& strName);	//读取配色

	void VisitNode(Node* pNode);
	
	void VisitAssignmentExpression(AssignmentExpression* pNode);
	void VisitSequenceExpression(SequenceExpression* pNode);
	Variant* VisitBinaryExpression(Node* pNode);
	Variant* VisitCallExpression(CallExpression* pNode);
	Variant* GetNodeValue(Node* pNode);
	void VisitStatement(Statement* pNode);

	Variant* GetDynaInfo(long ID, Node* pNode);
	Variant* GetDynaInfo(long lStartID, const Variant& n, Node* pNode);
	Variant* CallCustomFunction(const CUSTOM_FUNCTION_ITEM& funcItem, const Variant::ARRAY_CALL_ARGUMENT& args, Node* pNode);
	
	void ThrowUnexpectedNode(Node* pNode);
	void ThrowUnexpectedNode(const std::wstring& strMessage, Node* pNode);

	bool ReadAssignmentExpression(AssignmentExpression* pAssignmentExpression, OutVarInfo& varInfo);
	bool ReadIdentifier(Identifier* pIdentifier, OutVarInfo& varInfo);
	bool ReadVarAttribute(Identifier* pIdentifier, OutVarInfo& varInfo);
	bool ReadBinaryExpression(Expression* pExpression, OutVarInfo& varInfo);
	bool ReadNumericLiteral(NumericLiteral* pNumericLiteral, OutVarInfo& varInfo);
	bool ReadCallExpression(CallExpression* pCallExpression, OutVarInfo& varInfo);
	bool ReadCallExpressionExtend(CallExpression* pCallExpression, OutVarInfo& varInfo);
	bool ReadLogicalExpression(LogicalExpression* pLogicalExpression, OutVarInfo& varInfo);
	bool ReadStringLiteral(StringLiteral* pStringLiteral, OutVarInfo& varInfo);
	const std::wstring& GetTempVarName();

	MAP_VARIANT m_mapOutVarTable;	//输出变量
	ARRAY_OUTVAR_INFO m_aryOutVarInfo;
	MAP_VARIANT m_mapVarTable;		//所有的变量
	MAP_VARIANT m_mapConstVarTable;	//只读变量
	SET_VARIANT_NAME m_setConstRealTimeVarTable;	//只读实时变量

	const Program* m_pProgram = NULL;
	ARRAY_ARGUMENT m_aryArguments;	//参数

	InterruptInfo m_Interrupt;	//中断

	ISendMessage* m_pSendMessage = NULL;

	int32_t m_nCounter = 1;
	std::wstring m_strTempVarName;
};

class OutVariantAttribute
{
public:
	OutVariantAttribute();
	~OutVariantAttribute();

	static OutVariantAttribute& GetInstance();

	bool IsAttributeKey(const std::wstring& strValue) const;
	bool AddKey(const std::wstring& strKey);	//添加关键字

	bool IsDrawKey(const std::wstring& strValue) const;

private:
	void Initialize();

	std::set<std::wstring> m_setAttributeKey;		//输出变量属性关键字
	std::set<std::wstring> m_setDrawKey;			//绘图函数名
};

class ISendMessage
{
public:
	friend class Execute;
	virtual long operator()() = 0;

protected:
	Variant::ARRAY_CALL_ARGUMENT m_Argument;	//参数
	Node*	m_pNode = NULL;
};


}
}