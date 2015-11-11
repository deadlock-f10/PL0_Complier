#include "Tag.h"

class Word : public Token{
	public:
		std::String lexeme;
		Word(String s, int tag) : Token(tag) {lexeme = s;}
		String toString() {return lexeme;}
		static const Word 
		assign = new Word(":=",T_ASSIGN), 
		plus = new Word("+",T_PLUS),
		minus = new Word("-",T_MINUS),
		mult= new Word("*",T_MULT),	
		div= new Word("/",T_DIV),	
		eq= new Word("=",T_EQ),	
		gt= new Word(">",T_GT),	
		lt= new Word("<",T_LT),	
		le= new Word("<=",T_LE),	
		ge= new Word(">=",T_GE),	
		ne= new Word("<>",T_NE),	
		openparenthesis= new Word("(",T_OPENPARENTHESIS),	
		closeparenthesis= new Word(")",T_CLOSEPARENTHESIS),	
		openbracket= new Word("[",T_OPENBRACKET),	
		closebracket= new Word("]",T_CLOSEBRACKET),	
		comma= new Word(":",T_COMMA),	
		semicolon= new Word(";",T_SEMICOLON),	
		dot= new Word(".",T_DOT);	
}
