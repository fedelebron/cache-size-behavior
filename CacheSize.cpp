#include <iostream>
#include <random>
#include <cstdlib>
#include <limits>
#include <functional>
#include <chrono>
#include <memory>

using namespace std;
using namespace std::chrono;

void print_matrix(char name, int* M, int n) {
  cout << name << ":" << endl;
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      cout << M[i*n+j] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " n s" << endl
         << "n: matrix size" << endl << "s: block size" << endl;
    return -1;
  }
  default_random_engine re;
  uniform_int_distribution<int> dist(0, /*numeric_limits<int>::max()*/10);
  
  int n = atoi(argv[1]),
      s = atoi(argv[2]);

  if (n <= 0 || s <= 0 || n % s) {
    cerr << "Requirements: n > 0, s > 0, n % s == 0" << endl;
    return -1;
  }

  function<int()> rnd = bind(dist, re);
  unique_ptr<int[]> A(new int[n*n]);
  unique_ptr<int[]> B(new int[n*n]);
  unique_ptr<int[]> C(new int[n*n]);
  unique_ptr<int[]> D(new int[n*n]);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) { 
      A[i*n + j] = rnd();
      B[i*n + j] = rnd();
      C[i*n + j] = 0;
      D[i*n + j] = 0;
    }
  }
  typedef high_resolution_clock clock;
  clock::time_point t1 = clock::now();
  for(int s1 = 0; s1 < n; s1 += s)
    for(int s2 = 0; s2 < n; s2 += s)
      for(int s3 = 0; s3 < n; s3 += s)
        for(int i = s1; i < s1 + s; ++i)
          for(int j = s2; j < s2 + s; ++j)
            for(int k = s3; k < s3 + s; ++k)
              C[i*n+j] += A[i*n+k] * B[k*n+j];

  clock::time_point t2 = clock::now();
  cout << duration_cast<milliseconds>(t2-t1).count() << " milliseconds." << endl;
#ifdef ECHO
  for(int i = 0; i < n; ++i)
    for(int j = 0; j < n; ++j)
      for(int k = 0; k < n; ++k)
        D[i*n+j] += A[i*n+k] * B[k*n+j];

  print_matrix('A', A.get(), n);
  print_matrix('B', B.get(), n);
  print_matrix('C', C.get(), n);
  print_matrix('D', D.get(), n);
#endif
}