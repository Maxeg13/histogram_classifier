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
vector<vector<vector<float>>> data_l_inp;//[classes][samples][channels]
vector<vector<float>> classes;
int N_test,N_train;
int chans[3]={3,6,7};

int main(int argc, char *argv[])
{

    Histogram hist1, hist2;
    openFile(string(argv[1]),emg_train,targs_train, N_train);
    openFile(string(argv[2]),emg_test,targs_test, N_test);


    vector<int> constr;
    constr.push_back(hist1.N2+hist2.N2);
    constr.push_back(7);//10
    perc=new perceptron(constr);


    //data_l_inp.resize(hist1.N2+hist2.N2);
    data_l_inp.resize(7);

    int cnt=0;
    for(int j=0;j<(N_train);j++)
    {
        cnt++;
        hist1.increment(emg_train[0][j],emg_train[1][j]);
        hist2.increment(emg_train[0][j],emg_train[2][j]);
//        if(cnt==10)
        {

            cnt=0;
            vector<float> vv=hist1.b;
            vv.insert(vv.end(),hist2.b.begin(),hist2.b.end());
            cout<<vv.size()<<endl;
            data_l_inp[targs_train[j]-1].push_back(vv);
        }
    }
    //    vector<float> aa, bb;
    //    aa.push_back(1);
    //    aa.push_back(2);
    //    bb=aa;
    //    bb[1]=3;
    //    aa[1]+=10;
    //    aa.insert(aa.end(),bb.begin(),bb.end());
    //    cout<<aa[3];
    int class_i;
    for (int i=0;i<7;i++)
        cout<<data_l_inp[i].size()<<endl;

    int indent=200;
    int sample_i;
    for(int k=0;k<30000;k++)
    {
        class_i=rand()%7;
        vector<float> targs_v;
        sample_i=k%(data_l_inp[class_i].size()-indent)+indent;
        targs_v.resize(7);
        targs_v[class_i]=1;
        perc->learn1(data_l_inp[class_i][sample_i],targs_v);
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

        //        for(int j=0;j<N;j++)
        //        {
        //            for (int i=0;i<3;i++)
        //                cout<<x[i][j]<<"     ";//5 spaces
        //            cout<<targs[j];
        //            cout<<endl;
        //        }
        //        cout<<endl;

    }
}

