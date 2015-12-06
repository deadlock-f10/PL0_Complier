#include "Node.h"
#include "Quadruple.h"
#include "Optimizer.h"
//#include "Expr.h"
//#include "Stmt.h"
#include <utility>
#include <unordered_map>
#include <vector>
#ifndef PAF_H
#define PAF_H
#define align(x) ((x) % 4 == 0 ? (x) : (x) + 4 - (x) % 4)
typedef std::unordered_map<Token* , Node*> Hashtable;
typedef std::vector<Quadruple*> QuadList;
class Block;
class Seq;
class Id;
class Program;
//class Parser;
//typedef label int;
class Program :public Node{
	public :
		QuadList Instrlist;
		std::vector<BasicBlock*> blocklist;
		std::string beginlabel;
		Word* name;
		Program *prev = nullptr;
		int level;
		Hashtable symboltable;
		Block *block;
		int used = 0;
		static Program* Null;
		Program();
		virtual void gen();
		void put(Token* t , Node* i){
			symboltable.insert(std::make_pair(t,i));
		}
		Node* get(Token* w);
		void addlabel(std::string s){
			if(labellist == nullptr) 
				labellist = new std::vector<std::string>(); 
			labellist->push_back(s);
		}
		void addlabel(int l ){
			if(labellist == nullptr) 
				labellist = new std::vector<std::string>(); 
			labellist->push_back("L"+patch::to_string(l));
		}
		void addinstr(OP op,Arg1* arg1 , Arg2* arg2,Result * result);
		virtual void print();
		virtual void print_block();
		/*void setfsize(){framesize = align(used);}        // align 4
		int getfsize(){return framesize;}*/
	protected :
		int framesize;
		std::vector<std::string> *labellist = nullptr;
};
class Proc : public Program{
	public :
		std::vector<Id*> paralist;
		int para_used = 0;                // count it !!!!
		int para_offset;
		int display_offset;
		//label l;
//		std::string symtype = "procedure";
		Proc(Program  *p,Word *w , int l);
		void setfsize(){framesize = align(used);}        // align 4
};

class Func : public Program{
	public :
		int para_used = 0;
		int para_offset;
		int display_offset;
		std::vector<Id*> paralist;
		//label l;                     // L label:
		Type* type;
//		std::string symtype = "function";
		Func(Program *p,Word *w, int l);
};

class Seq_PAF : public Program{
	public :
	Program* paf;
	Program* pafs;
	Seq_PAF(Program* p1,Program *p2){paf = p1; pafs = p2;}
	void print();
	void print_block();
	void gen();
};

class Block {
	public :
	Seq_PAF * seq_paf = nullptr;
	Seq * seq_stmt = nullptr;
	void print();
	void print_block();
	void gen(Program *p);
};
#endif
