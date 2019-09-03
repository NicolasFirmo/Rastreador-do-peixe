#ifndef GUI_H
#define GUI_H

using namespace cv;
using namespace std;

class Botao{
  public:
  Mat img_at;
  Mat img_t;
  Mat img_f;
  Mat alpha_at;
  Mat alpha_t;
  Mat alpha_f;
  Mat tela;
  Mat telaRect;
  void (*func)(bool,void*);
  Rect hb;
  bool apertado;
  void* var;

  Botao(Mat img_t, Mat img_f, void (*func)(bool,void*), Point pos, Mat tela, void* var);

  void setPos(int x,int y);

  void mostrar();

  void exe();

  void aperta(int x, int y);

  void solta();

  void setApertado(bool b);

};

class Slider : public Botao{
  public:
  Mat slm;
  Rect sl;
  int len;
  int v;
  float* var;
  int min, max;

  Slider(Mat img_t, Mat img_f, Mat slm, void (*func)(bool, void*), Point pos, int len, float bt_pos, Mat tela, void* var, int min, int max);

  void mostrar();

  void aperta(int x, int y);

  void exe();

};
class Switch : public Botao{
  public:

  Switch(Mat img_t, Mat img_f, void (*func)(bool,void*), Point pos, Mat tela, void* var);

  void aperta(int x, int y);

};

class GUI{
  public:
  vector<Botao*> botoes;
  vector<Slider*> sliders;
  vector<Switch*> switches;

  void insert(Botao* bt);
  void insert(Slider* sl);
  void insert(Switch* sw);

  void mostrar();

  void exe();

  void aperta_botao(int x, int y);

  void solta_botao();

  void aperta_switch(int x, int y);

  void aperta_slider(int x, int y);

  void solta_slider();

  Slider* getApertado_slider();
};

void gui_func(int event, int x, int y, int flags, void* userdata);

#endif
