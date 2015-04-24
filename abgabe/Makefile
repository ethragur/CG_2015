CC = g++
CXXFLAGS =-g -Wall -Wextra 
OBJ = MerryGoRound.o LoadShader.o GraphicsObject.o Matrix.o

LDLIBS=-lm -lglut -lGLEW -lGL

MerryGoRound: $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f *.o MerryGoRound
.PHONY: all clean
