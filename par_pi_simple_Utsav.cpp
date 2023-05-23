#include <mpi.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
int main(int argc,char* argv[])
{
	int done = 0, npow, myid, numprocs, i, rc;
	double PI25DT = 3.141592653589793238462643;
	double mypi, pi, h, sum, x, a; 
	double start, end, startafterbcast;
	double baserun = 0.0;
	long int n;
	char nodescase;
	int nodes, ppn;
	std::string line;
	std::vector<std::string> segments, runtimesegment, nsegment;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid); 
	/* parallelization of pi= (1/n)* S i=1n 4/(1 +( (i-0.5)/n)2), where n is the accuracy term. */ 
	if (myid == 0)
	{
		nodescase = *argv[2];
		if (nodescase == 'a')
		{
			nodes = 1;
			ppn = numprocs;
		}
		else if (nodescase == 'b')
		{
			nodes = numprocs;
			ppn = 1;
		}
		else if (nodescase == 'c')
		{
			nodes = numprocs / 2;
			ppn = 2;
		}
		npow = atoi(argv[1]);
		n = pow(2, npow);
		start = MPI_Wtime();
	}
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	if (myid == 0)
	{
		startafterbcast = MPI_Wtime();
	}
	h = 1.0 / (double) n; 
	sum = 0.0;
	for (i = myid + 1; i <= n; i += numprocs)
		{
			x = h * ((double)i - 0.5);
			sum += 4.0 / (1.0 + x*x);
		}
	mypi = h * sum; 
	MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
	MPI_Barrier(MPI_COMM_WORLD);
	if (myid == 0)
	{
		end = MPI_Wtime();
		//extracting runtime of P=1
		if (numprocs > 1)
		{
			std::ifstream file;
			file.open("par_pi_op_simple_Utsav.txt");
			while (std::getline(file, line))
			{
				boost::split(segments, line, boost::is_any_of(","), boost::token_compress_on);
				//for (std::vector<std::string>::iterator i=segments.begin(); i<segments.end(); i++)
				//{
				//	std::cout << *i << std::endl;
				//}
				//std::cout << segments[0] << std::endl;
				//std::cout << segments[4] << std::endl;
				boost::split(nsegment, segments[4], boost::is_any_of("="), boost::token_compress_on);
				if (std::stoi(nsegment[1]) == npow)
				{
					//std::cout << "power=" << nsegment[1];
					boost::split(runtimesegment, segments[0], boost::is_any_of("="), boost::token_compress_on);
					baserun = std::stod(runtimesegment[1]);
					break;
				}
			}
			file.close();
		}
		std::ofstream fp;
		fp.open("par_pi_op_simple_Utsav.txt", std::ios_base::app);
		printf("\nOutput:\n");
		printf("Runtime = %f, Runtime after Broadcast= %f, pi = %.16f, Error = %.16f, n = %ld, nodes = %d, ppn = %d", end - start, end - startafterbcast, pi, fabs(pi - PI25DT), npow, nodes, ppn);
		fp << std::fixed << std::setprecision(16) << "Runtime=" << end - start << ",Runtime after Broadcast=" << end - startafterbcast << ",pi=" << pi << ",Error=" << fabs(pi - PI25DT) << ",n=" << npow << ",nodes=" << nodes << ",ppn=" << ppn;
		if (numprocs > 1)
		{
			std::cout << ",Speedup=" << baserun / (end - start) << ",Efficiency=" << baserun / (numprocs * (end - start)) << std::endl;
			fp << ",Speedup=" << baserun / (end - start) << ",Efficiency=" << baserun / (numprocs * (end - start)) << std::endl;
		}
		else
		{
			std::cout << std::endl;
			fp << std::endl;
		}
		fp.close();
	}
	MPI_Finalize();
} 
