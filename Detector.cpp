#include <iostream>
#include <fstream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <string>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace cv;
using namespace std;

class Botao{
  public:
  Mat img_at;
  Mat img_t;
  Mat img_f;
  Mat alpha_at;
  Mat alpha_t;
  Mat alpha_f;
  Mat tela;
  Mat telaRect;
  void (*func)(bool,void*);
  Rect hb;
  bool apertado;
  void* var;

  Botao(Mat img_t, Mat img_f, void (*func)(bool,void*), Point pos, Mat tela, void* var){
    this->var = var;
    if(img_t.type() == 16)cvtColor(img_t, img_t, CV_BGR2BGRA);
    vector<Mat> bgra_t;
    split(img_t, bgra_t);
    cvtColor(bgra_t[3], bgra_t[3], CV_GRAY2BGR);
    this->alpha_t = bgra_t[3];
    cvtColor(img_t, img_t, CV_BGRA2BGR);
    this->img_t = img_t;

    if(img_f.type() == 16)cvtColor(img_f, img_f, CV_BGR2BGRA);
    vector<Mat> bgra_f;
    split(img_f, bgra_f);
    cvtColor(bgra_f[3], bgra_f[3], CV_GRAY2BGR);
    this->alpha_f = bgra_f[3];
    cvtColor(img_f, img_f, CV_BGRA2BGR);
    this->img_f = img_f;

    this->img_at = this->img_f;
    this->alpha_at = this->alpha_f;
    this->setApertado(false);
    this->func = func;
    this->tela = tela;
    int x = pos.x + img_at.cols>tela.cols ? tela.cols - img_at.cols : pos.x;
    int y = pos.y + img_at.rows>tela.rows ? tela.rows - img_at.rows : pos.y;
    this->hb = Rect(x < 0 ? 0 : x,y < 0 ? 0 : y,img_at.cols,img_at.rows);
    this->telaRect = tela(Rect(this->hb.tl().x,this->hb.tl().y,img_at.cols,img_at.rows));
  }

  void setPos(int x,int y) {
    hb = Rect(x,y,hb.width,hb.height);
  }

  void mostrar(){
    this->telaRect = tela(Rect(this->hb.tl().x,this->hb.tl().y,img_at.cols,img_at.rows));

    subtract(this->telaRect, this->alpha_at, this->telaRect);

    cvtColor(this->alpha_at, this->alpha_at, CV_BGR2GRAY);

    add(img_at, this->telaRect, this->telaRect,this->alpha_at);

    cvtColor(this->alpha_at, this->alpha_at, CV_GRAY2BGR);
  }

  void exe(){
    func(apertado,var);
  }

  void aperta(int x, int y){
    if(hb.contains(Point(x,y))){
      this->setApertado(true);
    }
  }

  void solta(){
    this->setApertado(false);
  }

  void setApertado(bool b){
    this->apertado = b;
    if (b) {
      this->img_at = this->img_t;
      this->alpha_at = this->alpha_t;
    } else {
      this->img_at = this->img_f;
      this->alpha_at = this->alpha_f;
    }
  }


};
class Slider : public Botao{
  public:
  Mat slm;
  Rect sl;
  int len;
  int v;
  float* var;
  int min, max;

  Slider(Mat img_t, Mat img_f, Mat slm, void (*func)(bool, void*), Point pos, int len, float bt_pos, Mat tela, void* var, int min, int max) : Botao(img_t,img_f,func,Point(pos.x,pos.y+len*bt_pos-img_t.rows/2),tela, var){
    this->min = min;
    this->max = max;
    this->slm = slm;
    this->var = (float*) var;
    int x = pos.x + img_at.cols>tela.cols ? tela.cols - img_at.cols : pos.x;
    int y = pos.y + img_at.rows + len > tela.rows ? tela.rows - img_at.rows - len : pos.y;
    this->sl = Rect(x < 0 ? 0 : x,y < 0 ? 0 : y,img_at.cols,len+img_at.rows);
    this->len = len;
  }

  void mostrar(){
    for (int i = 0; i < this->sl.height-img_at.rows; i++) {
      this->slm.copyTo(tela(Rect(this->sl.tl().x+(img_t.cols-slm.cols)/2,this->sl.tl().y + i + img_at.rows/2,this->slm.cols, this->slm.rows)));
    }
    Botao::mostrar();
  }

  void aperta(int x, int y){
    if(sl.contains(Point(x,y))){
      this->setApertado(true);
      this->setPos(hb.tl().x,(y-img_at.rows/2)>=sl.tl().y?((y+img_at.rows/2)<=sl.br().y?(y-img_at.rows/2):sl.br().y-img_at.rows) : sl.tl().y);
      this->v = hb.tl().y-img_at.rows/2;
      *var = (min + max*((v-sl.tl().y+img_at.rows/2))/len);
    }else if(Rect(0,sl.tl().y,tela.cols,sl.height).contains(Point(x,y)) && apertado){
      this->setPos(hb.tl().x,(y-img_at.rows/2)>=sl.tl().y?((y+img_at.rows/2)<=sl.br().y?(y-img_at.rows/2):sl.br().y-img_at.rows) : sl.tl().y);
      this->v = hb.tl().y-img_at.rows/2;
      *var = (min + max*((v-sl.tl().y+img_at.rows/2))/len);
    }
  }

  void exe(){
    func(apertado,var);
  }

};
class Switch : public Botao{
  public:

  Switch(Mat img_t, Mat img_f, void (*func)(bool,void*), Point pos, Mat tela, void* var) : Botao(img_t,img_f,func,pos,tela, var){}

  void aperta(int x, int y){
    if(hb.contains(Point(x,y))){
      this->apertado != true ? this->setApertado(true) : this->setApertado(false);
    }
  }

};

class GUI{
  public:
  vector<Botao*> botoes;
  vector<Slider*> sliders;
  vector<Switch*> switches;

  void insert(Botao* bt){
    this->botoes.push_back(bt);
  }
  void insert(Slider* sl){
    this->sliders.push_back(sl);
  }
  void insert(Switch* sw){
    this->switches.push_back(sw);
  }

  void mostrar(){
    for (unsigned i = 0; i < botoes.size(); i++) {
      this->botoes[i]->mostrar();
      }
    for (unsigned i = 0; i < sliders.size(); i++) {
      this->sliders[i]->mostrar();
      }
    for (unsigned i = 0; i < switches.size(); i++) {
      this->switches[i]->mostrar();
      }
  }

  void exe(){
    for (unsigned i = 0; i < botoes.size(); i++) {
      this->botoes[i]->exe();
      }
    for (unsigned i = 0; i < sliders.size(); i++) {
      this->sliders[i]->exe();
      }
    for (unsigned i = 0; i < switches.size(); i++) {
      this->switches[i]->exe();
      }
  }

  void aperta_botao(int x, int y){
    for (unsigned i = 0; i < botoes.size(); i++) {
      this->botoes[i]->aperta(x,y);
    }
  }

  void solta_botao(){
    for (unsigned i = 0; i < botoes.size(); i++) {
      this->botoes[i]->solta();
    }
  }

  void aperta_switch(int x, int y){
    for (unsigned i = 0; i < switches.size(); i++) {
      this->switches[i]->aperta(x,y);
    }
  }

  void aperta_slider(int x, int y){
    for (unsigned i = 0; i < sliders.size(); i++) {
      this->sliders[i]->aperta(x,y);
    }
  }

  void solta_slider(){
    for (unsigned i = 0; i < sliders.size(); i++) {
      this->sliders[i]->solta();
    }
  }

  Slider* getApertado_slider(){
    for (unsigned i = 0; i < sliders.size(); i++) {
      if(this->sliders[i]->apertado)return this->sliders[i];
    }
    return NULL;
  }
};

struct Dado {
  Point pos;
  float tempo;
};

#define mask_t 30
#define peixe_V 85
#define peixe_OS 40
#define det_tam 40
#define soma_t 3000
#define mov_thr 10
#define vel_rep 1
#define vel_info_at 1
#define FPS 60 //ver como pega fps em opencv 3.3.1


void detectarpeixe(Mat a, Mat b, Rect* reg, Point* p) {

  int soma = 0;
  Point loc(0,0);

  threshold(a,a,mask_t,255,THRESH_BINARY);
  // GaussianBlur( a, a, Size( 1, 1 ), 1.6, 1.6 );

  for (int i = reg->tl().y; i < reg->br().y; i++) {
    for (int j = reg->tl().x; j < reg->br().x; j++) {
      // parâmetros de decisão:
      // de acordo com as observações até o momento os melhores
      // valores para o exemplo em questão foram de 70 para valor
      // da máscara e 120 para o valor do canal azul da cor do peixe
      if ((a.at<Vec3b>(i,j)(0) > 40 &&
      a.at<Vec3b>(i,j)(1) > 40 &&
      a.at<Vec3b>(i,j)(2) > 40 &&
      b.at<Vec3b>(i,j)(0) < (peixe_V + peixe_OS)) &&
      b.at<Vec3b>(i,j)(0) > (peixe_V - peixe_OS))
      {
        if (loc == Point(0,0)) {
          loc = Point(j,i);
          soma = 1;
        }else{
          loc.x += j*(a.at<Vec3b>(i,j)(0)+a.at<Vec3b>(i,j)(1)+a.at<Vec3b>(i,j)(2));
          loc.y += i*(a.at<Vec3b>(i,j)(0)+a.at<Vec3b>(i,j)(1)+a.at<Vec3b>(i,j)(2));
          soma+=(a.at<Vec3b>(i,j)(0)+a.at<Vec3b>(i,j)(1)+a.at<Vec3b>(i,j)(2));
        }
      }
    }
  }
  // cout << "soma: " << soma << " ";

  if (loc == Point(0,0) || soma < soma_t) {
    // *reg = Rect(0,0,a.cols,a.rows);
    // *reg = Rect(p->x-50,p->y-50,100,100);
    *reg = Rect(p->x - reg->width/2,p->y - reg->height/2,reg->width + 25,reg->height + 25);
    return;
  }

  reg->width = det_tam;
  reg->height = det_tam;
  p->x = loc.x/soma < a.cols ? loc.x/soma : a.cols/2;
  p->y = loc.y/soma < a.rows ? loc.y/soma : a.rows/2;
  reg->x = (p->x - det_tam/2) < 0 ? 0 : (p->x -  det_tam/2);
  reg->x = reg->y+det_tam >= a.cols ? a.cols - det_tam - 1: reg->x;
  reg->y = (p->y - det_tam/2) < 0 ? 0 : (p->y  -  det_tam/2);
  reg->y = reg->y+det_tam >= a.rows ? a.rows - det_tam - 1: reg->y;
  return;

};
void desenha_cruz(Point c, Mat a, Vec3b cor){
  for (int v = -10; v < 10; v++) {
    a.at<Vec3b>(c.y+v,c.x) = cor;
    a.at<Vec3b>(c.y,c.x+v) = cor;
  }
};
void lookup(Mat a, Mat lut, Mat b) {
  for (int i = 0; i < a.rows; i++) {
    for (int j = 0; j < a.cols; j++) {
      b.at<Vec3b>(i,j) = lut.at<Vec3b>(0,a.at<ushort>(i,j));
    }
  }
};

void gui_func(int event, int x, int y, int flags, void* userdata){
  GUI* gui = (GUI*) userdata;

  if (event == EVENT_LBUTTONDOWN) {
    gui->aperta_botao(x,y);
    gui->aperta_slider(x,y);
    gui->aperta_switch(x,y);
  }

  if (event == EVENT_LBUTTONUP) {
    gui->solta_botao();
    gui->solta_slider();
  }

  if (event == EVENT_MOUSEMOVE) {
    if(gui->getApertado_slider()){
      gui->getApertado_slider()->aperta(x,y);
    }
  }


}

//------------------------------Funções dos elementos da GUI------------------------------

void bt_a(bool b, void* userdata) {
  char* a = (char*) userdata;
  if(b){
    cout<<"BOTAO AAAAA CARAI";
    *a = 0;
  }
  return;
}
void bt_b(bool b, void* userdata) {
  char* a = (char*) userdata;
  if(b){
    cout<<"BOTAO BBBBB CARAI";
    *a = 1;
  }
  return;
}
void bt_c(bool b, void* userdata) {
  char* a = (char*) userdata;
  if(b){
    cout<<"BOTAO CCCCC CARAI";
    *a = 2;
  }
  return;
}
void bt_d(bool b, void* userdata) {
  char* a = (char*) userdata;
  if(b){
    cout<<"BOTAO DDDDD CARAI";
    *a = 3;
  }
  return;
}
void bt_sl(bool b, void* userdata) {
  float* v = (float*) userdata;
  if(b){
    cout<<"SLIDER_>>>>>"<<*v;
  }
  return;
}


int main(int argc, char** argv){
  VideoCapture cap("Exemplo/Video 177.wmv");
  int height = cap.get(CAP_PROP_FRAME_HEIGHT);
  int width = cap.get(CAP_PROP_FRAME_WIDTH);
  Mat video, bg, mov, mov_aux, mov_ant, mcu, mcu_aux(height, width, CV_8UC3);
  Mat rastro(height, width, CV_8UC3, Scalar(0,0,0));
  Mat mapa_de_calor(height, width, CV_16UC1, Scalar(0));
  Mat princ(height, width+200, CV_8UC3, Scalar(255,230,200));
  Mat princ_bg(height, width+200, CV_8UC3, Scalar(255,230,200));
  int i=0;
  float vel=0, vel_at=0;
  // VideoCapture cap("Exemplo/cima.wmv"); // arquivo de vídeo ou imagem da webcam
  int fps = FPS;
  float mspf = 1000/fps;
  char key;
  Dado trjt[100];
  Rect regiao(height - det_tam/2, width - det_tam/2, det_tam, det_tam);
  Rect atbg(height - det_tam/2, width - det_tam/2, det_tam, det_tam);
  Rect Reg_det[2];
  Reg_det[0] = Rect(23,259,600,80);
  Reg_det[1] = Rect(281,13,80,240);
  Rect r1(281,13,80,240);
  Rect r2(23,259,250,80);
  Rect r3(366,259,250,80);
  double t1 = 0, t2 = 0, t3 = 0, t=0;
  ofstream outdata; // outdata is like cin
  Point peixe(width/2,height/2);
  Vec3b cor_rastro = {0,255,255};
  Mat lut = imread("mapa_de_cor.png");
  GUI gg;
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
  // namedWindow("rastro",1);
  // moveWindow("rastro", 20,350);
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
  // Rect regiao(300,66,80,80);
  // Rect atbg(300,66,80,80);
  rectangle(rastro, r1, Scalar(255,0,0), 1, LINE_8, 0);
  rectangle(rastro, r2, Scalar(255,0,0), 1, LINE_8, 0);
  rectangle(rastro, r3, Scalar(255,0,0), 1, LINE_8, 0);

  float cor_de_mel = 0;

  Botao* bta = new Botao(imread("GUI/Botao/bt1.png",IMREAD_UNCHANGED),imread("GUI/Botao/bt2.png",IMREAD_UNCHANGED),bt_a,Point(10,10),princ,&sw_tela);
  Botao* btb = new Botao(imread("GUI/Botao/bt1.png",IMREAD_UNCHANGED),imread("GUI/Botao/bt2.png",IMREAD_UNCHANGED),bt_b,Point(10,50),princ,&sw_tela);
  Botao* btc = new Botao(imread("GUI/Botao/bt1.png",IMREAD_UNCHANGED),imread("GUI/Botao/bt2.png",IMREAD_UNCHANGED),bt_c,Point(10,90),princ,&sw_tela);
  Botao* btd = new Botao(imread("GUI/Botao/bt1.png",IMREAD_UNCHANGED),imread("GUI/Botao/bt2.png",IMREAD_UNCHANGED),bt_d,Point(10,130),princ,&sw_tela);
  // Switch* btc = new Switch(imread("GUI/Botao/bt1.png",IMREAD_UNCHANGED),imread("GUI/Botao/bt2.png",IMREAD_UNCHANGED),bt_c,Point(10,100),video);
  Slider* btsl = new Slider(imread("GUI/Slider/bt3.png",IMREAD_UNCHANGED),imread("GUI/Slider/bt4.png",IMREAD_UNCHANGED),imread("GUI/Slider/slm.png",IMREAD_UNCHANGED),bt_sl,Point(100,100),200,0.5,princ,&cor_de_mel,0,255);
  gg.insert(bta);
  gg.insert(btb);
  gg.insert(btc);
  gg.insert(btd);
  gg.insert(btsl);
  setMouseCallback("princ", gui_func, &gg); //Não posso mais de uma callback por tela

  while (true) {
    cap >> video;
    absdiff(bg, video, mov);
    rastro.at<Vec3b>(peixe.y,peixe.x) = cor_rastro;

    // btsl->mostrar();
    // btsl->exe();

    key = (char) waitKey(mspf);
    if( key == 27 ){
      outdata << "velocidade máxima: " << vel;
      outdata << "fps " << fps;
      outdata.close();
      imwrite("MdC.png", mcu_aux);
      imwrite("rastro.png", rastro);
      break; // pressionar esc para sair
    }

    if(!atbg.contains(peixe)){
      if(!mov_ant.empty()){
        absdiff(mov_ant,mov,mov_aux);
        threshold(mov_aux,mov_aux,mov_thr,255,THRESH_BINARY_INV);
        video.copyTo(bg, mov_aux);
      }
      mov_ant = mov.clone();
      atbg.x = regiao.x;
      atbg.y = regiao.y;
    }

    if (i == 10) { // desenha o mapa de calor
      for (int j = 0; j < i; j++) {
        outdata << trjt[j].pos.x << "\t" << trjt[j].pos.y<< "\t" << (t+j-i)/fps << endl;
        for (int m = trjt[j].pos.y - circulo.rows/2; m < (trjt[j].pos.y + circulo.rows/2); m++) {
          for (int n = trjt[j].pos.x - circulo.rows/2; n < trjt[j].pos.x + circulo.rows/2; n++) {
            mapa_de_calor.at<ushort>(m,n) += circulo.at<ushort>(m - (trjt[j].pos.y - circulo.rows/2),n - (trjt[j].pos.x - circulo.rows/2));
          }
        }
      }
      normalize(mapa_de_calor, mcu, 0, pow(2,16), NORM_MINMAX, CV_16U);
      // LUT(mapa_de_calor,lut,mcu);
      // applyColorMap(mcu, mcu, 2);
      lookup(mcu,lut,mcu_aux);
      // imshow("mapa_de_calor",mcu_aux);
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
    desenha_cruz(peixe, video, {0,0,(unsigned char)cor_de_mel});
    rectangle(video, regiao, Scalar(0,255,0), 1, LINE_8, 0);
    rectangle(video, atbg, Scalar(255,0,255), 1, LINE_8, 0);
    rectangle(video, r1, Scalar(255,0,0), 1, LINE_8, 0);
    rectangle(video, r2, Scalar(255,0,0), 1, LINE_8, 0);
    rectangle(video, r3, Scalar(255,0,0), 1, LINE_8, 0);

    // rastro.copyTo(video, rastro);
    line(video, Point(30,350), Point(30+vel_at/5,350), Scalar(0,0,255), 5,8,0); //velocímetro

    switch (sw_tela) {
      case 0:
      princ_bg.copyTo(princ);
      video.copyTo(princ(Rect(200,0,princ.cols-200,princ.rows)));
      break;
      case 1:
      princ_bg.copyTo(princ);
      rastro.copyTo(princ(Rect(200,0,princ.cols-200,princ.rows)));
      break;
      case 2:
      princ_bg.copyTo(princ);
      mcu_aux.copyTo(princ(Rect(200,0,princ.cols-200,princ.rows)));
      break;
      case 3:
      princ_bg.copyTo(princ);
      mov.copyTo(princ(Rect(200,0,princ.cols-200,princ.rows)));
      break;
    }

    gg.exe();
    gg.mostrar();

    // imshow("mascara", mov);
    // imshow("video", video);
    // imshow("rastro", rastro);
    imshow("princ", princ);

    if(i>0){vel_at = sqrt( pow(trjt[i].pos.x - trjt[i-1].pos.x , 2) + pow(trjt[i].pos.y - trjt[i-1].pos.y , 2) ) / ( trjt[i-1].tempo - trjt[i].tempo );}
    vel = (vel<vel_at && vel_at<500)  ? vel_at : vel;

    cout<<"tempo em cima: "<<t1/40<<"s | tempo na esquerda: "<<t2/40<<"s | tempo na direita: "<<t3/40<<"s "<<"vel: "<<vel_at<<endl;

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
