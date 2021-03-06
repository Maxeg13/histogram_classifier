#ifndef NET_H
#define NET_H
//#include
#include "layer.h"
#include <QDebug>
#include <vector>
using namespace std;
class perceptron
{
public:

    int N;

    int inp_size;
    int out_size;
    float **out;
    float *tg_ptr;
    float *inp_ptr;
    layer **lr;
    layer *lrh;
    perceptron(vector<int>& constr);
    int getMaxInd();
    float reset_w();
    float** refresh(float* x);
    float** refresh(vector<float>& x);
    void learn1(vector<float>&, vector<float>&);
    void learnRandom1(vector<float>&, vector<float>&, float);
    void learn1(vector<float>&, vector<float>&, float);
    void learn1(float* x,float* t);
    void learn1(float* x,float t);
    void learnFunc();
private:
    int i;
};
#endif // NET_H
