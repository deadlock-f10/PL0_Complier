#include "Node.h"
#include "Expr.h"
#include <queue>
#include <vector>

#ifndef STMT_H
#define STMT_H

class Stmt;
class Program;
class Stmt : public Node{
	public :
		Stmt(){}
		static Stmt* Null;
		virtual void gen(Program *p){;}
//		label after = 0;         used for break
};


class If : public Stmt{
	public :
		Rel* e;
		Stmt* s;
		If(Rel *expr , Stmt *stmt){e = expr;s =stmt;}
		void gen(Program *p);
};

class IfElse : public Stmt{
	public :
		Rel * e;
		Stmt * s1;
		Stmt * s2;
		IfElse(Rel *expr , Stmt *stmt1, Stmt *stmt2){e = expr;s1 =stmt1; s2 = stmt2;}
		//void gen(label before, label after);
		void gen(Program *p);
};

class DoWhile : public Stmt{
	public :
		Rel * e;
		Stmt * s;
		DoWhile(Rel *expr , Stmt *stmt){e = expr;s =stmt;}
		//void gen(label before, label after);
		void gen(Program *p);
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
		void gen(Program *p);
};

class Assign : public Stmt{
	public : 
		Id* id;
		Expr * e;		
		Assign(Id *id,Expr *e);
		bool check(Type *a,Type *b);
		//void gen(label begin , label after){emit(id->toString()+" = "+e->gen()->toString());}
		void gen(Program *p);
};

class AssignElem : public Stmt{
	public :
		Id * array;
		Expr *index;
		Expr *e;
		AssignElem(Access * x,Expr* y);
		bool check(Type *a ,Type *b);    // unimplemented
		//void gen(label begin,label after); 
		void gen(Program *p);
};

class Seq : public Stmt{
	public:
	Stmt * s1;
	Stmt * s2;
	Seq(Stmt *stmt1,Stmt *stmt2){s1 = stmt1; s2 = stmt2;}
	void gen(Program *p);
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
	std::vector<Id*> *idlist;
	Input(std::vector<Id*> *list) {idlist = list;}
	void gen(Program *p);
};

class Output: public Stmt{
	public:
	STring * s;
	Expr * e;
	Output(Expr *x,STring *string){s = string;e=x;}
	void gen(Program *p);
};

class Callproc: public Stmt{        
	public:
	Proc* prc;           // form para with "var" must correspond to a Id in actual para. should check that
	std::vector<Expr*> *actuallist;
	Callproc(Proc *proc , std::vector<Expr*> *list){prc = proc;list = actuallist;}
	void gen(Program *p);
};
#endif
