#include "../include/Optimizer.h"
#include "../include/Quadruple.h"
#include <vector>
#include <unordered_map>

class dagNode{
	public:
	std::vector<Arg_id*> attached_variable_list;
	dagNode(){;}
	virtual void pushid(Arg_id * i){
		unsigned int ind = 0;
		for(; ind < attached_variable_list.size(); ind++)
			if(attached_variable_list[ind]->id == i->id)
				return ;
		attached_variable_list.push_back(i);
	}
	void deleteid(Arg_id *i){
		for(std::vector<Arg_id*>::iterator it = attached_variable_list.begin() ; it != attached_variable_list.end() ; it++){
			if((*it)->id != i->id)
				continue;
			attached_variable_list.erase(it);
			break;
		}
	}
};
struct KeyEqual{
	bool operator()(Arg2* l, Arg2* r) const { 
		if(Arg_id* idl = dynamic_cast<Arg_id*>(l)){
			if(Arg_id* idr = dynamic_cast<Arg_id*>(r))
				return idl->id == idr->id;
			else
				return false;
		}
		else if(Arg_int* intl = dynamic_cast<Arg_int*>(l)){
			if(Arg_int* intr = dynamic_cast<Arg_int*>(r))
				return intl->value == intr->value;
			else
				return false;
		}
		else
			return false;
	}
};
struct Keyhash{
	std::size_t operator()(Arg2* a) const {
		if(Arg_id* idl = dynamic_cast<Arg_id*>(a))
			return (std::size_t)idl->id;
		else if(Arg_int* intl = dynamic_cast<Arg_int*>(a))
			return (std::size_t)intl->value;
		return 0;
	}
};
class interiorNode : public dagNode{
	public:
	OP op;
	int leftchild;
	int rightchild;
	interiorNode(OP o,int l,int r){leftchild = l ; op = o ; rightchild = r;}
};
class leafNode : public dagNode{
	public:
	Arg2* arg;
	leafNode(Arg2* a){arg = a;}
};
void BasicBlock::dag(){
	for(unsigned int i = 0; i != instrlist.size() ; i++){
		switch(instrlist[i]->op){
			case I_ADD:
			case I_MINUS:
			case I_DIV:
			case I_MULT:
			case I_COPY:
				i = dagtoquad(i)-1;
				break;
			default:
				daglized.push_back(instrlist[i]);
		}
	}
}

int BasicBlock::dagtoquad(int begin){
	std::vector<dagNode*> DAG;
	DAG.push_back(new dagNode());
	int num = 1;
	std::unordered_map<Arg2* , int,Keyhash,KeyEqual> nodetable;
	int a;
	int b;
	int c;
	Arg2 * arg;
	std::unordered_map<Arg2*,int,Keyhash,KeyEqual>::iterator it;
	unsigned int n = begin ;
	for(; n < instrlist.size(); n++){
		Quadruple *q = instrlist[n];
		bool find = false;
		if(q->op != I_ADD && q->op != I_COPY && q->op != I_MINUS && q->op != I_DIV && q->op != I_MULT)
			return n;
		if(dynamic_cast<Arg_id*>(q->arg1))
			arg = (Arg_id*)(q->arg1);
		else
			arg = (Arg_int*)(q->arg1);
		it = nodetable.find(arg);
		if(it == nodetable.end()){
            DAG.push_back(new leafNode(arg));
			if(dynamic_cast<Arg_id*>(arg))
				DAG[num]->pushid((Arg_id*)arg);
			a = num++;
			nodetable.insert(std::make_pair(arg,a));
		}
		else
			a = it->second;
		if(q->op != I_COPY){
			if(dynamic_cast<Arg_id*>(q->arg2))
				arg = (Arg_id*)(q->arg2);
			else
				arg = (Arg_int*)(q->arg2);
			it = nodetable.find(arg);
			if(it == nodetable.end()){
                DAG.push_back(new leafNode(arg));
				if(dynamic_cast<Arg_id*>(arg))
					DAG[num]->pushid((Arg_id*)arg);
				b = num++;
				nodetable.insert(std::make_pair(arg,b));
			}
			else
				b = it->second;
		}
		else
			b = 0;
        unsigned int j = 1 ;
		for(; j < DAG.size(); j++){
			if(interiorNode* intnode = dynamic_cast<interiorNode*>(DAG[j])){
				if(intnode->op == q->op && intnode->leftchild == a && intnode->rightchild == b){
					c = j;
					find = true;
					break;
				}
			}
		}
		if(j == DAG.size()){
			DAG.push_back(new interiorNode(q->op,a,b));
			c = num++;
		}
		arg = (Arg_id*)(q->result);
		it = nodetable.find(arg);
		if(it == nodetable.end()){
			nodetable.insert(std::make_pair(arg,c));
		}
		else{
			DAG[nodetable[arg]]->deleteid((Arg_id*)arg);
			nodetable[arg] = c;
		}
		DAG[c]->pushid((Arg_id*)arg);
		if(find == true){
			daglized.push_back(new Quadruple(nullptr,I_COPY,DAG[c]->attached_variable_list.front(),nullptr,(Arg_id*)arg));
		}
		else{
			if(dynamic_cast<Arg_id*>(q->arg1))
				q->arg1 = DAG[a]->attached_variable_list.front();
			if(dynamic_cast<Arg_id*>(q->arg2))
				q->arg2 = DAG[b]->attached_variable_list.front();
			daglized.push_back(q);
		}
	}
	return n;
}

