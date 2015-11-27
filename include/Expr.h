#include "Node.h"
#include "PAF.h"
#include <vector>
#ifndef EXPR_H
#define EXPR_H
class Rel;
class Expr : public Node{
	public :
		Token *op;
		Type *type;
		Expr(Token* tok , Type *t) {op = tok ; type = t;}
		virtual Expr* gen(Program *p) {return this;} //genarate a object with its subobject reduced
		virtual Expr* reduce(Program *p) {return this;} // resolve to a single address(identifier or constant)
		//virtual void jumping(label iftrue , label iffalse,Program *p);
		//virtual void emitjumps(Rel* rel, label iftrue , label iffalse,Program *p);
		//virtual std::string toString(){return op->toString();}
};

class Id : public Expr {          // need add field which specify whether this identifier is const or not
	public :
		int offset ;        // relative address ( of what ?)
		int level ;         //don't need level   --no~~~~ we need it
		bool isConst;
		bool isRef = false;
		Id(Word* w , Type* t , int o,int l): Expr(w,t) {offset = o; isConst = false; level = l;}
		Id(Word* w , Type* t , int o , bool b,int l): Expr(w,t) {offset = o; isConst = b; level = l;}
		Id(Word* w , Type* t , int o , bool b1 , bool b2,int l): Expr(w,t) {offset = o; isConst = b1; isRef = b2; level = l;}
};

class Callfunc : public Expr{
	public :
	Func* f;           // form para with "var" must correspond to a Id in actual para. should check that
	std::vector<Expr*> *actuallist;
	Callfunc(Func *func , std::vector<Expr*> *list):Expr(func->name,func->type){f = func;list = actuallist;}
	Expr* reduce(Program *p);
};
class Op : public Expr {
	public :
		Op(Token* tok , Type* t) : Expr(tok , t){}
		Expr* reduce(Program *p);
};

class Arith : public Op {
	public :
		Expr * e1;
		Expr * e2;
		Arith(Token* tok , Expr *x1 , Expr* x2); 
		Expr* gen(Program *p){ return new Arith(op, e1->reduce(p) , e2->reduce(p)); }
		Expr* reduce(Program *p);
		//std::string toString() {return e1->toString() + " " + op->toString() + " " + e2->toString();}
};

class Temp : public  Expr {
	public :
	static int count;
	int number = 0;
	Temp(Type *t) : Expr(Word::temp , t) {number = ++count;}
	//std::string toString(){return "t"+number;}
};

class Unary : public Op { // its  op will only be minus
	public :
		Expr* e;
		Unary(Token *tok , Expr *x);
		Expr* gen(Program *p){return new Unary(op, e->reduce(p)) ;}
		Expr* reduce(Program *p);
		//std::string  toString(){return op->toString() + " " + e->toString();}
};

class Constant : public Expr {
	public :
		int c ;
		Constant (Num *t): Expr(t , Type::Int){c = t->value;}
		Constant (Character *t): Expr(t , Type::Int){c = t->value;}
};

class Rel : public Expr {
	public :
		Expr *e1;
		Expr *e2;
		Rel (Token *t ,Expr *x1 , Expr *x2);
		Expr* gen(Program *p);
		//Expr* reduce(Program *p);
	private:
		bool check( Type* p1 ,Type *p2) ;    
};

class Access : public Op{
	public :
		Id * array ;
		Expr * index ; 
		Access (Id *id ,Type * t, Expr * e) : Op(new Word("[]" ,T_INDEX),t) {array = id ; index = e; }
		Expr * gen(Program *p) {return new Access(array,type,index->reduce(p));}
		//void jumping(label iftrue, label iffalse) {emitjumps(reduce()->toString(),iftrue,iffalse);}
		//std::string toString(){return array->toString()+"[" + index->toString() + "]";}
};
#endif
