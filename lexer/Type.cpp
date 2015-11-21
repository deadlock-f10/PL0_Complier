#include "../include/Type.h"
#define Int_Width (4)
#define Char_Width (1)
Type* Type::Int = new Type(T_INT , Int_Width , "integer") ;
Type* Type::Char = new Type(T_CHAR , Char_Width , "char");  
Type* Type::max(Type *p1, Type *p2)
{
	if( !numeric(p1) || !numeric(p2))
		return nullptr;
	if(p1 == Type::Int || p2 == Type::Int )
		return Type::Int;
	else
		return Type::Char;
}
Type* Type::numeric(Type *t){
	if(t == Type::Int || t == Type::Char) 
		return true;
	else 
		false;
}
