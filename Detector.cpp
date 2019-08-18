#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

bool detectarpeixe(Mat a, Mat b) {
  for (int i = 70; i < 370; i++) {
    for (int j = 290; j < 590; j++) {
      // parâmetros de decisão:
      // de acordo com as observações até o momento os melhores
      // valores para o exemplo em questão foram de 70 para valor
      // da máscara e 120 para o valor do canal azul da cor do peixe
      if ((a.at<uchar>(i,j) > 70) && (b.at<Vec3b>(i,j)(3) < 120)) {
        for (int v = -10; v < 10; v++) {
          b.at<Vec3b>(i+v,j) = {0,0,255};
          b.at<Vec3b>(i,j+v) = {0,0,255};
        }
        return true;
      }
    }
  }
  return false;
}

int main(int argc, char** argv){
  bool d0 = false, d1 = false; //flags para avisar se o está dentro ou fora de detecção
  VideoCapture cap("Exemplo/cima.wmv"); // arquivo de vídeo ou imagem da webcam
  char key;
  Mat video, videoaux, img, bg, auxbg ,mov;
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
  cvtColor(video, videoaux, CV_BGR2GRAY);
  videoaux.convertTo(videoaux, CV_8U);
  bg = videoaux.clone();

  while (true) {
    cap >> video;
    cvtColor(video, img, CV_BGR2GRAY);
    img.convertTo(img, CV_8U);
    absdiff(bg, img, mov);

    // desenha na tela um quadrado representando a seção de rastreamento
    for (int i = 1; i < 300; i++) {
      video.at<Vec3b>(70+i,290) = {255,0,000};
      video.at<Vec3b>(70,290+i) = {255,0,000};
      video.at<Vec3b>(370,290+i) = {255,0,000};
      video.at<Vec3b>(70+i,590) = {255,0,000};
    }

    key = (char) waitKey(10);
    if( key == 27 ) break; // pressionar esc para sair

    d0 = detectarpeixe(mov, video);
    if (d0 != d1) {
      d1 = d0;
      if (d0) {
        cout << "Peixe visto" << '\n';
      } else {
        cout << "Peixe desvisto" << '\n';
      }
    }

    imshow("video", video);
    imshow("mascara", mov);

    if(i%100==0){
      videoaux = img.clone();
      if(p){
        absdiff(auxbg,mov,img);
        for (int k = 0; k < bg.rows; k++) {
          for (int l = 0; l < bg.cols; l++) {
            if (img.at<uchar>(k,l) < 5) {
              bg.at<uchar>(k,l) = videoaux.at<uchar>(k,l);
            }
          }
        }
      }
      auxbg = mov.clone();
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
