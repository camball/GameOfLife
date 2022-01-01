compiler = clang++
binPath = bin

ifeq '$(compiler)' 'g++'
flags = --std=c++11 -Wall -o $(binPath)/GameOfLife
else ifeq '$(compiler)' 'clang++'
flags = -std=c++11 -o $(binPath)/GameOfLife
endif

all:
	$(compiler) $(flags) main.cpp

debug:
	@if [ '$(compiler)' = 'g++' ]; then\
		echo "$(compiler) -ggdb $(flags) main.cpp";\
		$(compiler) -ggdb $(flags) main.cpp;\
	elif [ '$(compiler)' = 'clang++' ]; then\
		echo "$(compiler) -Og -g $(flags) main.cpp";\
		$(compiler) -Og -g $(flags) main.cpp;\
	else\
		echo "Compiler \"$(compiler)\" not recognized... use g++ or clang++, or build without this makefile.";\
	fi