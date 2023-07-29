#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <assert.h>
#include <cmath>
using namespace std;
#include "matrix.h"
vector<string> tokenize(string s,char delim = ' '){
    vector<string> res;
    string tmp;
    s += delim;
    for(char c : s){
        if(c == ' '){
            if(!tmp.empty())
                res.push_back(tmp);
            tmp = "";
        }
        else
            tmp += c;
    }
    return res;
}
const double EPS = 1e-9;
const double PI = acos(-1.0);
const double convFactor = PI / 180.0;

void normalize(Matrix<double>& vec){
    assert(vec.n == 4 && vec.m == 1);
    double len = 0;
    for(int i = 0 ; i < 4; ++i)
        len += vec[i][0] * vec[i][0]; 
    len = sqrt(len);
    assert(len > EPS);
    for(int i = 0; i < 4; ++i)
        vec[i][0] /= len;
}
Matrix<double> R(int raxis, Matrix<double> vec, double angle){

}
int main(){
    ifstream fin;
    fin.open("scene.txt");
    stack<Matrix<double> > S;
    Matrix<double> M(4,4,1);
    while(!fin.eof())
    {
        string command;
        fin >> command;
        if(command == "triangle"){
            Matrix<double> points(4,4,0);
            for(int i = 0; i < 3; ++i){
                for(int j = 0; j < 3; ++j)
                    fin >> points[j][i];
                points[3][i] = 1;
            }
            points[3][3] = 1;

        }
        else if(command == "translate"){
            Matrix<double> T(4,4,1);
            for(int i = 0 ; i < 3; ++i)
                fin >> T[i][3];


        }
        else if(command == "scale"){
            Matrix<double> T(4,4,1);
            for(int i = 0 ; i < 3; ++i)
                fin >> T[i][i];
            
            
        }
        else if(command == "rotate"){
            Matrix<double> a(4,1,0);
            a[3][0] = 1;
            double angle;
            fin >> angle;
            angle *= convFactor;
            for(int i = 0; i < 3; ++i)
                fin >> a[i][0];
            normalize(a);
            Matrix<double> C[3];
            C[0] = R(0, a, angle);
            C[1] = R(1, a, angle);
            C[2] = R(2, a, angle);
            Matrix<double> RM(4,4,1);
            for(int i = 0; i < 3; ++i){
                for(int j = 0; j< 3; ++j){
                    RM[i][j] = C[j][i][0];
                }
            }
            //RM
        }
        else if(command == "push"){

        }
        else if(command == "pop"){

        }
        else if(command == "end"){

        }
        else{
            cout << "unknown command " << command << '\n';
            fin.close();
            exit(0);
        }

    }
}