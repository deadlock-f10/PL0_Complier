#include "Node.h"
//#include "Expr.h"
//#include "Stmt.h"
#include <utility>
#include <unordered_map>
#include <vector>
#ifndef PAF_H
#define PAF_H
#define align(x) ((x) % 4 == 0 ? (x) : (x) + 4 - (x) % 4)
typedef std::unordered_map<Token* , Node*> Hashtable;
class Block;
class Seq;
class Id;
class Program;
class Program :public Node{
	public :
		Word* name;
		Program *prev = nullptr;
		int level;
		Hashtable symboltable;
		Block *block;
		int used = 0;
		static Program* Null;
		Program(){level = 1;name = new Word("",T_IDENT);}
		virtual void gen(label begin , label after){};
		void put(Token* t , Node* i){symboltable.insert(std::make_pair(t,i));}
		Node* get(Token* w);
		void setfsize(){framesize = align(used);}        // align 4
		int getfsize(){return framesize;}
	protected :
		int framesize;
};
class Proc : public Program{
	public :
		std::vector<Id*> paralist;
		int para_used = 0;
		label l;
//		std::string symtype = "procedure";
		Proc(Program  *p,Word *w , int l) { prev = p;name = w;level = l;}
		void setfsize(){framesize = align(used);}        // align 4
};

class Func : public Program{
	public :
		int para_used = 0;
		std::vector<Id*> paralist;
		label l;                     // L label:
		Type* type;
//		std::string symtype = "function";
		Func(Program *p,Word *w, int l) {prev = p;name = w;level = l;}
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
};
#endif
