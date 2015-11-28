#include "Token.h"
#ifndef TYPE_H
#define TYPE_H
class Type ;
class Type : public Word{
	public:
		int width;
		Type(Tag t , int w, std::string s) : Word(s,t) {width = w;}
		static Type *Int,*Char;
		static Type* max(Type *p1,Type *p2); 
		static bool numeric(Type *t);
};

class Array : public Type{
	public :
	Type *of ;
	int size = 1;
	Array(int sz , Type* t) : Type(T_ARRAY , sz*(t->width), "array")
	{of = t; size = sz;}
	std::string to_string(){
		return (lexeme + "[" + patch::to_string(size) + "]" + "of" + of->to_string());
	}
};
#endif
