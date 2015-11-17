#include "Type.h"
#define Int_Width (4)
#define Char_Width (1)
Type* Type::Int = new Type(T_INT , Int_Width , "integer") ;
Type* Type::Char = new Type(T_CHAR , Char_Width , "char");  
Type* Type::max(Type *p1, Type *p2)
{
	if(p1 == Type::Int || p2 == Type::Char )
		return Type::Int;
	else return Type::Char;
}
