/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	麦语法编译器 (c++)
*/

#include "HQChart.Complier.h"
#include "HQChart.data.h"
#include "HQChart.Log.h"
#include <string>
#include <assert.h>
#include <sstream>


namespace HQChart { namespace Complier {


bool Character::IsDecimalDigit(wchar_t ch)
{
	return (ch >= 48 && ch <= 57);   // 0..9
}

bool Character::IsHexDigit(wchar_t ch)
{
	return std::wstring(L"0123456789abcdefABCDEF").find(ch) != std::wstring::npos;
}

bool Character::IsOctalDigit(wchar_t ch)
{
	return std::wstring(L"01234567").find(ch) != std::wstring::npos;
}

bool Character::IsWhiteSpace(wchar_t ch)
{
	return (ch == 32) ||  // space
		(ch == 9) ||      // tab
		(ch == 0xB) ||
		(ch == 0xC) ||
		(ch == 0xA0);
}

bool Character::IsLineTerminator(wchar_t ch)
{
	return (ch == 10) || (ch == 13) || (ch == 0x2028) || (ch == 0x2029);
}

bool Character::IsIdentifierStart(wchar_t ch)
{
	return (ch == 36) || (ch == 95) ||  // $ (dollar) and _ (underscore)
		(ch >= 65 && ch <= 90) ||         // A..Z
		(ch >= 97 && ch <= 122) ||        // a..z
		(ch>=0x4E00 && ch <= 0x9FA5) ||	  //中文汉字
		(ch>=0xFF21 && ch<= 0xFF3A) ||	  //全角大写英文字母
		(ch>=0xFF10 && ch<= 0xFF19) ||	  //全角阿拉伯数字

		(ch >= 0xFF1A && ch <= 0xFF20) ||	  //全角字符
		(ch >= 0x25A0 && ch <= 0x3017) ||	  //部分全角字符
		(ch >= 0xFF01 && ch <= 0xFF0F) ||	  //部分全角字符

		(ch=='@') ||
		(ch == 92);                      // \ (backslash)
}

bool Character::IsIdentifierPart(wchar_t ch)
{
	return (ch == 36) || (ch == 95) || (ch == 35) || // $ _  #
		(ch >= 65 && ch <= 90) ||         // A..Z
		(ch >= 97 && ch <= 122) ||        // a..z
		(ch >= 48 && ch <= 57) ||         // 0..9
		(ch >= 0x4E00 && ch <= 0x9FA5) ||	  //中文汉字
		(ch >= 0xFF21 && ch <= 0xFF3A) ||	  //全角大写英文字母
		(ch >= 0xFF10 && ch <= 0xFF19) ||	  //全角阿拉伯数字

		(ch >= 0xFF1A && ch <= 0xFF20) ||	  //部分全角字符
		(ch >= 0x25A0 && ch <= 0x3017) ||	  //部分全角字符
		(ch >= 0xFF01 && ch <= 0xFF0F) ||	  //部分全角字符

		(ch == '@') ||
		(ch == 92);                      // \ (backslash)
}

bool Character::IsRestrictedWord(const std::wstring &id) 
{
	return id == L"eval" || id == L"arguments";
}

bool Character::IsFutureReservedWord(const std::wstring &id) 
{
	return id == L"class" || id == L"enum" || id == L"export" || id == L"extends" || id == L"import" || id == L"super";
}

bool Character::IsStrictModeReservedWord(const std::wstring &id) 
{
	return id == L"implements" || id == L"interface" || id == L"package" ||
		id == L"private" || id == L"protected" || id == L"public" || id == L"static" || id == L"yield" || id == L"let";
}


std::wstring Character::Format(std::wstring format, const std::wstring &arg0) 
{
	size_t index = format.find(L"%0");
	assert(index != std::wstring::npos);
	format.replace(index, 2, arg0);
	return format;
}

std::wstring Character::Format(std::wstring format, const std::wstring &arg0, const std::wstring &arg1)
{
	size_t index = format.find(L"%0");
	assert(index != std::wstring::npos);
	format.replace(index, 2, arg0);
	index = format.find(L"%1");
	assert(index != std::wstring::npos);
	format.replace(index, 2, arg1);
	return format;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Poolable::Poolable(Pool &pool)
	:m_pPoolable(pool.GetFirst())
{
	pool.SetFirst(this);
}

Poolable::~Poolable()
{
	
}


Pool::Pool()
{

}

Pool::~Pool()
{
	while (m_pFirst)
	{
		Poolable *pPoolable = m_pFirst;
		m_pFirst = m_pFirst->GetPoolable();
		DELETE_PTR(pPoolable);
	}
}


Position::Position(Pool &pool) 
	: Poolable(pool)
{

}



Node::Node(Pool &pool) 
	: Poolable(pool)
{ 
	m_nRange[0] = m_nRange[1] = m_nGroupRange[0] = m_nGroupRange[1] = 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Token
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Token::Token(Pool &pool)
	: Poolable(pool)
{ 
	m_nRange[0] = m_nRange[1] = 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SyntaxTreeDelegate
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
SyntaxTreeDelegate::SyntaxTreeDelegate(Parser &parser)
	: m_Parser(parser)
{

}


void SyntaxTreeDelegate::PostProcess(Node *node)
{

}

ArrayExpression* SyntaxTreeDelegate::CreateArrayExpression(const std::vector<Expression *> &elements) 
{
	ArrayExpression* pNode = new ArrayExpression(m_Parser.GetPool());
	pNode->SetElements(elements);
	return pNode;
}

AssignmentExpression* SyntaxTreeDelegate::CreateAssignmentExpression(const std::wstring &strOperator, Expression* pLeft, Expression* pRight)
{
	AssignmentExpression* pNode = new AssignmentExpression(m_Parser.GetPool());
	pNode->SetOperator(strOperator);
	pNode->SetLeft(pLeft);
	pNode->SetRight(pRight);
	return pNode;
}

Expression* SyntaxTreeDelegate::CreateBinaryExpression(const std::wstring &strOperator, Expression* pLeft, Expression* pRight)
{
	if (strOperator == L"||" || strOperator == L"&&" || strOperator == L"OR" || strOperator == L"AND")
	{
		LogicalExpression* pNode = new LogicalExpression(m_Parser.GetPool());
		pNode->SetOperator(strOperator);
		pNode->SetLeft(pLeft);
		pNode->SetRight(pRight);
		return pNode;
	}
	else 
	{
		BinaryExpression* pNode = new BinaryExpression(m_Parser.GetPool());
		pNode->SetOperator(strOperator);
		pNode->SetLeft(pLeft);
		pNode->SetRight(pRight);
		return pNode;
	}
}

BlockStatement* SyntaxTreeDelegate::CreateBlockStatement(const std::vector<Statement *> &body) 
{
	BlockStatement* pNode = new BlockStatement(m_Parser.GetPool());
	pNode->SetBody(body);
	return pNode;
}

CallExpression* SyntaxTreeDelegate::CreateCallExpression(Expression *pCallee, const std::vector<Expression *> &args) 
{
	CallExpression* pNode = new CallExpression(m_Parser.GetPool());
	pNode->SetCallee(pCallee);
	pNode->SetArguments(args);
	return pNode;
}

ExpressionStatement* SyntaxTreeDelegate::CreateExpressionStatement(Expression *pExpression)
{
	ExpressionStatement* pNode = new ExpressionStatement(m_Parser.GetPool());
	pNode->SetExpression(pExpression);
	return pNode;
}

Identifier *SyntaxTreeDelegate::CreateIdentifier(const std::wstring &strName) 
{
	Identifier* pNode = new Identifier(m_Parser.GetPool());
	pNode->SetName(strName);
	return pNode;
}

MemberExpression* SyntaxTreeDelegate::CreateMemberExpression(int nAccessor, Expression *pObject, Expression *pProperty)
{
	MemberExpression* pNode = new MemberExpression(m_Parser.GetPool());
	pNode->SetComputed(nAccessor == '[');
	pNode->SetObject(pObject);
	pNode->SetProperty(pProperty);
	return pNode;
}

EmptyStatement* SyntaxTreeDelegate::CreateEmptyStatement() 
{
	EmptyStatement* pNode = new EmptyStatement(m_Parser.GetPool());
	return pNode;
}

Literal *SyntaxTreeDelegate::CreateLiteral(Token* pToken) 
{
	int nType = pToken->GetType();
	if (nType == Token::NumericLiteral) 
	{
		NumericLiteral* pNode = new NumericLiteral(m_Parser.GetPool());
		pNode->SetValue(pToken->GetDoubleValue());
		return pNode;
	}
	else if (nType == Token::NullLiteral) 
	{
		NullLiteral* pNode = new NullLiteral(m_Parser.GetPool());
		return pNode;
	}
	else if (nType == Token::StringLiteral) 
	{
		StringLiteral* pNode = new StringLiteral(m_Parser.GetPool());
		pNode->SetValue(pToken->GetStringValue());
		return pNode;
	}
	else if (nType == Token::BooleanLiteral)
	{
		BooleanLiteral* pNode = new BooleanLiteral(m_Parser.GetPool());
		pNode->SetValue(pToken->GetStringValue() == L"true");
		return pNode;
	}
	else 
	{
		assert(false);
		return NULL;
	}
}

Expression* SyntaxTreeDelegate::CreateUnaryExpression(const std::wstring &strOperator, Expression* pArgument) 
{
	if (strOperator == L"++" || strOperator == L"--")
	{
		UpdateExpression* pNode = new UpdateExpression(m_Parser.GetPool());
		pNode->SetOperator(strOperator);
		pNode->SetArgument(pArgument);
		pNode->SetPrefix(true);
		return pNode;
	}
	else 
	{
		UnaryExpression* pNode = new UnaryExpression(m_Parser.GetPool());
		pNode->SetOperator(strOperator);
		pNode->SetArgument(pArgument);
		pNode->SetPrefix(true);
		return pNode;
	}
}

SequenceExpression* SyntaxTreeDelegate::CreateSequenceExpression(const std::vector<Expression *> &expressions) 
{
	SequenceExpression* pNode = new SequenceExpression(m_Parser.GetPool());
	pNode->SetExpressions(expressions);
	return pNode;
}

ConditionalExpression* SyntaxTreeDelegate::CreateConditionalExpression(Expression* pTest, Expression* pConsequent, Expression* pAlternate)
{
	ConditionalExpression* pNode = new ConditionalExpression(m_Parser.GetPool());
	pNode->SetTest(pTest);
	pNode->SetConsequent(pConsequent);
	pNode->SetAlternate(pAlternate);
	return pNode;
}

LabeledStatement* SyntaxTreeDelegate::CreateLabeledStatement(Identifier* pLabel, Statement* pBody)
{
	LabeledStatement* pNode = new LabeledStatement(m_Parser.GetPool());
	pNode->SetLabel(pLabel);
	pNode->SetBody(pBody);
	return pNode;
}

UpdateExpression* SyntaxTreeDelegate::CreatePostfixExpression(const std::wstring& strOperator, Expression* pArgument)
{
	UpdateExpression* pNode = new UpdateExpression(m_Parser.GetPool());
	pNode->SetOperator(strOperator);
	pNode->SetArgument(pArgument);
	pNode->SetPrefix(false);
	return pNode;
}

FunctionExpression* SyntaxTreeDelegate::CreateFunctionExpression(Identifier* pId, const std::vector<Identifier *> &params, BlockStatement * pBody)
{
	FunctionExpression* pNode = new FunctionExpression(m_Parser.GetPool());
	pNode->SetID(pId);
	pNode->SetParams(params);
	pNode->SetBody(pBody);
	return pNode;
}

NewExpression* SyntaxTreeDelegate::CreateNewExpression(Expression* pCallee, const std::vector<Expression *> &args)
{
	NewExpression* pNode = new NewExpression(m_Parser.GetPool());
	pNode->SetCallee(pCallee);
	pNode->SetArguments(args);
	return pNode;
}

Program *SyntaxTreeDelegate::CreateProgram(const std::vector<Statement *> &body) 
{
	Program* pNode = new Program(m_Parser.GetPool());
	pNode->SetBody(body);
	return pNode;
}


////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////
WrapTrackingFunction::WrapTrackingFunction(Parser &parser) 
	: m_Parser(parser)
{
	m_Parser.SkipComment();
	m_pMarker = m_Parser.CreateLocationMarker();
}

WrapTrackingFunction::~WrapTrackingFunction() 
{
	DELETE_PTR(m_pMarker);
}

Marker::Marker(Parser &parser) 
	: m_Parser(parser)
{ 
	m_nRange[0] = m_nRange[1] = 0;
}

Marker::~Marker()
{
	
}



void Marker::End() 
{
	m_nRange[1] = m_Parser.GetIndex();
	m_pLoc->_pEnd->SetLine(m_Parser.GetLineNumber());
	m_pLoc->_pEnd->SetColumn(m_Parser.GetIndex() - m_Parser.GetLineStart());
}

void Marker::ApplyGroup(Node* pNode) 
{
	pNode->SetGroupRange(m_nRange[0], m_nRange[1]);
	pNode->SetGroupLocation(m_pLoc);
	m_Parser.GetDelegate().PostProcess(pNode);
}

void Marker::Apply(Node* pNode) 
{
	pNode->SetRange(m_nRange[0], m_nRange[1]);
	pNode->SetLocation(m_pLoc);
	m_Parser.GetDelegate().PostProcess(pNode);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
Parser::Parser(Pool &pool)
	: m_Pool(pool), m_Delegate(*this)
{
}

Program* Parser::Parse(const std::wstring &code) 
{
	TRACE_DEBUG(L"[Parser::Parse] start parse code\r\n\
***********************************************************************************************************\r\n\
%ls\r\n\
***********************************************************************************************************", code.c_str());

	m_strSource = code;
	m_nIndex = 0;
	m_nLineNumber = (m_strSource.size() > 0) ? 1 : 0;
	m_nLineStart = 0;
	m_nLength = (int)m_strSource.size();
	m_pLookahead = NULL;
	m_State = State();

	Program* pProgram=ParseProgram();

	TRACE_DEBUG(L"[Parser::Parse] parse code finish.");
	return pProgram;
}


Program* Parser::ParseProgram()
{
	WrapTrackingFunction wtf(*this);	
	std::vector<Statement*> body;
	m_bStrict = false;
	Peek();
	body = ParseSourceElements();
	return wtf.Close(m_Delegate.CreateProgram(body));
}

void Parser::Peek() 
{
	//backup data
	int nPos = m_nIndex;
	int nLine = m_nLineNumber;
	int nStart = m_nLineStart;

	m_pLookahead = Advance();

	m_nIndex = nPos;
	m_nLineNumber = nLine;
	m_nLineStart = nStart;
}

bool Parser::IsIdentifierName(Token* pToken)
{
	int nType = pToken->GetType();
	return nType == Token::Identifier || nType == Token::Keyword || nType == Token::BooleanLiteral ||nType == Token::NullLiteral;
}

Token* Parser::Advance() 
{
	wchar_t ch;

	SkipComment();

	if (m_nIndex >= m_nLength)
	{
		Token *token = new Token(m_Pool);
		token->SetType(Token::Eof);
		token->SetLineNumber(m_nLineNumber);
		token->SetLineStart(m_nLineStart);
		token->SetRange(m_nIndex, m_nIndex);
		return token;
	}

	ch = m_strSource[m_nIndex];

	// Very common: ( and ) and ;
	if (ch == 40 || ch == 41 || ch == 58) 
	{
		return ScanPunctuator();
	}

	// String literal starts with single quote (#39) or double quote (#34).
	if (ch == 39 || ch == 34) 
	{
		return ScanStringLiteral();
	}

	if (Character::IsIdentifierStart(ch))
	{
		return ScanIdentifier();
	}

	// Dot (.) char #46 can also start a floating-point number, hence the need
	// to check the next character.
	if (ch == 46) 
	{
		if (Character::IsDecimalDigit(m_strSource[m_nIndex + 1]))
			return ScanNumericLiteral();

		return ScanPunctuator();
	}

	if (Character::IsDecimalDigit(ch))
		return ScanNumericLiteral();
	

	return ScanPunctuator();
}

Token* Parser::Lex() 
{
	Token* pToken = NULL;

	pToken = m_pLookahead;
	m_nIndex = pToken->GetRange(1);
	m_nLineNumber = pToken->GetLineNumber();
	m_nLineStart = pToken->GetLineStart();

	m_pLookahead = Advance();

	m_nIndex = pToken->GetRange(1);
	m_nLineNumber = pToken->GetLineNumber();
	m_nLineStart = pToken->GetLineStart();

	return pToken;
}


void Parser::SkipComment() 
{
	wchar_t ch;

	bool bBlockComment = false;
	bool bLineComment = false;

	while (m_nIndex < m_nLength)
	{
		ch = m_strSource[m_nIndex];

		if (bLineComment)
		{
			++m_nIndex;
			if (Character::IsLineTerminator(ch))
			{
				bLineComment = false;
				if (ch == 13 && m_strSource[m_nIndex] == 10) 
				{
					++m_nIndex;
				}
				++m_nLineNumber;
				m_nLineStart = m_nIndex;
			}
		}
		else if (bBlockComment)
		{
			if (Character::IsLineTerminator(ch))
			{
				if (ch == 13 && m_strSource[m_nIndex + 1] == 10)
				{
					++m_nIndex;
				}
				++m_nLineNumber;
				++m_nIndex;
				m_nLineStart = m_nIndex;
				if (m_nIndex >= m_nLength)
				{
					ThrowError(NULL, Character::Format(Messages::UnexpectedToken, L"ILLEGAL"));
				}
			}
			else if (ch == '}')	//{ } 注释
			{
				++m_nIndex;
				bBlockComment = false;
			}
			else 
			{
				ch = m_strSource[m_nIndex++];
				if (m_nIndex >= m_nLength) 
				{
					ThrowError(NULL, Character::Format(Messages::UnexpectedToken, L"ILLEGAL"));
				}
				// Block comment ends with '*/' (char #42, char #47).
				if (ch == 42) 
				{
					ch = m_strSource[m_nIndex];
					if (ch == 47) 
					{
						++m_nIndex;
						bBlockComment = false;
					}
				}
			}
		}
		else if (ch == '{') //{ } 注释
		{
			++m_nIndex;
			bBlockComment = true;
			if (m_nIndex >= m_nLength)
			{
				ThrowError(NULL, Character::Format(Messages::UnexpectedToken, L"ILLEGAL"));
			}
		}
		else if (ch == 47) 
		{
			ch = m_strSource[m_nIndex + 1];
			// Line comment starts with '//' (char #47, char #47).
			if (ch == 47) 
			{
				m_nIndex += 2;
				bLineComment = true;
			}
			else if (ch == 42) 
			{
				// Block comment starts with '/*' (char #47, char #42).
				m_nIndex += 2;
				bBlockComment = true;
				if (m_nIndex >= m_nLength) 
				{
					ThrowError(NULL, Character::Format(Messages::UnexpectedToken, L"ILLEGAL"));
				}
			}
			else 
			{
				break;
			}
		}
		else if (Character::IsWhiteSpace(ch)) 
		{
			++m_nIndex;
		}
		else if (Character::IsLineTerminator(ch))
		{
			++m_nIndex;
			if (ch == 13 && m_strSource[m_nIndex] == 10)
			{
				++m_nIndex;
			}
			++m_nLineNumber;
			m_nLineStart = m_nIndex;
		}
		else 
		{
			break;
		}
	}
}

Token *Parser::ScanPunctuator()
{
	Token *pToken = NULL;
	int nStart = m_nIndex;
	wchar_t code = m_strSource[m_nIndex];
	wchar_t code2;
	wchar_t ch1 = m_strSource[m_nIndex];
	wchar_t ch2 = 0;
	wchar_t ch3 = 0;
	wchar_t ch4 = 0;

	//:= 赋值操作
	if (code == 58)
	{
		code2 = m_strSource[m_nIndex + 1];
		if (code2 == 61)
		{
			m_nIndex += 2;
			pToken = CreateToken(Token::Punctuator, std::wstring(1, code) + std::wstring(1, code2), nStart);
			return pToken;
		}
	}

	switch (code) 
	{
	// Check for most common single-character punctuators.
	case 46:   // . dot
	case 40:   // ( open bracket
	case 41:   // ) close bracket
	case 59:   // ; semicolon
	case 44:   // , comma
	case 123:  // { open curly brace
	case 125:  // } close curly brace
	case 91:   // [
	case 93:   // ]
	case 58:   // :
	case 63:   // ?
	case 126:  // ~
		++m_nIndex;
		pToken = CreateToken(Token::Punctuator, std::wstring(1, code), nStart);
		return pToken;

	default:
		code2 = m_strSource[m_nIndex + 1];

		// '=' (char #61) marks an assignment or comparison operator.
		if (code2 == 61) 
		{
			switch (code) 
			{
			case 37:  // %
			case 38:  // &
			case 42:  // *
			case 43:  // +
			case 45:  // -
			case 47:  // /
			case 60:  // <
			case 62:  // >
			case 94:  // ^
			case 124: // |
			case 58:  // :
				m_nIndex += 2;
				pToken = CreateToken(Token::Punctuator, std::wstring(1, code) + std::wstring(1, code2), nStart);
				return pToken;

			case 33: // !
			case 61: // =
				m_nIndex += 2;

				// != and ==
				if (m_strSource[m_nIndex] == 61) 
				{
					++m_nIndex;
				}
				pToken = CreateToken(Token::Punctuator, m_strSource.substr(nStart, m_nIndex - nStart), nStart);
				return pToken;

			default:
				break;
			}
		}
		break;
	}

	// Peek more characters.
	if (m_nIndex + 1<m_nLength) ch2 = m_strSource[m_nIndex + 1];
	if (m_nIndex + 2<m_nLength) ch3 = m_strSource[m_nIndex + 2];
	if (m_nIndex + 3<m_nLength) ch4 = m_strSource[m_nIndex + 3];

	if (ch1 == 'O' && ch2 == 'R')	//OR
	{
		m_nIndex += 2;
		pToken = CreateToken(Token::Punctuator, std::wstring(1, ch1) + std::wstring(1, ch2), nStart);
		return pToken;
	}

	if (ch1 == '<' && ch2 == '>')	//<> 不等于
	{
		m_nIndex += 2;
		pToken = CreateToken(Token::Punctuator, std::wstring(1, ch1) + std::wstring(1, ch2), nStart);
		return pToken;
	}

	if (ch1 == 'A' && ch2 == 'N' && ch3 == 'D')	//AND
	{
		m_nIndex += 3;
		pToken = CreateToken(Token::Punctuator, std::wstring(1, ch1) + std::wstring(1, ch2) + std::wstring(1, ch3), nStart);
		return pToken;
	}

	// 4-character punctuator: >>>=
	if (ch1 == '>' && ch2 == '>' && ch3 == '>') 
	{
		if (ch4 == '=') 
		{
			m_nIndex += 4;
			pToken = CreateToken(Token::Punctuator, L">>>=", nStart);
			return pToken;
		}
	}

	// 3-character punctuators: == != >>> <<= >>=
	if (ch1 == '>' && ch2 == '>' && ch3 == '>') 
	{
		m_nIndex += 3;
		pToken = CreateToken(Token::Punctuator, L">>>", nStart);
		return pToken;
	}

	if (ch1 == '<' && ch2 == '<' && ch3 == '=') 
	{
		m_nIndex += 3;
		pToken = CreateToken(Token::Punctuator, L"<<=", nStart);
		return pToken;
	}

	if (ch1 == '>' && ch2 == '>' && ch3 == '=') {
		m_nIndex += 3;
		pToken = CreateToken(Token::Punctuator, L">>=", nStart);
		return pToken;
	}

	// Other 2-character punctuators: ++ -- << >> && ||
	if (ch1 == ch2 && std::wstring(L"+-<>&|").find(ch1) != std::wstring::npos) 
	{
		m_nIndex += 2;
		pToken = CreateToken(Token::Punctuator, std::wstring(1, ch1) + std::wstring(1, ch2), nStart);
		return pToken;
	}

	if (std::wstring(L"<>=!+-*%&|^/").find(ch1) != std::wstring::npos)
	{
		++m_nIndex;
		pToken = CreateToken(Token::Punctuator, std::wstring(1, ch1), nStart);
		return pToken;
	}

	ThrowError(NULL, Character::Format(Messages::UnexpectedToken, L"ILLEGAL"));
	return NULL;
}

Token* Parser::ScanStringLiteral() 
{
	std::wstring str = L"";
	wchar_t quote, ch, code, unescaped=0 ;
	int nStart = 0, restore = 0;
	bool octal = false;

	quote = m_strSource[m_nIndex];
	assert((quote == '\'' || quote == '"') && "String literal must starts with a quote");

	nStart = m_nIndex;
	++m_nIndex;

	while (m_nIndex < m_nLength)
	{
		ch = m_strSource[m_nIndex++];

		if (ch == quote) 
		{
			quote = 0;
			break;
		}
		else if (ch == '\\') 
		{
			ch = m_strSource[m_nIndex++];
			if (!ch || !Character::IsLineTerminator(ch))
			{
				switch (ch) 
				{
				case 'n':
					str += '\n';
					break;
				case 'r':
					str += '\r';
					break;
				case 't':
					str += '\t';
					break;
				case 'u':
				case 'x':
					//restore = m_nIndex;
					//unescaped = scanHexEscape(ch);
					//if (unescaped != -1) 
					//{
					//	str += unescaped;
					//}
					//else 
					//{
					//	m_nIndex = restore;
					//	str += ch;
					//}
					break;
				case 'b':
					str += L'\b';
					break;
				case 'f':
					str += L'\f';
					break;
				case 'v':
					str += L'\v';
					break;

				default:
					if (Character::IsOctalDigit(ch)) 
					{
						code = ch - '0';

						// \0 is not octal escape sequence
						if (code != 0) 
						{
							octal = true;
						}

						if (m_nIndex < m_nLength && Character::IsOctalDigit(m_strSource[m_nIndex])) 
						{
							octal = true;
							code = code * 8 + m_strSource[m_nIndex++] - '0';

							// 3 digits are only allowed when string starts
							// with 0, 1, 2, 3
							if (std::wstring(L"0123").find(ch) != std::wstring::npos &&
								m_nIndex < m_nLength &&
								Character::IsOctalDigit(m_strSource[m_nIndex])) 
							{
								code = code * 8 + m_strSource[m_nIndex++] - '0';
							}
						}
						str += std::wstring(1, code);
					}
					else 
					{
						str += ch;
					}
					break;
				}
			}
			else 
			{
				++m_nLineNumber;
				if (ch == '\r' && m_strSource[m_nIndex] == '\n')
				{
					++m_nIndex;
				}
			}
		}
		else if (Character::IsLineTerminator(ch))
		{
			break;
		}
		else 
		{
			str += ch;
		}
	}

	if (quote != 0) 
	{
		ThrowError(NULL, Character::Format(Messages::UnexpectedToken, L"ILLEGAL"));
	}

	Token *pToken = CreateToken(Token::StringLiteral, str, nStart);
	pToken->SetOctal(octal);
	return pToken;
}

Token *Parser::ScanIdentifier() 
{
	int nStart = 0, nType = 0;
	std::wstring id;

	nStart = m_nIndex;

	// Backslash (char #92) starts an escaped character.
	id = (m_strSource[m_nIndex] == 92) ? GetEscapedIdentifier() : GetIdentifier();

	// There is no keyword or literal with only one character.
	// Thus, it must be an identifier.
	if (id.size() == 1) nType = Token::Identifier;
	//else if (IsKeyword(id)) nType = Token::Keyword;
	else if (id == L"null") nType = Token::NullLiteral;
	else if (id == L"true" || id == L"false") nType = Token::BooleanLiteral;
	else nType = Token::Identifier;

	if (id == L"AND" || id == L"OR") nType = Token::Punctuator;

	Token* pToken = CreateToken(nType, id, nStart);
	return pToken;
}

Token* Parser::ScanNumericLiteral() 
{
	std::wstring number;
	int nStart = 0;
	wchar_t ch;

	ch = m_strSource[m_nIndex];
	assert(Character::IsDecimalDigit(ch) || (ch == '.') && "Numeric literal must start with a decimal digit or a decimal point");

	nStart = m_nIndex;
	number = L"";
	if (ch != '.') 
	{
		number = std::wstring(1, m_strSource[m_nIndex++]);
		ch = m_strSource[m_nIndex];

		// Hex number starts with '0x'.
		// Octal number starts with '0'.
		if (number == L"0") 
		{
			if (ch == 'x' || ch == 'X') 
			{
				++m_nIndex;
				return ScanHexLiteral(nStart);
			}
			if (Character::IsOctalDigit(ch))
			{
				return ScanOctalLiteral(nStart);
			}

			// decimal number starts with '0' such as '09' is illegal.
			if (ch && Character::IsDecimalDigit(ch))
			{
				ThrowError(NULL, Character::Format(Messages::UnexpectedToken, L"ILLEGAL"));
			}
		}

		while (Character::IsDecimalDigit(m_strSource[m_nIndex]))
		{
			number += m_strSource[m_nIndex++];
		}
		ch = m_strSource[m_nIndex];
	}

	if (ch == '.') 
	{
		number += m_strSource[m_nIndex++];
		while (Character::IsDecimalDigit(m_strSource[m_nIndex]))
		{
			number += m_strSource[m_nIndex++];
		}
		ch = m_strSource[m_nIndex];
	}

	if (ch == 'e' || ch == 'E') 
	{
		number += m_strSource[m_nIndex++];

		ch = m_strSource[m_nIndex];
		if (ch == '+' || ch == '-')
		{
			number += m_strSource[m_nIndex++];
		}
		if (Character::IsDecimalDigit(m_strSource[m_nIndex]))
		{
			while (Character::IsDecimalDigit(m_strSource[m_nIndex])) 
			{
				number += m_strSource[m_nIndex++];
			}
		}
		else 
		{
			ThrowError(NULL, Character::Format(Messages::UnexpectedToken, L"ILLEGAL"));
		}
	}

	if (Character::IsIdentifierStart(m_strSource[m_nIndex]))
	{
		ThrowError(NULL, Character::Format(Messages::UnexpectedToken, L"ILLEGAL"));
	}

	double dValue = _wtof(number.c_str());
	Token *pToken = CreateToken(Token::NumericLiteral, dValue, nStart);
	return pToken;
}

Token* Parser::ScanHexLiteral(int nStart) 
{
	std::wstring number = L"";

	while (m_nIndex < m_nLength)
	{
		if (!Character::IsHexDigit(m_strSource[m_nIndex]))
		{
			break;
		}
		number += m_strSource[m_nIndex++];
	}

	if (number.size() == 0) 
	{
		ThrowError(NULL, Character::Format(Messages::UnexpectedToken, L"ILLEGAL"));
	}

	if (Character::IsIdentifierStart(m_strSource[m_nIndex]))
	{
		ThrowError(NULL, Character::Format(Messages::UnexpectedToken, L"ILLEGAL"));
	}

	double dValue = 0;
	//std::wstringstream(number) >> std::hex >> dValue;
	Token* pToken = CreateToken(Token::NumericLiteral, dValue,nStart);
	return pToken;
}

Token* Parser::ScanOctalLiteral(int nStart)
{
	std::wstring number = std::wstring(1, m_strSource[m_nIndex++]);
	while (m_nIndex < m_nLength)
	{
		if (!Character::IsOctalDigit(m_strSource[m_nIndex]))
		{
			break;
		}
		number += m_strSource[m_nIndex++];
	}

	if (Character::IsIdentifierStart(m_strSource[m_nIndex]) || Character::IsDecimalDigit(m_strSource[m_nIndex])) 
	{
		ThrowError(NULL, Character::Format(Messages::UnexpectedToken, L"ILLEGAL"));
	}

	double dValue = 0;
	//std::wstringstream(number) >> std::oct >> dValue;
	Token* pToken = CreateToken(Token::NumericLiteral, dValue, nStart);
	pToken->SetOctal(true);
	return pToken;
}

std::wstring Parser::GetEscapedIdentifier() 
{
	wchar_t ch;
	std::wstring id;

	ch = m_strSource[m_nIndex++];
	id = std::wstring(1, ch);

	while (m_nIndex < m_nLength) 
	{
		ch = m_strSource[m_nIndex];
		if (!Character::IsIdentifierPart(ch))
		{
			break;
		}
		++m_nIndex;
		id += ch;

		// '\u' (char #92, char #117) denotes an escaped character.
		/* not need escaped character
		if (ch == 92) 
		{
			id = id.substr(0, id.size() - 1);
			if (source[index] != 117) {
				ThrowError(NULL, format(Messages::UnexpectedToken, L"ILLEGAL"));
			}
			++index;
			ch = scanHexEscape('u');
			if (ch == -1 || ch == '\\' || !isIdentifierPart(ch)) {
				ThrowError(NULL, format(Messages::UnexpectedToken, L"ILLEGAL"));
			}
			id += ch;
		}
		*/
	}

	return id;
}

std::wstring Parser::GetIdentifier()
{
	int nStart = 0;
	wchar_t ch;

	nStart = m_nIndex++;
	while (m_nIndex < m_nLength)
	{
		ch = m_strSource[m_nIndex];
		if (ch == 92) 
		{
			// Blackslash (char #92) marks Unicode escape sequence.
			m_nIndex = nStart;
			return GetEscapedIdentifier();
		}
		if (Character::IsIdentifierPart(ch))
		{
			++m_nIndex;
		}
		else 
		{
			break;
		}
	}

	return m_strSource.substr(nStart, m_nIndex - nStart);
}


Marker* Parser::CreateLocationMarker()
{
	Marker *pMarker = new Marker(*this);
	SourceLocation* pLoc = new SourceLocation(m_Pool);
	pLoc->_pStart = new Position(m_Pool);
	pLoc->_pStart->SetLine(m_nLineNumber);
	pLoc->_pStart->SetColumn(m_nIndex - m_nLineStart);

	pLoc->_pEnd = new Position(m_Pool);
	pLoc->_pEnd->SetLine(m_nLineNumber);
	pLoc->_pEnd->SetColumn(m_nIndex - m_nLineStart);

	pMarker->SetLocation(pLoc);
	return pMarker;
}

Expression* Parser::TrackGroupExpression()
{
	Marker* pMarker = NULL;
	Expression* pExpr = NULL;

	SkipComment();
	pMarker = CreateLocationMarker();
	Expect(L"(");

	pExpr = ParseExpression();

	Expect(L")");

	pMarker->End();
	pMarker->ApplyGroup(pExpr);

	return pExpr;
}

Expression* Parser::TrackLeftHandSideExpression()
{
	Marker* pMarker=NULL;
	Expression* pExpr = NULL;
	Expression* pProperty = NULL;

	SkipComment();
	pMarker = CreateLocationMarker();

	pExpr = MatchKeyword(L"new") ? ParseNewExpression() : ParsePrimaryExpression();

	while (Match(L".") || Match(L"[")) 
	{
		if (Match(L"[")) 
		{
			pProperty = ParseComputedMember();
			pExpr = m_Delegate.CreateMemberExpression(L'[', pExpr, pProperty);
			pMarker->End();
			pMarker->Apply(pExpr);
		}
		else 
		{
			pProperty = ParseNonComputedMember();
			pExpr = m_Delegate.CreateMemberExpression(L'.', pExpr, pProperty);
			pMarker->End();
			pMarker->Apply(pExpr);
		}
	}

	return pExpr;
}

Expression* Parser::TrackLeftHandSideExpressionAllowCall()
{
	Marker* pMarker = NULL;
	Expression* pExpr = NULL;
	std::vector<Expression *> args;
	Expression* pProperty = NULL;

	SkipComment();
	pMarker = CreateLocationMarker();

	pExpr = MatchKeyword(L"new") ? ParseNewExpression() : ParsePrimaryExpression();

	while (Match(L".") || Match(L"[") || Match(L"(")) 
	{
		if (Match(L"(")) 
		{
			args = ParseArguments();
			pExpr = m_Delegate.CreateCallExpression(pExpr, args);
			pMarker->End();
			pMarker->Apply(pExpr);
		}
		else if (Match(L"[")) 
		{
			pProperty = ParseComputedMember();
			pExpr = m_Delegate.CreateMemberExpression(L'[', pExpr, pProperty);
			pMarker->End();
			pMarker->Apply(pExpr);
		}
		else 
		{
			pProperty = ParseNonComputedMember();
			pExpr = m_Delegate.CreateMemberExpression(L'.', pExpr, pProperty);
			pMarker->End();
			pMarker->Apply(pExpr);
		}
	}

	delete pMarker;
	return pExpr;
}

void Parser::VisitBinary(Node* pNode)
{
	if (pNode->Is<LogicalExpression>()) 
	{
		LogicalExpression* pExpr = pNode->As<LogicalExpression>();
		Visit(pExpr, pExpr->GetLeft(), pExpr->GetRight());
	}
	else if (pNode->Is<BinaryExpression>())
	{
		BinaryExpression* pExpr = pNode->As<BinaryExpression>();
		Visit(pExpr, pExpr->GetLeft(), pExpr->GetRight());
	}
}

void Parser::Visit(Expression* pNode, Expression* pLeft, Expression* pRight) 
{
	VisitBinary(pLeft);
	VisitBinary(pRight);

	if (pLeft->GetGroupRange(0) || pRight->GetGroupRange(0) )
	{
		int nStart = pLeft->GetGroupRange(0) ? pLeft->GetGroupRange(0) : pLeft->GetRange(0);
		int nEnd = pRight->GetGroupRange(0) ? pRight->GetGroupRange(1) : pRight->GetRange(1);
		pNode->SetRange(nStart,nEnd);
	}
	else if (!pNode->GetRange(1))
	{
		int nStart = pLeft->GetRange(0);
		int nEnd = pRight->GetRange(1);
		pNode->SetRange(nStart, nEnd);
	}

	if (pLeft->GetGroupLocation() || pRight->GetGroupLocation())
	{
		assert(pLeft->GetGroupLocation() || pLeft->GetLocation());
		assert(pRight->GetGroupLocation() || pRight->GetLocation());
		Position* pStart = pLeft->GetGroupLocation() ? pLeft->GetGroupLocation()->_pStart : pLeft->GetLocation()->_pStart;
		Position* pEnd = pRight->GetGroupLocation() ? pRight->GetGroupLocation()->_pEnd : pRight->GetLocation()->_pEnd;
		pNode->SetLocation(new SourceLocation(m_Pool));
		pNode->GetLocation()->_pStart = pStart;
		pNode->GetLocation()->_pEnd = pEnd;
		m_Delegate.PostProcess(pNode);
	}
	else if (pNode->GetLocation() == NULL)
	{
		assert(pLeft->GetLocation());
		assert(pRight->GetLocation());
		pNode->SetLocation(new SourceLocation(m_Pool));
		pNode->GetLocation()->_pStart = pLeft->GetLocation()->_pStart;
		pNode->GetLocation()->_pEnd = pRight->GetLocation()->_pEnd;
		m_Delegate.PostProcess(pNode);
	}
}

Statement* Parser::ParseSourceElement()
{
	int nType = m_pLookahead->GetType();
	if (nType == Token::Keyword)
	{
		//if (lookahead->stringValue == "const" || lookahead->stringValue == "let")
		//	return parseConstLetDeclaration(lookahead->stringValue);
		//else if (lookahead->stringValue == "function")
		//	return parseFunctionDeclaration();
		//else
		return ParseStatement();
	}

	if (nType != Token::Eof) return ParseStatement();

	return NULL;
}

std::vector<Statement *> Parser::ParseSourceElements()
{
	Statement *sourceElement;
	std::vector<Statement*> sourceElements;
	Token* pToken = NULL;
	std::wstring directive;
	Token* pFirstRestricted = NULL;

	/*
	while (m_nIndex < m_nLength) 
	{
		pToken = m_pLookahead;
		if (pToken->GetType() != Token::StringLiteral) break;

		sourceElement = ParseSourceElement();
		sourceElements.push_back(sourceElement);

		if (sourceElement->As<ExpressionStatement>()->GetExpression()->Is<StringLiteral>()) 
			break;// this is not directive
		
		directive = source.substr(token->range[0] + 1, token->range[1] - 1 - token->range[0]);
		if (directive == "use strict") {
			strict = true;
			if (firstRestricted) {
				throwError(firstRestricted, Messages::StrictOctalLiteral);
			}
		}
		else {
			if (!firstRestricted && token->octal) {
				firstRestricted = token;
			}
		}
	}
	*/

	while (m_nIndex < m_nLength) 
	{
		sourceElement = ParseSourceElement();
		if (sourceElement == NULL) break;
		
		sourceElements.push_back(sourceElement);
	}
	return sourceElements;
}

Statement *Parser::ParseStatement() 
{
	WrapTrackingFunction wtf(*this);
	int type = m_pLookahead->GetType();
	Expression* pExpr=NULL;
	Statement* pLabeledBody=NULL;
	std::wstring strKey;

	if (type == Token::Eof) ThrowUnexpected(m_pLookahead);

	if (type == Token::Punctuator) {
		switch (m_pLookahead->GetStringValue()[0]) 
		{
		case ';':
			return wtf.Close(ParseEmptyStatement());
		case '{':
			return wtf.Close(ParseBlock());
		case '(':
			return wtf.Close(ParseExpressionStatement());
		default:
			break;
		}
	}

	/*
	if (type == Token::Keyword) {
		const std::string &value = lookahead->stringValue;
		if (value == "break")
			return wtf.close(parseBreakStatement());
		if (value == "continue")
			return wtf.close(parseContinueStatement());
		if (value == "debugger")
			return wtf.close(parseDebuggerStatement());
		if (value == "do")
			return wtf.close(parseDoWhileStatement());
		if (value == "for")
			return wtf.close(parseForStatement());
		if (value == "function")
			return wtf.close(parseFunctionDeclaration());
		if (value == "if")
			return wtf.close(parseIfStatement());
		if (value == "return")
			return wtf.close(parseReturnStatement());
		if (value == "switch")
			return wtf.close(parseSwitchStatement());
		if (value == "throw")
			return wtf.close(parseThrowStatement());
		if (value == "try")
			return wtf.close(parseTryStatement());
		if (value == "var")
			return wtf.close(parseVariableStatement());
		if (value == "while")
			return wtf.close(parseWhileStatement());
		if (value == "with")
			return wtf.close(parseWithStatement());
	}
	*/

	pExpr = ParseExpression();

	// 12.12 Labelled Statements
	if (pExpr->Is<Identifier>() && Match(L":")) 
	{
		Lex();

		strKey = pExpr->As<Identifier>()->GetName();
		if (m_State.GetLabel().count(strKey))
			ThrowError(NULL, Character::Format(Messages::Redeclaration, L"Label", strKey.c_str()));
		

		m_State.GetLabel().insert(strKey);
		pLabeledBody = ParseStatement();
		m_State.GetLabel().erase(strKey);
		return wtf.Close(m_Delegate.CreateLabeledStatement(pExpr->As<Identifier>(), pLabeledBody));
	}

	ConsumeSemicolon();

	return wtf.Close(m_Delegate.CreateExpressionStatement(pExpr));
}

EmptyStatement* Parser::ParseEmptyStatement()
{
	Expect(L";");
	return m_Delegate.CreateEmptyStatement();
}

BlockStatement* Parser::ParseBlock()
{
	WrapTrackingFunction wtf(*this);
	std::vector<Statement *> block;

	Expect(L"{");

	block = ParseStatementList();

	Expect(L"}");

	return wtf.Close(m_Delegate.CreateBlockStatement(block));
}

int Parser::BinaryPrecedence(Token* pToken, bool allowIn)
{
	int prec = 0;

	if (pToken->GetType() != Token::Punctuator && pToken->GetType() != Token::Keyword)
		return 0;

	const std::wstring &value = pToken->GetStringValue();

	if (value == L"||" || value==L"OR") prec = 1;
	else if (value == L"&&" || value==L"AND") prec = 2;
	else if (value == L"|") prec = 3;
	else if (value == L"^") prec = 4;
	else if (value == L"&") prec = 5;
	else if (value == L"==" || value == L"!=" || value==L"<>" || value == L"===" || value == L"!==" || value==L"=") prec = 6;
	else if (value == L"<" || value == L">" || value == L"<=" || value == L">=" || value == L"instanceof") prec = 7;
	else if (value == L"in") prec = allowIn ? 7 : 0;
	else if (value == L"<<" || value == L">>" || value == L">>>") prec = 8;
	else if (value == L"+" || value == L"-") prec = 9;
	else if (value == L"*" || value == L"/" || value == L"%") prec = 11;

	return prec;
}

Identifier* Parser::ParseNonComputedProperty()
{
	WrapTrackingFunction wtf(*this);
	Token *pToken = Lex();

	if (!IsIdentifierName(pToken)) ThrowUnexpected(pToken);
	

	return wtf.Close(m_Delegate.CreateIdentifier(pToken->GetStringValue()));
}

Identifier* Parser::ParseNonComputedMember()
{
	Expect(L".");
	return ParseNonComputedProperty();
}

Expression* Parser::ParseComputedMember() 
{
	WrapTrackingFunction wtf(*this);
	Expression* pExpr=NULL;

	Expect(L"[");

	pExpr = ParseExpression();

	Expect(L"]");

	return wtf.Close(pExpr);
}

NewExpression* Parser::ParseNewExpression() 
{
	WrapTrackingFunction wtf(*this);
	Expression* pCallee = NULL;
	std::vector<Expression *> args;

	ExpectKeyword(L"new");
	pCallee = ParseLeftHandSideExpression();
	if (Match(L"(")) args = ParseArguments();

	return wtf.Close(m_Delegate.CreateNewExpression(pCallee, args));
}


Expression* Parser::ParseLeftHandSideExpressionAllowCall()
{
	WrapTrackingFunction wtf(*this);
	return wtf.Close(TrackLeftHandSideExpressionAllowCall());

	/*
	Expression *expr;
	std::vector<Expression *> args;
	Expression *property;

	expr = matchKeyword("new") ? parseNewExpression() : parsePrimaryExpression();

	while (match(".") || match("[") || match("(")) {
	if (match("(")) {
	args = parseArguments();
	expr = delegate.createCallExpression(expr, args);
	} else if (match("[")) {
	property = parseComputedMember();
	expr = delegate.createMemberExpression('[', expr, property);
	} else {
	property = parseNonComputedMember();
	expr = delegate.createMemberExpression('.', expr, property);
	}
	}

	return expr;
	*/
}

Expression* Parser::ParseLeftHandSideExpression()
{
	WrapTrackingFunction wtf(*this);
	return wtf.Close(TrackLeftHandSideExpression());

	/*
	Expression *expr;
	Expression *property;

	expr = matchKeyword("new") ? parseNewExpression() : parsePrimaryExpression();

	while (match(".") || match("[")) {
	if (match("[")) {
	property = parseComputedMember();
	expr = delegate.createMemberExpression('[', expr, property);
	} else {
	property = parseNonComputedMember();
	expr = delegate.createMemberExpression('.', expr, property);
	}
	}

	return expr;
	*/
}

// 11.1 Primary Expressions

Expression* Parser::ParsePrimaryExpression()
{
	WrapTrackingFunction wtf(*this);
	int nType=0;
	Token* pToken = NULL;

	nType = m_pLookahead->GetType();
	if (nType == Token::Identifier) return wtf.Close(m_Delegate.CreateIdentifier(Lex()->GetStringValue()));
	

	if (nType == Token::StringLiteral || nType == Token::NumericLiteral)
	{
		//if (strict && lookahead->octal) throwError(lookahead, Messages::StrictOctalLiteral);
		return wtf.Close(m_Delegate.CreateLiteral(Lex()));
	}

	if (nType == Token::Keyword) 
	{
		//if (MatchKeyword(L"this")) 
		//{
		//	Lex();
		//	return wtf.Close(m_Delegate.CreateThisExpression());
		//}

		//if (MatchKeyword(L"function")) return wtf.Close(ParseFunctionExpression());
	}

	if (nType == Token::BooleanLiteral) 
	{
		pToken = Lex();
		return wtf.Close(m_Delegate.CreateLiteral(pToken));
	}

	if (nType == Token::NullLiteral)
	{
		pToken = Lex();
		return wtf.Close(m_Delegate.CreateLiteral(pToken));
	}

	//if (Match(L"[")) return wtf.Close(ParseArrayInitialiser());
	//if (Match(L"{")) return wtf.Close(ParseObjectInitialiser());
	if (Match(L"(")) return wtf.Close(ParseGroupExpression());
	//if (Match(L"/") || Match(L"/=")) return wtf.Close(m_Delegate.CreateLiteral(ScanRegExp()));
	

	ThrowUnexpected(Lex());

	return NULL;
}

void Parser::ConsumeSemicolon() 
{
	int nLine=0;

	// Catch the very common case first: immediately a semicolon (char #59).
	if (m_strSource[m_nIndex] == 59) 
	{
		Lex();
		return;
	}

	m_nIndex = m_nLineNumber;
	SkipComment();
	if (m_nLineNumber != nLine) return;

	if (Match(L";")) 
	{
		Lex();
		return;
	}

	if (m_pLookahead->GetType() != Token::Eof && !Match(L"}")) ThrowUnexpected(m_pLookahead);
}

// 11.1.4 Array Initialiser
ArrayExpression* Parser::ParseArrayInitialiser()
{
	std::vector<Expression *> elements;

	Expect(L"[");

	while (!Match(L"]")) 
	{
		if (Match(L",")) 
		{
			Lex();
			elements.push_back(NULL);
		}
		else 
		{
			elements.push_back(ParseAssignmentExpression());

			if (!Match(L"]")) 
			{
				Expect(L",");
			}
		}
	}

	Expect(L"]");

	return m_Delegate.CreateArrayExpression(elements);
}

// 11.1.5 Object Initialiser

FunctionExpression* Parser::ParsePropertyFunction(const std::vector<Identifier *>& param, Token* pFirst)
{
	WrapTrackingFunction wtf(*this);
	bool bPreviousStrict = false;
	BlockStatement* pBody = NULL;

	bPreviousStrict = m_bStrict;
	pBody = ParseFunctionSourceElements();
	if (pFirst && m_bStrict && Character::IsRestrictedWord(param[0]->GetName())) ThrowError(pFirst, Messages::StrictParamName);
	
	m_bStrict = bPreviousStrict;
	return wtf.Close(m_Delegate.CreateFunctionExpression(NULL, param, pBody));
}

// 11.1.6 The Grouping Operator
Expression* Parser::ParseGroupExpression()
{
	WrapTrackingFunction wtf(*this);
	return wtf.Close(TrackGroupExpression());

	/*
	Expression *expr;

	expect("(");

	expr = parseExpression();

	expect(")");

	return expr;
	*/
}

// 11.2 Left-Hand-Side Expressions

std::vector<Expression*> Parser::ParseArguments()
{
	std::vector<Expression*> args;

	Expect(L"(");

	if (!Match(L")")) 
	{
		while (m_nIndex < m_nLength)
		{
			args.push_back(ParseAssignmentExpression());
			if (Match(L")")) break;
			
			Expect(L",");
		}
	}

	Expect(L")");

	return args;
}

// 11.3 Postfix Expressions
Expression* Parser::ParsePostfixExpression()
{
	Expression* pExpr = ParseLeftHandSideExpressionAllowCall();
	Token* pToken=NULL;

	if (m_pLookahead->GetType() != Token::Punctuator) return pExpr;
	
	if ((Match(L"++") || Match(L"--")) && !PeekLineTerminator()) 
	{
		// 11.3.1, 11.3.2
		//if (strict && expr->is<Identifier>() && isRestrictedWord(expr->as<Identifier>()->name)) {
		//	throwError(NULL, Messages::StrictLHSPostfix);
		//}

		if (!IsLeftHandSide(pExpr)) ThrowError(NULL, Messages::InvalidLHSInAssignment);
		

		pToken = Lex();
		pExpr = m_Delegate.CreatePostfixExpression(pToken->GetStringValue(), pExpr);
	}

	return pExpr;
}

// 11.4 Unary Operators
Expression* Parser::ParseUnaryExpression()
{
	WrapTrackingFunction wtf(*this);
	Token* pToken=NULL;
	Expression* pExpr=NULL;

	if (m_pLookahead->GetType() != Token::Punctuator && m_pLookahead->GetType() != Token::Keyword)  return wtf.Close(ParsePostfixExpression());
	

	if (Match(L"++") || Match(L"--")) 
	{
		pToken = Lex();
		pExpr = ParseUnaryExpression();
		// 11.4.4, 11.4.5
		//if (strict && expr->is<Identifier>() && isRestrictedWord(expr->as<Identifier>()->name)) {
		//	throwError(NULL, Messages::StrictLHSPrefix);
		//}

		if (!IsLeftHandSide(pExpr)) ThrowError(NULL, Messages::InvalidLHSInAssignment);
		

		return wtf.Close(m_Delegate.CreateUnaryExpression(pToken->GetStringValue(), pExpr));
	}

	if (Match(L"+") || Match(L"-") || Match(L"~") || Match(L"!")) 
	{
		pToken = Lex();
		pExpr = ParseUnaryExpression();
		return wtf.Close(m_Delegate.CreateUnaryExpression(pToken->GetStringValue(), pExpr));
	}

	if (MatchKeyword(L"delete") || MatchKeyword(L"void") || MatchKeyword(L"typeof")) 
	{
		pToken = Lex();
		pExpr = ParseUnaryExpression();
		pExpr = m_Delegate.CreateUnaryExpression(pToken->GetStringValue(), pExpr);
		//if (strict && expr->as<UnaryExpression>()->operator_ == "delete" && expr->as<UnaryExpression>()->argument->is<Identifier>()) {
		//	throwError(NULL, Messages::StrictDelete);
		//}
		return wtf.Close(pExpr);
	}

	return wtf.Close(ParsePostfixExpression());
}

// 11.5 Multiplicative Operators
// 11.6 Additive Operators
// 11.7 Bitwise Shift Operators
// 11.8 Relational Operators
// 11.9 Equality Operators
// 11.10 Binary Bitwise Operators
// 11.11 Binary Logical Operators
Expression* Parser::ParseBinaryExpression()
{
	WrapTrackingFunction wtf(*this);
	Expression* pExpr = NULL;
	Token* pToken = NULL;
	int prec;
	bool previousAllowIn;
	std::vector<int> precedenceStack;
	std::vector<Token*> operatorStack;
	std::vector<Expression *> expressionStack;
	Expression *pRight=NULL;
	std::wstring operator_;
	Expression *pLeft=NULL;
	int i;

	previousAllowIn = m_State.IsAllowIn();
	m_State.SetAllowIn(true);

	pExpr = ParseUnaryExpression();

	pToken = m_pLookahead;
	prec = BinaryPrecedence(pToken, previousAllowIn);
	if (prec == 0) return wtf.Close(pExpr);
	
	Lex();

	expressionStack.push_back(pExpr);
	precedenceStack.push_back(prec);
	operatorStack.push_back(pToken);
	expressionStack.push_back(ParseUnaryExpression());

	while ((prec = BinaryPrecedence(m_pLookahead, previousAllowIn)) > 0) 
	{

		// Reduce: make a binary expression from the three topmost entries.
		while ((expressionStack.size() > 1) && (prec <= precedenceStack[precedenceStack.size() - 1])) 
		{
			pRight = expressionStack[expressionStack.size() - 1]; expressionStack.pop_back();
			operator_ = operatorStack[operatorStack.size() - 1]->GetStringValue(); operatorStack.pop_back(); precedenceStack.pop_back();
			pLeft = expressionStack[expressionStack.size() - 1]; expressionStack.pop_back();
			expressionStack.push_back(m_Delegate.CreateBinaryExpression(operator_, pLeft, pRight));
		}

		// Shift.
		pToken = Lex();
		precedenceStack.push_back(prec);
		operatorStack.push_back(pToken);
		expressionStack.push_back(ParseUnaryExpression());
	}

	m_State.SetAllowIn(previousAllowIn);

	// Final reduce to clean-up the stack.
	i = (int)expressionStack.size() - 1;
	pExpr = expressionStack[i];
	while (i > 0) 
	{
		pExpr = m_Delegate.CreateBinaryExpression(operatorStack[i - 1]->GetStringValue(), expressionStack[i - 1], pExpr);
		i--;
	}
	return wtf.Close(pExpr);
}

// 11.12 Conditional Operator
Expression* Parser::ParseConditionalExpression()
{
	WrapTrackingFunction wtf(*this);
	Expression *pExpr=NULL;
	bool previousAllowIn;
	Expression *consequent, *alternate;

	pExpr = ParseBinaryExpression();

	if (Match(L"?")) 
	{
		Lex();
		previousAllowIn = m_State.IsAllowIn();
		m_State.SetAllowIn(true);
		consequent = ParseAssignmentExpression();
		m_State.SetAllowIn(previousAllowIn);
		Expect(L":");
		alternate = ParseAssignmentExpression();

		pExpr = m_Delegate.CreateConditionalExpression(pExpr, consequent, alternate);
	}

	return wtf.Close(pExpr);
}

// 11.13 Assignment Operators
Expression* Parser::ParseAssignmentExpression() 
{
	WrapTrackingFunction wtf(*this);
	Token* pToken = NULL;
	Expression* pLeft = NULL;
	Expression* pRight = NULL;

	pToken = m_pLookahead;
	pLeft = ParseConditionalExpression();

	if (MatchAssign()) 
	{
		// LeftHandSideExpression
		if (!IsLeftHandSide(pLeft)) ThrowError(NULL, Messages::InvalidLHSInAssignment);
		
		// 11.13.1
		//if (strict && left->is<Identifier>() && isRestrictedWord(left->as<Identifier>()->name)) {
		//	throwError(token, Messages::StrictLHSAssignment);
		//}

		pToken = Lex();
		pRight = ParseAssignmentExpression();
		return wtf.Close(m_Delegate.CreateAssignmentExpression(pToken->GetStringValue(), pLeft, pRight));
	}

	return wtf.Close(pLeft);
}

// 11.14 Comma Operator
Expression* Parser::ParseExpression()
{
	WrapTrackingFunction wtf(*this);
	Expression *expr = ParseAssignmentExpression();

	if (Match(L",")) 
	{
		std::vector<Expression *> expressions;
		expressions.push_back(expr);

		while (m_nIndex < m_nLength) 
		{
			if (!Match(L",")) break;
			
			Lex();
			expressions.push_back(ParseAssignmentExpression());
		}

		expr = m_Delegate.CreateSequenceExpression(expressions);
	}
	return wtf.Close(expr);
}

// 12.1 Block
std::vector<Statement *> Parser::ParseStatementList() 
{
	std::vector<Statement *> list;
	Statement* pStatement = NULL;

	while (m_nIndex < m_nLength) 
	{
		if (Match(L"}")) break;
		
		pStatement = ParseSourceElement();
		if (pStatement == NULL)  break;
		
		list.push_back(pStatement);
	}

	return list;
}

// 12.4 Expression Statement
ExpressionStatement* Parser::ParseExpressionStatement()
{
	Expression* pExpr = ParseExpression();
	ConsumeSemicolon();
	return m_Delegate.CreateExpressionStatement(pExpr);
}

// 13 Function Definition

BlockStatement *Parser::ParseFunctionSourceElements()
{
	WrapTrackingFunction wtf(*this);
	Statement* pSourceElement = NULL;
	std::vector<Statement *> sourceElements;
	Token* pToken = NULL;
	std::wstring strDirective;
	Token* pFirstRestricted = NULL;
	std::set<std::wstring> oldLabelSet;
	bool bOldInIteration=false, bOldInSwitch=false, bOldInFunctionBody=false;

	Expect(L"{");

	while (m_nIndex < m_nLength) 
	{
		if (m_pLookahead->GetType() != Token::StringLiteral) break;
		
		pToken = m_pLookahead;

		pSourceElement = ParseSourceElement();
		sourceElements.push_back(pSourceElement);
		if (!pSourceElement->As<ExpressionStatement>()->GetExpression()->Is<StringLiteral>()) 
		{
			// this is not directive
			break;
		}

		strDirective = m_strSource.substr(pToken->GetRange(0) + 1, pToken->GetRange(1) - 1 - pToken->GetRange(0));
		if (strDirective == L"use strict") 
		{
			m_bStrict = true;
			if (pFirstRestricted) ThrowError(pFirstRestricted, Messages::StrictOctalLiteral);
		}
		else 
		{
			if (!pFirstRestricted && pToken->IsOctal()) pFirstRestricted = pToken;
			
		}
	}

	oldLabelSet = m_State.GetLabel();
	bOldInIteration = m_State.IsInIteration();
	bOldInSwitch = m_State.IsInSwitch();
	bOldInFunctionBody = m_State.IsInFunctionBody();

	m_State.SetLabelSet(std::set<std::wstring>());
	m_State.SetInIteration(false);
	m_State.SetInSwitch(false);
	m_State.SetInFunctionBody(true);

	while (m_nIndex < m_nLength) 
	{
		if (Match(L"}")) break;
		
		pSourceElement = ParseSourceElement();
		if (pSourceElement == NULL) break;
		
		sourceElements.push_back(pSourceElement);
	}

	Expect(L"}");

	m_State.SetLabelSet(oldLabelSet);
	m_State.SetInIteration(bOldInIteration);
	m_State.SetInSwitch(bOldInSwitch);
	m_State.SetInFunctionBody(bOldInFunctionBody);

	return wtf.Close(m_Delegate.CreateBlockStatement(sourceElements));
}

/*	暂时不需要
FunctionExpression* Parser::ParseFunctionExpression()
{
	WrapTrackingFunction wtf(*this);
	Token* pToken = NULL;
	Identifier* pID = NULL;
	Token* pStricted = NULL;
	Token* pFirstRestricted = NULL;
	std::wstring message;
	ParseParams *tmp;
	std::vector<Identifier *> params;
	BlockStatement *body;
	bool previousStrict;

	expectKeyword("function");

	if (!match("(")) {
		token = lookahead;
		id = parseVariableIdentifier();
		if (strict) {
			if (isRestrictedWord(token->stringValue)) {
				throwError(token, Messages::StrictFunctionName);
			}
		}
		else {
			if (isRestrictedWord(token->stringValue)) {
				firstRestricted = token;
				message = Messages::StrictFunctionName;
			}
			else if (isStrictModeReservedWord(token->stringValue)) {
				firstRestricted = token;
				message = Messages::StrictReservedWord;
			}
		}
	}

	tmp = parseParams(firstRestricted);
	params = tmp->params;
	stricted = tmp->stricted;
	firstRestricted = tmp->firstRestricted;
	if (!tmp->message.empty()) {
		message = tmp->message;
	}

	previousStrict = strict;
	body = parseFunctionSourceElements();
	if (strict && firstRestricted) {
		throwError(firstRestricted, message);
	}
	if (strict && stricted) {
		throwError(stricted, message);
	}
	strict = previousStrict;

	return wtf.close(delegate.createFunctionExpression(id, params, body));
}
*/


// Expect the next token to match the specified punctuator.
// If not, an exception will be thrown.
void Parser::Expect(const std::wstring &value)
{
	Token *pToken = Lex();
	if (pToken->GetType() != Token::Punctuator || pToken->GetStringValue() != value)
		ThrowUnexpected(pToken);
}

void Parser::ExpectKeyword(const std::wstring &keyword) 
{
	Token* pToken = Lex();
	if (pToken->GetType() != Token::Keyword || pToken->GetStringValue() != keyword) ThrowUnexpected(pToken);
}

bool Parser::Match(const std::wstring &value)
{
	return m_pLookahead->GetType() == Token::Punctuator && m_pLookahead->GetStringValue() == value;
}

// Return true if the next token is an assignment operator
bool Parser::MatchAssign() 
{
	std::wstring op;

	if (m_pLookahead->GetType() != Token::Punctuator) return false;
	
	op = m_pLookahead->GetStringValue();
	return op == L":=" || op == L":";
}

bool Parser::MatchKeyword(const std::wstring &keyword)
{
	return m_pLookahead->GetType() == Token::Keyword && m_pLookahead->GetStringValue() == keyword;
}


// Return true if provided expression is LeftHandSideExpression
bool Parser::IsLeftHandSide(Node* pExpr) 
{
	return pExpr->Is<Identifier>() || pExpr->Is<MemberExpression>();
}

// Return true if there is a line terminator before the next token.
bool Parser::PeekLineTerminator() 
{
	int nPos = m_nIndex;
	int nLine = m_nLineNumber;
	int nStart = m_nLineStart;
	SkipComment();
	bool bFound = m_nLineNumber != nLine;
	m_nIndex = nPos;
	m_nLineNumber = nLine;
	m_nLineStart = nStart;

	return bFound;
}

Token* Parser::CreateToken(int nType,const std::wstring& strValue, int nStart)
{
	Token* pToken = new Token(m_Pool);
	pToken->SetType(nType);
	pToken->SetStringValue(strValue);
	pToken->SetLineNumber(m_nLineNumber);
	pToken->SetLineStart(m_nLineStart);
	pToken->SetRange(nStart, m_nIndex);

	return pToken;
}

Token* Parser::CreateToken(int nType, double dValue, int nStart)
{
	Token* pToken = new Token(m_Pool);
	pToken->SetType(nType);
	pToken->SetDoubleValue(dValue);
	pToken->SetLineNumber(m_nLineNumber);
	pToken->SetLineStart(m_nLineStart);
	pToken->SetRange(nStart, m_nIndex);

	return pToken;
}

void Parser::ThrowUnexpected(Token *pToken)
{
	if (pToken->GetType() == Token::Eof) ThrowError(pToken, Messages::UnexpectedEOS);
	if (pToken->GetType() == Token::NumericLiteral) ThrowError(pToken, Messages::UnexpectedNumber);
	if (pToken->GetType() == Token::StringLiteral) ThrowError(pToken, Messages::UnexpectedString);
	if (pToken->GetType() == Token::Identifier) ThrowError(pToken, Messages::UnexpectedIdentifier);
	
	/*
	if (token->type == Token::Keyword) {
		if (isFutureReservedWord(token->stringValue)) {
			throwError(token, Messages::UnexpectedReserved);
		}
		else if (strict && isStrictModeReservedWord(token->stringValue)) {
			throwError(token, Messages::StrictReservedWord);
		}
		throwError(token, format(Messages::UnexpectedToken, token->stringValue.c_str()));
	}
	*/

	// BooleanLiteral, NullLiteral, or Punctuator.
	ThrowError(pToken, Character::Format(Messages::UnexpectedToken, pToken->GetStringValue().c_str()));
}

void Parser::ThrowError(Token *token, const std::wstring &msg)
{
	ParseExcept error(msg);

	if (token) 
	{
		std::wstringstream ss;
		ss << L"Line " << token->GetLineNumber() << L": " << msg;
		error.SetDescription(ss.str());
		error.SetIndex(token->GetRange(0));
		error.SetLineNumber(token->GetLineNumber());
		error.SetColumn(token->GetRange(0) - m_nLineStart + 1);
	}
	else 
	{
		std::wstringstream ss;
		ss << L"Line " << m_nLineNumber << L": " << msg;
		error.SetDescription(ss.str());
		error.SetIndex(m_nIndex);
		error.SetLineNumber(m_nLineNumber);
		error.SetColumn(m_nIndex - m_nLineNumber + 1);
	}

	throw error;
}


Program* Parse(Pool &pool, const std::wstring &code) 
{
	return Parser(pool).Parse(code);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//

Variant* NumericLiteral::GetVariant(VariantOperator* pVariantOperator) const
{
	Variant* pResult = pVariantOperator->Create();
	pResult->SetDoubleValue(m_dValue);
	return pResult;
}

std::wstring CallExpression::GetCalleeName() const
{
	std::wstring strName;
	if (m_pCallee && m_pCallee->GetType() == Syntax::IDENTIFIER_ID)
	{
		Identifier * p = m_pCallee->As<Identifier>();
		strName = p->GetName();
	}

	return strName;
}



}
}