all: dave

dave: main.cpp
	g++ -g main.cpp -o dave `allegro-config --cflags --libs`

clean: 
	rm -rf dave
	
