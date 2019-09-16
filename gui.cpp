#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <string>
#include <iomanip>
#include <cmath>
#include <vector>
#include "gui.h"

using namespace cv;
using namespace std;

Botao::Botao(Mat img_t, Mat img_f, void (*func)(bool &, void *), Point pos, Mat tela, void *var) : tela(tela),
                                                                                                   var(var),
                                                                                                   func(func)
{
  if (img_t.type() == 16)
    cvtColor(img_t, img_t, CV_BGR2BGRA);
  vector<Mat> bgra_t;
  split(img_t, bgra_t);
  cvtColor(bgra_t[3], bgra_t[3], CV_GRAY2BGR);
  this->alpha_t = bgra_t[3];
  cvtColor(img_t, img_t, CV_BGRA2BGR);
  this->img_t = img_t;

  if (img_f.type() == 16)
    cvtColor(img_f, img_f, CV_BGR2BGRA);
  vector<Mat> bgra_f;
  split(img_f, bgra_f);
  cvtColor(bgra_f[3], bgra_f[3], CV_GRAY2BGR);
  this->alpha_f = bgra_f[3];
  cvtColor(img_f, img_f, CV_BGRA2BGR);
  this->img_f = img_f;

  this->img_at = this->img_f;
  this->alpha_at = this->alpha_f;
  this->setApertado(false);
  int x = pos.x + img_at.cols > tela.cols ? tela.cols - img_at.cols : pos.x;
  int y = pos.y + img_at.rows > tela.rows ? tela.rows - img_at.rows : pos.y;
  this->hb = Rect(x < 0 ? 0 : x, y < 0 ? 0 : y, img_at.cols, img_at.rows);
  this->telaRect = tela(Rect(this->hb.tl().x, this->hb.tl().y, img_at.cols, img_at.rows));
}

void Botao::setPos(int x, int y)
{
  hb = Rect(x, y, hb.width, hb.height);
  this->setTelaRect();
}

void Botao::setPos(Point p)
{
  hb = Rect(p.x, p.y, hb.width, hb.height);
  this->setTelaRect();
}

void Botao::setTelaRect()
{
  telaRect = tela(Rect(hb.tl().x, hb.tl().y, img_at.cols, img_at.rows));
}

void Botao::mostrar()
{

  subtract(telaRect, alpha_at, telaRect);

  cvtColor(alpha_at, alpha_at, CV_BGR2GRAY);

  add(img_at, telaRect, telaRect, alpha_at);

  cvtColor(alpha_at, alpha_at, CV_GRAY2BGR);
}

void Botao::exe()
{
  func(apertado, var);
}

void Botao::aperta(int x, int y)
{
  if (hb.contains(Point(x, y)))
  {
    this->setApertado(true);
  }
}

void Botao::solta()
{
  this->setApertado(false);
}

void Botao::setApertado(bool b)
{
  apertado = b;
  if (b)
  {
    img_at = img_t;
    alpha_at = alpha_t;
  }
  else
  {
    img_at = img_f;
    alpha_at = alpha_f;
  }
}

Slider::Slider(Mat img_t, Mat img_f, Mat slm, void (*func)(bool &, void *), Point pos, int len, float bt_pos, Mat tela, void *var, int min, int max) : Botao(img_t, img_f, func, Point(pos.x, pos.y + len * bt_pos), tela, var),
                                                                                                                                                       slm(slm),
                                                                                                                                                       len(len),
                                                                                                                                                       min(min),
                                                                                                                                                       max(max),
                                                                                                                                                       var((float *)var)
{
  int x = pos.x + img_at.cols > tela.cols ? tela.cols - img_at.cols : pos.x;
  int y = pos.y + img_at.rows + len > tela.rows ? tela.rows - img_at.rows - len : pos.y;
  this->sl = Rect(x < 0 ? 0 : x, y < 0 ? 0 : y, img_at.cols, len + img_at.rows);
  this->v = hb.tl().y - img_at.rows / 2;
  *(this->var) = (min + max * ((v - sl.tl().y + img_at.rows / 2)) / len);
}

void Slider::mostrar()
{
  for (int i = 0; i < this->sl.height - img_at.rows; i++)
  {
    this->slm.copyTo(tela(Rect(this->sl.tl().x + (img_t.cols - slm.cols) / 2, this->sl.tl().y + i + img_at.rows / 2, this->slm.cols, this->slm.rows)));
  }
  Botao::mostrar();
}

void Slider::aperta(int x, int y)
{
  if (sl.contains(Point(x, y)))
  {
    this->setApertado(true);
    this->setVar(x, y);
  }
  else if (apertado)
  {
    this->setVar(x, y);
  }
}

void Slider::setVar(int x, int y)
{
  this->setPos(hb.tl().x, (y - img_at.rows / 2) >= sl.tl().y ? ((y + img_at.rows / 2) <= sl.br().y ? (y - img_at.rows / 2) : sl.br().y - img_at.rows) : sl.tl().y);
  this->v = hb.tl().y - img_at.rows / 2;
  *var = (min + max * ((v - sl.tl().y + img_at.rows / 2)) / len);
}

void Slider::exe()
{
  func(apertado, var);
}

Switch::Switch(Mat img_t, Mat img_f, void (*func)(bool &, void *), Point pos, Mat tela, void *var, const bool &sw_down) : Botao(img_t, img_f, func, pos, tela, var) {
  setApertado(sw_down);
}

void Switch::aperta(int x, int y)
{
  if (hb.contains(Point(x, y)))
  {
    this->apertado != true ? this->setApertado(true) : this->setApertado(false);
  }
}

GUI::GUI()
{
  botoes.reserve(10);
  switches.reserve(10);
  sliders.reserve(10);
}

void GUI::insert(Botao *bt)
{
  this->botoes.emplace_back(bt);
}
void GUI::insert(Slider *sl)
{
  this->sliders.emplace_back(sl);
}
void GUI::insert(Switch *sw)
{
  this->switches.emplace_back(sw);
}

void GUI::mostrar()
{
  for (unsigned i = 0; i < botoes.size(); i++)
  {
    this->botoes[i]->mostrar();
  }
  for (unsigned i = 0; i < sliders.size(); i++)
  {
    this->sliders[i]->mostrar();
  }
  for (unsigned i = 0; i < switches.size(); i++)
  {
    this->switches[i]->mostrar();
  }
}

void GUI::exe()
{
  for (unsigned i = 0; i < botoes.size(); i++)
  {
    this->botoes[i]->exe();
  }
  for (unsigned i = 0; i < sliders.size(); i++)
  {
    this->sliders[i]->exe();
  }
  for (unsigned i = 0; i < switches.size(); i++)
  {
    this->switches[i]->exe();
  }
}

void GUI::aperta_botao(int x, int y)
{
  for (unsigned i = 0; i < botoes.size(); i++)
  {
    this->botoes[i]->aperta(x, y);
  }
}

void GUI::solta_botao()
{
  for (unsigned i = 0; i < botoes.size(); i++)
  {
    this->botoes[i]->solta();
  }
}

void GUI::aperta_switch(int x, int y)
{
  for (unsigned i = 0; i < switches.size(); i++)
  {
    this->switches[i]->aperta(x, y);
  }
}

void GUI::aperta_slider(int x, int y)
{
  for (unsigned i = 0; i < sliders.size(); i++)
  {
    this->sliders[i]->aperta(x, y);
  }
}

void GUI::solta_slider()
{
  for (unsigned i = 0; i < sliders.size(); i++)
  {
    this->sliders[i]->solta();
  }
}

Slider *GUI::getApertado_slider()
{
  for (unsigned i = 0; i < sliders.size(); i++)
  {
    if (this->sliders[i]->apertado)
      return this->sliders[i];
  }
  return NULL;
}

void gui_func(int event, int x, int y, int flags, void *userdata)
{
  GUI *gui = (GUI *)userdata;

  if (event == EVENT_LBUTTONDOWN)
  {
    gui->aperta_botao(x, y);
    gui->aperta_slider(x, y);
    gui->aperta_switch(x, y);
  }

  if (event == EVENT_LBUTTONUP)
  {
    gui->solta_botao();
    gui->solta_slider();
  }

  if (event == EVENT_MOUSEMOVE)
  {
    if (gui->getApertado_slider())
    {
      gui->getApertado_slider()->aperta(x, y);
    }
  }
}
