#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

Mat detectarpeixe(Mat a, Mat b) {
  Mat aux = b.clone();
  for (int i = 0; i < a.rows; i++) {
    for (int j = 0; j < a.cols; j++) {
      // parâmetros de decisão:
      // de acordo com as observações até o momento os melhores
      // valores para o exemplo em questão foram de 70 para valor
      // da máscara e 120 para o valor do canal azul da cor do peixe
      if ((a.at<Vec3b>(i,j)(1) > 50) && (b.at<Vec3b>(i,j)(3) < 120)) {
        for (int v = -10; v < 10; v++) {
          aux.at<Vec3b>(i+v,j) = {0,0,255};
          aux.at<Vec3b>(i,j+v) = {0,0,255};
        }
        return aux;
      }
    }
  }
  return aux;
}

int main(int argc, char** argv){
  //VideoCapture cap("lado_limpo.avi"); // arquivo de vídeo ou imagem da webcam
    VideoCapture cap("Exemplo/cima.wmv");
  char key;
  Mat video, bg, mov,mov_aux,mov_ant;
  bool p = false;

  if(!cap.isOpened()){
    cap.open(0);
    if(!cap.isOpened()){
      cout << "cameras indisponiveis";
      return -1;
    }
  }

  namedWindow("video",1);
  namedWindow("mascara",1);

  // trecho para fazer o salvamento periódico de imagens
  int i=0, j=0;
  string nome = ".png";

  //carregamento de imagem de background [obsoleto depois do v1.1]
  // cvtColor(imread("cimabg.png",CV_BGR2GRAY), bg, CV_BGR2GRAY); // imagem de fundo
  // bg.convertTo(bg, CV_8U);

  cap >> video;
  bg = video.clone();

  while (true) {
    cap >> video;
    absdiff(bg, video, mov);

    // desenha na tela um quadrado representando a seção de rastreamento
    // for (int i = 1; i < 300; i++) {
    //   video.at<Vec3b>(70+i,290) = {255,0,000};
    //   video.at<Vec3b>(70,290+i) = {255,0,000};
    //   video.at<Vec3b>(370,290+i) = {255,0,000};
    //   video.at<Vec3b>(70+i,590) = {255,0,000};
    // }

    key = (char) waitKey(10);
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

    imshow("video", detectarpeixe(mov, video));
    imshow("mascara", mov);

    if(i%100==0){
      if(p){
        absdiff(mov_ant,mov,mov_aux);
        threshold(mov_aux,mov_aux,10,255,THRESH_BINARY_INV);
        video.copyTo(bg, mov_aux);
      }
      mov_ant = mov.clone();
      p = true;
    }

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
