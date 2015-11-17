CC = g++
CFLAGS = -Wall -Werror --std=c++11

BUILDDIR = ../obj/lexer/

LEXER_C = Lexer.cpp Type.cpp 
LEXER_TEST = ../test/test_lexer.cpp
HEADER_PATH = ../include
LEXER_H = $(HEADER_PATH)/Lexer.h  $(HEADER_PATH)/Tag.h $(HEADER_PATH)/Token.h $(HEADER_PATH)/Type.h

OBJS = $(patsubst %.cpp,$(BUILDDIR)%.o,$(LEXER_C))

#all: Makefile $(OBJS)
#all: $(OBJS)

#.cpp.o:
#	$(CC) $(CFLAGS) -c $< -o $@

#$(BUILDDIR)$(LEXER_C:.cpp=.o): $(LEXER_C)
#	$(CC) $(CFLAGS) -c -o $(BUILDDIR)$(LEXER_C:.cpp=.o)

$(BUILDDIR)%.o: %.cpp
	g++ $(CFLAGS) -c $< -MD -o $@

test: $(OBJS)
	$(CC) $(CFLAGS) $(LEXER_TEST) $(OBJS) -o lexer_test
clean:
	rm -f $(OBJS) *~ *.o lexer_test

depend: $(LEXER_C)
	makedepend $(LEXER_H) $^
# DO NOT DELETE

../include/Lexer.o: ../include/Tag.h ../include/Token.h
../include/Token.o: ../include/Tag.h
../include/Type.o: ../include/Token.h ../include/Tag.h
Lexer.o: ../include/Lexer.h ../include/Tag.h ../include/Token.h
Lexer.o: ../include/Type.h
Type.o: ../include/Type.h ../include/Token.h ../include/Tag.h
