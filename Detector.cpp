// #define NDEBUG
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <string>
#include <iomanip>
#include <cmath>
#include <vector>
#include <thread>
#include <functional>
#include "GUI.h"
#include "nicfunc.h"
#include <chrono>
#include <unistd.h>

using namespace cv;
using namespace std;

static thread *desenha = nullptr;
static thread *WMCU = nullptr;
static unsigned int t_WMCU = 3600; // segundos

static const string path_data = "./data/";
static const string path_GUI = "./src/GUI/";
static const string path_exs = "./src/Exemplos/";

static std::string timePointAsString(const std::chrono::high_resolution_clock::time_point &t)
{
  std::time_t tt = std::chrono::high_resolution_clock::to_time_t(t);
  std::string ts = std::ctime(&tt);
  ts.resize(ts.size() - 1);
  return ts;
}

void WriteMcU(Mat mapa_de_calor, Mat mcu_aux, int &j, bool &WR, Mat legenda, unsigned long &t_desenhandoMCU)
{
  using namespace std::this_thread;     // sleep_for, sleep_until
  using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
  auto t = std::chrono::high_resolution_clock::now();

  if (WR)
  {
    WR = false;
  }
  else
  {
    WR = true;
  }

  if (!j)
  {
    sleep_until(t + 1s);
    j++;
    WR = true;
    return;
  }

  double max;
  minMaxLoc(mapa_de_calor, NULL, &max);

  legenda.copyTo(mcu_aux(Rect(Point(20, mapa_de_calor.rows - 20 - legenda.rows), legenda.size())));

  for (int i = 0; i <= 10 * 10000; i += 10000)
  {
    int NdD = 0;
    int result = (((int)max) * (i / 10)) / t_desenhandoMCU;
    int test = result;
    // cout << "i = " << i << endl
    //      << endl;
    // cout << "((int)max): " << ((int)max) << endl;
    // cout << "(i / 9): " << (i / 9) << endl;
    // cout << "(((int)max) * (i / 9)): " << (((int)max) * (i / 9)) << endl;
    // cout << "((int)max): " << (((int)max) * (i / 9)) / t_desenhandoMCU << endl;
    // cout << string(5, '\n');

    do
    {
      test /= 10;
      NdD++;
    } while (test);

    // cout << "NdD: " << NdD << endl;
    // cout << "result: " << max << endl;

    string numero = "-" + string(5 - NdD, ' ');
    numero.insert(6 - NdD, to_string(result));
    numero.insert(4, ",");
    numero.insert(7, "%");
    putText(mcu_aux, i ? numero : "-  0,00%", Point(18, mapa_de_calor.rows - 8 + -i / 10000 * 33), CV_FONT_HERSHEY_PLAIN, 1.2, Scalar(255, 255, 255));
    assert(result <= 10000);
  }
  string nome = timePointAsString(std::chrono::high_resolution_clock::now()) + " | Mdc_" + ".png";
  nome.insert(nome.size() - 4, WR ? "FINAL" : to_string(j));
  // imwrite(nome, mcu_aux);
  imwrite(path_data + "img/" + nome, mcu_aux);
  // imwrite("img/"+nome, mcu_aux);
  // imwrite("/img/"+nome, mcu_aux);
  // imwrite("/home/nicolas/Projetos/Rastrear peixe/img/"+nome, mcu_aux);

  j++;
  // cout << "t_DEs " << t_desenhandoMCU << endl;
  // cout << "max " << max << endl;
  sleep_until(t + 1s);
  t_desenhandoMCU = 0;
  mapa_de_calor = Mat::zeros(mapa_de_calor.size(), mapa_de_calor.type());
  WR = true;
  return;
}

//------------------------------Funções dos elementos da GUI------------------------------

void bt_rect(bool &b, void *userdata)
{
  if (b)
  {
    *((bool *)userdata) = true;
  }
  return;
}

void bt_a(bool &b, void *userdata)
{
  if (b)
  {
    *((char *)userdata) = 0;
  }
  return;
}
void bt_b(bool &b, void *userdata)
{
  if (b)
  {
    *((char *)userdata) = 1;
  }
  return;
}
void bt_c(bool &b, void *userdata)
{
  if (b)
  {
    *((char *)userdata) = 2;
  }
  return;
}
void bt_d(bool &b, void *userdata)
{
  if (b)
  {
    *((char *)userdata) = 3;
  }
  return;
}
void sw_mira(bool &b, void *userdata)
{
  if (b)
  {
    *((char *)userdata) = true;
  }
  else
  {
    *((char *)userdata) = false;
  }
  return;
}
void bt_sl(bool &b, void *userdata)
{
  if (b)
  {
    cout << "SLIDER_>>>>>" << *((float *)userdata) << endl;
  }
  return;
}

int main(int argc, char **argv)
{
  using namespace std::chrono;

  // Matrizes
  // VideoCapture cap(path_exs+"Video 177.wmv");
  VideoCapture cap(0);
  const int width = cap.get(CAP_PROP_FRAME_WIDTH);
  const int height = cap.get(CAP_PROP_FRAME_HEIGHT);
  Mat video(height, width, CV_8UC3), bg(height, width, CV_8UC3), mov(height, width, CV_8UC3), mov_aux(height, width, CV_8UC3), mov_ant(height, width, CV_8UC3), mcu(height, width, CV_8UC3), mcu_aux(height, width, CV_8UC3);
  Mat mapa_de_calor(height, width, CV_16UC1, Scalar(0));
  Mat princ(height, width + 200, CV_8UC3, Scalar(255, 230, 200));
  Mat princ_bg(height, width + 200, CV_8UC3, Scalar(255, 230, 200));
  Mat lut = imread(path_GUI + "mapa_de_cor.png");
  Mat legenda_IMG = imread(path_GUI + "Legenda_img.png");

  // variáveis de controle
  bool mcuWriteReady;
  Point peixe(width / 2, height / 2);
  Rect regiao((height - det_tam) / 2, (width - det_tam) / 2, det_tam, det_tam);
  Rect atbg((height - det_tam) / 2, (width - det_tam) / 2, det_tam, det_tam);
  unsigned int i = 0;
  int j = 0;
  char key;
  unsigned long t_desenhandoMCU = 1; // segundos

  // variáveis de dado
  Trajetoria<10> trjt;
  // Rect Reg_det[2]; Área de varredura
  // Reg_det[0] = Rect(23,259,600,80);
  // Reg_det[1] = Rect(281,13,80,240);
  Rect r1(281, 13, 80, 240);
  Rect r2(23, 259, 250, 80);
  Rect r3(366, 259, 250, 80);
  double t1 = 0, t2 = 0, t3 = 0;
  ofstream outdata; // outdata is like cin

  //variáveis da GUI
  GUI gg;
  float cor_de_mel = 0;
  char sw_tela = 0;
  bool desenha_gui = true;
  Mat circulo(det_tam, det_tam, CV_16UC1, Scalar(0));
  circle(circulo, Point(circulo.rows / 2, circulo.rows / 2), det_tam / 6, 1, -1, 8, 0);

  cout << fixed;
  cout << setprecision(2);
  outdata << fixed;
  outdata << setprecision(2);

  outdata.open(path_data + "dados.txt", ios::trunc); // abre o arquivo de salvamento dos dados
  if (!outdata)
  { // erro se o arquivo não pôde ser aberto
    cerr << "Error: file could not be opened" << endl;
    exit(1);
  }

  // Tenta abrir webcam caso não seja carregado video
  if (!cap.isOpened())
  {
    cap.open(0);
    if (!cap.isOpened())
    {
      cerr << "cameras indisponiveis";
      return -1;
    }
  }

  // Configura janelas e a posição delas na tela
  // namedWindow("mascara",1);
  // moveWindow("mascara", 700,350);
  // namedWindow("video",1);
  // moveWindow("video", 20,20);
  // namedWindow("mapa_de_calor",1);
  // moveWindow("mapa_de_calor", 700,20);
  namedWindow("princ", 1);

  //--------------------------------------------------Inicialização--------------------------------------------------

  high_resolution_clock::time_point t = high_resolution_clock::now();
  outdata << timePointAsString(t) << endl;

  cap >> video;
  bg = video.clone();
  absdiff(bg, video, mov);
  mov_ant = mov.clone();
  atbg.x = regiao.x;
  atbg.y = regiao.y;

  Botao btRect(imread(path_GUI + "Botao/bt1.png", IMREAD_UNCHANGED), imread(path_GUI + "Botao/bt2.png", IMREAD_UNCHANGED), bt_rect, Point(10, 210), princ, &gg.cria_rect);
  Botao bta(imread(path_GUI + "Botao/bt1.png", IMREAD_UNCHANGED), imread(path_GUI + "Botao/bt2.png", IMREAD_UNCHANGED), bt_a, Point(10, 10), princ, &sw_tela);
  Botao btb(imread(path_GUI + "Botao/bt1.png", IMREAD_UNCHANGED), imread(path_GUI + "Botao/bt2.png", IMREAD_UNCHANGED), bt_b, Point(10, 50), princ, &sw_tela);
  Botao btc(imread(path_GUI + "Botao/bt1.png", IMREAD_UNCHANGED), imread(path_GUI + "Botao/bt2.png", IMREAD_UNCHANGED), bt_c, Point(10, 90), princ, &sw_tela);
  Botao btd(imread(path_GUI + "Botao/bt1.png", IMREAD_UNCHANGED), imread(path_GUI + "Botao/bt2.png", IMREAD_UNCHANGED), bt_d, Point(10, 130), princ, &sw_tela);
  Switch btmira(imread(path_GUI + "Botao/bt1.png", IMREAD_UNCHANGED), imread(path_GUI + "Botao/bt2.png", IMREAD_UNCHANGED), sw_mira, Point(10, 170), princ, &desenha_gui, true);
  Slider btsl(imread(path_GUI + "Slider/bt3.png", IMREAD_UNCHANGED), imread(path_GUI + "Slider/bt4.png", IMREAD_UNCHANGED), imread(path_GUI + "Slider/slm.png", IMREAD_UNCHANGED), bt_sl, Point(100, 10), 100, 0.5, princ, &cor_de_mel, 0, 255);
  gg.insert(&btRect);
  gg.insert(&bta);
  gg.insert(&btb);
  gg.insert(&btc);
  gg.insert(&btd);
  gg.insert(&btmira);
  gg.insert(&btsl);
  setMouseCallback("princ", gui_func, &gg); //Não posso mais de uma callback por tela

  while (true)
  {
    cap >> video;
    absdiff(bg, video, mov);

    key = (char)waitKey(10);
    if (key == 27)
    {
      outdata << timePointAsString(high_resolution_clock::now()) << endl;
      outdata.close();
      WriteMcU(mapa_de_calor, mcu_aux.clone(), j, mcuWriteReady = false, legenda_IMG, t_desenhandoMCU);
      break; // pressionar esc para sair
    } else if (key == 'd')
    {
     cout<<"del\n";
      gg.pop_rect();
    } else if (key == 'c')
    {
      cout<<"cria_rect\n";
      gg.cria_rect = true;
    }
    

    // cout << "atualiza bg\n";
    if (!atbg.contains(peixe))
    {
      absdiff(mov_ant, mov, mov_aux);
      threshold(mov_aux, mov_aux, mov_thr, 255, THRESH_BINARY_INV);
      video.copyTo(bg, mov_aux);
      mov_ant = mov.clone();
      assert(regiao.x >= 0);
      assert(regiao.y >= 0);
      assert(regiao.x + det_tam < video.cols);
      assert(regiao.y + det_tam < video.cols);
      atbg.x = regiao.x;
      atbg.y = regiao.y;
      // cout << "terminou de atualizar bg\n";
    }

    assert(peixe.x >= 0);
    assert(peixe.y >= 0);
    assert(peixe.x < video.cols);
    assert(peixe.y < video.rows);

    if (i >= trjt_size && mcuWriteReady)
    { // desenha o mapa de calor
      if (desenha != nullptr)
      {
        desenha->join();
        delete desenha;
      }
      desenha = new thread(desenhaMdC, i, std::ref(outdata), trjt, mapa_de_calor, circulo, mcu, lut, mcu_aux, std::ref(t_desenhandoMCU));
      trjt.reset();
      i = 0;
    }

    if (r1.contains(peixe))
    {
      t1++;
    }
    else if (r2.contains(peixe))
    {
      t2++;
    }
    else if (r3.contains(peixe))
    {
      t3++;
    }

    // cout<<"Detecta peixe\n";
    detectarpeixe(mov, video, regiao, peixe);
    // cout<<"Detectou\n";
    // cout<<"trjt.push\n";
    trjt.push(peixe, (duration_cast<duration<double>>(high_resolution_clock::now() - t)).count());
    // cout<<"trjt.pushou\n";

    // cout<<"atualiza princ\n";
    switch (sw_tela)
    {
    case 0:
      princ_bg.copyTo(princ);
      video.copyTo(princ(Rect(200, 0, princ.cols - 200, princ.rows)));
      break;
    case 1:
      princ_bg.copyTo(princ);
      mov.copyTo(princ(Rect(200, 0, princ.cols - 200, princ.rows)));
      break;
    case 2:
      princ_bg.copyTo(princ);
      mcu_aux.copyTo(princ(Rect(200, 0, princ.cols - 200, princ.rows)));
      break;
    case 3:
      princ_bg.copyTo(princ);
      bg.copyTo(princ(Rect(200, 0, princ.cols - 200, princ.rows)));
      break;
    default:
      break;
    }
    // cout<<"princ atualizado\n";

    gg.exe();
    gg.mostrar();
    // cout<<"gui mostrado\n";

    if (desenha_gui)
    {
      desenha_cruz(peixe, princ(Rect(200, 0, width, height)), {0, 0, (unsigned char)cor_de_mel});
      rectangle(princ, regiao + Point(200, 0), Scalar(0, 255, 0), 1, LINE_8, 0);
      rectangle(princ, atbg + Point(200, 0), Scalar(255, 0, 255), 1, LINE_8, 0);
      rectangle(princ, r1 + Point(200, 0), Scalar(255, 0, 0), 1, LINE_8, 0);
      rectangle(princ, r2 + Point(200, 0), Scalar(255, 0, 0), 1, LINE_8, 0);
      rectangle(princ, r3 + Point(200, 0), Scalar(255, 0, 0), 1, LINE_8, 0);
      cv::line(princ, Point(230, 350), Point(230 + trjt.back().vel / 2, 350), Scalar(0, 0, 255), 5, 8, 0); //velocímetro
    }
    // cout<<"gui desenhado\n";

    // imshow("mascara", mov);
    // imshow("video", video);
    imshow("princ", princ);

    // if(i>0){vel_at = sqrt( pow(trjt[i].pos.x - trjt[i-1].pos.x , 2) + pow(trjt[i].pos.y - trjt[i-1].pos.y , 2) ) / ( trjt[i-1].tempo - trjt[i].tempo );}
    // vel = (vel<vel_at && vel_at<500)  ? vel_at : vel;
    //
    // cout<<"tempo em cima: "<<t1/40<<"s | tempo na esquerda: "<<t2/40<<"s | tempo na direita: "<<t3/40<<"s "<<"vel: "<<vel_at<<endl;

    // cout << "segundos: " << (int)trjt.back().tempo << " vel: " << trjt.back().vel << "\n";
    // outdata << timePointAsString(high_resolution_clock::now()) << endl;

    i++;
    if ((duration_cast<duration<int>>(high_resolution_clock::now() - t)).count() % t_WMCU == 0 && mcuWriteReady)
    {
      WMCU = new thread(WriteMcU, mapa_de_calor, mcu_aux.clone(), std::ref(j), std::ref(mcuWriteReady), legenda_IMG, std::ref(t_desenhandoMCU));
    }
  }

  return 0;
}

// salva imagens periodicamente
// if(i%60==0){
//   nome = ".png";
//   nome.insert(0,to_string(j));
//   imwrite(nome, img);
//   j++;
// }
