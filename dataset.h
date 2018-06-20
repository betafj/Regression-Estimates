#ifndef DATASET_H
#define DATASET_H
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
using std::vector;

class Dataset
{
public:
    Dataset();
    void addDataPoint(double x, double y);
    void addWeight(double w);
    void printEquation();
    void weightedLRegression();
    double computeYVariance();
    double computeSquareResSum();
    int sizeOfSet();
    double getX(int num);
    double getY(int num);
    double getA();
    double getB();
    void weightedLRegression(int first, int last);
    void setDefaultWeights();
    double getYVariance();
    double getSquareRes();
    static double average(vector<double> list);
    static double variance(vector<double> list);
    void listAllData();
    void clearWeights();
    double covOfEandX(double aValue, double bValue);
    double correlationEandX(double aValue, double bValue);

private:
    double a;
    double b;
    vector<double> xVector;
    vector<double> yVector;
    vector<double> weights;

};




#endif // DATASET_H
