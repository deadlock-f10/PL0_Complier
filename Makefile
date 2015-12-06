all: 
	make -C ./symboltable
	make -C ./inter
	make -C ./lexer 
	make -C ./parser 
	make -C ./optimizer 
	make -C codegenerator test
clean:
	make -C lexer clean
	make -C ./symboltable clean
	make -C ./inter clean
	make -C parser clean
	make -C ./optimizer clean 
	make -C codegenerator clean 
