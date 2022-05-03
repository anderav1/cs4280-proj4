# Lexi Anderson
# CS4280, Proj 4
# Makefile


CC = g++
CFLAGS = -g -Wall -std=c++11
OBJS = main.o parser.o scanner.o semantics.o generateCode.o list.o
DEPS = parser.h scanner.h semantics.h generateCode.h list.h node.h token.h
TARGET = genCode

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
  
$(OBJS): %.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -o $@ -c $<
  

# clean up generated files
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)
  
  
