test: clean
	make -C ./symboltable
	make -C ./inter
	make -C ./lexer test
	make -C ./parser test
clean:
	make -C lexer clean
	make -C parser clean
parser: clean 
	make -C parser 

