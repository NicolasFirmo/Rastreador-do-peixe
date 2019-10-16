#include <include/Slider.h>

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