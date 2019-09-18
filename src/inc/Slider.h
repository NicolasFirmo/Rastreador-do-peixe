#ifndef SLIDER_H
#define SLIDER_H

#include <opencv2/opencv.hpp>
#include "Botao.h"

using namespace cv;
using namespace std;

class Slider : public Botao
{
public:
  Mat slm;
  Rect sl;
  int len;
  int v;
  int min;
  int max;
  float *var;

  Slider(Mat img_t, Mat img_f, Mat slm, void (*func)(bool &, void *), Point pos, int len, float bt_pos, Mat tela, void *var, int min, int max);

  void mostrar();

  void aperta(int x, int y);
  void setVar(int x, int y);

  void exe();
};

#endif