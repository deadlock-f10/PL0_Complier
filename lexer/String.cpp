#include "Tag.h"

class STring : public Token{
	public :
		String line;
		STring(String s) : Token(T_STRING){line = s;}
		String toString(){return line;}
}
