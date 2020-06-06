CXX = g++
CXXFLAGS = -Wall -g -std=c++17 -mavx2 -O2

DD = default.h

main: model.o collide_two_particles.o eps_close.o \
	  myrandom.o particle.o
	$(CXX) $(CXXFLAGS) -o main model.o collide_two_particles.o \
	eps_close.o \
	myrandom.o particle.o

model.o: default.h collide_two_particles.h box.h

particle.o: default.h myrandom.h box.h particle.h

collide_two_particles.o: eps_close.h collide_two_particles.h

eps_close.o: eps_close.h

myrandom.o: myrandom.h

clean :
	del *.o