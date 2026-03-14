all:
	g++ main.cpp -I src/include -I ext/glm -L src/lib -o main \
	-lSDL3 -lmingw32 -lSDL3_image -lSDL3_mixer \
	-static-libgcc -static-libstdc++