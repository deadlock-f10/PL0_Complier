#include "../include/Optimizer.h"
#include "../include/PAF.h"
#include "../include/Tag.h"
#include <map>
#include <string>
#include <vector>

extern ofstream targetcode;
void Optimizer::dag(Program *p){
	for(unsigned int i = 1; i < p->blocklist.size(); i++){
		BasicBlock * b = p->blocklist[i];
		b->dag();
		b->instrlist.clear();
		for(QuadList::iterator it = b->daglized.begin(); it != b->daglized.end(); it++)
			b->instrlist.push_back(*it);
	}
}
void Optimizer::splitseq_paf(Seq_PAF * s){
	if(s->paf != Program::Null)
		splitblock(s->paf);
	if(s->pafs != Program::Null)
		splitseq_paf((Seq_PAF*)(s->pafs));
}
int BasicBlock::num = 0;
void BasicBlock::print(){
	for(std::vector<std::string>::iterator it = list.begin(); it != list.end();it++)
		targetcode<<*it<<endl;
}

void Optimizer::changelabel(Program *p,std::map<std::string,std::string> *relabel){
	for(unsigned int i = 1 ; i < p->blocklist.size(); i++){
		BasicBlock *b = p->blocklist[i];
		for(unsigned int j = 0; j < b->instrlist.size(); j++){
			Quadruple *q = b->instrlist[j];
			if(q->op == I_GOTO || q->op == I_IF || q->op == I_IFFALSE){
				Result_label *x = (Result_label *)(q->result);
				std::string m = (relabel->find(x->label))->second;
				x->label = m;
			}
		}
	}
}

void Optimizer::splitblock(Program *p){
	std::map<std::string,std::string> *relabel = new std::map<std::string,std::string>();
	p->blocklist.push_back(new BasicBlock(nullptr,0,-1)); //place holder
	//int blocknum = 1;
	std::vector<int> begin;
	begin.push_back(0);
	for(unsigned int i = 1; i < p->Instrlist.size();i++){
		Quadruple *x = p->Instrlist[i]; if( x->labellist != nullptr){
			if(begin.back() != (signed)i)
				begin.push_back(i);
		}
		if(x->op == I_IF || x->op == I_IFFALSE || x->op == I_CALLFUNC || x->op == I_CALLPROC || x->op == I_GOTO){
				begin.push_back(i+1);      // they won't be the last instruction, hence i+1 is always valid.
		}
	}
	unsigned int i = 0;
	for( ; i < begin.size()-1;i++){
		Quadruple *x = p->Instrlist[begin[i]];
		BasicBlock *b = new BasicBlock(&(p->Instrlist),begin[i],begin[i+1] - 1);
		if(x->labellist != nullptr)
			for(unsigned int j = 0 ; j< x->labellist->size() ;j++)
				relabel->insert(std::make_pair(x->labellist->at(j),b->label));
		x->labellist = nullptr;
		p->blocklist.push_back(b);
	}
	Quadruple *x = p->Instrlist[begin[i]];
	BasicBlock *b = new BasicBlock(&(p->Instrlist),begin[i],p->Instrlist.size()-1);
		if(x->labellist != nullptr)
			for(unsigned int j = 0 ; j< x->labellist->size() ;j++)
				relabel->insert(std::make_pair(x->labellist->at(j),b->label));
    x->labellist = nullptr;
	p->blocklist.push_back(b);
	changelabel(p,relabel);
	if(isoptimize == true){
		flowgraph(p);
		livevariable(p);
		dag(p);
		elimideadcode(p);
		globalregassign(p);
	}
	if(Seq_PAF *x = dynamic_cast<Seq_PAF*>(p->block->seq_paf))
		splitseq_paf(x);
}
