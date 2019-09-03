#ifndef NICFUNC_H
#define NICFUNC_H

#define mask_t 30
#define peixe_V 85
#define peixe_OS 40
#define det_tam 40
#define soma_t 3000
#define mov_thr 10
#define vel_rep 1
#define vel_info_at 1

using namespace cv;
using namespace std;

void detectarpeixe(Mat a, Mat b, Rect* reg, Point* p);
void desenha_cruz(Point c, Mat a, Vec3b cor);
void lookup(Mat a, Mat lut, Mat b);

#endif
