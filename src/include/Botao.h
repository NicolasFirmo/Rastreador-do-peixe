#ifndef BOTAO_H
#define BOTAO_H

#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Botao
{
public:
  Mat img_t;
  Mat img_f;
  Mat img_at;
  Mat alpha_at;
  Mat alpha_t;
  Mat alpha_f;
  Mat tela;
  Mat telaRect;
  Rect hb;
  bool apertado;
  void *var;
  void (*func)(bool &, void *);

  Botao(Mat img_t, Mat img_f, void (*func)(bool &, void *), Point pos, Mat tela, void *var);

  void setPos(int x, int y);
  void setPos(Point p);

  void setTelaRect();

  void mostrar();

  void exe();

  void aperta(int x, int y);

  void solta();

  void setApertado(bool b);
};

#endif