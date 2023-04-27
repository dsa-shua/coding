EXECUTABLE=main prod_con ref open_mp mat

all:
	clear
	g++ -std=c++11 -pthread -o main main.cc
	./main

clean: 
	rm -f $(EXECUTABLE)

run:
	g++ -std=c++11 -pthread -o main main.cc
	./main 1234567890 10

prod:
	g++ -std=c++11 -pthread -o prod_con prod-con.cc
	./prod_con 

mp:
	clear
	clang++ -Xpreprocessor -fopenmp -L/usr/local/opt/llvm/lib -lomp open_mp.cpp -o open_mp
	./open_mp | tee results.txt

mm:
	clear
	g++ -std=c++11 -pthread -o mat mat.cpp
	./mat

