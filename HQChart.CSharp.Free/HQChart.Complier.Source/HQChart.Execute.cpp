/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	��Դ��Ŀ https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	���﷨ִ���� (c++)
*/


#include "HQChart.Execute.h"
#include "HQChart.KLineData.h"
#include "HQChart.Log.h"
#include <list>
#include <sstream>
#include <ctime>
#include "HQChart.BlockScriptResultCache.h"



namespace HQChart { namespace Complier {

const std::set<std::wstring> SET_HQ_VAR_NAME =
{
	L"CLOSE",L"C" ,
	L"OPEN",L"O",
	L"HIGH",L"H",
	L"LOW",L"L",
	L"VOL",L"V",
	L"AMOUNT",L"AMO",
};


Execute::Execute()
{
	
}

void Execute::SetProgram(const Program* program)
{
	BlockScriptResultCache::GetInstance();

	m_pProgram = program;
}

void Execute::SetArguments(const ARRAY_ARGUMENT& aryArgs)
{
	m_aryArguments = aryArgs;
}

void Execute::SetHistoryData(const IHistoryData* pData)
{
	m_pHistoryData = pData;
}

void Execute::SetSendMessageCallback(ISendMessage* pSendMessage)
{
	m_pSendMessage = pSendMessage;
}

#if (defined WIN32) || (defined WIN64)
	void write_log(const char* szStr) 
	{ 

	}
#else
	void write_log(const char* szStr) 
	{
		FILE* fp;
		fp = fopen("run.log", "at+");
		if (fp != NULL) {
			fwrite(szStr, strlen(szStr), 1, fp); //
			fwrite("\r\n", 2, 1, fp); //
			fclose(fp);
			fp = NULL;
		}
	}
#endif

void Execute::Run()
{
	//TRACE_DEBUG(L"[Execute::Run] start run ......");
	//Log::WriteLogFile("[Execute::Run] start run ......");
	m_Interrupt._bExit = false;
	m_nCounter = 1;
	m_VariantOperator.ClearCache();
	InitalVarTable();
	InitalConstVarTable();
	
	//write_log("[Execute::Run] GetBody ......");
	auto body = m_pProgram->GetBody();
	for (auto iter : body)
	{
		//write_log("[Execute::Run] VisitNode ......");
		VisitNode(iter);
		VisitStatement(iter);

		if (m_Interrupt._bExit)
		{
			TRACE_DEBUG(L"[JSExecute::Run] Exit. line=%d, column=%d", m_Interrupt._nLine, m_Interrupt._nColumn);
			break;
		}

		//write_log("[Execute::Run] end VisitNode ......");
	}
	//write_log("[Execute::Run] finsh ......");
	//TRACE_DEBUG(L"[Execute::Run] finish.");
}

void Execute::VisitStatement(Statement* pNode)
{
	long lType = pNode->GetType();
	if (lType != Syntax::EXPRESSIONSTATEMENT_ID) return;

	ExpressionStatement* p = pNode->As<ExpressionStatement>();
	Expression* pExpression = p->GetExpression();
	if (!pExpression) return;

	if (pExpression->GetType() == Syntax::ASSIGNMENTEXPRESSION_ID)
	{
		AssignmentExpression* pAssignmentExpression = pExpression->As<AssignmentExpression>();
		OutVarInfo varInfo;
		if (ReadAssignmentExpression(pAssignmentExpression, varInfo))
		{
			m_mapOutVarTable[varInfo.GetName()] = varInfo.GetOutValue();
			m_aryOutVarInfo.push_back(varInfo);
		}
	}
	else if (pExpression->GetType()==Syntax::IDENTIFIER_ID)
	{
		Identifier* pIdentifier= pExpression->As<Identifier>();
		OutVarInfo varInfo;	//������Ϣ
		if (ReadIdentifier(pIdentifier, varInfo))
		{
			m_mapOutVarTable[varInfo.GetName()] = varInfo.GetOutValue();
			m_aryOutVarInfo.push_back(varInfo);
		}
	}
	else if (pExpression->GetType() == Syntax::CALLEXPRESSION_ID)
	{
		CallExpression* pCallExpression = pExpression->As<CallExpression>();
		OutVarInfo varInfo;
		if (ReadCallExpression(pCallExpression, varInfo))
		{
			m_mapOutVarTable[varInfo.GetName()] = varInfo.GetOutValue();
			m_aryOutVarInfo.push_back(varInfo);
		}
	}
	else if (pExpression->GetType() == Syntax::NUMERICLITERAL_ID)
	{
		NumericLiteral* pNumericLiteral = pExpression->As<NumericLiteral>();
		OutVarInfo varInfo;	//������Ϣ
		if (ReadNumericLiteral(pNumericLiteral, varInfo))
		{
			m_mapOutVarTable[varInfo.GetName()] = varInfo.GetOutValue();
			m_aryOutVarInfo.push_back(varInfo);
		}
	}
	else if (pExpression->GetType() == Syntax::BINARYEXPRESSION_ID)
	{
		OutVarInfo varInfo;	//������Ϣ
		if (ReadBinaryExpression(pExpression, varInfo))
		{
			m_mapOutVarTable[varInfo.GetName()] = varInfo.GetOutValue();
			m_aryOutVarInfo.push_back(varInfo);
		}
	}
	else if (pExpression->GetType() == Syntax::STRINGLITERAL_ID)
	{
		StringLiteral* pStringLiteral = pExpression->As<StringLiteral>();
		OutVarInfo varInfo;	//������Ϣ
		if (ReadStringLiteral(pStringLiteral, varInfo))
		{
			m_mapOutVarTable[varInfo.GetName()] = varInfo.GetOutValue();
			m_aryOutVarInfo.push_back(varInfo);
		}
	}
	else if (pExpression->GetType() == Syntax::LOGICALEXPRESSION_ID)		//�߼���� �� T1 AND T2 
	{
		OutVarInfo varInfo;		//������Ϣ
		LogicalExpression* pLogicalExpression = pExpression->As<LogicalExpression>();
		if (ReadLogicalExpression(pLogicalExpression, varInfo))
		{
			m_mapOutVarTable[varInfo.GetName()] = varInfo.GetOutValue();
			m_aryOutVarInfo.push_back(varInfo);
		}
	}
	else if (pExpression->GetType() == Syntax::UNARYEXPRESSION_ID)
	{
		UnaryExpression* pUnaryExpression = pExpression->As<UnaryExpression>();
		OutVarInfo varInfo;		//������Ϣ
		if (ReadUnaryExpression(pUnaryExpression, varInfo))
		{
			m_mapOutVarTable[varInfo.GetName()] = varInfo.GetOutValue();
			m_aryOutVarInfo.push_back(varInfo);
		}

	}
	else if (pExpression->GetType() == Syntax::SEQUENCEEXPRESSION_ID)
	{
		SequenceExpression* pSequenceExpression = pExpression->As<SequenceExpression>();

		OutVarInfo varInfo;	//������Ϣ
		const auto& aryExpressions = pSequenceExpression->GetExpressions();
		for(int i=0; i<(int)aryExpressions.size(); ++i)
		//for (const auto& itemExpression : pSequenceExpression->GetExpressions())
		{
			const auto& itemExpression = aryExpressions[i];
			if (itemExpression->GetType() == Syntax::ASSIGNMENTEXPRESSION_ID)
			{
				AssignmentExpression* pAssignmentExpression = itemExpression->As<AssignmentExpression>();
				ReadAssignmentExpression(pAssignmentExpression, varInfo);
			}
			else if (itemExpression->GetType() == Syntax::BINARYEXPRESSION_ID)
			{
				ReadBinaryExpression(itemExpression, varInfo);
			}
			else if (itemExpression->GetType() == Syntax::IDENTIFIER_ID)
			{
				Identifier* pIdentifier = itemExpression->As<Identifier>();
				if (i == 0) 
					ReadIdentifier(pIdentifier, varInfo);
				else
					ReadVarAttribute(pIdentifier, varInfo);
			}
			else if (itemExpression->GetType() == Syntax::NUMERICLITERAL_ID)
			{
				NumericLiteral* pNumericLiteral = itemExpression->As<NumericLiteral>();
				ReadNumericLiteral(pNumericLiteral, varInfo);
			}
			else if (itemExpression->GetType() == Syntax::CALLEXPRESSION_ID)
			{
				if (i == 0)
				{
					CallExpression* pCallExpression = itemExpression->As<CallExpression>();
					ReadCallExpression(pCallExpression, varInfo);
				}
				else
				{
					CallExpression* pCallExpression = itemExpression->As<CallExpression>();
					ReadCallExpressionExtend(pCallExpression, varInfo);
				}
				
			}
			else if (itemExpression->GetType() == Syntax::UNARYEXPRESSION_ID)
			{
				if (i == 0)
				{
					UnaryExpression* pUnaryExpression = itemExpression->As<UnaryExpression>();
					ReadUnaryExpression(pUnaryExpression, varInfo);
				}
			}
		}

		if (!varInfo.GetName().empty())
		{
			m_mapOutVarTable[varInfo.GetName()] = varInfo.GetOutValue();
			m_aryOutVarInfo.push_back(varInfo);
		}
	}
}

bool Execute::ReadUnaryExpression(UnaryExpression* pUnaryExpression, OutVarInfo& varInfo)
{
	Expression* pArgument = pUnaryExpression->GetArgument();
	bool bResult = false;
	if (pArgument->GetType() == Syntax::IDENTIFIER_ID)
	{
		Identifier* pIdentifier = pArgument->As<Identifier>();
		bResult = ReadIdentifier(pIdentifier, varInfo);
	}
	else if (pArgument->GetType() == Syntax::NUMERICLITERAL_ID)
	{
		NumericLiteral* pNumericLiteral = pArgument->As<NumericLiteral>();
		bResult = ReadNumericLiteral(pNumericLiteral, varInfo);
	}
	else if (pArgument->GetType() == Syntax::BINARYEXPRESSION_ID)
	{
		bResult = ReadBinaryExpression(pArgument, varInfo);
	}
	else if (pArgument->GetType() == Syntax::CALLEXPRESSION_ID)
	{
		CallExpression* pCallExpression = pArgument->As<CallExpression>();
		bResult = ReadCallExpression(pCallExpression, varInfo);
	}
	else
	{
		return false;
	}

	if (!bResult) return false;

	
	if (pUnaryExpression->GetOperator() == L"-")
	{
		Variant varZeor;
		varZeor.SetDoubleValue(0);
		varInfo.GetOutValue();
		Variant* pTempValue = varInfo.GetOutValue();
		Variant* pValue = m_VariantOperator.Subtract(varZeor, *pTempValue);
		varInfo.SetOutValue(pValue);
	}

	return true;
}

bool Execute::ReadStringLiteral(StringLiteral* pStringLiteral, OutVarInfo& varInfo)
{
	if (!pStringLiteral) return false;

	varInfo.SetName(GetTempVarName());
	Variant* pVariant = NULL;
	const std::wstring& strValue = pStringLiteral->GetValue();
	if (strValue.find('$') > 0)
	{
		pVariant = ReadOtherSymbolData(strValue, pStringLiteral);
	}
	else
	{
		pVariant = m_VariantOperator.Create();
		pVariant->SetStringValue(strValue);
	}
	
	varInfo.SetOutValue(pVariant);
	varInfo.AddAttribute(OutVarAttribute::NONE_VAR_NAME);

	return true;
}

bool Execute::ReadAssignmentExpression(AssignmentExpression* pAssignmentExpression, OutVarInfo& varInfo)
{
	if (!pAssignmentExpression) return false;

	if (pAssignmentExpression->GetOperator() == L":" && pAssignmentExpression->GetLeft())
	{
		Expression* pLeft = pAssignmentExpression->GetLeft();
		if (pLeft->GetType() == Syntax::IDENTIFIER_ID)
		{
			auto strName = pLeft->As<Identifier>()->GetName();
			auto find = m_mapVarTable.find(strName);
			if (find != m_mapVarTable.end())
			{
				varInfo.SetName(strName);
				varInfo.SetOutValue(find->second);

				if(strName.find(L"@NODRAW@")!=std::wstring::npos)
					varInfo.AddAttribute(OutVarAttribute::NODRAW);
				return true;
			}
		}
	}

	return false;
}

bool Execute::ReadIdentifier(Identifier* pIdentifier, OutVarInfo& varInfo)
{
	if (!pIdentifier) return false;

	const auto strName = pIdentifier->GetName();
	const auto pVarraint = ReadVariable(strName, pIdentifier);
	
	varInfo.SetName(GetTempVarName());
	varInfo.SetOutValue(pVarraint);
	varInfo.AddAttribute(OutVarAttribute::NONE_VAR_NAME);

	return true;
}

bool Execute::ReadVarAttribute(Identifier* pIdentifier, OutVarInfo& varInfo)
{
	const auto strName = pIdentifier->GetName();

	if (OutVariantAttribute::GetInstance().IsAttributeKey(strName))
	{
		varInfo.AddAttribute(strName);
		return true;
	}
	else
	{
		//varInfo.AddAttribute(strName);
	}

	return false;
}

bool Execute::ReadBinaryExpression(Expression* pExpression, OutVarInfo& varInfo)
{
	if (!pExpression) return false;
	//BinaryExpression* pBinaryExpression = pExpression->As<BinaryExpression>();
	const auto pVariant=pExpression->GetVariantOut();

	varInfo.SetName(GetTempVarName());
	varInfo.SetOutValue(pVariant);
	varInfo.AddAttribute(OutVarAttribute::NONE_VAR_NAME);

	return true;
}

bool Execute::ReadNumericLiteral(NumericLiteral* pNumericLiteral, OutVarInfo& varInfo)
{
	if (!pNumericLiteral) return false;

	double dValue = pNumericLiteral->GetValue();

	auto pVariant = m_VariantOperator.Create();
	pVariant->SetDoubleValue(dValue);

	varInfo.SetName(GetTempVarName());
	varInfo.SetOutValue(pVariant);
	varInfo.AddAttribute(OutVarAttribute::NONE_VAR_NAME);
	return true;
}

bool Execute::ReadCallExpression(CallExpression* pCallExpression, OutVarInfo& varInfo)
{
	if (!pCallExpression) return false;

	auto strName = pCallExpression->GetCalleeName();
	auto pVariant = pCallExpression->GetVariantOut();
	if (pVariant->GetType() == Variant::TESTSKIP_TYPE) return false;

	varInfo.SetName(GetTempVarName());
	varInfo.SetOutValue(pVariant);
	varInfo.AddAttribute(OutVarAttribute::NONE_VAR_NAME);

	if (OutVariantAttribute::GetInstance().IsDrawKey(strName))	//��ͼ���������м��Ϻ�����
	{
		if (pVariant)
		{
			auto pDrawData = pVariant->GetDrawData();
			if (pDrawData)
				varInfo.AddAttribute(L"Function", pDrawData->_strDrawName);
		}
	}

	return true;
}

bool Execute::ReadCallExpressionExtend(CallExpression* pCallExpression, OutVarInfo& varInfo)
{
	if (!pCallExpression) return false;

	auto strName = pCallExpression->GetCalleeName();
	auto pVariant = pCallExpression->GetVariantOut();

	if (strName == L"RGB" || strName == L"RGAB")
	{
		if (pVariant && pVariant->IsVaildString())
		{
			varInfo.AddAttribute(L"Color", pVariant->GetStringValue());
			return true;
		}
	}
	else if (strName == L"ICON")
	{
		double dValue = 0;
		Variant* pVarOut = varInfo.GetOutValue();
		const DrawData* pDrawData = NULL;
		if (pVariant) pDrawData=pVariant->GetDrawData();
		if (pVarOut && pVarOut->GetType() == Variant::ARRAY_DOUBLE_TYPE)
		{
			const auto& aryCond = pVarOut->GetArrayValue();
			auto& aryValue = pVariant->GetArrayValue();
			int nCount = (int)aryCond.size();
			for (int i = 0; i < nCount; ++i)
			{
				const auto& itemCond = aryCond[i];
				if (itemCond.IsVaild()) continue;

				if (i<(int)aryValue.size()) aryValue[i].SetNull();
			}

			varInfo.SetOutValue(pVariant);
			if (pDrawData) varInfo.AddAttribute(L"Function", pDrawData->_strDrawName);
			varInfo.AddAttribute(OutVarAttribute::NONE_VAR_NAME);
		}
		else if (pVarOut && pVarOut->GetType() == Variant::DOUBLE_TYPE)
		{
			if (pVarOut->IsVaildDoulbe())
			{
				dValue = pVarOut->GetDoubleValue();
				if (dValue)
				{
					varInfo.SetOutValue(pVariant);
					varInfo.AddAttribute(OutVarAttribute::NONE_VAR_NAME);
					if (pDrawData) varInfo.AddAttribute(L"Function", pDrawData->_strDrawName);
				}
				else
				{
					varInfo.SetOutValue(NULL);
					varInfo.SetName(L"");
				}
			}
		}
	}

	return false;
}

bool Execute::ReadLogicalExpression(LogicalExpression* pLogicalExpression, OutVarInfo& varInfo)
{
	if (!pLogicalExpression) return false;

	auto pVariant = pLogicalExpression->GetVariantOut();

	varInfo.SetName(GetTempVarName());
	varInfo.SetOutValue(pVariant);
	varInfo.AddAttribute(OutVarAttribute::NONE_VAR_NAME);

	return true;
}

const std::wstring& Execute::GetTempVarName()
{
	int32_t nValue = m_nCounter;
	++m_nCounter;

	std::wstringstream strDescription;
	strDescription << L"__temp_ " << m_nCounter << L"__";
	m_strTempVarName = strDescription.str();

	return m_strTempVarName;
}

void Execute::VisitNode(Node* pNode)
{
	int nType = pNode->GetType();
	switch (nType)
	{
	case Syntax::SEQUENCEEXPRESSION_ID:
		{	
			SequenceExpression* p = pNode->As<SequenceExpression>();
			VisitSequenceExpression(p);
		}
		break;
	case Syntax::EXPRESSIONSTATEMENT_ID:
		{
			ExpressionStatement* p = pNode->As<ExpressionStatement>();
			VisitNode(p->GetExpression());
		}
		break;
	case Syntax::ASSIGNMENTEXPRESSION_ID:
		{
			AssignmentExpression* p = pNode->As<AssignmentExpression>();
			VisitAssignmentExpression(p);
		}
		break;
	case Syntax::BINARYEXPRESSION_ID:
	case Syntax::LOGICALEXPRESSION_ID:
		VisitBinaryExpression(pNode);
		break;
	case Syntax::CALLEXPRESSION_ID:
		{
			CallExpression* p = pNode->As<CallExpression>();
			VisitCallExpression(p);
		}
		break;
	case Syntax::UNARYEXPRESSION_ID:
		VisitUnaryExpression(pNode->As<UnaryExpression>());
		break;

	default:
		break;
	}
}

void Execute::VisitSequenceExpression(SequenceExpression* pNode)
{
	const std::vector<Expression *>& expressions = pNode->GetExpressions();
	for (auto item : expressions)
	{
		VisitNode(item);
	}
}

void Execute::VisitAssignmentExpression(AssignmentExpression* pNode)
{
	auto pLeft = pNode->GetLeft();
	if (pLeft->GetType() != Syntax::IDENTIFIER_ID) ThrowUnexpectedNode(pLeft);
	Identifier* pLeftIdentifier = pLeft->As<Identifier>();
	std::wstring strName = pLeftIdentifier->GetName();

	auto pRight = pNode->GetRight();
	int nRightType = pRight->GetType();
	Variant* pValue = NULL;
	if (nRightType == Syntax::NUMERICLITERAL_ID)
	{
		pValue = pRight->GetVariant(&m_VariantOperator);
	}
	else if (nRightType == Syntax::BINARYEXPRESSION_ID || nRightType == Syntax::LOGICALEXPRESSION_ID)
	{
		pValue = VisitBinaryExpression(pRight);
	}
	else if (nRightType == Syntax::IDENTIFIER_ID)
	{
		Identifier* p = pRight->As<Identifier>();
		pValue = ReadVariable(p->GetName(), p);
	}
	else if (nRightType == Syntax::CALLEXPRESSION_ID)
	{
		CallExpression* p = pRight->As<CallExpression>();
		pValue = VisitCallExpression(p);
	}
	else if (nRightType == Syntax::MEMBEREXPRESSION_ID)
	{
		MemberExpression* p = pRight->As<MemberExpression>();
		pValue = ReadMemberVariable(p);
	}
	else if (nRightType == Syntax::UNARYEXPRESSION_ID)
	{
		UnaryExpression* p = pRight->As<UnaryExpression>();
		pValue=VisitUnaryExpression(p);
	}
	else if (nRightType == Syntax::STRINGLITERAL_ID)
	{
		StringLiteral* p = pRight->As<StringLiteral>();
		pValue = ReadStringData(p);
	}
	else
	{
		//TRACE_WARNING();
	}

	m_mapVarTable[strName] = pValue;
}

Variant* Execute::VisitBinaryExpression(Node* pNode)
{
	std::vector<Node*> stack;
	stack.push_back(pNode);
	Node* pTemp = NULL;
	int nTempType = 0;
	Node* pLeft = NULL;
	Node* pRight = NULL;

	while (!stack.empty())
	{
		pTemp = stack[stack.size() - 1];
		nTempType = pTemp->GetType();
		pLeft = pRight = NULL;
		if (nTempType == Syntax::BINARYEXPRESSION_ID)
		{
			BinaryExpression* p = pTemp->As<BinaryExpression>();
			pLeft = p->GetLeft();
			pRight = p->GetRight();
		}
		else if (nTempType == Syntax::LOGICALEXPRESSION_ID)
		{
			LogicalExpression* p = pTemp->As<LogicalExpression>();
			pLeft = p->GetLeft();
			pRight = p->GetRight();
		}

		if (pLeft && pLeft != pNode && pRight != pNode)
		{
			stack.push_back(pLeft);
		}
		else if (pRight && pRight != pNode)
		{
			stack.push_back(pRight);
		}
		else
		{
			Node* pValueNode = stack[stack.size() - 1];
			stack.pop_back();
			if (pValueNode->GetType() == Syntax::BINARYEXPRESSION_ID)
			{
				BinaryExpression* p = pTemp->As<BinaryExpression>();
				const Variant* pLeftValue = GetNodeValue(p->GetLeft());
				const Variant* pRightValue = GetNodeValue(p->GetRight());
				pValueNode->SetVariantOut(NULL);
				const std::wstring& strOp = p->GetOperator();
				Variant* pVariantOut = NULL;
				if (pLeftValue && pRightValue)
				{
					if (strOp == L"-") pVariantOut = m_VariantOperator.Subtract(*pLeftValue, *pRightValue);
					else if (strOp == L"*") pVariantOut = m_VariantOperator.Multiply(*pLeftValue, *pRightValue);
					else if (strOp == L"/") pVariantOut = m_VariantOperator.Divide(*pLeftValue, *pRightValue);
					else if (strOp == L"+") pVariantOut = m_VariantOperator.Add(*pLeftValue, *pRightValue);
					else if (strOp == L">") pVariantOut = m_VariantOperator.GT(*pLeftValue, *pRightValue);
					else if (strOp == L">=") pVariantOut = m_VariantOperator.GTE(*pLeftValue, *pRightValue);
					else if (strOp == L"<") pVariantOut = m_VariantOperator.LT(*pLeftValue, *pRightValue);
					else if (strOp == L"<=") pVariantOut = m_VariantOperator.LTE(*pLeftValue, *pRightValue);
					else if (strOp == L"=" || strOp == L"==") pVariantOut = m_VariantOperator.EQ(*pLeftValue, *pRightValue);
					else if (strOp == L"<>" || strOp == L"!=") pVariantOut = m_VariantOperator.NEQ(*pLeftValue, *pRightValue);
				}

				pValueNode->SetVariantOut(pVariantOut);
			}
			else if (pValueNode->GetType() == Syntax::LOGICALEXPRESSION_ID)
			{
				LogicalExpression* p = pTemp->As<LogicalExpression>();
				const Variant* pLeftValue = GetNodeValue(p->GetLeft());
				const Variant* pRightValue = GetNodeValue(p->GetRight());
				pValueNode->SetVariantOut(NULL);
				const std::wstring& strOp = p->GetOperator();
				Variant* pVariantOut = NULL;
				if (pLeftValue && pRightValue)
				{
					if (strOp == L"&&" || strOp == L"AND") pVariantOut = m_VariantOperator.And(*pLeftValue, *pRightValue);
					else if (strOp == L"||" || strOp == L"OR") pVariantOut = m_VariantOperator.Or(*pLeftValue, *pRightValue);
				}

				pValueNode->SetVariantOut(pVariantOut);
			}

			pNode = pTemp;
		}
	}

	return pNode->GetVariantOut();
}

Variant* Execute::GetNodeValueEx(Node* pNode)
{
	Variant* pValue = NULL;
	int nType= pNode->GetType();
	if (nType == Syntax::BINARYEXPRESSION_ID || nType == Syntax::LOGICALEXPRESSION_ID)
	{
		pValue = VisitBinaryExpression(pNode->As<BinaryExpression>());
	}
	else if (nType == Syntax::CALLEXPRESSION_ID)
	{
		pValue = VisitCallExpression(pNode->As<CallExpression>());
	}
	else
	{
		pValue = GetNodeValue(pNode);
	}

	return pValue;
}

Variant* Execute::GetNodeValue(Node* pNode)
{
	int nType = pNode->GetType();
	switch (nType)
	{
	case Syntax::NUMERICLITERAL_ID:
		{
			Variant* pValue = pNode->GetVariant(&m_VariantOperator);
			return pValue;
		}
	case Syntax::UNARYEXPRESSION_ID:
		{
			UnaryExpression* p = pNode->As<UnaryExpression>();
			Variant* pValue=VisitUnaryExpression(p);
			return pValue;
		}
	case Syntax::IDENTIFIER_ID:	//����
		{
			Identifier* p = pNode->As<Identifier>();
			Variant* pValue = ReadVariable(p->GetName(), p);
			return pValue;
		}
	case Syntax::STRINGLITERAL_ID:	//�ַ���
		{
			StringLiteral* p = pNode->As<StringLiteral>();
			Variant* pValue = m_VariantOperator.Create();
			pValue->SetStringValue(p->GetValue());
			return pValue;
		}
	case Syntax::BINARYEXPRESSION_ID:
	case Syntax::LOGICALEXPRESSION_ID:
		return pNode->GetVariantOut();
	case Syntax::CALLEXPRESSION_ID:
		return VisitCallExpression(pNode->As<CallExpression>());
	default:
		ThrowUnexpectedNode(pNode->As<UnaryExpression>());
		return NULL;
	}
}

Variant* Execute::VisitUnaryExpression(UnaryExpression* pNode)
{
	Variant* pValue = NULL;
	Expression* pArgument = pNode->GetArgument();

	if (pNode->GetOperator() == L"-")
	{
		Variant* pTempValue = GetNodeValueEx(pArgument);
		if (pTempValue)
		{
			Variant varZeor;
			varZeor.SetDoubleValue(0);
			pValue = m_VariantOperator.Subtract(varZeor, *pTempValue);
		}
	}
	else
	{
		pValue = GetNodeValueEx(pArgument);
	}

	return pValue;
}

Variant* Execute::VisitCallExpression(CallExpression* pNode)
{
	std::wstring strFuncName = pNode->GetCalleeName();
	const CallExpression::ARRAY_ARGUMENT& arguments = pNode->GetArguments();
	long lType = 0;
	Variant::ARRAY_CALL_ARGUMENT args;
	Variant* pValue = NULL;
	bool bIFCFunc = (strFuncName == L"IFC");
	for (int i=0;i< (int)arguments.size();++i) 
	{
		const auto& item = arguments[i];
		lType = item->GetType();

		if (bIFCFunc && i >= 1)
			break;

		pValue = NULL;
		if (lType == Syntax::BINARYEXPRESSION_ID || lType == Syntax::LOGICALEXPRESSION_ID)
		{
			pValue = VisitBinaryExpression(item);
		}
		else if (lType == Syntax::CALLEXPRESSION_ID)
		{
			CallExpression* p = item->As<CallExpression>();
			pValue = VisitCallExpression(p);
		}
		else
		{
			pValue = GetNodeValue(item);
		}

		args.push_back(pValue);
	}

	if (bIFCFunc)
	{
		if (arguments.size() < 3)
		{
			std::wstringstream strDescription;
			strDescription << L"����'" << strFuncName << L"'������������";
			ExecuteExcept error(strDescription.str(), L"VariantOperator::VisitCallExpression", pNode);
			throw error;
		}

		bool bResult = m_VariantOperator.IFC(*args[0]);
		const auto& item = bResult ? arguments[1] : arguments[2];

		lType = item->GetType();
		if (lType == Syntax::BINARYEXPRESSION_ID || lType == Syntax::LOGICALEXPRESSION_ID)
		{
			pValue = VisitBinaryExpression(item);
		}
		else if (lType == Syntax::CALLEXPRESSION_ID)
		{
			CallExpression* p = item->As<CallExpression>();
			pValue = VisitCallExpression(p);
		}
		else
		{
			pValue = GetNodeValue(item);
		}

		pNode->SetVariantOut(pValue);
		return pNode->GetVariantOut();
	}

	CUSTOM_FUNCTION_ITEM funcItem;
	Variant* pVariantOut = NULL;
	if (strFuncName == L"FINANCE")
	{
		pVariantOut = m_pHistoryData->GetFinance(args, pNode);
	}
	else if (strFuncName==L"DYNAINFO")
	{
		pVariantOut = m_pHistoryData->GetDynaInfo(args, pNode);
	}
	else if (strFuncName == L"ASKPRICE")
	{
		VariantOperator::CheckFunctinArgument(L"ASKPRICE", args, 1, pNode);
		pVariantOut = GetDynaInfo(10010, *args[0], pNode);
	}
	else if (strFuncName == L"ASKVOL")
	{
		VariantOperator::CheckFunctinArgument(L"ASKVOL", args, 1, pNode);
		pVariantOut = GetDynaInfo(10020, *args[0], pNode);
	}
	else if (strFuncName == L"BIDPRICE")
	{
		VariantOperator::CheckFunctinArgument(L"BIDPRICE", args, 1, pNode);
		pVariantOut = GetDynaInfo(10030, *args[0], pNode);
	}
	else if (strFuncName == L"BIDVOL")
	{
		VariantOperator::CheckFunctinArgument(L"BIDVOL", args, 1, pNode);
		pVariantOut = GetDynaInfo(10040, *args[0], pNode);
	}
	else if (strFuncName == L"BLOCKSETNUM")
	{
		VariantOperator::CheckFunctinArgument(L"BLOCKSETNUM", args, 1, pNode);
		pVariantOut = m_pHistoryData->GetBlockMemberCount(args, pNode);
	}
	else if (strFuncName == L"HORCALC")
	{
		VariantOperator::CheckFunctinArgument(L"HORCALC", args, 4, pNode);
		pVariantOut = m_pHistoryData->GetBlockCalculate(args,pNode);
	}
	else if (strFuncName == L"SENDMESSAGE")
	{
		if (m_pSendMessage)
		{
			m_pSendMessage->m_Argument = args;
			m_pSendMessage->m_pNode = pNode;
			long lValue = m_pSendMessage->operator()();
			pVariantOut = m_VariantOperator.Create();
			pVariantOut->SetDoubleValue(lValue);
		}
	}
	else if (strFuncName == L"RPCCALL")
	{
		pVariantOut = m_VariantOperator.RPCCall(args, m_pHistoryData, pNode);
	}
	else if (strFuncName == L"TESTSKIP")
	{
		if (arguments.size() < 1)
		{
			std::wstringstream strDescription;
			strDescription << L"����'" << strFuncName << L"'������������";
			ExecuteExcept error(strDescription.str(), L"VariantOperator::VisitCallExpression", pNode);
			throw error;
		}

		bool bExit = m_VariantOperator.TESTSKIP(*args[0]);
		if (bExit)	//�ж�
		{
			m_Interrupt._bExit = true;
			SourceLocation* pLocation = pNode->GetLocation();
			if (pLocation && pLocation->_pStart)
			{
				m_Interrupt._nLine = pLocation->_pStart->GetLine();
				m_Interrupt._nColumn = pLocation->_pStart->GetColumn();
			}

			pVariantOut = m_VariantOperator.Create();
			pVariantOut->SetType(Variant::TESTSKIP_TYPE);
		}
	}
	else if (SET_HQ_VAR_NAME.find(strFuncName) != SET_HQ_VAR_NAME.end())	//�������
	{
		VariantOperator::CheckFunctinArgument(strFuncName, args, 1, pNode);
		pVariantOut = ReadOtherSymbolData(*args[0], strFuncName, pNode);
	}
	else if (CustomFunction::GetInstance().Get(strFuncName, funcItem))
	{
		pVariantOut = CallCustomFunction(funcItem, args, m_pHistoryData, pNode);
	}
	else
	{
		pVariantOut = m_VariantOperator.CallFunction(strFuncName, args, m_pHistoryData,pNode);
	}
	
	pNode->SetVariantOut(pVariantOut);

	return pNode->GetVariantOut();
}

Variant* Execute::CallCustomFunction(const CUSTOM_FUNCTION_ITEM& funcItem, const Variant::ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode)
{
	std::vector<double> aryArgment;
	if (funcItem._nArgCount == 0)
	{
		return m_pHistoryData->CallCustomFunction(funcItem._strName, args, pHistoryData, pNode);
	}

	if (args.size() != funcItem._nArgCount)
	{
		std::wstringstream strDescription;
		strDescription << L"����'" << funcItem._strName << L"'������������";
		ExecuteExcept error(strDescription.str(), L"VariantOperator::CallCustomFunction", pNode);
		throw error;
	}

	return m_pHistoryData->CallCustomFunction(funcItem._strName, args, pHistoryData, pNode);
}



void Execute::ThrowUnexpectedNode(Node* pNode)
{
	ThrowUnexpectedNode(L"ִ���쳣", pNode);
}

void Execute::ThrowUnexpectedNode(const std::wstring& strMessage, Node* pNode)
{
	ExecuteExcept error(strMessage, L"Execute::ThrowUnexpectedNode", pNode);
	throw error;
}


void Execute::InitalConstVarTable()
{
	const wchar_t VARIANT_NAME[][16] =
	{
		L"CLOSE", L"VOL", L"OPEN", L"HIGH", L"LOW", L"AMOUNT", L"ADVANCE", L"DECLINE",
		L"C", L"V", L"O", L"H", L"L", L"AMO", L"TIME", L"DATE", 
		L"WEEKDAY", L"WEEK", L"DAY", L"YEAR", L"MONTH", L"HOUR", L"MINUTE", L"FROMOPEN",
		L"LDAY", L"LMONTH", L"LYEAR",	//ũ������
		L"CAPITAL",L"TOTALCAPITAL",
		L"INDEXC", L"INDEXH", L"INDEXL", L"INDEXO", L"INDEXV", L"INDEXA", L"INDEXADV", L"INDEXDEC",
		L"CURRBARSCOUNT", L"TOTALBARSCOUNT",L"BARPOS",
		L"SELLVOL", L"BUYVOL", L"ISBUYORDER", L"ISSELLORDER",

		L"ISLASTBAR",
		L"BARSTATUS",

		L"CODE",		//���� ����׺
		L"STKLABEL",	//���� ������׺
		L"STKNAME",		//����
		L"BKNAME",		//�������

		L"MARKETLABEL",	//�г�����
		L"MARKETNAME",	//�г�����

		L"DRAWNULL",
		L"NULL",

		L"MACHINEDATE",	//ȡ�õ�ǰ�ͻ��˻�����1900�����ĵ�������
		L"MACHINETIME",	//ȡ�õ�ǰ�ͻ��˻�����ʱ��,����11:01:15ʱΪ110115
		L"MACHINEWEEK",	//ȡ�õ�ǰ�ͻ��˻���Ϊ���ڼ�(1,2,3,4,5,6,0)

		/*
			DATATYPE	��������
			���ͣ�DATATYPE�������������ͣ���ʱ��=1,�ֱʳɽ�=2,1������=3,5������=4,15������=5,30������=6,60������=7,����=8,����=9,����=10,������=11,����=12,����=13,������=14��
			ע�⣺�ú������س���
		*/
		L"DATATYPE",
		L"TYPE",

		L"ISDOWN",
		L"ISEQUAL",
		L"ISUP",


		L"NONE",	//������
	};

	m_mapConstVarTable.clear();
	for (int i = 0; i < ARRAYSIZE(VARIANT_NAME);++i)
	{
		m_mapConstVarTable.insert(PARI_VARIANT(VARIANT_NAME[i], NULL));
	}

	const wchar_t REALTIME_VARIANT_NAME[][16] =
	{
		L"CURRENTDATE",	//��ǰ����
		L"CURRENTTIME",	//��ǰʱ��
	};

	m_setConstRealTimeVarTable.clear();
	for (const auto& item : REALTIME_VARIANT_NAME)
	{
		m_setConstRealTimeVarTable.insert(item);
	}
}

void Execute::InitalVarTable()
{
	m_mapVarTable.clear();
	m_mapOutVarTable.clear();
	m_aryOutVarInfo.clear();

	for (auto iter : m_aryArguments)
	{
		if (iter._lValueType == 0)
		{
			Variant* pVariant = m_VariantOperator.Create(iter._dValue);
			m_mapVarTable.insert(PARI_VARIANT(iter._strName, pVariant));
		}
		else if (iter._lValueType == 1)
		{
			Variant* pVariant = m_VariantOperator.Create();
			pVariant->SetStringValue(iter._strValue);
			m_mapVarTable.insert(PARI_VARIANT(iter._strName, pVariant));
		}
	}
}

Variant* Execute::ReadVariable(const std::wstring& strName, Identifier* pNode)
{
	{//ʵʱ����������
		auto find = m_setConstRealTimeVarTable.find(strName);
		if (find!= m_setConstRealTimeVarTable.end())
		{
			return ReadSymbolData(strName, pNode);
		}
	}
	
	MAP_VARIANT::iterator find = m_mapConstVarTable.find(strName);
	if (find != m_mapConstVarTable.end())
	{
		if (find->second == NULL)
		{
			find->second = ReadSymbolData(strName, pNode);
		}
		return find->second;
	}

	find = m_mapVarTable.find(strName);
	if (find != m_mapVarTable.end())
	{
		return find->second;
	}

	if (CustomVariant::GetInstance().IsExist(strName))	//�ⲿ�Զ������
	{
		return m_pHistoryData->GetCustomValue(strName, pNode);
	}

	if (strName.find('#') !=std::wstring::npos)
	{
		return ReadPeriodData(strName, pNode);
	}

	if (strName.find(L"COLOR") == 0)
	{
		Variant* pVarColor = ReadColorData(strName);
		if (pVarColor) return pVarColor;
	}

	std::wstringstream strMessage;
	strMessage << L"\"" << strName << L"\"" << L" not exist.";
	ThrowUnexpectedNode(strMessage.str(), pNode);

	return NULL;
}

Variant* Execute::ReadMemberVariable(MemberExpression* pNode)
{
	Expression* pObject = pNode->GetRObject();
	Expression* pProperty = pNode->GetProperty();
	if (pObject == NULL || pProperty == NULL) return NULL;
	if (pObject->GetType() != Syntax::IDENTIFIER_ID || pProperty->GetType() != Syntax::IDENTIFIER_ID) return NULL;

	Identifier* pIdentifierObject = pObject->As<Identifier>();
	Identifier* pIdentifierPropert = pProperty->As<Identifier>();

	const std::wstring& strObjectName = pIdentifierObject->GetName();
	const std::wstring& strPropertyName = pIdentifierPropert->GetName();
	std::wstring strName = strObjectName + L"." + strPropertyName;

	TRACE_DEBUG(L"[Execute::ReadMemberVariable] read %ls.%ls value ......", strObjectName.c_str(), strPropertyName.c_str());
	//KDJ.K#WEEK;
	MAP_VARIANT::iterator find = m_mapVarTable.find(strObjectName);
	if (find == m_mapVarTable.end()) 
		return m_VariantOperator.CallIndex(strName, m_pHistoryData, pNode);

	Variant* pValue = find->second;
	if (!pValue || pValue->GetType() != Variant::MEMBER_TYPE) 
		return m_VariantOperator.CallIndex(strName, m_pHistoryData, pNode);

	Variant* pMemberValue = pValue->GetMemberValue(strPropertyName);

	return pMemberValue;
}

Variant* Execute::ReadColorData(const std::wstring& strName)
{
	const std::set<std::wstring> DEFAULT_COLOR(
		{
			L"COLORBLACK",L"COLORBLUE", L"COLORGREEN", L"COLORCYAN",L"COLORRED",L"COLORMAGENTA", L"COLORBROWN", L"COLORLIGRAY",L"COLORGRAY",
			L"COLORLIBLUE",L"COLORLIGREEN",L"COLORLICYAN", L"COLORLIRED", L"COLORLIMAGENTA", L"COLORWHITE",L"COLORYELLOW"
		}
	);

	if (DEFAULT_COLOR.find(strName) != DEFAULT_COLOR.end())
	{
		Variant* pResult = m_VariantOperator.Create();
		pResult->SetStringValue(strName);
		return pResult;
	}

	if (strName.length() != 11) return NULL;

	//COLOR �Զ���ɫ
	//��ʽΪCOLOR+��BBGGRR����BB��GG��RR��ʾ��ɫ����ɫ�ͺ�ɫ�ķ�����ÿ����ɫ��ȡֵ��Χ��00-FF��������16���ơ�
	//���磺MA5:MA(CLOSE,5)��COLOR00FFFF��ʾ����ɫ�봿��ɫ�Ļ��ɫ��COLOR808000��ʾ����ɫ�͵���ɫ�Ļ��ɫ��
	bool bVaild = false;
	for (long i = 5; i < (long)strName.length(); ++i)
	{
		const auto& item=strName[i];
		bVaild = (item >= L'0' && item <= L'9') || (item >= L'A' && item <= L'F');
		if (!bVaild) return NULL;
	}

	Variant* pResult = m_VariantOperator.Create();
	pResult->SetStringValue(strName);
	return pResult;
}


struct PERIOD_INFO_ITEM
{
	std::wstring _strName;
	long		_lPeriod;
	long		_lOrder;
};

const PERIOD_INFO_ITEM PERIOD_INFO_LIST[] =
{
	{L"MIN1",	4, 1 },
	{L"MIN5",	5, 2 },
	{L"MIN15",	6, 3 },
	{L"MIN30",	7, 4 },
	{L"MIN60",	8, 5 },

	{L"DAY",		0, 1000 },
	{L"WEEK",		1, 1005 },
	{L"WEEK2",		21,1010 },
	{L"MONTH",		2, 1030 },
	{L"SEASON",		9, 1090 },
	{L"HALFYEAR",	22,1180 },
	{L"YEAR",		3, 1365 },

	{L"DAY2", 40002, 1002 },
	{L"DAY3", 40003, 1003 },
	{L"DAY4", 40004, 1004 },
	{L"DAY5", 40005, 1005 },
	{L"DAY6", 40006, 1006 },
	{L"DAY7", 40007, 1007 },
	{L"DAY8", 40008, 1008 },
	{L"DAY9", 40009, 1009 },
	{L"DAY10", 40010, 1010 },
	{L"DAY11", 40011, 1011 },
	{L"DAY12", 40012, 1012 },
	{L"DAY13", 40013, 1013 },
	{L"DAY14", 40014, 1014 },
	{L"DAY15", 40015, 1015 },

};

bool GetPeriodInfoByName(const std::wstring& strName, PERIOD_INFO_ITEM& info)
{
	for (const auto& item : PERIOD_INFO_LIST)
	{
		if (strName == item._strName)
		{
			info = item;
			return true;
		}
	}

	return false;
}

bool GetPeriodInfoByID(long lPeriod, PERIOD_INFO_ITEM& info)
{
	for (const auto& item : PERIOD_INFO_LIST)
	{
		if (lPeriod == item._lPeriod)
		{
			info = item;
			return true;
		}
	}

	return false;
}

bool GetVarianName(const std::wstring& strName, std::wstring& strVarName)
{
	const std::map<std::wstring, std::wstring> MAP_VARIANT_NAME(
	{
		{L"CLOSE", L"C"}, {L"C",L"C"},
		{L"HIGH", L"H"}, {L"H", L"H"},
		{L"OPEN", L"O"}, {L"O", L"O"},
		{L"LOW", L"L"}, {L"L",L"L"},

		{L"VOL", L"V"}, {L"V", L"V"},
		{L"AMOUNT", L"AMO"}, {L"AMO", L"AMO"},
	});

	auto find = MAP_VARIANT_NAME.find(strName);
	if (find == MAP_VARIANT_NAME.end()) return false;

	strVarName = find->second;
	return true;
}


Variant* Execute::ReadPeriodData(const std::wstring& strName, Identifier* pNode)
{
	auto nPos=strName.find('#');
	if (nPos <= 0) return NULL;

	std::wstring strData[2] = { strName.substr(0, nPos), strName.substr(nPos + 1) };

	PERIOD_INFO_ITEM destPeriod;
	if (!GetPeriodInfoByName(strData[1], destPeriod))
	{
		std::wstringstream strBuffer;
		strBuffer << L"period name error.[Period=" << strData[1] << L"], [Name=" << strName << L"]";
		std::wstring strMessage = strBuffer.str();
		ExecuteExcept error(strMessage, L"Execute::ReadPeriodData", pNode);
		throw error;
	}

	std::wstring strVarName;
	if (!GetVarianName(strData[0], strVarName))
	{
		std::wstringstream strBuffer;
		strBuffer << L"period name error.[VarName=" << strData[0] << L"], [Name=" << strName << L"]";
		std::wstring strMessage = strBuffer.str();
		ExecuteExcept error(strMessage, L"Execute::ReadPeriodData", pNode);
		throw error;
	}

	long lSrcPeriod = m_pHistoryData->GetPeriod();
	if (destPeriod._lPeriod == lSrcPeriod)	//ͬ���ڵ�
	{
		return ReadSymbolData(strVarName, pNode);
	}

	PERIOD_INFO_ITEM srcPeriod;
	if (!GetPeriodInfoByID(lSrcPeriod, srcPeriod))
	{
		return NULL;
	}

	if (srcPeriod._lOrder > destPeriod._lOrder)
	{
		std::wstringstream strBuffer;
		strBuffer << L"can't convert period " << destPeriod._strName <<L" to "<< srcPeriod._strName<< L". [Name=" << strName<< L"]";
		std::wstring strMessage = strBuffer.str();
		ExecuteExcept error(strMessage, L"Execute::ReadPeriodData", pNode);
		throw error;
	}

	const IHistoryData::ARRAY_KDATA* pKData = m_pHistoryData->GetKData(m_pHistoryData->GetSymbolString(), destPeriod._lPeriod, m_pHistoryData->GetRight());
	if (!pKData)
	{
		IHistoryData* pHistoryData = (IHistoryData*)m_pHistoryData;
		pHistoryData->LoadKData(m_pHistoryData->GetSymbolString(), destPeriod._lPeriod, m_pHistoryData->GetRight(), pNode, strName);
		pKData = m_pHistoryData->GetKData(m_pHistoryData->GetSymbolString(), destPeriod._lPeriod, m_pHistoryData->GetRight());
	}
	if (!pKData)
	{
		std::wstringstream strBuffer;
		strBuffer << L"get period [" << destPeriod._strName << L"] data failed. [Name=" << strName << L"]";
		std::wstring strMessage = strBuffer.str();
		ExecuteExcept error(strMessage, L"Execute::ReadPeriodData", pNode);
		throw error;
	}

	return m_pHistoryData->GetKDataItem(*pKData, strVarName);
}

Variant* Execute::ReadStringData(StringLiteral* pNode)
{
	std::wstring strValue = pNode->GetValue();
	if (strValue.find('$') > 0)
	{
		return ReadOtherSymbolData(strValue, pNode);
	}

	Variant* pVariant=m_VariantOperator.Create();
	pVariant->SetStringValue(strValue);
	return pVariant;
}

Variant* Execute::ReadOtherSymbolData(const std::wstring& strParam, Node* pNode)
{
	int nPos = (int)strParam.find('$');
	if (nPos <= 0)
	{
		std::wstringstream strBuffer;
		strBuffer << L"'" << strParam << L"'��ʽ����";
		std::wstring strMessage = strBuffer.str();
		ExecuteExcept error(strMessage, L"Execute::ReadOtherSymbolData", pNode);
		throw error;
	}

	std::wstring strSymbol = strParam.substr(0, nPos);
	std::wstring strVarName = strParam.substr(nPos+1);
	std::transform(strVarName.begin(), strVarName.end(), strVarName.begin(), ::toupper);
	long lPeriod = m_pHistoryData->GetPeriod();
	long lRight = m_pHistoryData->GetRight();

	if (SET_HQ_VAR_NAME.find(strVarName) == SET_HQ_VAR_NAME.end())
	{
		std::wstringstream strBuffer;
		strBuffer << L"'" << strParam << L"'����������";
		std::wstring strMessage = strBuffer.str();
		ExecuteExcept error(strMessage, L"Execute::ReadOtherSymbolData", pNode);
		throw error;
	}

	const IHistoryData::ARRAY_KDATA* pKData = m_pHistoryData->GetKData(strSymbol, lPeriod, lRight);
	if (!pKData)
	{
		IHistoryData* pHistoryData = (IHistoryData*)m_pHistoryData;
		pHistoryData->LoadKData(strSymbol, lPeriod, lRight, pNode, strParam);
		pKData = m_pHistoryData->GetKData(strSymbol, lPeriod, lRight);
	}
	if (!pKData)
	{
		std::wstringstream strBuffer;
		strBuffer << L"'" << strParam << L"'���ݻ�ȡʧ��";
		std::wstring strMessage = strBuffer.str();
		ExecuteExcept error(strMessage, L"Execute::ReadOtherSymbolData", pNode);
		throw error;
	}

	return m_pHistoryData->GetKDataItem(*pKData, strVarName);
}

Variant* Execute::ReadOtherSymbolData(const Variant& varSymbol, const std::wstring& strVarName, Node* pNode)
{
	if (!varSymbol.IsVaildString())
	{
		std::wstringstream strBuffer;
		strBuffer << L"'" << strVarName << L"()��������";
		std::wstring strMessage = strBuffer.str();
		ExecuteExcept error(strMessage, L"Execute::ReadOtherSymbolData", pNode);
		throw error;
	}

	const std::wstring& strSymbol = varSymbol.GetStringValue();
	long lPeriod = m_pHistoryData->GetPeriod();
	long lRight = m_pHistoryData->GetRight();
	std::wstring strCallInfo = strVarName + L"('" + strSymbol + L"')";

	const IHistoryData::ARRAY_KDATA* pKData = m_pHistoryData->GetKData(strSymbol, lPeriod, lRight);
	if (!pKData)
	{
		IHistoryData* pHistoryData = (IHistoryData*)m_pHistoryData;
		pHistoryData->LoadKData(strSymbol, lPeriod, lRight, pNode, strCallInfo);
		pKData = m_pHistoryData->GetKData(strSymbol, lPeriod, lRight);
	}
	if (!pKData)
	{
		std::wstringstream strBuffer;
		strBuffer << L"'" << strCallInfo << L"'���ݻ�ȡʧ��";
		std::wstring strMessage = strBuffer.str();
		ExecuteExcept error(strMessage, L"Execute::ReadOtherSymbolData", pNode);
		throw error;
	}

	return m_pHistoryData->GetKDataItem(*pKData, strVarName);
}

Variant* Execute::ReadSymbolData(const std::wstring& strName, Identifier* pNode)
{
	Variant* pResult = NULL;

	if (strName == L"CLOSE" || strName == L"C")
		pResult = m_pHistoryData->GetClose();
	else if (strName == L"OPEN" || strName == L"O")
		pResult = m_pHistoryData->GetOpen();
	else if (strName == L"HIGH" || strName == L"H")
		pResult = m_pHistoryData->GetHigh();
	else if (strName == L"LOW" || strName == L"L")
		pResult = m_pHistoryData->GetLow();
	else if (strName == L"VOL" || strName == L"V")
		pResult = m_pHistoryData->GetVol();
	else if (strName == L"AMOUNT" || strName == L"AMO")
		pResult = m_pHistoryData->GetAmount();
	else if (strName == L"ZSTJJ")
		pResult = m_pHistoryData->GetAvPrice();
	else if (strName == L"VOLINSTK")
		pResult = m_pHistoryData->GetPosition();
	else if (strName == L"QHJSJ")
		pResult = m_pHistoryData->GetSettle();

	else if (strName == L"DATE")
		pResult = m_pHistoryData->GetDate();
	else if (strName == L"WEEKDAY" || strName == L"WEEK")
		pResult = m_pHistoryData->GetWeekDay();
	else if (strName == L"YEAR")
		pResult = m_pHistoryData->GetYear();
	else if (strName == L"MONTH")
		pResult = m_pHistoryData->GetMonth();
	else if (strName == L"DAY")
		pResult = m_pHistoryData->GetDay();
	else if (strName == L"TIME")
		pResult = m_pHistoryData->GetTime();
	else if (strName == L"HOUR")
		pResult = m_pHistoryData->GetHour();
	else if (strName == L"MINUTE")
		pResult = m_pHistoryData->GetMinute();

	else if (strName == L"CAPITAL")
		pResult = m_pHistoryData->GetCapital(pNode);
	else if (strName == L"TOTALCAPITAL")
		pResult = m_pHistoryData->GetTotalCaptialAll(pNode);
	else if (strName == L"INDEXA" || strName == L"INDEXC" || strName == L"INDEXH" || strName == L"INDEXO" || strName == L"INDEXV" || strName == L"INDEXL" ||
		strName == L"INDEXADV" || strName == L"INDEXDEC")
		pResult = m_pHistoryData->GetIndexData(strName, pNode);
	else if (strName == L"CURRBARSCOUNT")
		pResult = m_pHistoryData->GetCurrBarsCount();
	else if (strName == L"BARPOS")
		pResult = m_pHistoryData->GetBarPos();
	else if (strName == L"TOTALBARSCOUNT")
		pResult = m_pHistoryData->GetTotalBarsCount();
	else if (strName == L"FROMOPEN")
		pResult = m_pHistoryData->GetFromOpen(pNode);
	else if (strName == L"ADVANCE")
		pResult = m_pHistoryData->GetAdvance();
	else if (strName == L"DECLINE")
		pResult = m_pHistoryData->GetDecline();
	else if (strName == L"SELLVOL")		//��������, ��DYNAINFO(22) ��ʱ��������
		pResult = GetDynaInfo(22, pNode);
	else if (strName == L"BUYVOL")		//��������,��DYNAINFO(23) ��ʱ��������
		pResult = GetDynaInfo(23, pNode);

	else if (strName == L"ISBUYORDER")		//��������, ��DYNAINFO(10001) ��ʱ��������
		pResult = GetDynaInfo(10001, pNode);
	else if (strName == L"ISSELLORDER")		//����������, ��DYNAINFO(10002) ��ʱ��������
		pResult = GetDynaInfo(10002, pNode);

	else if (strName == L"CODE")	//����
		pResult = m_pHistoryData->GetSymbol(0);
	else if (strName == L"STKLABEL")
		pResult = m_pHistoryData->GetSymbol(1);
	else if (strName == L"STKNAME")	//����
		pResult = m_pHistoryData->GetName();
	else if (strName == L"BKNAME")	//�������
		pResult = m_pHistoryData->GetCategroyName(-1);
	else if (strName == L"MARKETLABEL")	//�г�����
		pResult = m_pHistoryData->GetMarketName();
	else if (strName == L"MARKETNAME")	//�г�����
		pResult = m_pHistoryData->GetMarketCNName();
	else if (strName == L"DATATYPE")	//��������
		pResult = m_pHistoryData->GetPeriodID();
	else if (strName == L"TYPE")		//��Ʊ����
		pResult = m_pHistoryData->GetSymbolTypeID();
	else if (strName == L"CURRENTDATE")
		pResult = m_VariantOperator.CallFunction(L"CURRENTDATE", VariantOperator::ARRAY_CALL_ARGUMENT(), NULL, pNode);
	else if (strName == L"CURRENTTIME")
		pResult = m_VariantOperator.CallFunction(L"CURRENTTIME", VariantOperator::ARRAY_CALL_ARGUMENT(), NULL, pNode);

	else if (strName == L"LDAY")
		pResult = m_VariantOperator.GetLunarCalendar(m_pHistoryData, 3);
	else if (strName == L"LMONTH")
		pResult = m_VariantOperator.GetLunarCalendar(m_pHistoryData, 2);
	else if (strName == L"LYEAR")
		pResult = m_VariantOperator.GetLunarCalendar(m_pHistoryData, 1);
	else if (strName == L"DRAWNULL" || strName==L"NULL")
		pResult = m_pHistoryData->GetDrawNULL();

	else if (strName == L"ISDOWN" || strName == L"ISEQUAL" || strName == L"ISUP" || strName==L"ISLASTBAR" || strName==L"BARSTATUS")
		pResult = m_pHistoryData->GetCustomValue(strName, pNode);

	else if (strName == L"MACHINEDATE" || strName == L"MACHINETIME" || strName == L"MACHINEWEEK")
		pResult = ReadMachineDateTime(strName);

	else if (strName==L"NONE")
		return  NULL;
	
	return pResult;
}

Variant* Execute::ReadMachineDateTime(const std::wstring& strName)
{
	Variant* pResult = m_VariantOperator.Create();
	std::time_t result = std::time(nullptr);
	auto* pDay = std::localtime(&result);
	if (strName == L"MACHINEDATE")	//ȡ�õ�ǰ�ͻ��˻�����1900�����ĵ�������
	{
		int nValue = ((pDay->tm_year + 1900) * 10000 + (pDay->tm_mon + 1) * 100 + pDay->tm_mday) - 19000000;
		pResult->SetDoubleValue(nValue);
	}
	else if (strName == L"MACHINETIME")	//ȡ�õ�ǰ�ͻ��˻�����ʱ��,����11:01:15ʱΪ110115
	{
		int nValue = pDay->tm_hour * 10000 + pDay->tm_min * 100 + pDay->tm_sec;
		pResult->SetDoubleValue(nValue);
	}
	else if (strName == L"MACHINEWEEK")	//ȡ�õ�ǰ�ͻ��˻���Ϊ���ڼ�(1,2,3,4,5,6,0)
	{
		pResult->SetDoubleValue(pDay->tm_wday);
	}
	return pResult;
}

Variant* Execute::GetDynaInfo(long ID, Node* pNode)
{
	Variant* pResult = NULL;

	IHistoryData::ARRAY_CALL_ARGUMENT args;
	Variant id;
	id.SetDoubleValue(ID);
	args.push_back(&id);

	pResult = m_pHistoryData->GetDynaInfo(args, pNode);
	return pResult;
}

Variant* Execute::GetDynaInfo(long lStartID, const Variant& n, Node* pNode)
{
	if (n.GetType() != Variant::DOUBLE_TYPE || !n.IsVaildDoulbe())
	{
		ExecuteExcept error(L"DYNAINFO(n)��������", L"Execute::GetDynaInfo", pNode);
		throw error;
	}

	long lID = lStartID + (long)n.GetDoubleValue();

	return GetDynaInfo(lID, pNode);
}

bool Execute::ToJson(std::wstring& strOut) const
{
	return true;
}


ExecuteExcept::ExecuteExcept()
{
	
}

ExecuteExcept::ExecuteExcept(const std::wstring& strMessage, const std::wstring& strFunctionInfo, Node* pNode)
	:m_strMessage(strMessage)
{
	std::wstringstream strDescription;
	strDescription << strMessage << L".";

	if (pNode)
	{
		SourceLocation* pLocation = pNode->GetLocation();
		if (pLocation && pLocation->_pStart)
		{
			strDescription << L"\r\nLine:" << pLocation->_pStart->GetLine() << L" Column:" << pLocation->_pStart->GetColumn() << L".";
			m_nLine = pLocation->_pStart->GetLine();
			m_nColumn = pLocation->_pStart->GetColumn();
		}
	}

	strDescription << L"\r\nCallStack:" << strFunctionInfo;

	m_strDescription = strDescription.str();

	m_strFunctionInfo = strFunctionInfo;
	m_strMessage = strMessage;
}

void ExecuteExcept::ToJson(std::wstring& strOut) const
{

}

ExecuteExcept::~ExecuteExcept()
{

}

OutVarInfo::OutVarInfo()
{

}

OutVarInfo::~OutVarInfo()
{

}

void OutVarInfo::SetName(const std::wstring& strName)
{
	m_strName = strName;
}

OutVarInfo::OutVarInfo(const std::wstring& strName)
	:m_strName(strName)
{

}

void OutVarInfo::AddAttribute(const std::wstring& strAttribute)
{
	m_aryAttributes.push_back(strAttribute);
}

void OutVarInfo::AddAttribute(const std::wstring& strKey, const std::wstring& strValue)
{
	m_mapAttributes[strKey] = strValue;
}

void OutVarInfo::SetOutValue(Variant* pVar)
{
	m_pVar = pVar;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
OutVariantAttribute::OutVariantAttribute()
{
	Initialize();
}

OutVariantAttribute::~OutVariantAttribute()
{

}

OutVariantAttribute& OutVariantAttribute::GetInstance()
{
	static OutVariantAttribute instance;
	return instance;
}

void OutVariantAttribute::Initialize()
{
	const wchar_t VAR_ATTRIBUTE[][16] =
	{
		L"POINTDOT", L"CIRCLEDOT",L"DOTLINE", L"LINESTICK",L"STICK",L"VOLSTICK",L"COLORSTICK",

		//�߶�
		L"LINETHICK", L"LINETHICK0", L"LINETHICK1", L"LINETHICK2", L"LINETHICK3", L"LINETHICK4", L"LINETHICK5", L"LINETHICK6", L"LINETHICK7", L"LINETHICK8",L"LINETHICK9",

		L"NODRAW",  //��������
		L"NOTEXT",	//����ʾָ�������Ϣ

		//��ʾ��λ��֮��,����DRAWTEXT��DRAWNUMBER�Ⱥ�������,�������������(������LINETHICK�Ⱥ�������),����: DRAWNUMBER(CLOSE > OPEN,HIGH,CLOSE),DRAWABOVE;
		L"DRAWABOVE",

		//��ɫ
		L"COLORBLACK",L"COLORBLUE",L"COLORGREEN",L"COLORCYAN",L"COLORRED",L"COLORMAGENTA",L"COLORBROWN",L"COLORLIGRAY",
		L"COLORGRAY",L"COLORLIBLUE",L"COLORLIGREEN",L"COLORLICYAN",L"COLORLIRED",L"COLORLIMAGENTA",L"COLORYELLOW",L"COLORWHITE",

		L"VALIGN0",L"VALIGN1",L"VALIGN2",
		L"ALIGN0",L"ALIGN1",L"ALIGN2",
	};

	for (int i = 0; i < ARRAYSIZE(VAR_ATTRIBUTE); ++i)
	{
		m_setAttributeKey.insert(VAR_ATTRIBUTE[i]);
	}

	const wchar_t DRAW_FUNCTION_NAME[][16] =
	{
		L"DRAWKLINE",L"STICKLINE",L"DRAWICON",L"DRAWTEXT",L"DRAWNUMBER",L"DRAWBAND",L"PLOYLINE",L"DRAWLINE",L"DRAWTEXT_FIX",L"DRAWNUMBER_FIX",
		L"EXPLAIN",L"FILLRGN",L"PARTLINE", L"DRAWCOLORKLINE",
	};

	for (int i = 0; i < ARRAYSIZE(DRAW_FUNCTION_NAME); ++i)
	{
		m_setDrawKey.insert(DRAW_FUNCTION_NAME[i]);
	}
}

bool OutVariantAttribute::IsAttributeKey(const std::wstring& strValue) const
{
	auto find = m_setAttributeKey.find(strValue);
	if (find != m_setAttributeKey.end()) return true;

	//��ʽΪCOLOR + ��BBGGRR����BB��GG��RR��ʾ��ɫ����ɫ�ͺ�ɫ�ķ�����ÿ����ɫ��ȡֵ��Χ��00 - FF��������16���ơ�
	//���磺MA5 : MA(CLOSE, 5)��COLOR00FFFF��ʾ����ɫ�봿��ɫ�Ļ��ɫ��COLOR808000��ʾ����ɫ�͵���ɫ�Ļ��ɫ��
	if (strValue.find(L"COLOR") == 0 && strValue.length() == 11) return true;
	if (strValue.find(L"FONTSIZE") == 0 && strValue.length() <= 10) return true;
	
	return false;
}

bool OutVariantAttribute::IsDrawKey(const std::wstring& strValue) const
{
	auto find = m_setDrawKey.find(strValue);
	if (find != m_setDrawKey.end()) return true;

	return false;
}

bool OutVariantAttribute::AddKey(const std::wstring& strKey)
{
	if (m_setAttributeKey.find(strKey) == m_setAttributeKey.end())
	{
		m_setAttributeKey.insert(strKey);
		return true;
	}
		
	return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}
}