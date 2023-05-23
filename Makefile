all: program

program: par_pi_simple_Utsav par_pi_Utsav

par_pi_simple_Utsav: par_pi_simple_Utsav.cpp
	mpicc -o par_pi_simple_Utsav par_pi_simple_Utsav.cpp -lm -lstdc++
par_pi_Utsav: par_pi_Utsav.cpp
	mpicc -o par_pi_Utsav par_pi_Utsav.cpp -lm -lstdc++