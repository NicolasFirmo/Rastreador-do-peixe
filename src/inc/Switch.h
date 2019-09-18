#ifndef SWITCH_H
#define SWITCH_H

#include "Botao.h"

using namespace cv;
using namespace std;

class Switch : public Botao
{
public:
  Switch(Mat img_t, Mat img_f, void (*func)(bool &, void *), Point pos, Mat tela, void *var, const bool &sw_down);

  void aperta(int x, int y);
};

#endif