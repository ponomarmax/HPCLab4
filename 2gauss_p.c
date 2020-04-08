#include <iostream> 
#include <cmath> 
#include <cstdlib> 
#include <ctime>  
#include <fstream>
#include <string>
#include <stdio.h>
#include <omp.h>
using namespace std;

void Gausss_parallel(double **matrix, int n, int m){
         /*cout << "Матрица: " << endl;
          for (int i = 0; i < n; i++)         {
             for (int j = 0; j < m; j++)
                 cout << matrix[i][j] << " ";
             cout << endl;
         }         cout << endl;*/
         double  tmp;
         int i, j, k;
         double dt, timein, timeout;
         timein = omp_get_wtime();
         omp_set_num_threads(4);
	for (i = 0; i < n; i++) {    
		tmp = matrix[i][i];
		for (j = n; j >= i; j--)
			matrix[i][j] /= tmp;  
#pragma omp parallel for private (j, k, tmp)    
		for (j = i + 1; j < n; j++) {
			tmp = matrix[j][i];
			for (k = n; k >= i; k--)
				matrix[j][k] -= tmp * matrix[i][k];
		}
	}
}

int main(int argc, char *argv[]) {
int result, i, j, N;
double **a;
N = atoi(argv[1]);
cout<<N;
if (N < 0){N *= -1;}
a = new double *[N];
for (i = 0; i < N; i++)
	a[i] = new double[N];


ifstream f;
f.open("matrix"+to_string(N)+".txt");
for (i = 0; i<N; i++){
	for (j = 0; j < N; j++)
	f >> a[i][j];
}
double start; double end;
start = omp_get_wtime();
Gausss_parallel(a, N, N);
end = omp_get_wtime();
printf("Work took %f sec. time.\n", end-start);
ofstream res;
res.open("result"+to_string(N)+argv[2]+".txt");

for (i = 0; i<N; i++){
	for (j = 0; j < N; j++) {
		res<<a[i][j]<<" ";
	}
	res<<endl;
}

return 0;

} 

