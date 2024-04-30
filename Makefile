pagerank: main.cpp table.cpp table.h csr.h csr.cpp
	g++ -O3 -Wall -o pagerank main.cpp csr.cpp table.cpp --std=c++11
