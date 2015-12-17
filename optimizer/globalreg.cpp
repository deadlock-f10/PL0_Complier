#include "../include/Optimizer.h"
#include "../include/PAF.h"
#include "../include/Expr.h"
#include <unordered_map>

void Optimizer::globalregassign(Program *p){              // what if there is one variable or none at all?
	std::unordered_map<Id*,int> countmap;
	for(unsigned int i = 1 ; i < p->blocklist.size(); i++){
		BasicBlock *b = p->blocklist[i];
		for(Idset::iterator it = b->In.begin(); it != b->In.end(); it++){
			if(countmap.fine(*it) == countmap.end())
				countmap.insert(*it,1);
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
	int count = 0;
	for(std::unordered_map<Id*,int>::iterator it = countmap.begin() ; it != countmap.end(); it++){
		if(it->second <= count)
			continue;
		count = it->second;
		x = it->first;
	}
	p->toa2 = x;
}
