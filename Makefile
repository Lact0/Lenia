all:
	g++ -ISDL2/include -LSDL2/lib -o main src/main.cpp src/growth.cpp src/kernal.cpp -lmingw32 -lSDL2main -lSDL2
#g++ -no-pie -ISDL2/include -LSDL2/lib -o main src/main.cpp src/growth.cpp src/kernal.cpp -lmingw32 -lSDL2main -lSDL2 -pg