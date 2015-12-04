#include "Quadruple.h"
#include <map>
#include <vector>
#include <string>
#ifndef OPTIMIZERR_H
#define	OPTIMIZERR_H
class Program;
class Seq_PAF;

typedef std::vector<Quadruple*> QuadList;
class BasicBlock{
	public:
		QuadList instrlist;
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
		}
}
};

class Optimizer{
	public :
	Program *p;
	Optimizer(Program *prog) { p = prog;}
	void splitblock(Program *p);
	void optimize(){splitblock(p);}
	void splitseq_paf(Seq_PAF * s);
	void changelabel(Program *p,std::map<std::string,std::string> *relabel);
};
#endif
