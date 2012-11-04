CXXFLAGS = -lglut -lGL -lGLU -ljpeg -lm -lpthread -lGLEW
LINKOBJ  = main.o bibutil.o importar.o
BIN  = space_ataque
OBJ  = main.o bibutil.o importar.o
RM = rm -f


$(BIN): $(OBJ)
	g++ $(LINKOBJ) -g -o space_ataque $(CXXFLAGS)
	chmod 777 $(BIN)
	
gdb: $(OBJ)
	g++ $(LINKOBJ) -g -o space_ataque $(CXXFLAGS)

main.o: main.cpp
	g++ -c main.cpp -g -o main.o $(CXXFLAGS)

bibutil.o: bibutil.cpp
	g++ -c bibutil.cpp -g -o bibutil.o $(CXXFLAGS)
	
importar.o: importar.c
	g++ -c importar.c -g -o importar.o $(CXXFLAGS)	
	
clean: 
	${RM} $(OBJ) $(BIN)	
 
#-O2 -L/usr/X11R6/lib -lm -lpthread -lX11
