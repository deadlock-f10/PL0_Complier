#include "../include/Optimizer.h"
#include "../include/Quadruple.h"
#include "../include/Expr.h"
#include <vector>
#include <unordered_set>

void BasicBlock::elimideadcode(){
	std::unordered_set<Id*> liveinblock;
	liveinblock.clear();
	QuadList temp;
	temp.clear();
	std::unordered_set<Id*>::iterator it,it2;
	for(int i = instrlist.size() - 1; i >= 0 ; i--){
		Quadruple *q = instrlist[i];
		switch(q->op){
			case I_COPY:
				{
					Id * result = dynamic_cast<Id*>(((Arg_id*)(q->result))->id);
					it = liveinblock.find(result);
					it2 = Out.find(result);
					if(it == liveinblock.end() && it2 == Out.end()){
						if(dynamic_cast<Temp *>(result))
							;
						else
						{
							if(Arg_id * argid1 = dynamic_cast<Arg_id*>(q->arg1))
								liveinblock.insert(argid1->id);
							temp.push_back(q);
						}
					}
					else{
						if(Arg_id * argid1 = dynamic_cast<Arg_id*>(q->arg1))
							liveinblock.insert(argid1->id);
						temp.push_back(q);
					}
					break;
				}
			case I_COPYIND:
				{
					if(Arg_id * argid2 = dynamic_cast<Arg_id*>(q->arg2))
						liveinblock.insert(argid2->id);
					temp.push_back(q);
					break;
				}
			case I_INDCOPY:
			case I_MULT:
			case I_ADD:
			case I_DIV:
			case I_MINUS:
				{
					if(Arg_id * argid1 = dynamic_cast<Arg_id*>(q->arg1))
						liveinblock.insert(argid1->id);
					if(Arg_id * argid2 = dynamic_cast<Arg_id*>(q->arg2))
						liveinblock.insert(argid2->id);
					temp.push_back(q);
					break;
				}
			case I_IF:
			case I_IFFALSE:
				{
					Rel * r = ((Arg_rel*)q->arg1)->relation;
					if(Id * id1 = dynamic_cast<Id*>(r->e1))
						liveinblock.insert(id1);
					if(Id * id2 = dynamic_cast<Id*>(r->e2))
						liveinblock.insert(id2);
					temp.push_back(q);
					break;
				}
			case I_WRITE:
				{
					if(Arg_id * argid2 = dynamic_cast<Arg_id*>(q->arg2))
						liveinblock.insert(argid2->id);
					temp.push_back(q);
					break;
				}
			case I_PARAM:
				{
					if(Arg_id * arg1 = dynamic_cast<Arg_id*>(q->arg1))
							liveinblock.insert(arg1->id);
					else if(Arg_ace * ace = dynamic_cast<Arg_ace*>(q->arg1)){
							Id *index = (Id*)(ace->ace->index);
							liveinblock.insert(index);
					}
					temp.push_back(q);
					break;
				}
			case I_READ:
			case I_CALLPROC:
			case I_CALLFUNC:
			case I_GOTO:
			case I_END:
			case I_INVOKE:
				temp.push_back(q);
		}
	}
	instrlist.clear();
	for(int i = temp.size()-1; i >= 0 ; i--)
		instrlist.push_back(temp[i]);
	// insert backwards
}
