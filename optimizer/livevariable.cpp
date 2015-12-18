#include "../include/Optimizer.h"
#include "../include/PAF.h"
#include "../include/Quadruple.h"
#include "../include/Expr.h"
#include <stdio.h>
#include <unordered_set>

void Optimizer::livevariable(Program *p){
	for(unsigned int i = 1; i < p->blocklist.size(); i++){
		p->blocklist[i]->find_usedef();
		BasicBlock::set_union(p->blocklist[i]->In , p->blocklist[i]->use);
	}
	for(unsigned int i = 1; i < p->blocklist.size(); i++){
		BasicBlock *b = p->blocklist[i];
		for(unsigned j = 0; j < b->following.size() ; j++)
			BasicBlock::set_union(b->Out,b->following[j]->In);
		Idset temp(b->Out);
		BasicBlock::set_diff(temp,b->def);
		BasicBlock::set_union(b->In,temp);
	}
	bool issame = true;
	do{
		issame = true;
		for(unsigned int i = 1; i < p->blocklist.size(); i++)
			p->blocklist[i]->pre_In = new Idset(p->blocklist[i]->In);
		for(unsigned int i = 1; i < p->blocklist.size(); i++){
			BasicBlock *b = p->blocklist[i];
			for(unsigned j = 0; j < b->following.size() ; j++)
				BasicBlock::set_union(b->Out,b->following[j]->In);
			Idset temp(b->Out);
			BasicBlock::set_diff(temp,b->def);
			BasicBlock::set_union(b->In,temp);
			issame &= b->test_same();
		}
	}while(issame == false);
}
void BasicBlock::set_union(Idset &dest,Idset &b){
	for(Idset::iterator it = b.begin(); it != b.end(); it++)
		dest.insert(*it);
}
void BasicBlock::set_diff(Idset &dest,Idset &b){
	for(Idset::iterator it = b.begin(); it != b.end(); it++)
		dest.erase(*it);
}
void Optimizer::flowgraph(Program *p){
	int begin = p->blocklist[1]->blocknum;
	int blocknum;
	int m;
	std::string s;
	for(unsigned int i = 1; i < p->blocklist.size(); i++){
		BasicBlock * b = p->blocklist[i];
		Quadruple * lastinstr = b->instrlist.back();
		switch(lastinstr->op){
			case I_END:
				break;
			case I_GOTO:
				s = ((Result_label*)lastinstr->result)->label;
				sscanf(s.c_str(),"B%d",&m);
				blocknum = m - begin + 1;
				b->following.push_back(p->blocklist[blocknum]);
				p->blocklist[blocknum]->preceeding.push_back(b);
				break;
			case I_IFFALSE:
			case I_IF:
				s = ((Result_label*)lastinstr->result)->label;
				sscanf(s.c_str(),"B%d",&m);
				blocknum = m - begin + 1;
				b->following.push_back(p->blocklist[blocknum]);
				p->blocklist[blocknum]->preceeding.push_back(b);
				b->following.push_back(p->blocklist[i+1]);
				p->blocklist[i+1]->preceeding.push_back(b);//if , iffalse can not be the last basicblock. so it is safe
				break;
			default:
				b->following.push_back(p->blocklist[i+1]);
				p->blocklist[i+1]->preceeding.push_back(b);// as is if and iffalse , it can't be the last basicblock.
		}
	}
}

void BasicBlock::find_usedef(){
	for(unsigned int i = 0 ; i < instrlist.size() ; i++){
		Quadruple *q = instrlist[i];
		switch(q->op){
			case I_COPY:
				{
					if(Arg_id * argid1 = dynamic_cast<Arg_id*>(q->arg1)){
						Id *id1 = argid1->id;
						std::unordered_set<Id*>::iterator it = def.find(id1);
						if(it == def.end())
							use.insert(id1);
					}
					Id * result = dynamic_cast<Id*>(((Arg_id*)(q->result))->id);
					std::unordered_set<Id*>::iterator it = use.find(result);
					if(it == use.end())
						def.insert(result);
					break;
				}
			case I_COPYIND:
				{
					if(Arg_id * argid2 = dynamic_cast<Arg_id*>(q->arg2)){
						Id *id2 = argid2->id;
						std::unordered_set<Id*>::iterator it = def.find(id2);
						if(it == def.end())
							use.insert(id2);
					}
					Id * result = dynamic_cast<Id*>(((Arg_id*)(q->result))->id);
					std::unordered_set<Id*>::iterator it = use.find(result);
					if(it == use.end())
						def.insert(result);
					break;
				}
			case I_INDCOPY:
				{
					if(Arg_id * argid1 = dynamic_cast<Arg_id*>(q->arg1)){
						Id *id1 = argid1->id;
						std::unordered_set<Id*>::iterator it = def.find(id1);
						if(it == def.end())
							use.insert(id1);
					}
					if(Arg_id * argid2 = dynamic_cast<Arg_id*>(q->arg2)){
						Id *id2 = argid2->id;
						std::unordered_set<Id*>::iterator it = def.find(id2);
						if(it == def.end())
							use.insert(id2);
					}
					break;
				}
			case I_MULT:
			case I_ADD:
			case I_DIV:
			case I_MINUS:
				{
					if(Arg_id * argid1 = dynamic_cast<Arg_id*>(q->arg1)){
						Id *id1 = argid1->id;
						std::unordered_set<Id*>::iterator it = def.find(id1);
						if(it == def.end())
							use.insert(id1);
					}
					if(Arg_id * argid2 = dynamic_cast<Arg_id*>(q->arg2)){
						Id *id2 = argid2->id;
						std::unordered_set<Id*>::iterator it = def.find(id2);
						if(it == def.end())
							use.insert(id2);
					}
					Id * result = dynamic_cast<Id*>(((Arg_id*)(q->result))->id);
					std::unordered_set<Id*>::iterator it = use.find(result);
					if(it == use.end())
						def.insert(result);
					break;
				}
			case I_IF:
			case I_IFFALSE:
				{
					Rel * r = ((Arg_rel*)q->arg1)->relation;
					if(Id * id1 = dynamic_cast<Id*>(r->e1)){
						std::unordered_set<Id*>::iterator it = def.find(id1);
						if(it == def.end())
							use.insert(id1);
					}
					if(Id * id2 = dynamic_cast<Id*>(r->e2)){
						std::unordered_set<Id*>::iterator it = def.find(id2);
						if(it == def.end())
							use.insert(id2);
					}
					break;
				}
			case I_WRITE:
				{
					if(Arg_id * argid2 = dynamic_cast<Arg_id*>(q->arg2)){
						Id *id2 = argid2->id;
						std::unordered_set<Id*>::iterator it = def.find(id2);
						if(it == def.end())
							use.insert(id2);
					}
					break;
				}
			case I_READ:
			case I_CALLFUNC:
				{
					Id * result = dynamic_cast<Id*>(((Arg_id*)(q->result))->id);
					std::unordered_set<Id*>::iterator it = use.find(result);
					if(it == use.end())
						def.insert(result);
					break;
				}
			case  I_PARAM:
				{
					if(Arg_id * argid1 = dynamic_cast<Arg_id*>(q->arg1)){
						Id *id1 = argid1->id;
						std::unordered_set<Id*>::iterator it = def.find(id1);
						if(it == def.end())
							use.insert(id1);
					}
					else if(Arg_ace* argace1 = dynamic_cast<Arg_ace*>(q->arg1)){
						Id *id1 = (Id*)argace1->ace->index;
						std::unordered_set<Id*>::iterator it = def.find(id1);
						if(it == def.end())
							use.insert(id1);
					}
					break;
				}
			case I_GOTO:
			case I_END:
			case I_INVOKE:
			case I_CALLPROC:
				;
		}
	}
}
					/*
					if(Arg_id * argid1 = dynamic_cast<Arg_id*>(q->arg1)){
						Id *id1 = argid1->id;
						std::unordered_set<Id*>::iterator it = def.find(id1);
						if(it == def.end())
							use.insert(id1);
					}
					if(Arg_id * argid2 = dynamic_cast<Arg_id*>(q->arg2)){
						Id *id2 = argid2->id;
						std::unordered_set<Id*>::iterator it = def.find(id2);
						if(it == def.end())
							use.insert(id2);
					}
					Id * result = dynamic_cast<Id*>(((Arg_id*)(q->result))->id);
					std::unordered_set<Id*>::iterator it = use.find(result);
					if(it == use.end())
						def.insert(result);
					*/
