CXXFLAGS = -lglut -lGL -lGLU -ljpeg -lX11
LINKOBJ  = main.o bibutil.o importar.o
BIN  = space_ataque
OBJ  = main.o bibutil.o importar.o
RM = rm -f


$(BIN): $(OBJ)
	g++ $(LINKOBJ) -o space_ataque $(CXXFLAGS)

main.o: main.cpp
	g++ -c main.cpp -o main.o $(CXXFLAGS)

bibutil.o: bibutil.cpp
	g++ -c bibutil.cpp -o bibutil.o $(CXXFLAGS)
	
importar.o: importar.c
	g++ -c importar.c -o importar.o $(CXXFLAGS)	
	
clean: 
	${RM} $(OBJ) $(BIN)	
