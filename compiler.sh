g++ -c main.cpp -I./SFML/include
g++ main.o -o bin/app -L./SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
export LD_LIBRARY_PATH=./SFML/lib
./bin/app