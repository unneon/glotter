MC_CPP=${CXX} -std=c++14 -Wall -Wextra -Wpedantic -Iinclude
MC_LFLAG=-lssl -luWS -lz -pthread

tests:
	$(MC_CPP) test/bfs/main.cpp -o test/bfs.out $(MC_LFLAG)
