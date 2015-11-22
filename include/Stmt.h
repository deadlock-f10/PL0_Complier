#include "Node.h"
#include "Expr.h"

#ifndef STMT_H
#define STMT_H

class Stmt;
class Stmt : public Node{
	public :
		Stmt(){}
		static Stmt* Null;
		virtual gen(label begin , label after){}
//		label after = 0;         used for break
};

class If : public Stmt{
	public :
		Expr* e;
		Stmt* s;
		If(Expr *expr , Stmt *stmt){e = expr;s =stmt;}
		void gen(label before, label after);
};

class IfElse : public Stmt{
	public :
		Expr * e;
		Stmt * s1;
		Stmt * s2;
		If(Expr *expr , Stmt *stmt1, Stmt *stmt 2){e = expr;s1 =stmt1; s2 = stmt2;}
		void gen(label before, label after);
};

class DoWhile : public Stmt{
	public :
		Expr * e;
		Stmt * s;
		DoWhile(Expr *expr , Stmt *stmt){e = expr;s =stmt;}
		void gen(label before, label after);
};

class Assign : public Stmt{
	public : 
		Id* id;
		Expr * e;		
		Assign(Id *id,Expr *e);
		bool check(Type *a,Type *b);
		void gen(label begin , label after){emit(id->toString()+" = "+e->gen()->toString());}
};

class AssignElem : public Stmt{
	public :
		Id * array;
		Expr *index;
		Expr *e;
		AssignElem(Access * x,Expr* y);
		check(Type *a ,Type *b);
		void gen(label begin,label after); 
};

class Seq : public Stmt{
	Stmt * s1;
	Stmt * s2;
	Seq(Stmt *stmt1,Stmt *stmt2){s1 = stmt1; s2 = stmt2;}
	void gen(label begin , label after);
};
#endif
