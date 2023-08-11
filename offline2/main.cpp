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
#include "bitmap_image.hpp"
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
    if(!(len > EPS)){
        cout << vec.n << ' ' << vec.m << endl;
        cout << len << endl;
        assert(false);
    }
    len = sqrt(len);
    

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
Matrix<double> cross(Matrix<double> a, Matrix<double> b){ // row matrix
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

    normvec(axis);
    Matrix<double> res = x * cos(angle);
    res = res + axis * (1 - cos(angle)) * dot(axis, x);
    res = res + cross(axis, x) * sin(angle);
    return res;
}
const string inputPath = "./TestCases/1/";
const string outputPath = "";
const string imageFile = "out.bmp";
const double z_max = 1.0;
int Screen_Width, Screen_Height;
const double z_front_limit = -1.0;
static unsigned long int g_seed = 1;
inline int random1(){
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}
pair<double,double> intersect(Matrix<double> &A, Matrix<double> &B, double ys){

    double ipx, ipz;
    if(abs(B[0][1] - A[0][1]) < EPS){
        ipx = A[0][0];
        ipz = A[0][2];
    }
    else{
        ipx = A[0][0] + 
        ((ys - A[0][1])/(B[0][1] - A[0][1])) * (B[0][0] - A[0][0]);

        ipz = A[0][2] + 
        ((ys - A[0][1])/(B[0][1] - A[0][1])) * (B[0][2] - A[0][2]);
    }
    return make_pair(ipx,ipz);
}
void stage4(vector<Matrix<double> > triangles)
{
    ifstream config;
    config.open(inputPath + "config.txt");

    if(!config.is_open()){
        cout << "Cannot open config file\n";
        exit(0);
    }

    config >> Screen_Width >> Screen_Height;
    config.close();
    
    int n = triangles.size();

    vector<vector<int> > color(n, vector<int>(3));
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < 3; ++j){
            color[i][j] = random1() ;//& 0xFF;
        }
    }
    
    cout <<"screen height and width\n";
    cout << Screen_Width << ' ' << Screen_Height << '\n';

    double dx = 2.0 / Screen_Width;
    double dy = 2.0 / Screen_Height;

    cout << "dx dy \n";
    cout << dx << ' ' << dy << endl;

    double Top_Y = 1 - dy / 2;
    double Left_X = -1 + dx / 2;
    
    vector<vector<double> > z_buffer(Screen_Height,
    vector<double>(Screen_Width, z_max));
    vector<vector<vector<int> > > z_color(Screen_Height,
    vector<vector<int>>(Screen_Width, vector<int>(3,0))
    );

    

    for(int i = 0; i < n; ++i){
        Matrix<double> A(1,3),B(1,3),C(1,3);
        for(int j = 0; j < 3; ++j){
            A[0][j] = triangles[i][j][0];
            B[0][j] = triangles[i][j][1];
            C[0][j] = triangles[i][j][2];
        }
        //order such that y coordinates sorted order becomes A , B , C
        if(A[0][1] > B[0][1]){
            swap(A,B);
        }
        if(C[0][1] < A[0][1]){
            swap(A,C);
            swap(B,C);
        }
        if(C[0][1] < B[0][1]){
            swap(B,C);
        }
        if(i == 0 ){
            cout <<"debug\n";
            cout << A << '\n';
            cout << B << '\n';
            cout << C << '\n';
        }
        double ys = Top_Y;
        for(int x = 0; x < Screen_Height; ++x){
            double xa = INT_MAX, xb = INT_MIN;
            double za,zb;
            bool isect = false;
            if(ys >= A[0][1] && ys <= B[0][1]){
                double ipx,ipz;
                tie(ipx, ipz) = intersect(A, B, ys);
                if(ipx < xa){
                    xa = ipx;
                    za = ipz;
                }
                if(ipx > xb){
                    xb = ipx;
                    zb = ipz;
                }
                isect = true;
            }
            if(ys >= A[0][1] && ys <= C[0][1]){
                double ipx, ipz;
                tie(ipx, ipz) = intersect(A, C, ys);
                if(ipx < xa){
                    xa = ipx;
                    za = ipz;
                }
                if(ipx > xb){
                    xb = ipx;
                    zb = ipz;
                }
                isect = true;
            }
            if(ys >= B[0][1] && ys <= C[0][1]){
                double ipx, ipz;
                tie(ipx, ipz) = intersect(B, C, ys);
                if(ipx < xa){
                    xa = ipx;
                    za = ipz;
                }
                if(ipx > xb){
                    xb = ipx;
                    zb = ipz;
                }
                isect = true;
            }
            if(!isect){
                ys -= dy;
                continue;
            }
            int lo = round(xa / dx) + EPS + Screen_Width / 2.0;
            int hi = round(xb / dx) + EPS + 1 + Screen_Width / 2.0;
            
            for(int y = max(lo,0); y < min(hi, Screen_Width); ++y){
                double xx = Left_X + y * dx;
                double factor = 0;
                if(abs(xx - xa) < EPS){
                    factor = 0;
                }
                else{
                    factor = (xx - xa) / (xb - xa);
                }
                double pz = za + factor * (zb - za);
                // pz = pz + 1; // depth from screen
                //pz = 1 - pz;
                if(pz < z_buffer[x][y] && pz >= z_front_limit)
                {
                    z_buffer[x][y] = pz;
                    z_color[x][y] = color[i]; 
                }
                ++xx;
            }
            ys -= dy; 
        }
    }
    ofstream fout4;
    fout4.open("z_buffer.txt");
    if(!fout4.is_open()){
        cout << "Cannot open z_buffer.txt\n";
        exit(0);
    }
    fout4 << fixed << setprecision(6);
    for(int i = 0; i < Screen_Height; ++i){
        for(int j = 0; j < Screen_Width; ++j){
            if(!(abs(z_buffer[i][j] - z_max) < EPS))
                fout4 << z_buffer[i][j] << '\t';
        }
        fout4 << '\n';
    }
    fout4.close();
    cout << "finished z buffer\n";
    bitmap_image image(Screen_Height, Screen_Width);
    for(int x = 0; x < Screen_Height; ++x){
        for(int y = 0; y < Screen_Width; ++y){
            image.set_pixel(y,x,z_color[x][y][0], z_color[x][y][1], z_color[x][y][2]);
        }
    }
    image.save_image(outputPath + imageFile);

}
int main(){
    Matrix<double> ihat(1,3,0),jhat(1,3,0),khat(1,3,0);
    ihat[0][0] = 1;
    jhat[0][1] = 1;
    khat[0][2] = 1;
    ifstream fin;
    ofstream fout1,fout2,fout3;
    fin.open(inputPath + "scene.txt");
    fout1.open(outputPath + "stage1.txt");
    fout2.open(outputPath + "stage2.txt");
    fout3.open(outputPath + "stage3.txt");
    if(!fin.is_open()){
        cout << "Cant open scene.txt file\n";
        exit(0);
    }
    if(!fout1.is_open()){
        cout << "Cant open stage1.txt file\n";
        exit(0);
    }
    if(!fout2.is_open()){
        cout << "Cant open stage2.txt file\n";
        exit(0);
    }
    if(!fout3.is_open()){
        cout << "Cant open stage3.txt file\n";
        exit(0);
    }
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

    vector<Matrix<double> > triangles;


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
            triangles.push_back(out3);
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


    stage4(triangles);

    return 0;
}