#include "Botao.h"

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