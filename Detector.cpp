#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

bool detectarpeixe(Mat a, Mat b) {
  for (int i = 70; i < 370; i++) {
    for (int j = 290; j < 590; j++) {
      if ((a.at<uchar>(i,j) > 30) && (b.at<Vec3b>(i,j)(3) < 200)) {
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
  bool d0 = false, d1 = false;
  VideoCapture cap("cima.wmv"); // arquivo de vídeo ou imagem da webcam
  char key;
  Mat video, img, bg, mov;


  if(!cap.isOpened()){
    cap.open(0);
    if(!cap.isOpened()){
      cout << "cameras indisponiveis";
      return -1;
    }
  }

  namedWindow("video",1);
  namedWindow("img",1);

  // int i=0, j=0; // trecho para fazer o salvamento das imagens para montar o bg
  // string nome = ".png";

  cvtColor(imread("cimabg.png",CV_BGR2GRAY), bg, CV_BGR2GRAY) // imagem de fundo
  bg.convertTo(bg, CV_8U);

  while (true) {
    cap >> video;
    cvtColor(video, img, CV_BGR2GRAY);
    img.convertTo(img, CV_8U);
    absdiff(bg, img, mov);
    for (int i = 1; i < 300; i++) {
      video.at<Vec3b>(70+i,290) = {255,0,000};
      video.at<Vec3b>(70,290+i) = {255,0,000};
      video.at<Vec3b>(370,290+i) = {255,0,000};
      video.at<Vec3b>(70+i,590) = {255,0,000};
    }
    key = (char) waitKey(30);
    if( key == 27 ) break; // pressionar esc
    d0 = detectarpeixe(mov, video);
    if (d0 != d1) {
      d1 = d0;
      if (d0) {
        cout << "Etiqueta vista" << '\n';
      } else {
        cout << "Etiqueta desvista" << '\n';
      }
    }

    imshow("video", video);
    imshow("img", mov);

    // if(i%60==0){ //salva imagens periodicamente para montar back ground
    //   nome = ".png";
    //   nome.insert(0,to_string(j));
    //   imwrite(nome, video);
    //   j++;
    // }

    i++;
  }

  return 0;
}
