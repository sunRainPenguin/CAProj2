MIPSsim.exe: MIPSsim.o instruction.o Simulator.o code.o data.o Util.o
	g++ MIPSsim.o instruction.o Simulator.o code.o data.o Util.o -o MIPSsim

MIPSsim.o: MIPSsim.cpp instruction.h Simulator.h code.h data.h Util.h
	g++ -c MIPSsim.cpp

instruction.o: instruction.cpp instruction.h code.h data.h Util.h
	g++ -c instruction.cpp

Simulator.o: Simulator.cpp Simulator.h code.h data.h Util.h
	g++ -c Simulator.cpp

code.o: code.cpp code.h const.h
	g++ -c code.cpp

data.o: data.cpp data.h
	g++ -c data.cpp

Util.o: Util.cpp Util.h
	g++ -c Util.cpp

clean:
	rm -v  MIPSsim *.o disassembly.txt 
