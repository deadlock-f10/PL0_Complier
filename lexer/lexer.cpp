#include "Tag.h"
#include <fstream>
#include <cctype>
#include <unordered_map>
#define MAX_STR_LEN (2048)
#define MAX_WORD_LEN (2048)
typedef unordered_map<std::String , Token> hashtable;
typedef std::make_pair<std:;String , Token> pair;

class Lexer {
	public :
		static int line = 1, column = 0;
		char peek = ' ';
		std::fstream fs;
		hashtable words;
		Lexer(std::fstream F);
		Token scan();
	private:
		void reserve(Word w) {words.insert(pair(w.lexeme,w));}
		void readch(); 
		bool readch(char c);
}

Lexer::Lexer(std::fstream F) {
			fs = F;
			reserve(new Word("of" , T_OF));
			reserve(new Word("const" , T_CONST));
			reserve(new Word("var" , T_VAR));
			reserve(new Word("procedure" , T_PROCEDURE));
			reserve(new Word("function" , T_FUNCTION));
			reserve(new Word("begin" , T_BEGIN));
			reserve(new Word("end" , T_END));
			reserve(new Word("if" , T_IF));
			reserve(new Word("then" , T_THEN));
			reserve(new Word("else" , T_ELSE));
			reserve(new Word("while" , T_WHILE));
			reserve(new Word("do" , T_DO));
			reserve(new Word("for" , T_FOR));
			reserve(new Word("downto" , T_DOWNTO));
			reserve(new Word("to" , T_TO));
			reserve(new Word("read" , T_READ));
			reserve(new Word("write" , T_WRITE));
			
			//reserve(new Type(T_CHAR));
			//reserve(new Type(T_INT));
}
inline void Lexer::readch(){
	peek = fs.get();
	/*if(peek == '\n'){
		line++;
		column = 0;
	}
	else
		column++;*/
}
bool Lexer::reach(char c){
	reach();
	if(peek != c)
		return false;
	peek = ' ';
	return true;
}

Token Lexer::scan(){
	for( ; ; readch()){
		if(peek == ' ' || peek == '\t')
			continue;
		else if(peek == '\n') 
			line++;
		else
			break;
	}
	switch (peek){
		case ':':
			if(readch('='))
				return Word.assign;
			else
				return Word.comma;
		case '<':
			peek = readch();
			if(peek == '>'){
				peek = ' ';
				return Word.ne;
			}
			else if(peek == '='){
				peek = ' ';
				return Word.le;
			}
			else {
				peek = ' ';
				return Word.lt;
			}
		case '>':
			if(readch('='))
				return Word.ge;
			else
				return Word.gt;
		case '=':
			return Word.eq;
		case '+':
			return Word.plus;
		case '-':
			return Word.minus;
		case '*':
			return Word.mult;
		case '/':
			return Word.div;
		case ';':
			return Word.semicolon;
		case '.':
			return Word.dot;
		case '(':
			return Word.openparenthesis;
		case ')':
			return Word.closeparenthesis;
		case '[':
			return Word.openbracket;
		case ']':
			return Word.closebracket;
		case ''':
			readch();		
			char c = peek;
			if(readch('''))
				return new Character(c);
			else
				throw new SingleQuoteException();
		default:
	}
	else if(peek == '"'){
		char str[MAX_STR_LEN];
		int i = 0;
		readch();
		while( peek == ' ' || peek == '!' || (peek >= '#' && peek <= '~')){
			str[i++] = peek;
			readch();
		}
		if(peek == '"')
			return new STring(new String(str , i));
		else
			throw new DoubleQuoteException();
	}
	else if(isdigit(peek)){
		int v = 0;
         do {
            v = 10*v + peek-'0'; 
			readch();
         } while(isdigit(peek));
		 return new Num(v);
	}
	else if(isalpha(peek)){
		char str[MAX_WORD_LEN];
		int i = 0;
		readch();
		do{
			str[i++] = peek;
			readch();
		}while(isalnum(peek));
		String s = new String(str,i);
		hashtable::const_iterator it = words.find(s);
		if(it != words.end())
			return it->second;
		Word w = new Word(s,T_IDENT);
		words[w.lexeme] = w;
		return w;
	}
	else 
		throw new WrongSymbolException();
}
