#include "Node.h"
//#include "Expr.h"
//#include "Stmt.h"
#include <utility>
#include <unordered_map>
#include <vector>
#ifndef PAF_H
#define PAF_H
typedef std::unordered_map<Token* , Node*> Hashtable;
class Block;
class Seq;
class Id;
class Program;
class Program :public Node{
	public :
	Program *prev = nullptr;
	int level;
	Hashtable symboltable;
	Block *block;
	int used = 0;
	static Program* Null;
	Program(){level = 1;}
	virtual void gen(label begin , label after){};
	void put(Token* t , Node* i){symboltable.insert(std::make_pair(t,i));}
	Node* get(Token* w);
};
class Proc : public Program{
	public :
		std::vector<Id*> paralist;
		Word* name; 
		label l;
//		std::string symtype = "procedure";
		Proc(Program  *p,Word *w , int l) { prev = p;name = w;level = l+1;}
};

class Func : public Program{
	public :
		std::vector<Id*> paralist;
		label l;                     // L label:
		Word* name; 
		Type* type;
//		std::string symtype = "function";
		Func(Program *p,Word *w, int l) {prev = p;name = w;level = l+1;}
};

class Seq_PAF : public Program{
	public :
	Program* paf;
	Program* pafs;
	Seq_PAF(Program* p1,Program *p2){paf = p1; pafs = p2;}
};

class Block {
	public :
	Seq_PAF * seq_paf;
	Seq * seq_stmt;
	Program * belongto;
	Block(Program *p){belongto = p;}
};
#endif
