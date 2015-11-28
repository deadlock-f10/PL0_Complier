#include "Tag.h"
#include <string>
#ifndef TOKEN_H
#define TOKEN_H
using namespace std;
class Token{
	public :
		Tag tag;
		Token (Tag i){
			tag = i;
		}
		virtual std::string to_string(){
			return "Token";
		}
};
class Num : public Token{
	public :
		int value;
		Num(int v) : Token(T_NUMBER) {value = v;}
		std::string to_string() {return patch::to_string(value);}
};
class Character : public Token{
	public:
		char character;
		int value;
		Character(char c) : Token(T_CHARACTER){character = c; value = character;}
		std::string to_string() {return patch::to_string(character);}
};
class STring : public Token{
	public :
		std::string line;
		STring(std::string s) : Token(T_STRING){line = s;}
		std::string to_string(){return line;}
};
class Word;
class Word : public Token{
	public:
		std::string lexeme;
		Word(std::string s, Tag tag) : Token(tag) {lexeme = s;}
		Word(char const *s, Tag tag) : Token(tag) {lexeme = s;}
		string to_string() {return lexeme;}
		static Word  *assign;
		static Word *Plus ; 
		static Word *Minus ; 
		static Word *Mult ; 
		static Word *Div ; 
		static Word *eq ; 
		static Word *gt ; 
		static Word *lt ; 
		static Word *le ; 
		static Word *ge ; 
		static Word *ne ; 
		static Word *openbracket ; 
		static Word *closebracket ; 
		static Word *openparenthesis ; 
		static Word *closeparenthesis ; 
		static Word *comma ; 
		static Word *colon ; 
		static Word *semicolon ; 
		static Word *dot ; 
		static Word *temp;
		
};
#endif
