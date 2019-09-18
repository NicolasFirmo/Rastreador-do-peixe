#include "Switch.h"

Switch::Switch(Mat img_t, Mat img_f, void (*func)(bool &, void *), Point pos, Mat tela, void *var, const bool &sw_down) : Botao(img_t, img_f, func, pos, tela, var)
{
  setApertado(sw_down);
}

void Switch::aperta(int x, int y)
{
  if (hb.contains(Point(x, y)))
  {
    this->apertado != true ? this->setApertado(true) : this->setApertado(false);
  }
}