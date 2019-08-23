#include <iostream>
#include <fstream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <string>
#include <iomanip>
#include <cmath>

using namespace cv;
using namespace std;

struct Dado {
  Point pos;
  float tempo;
};

#define mask_t 25
#define peixe_V 85
#define peixe_OS 50
#define det_tam 30

void detectarpeixe(Mat a, Mat b, Rect* reg, Point* p);
void desenha_cruz(Point c, Mat a, Vec3b cor);
void desenha_regiao(Rect* reg, Mat a, Vec3b cor);

int main(int argc, char** argv){
  int i=0;
  float vel=0, vel_at=0;
  // VideoCapture cap("Exemplo/cima.wmv"); // arquivo de vídeo ou imagem da webcam
  VideoCapture cap("Video 177.wmv");
  char key;
  Mat video, bg, mov,mov_aux,mov_ant;
  bool p = false;
  Point peixe;
  Dado trjt[100];
  Rect interesse[2];
  interesse[0] = Rect(259,23,80,600);
  interesse[1] = Rect(13,281,240,80);
  Rect r1(13,281,240,80);
  Rect r2(259,23,80,250);
  Rect r3(259,366,80,250);
  double t1 = 0, t2 = 0, t3 = 0, t=0;
  ofstream outdata; // outdata is like cin

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
  namedWindow("video",1);
  moveWindow("video", 700,20);
  namedWindow("mascara",1);
  moveWindow("mascara", 20,350);
  namedWindow("rastro",1);
  moveWindow("rastro", 20,20);

  cap >> video;
  bg = video.clone();
  Mat rastro(video.rows, video.cols, video.type(), Scalar(0,0,0));
  // Rect regiao(0,0,video.rows,video.cols);
  Rect regiao(63,300,200,300);
  Rect atbg(0,0,det_tam,det_tam);
  desenha_regiao(&r1, rastro, {255,0,0});
  desenha_regiao(&r2, rastro, {255,0,0});
  desenha_regiao(&r3, rastro, {255,0,0});

  while (true) {
    cap >> video;
    absdiff(bg, video, mov);
    rastro.at<Vec3b>(peixe.x,peixe.y) = {0,255,255};

    key = (char) waitKey(20);
    if( key == 27 ){
      outdata << "velocidade máxima: " << vel;
      outdata.close();
      break; // pressionar esc para sair
    }

    if(i == 100 || !atbg.contains(peixe)){
      if(p){
        absdiff(mov_ant,mov,mov_aux);
        threshold(mov_aux,mov_aux,10,255,THRESH_BINARY_INV);
        video.copyTo(bg, mov_aux);
      }

      for (int j = 0; j < i; j++) { //salva no arquivo
        outdata << trjt[j].pos.y<< "\t" << trjt[j].pos.x<< "\t" << (t+j-i)/40 << endl;
      }

      mov_ant = mov.clone();
      atbg.x = regiao.x;
      atbg.y = regiao.y;
      p = true;
      i = 0;
    }

    if (r1.contains(peixe)) {
      t1++;
    } else if (r2.contains(peixe)) {
      t2++;
    } else if (r3.contains(peixe)) {
      t3++;
    }

    detectarpeixe(mov, video, &regiao, &peixe);
    desenha_cruz(peixe, video, {0,0,255});
    desenha_regiao(&regiao, video, {0,255,0});
    desenha_regiao(&atbg, video, {255,0,255});
    desenha_regiao(&r1, video, {255,0,0});
    desenha_regiao(&r2, video, {255,0,0});
    desenha_regiao(&r3, video, {255,0,0});

    // rastro.copyTo(video, rastro);

    imshow("mascara", mov);
    imshow("video", video);
    imshow("rastro", rastro);

    if(i>0){vel_at = sqrt(pow(trjt[i].pos.x-trjt[i-1].pos.x,2)+pow(trjt[i].pos.y-trjt[i-1].pos.y,2))/(trjt[i-1].tempo-trjt[i].tempo);}
    vel = vel<vel_at ? vel_at : vel;

    cout<<"tempo em cima: "<<t1/40<<"s | tempo na esquerda: "<<t2/40<<"s | tempo na direita: "<<t3/40<<"s "<<"vel: "<<vel_at/10<<endl;

    trjt[i].pos = peixe;
    trjt[i].tempo = t/40;

    i++;
    t++;
  }

  return 0;
}

void detectarpeixe(Mat a, Mat b, Rect* reg, Point* p) {

  threshold(a,a,mask_t,255,THRESH_BINARY);
  GaussianBlur( a, a, Size( 5, 5 ), 1, 1 );

  for (int i = reg->tl().x; i < reg->br().x; i++) {
    for (int j = reg->tl().y; j < reg->br().y; j++) {
      // parâmetros de decisão:
      // de acordo com as observações até o momento os melhores
      // valores para o exemplo em questão foram de 70 para valor
      // da máscara e 120 para o valor do canal azul da cor do peixe
      if ((a.at<Vec3b>(i,j)(0) > 180 &&
      a.at<Vec3b>(i,j)(1) > 180 &&
      a.at<Vec3b>(i,j)(2) > 180 &&
      b.at<Vec3b>(i,j)(0) < (peixe_V + peixe_OS)) &&
      b.at<Vec3b>(i,j)(0) > (peixe_V - peixe_OS))
      {
        reg->width = det_tam;
        reg->height = det_tam;
        reg->x = (i - det_tam/2) < 0 ? 0 : (i -  det_tam/2);
        reg->y = (j - det_tam/2) < 0 ? 0 : (j  -  det_tam/2);
        *p = Point(i,j);
        cout << "sim" << endl;
        return;
      }
    }
  }
  *reg = Rect(0,0,a.rows,a.cols);
  // *reg = Rect(p->x-50,p->y-50,100,100);
  // *reg = Rect(p->x - reg->width/2,p->y - reg->height/2,reg->width + 40,reg->height + 40);
  return;
}

void desenha_cruz(Point c, Mat a, Vec3b cor){
  for (int v = -10; v < 10; v++) {
    a.at<Vec3b>(c.x+v,c.y) = cor;
    a.at<Vec3b>(c.x,c.y+v) = cor;
  }
}

void desenha_regiao(Rect* reg, Mat a, Vec3b cor){
  for (int i = reg->tl().x; i < reg->br().x; i++){
    a.at<Vec3b>(i,reg->tl().y) = cor;
    a.at<Vec3b>(i,reg->br().y) = cor;
  }

  for (int j = reg->tl().y; j < reg->br().y; j++) {
    a.at<Vec3b>(reg->tl().x,j) = cor;
    a.at<Vec3b>(reg->br().x,j) = cor;

  }
}

//salva imagens periodicamente
// if(i%60==0){
//   nome = ".png";
//   nome.insert(0,to_string(j));
//   imwrite(nome, img);
//   j++;
// }
