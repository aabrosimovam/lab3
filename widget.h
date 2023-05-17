#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include "matrix.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
    int Ngr=6;//число граней
    int Nver=12;//число вершин

    //точка зрения
    float x0 = 10;
    float y0 = 7;
    float z0 = 5;
    //координаты центра поля и полуосей поля(я хз это типа помогает регулировать размещение куба в окне)
    int xc = 435;
    int yc = 275;
    int xe = 200;
    int ye = 200;
    //всякие матрицы, нужные чтоб перейти от одной системы координат к другой
    Matrix T;
    Matrix S;
    Matrix Rx90;
    Matrix Ry;
    Matrix Rx;
    Matrix V;
    //координаты вершин
    float Ver[12][3] = {{0,0,0},{5,0,0},{0,5,0},{0,0,5},{1,-1,-1},{1,1,-1},{-1,1,-1},{-1,-1,-1},{1,-1,1},{1,1,1},{-1,1,1},{-1,-1,1}};
    //копия координат вершин, это надо чтоб действия производились с изначальными вершинами, а не с поменяными
    float NewVer[12][3] = {{0,0,0},{5,0,0},{0,5,0},{0,0,5},{1,-1,-1},{1,1,-1},{-1,1,-1},{-1,-1,-1},{1,-1,1},{1,1,1},{-1,1,1},{-1,-1,1}};
    //массив граней
    int Gran[6][9]={{4,5,9,8,4,0,0,0,0},{5,6,10,9,5,0,0,0,0},{6,10,11,7,6,0,0,0,0},{7,11,8,4,7,0,0,0,0},{4,5,6,7,4,0,0,0,0},{8,9,10,11,8,0,0,0,0}};
    //массивы вершин для картинной и экранной систем координат
    float VerKa[12][2];
    float VerEk[12][2];
    //это тоже вроде для регулирования положения куба в окне
    int P = 4;
    float** W;//матрица тела
    float Center[3];//центр тяжести(массив x,y,z)
    bool* FrontFace;//лицевые грани

public:
    Widget(QWidget *parent = nullptr);
    void SKM_SKN();//переход от мировой системы к системе координат наблюдателя
    void SKN_SKK();//от наблюдателя к картинной
    void SKK_SKEi();//от картинной к экранной
    void print_scene();//рисуем куб
    void updateMatr();//обновляем матрицы
    void GravityCenter();//считаем центр тяжести
    void MatrixW();//находим матрицу тела
    void FrontFaces();//определяем лицевые грани
    ~Widget();//деструктор

private:
    Ui::Widget *ui;
    QGraphicsScene *scene;
    QTimer *timer;

public slots:
    void Draw();//это вызывается по таймеру, все рисуется и вращается
};

#endif // WIDGET_H
