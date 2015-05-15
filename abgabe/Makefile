CC = g++
CXXFLAGS =-g -Wall -Wextra 
OBJ = MerryGoRound.o InitClass.o camera.o tiny_obj_loader.o GraphicsObject.o objloader.o

LDLIBS=-lm -lglut -lGLEW -lGL

MerryGoRound: $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f *.o MerryGoRound
.PHONY: all clean
