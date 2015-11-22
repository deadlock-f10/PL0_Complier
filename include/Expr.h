#include "Node.h"
#ifndef EXPR_H
#define EXPR_H
class Expr : public Node{
	public :
		Token *op;
		Type *type;
		Expr(Token* tok , Type *t) {op = tok ; type = t;}
		virtual Expr* gen() {return this;}
		virtual Expr* reduce {return this;}
		virtual void jumping(label iftrue , label iffalse);
		virtual void emitjumps(std::string test , label iftrue , label iffalse);
		virtual std::string toString(){return op.toString();}
};

class Id : public Expr {          // need add field which specify whether this identifier is const or not
	public :
		int offset ;        // relative address ( of what ?)
		int level ;
		bool isConst;
		bool isRef = false;
		Id(Word* w , Type* t , int o): Expr(w,t) {offset = o; isConst = false;}
		Id(Word* w , Type* t , int o , bool b): Expr(w,t) {offset = o; isConst = bool;}
		Id(Word* w , Type* t , int o , bool b1 , bool b2): Expr(w,t) {offset = o; isConst = b1; isRef = b2;}
};

class Op : public Expr {
	public :
		Op(Token* tok , Type* t) : Expr(tok , t){}
		Expr* reduce();
};

class Arith : public Op {
	public :
		Expr * e1;
		Expr * e2;
		Arith(Token* tok , Expr *x1 , Expr* x2); Expr* gen(){ return new Arith(op, e1.reduce() , e2.reduce()); }
		std::string toString() {return e1.toString() + " " + op.toString + " " + e2.toString();}
};

class Temp : public  Expr {
	public :
	static int count;
	int number = 0;
	Temp(Type *t) : Expr(Word::temp , t) {number += count;}
	std::string toString(){return "t"+number;}
};

class Unary : public Op {
	public :
		Expr* e;
		Unary(Token *tok , Expr *x);
		Expr* gen(){return new Unary(Op, e.reduce()) ;}
		std::string  toString(){return op.toString() + " " + expr.toString();}
};

class Constant : public Expr {
	public :
		int c ;
		Constant (Token *t): Expr(t , Type::int){}
};

class Rel : public Expr {
	public :
		Expr *e1;
		Expr *e2;
		Rel (Token *t ,Expr *x1 , Expr *x2);
	private:
		bool check( Type* p1 ,Type *p2) ;    
};

class Access : public Op{
	public :
		Id * array ;
		Expr * index ; 
		Access (Id *id ,Type * t, Expr * e) : Op(new Word("[]" ,T_INDEX),t) {array = id ; index = e }
		Expr * gen() {return new Access*(id,t,e->reduce());}
		void jumping(label iftrue, label iffalse) {emitjumps(reduce()->toString(),t,f);}
		std::string toString(){return array->toString()+"[" + index.toString + "]";}
}
#endif

