#include "Tag.h"

class Character : public Token{
	public:
		int character;
		Character(char c) : Token(T_CHARACTER){character = c;}
		String toString() {return String::to_string(character);}
}
