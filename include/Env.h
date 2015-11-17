#include "Token.h"
#include <utility>
#include <unordered_map>

typedef std::unordered_map<Token , Id> Hashtable;


class Env{
	private:
		Hashtable table;
		Env *prev;
	public :
		Env(Env *prev){this.prev = prev;}
		void put(Token t , Id i){table.insert(std::make_pair(t,i))}
		Id get(Token w); {
			for(Env *e = this ; e != nullptr ; e = e->prev){
				Hashtable::const_iterator found = e->table.find(w).second; 
				if ( found != e->table.end()) 
					return found->second;
			}
			return nullptr;
		}
}
