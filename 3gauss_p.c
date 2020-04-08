#include <iostream>
#include <time.h>
#include <omp.h>
#include <vector>
#include <cmath>
using namespace std;

bool search_reverse_matrix(vector <vector<double>> &matrix)
{
    int size = matrix.size();
    vector <vector<double>> E(size, vector<double>(size));

    //Заполнение единичной матрицы
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i == j) E[i][j] = 1.0;
            else E[i][j] = 0.0;
        }
    }

    //Трансформация исходной матрицы в верхнетреугольную
    for (int k = 0; k < size; k++)
    {
        if (abs(matrix[k][k]) < 1e-8)
        {
            bool changed = false;

            for (int i = k + 1; i < size; i++)
            {
                if (abs(matrix[i][k]) > 1e-8)
                {
                    swap(matrix[k], matrix[i]);
                    swap(E[k], E[i]);
                    changed = true;
                    break;
                }
            }

            if (!changed)
                return false;
        }

        double div = matrix[k][k];

#pragma omp parallel
        {
#pragma omp for
            for (int j = 0; j < size; j++)
            {
                matrix[k][j] /= div;
                E[k][j] /= div;
            }
        }

#pragma omp parallel
        {
#pragma omp for
            for (int i = k + 1; i < size; i++)
            {
                double multi = matrix[i][k];


                for (int j = 0; j < size; j++)
                {
                    matrix[i][j] -= multi * matrix[k][j];
                    E[i][j] -= multi * E[k][j];
                }
            }
        }
    }

    //Формирование единичной матрицы из исходной
    //и обратной из единичной
    for (int k = size - 1; k > 0; k--)
    {
#pragma omp parallel
    {
#pragma omp for
        for (int i = k - 1; i > -1; i--)
        {
            double multi = matrix[i][k];

            for (int j = 0; j < size; j++)
            {
                matrix[i][j] -= multi * matrix[k][j];
                E[i][j] -= multi * E[k][j];
            }
        }
    }
    }
    matrix = E;
    return true;
}

double random(const int min, const int max)
{
    if (min == max)
        return min;
    return min + rand() % (max - min);
}

int main(int argc, char *argv[])
{
	int N;
	N = atoi(argv[1]);
    

    vector<vector<double>> matrix(N, vector<double>(N));
    vector<double> B(N);
	ifstream f;
	f.open("matrix"+to_string(N)+".txt");
	for (i = 0; i<N; i++){
		for (j = 0; j < N; j++)
			f >> matrix[i][j];
	}

    // Вывод системы уравнений
    /*cout << "\nСистема уравнений:\n";
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {

            if (j != 0 && matrix[i][j] >= 0)
                cout << " +";
            cout << " " << matrix[i][j] << "x" << j + 1;
        }
        cout << " = " << B[i] << endl;
    }*/

	double start; double end;
	start = omp_get_wtime();
	
    // Вычисление обратной матрицы
    if (!search_reverse_matrix(matrix))
    {
        cout << "\nНевозможно найти обратную матрицу!\n";
        exit(1);
    }
	end = omp_get_wtime();
	printf("Work took %f sec. time.\n", end-start);
	ofstream res;
	res.open("result"+to_string(N)+argv[2]+".txt");

	

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			res<<matrix[i][j]<<" ";
		res<<endl;
	}

    // Вывод окончательного результата
    /*cout << "\nРешение системы уравнений:";
    for (int i = 0; i < N; i++)
        cout << "\nx" << i + 1 << " = " << X[i];*/

    return 0;
}