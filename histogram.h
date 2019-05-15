#ifndef Histogram_H
#define Histogram_H
#include <vector>
using namespace std;

class Histogram
{
public:
    int** b_ind;
    Histogram();
    vector<float> b;
    int N;
    int N2;
    int width;
    int* grid;
    int* grid_out;
    float** a;
    float k;
    int range;
    int discr;
    float incr;

    void increment(int x, int y);

};

#endif // Histogram_H
