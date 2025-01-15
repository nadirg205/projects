/* Implementation of Recursive-Descent Parser
 * for Mini C-Like Language
 * Programming Assignment 2
 * Fall 2024
*/

#include "parserInterp.h"

map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value>* ValQue; //declare a pointer variable to a queue of Value objects

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if (pushed_back) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem& t) {
		if (pushed_back) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;
	}

}

static int error_count = 0;

int ErrCount()
{
	return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

//Program is: Prog = PROGRAM IDENT CompStmt
bool Prog(istream& in, int& line)
{
	bool sl = false;
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == PROGRAM) {
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT) {
			sl = CompStmt(in, line);
			if (!sl)
			{
				ParseError(line, "Invalid Program");
				return false;
			}
			cout << "(DONE)" << endl;
			return true;
		}
		else
		{
			ParseError(line, "Missing Program name");
			return false;
		}
	}
	else if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	else
	{
		ParseError(line, "Missing Program keyword");
		return false;
	}
}//End of Prog

//StmtList ::= Stmt { Stmt }
bool StmtList(istream& in, int& line)
{
	bool status;
	LexItem tok;

	status = Stmt(in, line);
	tok = Parser::GetNextToken(in, line);
	while (status && tok != RBRACE)
	{
		Parser::PushBackToken(tok);

		status = Stmt(in, line);
		tok = Parser::GetNextToken(in, line);
	}
	if (!status)
	{
		ParseError(line, "Syntactic error in statement list.");

		return false;
	}
	Parser::PushBackToken(tok);
	return true;
}//End of StmtList

//DeclStmt ::= ( INT | FLOAT | BOOL | CHAR | STRING) VarList 
bool DeclStmt(istream& in, int& line) {
	bool status = false;
	LexItem tok;

	LexItem t = Parser::GetNextToken(in, line);

	if (t == INT || t == FLOAT || t == CHAR || t == BOOL || t == STRING) {

		status = VarList(in, line, t);

		if (!status)
		{
			ParseError(line, "Incorrect variable in Declaration Statement.");
			return status;
		}
		return true;
	}


	return true;
}//End of Decl

//Stmt ::= DeclStmt ; | ControlStmt ; | CompStmt
bool Stmt(istream& in, int& line) {
	bool status = true;

	LexItem t = Parser::GetNextToken(in, line);

	switch (t.GetToken()) {

	case PRINT: case IF: case IDENT:
		Parser::PushBackToken(t);
		status = ControlStmt(in, line);

		if (!status)
		{
			ParseError(line, "Invalid control statement.");
			return false;
		}

		break;
	case INT: case FLOAT: case CHAR: case BOOL: case STRING:
		Parser::PushBackToken(t);
		status = DeclStmt(in, line);
		if (!status)
		{
			ParseError(line, "Invalid declaration statement.");
			return false;
		}
		t = Parser::GetNextToken(in, line);
		if (t == SEMICOL)
		{
			return true;
		}
		else
		{
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}
		break;
	case LBRACE:
		Parser::PushBackToken(t);
		status = CompStmt(in, line);
		if (!status)
		{
			ParseError(line, "Invalid compound statement.");
			return false;
		}

		break;
	default:
		Parser::PushBackToken(t);

		return false;
	}

	return status;
}//End of Stmt

bool CompStmt(istream& in, int& line)
{
	bool status = false;

	LexItem t = Parser::GetNextToken(in, line);
	if (t == LBRACE)
	{
		status = StmtList(in, line);
		if (!status)
		{
			ParseError(line, "Incorrect statement list");
			return false;
		}
		LexItem t = Parser::GetNextToken(in, line);
		if (t == RBRACE)
		{
			return true;
		}
		else if (t == DONE)
		{
			ParseError(line, "Missing end of program right brace.");
			return false;
		}
		else
		{
			ParseError(line, "Missing right brace.");
			return false;
		}
	}
	return status;
}//End of CompStmt

bool ControlStmt(istream& in, int& line) {
	bool status;

	LexItem t = Parser::GetNextToken(in, line);

	switch (t.GetToken()) {

	case PRINT:
		status = PrintStmt(in, line);

		if (!status)
		{
			ParseError(line, "Incorrect Print Statement");
			return false;
		}
		t = Parser::GetNextToken(in, line);

		if (t == SEMICOL)
		{
			return true;
		}
		else
		{
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}

		break;

	case IDENT:
		Parser::PushBackToken(t);
		status = AssignStmt(in, line);
		if (!status)
		{
			ParseError(line, "Incorrect Assignment Statement");
			return false;
		}
		t = Parser::GetNextToken(in, line);

		if (t == SEMICOL)
		{
			return true;
		}
		else
		{
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}

		break;
	case IF:
		status = IfStmt(in, line);

		if (!status)
		{
			ParseError(line, "Incorrect IF Statement");
			return false;
		}

		break;

	default:
		Parser::PushBackToken(t);
		return true;
	}

	return status;
}//End of ControlStmt

//VarList ::= Var [= Expr] {, Var [= Expr]}
bool VarList(istream& in, int& line, LexItem& idtok) {
	bool status = false, exprstatus = false;
	string identstr;
	Value retVal;

	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == IDENT)
	{

		identstr = tok.GetLexeme();
		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
			SymTable[identstr] = idtok.GetToken();
		}
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}

	}
	else
	{

		ParseError(line, "Missing Variable Name");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok == ASSOP)
	{

		exprstatus = Expr(in, line, retVal);
		if (!exprstatus)
		{
			ParseError(line, "Incorrect initialization for a variable.");
			return false;
		}
		if (idtok == INT) {
			if (retVal.IsInt()) {
				TempsResults[identstr] = retVal;
			}
			else if (retVal.IsChar()) {
				TempsResults[identstr] = Value((int)retVal.GetChar());
			}
		}
		else if (idtok == FLOAT) {
			if (retVal.IsReal()) {
				TempsResults[identstr] = retVal;
			}
			else if (retVal.IsInt()) {
				TempsResults[identstr] = Value((double)retVal.GetInt());
			}
			else if (retVal.IsChar()) {
				TempsResults[identstr] = Value((double)retVal.GetChar());
			}
		}
		else if (idtok == CHAR) {
			if (retVal.IsChar()) {
				TempsResults[identstr] = retVal;
			}
			else if (retVal.IsInt()) {
				TempsResults[identstr] = Value((char)retVal.GetInt());
			}
		}
		else if (idtok == BOOL) {
			if (retVal.IsBool()) {
				TempsResults[identstr] = retVal;
			}
		}
		else if (idtok == STRING) {
			if (retVal.IsString()) {
				TempsResults[identstr] = retVal;
			}
		}

		//cout<< "Initialization of the variable " << identstr <<" in the declaration statement at line " << line << endl;
		tok = Parser::GetNextToken(in, line);

		if (tok == COMMA) {

			status = VarList(in, line, idtok);

		}
		else
		{
			Parser::PushBackToken(tok);
			return true;
		}
	}
	else if (tok == COMMA) {

		status = VarList(in, line, idtok);
	}
	else if (tok == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");

		return false;
	}
	else {

		Parser::PushBackToken(tok);
		return true;
	}

	return status;

}//End of VarList



//PrintStmt:= PRINT (ExpreList) 
bool PrintStmt(istream& in, int& line) {
	LexItem t;
	ValQue = new queue<Value>;

	t = Parser::GetNextToken(in, line);
	if (t != LPAREN) {
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}

	bool ex = ExprList(in, line);

	if (!ex) {
		ParseError(line, "Missing expression list after Print");
		while (!(*ValQue).empty())
		{
			ValQue->pop();
		}
		delete ValQue;
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != RPAREN) {
		ParseError(line, "Missing Right Parenthesis");
		while (!(*ValQue).empty())
		{
			ValQue->pop();
		}
		delete ValQue;
		return false;
	}

	while (!(*ValQue).empty()) //Evaluate: print out the list of expressions' values
	{
		Value nextVal = (*ValQue).front();
		cout << nextVal;
		ValQue->pop();
	}
	delete ValQue;
	cout << endl;
	return true;
}//End of PrintStmt

//IfStmt ::= IF (Expr) Stmt [ ELSE Stmt ]
bool IfStmt(istream& in, int& line) {
	bool ex = false, status;
	static int nestlevel = 0;
	LexItem t;
	Value retVal;

	int braceCount = 0;

	t = Parser::GetNextToken(in, line);
	if (t != LPAREN) {

		ParseError(line, "Missing Left Parenthesis");
		return false;
	}

	ex = Expr(in, line, retVal);
	if (!ex) {
		ParseError(line, "Missing if statement condition");
		return false;
	}

	if (!retVal.IsBool()) {
		ParseError(line, "Illegal type for if statement condition.");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != RPAREN) {

		ParseError(line, "Missing Right Parenthesis");
		return false;
	}

	if (retVal.GetBool() == false) {
		t = Parser::GetNextToken(in, line);
		if (t == LBRACE) {
			braceCount++;
			while (braceCount != 0) {
				t = Parser::GetNextToken(in, line);
				if (t == LBRACE) {
					braceCount++;
				}
				if (t == RBRACE) {
					braceCount--;
				}
			}
		}
		else {
			while (t != SEMICOL) {
				t = Parser::GetNextToken(in, line);
			}
		}

		t = Parser::GetNextToken(in, line);
		if (t == ELSE) {
			//cout << "in IfStmt else-clause at nesting level: " << nestlevel << endl;
			status = Stmt(in, line);
			if (!status)
			{
				ParseError(line, "Missing Statement for If-Stmt Else-Part");
				return false;
			}

			nestlevel--;
			return true;
		}

		nestlevel--;
		Parser::PushBackToken(t);
	}
	else {
		nestlevel += 1;
		//cout << "in IfStmt then-clause at nesting level: " << nestlevel << endl;
		status = Stmt(in, line);
		if (!status)
		{
			ParseError(line, "If-Stmt Syntax Error");
			return false;
		}

		t = Parser::GetNextToken(in, line);
		if (t == ELSE) {
			t = Parser::GetNextToken(in, line);
			if (t == LBRACE) {
				braceCount++;
				while (braceCount != 0) {
					t = Parser::GetNextToken(in, line);
					if (t == LBRACE) {
						braceCount++;
					}
					if (t == RBRACE) {
						braceCount--;
					}
				}
			}
			else {
				while (t != SEMICOL) {
					t = Parser::GetNextToken(in, line);
				}
			}
		}
		else {
			Parser::PushBackToken(t);
		}
	}

	return true;
}//End of IfStmt function

//Var:= ident
bool Var(istream& in, int& line, LexItem& idtok)
{
	string identstr;

	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == IDENT) {
		identstr = tok.GetLexeme();

		if (!(defVar.find(identstr)->second))
		{
			ParseError(line, "Undeclared Variable");
			return false;
		}
		idtok = tok;
		return true;
	}
	else if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}//End of Var

//AssgnStmt ::= Var ( = | += | -= | *= | /= | %= ) Expr
bool AssignStmt(istream& in, int& line) {

	bool varstatus = false, status = false;
	LexItem t, idtok;
	Value retVal;


	varstatus = Var(in, line, idtok);

	if (varstatus) {
		t = Parser::GetNextToken(in, line);

		if (t == ASSOP || t == ADDASSOP || t == SUBASSOP || t == MULASSOP || t == DIVASSOP || t == REMASSOP)
		{
			status = Expr(in, line, retVal);
			if (!status) {
				ParseError(line, "Missing Expression in Assignment Statement");
				return status;
			}
			Token type = SymTable[idtok.GetLexeme()];
			if (t == ASSOP) {
				if (type == INT && !retVal.IsInt()) {
					ParseError(line, "Invalid operand in assignment.");
					return false;
				}
				else if (type == FLOAT) {
					if (!retVal.IsInt() && !retVal.IsReal()) {
						ParseError(line, "Invalid operand in assignment.");
						return false;
					}
				}
				else if (type == BOOL && !retVal.IsBool()) {
					ParseError(line, "Invalid operand in assignment.");
					return false;
				}
				else if (type == CHAR) {
					if (!retVal.IsInt() && !retVal.IsChar()) {
						ParseError(line, "Invalid operand in assignment.");
						return false;
					}
				}
				else if (type == STRING && !retVal.IsString()) {
					ParseError(line, "Invalid operand in assignment.");
					return false;
				}
				TempsResults[idtok.GetLexeme()] = retVal;
			}
			else {
				if (TempsResults.find(idtok.GetLexeme()) == TempsResults.end()) {
					ParseError(line, "Uninitialized variable in assignment.");
					return false;
				}
				if (t == ADDASSOP) {
					TempsResults[idtok.GetLexeme()] = TempsResults[idtok.GetLexeme()] + retVal;
				}
				else if (t == SUBASSOP) {
					TempsResults[idtok.GetLexeme()] = TempsResults[idtok.GetLexeme()] - retVal;
				}
				else if (t == MULASSOP) {
					TempsResults[idtok.GetLexeme()] = TempsResults[idtok.GetLexeme()] * retVal;
				}
				else if (t == DIVASSOP) {
					TempsResults[idtok.GetLexeme()] = TempsResults[idtok.GetLexeme()] / retVal;
				}
				else if (t == REMASSOP) {
					TempsResults[idtok.GetLexeme()] = TempsResults[idtok.GetLexeme()] % retVal;
				}
			}
			if (TempsResults[idtok.GetLexeme()].IsErr()) {
				ParseError(line, "Invalid operand in assignment.");
				return false;
			}

		}
		else if (t.GetToken() == ERR) {
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;
}//End of AssignStmt

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	Value retVal;
	status = Expr(in, line, retVal);
	if (!status) {
		ParseError(line, "Missing Expression");
		return false;
	}
	ValQue->push(retVal);
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == COMMA) {
		status = ExprList(in, line);
	}
	else if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else {
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//End of ExprList

//Expr ::= LogANDExpr { || LogANDRxpr }
bool Expr(istream& in, int& line, Value& retVal) {

	LexItem tok;
	Value retVal2;
	bool t1 = LogANDExpr(in, line, retVal);

	if (!t1) {
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok == OR)
	{
		t1 = LogANDExpr(in, line, retVal2);
		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		} //handle OR

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR) {
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}

		retVal = retVal || retVal2;
		if (retVal.IsErr()) {
			ParseError(line, "Invalid operand for OR operation.");
			return false;
		}

	}
	Parser::PushBackToken(tok);
	return true;
}//End of Expr/LogORExpr

//LogANDExpr ::= EqualExpr { && EqualExpr }
bool LogANDExpr(istream& in, int& line, Value& retVal) {
	LexItem tok;
	Value retVal2;
	bool t1 = EqualExpr(in, line, retVal);

	if (!t1) {
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok == AND)
	{
		t1 = EqualExpr(in, line, retVal2);
		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR) {
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}

		retVal = retVal && retVal2;
		if (retVal.IsErr()) {
			ParseError(line, "Invalid operand for AND operation.");
			return false;
		}

	}

	Parser::PushBackToken(tok);
	return true;
}//End of LogANDExpr

//EqualExpr ::= RelExpr [ (== | !=) RelExpr ]
bool EqualExpr(istream& in, int& line, Value& retVal) {
	LexItem tok;
	Value retVal2;
	bool t1 = RelExpr(in, line, retVal);

	if (!t1) {
		return false;
	}

	tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if (tok == EQ || tok == NEQ)
	{
		t1 = RelExpr(in, line, retVal2);

		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		if (tok == EQ) {
			retVal = retVal == retVal2;
			if (retVal.IsErr()) {
				ParseError(line, "Invalid operand for == operation.");
				return false;
			}
		}
		else {
			retVal = retVal != retVal2;
			if (retVal.IsErr()) {
				ParseError(line, "Invalid operand for != operation.");
				return false;
			}
		}

		tok = Parser::GetNextToken(in, line);
		if (tok == EQ || tok == NEQ)
		{
			ParseError(line, "Illegal Equality Expression.");
			return false;
		}
		else if (tok.GetToken() == ERR) {
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}

	}
	Parser::PushBackToken(tok);

	return true;
}//End of EqualExpr

//RelExpr ::= AddExpr [ ( < | > ) AddExpr ]
bool RelExpr(istream& in, int& line, Value& retVal) {
	LexItem tok;
	Value retVal2;
	bool t1 = AddExpr(in, line, retVal);

	if (!t1) {
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if (tok == LTHAN || tok == GTHAN)
	{
		t1 = AddExpr(in, line, retVal2);
		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		if (tok == LTHAN) {
			retVal = retVal < retVal2;
			if (retVal.IsErr()) {
				ParseError(line, "Invalid operand for LTHAN operation.");
				return false;
			}
		}
		else {
			retVal = retVal > retVal2;
			if (retVal.IsErr()) {
				ParseError(line, "Invalid operand for GTHAN operation.");
				return false;
			}
		}

		tok = Parser::GetNextToken(in, line);

		if (tok == LTHAN || tok == GTHAN)
		{
			ParseError(line, "Illegal Relational Expression.");
			return false;
		}
		else if (tok.GetToken() == ERR) {
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}

	}
	Parser::PushBackToken(tok);
	return true;
}//End of RelExpr

//AddExpr :: MultExpr { ( + | - ) MultExpr }
bool AddExpr(istream& in, int& line, Value& retVal) {

	bool t1 = MultExpr(in, line, retVal);
	LexItem tok;
	Value retVal2;

	if (!t1) {
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok == PLUS || tok == MINUS)
	{
		t1 = MultExpr(in, line, retVal2);
		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		if (tok == PLUS) {
			retVal = retVal + retVal2;
			if (retVal.IsErr()) {
				ParseError(line, "Invalid operand for ADD operation.");
				return false;
			}
		}
		else {
			retVal = retVal - retVal2;
			if (retVal.IsErr()) {
				ParseError(line, "Invalid operand for SUB operation.");
				return false;
			}
		}

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR) {
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}

	}
	Parser::PushBackToken(tok);
	return true;
}//End of AddExpr

//MultExpr ::= UnaryExpr { ( * | / | %) UnaryExpr }
bool MultExpr(istream& in, int& line, Value& retVal) {

	bool t1 = UnaryExpr(in, line, retVal);
	Value retVal2;
	LexItem tok;

	if (!t1) {
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok == MULT || tok == DIV || tok == REM)
	{
		t1 = UnaryExpr(in, line, retVal2);

		if (!t1) {
			ParseError(line, "Missing operand after operator");
			return false;
		}

		if (tok == MULT) {
			retVal = retVal * retVal2;
			if (retVal.IsErr()) {
				ParseError(line, "Invalid operand for MULT operation.");
				return false;
			}
		}
		else if (tok == DIV) {
			retVal = retVal / retVal2;
			if (retVal.IsErr()) {
				ParseError(line, "Invalid operand for DIV operation.");
				return false;
			}
		}
		else {
			retVal = retVal % retVal2;
			if (retVal.IsErr()) {
				ParseError(line, "Invalid operand for REM operation.");
				return false;
			}
		}

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR) {
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}

	}
	Parser::PushBackToken(tok);
	return true;
}//End of MultExpr

//UnaryExpr ::= ( - | + | ! ) PrimaryExpr | PrimaryExpr
bool UnaryExpr(istream& in, int& line, Value& retVal)
{
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	if (t == MINUS)
	{
		sign = -1;
	}
	else if (t == PLUS)
	{
		sign = 1;
	}
	else if (t == NOT)
	{
		sign = 2;
	}
	else
		Parser::PushBackToken(t);

	status = PrimaryExpr(in, line, sign, retVal);
	return status;
}//End of UnaryExpr



//PrimaryExpr ::= IDENT | ICONST | RCONST | SCONST | BCONST | CCONST | ( Expr )
bool PrimaryExpr(istream& in, int& line, int sign, Value& retVal) { // sign:  MINUS = -1, PLUS = 1, NOT = 2

	LexItem tok = Parser::GetNextToken(in, line);


	if (tok == IDENT) {

		string lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(line, "Using Undeclared Variable");
			return false;
		}
		if (sign == 2) {
			if (SymTable[lexeme] != BOOL) {
				ParseError(line, "NOT symbol used on nonboolean value");
				return false;
			}
			retVal = !TempsResults[lexeme];
		}
		else if (sign == 0) {
			retVal = TempsResults[lexeme];
		}
		else {
			if (SymTable[lexeme] == BOOL || SymTable[lexeme] == STRING) {
				ParseError(line, "PLUS or MINUS symbol used on nonnumeric value");
				return false;
			}
			retVal = Value(sign) * TempsResults[lexeme];
		}

		return true;
	}
	else if (tok == ICONST) {
		if (sign == 2) {
			ParseError(line, "NOT symbol used on nonboolean value");
			return false;
		}
		else if (sign == 0) {
			retVal = Value(stoi(tok.GetLexeme()));
		}
		else {
			retVal = Value(sign) * Value(stoi(tok.GetLexeme()));
		}
		return true;
	}
	else if (tok == SCONST) {
		if (sign != 0) {
			ParseError(line, "UNARY operator used on string value");
			return false;
		}
		retVal = Value(tok.GetLexeme());
		return true;
	}
	else if (tok == RCONST) {
		if (sign == 2) {
			ParseError(line, "NOT symbol used on nonboolean value");
			return false;
		}
		else if (sign == 0) {
			retVal = Value(stod(tok.GetLexeme()));
		}
		else {
			retVal = Value(sign) * Value(stod(tok.GetLexeme()));
		}
		return true;
	}
	else if (tok == BCONST) {
		if (sign == 2) {
			if (tok.GetLexeme() == "true") {
				retVal = Value(false);
			}
			else {
				retVal = Value(true);
			}
		}
		else if (sign == 0) {
			if (tok.GetLexeme() == "true") {
				retVal = Value(true);
			}
			else {
				retVal = Value(false);
			}
		}
		else {
			ParseError(line, "SIGN symbol used on boolean value");
			return false;
		}
		return true;
	}
	else if (tok == CCONST)
	{
		if (sign == 2) {
			ParseError(line, "NOT symbol used on nonboolean value");
			return false;
		}
		else if (sign == 0) {
			retVal = Value(tok.GetLexeme()[0]);
		}
		else {
			retVal = Value(sign) * Value(tok.GetLexeme()[0]);
		}
		return true;
	}
	else if (tok == LPAREN) {
		bool ex = Expr(in, line, retVal);
		if (!ex) {
			ParseError(line, "Missing expression after Left Parenthesis");
			return false;
		}

		if (sign == 2) {
			if (!retVal.IsBool()) {
				ParseError(line, "NOT symbol used on nonboolean value");
				return false;
			}
			retVal = !retVal;
		}
		else if (sign == 1 || sign == -1) {
			if (retVal.IsBool() || retVal.IsString()) {
				ParseError(line, "PLUS or MINUS symbol used on nonnumeric value");
				return false;
			}
			retVal = retVal * Value(sign);
		}

		if (Parser::GetNextToken(in, line) == RPAREN)
			return ex;
		else
		{
			Parser::PushBackToken(tok);
			ParseError(line, "Missing right Parenthesis after expression");
			return false;
		}
	}
	else if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	return false;
}//End of PrimaryExpr

