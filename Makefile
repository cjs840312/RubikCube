all : cube state

cube : obj/cube.o obj/File.o obj/Proof.o obj/Solver.o
	g++ $^ -o bin/$@

obj/cube.o : src/main/cube.cpp src/main/shortTurn.h 
	g++ -c $< -o $@

obj/File.o: src/sat/File.cpp
	g++ -c $< -o $@

obj/Proof.o: src/sat/Proof.cpp
	g++ -c $< -o $@

obj/Solver.o : src/sat/Solver.cpp src/sat/Solver.h
	g++ -c $< -o $@

state : src/main/state_maker.cpp src/main/shortTurn.h
	g++ $< -o bin/$@

clean :
	rm -f bin/* obj/*