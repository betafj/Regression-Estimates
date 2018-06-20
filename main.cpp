
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include<dataset.h>

using namespace std;

int numOfDataPoints[5] = {100, 100, 50, 50, 50};
int numOfPartitions[5]={5, 9, 4, 4, 4};


//This evaluates a step function whose function values are contained in a list given by 'values'.
double valuationOfLCF(vector<double> values, double input, int period, double min){
    int index = floor((input - min)/period);
    if (index < values.size())
        return values[index];
    return 0;
}


bool scanData(Dataset &d, int fileNumber){
    string index = "012345";
    string fileName = "data_1_3.csv";
    fileName.at(7) =  index.at(fileNumber);
    std::string::size_type sz;
    int counter = 0;
    ifstream ip(fileName);
    if (!ip.is_open()){
        cout<<"ERROR"<<endl;
        return false;
    }
    string x;
    string y;
    while(ip.good()){
        getline(ip, x, ',');
        getline(ip, y, '\n');
        //cout<<counter<<"|  x: " << x << " y: "<< y << endl;
        if (counter > 0 && counter <= numOfDataPoints[fileNumber-1]){
            d.addDataPoint(stod(x, &sz), stod(y, &sz));
        }
        counter++;
    }
    ip.close();
    return true;
}

void analyzeOverInterval(Dataset data,  int i, int intervalSize, Dataset &intervalData){
    for (int j = 0; j < data.sizeOfSet(); j++){
        if (data.getX(j) > i && data.getX(j) < i + intervalSize){
            intervalData.addDataPoint(data.getX(j), data.getY(j));
        }

    }
    intervalData.setDefaultWeights();
    intervalData.weightedLRegression();
    cout <<"________________________" << endl;
    cout <<"Over the interval: ("<<i << ", " << i+intervalSize << ")" << endl;
    intervalData.printEquation();
    cout <<"n:= Number of data points  in this interval: " << intervalData.sizeOfSet() << endl;
    if (intervalData.sizeOfSet() >= 4)
        cout << "SUM_ {i = 1}^n (y_i - a*x_i - b)^2/n = " <<intervalData.computeSquareResSum()/intervalData.sizeOfSet() <<endl;
    else
        cout << "10000" << endl;
    if (intervalData.sizeOfSet() >=4)
        cout << "Variance of Y_i's: " << intervalData.computeYVariance() << endl;

}

void displayWeightedRegression(Dataset data){
    cout<<"___________________________" << endl <<endl;
    cout <<"WEIGHTED REGRESSION Line: " << endl;
    data.weightedLRegression();
    data.printEquation();
}

void findXRange(Dataset data, double &minX, double & maxX){

    for (int i = 0; i < data.sizeOfSet(); i++){
        if (minX > data.getX(i))
            minX = data.getX(i);
        if (maxX < data.getX(i))
            maxX = data.getX(i);
    }
}

void runOneFile(Dataset &data, int fileNumber){
    if (scanData(data, fileNumber)){
        data.setDefaultWeights();
        data.weightedLRegression();
        cout << "Ordinary Linear Regression: " << endl;
        data.printEquation();
        cout << "_________________" <<endl <<endl;
    }

    vector<double> varianceList;

    double minX = 0;
    double maxX = 0;
    findXRange(data, minX, maxX);
    cout << "Minimum X: " << minX << " Max X: "<< maxX << endl << endl << endl;

    int intervalSize = (floor(maxX) + 1  - floor(minX))/numOfPartitions[fileNumber -1];
    vector<double> sampleXValues;
    for (int i = floor(minX); i < floor(maxX) + 1; i+= intervalSize){
        Dataset intervalData;

        analyzeOverInterval(data, i, intervalSize, intervalData);
        sampleXValues.push_back(i + intervalSize/2);
        if (intervalData.sizeOfSet() >= 4)
        //varianceList.push_back(intervalData.computeSquareResSum()/intervalData.sizeOfSet());
            varianceList.push_back(intervalData.computeYVariance());
        else
            varianceList.push_back(10000.0);
    }
    cout <<"__________________________________________" << endl;

    cout<<"ADJUST WEIGHTS" << endl;
    data.clearWeights();
    for (int i = 0; i< data.sizeOfSet(); i++){
        data.addWeight(valuationOfLCF(varianceList, data.getX(i), intervalSize, floor(minX)));
    }
}


int main(){

    for (int fileNumber = 1; fileNumber < 6; fileNumber++){
        cout << "DATASET: " << "data_1_" << fileNumber <<".csv" << endl;
        Dataset data;
        runOneFile(data, fileNumber);
        displayWeightedRegression(data);
        cout<<"End of analysis for this dataset!" << endl << endl << endl;
    }
    return 0;
}
