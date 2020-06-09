CXX = g++
CXXFLAGS = -Wall -g -std=c++17 -mavx2


main: main.o collide_two_particles.o  eps_close.o \
	  hashing.o model_base.o model_dump.o model_parallel_collide.o \
	  myrandom.o particle.o thread_pool.o time_queue.o
	$(CXX) $(CXXFLAGS) -o main main.o collide_two_particles.o  eps_close.o \
  hashing.o model_base.o model_dump.o model_parallel_collide.o \
  myrandom.o particle.o thread_pool.o time_queue.o

main.o: box.h model.h eps_close.h

collide_two_particles.o: eps_close.h collide_two_particles.h

eps_close.o: eps_close.h

hashing.o: hashing.h

model_base.o: model.h

model_dump.o: model.h

model_parallel_collide.o: model.h

myrandom.o: myrandom.h

particle.o: default.h particle.h

thread_pool.o: thread_pool.h

time_queue.o: time_queue.h


clean :
	del *.o