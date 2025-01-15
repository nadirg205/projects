# Parser
A top down recursive-descent parser made for a language, known as the Mini C-Like (MCL) Language, which employs the following grammar rules.

Prog ::= PROGRAM IDENT CompStmt

StmtList ::= Stmt { Stmt }

Stmt ::= DeclStmt | ControlStmt | CompStmt

DeclStmt ::= ( INT | FLOAT | BOOL | CHAR | STRING ) VarList ;
VarList ::= Var [= Expr] { ,Var [= Expr]}
ControlStmt ::= AssgnStmt ; | IfStmt | PrintStmt ;
PrintStmt ::= PRINT (ExprList)
CompStmt ::= ‘{‘ StmtList ‘}’
IfStmt ::= IF (Expr) Stmt [ ELSE Stmt ]
AssgnStmt ::= Var ( = | += | -= | *= | /= | %= ) Expr
Var ::= IDENT
ExprList ::= Expr { , Expr }
Expr ::= LogANDExpr { || LogANDRxpr }
LogANDExpr ::= EqualExpr { && EqualExpr }
EqualExpr ::= RelExpr [ (== | != ) RelExpr ]
RelExpr ::= AddExpr [ ( < | > ) AddExpr ]
AddExpr :: MultExpr { ( + | - ) MultExpr }
MultExpr ::= UnaryExpr { ( * | / | % ) UnaryExpr }
UnaryExpr ::= [( - | + | ! )] PrimaryExpr
PrimaryExpr ::= IDENT | ICONST | RCONST | SCONST | BCONST | CCONST | ( Expr )
