#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <string>
#include <QFile>
#include "histogram.h"
#include <deque>
#include "perceptron.h"

perceptron* perc;
using namespace std;



void openFile(string str, float**& x, int*& targs, int& N);

int* targs_train;
int* targs_test;
float** emg_test;
float** emg_train;
vector<deque<vector<float>>> data_l_inp;
vector<vector<float>> classes;
int N_test,N_train;
int chans[3]={3,6,7};

int main(int argc, char *argv[])
{
    Histogram hist1, hist2;
    openFile(string(argv[1]),emg_train,targs_train, N_train);
    openFile(string(argv[2]),emg_test,targs_test, N_test);
    cout<<argv[1]<<endl;

    vector<int> constr;
    constr.push_back(hist1.N2+hist2.N2);
    constr.push_back(5);//10
    perc=new perceptron(constr);

    for(int j=0;j<N_train;j++)
    {
        hist1.increment(emg_train[0][j],emg_train[1][j]);
        hist2.increment(emg_train[0][j],emg_train[2][j]);
    }





}



void openFile(string str, float**& x, int*& targs, int& N)
{

    QString qstr=QString::fromUtf8((str).c_str());

    QFile inputFile(qstr);
    //    cout<<inputFile.open(QIODevice::ReadOnly)<<endl;
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        QString line = in.readLine();

        QStringList s_list;
        while (!line.isNull())
        {
            s_list.push_back(line);
            line = in.readLine();
            //           cout<<s_list.size()<<'\n';
        }
        N=s_list.size();

        x=new float*[3];
        for (int i=0;i<3;i++)
        {
            x[i]=new float[N];
        }

        targs=new int[N];


        QStringList vstr;


        for(int j=0;j<N;j++)
        {
            vstr=s_list[j].split("     ");
            for(int i=0;i<3;i++)
            {
                //                                        cout<<i<<endl;
                x[i][j]=vstr[chans[i]+1].toFloat();
                //                cout<<vstr[chans[i]+1].toFloat()<<" ";
            }
            targs[j]=vstr[9].toFloat();
        }

        for(int j=0;j<N;j++)
        {
            for (int i=0;i<3;i++)
                cout<<x[i][j]<<"  ";
            cout<<targs[j];
            cout<<endl;
        }
    }
}

