#ifndef NICFUNC_H
#define NICFUNC_H

int const mask_t = 30;
int const peixe_V = 85;
int const peixe_OS = 40;
int const det_tam = 40;
int const soma_t = 3000;
int const mov_thr = 10;
int const vel_rep = 1;
int const vel_info_at = 1;
int const trjt_size = 100;

using namespace cv;
using namespace std;

void detectarpeixe(Mat a, Mat b, Rect& reg, Point& p);
void desenha_cruz(Point& c, Mat a, Vec3b cor);
void lookup(Mat a, Mat lut, Mat b);

#endif
