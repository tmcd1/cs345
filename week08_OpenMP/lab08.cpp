/***********************************************************************
 * Program:
 *    Week 08, OpenMP Lab
 * Author:
 *    Ted McDaniel
 * Summary:
 *    Matrix Multiplicaiton with OpenMP
 ************************************************************************/

#include <iostream>    // for COUT and CERR
#include <fstream>     // for IFSTREAM
#include <pthread.h>   // for PTHREAD_T and the PTHREAD functions
#include <iomanip>     // for SETW
#include <cassert>     // because I am paranoid
using namespace std;

/***********************************************************
 * MATRIX MULTIPLICATION
 * A data-structure to contain the notion of matrix multiplication
 **********************************************************/
struct MatrixMultiplication
{
   int * arrayDestination = NULL;   // destination array
   int * arrayRHS = NULL;           // right-hand-side
   int * arrayLHS = NULL;           // left-hand-side

   int numRowRHS;
   int numColRHS;
   int numRowLHS;
   int numColLHS;
};

// This variable represents the shared memory between the parent thread
// and all the children. Recall from the reading how the main way for
// threads to communicate is through shared memory. This shared memory
// needs to be global.
MatrixMultiplication matrix;

bool readMatrix(const char * fileName,
                int * & array,
                int & numRow,
                int & numCol);
bool multiply(MatrixMultiplication & m, int);
void display( MatrixMultiplication & m);

/*************************************************
 * MAIN
 * Turn the input parameters into calls to threads
 ************************************************/
int main(int argc, char ** argv)
{
   int numRow;
   int numCol;

   // Request # of threads from user
   bool iFeelLikeIt = true;
   unsigned int numThreads;
   while (iFeelLikeIt)
   {
      cout << "How many threads? ";
      cin >> numThreads;

      if (numThreads < 65535 && numThreads > 0)
         break;
      else
         cout << "Invalid number of threads. Try again.\n";
   }

   // Check to make sure OpenMP is defined
//   #ifdef _OPENMP
//      cout << "_OPENMP defined\n";
//   #else
//      cout << "_OPENMP not defined\n";
//   #endif

   // make sure we have the proper number of parameters
   if (argc != 3)
   {
      cout << "Usage:\n\t" << argv[0] << "  left-hand-side  right-hand-side\n";
      return 1;
   }

   // read the LHS
   if (!readMatrix(argv[1], matrix.arrayLHS, numRow, numCol))
   {
      cout << "Unable to open matrix file " << argv[1] << endl;
      return 1;
   }
   matrix.numRowLHS = numRow;
   matrix.numColLHS = numCol;

   // read the RHS
   if (!readMatrix(argv[2], matrix.arrayRHS, numRow, numCol))
   {
      cout << "Unable to open matrix file " << argv[1] << endl;
      return 1;
   }
   matrix.numRowRHS = numRow;
   matrix.numColRHS = numCol;

   // multiply
   if (!multiply(matrix, numThreads))
      cout << "Unable to perform matrix multiplication\n";
   else
      display(matrix);

   // free
   delete [] matrix.arrayLHS;
   delete [] matrix.arrayRHS;
   delete [] matrix.arrayDestination;
   return 0;
}

/*******************************************************
 * READ MATRIX
 * Read a matrix from a file (fileName) and put the results
 * into an array (array). Return false if anything bad happens
 ******************************************************/
bool readMatrix(const char * fileName,
                int * & array,
                int & numRow,
                int & numCol)
{
   numRow = 0;
   numCol = 0;
   
   // open the file
   ifstream fin(fileName);
   if (fin.fail())
      return false;
   
   // read the file
   fin >> numCol >> numRow;
   if (numRow < 1 || numCol < 1)
   {
      fin.close();
      return false;
   }
   
   array = new int[numRow * numCol];
   for (int r = 0; r < numRow; r++)
      for (int c = 0; c < numCol; c++)
         fin >> array[r * numCol + c];

   if (fin.fail())
   {
      fin.close();
      return false;
   }
   
   // close the file
   fin.close();
   return true;
}

/**************************************************
 * MULTIPLY
 * Perform matrix multiplication
 *************************************************/
bool multiply(MatrixMultiplication & m, int numThreads)
{
   int numRowDes = m.numRowLHS;
   int numColDes = m.numColRHS;

   int lhsCols = m.numColLHS;

   m.arrayDestination = new int[numRowDes * numColDes];
   for (int row = 0; row < numRowDes; row++)
      for (int col = 0; col < numColDes; col++)
         m.arrayDestination[row * numColDes + col] = 0;
   
   if (m.numRowRHS != m.numColLHS)
      return false;

   /////////////////////////////
   // Your code goes here
   for (int i = 0; i < numRowDes; i++)
   {
      for (int j = 0; j < numColDes; j++)
      {
         #pragma omp parallel for num_threads(numThreads)
         for (int k = 0; k < lhsCols; k++)
         {
            // I got help from this site: bit.ly/cppMatrixMult
            m.arrayDestination[i*numRowDes + j] += m.arrayLHS[i*numRowDes + k] * m.arrayRHS[k*lhsCols + j];
         }
      }
   }
   // Your code goes here
   ////////////////////////////
   
   return true;
}

/******************************************************
 * DISPLAY
 * Display the entire matrix operation
 *****************************************************/
void display(MatrixMultiplication & m)
{
   int maxRow = (m.numRowRHS > m.numRowLHS) ? m.numRowRHS : m.numRowLHS;
   int numRowDes = m.numRowLHS;
   int numColDes = m.numColRHS;

   for (int row = 0; row < maxRow; row++)
   {
      // Left-hand-side
      if (row < m.numRowLHS)
      {
         cout << "|";
         for (int col = 0; col < m.numColLHS; col++)
            cout << setw(3) << m.arrayLHS[row * m.numColLHS + col];
         cout << "  |";
         
      }
      else
         cout << setw(3 * m.numColLHS + 4) << " ";
      
      // multiply symbol or space
      cout << ((maxRow - 1) / 2 == row ? " X " : "   ");

      // Right-hand-side
      if (row < m.numRowRHS)
      {
         cout << "|";
         for (int col = 0; col < m.numColRHS; col++)
            cout << setw(3) << m.arrayRHS[row * m.numColRHS + col];
         cout << "  |";
      }
      else
         cout << setw(3 * m.numColRHS + 4) << " ";
      
      // equals
      cout << ((maxRow - 1) / 2 == row ? " = " : "   ");

      // result
      if (row < numRowDes)
      {
         cout << "|";
         for (int col = 0; col < numColDes; col++)
            cout << setw(4) << m.arrayDestination[row * numColDes + col];
         cout << "   |";
      }
      
      // endl at the end of the line
      cout << endl;
   }
}
