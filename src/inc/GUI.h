#ifndef GUI_H
#define GUI_H

using namespace cv;
using namespace std;

#include <vector>
#include <list>
#include "Botao.h"
#include "Slider.h"
#include "Switch.h"

class GUI
{
public:
  vector<Botao *> botoes;
  vector<Slider *> sliders;
  vector<Switch *> switches;
  vector<Rect *> rects;
  Rect *c_rect;
  int s_rect_it;
  bool cria_rect;

  void insert(Botao *bt);
  void insert(Slider *sl);
  void insert(Switch *sw);

  GUI();

  void create_rect(int x, int y);

  void select_rect(int x, int y);

  void scale_rect(int x, int y);

  void save_rect();

  void pop_rect();

  void mostrar();

  void exe();

  void aperta_botao(int x, int y);

  void solta_botao();

  void aperta_switch(int x, int y);

  void aperta_slider(int x, int y);

  void solta_slider();

  Slider *getApertado_slider();
};

void gui_func(int event, int x, int y, int flags, void *userdata);
void segurando(int event, int x, int y, int flags, void *userdata);

#endif
