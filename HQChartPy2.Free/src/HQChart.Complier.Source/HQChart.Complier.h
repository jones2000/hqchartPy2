/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	麦语法编译器 (c++)
*/

#pragma once
#include <string>
#include <vector>
#include <set>
#include <assert.h>
#include <exception>

#ifndef	DELETE_PTR
	#define	DELETE_PTR(p) if(p){delete p;p=NULL;}
#endif

namespace HQChart { namespace Complier {

namespace Messages
{

	//错误信息	
	static const wchar_t UnexpectedToken[] = L"Unexpected token %0";
	static const wchar_t UnexpectedNumber[] = L"Unexpected number";
	static const wchar_t UnexpectedString[] = L"Unexpected string";
	static const wchar_t UnexpectedIdentifier[] = L"Unexpected identifier";
	static const wchar_t UnexpectedReserved[] = L"Unexpected reserved word";
	static const wchar_t UnexpectedEOS[] = L"Unexpected end of input";
	static const wchar_t NewlineAfterThrow[] = L"Illegal newline after throw";
	// static const char *InvalidRegExp = "Invalid regular expression";
	static const wchar_t UnterminatedRegExp[] = L"Invalid regular expression: missing /";
	static const wchar_t InvalidLHSInAssignment[] = L"Invalid left-hand side in assignment";
	static const wchar_t InvalidLHSInForIn[] = L"Invalid left-hand side in for-in";
	static const wchar_t MultipleDefaultsInSwitch[] = L"More than one default clause in switch statement";
	static const wchar_t NoCatchOrFinally[] = L"Missing catch or finally after try";
	static const wchar_t UnknownLabel[] = L"Undefined label '%0'";
	static const wchar_t Redeclaration[] = L"%0 '%1' has already been declared";
	static const wchar_t IllegalContinue[] = L"Illegal continue statement";
	static const wchar_t IllegalBreak[] = L"Illegal break statement";
	static const wchar_t IllegalReturn[] = L"Illegal return statement";
	static const wchar_t StrictModeWith[] = L"Strict mode code may not include a with statement";
	static const wchar_t StrictCatchVariable[] = L"Catch variable may not be eval or arguments in strict mode";
	static const wchar_t StrictVarName[] = L"Variable name may not be eval or arguments in strict mode";
	static const wchar_t StrictParamName[] = L"Parameter name eval or arguments is not allowed in strict mode";
	static const wchar_t StrictParamDupe[] = L"Strict mode function may not have duplicate parameter names";
	static const wchar_t StrictFunctionName[] = L"Function name may not be eval or arguments in strict mode";
	static const wchar_t StrictOctalLiteral[] = L"Octal literals are not allowed in strict mode.";
	static const wchar_t StrictDelete[] = L"Delete of an unqualified identifier in strict mode.";
	static const wchar_t StrictDuplicateProperty[] = L"Duplicate data property in object literal not allowed in strict mode";
	static const wchar_t AccessorDataProperty[] = L"Object literal may not have data and accessor property with the same name";
	static const wchar_t AccessorGetSet[] = L"Object literal may not have multiple get/set accessors with the same name";
	static const wchar_t StrictLHSAssignment[] = L"Assignment to eval or arguments is not allowed in strict mode";
	static const wchar_t StrictLHSPostfix[] = L"Postfix increment/decrement may not have eval or arguments operand in strict mode";
	static const wchar_t StrictLHSPrefix[] = L"Prefix increment/decrement may not have eval or arguments operand in strict mode";
	static const wchar_t StrictReservedWord[] = L"Use of future reserved word in strict mode";
}


namespace PropertyKind 
{
	enum 
	{
		Data = 1,
		Get = 2,
		Set = 4
	};
}

namespace Syntax
{
	enum
	{
		NONE_ID=0,
		ASSIGNMENTEXPRESSION_ID,
		EXPRESSIONSTATEMENT_ID,
		IDENTIFIER_ID,
		NUMERICLITERAL_ID,
		CALLEXPRESSION_ID,
		LOGICALEXPRESSION_ID,
		BINARYEXPRESSION_ID,
		LITERAL_ID,
		UNARYEXPRESSION_ID,
		SEQUENCEEXPRESSION_ID,
		STRINGLITERAL_ID,
		MEMBEREXPRESSION_ID,	//变量集合
	};
}


class Character
{
public:
	static bool IsDecimalDigit(wchar_t ch);
	static bool IsHexDigit(wchar_t ch);
	static bool IsOctalDigit(wchar_t ch);
	static bool IsWhiteSpace(wchar_t ch);
	static bool IsLineTerminator(wchar_t ch);
	static bool IsIdentifierStart(wchar_t ch);
	static bool IsIdentifierPart(wchar_t ch);
	static bool IsRestrictedWord(const std::wstring &id);
	static bool IsFutureReservedWord(const std::wstring &id);
	static bool IsStrictModeReservedWord(const std::wstring &id);

	static std::wstring Format(std::wstring format, const std::wstring &arg0);
	static std::wstring Format(std::wstring format, const std::wstring &arg0, const std::wstring &arg1);
};


class Pool;
class Parser;
class Visitor;
class Program;

class Poolable
{
public:
	Poolable(Pool &pool);
	virtual ~Poolable();

	Poolable* GetPoolable() { return m_pPoolable; }
protected:
	Poolable *m_pPoolable;
};

class Pool 
{
public:
	Pool();
	~Pool();

	inline Poolable* GetFirst() const { return m_pFirst; }
	inline void SetFirst(Poolable* first) { m_pFirst = first; }

private:
	Poolable *m_pFirst=NULL;
};


class Position : public Poolable 
{
public:
	Position(Pool &pool);

	int GetLine() const { return m_nLine; }
	int GetColumn() const { return m_nColumn; }
	void SetLine(int value) { m_nLine = value; }
	void SetColumn(int value) { m_nColumn = value; }

protected:
	int m_nLine=0;
	int m_nColumn=0;
	
};

class SourceLocation : public Poolable
{
public:
	SourceLocation(Pool &pool) : Poolable(pool) { }
	virtual ~SourceLocation() { }

	Position* _pStart = NULL;
	Position* _pEnd = NULL;
};

/*
typedef struct tagSourceLocation
{
	Position* _pStart;
	Position* _pEnd;
}SOURCE_LOCATION, *PSOURCE_LOCATION;
*/

class Program;
class Identifier;
class BlockStatement;
class EmptyStatement;
class ExpressionStatement;
class LabeledStatement;
class ArrayExpression;
class FunctionExpression;
class SequenceExpression;
class UnaryExpression;
class BinaryExpression;
class AssignmentExpression;
class UpdateExpression;
class LogicalExpression;
class ConditionalExpression;
class NewExpression;
class CallExpression;
class MemberExpression;
class NullLiteral;
class StringLiteral;
class NumericLiteral;
class BooleanLiteral;

class Variant;
class VariantOperator;

class Visitor
{
public:
	virtual void Visit(Program *node) = 0;
	virtual void Visit(Identifier *node) = 0;
	virtual void Visit(BlockStatement *node) = 0;
	virtual void Visit(EmptyStatement *node) = 0;
	virtual void Visit(ExpressionStatement *node) = 0;
	//virtual void Visit(IfStatement *node) = 0;
	virtual void Visit(LabeledStatement *node) = 0;
	virtual void Visit(ArrayExpression *node) = 0;
	//virtual void visit(Property *node) = 0;
	//virtual void visit(ObjectExpression *node) = 0;
	virtual void Visit(FunctionExpression *node) = 0;
	virtual void Visit(SequenceExpression *node) = 0;
	virtual void Visit(UnaryExpression *node) = 0;
	virtual void Visit(BinaryExpression *node) = 0;
	virtual void Visit(AssignmentExpression *node) = 0;
	virtual void Visit(UpdateExpression *node) = 0;
	virtual void Visit(LogicalExpression *node) = 0;
	virtual void Visit(ConditionalExpression *node) = 0;
	virtual void Visit(NewExpression *node) = 0;
	virtual void Visit(CallExpression *node) = 0;
	virtual void Visit(MemberExpression *node) = 0;
	virtual void Visit(NullLiteral *node) = 0;
	//virtual void Visit(RegExpLiteral *node) = 0;
	virtual void Visit(StringLiteral *node) = 0;
	virtual void Visit(NumericLiteral *node) = 0;
	virtual void Visit(BooleanLiteral *node) = 0;
};

class Token : public Poolable 
{
public:
	Token(Pool &pool);

	enum
	{
		BooleanLiteral = 1,
		Eof = 2,
		Identifier = 3,
		Keyword = 4,
		NullLiteral = 5,
		NumericLiteral = 6,
		Punctuator = 7,
		StringLiteral = 8
	};

	void SetType(int nValue) { m_nType = nValue; }
	void SetLineNumber(int nValue) { m_nLineNumber = nValue; }
	void SetLineStart(int nValue) { m_nLineStart = nValue; }
	void SetRange(int nValue, int nValue2) { m_nRange[0] = nValue; m_nRange[1] = nValue2; }
	void SetStringValue(const std::wstring& strValue) { m_strValue = strValue; }
	void SetOctal(bool bValue) { m_bOctal = bValue; }
	void SetDoubleValue(double dValue) { m_dValue = dValue; }

	int GetType() const { return m_nType; }
	const std::wstring& GetStringValue() const { return m_strValue; }
	int GetRange(int nIndex) const { return m_nRange[nIndex]; }
	int GetLineNumber() const { return m_nLineNumber; }
	int GetLineStart() const { return m_nLineStart; }
	double GetDoubleValue() const { return m_dValue; }
	bool IsOctal() const { return m_bOctal; }

protected:
	int m_nType=0;

	std::wstring	m_strValue;
	double			m_dValue=0;
	bool			m_bOctal=false;

	int m_nLineNumber=0;
	int m_nLineStart=0;
	int m_nRange[2];
};


class Node : public Poolable 
{
public:
	Node(Pool &pool);
	virtual ~Node() {}
	virtual void Accept(Visitor *visitor) = 0;

	virtual Variant* GetVariant(VariantOperator* pVariantOperator) const { return NULL; }

	template <class T> bool Is()
	{
		return dynamic_cast<T *>(this) != NULL;
	}

	template <class T> T *As()
	{
		T *t = dynamic_cast<T *>(this);
		assert(t != NULL);
		return t;
	}

	void SetRange(int value, int value2) { m_nRange[0] = value; m_nRange[1] = value2; }
	void SetGroupRange(int value, int value2) { m_nGroupRange[0] = value; m_nGroupRange[1] = value2; }

	SourceLocation* GetLocation() const { return m_pLoc; }
	SourceLocation* GetGroupLocation() const { return m_pGroupLoc; }
	int GetRange(int nIndex) { return m_nRange[nIndex]; }
	int GetGroupRange(int nIndex) { return m_nGroupRange[nIndex]; }

	void SetLocation(SourceLocation* pValue) { m_pLoc = pValue; }
	void SetGroupLocation(SourceLocation* pValue) { m_pGroupLoc = pValue; }

	void SetVariantOut(Variant* pVaraint) { m_pVariantOut = pVaraint; }
	Variant* GetVariantOut() const { return m_pVariantOut; }

	int GetType() { return m_nType; }
protected:
	int m_nType = Syntax::NONE_ID;

	SourceLocation *m_pLoc = NULL;
	SourceLocation *m_pGroupLoc = NULL;
	int m_nRange[2];
	int m_nGroupRange[2];

	Variant* m_pVariantOut;
};

class Statement : public Node
{
public:
	Statement(Pool &pool) : Node(pool) { }
};

class Expression : public Node 
{
public:
	Expression(Pool &pool) : Node(pool) {}
};

class Program : public Node 
{
public:
	Program(Pool &pool) : Node(pool) {}
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetBody(const std::vector<Statement *> body) { m_Body = body; }
	const std::vector<Statement *>& GetBody() const { return m_Body; }
protected:
	std::vector<Statement *> m_Body;
};

class Identifier : public Expression 
{
public:
	Identifier(Pool &pool) : Expression(pool) { m_nType = Syntax::IDENTIFIER_ID; }
	void Accept(Visitor *visitor) { visitor->Visit(this); }
	
	const std::wstring& GetName() const { return m_strName; }
	void SetName(const std::wstring& strName) { m_strName = strName; };
protected:
	std::wstring m_strName;
};

// This is a mixin, not a base class, and so isn't poolable
class Function
{
public:
	Function() { }

	void SetID(Identifier* pID) { m_pID = pID; }
	void SetParams(const std::vector<Identifier *>& params) { m_Params = params; }
	void SetBody(BlockStatement* body) { m_pBody = body; }

protected:
	Identifier* m_pID = NULL;;
	std::vector<Identifier *> m_Params;
	BlockStatement* m_pBody = NULL;
};

class EmptyStatement : public Statement
{
public:
	EmptyStatement(Pool &pool) : Statement(pool) {}
	void Accept(Visitor *visitor) { visitor->Visit(this); }
};

class LabeledStatement : public Statement 
{
public:
	LabeledStatement(Pool &pool) : Statement(pool) {}
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetLabel(Identifier* pLabel) { m_pLabel = pLabel; }
	void SetBody(Statement* pBody) { m_pBody = pBody; }

protected:
	Identifier* m_pLabel = NULL;
	Statement* m_pBody = NULL;
};

class ExpressionStatement : public Statement 
{
public:
	ExpressionStatement(Pool &pool) : Statement(pool) { m_nType = Syntax::EXPRESSIONSTATEMENT_ID; }
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	Expression* GetExpression() { return m_pExpression; }
	void SetExpression(Expression* pExpression) { m_pExpression = pExpression; }
protected:
	Expression* m_pExpression = NULL;
};

class BlockStatement : public Statement 
{
public:
	BlockStatement(Pool &pool) : Statement(pool) {}
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetBody(const std::vector<Statement *>& body) { m_Body = body; }

protected:
	std::vector<Statement *> m_Body;
};


class MemberExpression : public Expression
{
public:
	MemberExpression(Pool &pool) : Expression(pool) { m_nType = Syntax::MEMBEREXPRESSION_ID; }
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetComputed(bool bValue) { m_bComputed = bValue; }
	void SetObject(Expression* pValue) { m_pObject = pValue; }
	void SetProperty(Expression* pValue) { m_pProperty = pValue; }

	Expression* GetRObject() const { return m_pObject; }
	Expression* GetProperty() const { return m_pProperty; }
protected:
	Expression* m_pObject=NULL;
	Expression* m_pProperty=NULL; // Identifier if computed == false
	bool m_bComputed=false;
};

class NewExpression : public Expression
{
public:
	
	NewExpression(Pool &pool) : Expression(pool) {}
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetCallee(Expression* pCallee) { m_pCallee = pCallee; }
	void SetArguments(const std::vector<Expression *>& arguments) { m_Arguments = arguments; }

protected:
	Expression* m_pCallee = NULL;
	std::vector<Expression *> m_Arguments;
};

class CallExpression : public Expression 
{
public:
	typedef std::vector<Expression *> ARRAY_ARGUMENT;

	CallExpression(Pool &pool) : Expression(pool) { m_nType = Syntax::CALLEXPRESSION_ID; }
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetCallee(Expression* pValue) { m_pCallee = pValue; }
	void SetArguments(const ARRAY_ARGUMENT& args) { m_Arguments = args; }

	std::wstring GetCalleeName() const;
	const ARRAY_ARGUMENT& GetArguments() const { return m_Arguments; }

protected:
	Expression *m_pCallee = NULL;
	ARRAY_ARGUMENT m_Arguments;
};

class UnaryExpression : public Expression 
{
public:
	UnaryExpression(Pool &pool) : Expression(pool) { m_nType = Syntax::UNARYEXPRESSION_ID; }
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetOperator(const std::wstring strValue) { m_strOperator = strValue; }
	void SetArgument(Expression* pArgument) { m_pArgument = pArgument; }
	void SetPrefix(bool bValue) { m_bPrefix = bValue; }

	const std::wstring& GetOperator() const { return m_strOperator; }
	Expression* GetArgument() const { return m_pArgument; }


protected:
	std::wstring m_strOperator;
	bool m_bPrefix=false;
	Expression* m_pArgument = NULL;
};


class BinaryExpression : public Expression 
{
public:
	BinaryExpression(Pool &pool) : Expression(pool) { m_nType = Syntax::BINARYEXPRESSION_ID; }
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetOperator(const std::wstring& strValue) { m_strOperator = strValue; }
	void SetLeft(Expression* pValue) { m_pLeft = pValue; }
	void SetRight(Expression* pValue) { m_pRight = pValue; }

	Expression* GetLeft() const { return m_pLeft; }
	Expression* GetRight() const { return m_pRight; }
	const std::wstring& GetOperator() const { return m_strOperator; }

protected:
	std::wstring m_strOperator;
	Expression* m_pLeft = NULL;
	Expression* m_pRight = NULL;
};


class AssignmentExpression : public Expression 
{
public:
	AssignmentExpression(Pool &pool) : Expression(pool) { m_nType = Syntax::ASSIGNMENTEXPRESSION_ID; }
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetOperator(const std::wstring& strValue) { m_strOperator = strValue; }
	void SetLeft(Expression* pValue) { m_pLeft = pValue; }
	void SetRight(Expression* pValue) { m_pRight = pValue; }

	Expression* GetLeft() const { return m_pLeft; }
	Expression* GetRight() const { return m_pRight; }
	const std::wstring& GetOperator() const{ return m_strOperator; }

protected:
	std::wstring m_strOperator;
	Expression* m_pLeft = NULL;
	Expression* m_pRight = NULL;
};

class LogicalExpression : public Expression 
{
public:
	LogicalExpression(Pool &pool) : Expression(pool) { m_nType = Syntax::LOGICALEXPRESSION_ID; }
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetOperator(const std::wstring& strValue) { m_strOperator = strValue; }
	void SetLeft(Expression* pValue) { m_pLeft = pValue; }
	void SetRight(Expression* pValue) { m_pRight = pValue; }

	Expression* GetLeft() const { return m_pLeft; }
	Expression* GetRight() const { return m_pRight; }
	const std::wstring& GetOperator() const { return m_strOperator; }

protected:
	std::wstring m_strOperator;
	Expression* m_pLeft = NULL;
	Expression* m_pRight = NULL;
};

class ArrayExpression : public Expression
{
public:
	ArrayExpression(Pool &pool) : Expression(pool) {}
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetElements(const std::vector<Expression *>& element) { m_Elements = element; }

protected:
	std::vector<Expression *> m_Elements;
};

class FunctionExpression : public Expression, public Function
{
public:
	FunctionExpression(Pool &pool) : Expression(pool) {}
	void Accept(Visitor *visitor) { visitor->Visit(this); }
};

class SequenceExpression : public Expression 
{
public:
	SequenceExpression(Pool& pool) : Expression(pool) { m_nType = Syntax::SEQUENCEEXPRESSION_ID; }
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetExpressions(const std::vector<Expression *>& expressions) { m_Expressions = expressions; }
	const std::vector<Expression *>& GetExpressions() { return m_Expressions; }

protected:
	std::vector<Expression *> m_Expressions;
};

class UpdateExpression : public Expression 
{
public:
	UpdateExpression(Pool &pool) : Expression(pool) {}
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetOperator(const std::wstring strValue) { m_strOperator = strValue; }
	void SetArgument(Expression* pArgument) { m_pArgument = pArgument; }
	void SetPrefix(bool bValue) { m_bPrefix = bValue; }

protected:
	std::wstring m_strOperator;
	Expression* m_pArgument=NULL;
	bool m_bPrefix = false;;
};

class ConditionalExpression : public Expression 
{
public:
	ConditionalExpression(Pool &pool) : Expression(pool) {}
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetTest(Expression* pTest) { m_pTest = pTest; }
	void SetAlternate(Expression* pAlternate) { m_pAlternate = pAlternate; }
	void SetConsequent(Expression* pConsequent) { m_pConsequent = pConsequent; }
protected:
	Expression* m_pTest = NULL;
	Expression* m_pAlternate = NULL;
	Expression* m_pConsequent = NULL;
};

class Literal : public Expression 
{
public:
	Literal(Pool &pool) : Expression(pool) {}
};

class StringLiteral : public Literal 
{
public:
	StringLiteral(Pool &pool) : Literal(pool) { m_nType = Syntax::STRINGLITERAL_ID; }
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetValue(const std::wstring& strValue) { m_strValue = strValue; }
	const std::wstring& GetValue() const { return m_strValue; }
protected:
	std::wstring m_strValue;
};

class NumericLiteral : public Literal 
{
public:
	NumericLiteral(Pool &pool) : Literal(pool) { m_nType = Syntax::NUMERICLITERAL_ID; }
	
	virtual Variant* GetVariant(VariantOperator* pVariantOperator) const;

	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetValue(double dValue) { m_dValue = dValue; }
	double GetValue() const { return m_dValue; }
protected:
	double m_dValue = 0;
};

class BooleanLiteral : public Literal 
{
public:
	BooleanLiteral(Pool &pool) : Literal(pool) {}
	void Accept(Visitor *visitor) { visitor->Visit(this); }

	void SetValue(bool bValue) { m_bValue = bValue; }
	bool GetValue() const { return m_bValue; }
protected:
	bool m_bValue=false;
};

class NullLiteral : public Literal 
{
public:
	NullLiteral(Pool &pool) : Literal(pool) {}
	void Accept(Visitor *visitor) { visitor->Visit(this); }
};


class SyntaxTreeDelegate
{
public:
	SyntaxTreeDelegate(Parser &parser);
	void PostProcess(Node *node);

	ArrayExpression* CreateArrayExpression(const std::vector<Expression*> &elements);
	AssignmentExpression* CreateAssignmentExpression(const std::wstring &strOperator, Expression* pLeft, Expression* pRight);
	Expression* CreateBinaryExpression(const std::wstring &strOperator, Expression* pLeft, Expression* pRight);
	BlockStatement* CreateBlockStatement(const std::vector<Statement *> &body);
	CallExpression* CreateCallExpression(Expression *pCallee, const std::vector<Expression *> &args);
	ExpressionStatement* CreateExpressionStatement(Expression *pExpression);
	Identifier* CreateIdentifier(const std::wstring &name);
	MemberExpression* CreateMemberExpression(int nAccessor, Expression *pObject, Expression *pProperty);
	EmptyStatement* CreateEmptyStatement();
	Literal *CreateLiteral(Token* pToken);
	SequenceExpression* CreateSequenceExpression(const std::vector<Expression *> &expressions);
	Expression* CreateUnaryExpression(const std::wstring &strOperator, Expression *argument);
	ConditionalExpression* CreateConditionalExpression(Expression* pTest, Expression* pConsequent, Expression* pAlternate);
	LabeledStatement* CreateLabeledStatement(Identifier* pLabel, Statement* pBody);
	UpdateExpression* CreatePostfixExpression(const std::wstring& strOperator, Expression* pArgument);
	FunctionExpression* CreateFunctionExpression(Identifier* pId, const std::vector<Identifier *> &params, BlockStatement * pBody);
	NewExpression* CreateNewExpression(Expression* pCallee, const std::vector<Expression *> &args);

	Program* CreateProgram(const std::vector<Statement *> &body);

private:
	Parser &m_Parser;
};

class State 
{
public:
	State() {}

	bool IsAllowIn() const { return m_bAllowIn; }
	void SetAllowIn(bool bValue) { m_bAllowIn = bValue; }

	std::set<std::wstring>& GetLabel() { return m_LabelSet; }
	const std::set<std::wstring>& GetLabel() const { return m_LabelSet; }

	bool IsInIteration() const { return m_bInIteration; }
	bool IsInSwitch() const { return m_bInSwitch; }
	bool IsInFunctionBody() const { return m_bInFunctionBody; }

	void SetLabelSet(const std::set<std::wstring>& labelSet) { m_LabelSet = labelSet; }
	void SetInIteration(bool bValue) { m_bInFunctionBody = bValue; }
	void SetInSwitch(bool bValue) { m_bInSwitch = bValue; }
	void SetInFunctionBody(bool bValue) { m_bInFunctionBody = bValue; }

private:
	bool m_bInFunctionBody = false;
	bool m_bInIteration = false;
	bool m_bInSwitch = false;
	bool m_bAllowIn = false;
	std::set<std::wstring> m_LabelSet;
};

class Marker 
{
public:
	Marker(Parser &parser);
	~Marker();

	void End();
	void ApplyGroup(Node *node);
	void Apply(Node *node);
	
	void SetLocation(SourceLocation* pLoc) { m_pLoc = pLoc; }

private:
	Parser &m_Parser;
	int m_nRange[2];
	SourceLocation *m_pLoc = NULL;
};

class WrapTrackingFunction 
{
public:
	WrapTrackingFunction(Parser &parser);
	~WrapTrackingFunction();

	template <typename T> T *Close(T* pNode);

private:
	Parser &m_Parser;
	Marker *m_pMarker=NULL;
};


class ParseExcept : public std::exception
{
public:
	ParseExcept(const std::wstring& strMessage);
	virtual ~ParseExcept();

	void SetIndex(int nValue) { m_nIndex = nValue; }
	void SetLineNumber(int nValue) { m_nLineNumber = nValue; }
	void SetColumn(int nValue) { m_nColumn = nValue; }
	void SetDescription(const std::wstring& strValue) { m_strDescription = strValue; }

	const std::wstring& GetDescription() const { return m_strDescription; }

protected:
	std::wstring m_strDescription;
	std::wstring m_strMessage;
	int			m_nIndex = 0;
	int			m_nLineNumber = 0;
	int			m_nColumn = 0;
};


class Parser
{
public:
	Parser(Pool &pool);


	Program* Parse(const std::wstring &code);

	void SkipComment();
	Marker *CreateLocationMarker();
	void VisitBinary(Node* pNode);

	inline int GetIndex() const { return m_nIndex; }
	inline int GetLineNumber() const { return m_nLineNumber; }
	inline int GetLineStart() const { return m_nLineStart; }
	inline Pool& GetPool() const { return m_Pool; }

	SyntaxTreeDelegate& GetDelegate() { return m_Delegate; }

private:

	Program *ParseProgram();
	void Peek();
	Token* Advance();
	bool IsIdentifierName(Token* pToken);
	Token* Lex();

	Token* ScanPunctuator();
	Token* ScanStringLiteral();
	Token* ScanIdentifier();
	Token* scanNumericLiteral();
	Token* ScanNumericLiteral();
	Token* ScanHexLiteral(int nStart);
	Token* ScanOctalLiteral(int nStart);

	
	std::vector<Statement*> ParseSourceElements();
	Statement* ParseSourceElement();
	std::wstring GetEscapedIdentifier();
	std::wstring GetIdentifier();
	Statement* ParseStatement();
	void ConsumeSemicolon();

	EmptyStatement* ParseEmptyStatement();
	BlockStatement* ParseBlock();
	ExpressionStatement* ParseExpressionStatement();
	Expression* ParseExpression();
	Expression* ParseAssignmentExpression();
	Expression* ParseConditionalExpression();
	Expression* ParseBinaryExpression();
	Expression* ParseUnaryExpression();
	Expression* ParsePostfixExpression();
	Expression* ParseLeftHandSideExpression();
	Expression* ParseLeftHandSideExpressionAllowCall();
	Identifier* ParseNonComputedMember();
	Expression* ParseComputedMember();
	NewExpression* ParseNewExpression();
	Expression* ParsePrimaryExpression();
	Identifier* ParseNonComputedProperty();
	FunctionExpression* ParseFunctionExpression();
	ArrayExpression* ParseArrayInitialiser();
	Expression* ParseGroupExpression();
	BlockStatement* ParseFunctionSourceElements();
	std::vector<Statement *> ParseStatementList();
	std::vector<Expression*> ParseArguments();
	FunctionExpression *ParsePropertyFunction(const std::vector<Identifier *> &param, Token* pFirst);

	Expression* TrackLeftHandSideExpression();
	Expression* TrackLeftHandSideExpressionAllowCall();
	Expression* TrackGroupExpression();

	void Visit(Expression* pNode, Expression* pLeft, Expression* pRight);

	int BinaryPrecedence(Token* pToken, bool allowIn);

	void Expect(const std::wstring &value);
	void ExpectKeyword(const std::wstring &keyword);
	bool Match(const std::wstring &value);
	bool MatchAssign();
	bool MatchKeyword(const std::wstring &keyword);
	bool IsLeftHandSide(Node* pExpr);
	bool PeekLineTerminator();

	Token* CreateToken(int nType, const std::wstring& strValue, int nStart);
	Token* CreateToken(int nType, double dValue, int nStart);

	void ThrowUnexpected(Token *pToken);
	void ThrowError(Token *token, const std::wstring &msg);

	Pool &m_Pool;
	std::wstring m_strSource;
	bool m_bStrict=false;
	int m_nIndex=0;
	int m_nLineNumber=0;
	int m_nLineStart=0;
	int m_nLength=0;
	SyntaxTreeDelegate m_Delegate;
	Token *m_pLookahead=NULL;
	State m_State;
};

template <typename T> T *WrapTrackingFunction::Close(T* pNode)
{
	if (pNode->GetLocation()==NULL)
	{
		m_pMarker->Apply(pNode);
	}

	m_Parser.VisitBinary(pNode);

	return pNode;
}

Program* Parse(Pool &pool, const std::wstring &code);

}
}