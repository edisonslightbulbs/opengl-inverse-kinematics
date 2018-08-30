CC = g++

CFLAGS = -g -std=c++11

LIBS = -lglut -lGL -lGLU -lm -lGLEW

SOURCES = main.cpp arm.cpp display.cpp

TARGET = inverse-kinematics

output:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)

clean: 
	rm -rf *o $(TARGET)
