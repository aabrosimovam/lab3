#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);//устанавливаем сцену
    scene = new QGraphicsScene;

    ui->graphicsView->setScene(scene);

    timer = new QTimer(this);//таймер
    connect(timer,SIGNAL(timeout()),this,SLOT(Draw()));
    timer->start(100);

    FrontFace = new bool[Ngr];//память под массив граней(лицевых, нелицевых)
     W = new float*[4];//память под матрицу тела
     for (int i =0; i<4;i++)
         {
             W[i] = new float[Ngr];
         }

    GravityCenter();//находим центр тяжести
    MatrixW();//считаем матрицу тела
}

void Widget::GravityCenter()
{
    float Xc=0;
    float Yc=0;
    float Zc=0;
    for(int i=4;i<Nver;i++)//с 4 начиная, потому что там оси
    {
        Xc+=Ver[i][0];
        Yc+=Ver[i][1];
        Zc+=Ver[i][2];
    }
    Xc/=Nver-4;//-4 тоже из-за осей
    Yc/=Nver-4;
    Zc/=Nver-4;
    Center[0] = Xc;
    Center[1] = Yc;
    Center[2] = Zc;
}
void Widget::MatrixW()
{
    for (int i=0;i<Ngr;i++)
     {
       //тут то что Gran[][] это типа номер вершины из массива граней, а Ver[номер вершины][x,y или z]
        //все по формулам из методички
        float A = (Ver[Gran[i][2]][1]-Ver[Gran[i][0]][1])*(Ver[Gran[i][1]][2]-Ver[Gran[i][0]][2]) -
                (Ver[Gran[i][1]][1]-Ver[Gran[i][0]][1])*(Ver[Gran[i][2]][2]-Ver[Gran[i][0]][2]);
        float B = (Ver[Gran[i][1]][0]-Ver[Gran[i][0]][0])*(Ver[Gran[i][2]][2]-Ver[Gran[i][0]][2]) -
                (Ver[Gran[i][2]][0]-Ver[Gran[i][0]][0])*(Ver[Gran[i][1]][2]-Ver[Gran[i][0]][2]);
        float C = (Ver[Gran[i][2]][0]-Ver[Gran[i][0]][0])*(Ver[Gran[i][1]][1]-Ver[Gran[i][0]][1]) -
                (Ver[Gran[i][1]][0]-Ver[Gran[i][0]][0])*(Ver[Gran[i][2]][1]-Ver[Gran[i][0]][1]);
        float D = -(A*Ver[Gran[i][0]][0]+B*Ver[Gran[i][0]][1]+C*Ver[Gran[i][0]][2]);

        if (A*Center[0]+B*Center[1]+C*Center[2]+D<0)
        {
            A=-A;
            B=-B;
            C=-C;
            D=-D;
        }
        W[0][i] = A;
        W[1][i] = B;
        W[2][i] = C;
        W[3][i] = D;
     }
}
void Widget::FrontFaces()//проверка на лицевые грани
{
    for(int i=0;i<Ngr;i++)
        {
            if(W[0][i]*x0+W[1][i]*y0+W[2][i]*z0<0)//тут проекция параллельная, поэтому без D по формуле
            {
                FrontFace[i] = true;
            }
            else
            {
                FrontFace[i] = false;
            }
        }
}
void Widget::updateMatr()//считаем матрицы
{
    float arrT[] = {1,0,0,0,
                    0,1,0,0,
                    0,0,1,0,
                   -x0,-y0,-z0,1};
    float arrS[] = {-1,0,0,0,
                    0,1,0,0,
                    0,0,1,0,
                    0,0,0,1};
    float arrRx90[] = {1,0,0,0,
                       0,0,-1,0,
                       0,1,0,0,
                       0,0,0,1};
    float d = sqrt(x0*x0+y0*y0);
    float arrRy[] = {y0/d,0,x0/d,0,
                       0,1,0,0,
                     -x0/d,0,y0/d,0,
                       0,0,0,1};
    float s = sqrt(x0*x0+y0*y0+z0*z0);
    float arrRx[] = {1,0,0,0,
                       0,d/s,-z0/s,0,
                       0,z0/s,d/s,0,
                       0,0,0,1};
    T = Matrix(4,4,arrT);
    S = Matrix(4,4,arrS);
    Rx90= Matrix(4,4,arrRx90);
    Ry = Matrix(4,4,arrRy);
    Rx = Matrix(4,4,arrRx);
    //сохраняем начальные вершины в NewVer

    for(int i=0;i<12;i++)
    {
        for(int j=0;j<3;j++)
        {
            NewVer[i][j] = Ver[i][j];
        }
    }
    FrontFaces();//добавилась проврка на лицевые грани
}

void Widget::SKM_SKN()
{
  Matrix M();
  V = T;
  V *= S;
  V *= Rx90;
  V *= Ry;
  V *= Rx;//Считам матрицу(Вот тут поменялись вершины Ver, для чего и создан был массив NewVer)
  for(int i=0;i<12;i++)
  {
    float M[] = {NewVer[i][0],NewVer[i][1],NewVer[i][2],1};//Тут все по формулам перемножаем
    Matrix Matr(1,4,M);
    Matr*=V;
    float** new_ver = Matr.GetArr();
    NewVer[i][0] = new_ver[0][0];//пересчитали вершины
    NewVer[i][1] = new_ver[0][1];
    NewVer[i][2] = new_ver[0][2];
  }
}

void Widget::SKN_SKK()
{
  //перешли к картинной системе(тут использовалась парралельная проекция)
    for(int i=0;i<12;i++)
    {
        VerKa[i][0] = NewVer[i][0];
        VerKa[i][1] = NewVer[i][1];
    }
}

void Widget::SKK_SKEi()
{
    for(int i =0;i<12;i++)
    {
        VerEk[i][0] = VerKa[i][0]/P*xe+xc;
        VerEk[i][1] = VerKa[i][1]/P*ye+yc;
    }
}

void Widget::print_scene()
{
    scene->addLine(QLineF (QPointF(VerEk[0][0],-VerEk[0][1]),QPointF(VerEk[1][0],-VerEk[1][1])));//рисуем оси
    scene->addLine(QLineF (QPointF(VerEk[0][0],-VerEk[0][1]),QPointF(VerEk[2][0],-VerEk[2][1])));
    scene->addLine(QLineF (QPointF(VerEk[0][0],-VerEk[0][1]),QPointF(VerEk[3][0],-VerEk[3][1])));
    for(int i =0;i<Ngr;i++)//потом по граням все остальное
    {
        if (FrontFace[i])//если грань лицевая
        {
            int j =0;
            while(Gran[i][j+1]!=0)
            {
                QLineF temp(QPointF(VerEk[Gran[i][j]][0],-VerEk[Gran[i][j]][1]),QPointF(VerEk[Gran[i][j+1]][0],-VerEk[Gran[i][j+1]][1]));
                scene->addLine(temp);
                j++;
            }
        }
    }
}


void Widget::Draw()
{
    scene->clear();//очищаем сцену

    updateMatr();//считаем марицы
    SKM_SKN();
    SKN_SKK();
    SKK_SKEi();//переходим к нужной системе
    print_scene();//рисуем
    if(x0>1)//меняем точку зрения
    {
           x0--;
           y0--;
    }

}

Widget::~Widget()
{
    delete FrontFace;
    for (int i =0; i<4;i++)
        {
            delete W[i];
        }
        delete W;
    delete ui;
}
