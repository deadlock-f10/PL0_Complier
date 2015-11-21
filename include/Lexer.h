#include "Tag.h"
#include "Token.h"
#include <string>
#include <fstream>
#include <utility>
#include <unordered_map>
#define MAX_STR_LEN (2048)
#define MAX_WORD_LEN (2048)

#ifndef LEXER_H
#define LEXER_H
typedef std::unordered_map<std::string , Word*> hashtable;

class Lexer {
	public :
		static int line;         // not even a declaration!
		static int column;
		char peek = ' ';
		std::fstream *fs;
		hashtable words;
		Lexer(std::string str);
		Token* scan();
	private:
		void reserve(Word* w) {words.insert(std::make_pair(w->lexeme,w));}
		void readch(); 
		bool readch(char c);
};
#endif
