#include "Tag.h"
class Num : public Token{
	public :
		const int value;
		Num(int v) : Token(T_Num) {value = v;}
		public String toString() {return std::to_string(value);}
}
