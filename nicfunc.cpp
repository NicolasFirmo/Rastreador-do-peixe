#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <string>
#include <iomanip>
#include <cmath>
#include <vector>
#include "nicfunc.h"

using namespace cv;
using namespace std;

// Detecta a posição do peixe na tela recebendo duas matrizes, uma que contenha a imagem em cores do peixe e outra que contenha uma imagem em escala de cinza que retonar em valores altos o que se move na tela.
// Fora as matrizes, também recebe um Rect por referência que determina a região de detecção e a manipula para otimizar o rastreamento na próxima chamada e um ponto P por referência que será a localização do do centro geométrico do peixe
void detectarpeixe(Mat a, Mat b, Rect &reg, Point &p)
{

  int soma = 0;
  Point loc(0, 0);

  threshold(a, a, mask_t, 255, THRESH_BINARY);
  // GaussianBlur( a, a, Size( 1, 1 ), 1.6, 1.6 );

  for (int i = reg.tl().y; i < reg.br().y; i++)
  {
    for (int j = reg.tl().x; j < reg.br().x; j++)
    {
      // parâmetros de decisão:
      // de acordo com as observações até o momento os melhores
      // valores para o exemplo em questão foram de 70 para valor
      // da máscara e 120 para o valor do canal azul da cor do peixe
      if ((a.at<Vec3b>(i, j)(0) > 40 &&
           a.at<Vec3b>(i, j)(1) > 40 &&
           a.at<Vec3b>(i, j)(2) > 40 &&
           b.at<Vec3b>(i, j)(0) < (peixe_V + peixe_OS)) &&
          b.at<Vec3b>(i, j)(0) > (peixe_V - peixe_OS))
      {
        if (loc == Point(0, 0))
        {
          loc = Point(j, i);
          soma = 1;
        }
        else
        {
          loc.x += j * (a.at<Vec3b>(i, j)(0) + a.at<Vec3b>(i, j)(1) + a.at<Vec3b>(i, j)(2));
          loc.y += i * (a.at<Vec3b>(i, j)(0) + a.at<Vec3b>(i, j)(1) + a.at<Vec3b>(i, j)(2));
          soma += (a.at<Vec3b>(i, j)(0) + a.at<Vec3b>(i, j)(1) + a.at<Vec3b>(i, j)(2));
        }
      }
    }
  }

  // cout << "soma: " << soma << " ";

  if (loc == Point(0, 0) || soma < soma_t)
  {
    // *reg = Rect(0,0,a.cols,a.rows);
    // *reg = Rect(p->x-50,p->y-50,100,100);
    reg = Rect(p.x - reg.width / 2, p.y - reg.height / 2, reg.width + 30, reg.height + 30);
  }
  else
  {
    reg.width = det_tam;
    reg.height = det_tam;
    p.x = loc.x / soma < a.cols ? loc.x / soma : a.cols / 2;
    p.y = loc.y / soma < a.rows ? loc.y / soma : a.rows / 2;
    reg.x = (p.x - det_tam / 2) < 0 ? 0 : (p.x - det_tam / 2);
    reg.x = reg.y + det_tam >= a.cols ? a.cols - det_tam - 1 : reg.x;
    reg.y = (p.y - det_tam / 2) < 0 ? 0 : (p.y - det_tam / 2);
    reg.y = reg.y + det_tam >= a.rows ? a.rows - det_tam - 1 : reg.y;
  }
  return;
}
void desenha_cruz(Point &c, Mat a, Vec3b cor)
{
  for (int v = -10; v < 10; v++)
  {
    a.at<Vec3b>(c.y + v, c.x) = cor;
    a.at<Vec3b>(c.y, c.x + v) = cor;
  }
}
void lookup(Mat a, Mat lut, Mat b)
{
  for (int i = 0; i < a.rows; i++)
  {
    for (int j = 0; j < a.cols; j++)
    {
      b.at<Vec3b>(i, j) = lut.at<Vec3b>(0, a.at<ushort>(i, j));
    }
  }
}

void desenhaMdC(const unsigned int &i, ofstream &outdata, Trajetoria<10> trjt, Mat mapa_de_calor, Mat circulo, Mat mcu, Mat lut, Mat mcu_aux, int &E)
{
  for (unsigned int j = 0; j < i; j++)
  {
    outdata << trjt.front().pos.x << "\t" << trjt.front().pos.y << "\t" << trjt.front().tempo << "\t" << trjt.front().vel << endl;
    add(mapa_de_calor(Rect(trjt.front().pos.x - circulo.cols / 2, trjt.front().pos.y - circulo.rows / 2, circulo.cols, circulo.rows)), circulo, mapa_de_calor(Rect(trjt.front().pos.x - circulo.cols / 2, trjt.front().pos.y - circulo.rows / 2, circulo.cols, circulo.rows)));
    trjt.pop();
  }
  normalize(mapa_de_calor, mcu, 0, 65536, NORM_MINMAX, CV_16U);
  lookup(mcu, lut, mcu_aux);

  E = 0;
  return;
}

void atualizabg(Rect &atbg, Point &peixe, Mat mov_ant, Mat mov, Mat mov_aux, Mat video, Mat bg, Rect &regiao)
{
  if (!atbg.contains(peixe))
  {
    absdiff(mov_ant, mov, mov_aux);
    threshold(mov_aux, mov_aux, mov_thr, 255, THRESH_BINARY_INV);
    video.copyTo(bg, mov_aux);
    mov_ant = mov.clone();
    atbg.x = regiao.x;
    atbg.y = regiao.y;
  }
}
