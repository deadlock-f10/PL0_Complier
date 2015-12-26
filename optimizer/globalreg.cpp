#include "../include/Optimizer.h"
#include "../include/PAF.h"
#include "../include/Expr.h"
#include <unordered_map> 
#include <iostream>
void Optimizer::globalregassign(Program *p){              // what if there is one variable or none at all?
	std::unordered_map<Id*,int> countmap;
	for(unsigned int i = 1 ; i < p->blocklist.size(); i++){
		BasicBlock *b = p->blocklist[i];
		for(Idset::iterator it = b->In.begin(); it != b->In.end(); it++){
			if(countmap.find(*it) == countmap.end())
				countmap.insert(std::make_pair(*it,1));
			else
				countmap[*it] += 1;
		}
	}
	Id * x = nullptr;
	int count = 0;
	for(std::unordered_map<Id*,int>::iterator it = countmap.begin() ; it != countmap.end(); it++){
		if(it->second <= count)
			continue;
		count = it->second;
		x = it->first;
	}
	countmap.erase(x);
	p->toa1 = x;
	x = nullptr;
	count = 0;
	for(std::unordered_map<Id*,int>::iterator it = countmap.begin() ; it != countmap.end(); it++){
		if(it->second <= count)
			continue;
		count = it->second;
		x = it->first;
	}
	p->toa2 = x;
	std::cout<<"\nin "<<p->beginlabel<<endl;
	if(p->toa1 != nullptr)
		std::cout<< "assign variable "<<p->toa1->to_string()<<" to global register a1"<<endl;
	if(p->toa2 != nullptr)
		std::cout<< "assign variable "<<p->toa2->to_string()<<" to global register a2"<<endl;
}
