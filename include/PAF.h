#include "Node.h"
#include "Expr.h"
#include "Stmt.h"
#include <utility>
#include <unordered_map>
#ifndef PAF_H
#define PAF_H
typedef std::unordered_map<Token , Node> Hashtable;
class Program :public Node{
	public :
	int level;
	Hashtable symboltable;
	Block *block;
	int used = 0;
	static Program* Null;
	Program(){level = 1;}
	virtual gen(label begin , label after){};
	void put(Token* t , Node* i){table.insert(std::make_pair(t,i))}
	Id* get(Token* w);
}
class Proc : public Program{
	public :
		Program *prev;
		std::vector<Id*> paralist;
		Word* name; 
		label l;
//		std::string symtype = "procedure";
		Proc(Program  *p,Word *w , int l) { prev = p;name = w;level = l+1;}
};

class Func : public Program{
	public :
		Program *prev;
		std::vector<Id*> paralist;
		label l;                     // L label:
		Word* name; 
		Type* type;
//		std::string symtype = "function";
		Proc(Program *p,Word *w, int l) {prev = p;name = w;level = l+1;}
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
}
#endif
