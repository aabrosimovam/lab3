#include "matrix.h"

Matrix::Matrix()
{
    m = 0;
    n = 0;
}

Matrix::Matrix(int M, int N, float array[])
{
    m = M;
    n = N;
    arr = new float*[m];
    for(int i=0;i<m;i++)
    {
        arr[i] = new float[n];
        for(int j=0;j<n;j++)
        {
            arr[i][j] = array[i*n+j];
        }
    }

}

Matrix::Matrix(const Matrix & M)
{
    m = M.m;
    n = M.n;
    float** tmp_arr = M.arr;
    arr = new float*[m];
    for(int i =0; i <m;i++)
    {
        arr[i] = new float[n];
        for(int j=0;j<n;j++)
        {
            arr[i][j] = tmp_arr[i][j];
        }
    }
}

Matrix &Matrix::operator=(const Matrix &M)
{
    arr = M.arr;
    m = M.m;
    n = M.n;
    return *this;
}

float **Matrix::GetArr() const
{
    return arr;
}


bool Matrix::Multiply(const Matrix& M)
{
    if (n == M.m)
    {
       float** array = M.arr;
       float** matr= new float*[m];
       for(int i=0;i<m;i++)
       {
           matr[i]=new float[M.n];
           for(int j=0;j<M.n;j++)
           {
               matr[i][j]=0;
               for (int k = 0; k < n; k++)
               {
                matr[i][j] += arr[i][k] * array[k][j];
               }
           }
       }
       n = M.n;
       arr = matr;
       return true;
    }
    return false;
}


Matrix &Matrix::operator*=(const Matrix &M)
{
    this->Multiply(M);
    return *this;
}




