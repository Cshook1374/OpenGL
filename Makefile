make: 
	gcc -g -o build/debug src/*.c src/engine/*.c -lOpenGL -lglfw -lGLEW

run:
	./build/debug

debug:
	gdb -tui build/debug