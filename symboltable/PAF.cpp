#include "../include/PAF.h"

Program * Program::Null = nullptr;

Node* Program::get(Token *w) {
	for(Program *e = this ; e != nullptr ; e = e->prev){
		Hashtable::const_iterator found = e->symboltable.find(w); 
		if ( found != e->symboltable.end()) 
			return found->second;
	}
	return nullptr;
}
