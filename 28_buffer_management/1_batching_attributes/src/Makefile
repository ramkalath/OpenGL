CC = g++

CFLAGS = -std=c++0x -g -Wall

INCLUDE = -I/usr/local/include/GLFW -I/usr/include/SOIL

LIBS = -lglfw3 -lm -lGLEW -lGL -lGLU -lSOIL -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread -lassimp

# enter filename without extension
FILENAME1 = main
FILENAME2 = ../include/shader
FILENAME3 = ../include/modelloader
FILENAME4 = ../include/gamesettings

all: $(FILENAME4).cpp $(FILENAME3).cpp $(FILENAME2).cpp $(FILENAME1).cpp
	@$(CC) $(CFLAGS) $(INCLUDE) $(FILENAME4).cpp $(FILENAME3).cpp $(FILENAME2).cpp $(FILENAME1).cpp -o ../bin/$(FILENAME1) $(LIBS)

run:
	@../bin/./$(FILENAME1)	

clean:
	rm ../bin/./$(FILENAME1) 

