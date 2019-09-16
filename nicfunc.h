// É a função do leita, e o que eu quero dizer com leita?
// E eu vo dizê é?

#ifndef NICFUNC_H
#define NICFUNC_H
#include <cmath>

static unsigned int const mask_t = 30;
static unsigned int const peixe_V = 85;
static unsigned int const peixe_OS = 40;
static int const det_tam = 40;
static unsigned int const mov_thr = 10;
static unsigned int const vel_rep = 1;
static unsigned int const vel_info_at = 1;
static unsigned int const trjt_size = 5;

using namespace cv;
using namespace std;

void detectarpeixe(Mat a, Mat b, Rect &reg, Point &p);
void desenha_cruz(Point &c, Mat a, Vec3b cor);
void lookup(Mat a, Mat lut, Mat b);

struct Dado
{
  Point pos;
  double tempo;
  double vel;
};

template <int tam>
struct Trajetoria
{
  queue<Dado> m_Dados;

private:
  double m_velBuffer[tam];
  unsigned int m_Bufferindx;

public:
  Trajetoria() : m_Bufferindx(0)
  {
    for (unsigned int i = 0; i < tam; i++)
      m_velBuffer[i] = 0;
    m_Dados.push({Point(100, 100), 0, 0});
  }

  void push(const Point pos, const double tempo)
  {
    insertVelBufer(pos, tempo);
    double vel = 0;
    for (unsigned int i = 0; i < tam; i++)
    {
      vel += m_velBuffer[i] / tam;
    }
    m_Dados.push({pos, tempo, vel});
    atbufferindx();
  }

  void insertVelBufer(const Point &pos, const double &tempo)
  {
    m_velBuffer[m_Bufferindx] = sqrt(pow(pos.x - m_Dados.back().pos.x, 2) + pow(pos.y - m_Dados.back().pos.y, 2)) / (tempo - m_Dados.back().tempo);
  }

  void reset()
  {
    while (!m_Dados.empty())
    {
      m_Dados.pop();
    }
  }

  inline Dado front() { return m_Dados.front(); }
  inline Dado back() { return m_Dados.back(); }
  inline void pop() { m_Dados.pop(); }

private:
  inline void atbufferindx()
  {
    m_Bufferindx == tam - 1 ? m_Bufferindx = 0 : m_Bufferindx++;
    return;
  }
};

void desenhaMdC(const unsigned int &i, ofstream &outdata, Trajetoria<10> trjt, Mat mapa_de_calor, Mat circulo, Mat mcu, Mat lut, Mat mcu_aux);
#endif
