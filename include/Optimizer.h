#include "Quadruple.h"
#include <map>
#include <vector>
#include <string>
#include <unordered_set>
#ifndef OPTIMIZERR_H
#define	OPTIMIZERR_H
class Program;
class Seq_PAF;

typedef std::vector<Quadruple*> QuadList;
typedef std::unordered_set<Id*> Idset;
class BasicBlock{
	public:
		int blocknum;
		QuadList instrlist;
		QuadList daglized;
		std::vector<BasicBlock*> preceeding;
		std::vector<BasicBlock*> following;
		Idset use;
		Idset def;
		Idset In;
		Idset *pre_In;
		Idset Out;
	//int size;
		std::vector<std::string> list;
		static int num;
		std::string label;
		void print();
		BasicBlock(QuadList *l,int begin,int end){
			if(begin > end){
				label = "";
			}
			else{
				for(int i = begin ; i <= end ; i++)
					instrlist.push_back(l->at(i));
				label = "B" + patch::to_string(++num);
				blocknum = num;
			}
		}
		void dag();
		int dagtoquad(int begin); //return end index
		void find_usedef();
		bool test_same(){return In.size() == pre_In->size();}// once a Id go into set In , it will never get out.
		static void set_union(Idset &dest,Idset &b);
		static void set_diff(Idset &dest,Idset &b);
};

class Optimizer{
	public :
	Program *p;
	bool isoptimize = false;
	Optimizer(Program *prog,bool o) { p = prog;isoptimize = o;}
	void splitblock(Program *p);
	void optimize(){
		splitblock(p);
	}
	void splitseq_paf(Seq_PAF * s);
	void changelabel(Program *p,std::map<std::string,std::string> *relabel);
	void dag(Program *p);
	void flowgraph(Program *p);
	void livevariable(Program *p);
	void globalregassign(Program *p);
};
#endif
