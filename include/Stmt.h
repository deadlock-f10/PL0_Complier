#include "Node.h"
#include "Expr.h"
#include <queue>
#include <vector>

#ifndef STMT_H
#define STMT_H

class Stmt;
class Stmt : public Node{
	public :
		Stmt(){}
		static Stmt* Null;
		virtual void gen(label begin , label after){}
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
		IfElse(Expr *expr , Stmt *stmt1, Stmt *stmt2){e = expr;s1 =stmt1; s2 = stmt2;}
		void gen(label before, label after);
};

class DoWhile : public Stmt{
	public :
		Expr * e;
		Stmt * s;
		DoWhile(Expr *expr , Stmt *stmt){e = expr;s =stmt;}
		void gen(label before, label after);
};

class For : public Stmt{
	public :
		Id * id;
		Expr* e1;
		bool is_to;
		Expr* e2;
		Stmt* s;
		For(Id * i,Expr *x1,bool to,Expr* x2,Stmt* stmt){
			id = i; e1=x1;is_to = to; e2=x2;stmt = s;
		}
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
		bool check(Type *a ,Type *b);    // unimplemented
		void gen(label begin,label after); 
};

class Seq : public Stmt{
	public:
	Stmt * s1;
	Stmt * s2;
	Seq(Stmt *stmt1,Stmt *stmt2){s1 = stmt1; s2 = stmt2;}
	void gen(label begin , label after);
};
/*
class Compound : public stmt{
	Stmt * s1;
	Stmt * s2;
	Compound(Stmt *stmt1,Stmt *stmt2){s1 = stmt1; s2 = stmt2;}
	void gen(label begin , label after);
};*/

class Input: public Stmt{
	public :
	std::queue<Word*> *idlist;
	Input(std::queue<Word*> *list) {idlist = list;}
};

class Output: public Stmt{
	public:
	STring * s;
	Expr * e;
	Output(Expr *x,STring *string){s = string;e=x;}
};

class Callproc: public Stmt{        
	public:
	Proc* p;           // form para with "var" must correspond to a Id in actual para. should check that
	std::vector<Expr*> *actuallist;
	Callproc(Proc *proc , std::vector<Expr*> *list){p = proc;list = actuallist;}
};
#endif
