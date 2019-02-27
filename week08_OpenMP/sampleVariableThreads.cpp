/***********************************************************************
 * Program:
 *    Sample 08, Variable Number of Threads
 * Author:
 *    Br. Helfrich
 * Summary:
 *    This program will demonstrate how to create a variable
 *    number of threads using OpenMP
 *
 *    This example will sum the factorals. For example, if the use
 *    wishes to see the sum of the first 5 factorals, it will compute:
 *      1! + 2! + 3! + 4! + 5!
 *
 *    Note that you must compile this with the -lpthread switch:
 *        g++ -fopenmp sampleVariableThreads.cpp
 ************************************************************************/

#include <iostream>     // for COUT and CIN
#include <unistd.h>     // for USLEEP
#include <omp.h>        // for OpenMP
#include <time.h>       // for CLOCK
using namespace std;

/**********************************************
 * COMPUTE FACTORALS
 * Compute the first n factorals
 *********************************************/
void computeFactorals(const int numFactorals,
                      double * array,
                      const int numThreads)
{
   array[0] = 0;
#pragma omp parallel for num_threads(numThreads) \
   shared(array) 
   for (int iFactoral = 1; iFactoral < numFactorals; iFactoral++)
   {
      array[iFactoral] = 1;
      for (int i = 1; i <= iFactoral; i++)
      {
         usleep(5000);
         array[iFactoral] *= i;
      }
   }
   
#ifdef _OPENMP
   cout << "_OPENMP defined, threadct = " << numThreads << endl;
#else
   cout << "_OPENMP not defined" << endl;
#endif  
}

/*********************************************
 * SUM ARRAY
 * Compute the sum of the array
 ********************************************/
double sumArray(double * array, int num)
{
   double sum = 0;
   for (int i = 0; i < num; i++)
      sum += array[i];
   return sum;
}

/*********************************************
 * MAIN
 * Prompt the user for the variables
 *********************************************/
int main()
{
   // prompt for number of threads
   int numThreads;
   cout << "How many threads?   ";
   cin  >> numThreads;

   // prmopt for the number of factorals
   int numFactorals;
   cout << "How many factorals? ";
   cin  >> numFactorals;
   double * array = new double[numFactorals + 1];

   // display the results
   computeFactorals(numFactorals + 1, array, numThreads);
   cout.setf(ios::fixed | ios::showpoint);
   cout.precision(0);
   cout << "The sum is: "
        << sumArray(array, numFactorals + 1)
        << endl;

   delete [] array;
   return 0;
}
   
