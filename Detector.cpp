#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <iomanip>

using namespace cv;
using namespace std;

int mask_t = 40;
int peixe_V = 85;
int peixe_OS = 20;
int det_tam = 30;

void detectarpeixe(Mat a, Mat b, Rect* reg, Point* p) {
  for (int i = reg->tl().x; i < reg->br().x; i++) {
    for (int j = reg->tl().y; j < reg->br().y; j++) {
      // parâmetros de decisão:
      // de acordo com as observações até o momento os melhores
      // valores para o exemplo em questão foram de 70 para valor
      // da máscara e 120 para o valor do canal azul da cor do peixe
      if ((a.at<Vec3b>(i,j)(0) > mask_t &&
      a.at<Vec3b>(i,j)(1) > mask_t &&
      a.at<Vec3b>(i,j)(2) > mask_t &&
      b.at<Vec3b>(i,j)(0) < (peixe_V + peixe_OS)) &&
      b.at<Vec3b>(i,j)(0) > (peixe_V - peixe_OS)) {
        reg->width = det_tam;
        reg->height = det_tam;
        reg->x = (i - det_tam/2) < 0 ? 0 : (i -  det_tam/2);
        reg->y = (j - det_tam/2) < 0 ? 0 : (j  -  det_tam/2);
        *p = Point(i,j);
        return;
      }
    }
  }
  // *reg = Rect(0,0,a.rows,a.cols);
  *reg = Rect(p->x-50,p->y-50,100,100);
  // *reg = Rect(p->x - reg->width/2,p->y - reg->height/2,reg->width + 40,reg->height + 40);
  return;
}

void desenha_cruz(Point c, Mat a){
  for (int v = -10; v < 10; v++) {
    a.at<Vec3b>(c.x+v,c.y) = {0,0,255};
    a.at<Vec3b>(c.x,c.y+v) = {0,0,255};
  }
}


void desenha_regiao(Rect* reg, Mat a){
  for (int i = reg->tl().x; i < reg->br().x; i++){
    a.at<Vec3b>(i,reg->tl().y) = {255,0,255};
    a.at<Vec3b>(i,reg->br().y) = {255,0,255};
  }

  for (int j = reg->tl().y; j < reg->br().y; j++) {
    a.at<Vec3b>(reg->tl().x,j) = {255,0,255};
    a.at<Vec3b>(reg->br().x,j) = {255,0,255};

  }
}

void desenha_regiaoblue(Rect* reg, Mat a){
  for (int i = reg->tl().x; i < reg->br().x; i++){
    a.at<Vec3b>(i,reg->tl().y) = {255,0,0};
    a.at<Vec3b>(i,reg->br().y) = {255,0,0};
  }

  for (int j = reg->tl().y; j < reg->br().y; j++) {
    a.at<Vec3b>(reg->tl().x,j) = {255,0,0};
    a.at<Vec3b>(reg->br().x,j) = {255,0,0};

  }
}

int main(int argc, char** argv){
  cout << fixed;
  cout << setprecision(2);
  // VideoCapture cap("Exemplo/cima.wmv"); // arquivo de vídeo ou imagem da webcam
  VideoCapture cap("Video 177.wmv");
  char key;
  Mat video, bg, mov,mov_aux,mov_ant;
  bool p = false;
  Point peixe;
  Rect r1(13,281,240,80);
  Rect r2(259,23,80,250);
  Rect r3(259,366,80,250);
  double t1 = 0, t2 = 0, t3 = 0;

  if(!cap.isOpened()){
    cap.open(0);
    if(!cap.isOpened()){
      cout << "cameras indisponiveis";
      return -1;
    }
  }

  namedWindow("video",1);
  moveWindow("video", 700,20);
  namedWindow("mascara",1);
  moveWindow("mascara", 20,350);
  namedWindow("rastro",1);
  moveWindow("rastro", 20,20);

  // trecho para fazer o salvamento periódico de imagens
  int i=0, j=0;
  string nome = ".png";

  //carregamento de imagem de background [obsoleto depois do v1.1]
  // cvtColor(imread("cimabg.png",CV_BGR2GRAY), bg, CV_BGR2GRAY); // imagem de fundo
  // bg.convertTo(bg, CV_8U);

  cap >> video;
  bg = video.clone();
  Mat rastro(video.rows, video.cols, video.type(), Scalar(0,0,0));
  // Rect regiao(0,0,video.rows,video.cols);
  Rect regiao(63,300,200,300);
  Rect atbg(0,0,det_tam,det_tam);
  desenha_regiaoblue(&r1, rastro);
  desenha_regiaoblue(&r2, rastro);
  desenha_regiaoblue(&r3, rastro);

  while (true) {
    cap >> video;
    absdiff(bg, video, mov);
    rastro.at<Vec3b>(peixe.x,peixe.y) = {0,255,255};

    key = (char) waitKey(20);
    if( key == 27 ) break; // pressionar esc para sair

    // d0 = detectarpeixe(mov, video);
    // if (d0 != d1) {
    //   d1 = d0;
    //   if (d0) {
    //     cout << "Peixe visto" << '\n';
    //   } else {
    //     cout << "Peixe desvisto" << '\n';
    //   }
    // }

    if(i == 100 || !atbg.contains(peixe)){
      if(p){
        absdiff(mov_ant,mov,mov_aux);
        threshold(mov_aux,mov_aux,10,255,THRESH_BINARY_INV);
        video.copyTo(bg, mov_aux);
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

    cout<<"tempo em cima: "<<t1/40<<"s | tempo na esquerda: "<<t2/40<<"s | tempo na direita: "<<t3/40<<"s\n";

    detectarpeixe(mov, video, &regiao, &peixe);
    desenha_cruz(peixe, video);
    desenha_regiao(&regiao, video);
    desenha_regiao(&atbg, video);
    desenha_regiaoblue(&r1, video);
    desenha_regiaoblue(&r2, video);
    desenha_regiaoblue(&r3, video);

    // rastro.copyTo(video, rastro);

    imshow("mascara", mov);
    imshow("video", video);
    imshow("rastro", rastro);

    //salva imagens periodicamente
    // if(i%60==0){
    //   nome = ".png";
    //   nome.insert(0,to_string(j));
    //   imwrite(nome, img);
    //   j++;
    // }

    i++;
  }

  return 0;
}
