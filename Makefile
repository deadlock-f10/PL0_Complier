all: 
	make -C ./symboltable
	make -C ./inter
	make -C ./lexer test
	make -C ./parser test
	make -C ./optimizer test
clean:
	make -C lexer clean
	make -C ./symboltable clean
	make -C ./inter clean
	make -C parser clean
	make -C ./optimizer clean 

