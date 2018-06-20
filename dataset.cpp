#include "dataset.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
using namespace std;

Dataset::Dataset():
    xVector(),
    yVector(),
    a(0),
    b(0),
    weights()
{
    xVector.clear();
    yVector.clear();
    weights.clear();
}

void Dataset::printEquation(){
    cout<<"The regression equation is y = " << a << "x + " << b << endl;
}

double Dataset::computeYVariance(){
    return Dataset::variance(yVector);
}

double Dataset::computeSquareResSum(){
    double sum = 0;
    double term;
    for (int i =0; i<sizeOfSet(); i++){
        term = yVector[i] - a*xVector[i] - b;
        sum+=term*term;
    }
    return sum;
}


double Dataset::getX(int num){
    return xVector[num];
}

double Dataset::getY(int num){
    return yVector[num];
}

int Dataset::sizeOfSet(){
    return xVector.size();
}

void Dataset::addDataPoint(double x, double y){
    xVector.push_back(x);
    yVector.push_back(y);
}

void Dataset::listAllData(){
    for (int i = 0; i < xVector.size();i++)
        cout <<i << ": (" << xVector[i] << ", " << yVector[i] <<") "<< endl;
}

void Dataset::setDefaultWeights(){
    for (int i =0; i<xVector.size(); i++)
        weights.push_back(1.0);
}

void Dataset::addWeight(double w){
    weights.push_back(w);
}


void Dataset::weightedLRegression(){
    weightedLRegression(0, sizeOfSet() - 1);
}

void Dataset::clearWeights(){
    weights.clear();
}

void Dataset::weightedLRegression(int first, int last){
    double XtMY_0 = 0;
    double XtMY_1 = 0;
    double XtMX_0 =0;
    double XtMX_1 = 0;
    double XtMX_3 = 0;
    for (int i =first; i<=last; i++){
        XtMY_0 += xVector[i]*yVector[i]/weights[i];
        XtMY_1 += yVector[i]/weights[i];
        XtMX_0 += xVector[i]*xVector[i]/weights[i];
        XtMX_1 += xVector[i]/weights[i];
        XtMX_3 += 1/weights[i];
    }
    double detXtMX = XtMX_0*XtMX_3 -XtMX_1*XtMX_1;

    a = (XtMX_3*XtMY_0 - XtMX_1*XtMY_1)/detXtMX;
    b = (-1.0*XtMX_1*XtMY_0 + XtMX_0*XtMY_1)/detXtMX;

}

double Dataset::average(vector<double> list){
    double sum = 0;
    int size = list.size();
    for (int i = 0; i< size; i++)
        sum += list[i];
    return sum/size;
}

double Dataset::getA(){
    return a;
}

double Dataset::getB(){
    return b;
}

double Dataset::covOfEandX(double aValue, double bValue){
    double sum_1 = 0;
    double sum_2 = 0;
    double sum_3 = 0;
    for (int i = 0; i < sizeOfSet(); i++){
        sum_1 += (yVector[i] - aValue*xVector[i] - bValue)*(yVector[i] - aValue*xVector[i] - bValue)*xVector[i];
        sum_2 += (yVector[i] - aValue*xVector[i] - bValue)*(yVector[i] - aValue*xVector[i] - bValue);
        sum_3 += xVector[i];
    }
    return sum_1/sizeOfSet() - sum_2*sum_3/(sizeOfSet()*sizeOfSet());
}

double Dataset::correlationEandX(double aValue, double bValue){
    double covariance = covOfEandX(aValue, bValue);
    vector<double> list;
    for (int i = 0; i < sizeOfSet(); i++)
        list.push_back( (yVector[i] - aValue*xVector[i] - bValue)*(yVector[i] - aValue*xVector[i] - bValue));
    return covariance/sqrt(variance(list)*variance(xVector));
}

double Dataset::variance(vector<double> list){
    double var = 0;
    double avg = average(list);
    for (int i = 0; i < list.size(); i++){
        var += (list[i] - avg)*(list[i] - avg);
    }
    return var/list.size();
}
