CXX=g++ -std=c++17
CXXFLAGS=-c -Wall -Wextra -Werror
STATICLIBNAME=libmatrix.a
SOURCES=matrix.cpp
OBJECTS=$(SOURCES:.cpp=.o)


all: $(STATICLIBNAME)

$(STATICLIBNAME): $(OBJECTS)
	ar rc $@ $(OBJECTS)
	ranlib $@

.cpp.o:
	$(CC) $(CPPFLAGS) $<

clean:
	rm -rf *.o

fclean: clean
	rm -rf libmatrix.a