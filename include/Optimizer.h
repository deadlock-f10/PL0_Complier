#include "Quadruple.h"
#include <map>
#include <vector>
#ifndef OPTIMIZERR_H
#define	OPTIMIZERR_H
class Program;
class Seq_PAF;

typedef std::vector<Quadruple*> QuadList;
class BasicBlock{
	public:
		QuadList instrlist;
	//int size;
		static int num;
		std::string label;
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
