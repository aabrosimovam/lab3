#ifndef MATRIX_H
#define MATRIX_H

#include <QApplication>
#include <iostream>
class Matrix
{
private:
    int m;//количество строк
    int n;//количество столбцов
    float** arr;

public:
    Matrix();//конструктор по умолчанию
    Matrix(int M, int N, float array[]);//конструктор с аргументами
    Matrix(const Matrix&);//конструктор копирования
    Matrix& operator=(const Matrix& M);//перегрузка оператора присвоения

    float** GetArr() const;//геттер возвращает массив значений из матрицы

    bool Multiply(const Matrix& M);//умножение матриц
    Matrix& operator*=(const Matrix& M);//перегрузка оператора умножения

};

#endif // MATRIX_H
