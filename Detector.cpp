#include <iostream>
#include <fstream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <string>
#include <iomanip>
#include <cmath>
#include <vector>
#include "gui.h"
#include "nicfunc.h"

using namespace cv;
using namespace std;

struct Dado {
  Point pos;
  float tempo;
};

//------------------------------Funções dos elementos da GUI------------------------------

void bt_a(bool& b, void* userdata) {
  if(b){*((char*) userdata) = 0;}
  return;
}
void bt_b(bool& b, void* userdata) {
  if(b){*((char*) userdata) = 1;}
  return;
}
void bt_c(bool& b, void* userdata) {
  if(b){*((char*) userdata) = 2;}
  return;
}
void bt_sl(bool& b, void* userdata) {
  if(b){
    cout<<"SLIDER_>>>>>"<<*((float*) userdata)<<endl;
  }
  return;
}

int main(int argc, char** argv){

  // Matrizes
  VideoCapture cap("Exemplo/Video 177.wmv");
  // VideoCapture cap(0);
  int height = cap.get(CAP_PROP_FRAME_HEIGHT);
  int width = cap.get(CAP_PROP_FRAME_WIDTH);
  Mat video, bg, mov, mov_aux, mov_ant, mcu, mcu_aux(height, width, CV_8UC3);
  Mat mapa_de_calor(height, width, CV_16UC1, Scalar(0));
  Mat princ(height, width+200, CV_8UC3, Scalar(255,230,200));
  Mat princ_bg(height, width+200, CV_8UC3, Scalar(255,230,200));
  Mat lut = imread("mapa_de_cor.png");

  // variáveis de controle
  Point peixe(width/2,height/2);
  Rect regiao(height - det_tam/2, width - det_tam/2, det_tam, det_tam);
  Rect atbg(height - det_tam/2, width - det_tam/2, det_tam, det_tam);
  int i=0;
  int fps = FPS;
  float mspf = 1000/fps;
  char key;

  // variáveis de dado
  float vel=0, vel_at=0;
  Dado trjt[trjt_size];
  // Rect Reg_det[2]; Área de varredura
  // Reg_det[0] = Rect(23,259,600,80);
  // Reg_det[1] = Rect(281,13,80,240);
  Rect r1(281,13,80,240);
  Rect r2(23,259,250,80);
  Rect r3(366,259,250,80);
  double t1 = 0, t2 = 0, t3 = 0, t=0;
  ofstream outdata; // outdata is like cin

  //variáveis da GUI
  GUI gg;
  float cor_de_mel = 0;
  char sw_tela = 0;
  Mat circulo(det_tam, det_tam, CV_16UC1, Scalar(0));
  circle(circulo, Point(circulo.rows/2, circulo.rows/2), det_tam/4, 5, -1, 8, 0);
  GaussianBlur( circulo, circulo, Size(15, 15), 0, 0);

  cout << fixed;
  cout << setprecision(2);
  outdata << fixed;
  outdata << setprecision(2);

  outdata.open("dados.txt",ios::trunc); // abre o arquivo de salvamento dos dados
  if( !outdata ) { // erro se o arquivo não pôde ser aberto
    cerr << "Error: file could not be opened" << endl;
    exit(1);
  }

  // Tenta abrir webcam caso não seja carregado video
  if(!cap.isOpened()){
    cap.open(0);
    if(!cap.isOpened()){
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
  namedWindow("princ",1);

  //--------------------------------------------------Inicialização--------------------------------------------------

  cap >> video;
  bg = video.clone();
  absdiff(bg, video, mov);
  mov_ant = mov.clone();
  atbg.x = regiao.x;
  atbg.y = regiao.y;

  Botao bta(imread("GUI/Botao/bt1.png",IMREAD_UNCHANGED),imread("GUI/Botao/bt2.png",IMREAD_UNCHANGED),bt_a,Point(10,10),princ,&sw_tela);
  Botao btb(imread("GUI/Botao/bt1.png",IMREAD_UNCHANGED),imread("GUI/Botao/bt2.png",IMREAD_UNCHANGED),bt_b,Point(10,50),princ,&sw_tela);
  Botao btc(imread("GUI/Botao/bt1.png",IMREAD_UNCHANGED),imread("GUI/Botao/bt2.png",IMREAD_UNCHANGED),bt_c,Point(10,90),princ,&sw_tela);
  // Switch btc(imread("GUI/Botao/bt1.png",IMREAD_UNCHANGED),imread("GUI/Botao/bt2.png",IMREAD_UNCHANGED),bt_c,Point(10,100),video);
  Slider btsl(imread("GUI/Slider/bt3.png",IMREAD_UNCHANGED),imread("GUI/Slider/bt4.png",IMREAD_UNCHANGED),imread("GUI/Slider/slm.png",IMREAD_UNCHANGED),bt_sl,Point(100,10),100,1,princ,&cor_de_mel,0,255);
  gg.insert(&bta);
  gg.insert(&btb);
  gg.insert(&btc);
  gg.insert(&btsl);
  setMouseCallback("princ", gui_func, &gg); //Não posso mais de uma callback por tela


  while (true) {
    cap >> video;
    absdiff(bg, video, mov);

    key = (char) waitKey(mspf);
    if( key == 27 ){
      outdata << "velocidade máxima: " << vel;
      outdata << "fps " << fps;
      outdata.close();
      imwrite("MdC.png", mcu_aux);
      break; // pressionar esc para sair
    }

    if(!atbg.contains(peixe)){
      absdiff(mov_ant,mov,mov_aux);
      threshold(mov_aux,mov_aux,mov_thr,255,THRESH_BINARY_INV);
      video.copyTo(bg, mov_aux);
      mov_ant = mov.clone();
      atbg.x = regiao.x;
      atbg.y = regiao.y;
    }

    if (i == trjt_size) { // desenha o mapa de calor
      for (int j = 0; j < i; j++) {
        outdata << trjt[j].pos.x << "\t" << trjt[j].pos.y<< "\t" << (t+j-i)/fps << endl;
        add(mapa_de_calor(Rect(trjt[j].pos.x - circulo.cols/2,trjt[j].pos.y - circulo.rows/2,circulo.cols,circulo.rows)), circulo, mapa_de_calor(Rect(trjt[j].pos.x - circulo.cols/2,trjt[j].pos.y - circulo.rows/2,circulo.cols,circulo.rows)));
      }
      normalize(mapa_de_calor, mcu, 0, 65536, NORM_MINMAX, CV_16U);
      lookup(mcu,lut,mcu_aux);
      i = 0;
    }

    if (r1.contains(peixe)){
      t1++;
    } else if (r2.contains(peixe)) {
      t2++;
    } else if (r3.contains(peixe)) {
      t3++;
    }

    detectarpeixe(mov, video, regiao, peixe);
    desenha_cruz(peixe, video, {0,0,(unsigned char)cor_de_mel});
    rectangle(video, regiao, Scalar(0,255,0), 1, LINE_8, 0);
    rectangle(video, atbg, Scalar(255,0,255), 1, LINE_8, 0);
    rectangle(video, r1, Scalar(255,0,0), 1, LINE_8, 0);
    rectangle(video, r2, Scalar(255,0,0), 1, LINE_8, 0);
    rectangle(video, r3, Scalar(255,0,0), 1, LINE_8, 0);

    line(video, Point(30,350), Point(30+vel_at/5,350), Scalar(0,0,255), 5,8,0); //velocímetro

    switch (sw_tela) {
      case 0:
      princ_bg.copyTo(princ);
      video.copyTo(princ(Rect(200,0,princ.cols-200,princ.rows)));
      break;
      case 1:
      princ_bg.copyTo(princ);
      mov.copyTo(princ(Rect(200,0,princ.cols-200,princ.rows)));
      break;
      case 2:
      princ_bg.copyTo(princ);
      mcu_aux.copyTo(princ(Rect(200,0,princ.cols-200,princ.rows)));
      break;
      default:
      break;
    }

    gg.exe();
    gg.mostrar();

    // imshow("mascara", mov);
    // imshow("video", video);
    imshow("princ", princ);

    // if(i>0){vel_at = sqrt( pow(trjt[i].pos.x - trjt[i-1].pos.x , 2) + pow(trjt[i].pos.y - trjt[i-1].pos.y , 2) ) / ( trjt[i-1].tempo - trjt[i].tempo );}
    // vel = (vel<vel_at && vel_at<500)  ? vel_at : vel;
    //
    // cout<<"tempo em cima: "<<t1/40<<"s | tempo na esquerda: "<<t2/40<<"s | tempo na direita: "<<t3/40<<"s "<<"vel: "<<vel_at<<endl;

    trjt[i].pos = peixe;
    trjt[i].tempo = t/40;

    i++;
    t++;
  }

  return 0;
}

//salva imagens periodicamente
// if(i%60==0){
//   nome = ".png";
//   nome.insert(0,to_string(j));
//   imwrite(nome, img);
//   j++;
// }
