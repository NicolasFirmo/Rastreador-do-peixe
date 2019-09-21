#ifndef GUI_H

#include <vector>
#include <chrono>
#include <thread>
#include <functional>
#include "nicfunc.h"
#include "Botao.h"
#include "Slider.h"
#include "Switch.h"
#define GUI_H

using namespace cv;
using namespace std;

enum clickmode
{
  CRIA_RECT,
  SELECT_RECT,
  POINT_PEIXE
};

struct Regiao
{
  Rect *rect;
  unsigned char type;
  std::chrono::nanoseconds t;
  std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::nanoseconds> t0;
  bool contemPeixe;

  inline Regiao(Rect *r) : rect(r), type(4), t(0), t0(std::chrono::high_resolution_clock::now()), contemPeixe(false) {}
};

class GUI
{
public:
  Point &p;
  Rect &reg;
  vector<Botao *> botoes;
  vector<Slider *> sliders;
  vector<Switch *> switches;
  vector<Regiao> rects;
  Rect *c_rect;
  int s_rect_it;
  clickmode click_mode;

  void insert(Botao *bt);
  void insert(Slider *sl);
  void insert(Switch *sw);

  GUI(Point &peixe, Rect &regiao);

  void create_rect(int x, int y);

  void select_rect(int x, int y);

  void scale_rect(int x, int y);

  void save_rect();

  void pop_rect();

  void setSetectedType(unsigned char type);

  void computaTempo(const Point &p);

  void zeraCont();

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
