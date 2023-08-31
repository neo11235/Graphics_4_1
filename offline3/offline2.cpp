#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <assert.h>
#include <cmath>
#include <cstring>
#include <iomanip>
using namespace std;
#include "matrix.h"
const double EPS = 1e-9;
const double PI = acos(-1.0);
const double convFactor = PI / 180.0;

void normalize(Matrix<double>& vec){
    assert(vec.n >= 3 && vec.m == 1);
    // cout << "vec norm \n" << vec << "\n\n";
    double len = 0;
    for(int i = 0 ; i < 3; ++i)
        len += vec[i][0] * vec[i][0]; 
    len = sqrt(len);
    assert(len > EPS);
    for(int i = 0; i < 3; ++i)
        vec[i][0] /= len;
}
void normvec(Matrix<double>& vec){
    double len = 0;
    for(int i = 0; i < vec.n; ++i){
        for(int j = 0; j < vec.m; ++j){
            len += vec[i][j] * vec[i][j];
        }
    }
    len = sqrt(len);
    assert(len > EPS);
    for(int i = 0 ; i < vec.n; ++i){
        for(int j = 0; j < vec.m; ++j){
            vec[i][j] /= len;
        }
    }
}

void normalizew(Matrix<double> & mat){
    
    for(int i = 0; i < mat.m; ++i){
        double val = mat[3][i];
        for(int j = 0; j < mat.n; ++j){
            assert(abs(val) > EPS);
            mat[j][i] /= val;
        }
    }
}
Matrix<double> cross(Matrix<double> a, Matrix<double> b){
    return Matrix<double>(
        {{a[0][1] * b[0][2] - a[0][2] * b[0][1],
        -a[0][0] * b[0][2] + a[0][2] * b[0][0],
         a[0][0] * b[0][1] - a[0][1] * b[0][0]
        }});
}
double dot(Matrix<double> a, Matrix<double> b){
    double res = 0;
    for(int i = 0; i < 3 ; ++i){
        res += a[0][i] * b[0][i];
    }
    return res;
}
Matrix<double> R(Matrix<double> x, Matrix<double> axis, double angle){
    // Matrix<double> RM(4,4,1);
    // int i = (raxis +1 ) % 3;
    // int j = (raxis +2 ) % 3;
    // RM[i][i] = cos(angle);
    // RM[i][j] = -sin(angle);
    // RM[j][j] = cos(angle);
    // RM[j][i] = sin(angle);
    // assert(vec.n == 4 && vec.m == 1);
    // Matrix<double> tmp(1,4);
    // for(int i = 0 ;i < 4; ++i)
    //     tmp[0][i] = vec[i][0];
    // return tmp * RM;
    normvec(axis);
    Matrix<double> res = x * cos(angle);
    // return res;
    res = res + axis * (1 - cos(angle)) * dot(axis, x) ;
    // return res;
    res = res + cross(axis, x) * sin(angle);
    return res;
}
int main(){
    Matrix<double> ihat(1,3,0),jhat(1,3,0),khat(1,3,0);
    ihat[0][0] = 1;
    jhat[0][1] = 1;
    khat[0][2] = 1;
    ifstream fin;
    ofstream fout1,fout2,fout3;
    fin.open("scene.txt");
    fout1.open("stage1.txt");
    fout2.open("stage2.txt");
    fout3.open("stage3.txt");
    stack<Matrix<double> > S;
    Matrix<double> M(4,4,1);
    fout1 << fixed << setprecision(7);
    fout2 << fixed << setprecision(7);
    fout3 << fixed << setprecision(7);
    cout << fixed << setprecision(7);

    Matrix<double> eye(1,3),look(1,3),up(1,3),f(1,4);
    double fovy, aspect, near,far;
    fin >> eye >> look >> up 
    >> fovy >> aspect >> near >> far;
    fovy *= convFactor;
    Matrix<double> l = look - eye;
    
    normvec(l);
    
    Matrix<double> r = cross(l, up);
    normvec(r);
    Matrix<double> u = cross(r, l);
    Matrix<double> Tmp(4,4,1),Rmp(4,4,1),V;
    for(int i = 0 ; i < 3 ; ++i){
        Tmp[i][3] = -eye[0][i];
        Rmp[0][i] = r[0][i];
        Rmp[1][i] = u[0][i];
        Rmp[2][i] = -l[0][i];
    }
    V = Rmp * Tmp;

    double fovx = fovy * aspect;
    double t = near * tan(fovy / 2);
    double rr = near * tan(fovx / 2);
    Matrix<double> PT(4,4,0);
    PT[0][0] = near / rr;
    PT[1][1] = near / t;
    PT[2][2] = -(far + near) / ( far - near);
    PT[2][3] = -(2 * far * near)/ (far - near);
    PT[3][2] = -1;
    // cout << PT << endl;

    


    while(!fin.eof())
    {
        string command;
        fin >> command;
        //  cout << command <<endl;
        if(command == "triangle"){
            // cout << "debug M \n" << M << "\n\n";
            Matrix<double> points(4,4,0);
            for(int i = 0; i < 3; ++i){
                for(int j = 0; j < 3; ++j)
                    fin >> points[j][i];
                points[3][i] = 1;
            }
            points[3][3] = 1;
            // cout << "debug points\n" << points << "\n\n";
            Matrix<double> out1 = M * points;
            normalizew(out1);
            for(int i = 0; i < 3; ++i){
                for(int j = 0; j < 3; ++j){
                    fout1 << out1[j][i] << " \n"[ j == 2];
                }
            }
            fout1 << '\n';
            Matrix<double> out2 = V * out1;
            normalizew(out2);
            for(int i = 0; i < 3; ++i){
                for(int j = 0; j < 3; ++j){
                    fout2 << out2[j][i] << " \n"[ j == 2];
                }
            }
            fout2 << '\n';

            Matrix<double> out3 = PT * out2;
            // cout << "debug out3 \n" << out3 <<  "\n\n";
            // cout << "dbg out 2\n " << out2 << "\n\n";
            normalizew(out3);
            for(int i = 0; i < 3; ++i){
                for(int j = 0; j < 3; ++j){
                    fout3 << out3[j][i] << " \n"[j == 2];
                }
            }
            fout3 << '\n';

        }
        else if(command == "translate"){
            Matrix<double> T(4,4,1);
            for(int i = 0 ; i < 3; ++i)
                fin >> T[i][3];
            // cout << "debug\n" << T << '\n' << "debug end\n";

            // M = T * M ;
            M = M * T;
            // normalizew(M);

        }
        else if(command == "scale"){
            Matrix<double> T(4,4,1);
            for(int i = 0 ; i < 3; ++i)
                fin >> T[i][i];
            
            // cout << "debug scale\n" << T << "\n\n";

            // M = T * M;
            M = M * T;
            // normalizew(M);
        }
        else if(command == "rotate"){
            Matrix<double> a(1,3,0);
            double angle;
            fin >> angle;
            angle *= convFactor;
            for(int i = 0; i < 3; ++i)
                fin >> a[0][i];
            normvec(a);
            Matrix<double> C[3];
            C[0] = R(ihat, a, angle);
            C[1] = R(jhat, a, angle);
            C[2] = R(khat, a, angle);
            // cout << "debug\n"
            // << C[0] << "\n\n" 
            // << C[1] << "\n\n"
            // << C[2] << "\n\n";
            Matrix<double> RM(4,4,1);
            for(int i = 0; i < 3; ++i){
                for(int j = 0; j < 3; ++j){
                    RM[i][j] = C[j][0][i];
                }
            }
            // cout << "rm \n" << RM << "\n\n";
            //RM
            // M = RM * M;
            M = M * RM;
        }
        else if(command == "push"){
            S.push(M);

        }
        else if(command == "pop"){
            if(S.empty()){
                cout <<"error: stack is empty \n";
                return 0;
            }
            M = S.top();
            S.pop();
        }
        else if(command == "end"){
            break;
        }
        else{
            cout << "unknown command \'" << command << '\'' << '\n';
            fin.close();
            fout1.close();
            fout2.close();
            fout3.close();
            exit(0);
        }
    }
    fin.close();
    fout1.close();
    fout2.close();
    fout3.close();
    return 0;
}