#include "../include/PAF.h"

Program * Program::Null = nullptr;

Id Program::get(Token *w) {
	for(Env *e = this ; e != nullptr ; e = e->prev){
		Hashtable::const_iterator found = e->table.find(w).second; 
		if ( found != e->table.end()) 
			return found->second;
	}
	return nullptr;
}

