SSproj: FileRel.o KodoviQ.o OpCodeTable.o OpUlaz.o rad.o RelocTable.o SymTable.o SymUlaz.o main.o
	g++ -o SSproj -std=c++0x -g FileRel.o KodoviQ.o OpCodeTable.o OpUlaz.o rad.o RelocTable.o SymTable.o SymUlaz.o main.o

FileRel.o: FileRel.cpp FileRel.h KodoviQ.h RelocTable.h SymTable.h
	g++ -c -std=c++0x -g FileRel.cpp

KodoviQ.o: KodoviQ.cpp KodoviQ.h 
	g++ -c -std=c++0x -g KodoviQ.cpp

main.o: main.cpp rad.h
	g++ -c -std=c++0x -g main.cpp

OpCodeTable.o: OpCodeTable.cpp OpCodeTable.h OpUlaz.h
	g++ -c -std=c++0x -g OpCodeTable.cpp

OpUlaz.o: OpUlaz.cpp OpUlaz.h
	g++ -c -std=c++0x -g OpUlaz.cpp

rad.o: rad.cpp rad.h SymTable.h OpCodeTable.h RelocTable.h KodoviQ.h FileRel.h
	g++ -c -std=c++0x -g rad.cpp

RelocTable.o: RelocTable.cpp RelocTable.h
	g++ -c -std=c++0x -g RelocTable.cpp

SymTable.o: SymTable.cpp SymTable.h SymUlaz.h
	g++ -c -std=c++0x -g SymTable.cpp

SymUlaz.o: SymUlaz.cpp SymUlaz.h
	g++ -c -std=c++0x -g SymUlaz.cpp

clean:
	rm SymUlaz.o
	rm OpUlaz.o
	rm KodoviQ.o
	rm SymTable.o
	rm OpCodeTable.o
	rm RelocTable.o
	rm FileRel.o
	rm rad.o
	rm main.o
	rm SSproj
