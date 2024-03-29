ifdef SYSTEMROOT
   RM = del /Q
   FixPath = $(subst /,\,$1)
else
   ifeq ($(shell uname), Linux)
      RM = rm -f
      FixPath = $1
   endif
endif

all:
	mkdir -p build
	g++ -c -g3 -o build/fem.o src/fem.cpp
	g++ -c -g3 -o build/solver.o src/solver.cpp
	g++ -c -g3 -o build/mesh.o src/mesh.cpp
	g++ -c -g3 -o build/OpenNL_psm.o third_party/OpenNL_psm.c
	g++ -c -g3 -o build/main.o main.cpp
	g++ -o build/fem2a build/fem.o build/mesh.o build/solver.o build/main.o build/OpenNL_psm.o
clean:
	rm -rf *.o    
