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
void getData(vector<vector<vector<float>>>& data, float**& x, int*& t, int N);
void saveFile(string str);

Histogram* hist1;
Histogram* hist2;
int* targs_train;
int* targs_test;
float** emg_test;
float** emg_train;
QTextStream* out;
vector<vector<vector<float>>> data_train, data_test;//[classes][samples][channels]
vector<vector<float>> classes;
int N_test,N_train;
int chans[3]={3,6,7};
int indent=500;

int main(int argc, char *argv[])
{
    hist1=new Histogram();
    hist2=new Histogram();
    string path;

    openFile(string(argv[1])+string("/")+string(argv[2]),emg_train,targs_train, N_train);
    openFile(string(argv[1])+string("/")+string(argv[3]),emg_test,targs_test, N_test);



    vector<int> constr;
    constr.push_back(hist1->N2+hist2->N2);
    constr.push_back(7);//10
    perc=new perceptron(constr);


    getData(data_train, emg_train, targs_train, N_train);
    getData(data_test, emg_test, targs_test, N_test);
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
        cout<<data_train[i].size()<<endl;




    saveFile(string(argv[1])+string("/")+string(argv[4]));
}


void getData(vector<vector<vector<float>>>& data, float**& x, int*& targs, int N)
{
    data.resize(7);
    vector<float> vv;

    for(int j=0;j<N;j++)
    {

        hist1->increment(x[0][j],x[1][j]);
        hist2->increment(x[0][j],x[2][j]);

        vv=hist1->b;
        vv.insert(vv.end(),hist2->b.begin(),hist2->b.end());
        //        cout<<vv.size()<<endl;
        data[targs[j]-1].push_back(vv);

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


void saveFile(string str)
{
    QString file3Name=QString::fromUtf8((str).c_str());
    //     QString file3Name="hello1";
    QFile file3(file3Name);
    file3.open(QIODevice::WriteOnly | QIODevice::Text);
    //         return 1;
    out=new QTextStream(&file3);
    out->flush();
    int class_i;
    int sample_i;

    for(int k=0;k<300000;k++)
    {
        class_i=rand()%7;
        vector<float> targs_v;
        sample_i=rand()%(data_train[class_i].size()-indent)+indent;
        targs_v.resize(7);
        targs_v[class_i]=1;
        perc->learn1(data_train[class_i][sample_i],targs_v);
    }

    for(int k=0;k<1000;k++)
    {
        class_i=rand()%7;
        sample_i=(rand()%(data_test[class_i].size()-indent-1))+indent;
        perc->refresh(data_test[class_i][sample_i]);
        *out<<class_i<<"    "<<perc->getMaxInd()<<endl;
    }

    perc->reset_w();

    for(int k=0;k<300000;k++)
    {
        class_i=rand()%7;
        vector<float> targs_v;
        sample_i=rand()%(data_test[class_i].size()-indent)+indent;
        targs_v.resize(7);
        targs_v[class_i]=1;
        perc->learn1(data_test[class_i][sample_i],targs_v);
    }


    for(int k=0;k<1000;k++)
    {
        class_i=rand()%7;
        sample_i=(rand()%(data_train[class_i].size()-indent-1))+indent;
        perc->refresh(data_train[class_i][sample_i]);
        *out<<class_i<<"    "<<perc->getMaxInd()<<endl;
    }
    file3.close();
    //    cout<<inputFile.open(QIODevice::ReadOnly)<<endl;

}
