#ifndef NICFUNC_H
#define NICFUNC_H

using namespace cv;
using namespace std;

void detectarpeixe(Mat a, Mat b, Rect* reg, Point* p);
void desenha_cruz(Point c, Mat a, Vec3b cor);
void lookup(Mat a, Mat lut, Mat b);

#endif
