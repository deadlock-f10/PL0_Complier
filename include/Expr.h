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
		virtual Expr* reduce(Program *p) {return this;} // resolve to a single address(identifier or constant or temp)
		virtual std::string to_string(){return "";}
		//virtual void jumping(label iftrue , label iffalse,Program *p);
		//virtual void emitjumps(Rel* rel, label iftrue , label iffalse,Program *p);
		//virtual std::string to_string(){return op->to_string();}
};

class Id : public Expr {          // need add field which specify whether this identifier is const or not
	public :
		int offset ;        // relative address ( of what ?)  --- symboltable of this func/proc
		int level ;         //don't need level   --no~~~~ we need it
		bool isConst =false;
		int constvalue = 0;
		bool isRef = false;
		Id(Word* w , Type* t , int o,int l): Expr(w,t) {offset = o; isConst = false; level = l;}
		//Id(Word* w , Type* t , int o , bool b,int l): Expr(w,t) {offset = o; isConst = b; level = l;}
		Id(Word* w , Type* t , int o , bool b,int cv,int l): Expr(w,t) {offset = o; isConst = b; constvalue = cv; level = l;}
		Id(Word* w , Type* t , int o , bool b1 , bool b2,int l): Expr(w,t) {offset = o; isConst = b1; isRef = b2; level = l;}
		std::string to_string(){return op->to_string();}
};

class Callfunc : public Expr{
	public :
	Func* f;           // form para with "var" must correspond to a Id in actual para. should check that
	std::vector<Expr*> *actuallist;
	Callfunc(Func *func , std::vector<Expr*> *list):Expr(func->name,func->type){f = func;actuallist = list;}
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
		Expr* gen(Program *p){return new Arith(op, e1->reduce(p) , e2->reduce(p));}
		Expr* reduce(Program *p);
		//std::string to_string() {return e1->to_string() + " " + op->to_string() + " " + e2->to_string();}
};

class Temp : public  Id {
	public :
	static int count;
	//int number = 0;
	//Temp(Type *t) : Expr(Word::temp , t) {number = ++count;}             // will not allocate space until final code generation phase
	Temp(Type *t,int l) : Id(new Word("_t"+patch::to_string(++count),T_IDENT),t,0,l){;}      // will not allocate space until final code generation phase
	std::string to_string(){return op->to_string();}
};

class Unary : public Op { // its  op will only be minus
	public :
		Expr* e;
		Unary(Token *tok , Expr *x);
		Expr* gen(Program *p){return new Unary(op, e->reduce(p)) ;}
		Expr* reduce(Program *p);
		//std::string  to_string(){return op->to_string() + " " + e->to_string();}
};

class Constant : public Expr {
	public :
		int c ;
		Constant (Num *t): Expr(t , Type::Int){c = t->value;}
		Constant (Character *t): Expr(t , Type::Char){c = t->value;}
		std::string to_string(){return patch::to_string(c);}
};

class Rel : public Expr {
	public :
		Expr *e1;
		Expr *e2;
		Rel (Token *t ,Expr *x1 , Expr *x2);
		Expr* gen(Program *p);
		std::string to_string(){return e1->to_string() + op->to_string() + e2->to_string();};
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
		Expr* reduce(Program *p);
		//void jumping(label iftrue, label iffalse) {emitjumps(reduce()->to_string(),iftrue,iffalse);}
		//std::string to_string(){return array->to_string()+"[" + index->to_string() + "]";}
};
#endif
