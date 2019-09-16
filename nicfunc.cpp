#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <string>
#include <iomanip>
#include <cmath>
#include <vector>
#include "nicfunc.h"
#define NDEBUG
#include <assert.h>

using namespace cv;
using namespace std;

static unsigned int const soma_t = 3000;

// Detecta a posição do peixe na tela recebendo duas matrizes, uma que contenha a imagem em cores do peixe e outra que contenha uma imagem em escala de cinza que retonar em valores altos o que se move na tela.
// Fora as matrizes, também recebe um Rect por referência que determina a região de detecção e a manipula para otimizar o rastreamento na próxima chamada e um ponto P por referência que será a localização do do centro geométrico do peixe
void detectarpeixe(Mat a, Mat b, Rect &reg, Point &p)
{
  float soma = 0;
  unsigned long locX = 0, locY = 0;
  // cout << "ANTES locX: " << locX << endl;
  // cout << "ANTES locY: " << locY << endl;

  threshold(a, a, mask_t, 255, THRESH_BINARY);
  // GaussianBlur( a, a, Size( 1, 1 ), 1.6, 1.6 );

  for (int i = reg.tl().y; i < reg.br().y; i++)
  {
    for (int j = reg.tl().x; j < reg.br().x; j++)
    {
      // cout << "for interno: " << endl;
      assert(i >= 0);
      assert(j >= 0);
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
        // cout << "avalização: " << endl;
        if (locX == 0 && locY == 0)
        {
          // cout << "if: " << endl;
          locX = j;
          locY = i;
          soma = 1;
        }
        else
        {
          // cout << "else: " << endl;
          assert((a.at<Vec3b>(i, j)(0) + a.at<Vec3b>(i, j)(1) + a.at<Vec3b>(i, j)(2)) > 0);
          // cout << "DURANTE locX: " << locX << " DURANTE locY: " << locY << " ij: " << i << "," << j << endl;
          locX += j * (a.at<Vec3b>(i, j)(0) + a.at<Vec3b>(i, j)(1) + a.at<Vec3b>(i, j)(2));
          locY += i * (a.at<Vec3b>(i, j)(0) + a.at<Vec3b>(i, j)(1) + a.at<Vec3b>(i, j)(2));
          soma += (a.at<Vec3b>(i, j)(0) + a.at<Vec3b>(i, j)(1) + a.at<Vec3b>(i, j)(2));
        }
      }
    }
  }

  if ((locX == 0 && locY == 0) || soma < soma_t)
  {
    int n_regX = reg.tl().x - 15 < 0 ? reg.tl().x - 1 < 0 ? 0 : reg.tl().x - 1 : reg.tl().x - 15;
    int n_regY = reg.tl().y - 15 < 0 ? reg.tl().y - 1 < 0 ? 0 : reg.tl().y - 1 : reg.tl().y - 15;
    int n_regW = reg.width + 30 + n_regX >= a.cols ? (reg.width + 1 + n_regX >= a.cols ? reg.width : reg.width + 1) : reg.width + 30;
    int n_regH = reg.height + 30 + n_regY >= a.rows ? (reg.height + 1 + n_regY >= a.rows ? reg.height : reg.height + 1) : reg.height + 30;
    // assert(n_regX < 0 || n_regX + n_regW >= a.cols);
    // assert(n_regY < 0 || n_regY + n_regH >= a.cols);
    reg = Rect(n_regX, n_regY, n_regW, n_regH);
  }
  else
  {
    // cout << "locX: " << locX << endl;
    // cout << "locY: " << locY << endl;
    // cout << "soma: " << soma << endl;
    p.x = locX / soma < a.cols ? locX / soma : a.cols / 2;
    p.y = locY / soma < a.rows ? locY / soma : a.rows / 2;
    // cout << "p.x: " << p.x << endl;
    // cout << "p.y: " << p.y << endl;
    assert(p.x >= 0);
    assert(p.y >= 0);
    assert(p.x < a.cols);
    assert(p.y < a.rows);
    int n_regX = (p.x - det_tam / 2) < 0 ? 0 : p.x + det_tam / 2 >= a.cols ? a.cols - det_tam - 1 : (p.x - det_tam / 2);
    int n_regY = (p.y - det_tam / 2) < 0 ? 0 : p.y + det_tam / 2 >= a.rows ? a.rows - det_tam - 1 : (p.y - det_tam / 2);
    assert(n_regX >= 0);
    assert(n_regY >= 0);
    assert(n_regX + det_tam < a.cols);
    assert(n_regY + det_tam < a.cols);
    reg.x = n_regX;
    reg.y = n_regY;
    reg.width = det_tam;
    reg.height = det_tam;
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

void desenhaMdC(const unsigned int &i, ofstream &outdata, Trajetoria<10> trjt, Mat mapa_de_calor, Mat circulo, Mat mcu, Mat lut, Mat mcu_aux)
{
  // cout << "desenha MdC\n";

  for (unsigned int j = 0; j < i; j++)
  {
    // cout << "pos x,y: " << trjt.front().pos.x << "," << trjt.front().pos.y << endl;
    int map_X = trjt.front().pos.x - circulo.cols / 2 <= 0 ? 0 : trjt.front().pos.x - circulo.cols / 2;
    int map_Y = trjt.front().pos.y - circulo.rows / 2 <= 0 ? 0 : trjt.front().pos.y - circulo.rows / 2;
    int c_X = trjt.front().pos.x - circulo.cols / 2 <= 0 ? trjt.front().pos.x : 0;
    int c_Y = trjt.front().pos.y - circulo.rows / 2 <= 0 ? trjt.front().pos.y : 0;
    int wid = trjt.front().pos.x + circulo.cols / 2 >= mapa_de_calor.cols ? mapa_de_calor.cols - trjt.front().pos.x : trjt.front().pos.x - circulo.cols / 2 <= 0 ? circulo.cols - trjt.front().pos.x : circulo.cols;
    int hei = trjt.front().pos.y + circulo.rows / 2 >= mapa_de_calor.rows ? mapa_de_calor.rows - trjt.front().pos.y : trjt.front().pos.y - circulo.rows / 2 <= 0 ? circulo.rows - trjt.front().pos.y : circulo.rows;
    
    outdata << trjt.front().pos.x << "\t" << trjt.front().pos.y << "\t" << trjt.front().tempo << "\t" << trjt.front().vel << endl;
    add(mapa_de_calor(Rect(map_X, map_Y, wid, hei)),
        circulo(Rect(c_X, c_Y, wid, hei)),
        mapa_de_calor(Rect(map_X, map_Y, wid, hei)));
    trjt.pop();
  }
  normalize(mapa_de_calor, mcu, 0, 65536, NORM_MINMAX, CV_16U);
  lookup(mcu, lut, mcu_aux);
  // cout << "terminou de desenhar MdC\n";

  return;
}
