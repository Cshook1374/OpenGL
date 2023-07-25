make: 
	gcc -g -o build/debug src/*.c src/engine/*.c -lOpenGL -lglfw -lGLEW -lm

run:
	./build/debug 800 600

debug:
	gdb -tui build/debug