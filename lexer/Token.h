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
		virtual std::string toString(){
			return "Token";
		}
};
class Num : public Token{
	public :
		int value;
		Num(int v) : Token(T_NUMBER) {value = v;}
		std::string toString() {return std::to_string(value);}
};
class Character : public Token{
	public:
		char character;
		Character(char c) : Token(T_CHARACTER){character = c;}
		std::string toString() {return to_string(character);}
};
class STring : public Token{
	public :
		std::string line;
		STring(std::string s) : Token(T_STRING){line = s;}
		std::string toString(){return line;}
};
class Word;
class Word : public Token{
	public:
		std::string lexeme;
		Word(std::string s, Tag tag) : Token(tag) {lexeme = s;}
		Word(char const *s, Tag tag) : Token(tag) {lexeme = s;}
		string toString() {return lexeme;}
		
};
#endif
