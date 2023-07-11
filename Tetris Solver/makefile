all: clean compile run

compile: driver1.cpp driver2.cpp util.cpp driver1.hpp driver2.hpp
	@echo "Compiling Test driver."
	@g++ -o test1 driver1.cpp util.cpp
	@echo "Compiling Game driver."
	@g++ -o test2 driver2.cpp util.cpp
 
run:
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "........Running Test Driver........"
	@./test1
	@echo "\n~~~~~~~~~~~~~~Test Output Is Above~~~~~~~~~~~~~~~\n\n"
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "........Running The Game........\n"
	@./test2

clean:
	@echo "Removing compiled files.."
	@rm -f *.o
	@rm -f test1
	@rm -f test2
