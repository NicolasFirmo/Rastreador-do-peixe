#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <string>
#include <iomanip>
#include <cmath>
#include "GUI.h"

GUI::GUI(Point &peixe, Rect &regiao) : p(peixe), reg(regiao), c_rect(nullptr), s_rect_it(-1), click_mode(SELECT_RECT)
{
  botoes.reserve(10);
  switches.reserve(10);
  sliders.reserve(10);
  rects.reserve(10);
}

void GUI::create_rect(int x, int y)
{
  c_rect = new Rect(x, y, 0, 0);
}

void GUI::select_rect(int x, int y)
{
  if (rects.empty())
    return;
  s_rect_it = -1;
  for (unsigned int i = 0; i < rects.size(); i++)
  {
    if (rects[i].rect->contains({x, y}))
    {
      s_rect_it = i;
    }
  }
}

void GUI::scale_rect(int x, int y)
{
  if (!c_rect)
    return;
  c_rect->width = x - c_rect->x;
  c_rect->height = y - c_rect->y;
}

void GUI::save_rect()
{
  if (!c_rect)
    return;
  this->rects.emplace_back(c_rect);
  c_rect = nullptr;
  click_mode = SELECT_RECT;
}

void GUI::pop_rect()
{
  if (rects.empty())
    return;
  if (s_rect_it != -1)
  {
    delete rects[s_rect_it].rect;
    rects.erase(rects.begin() + s_rect_it);
    s_rect_it = -1;
    return;
  }
  delete rects.back().rect;
  rects.pop_back();
}

void GUI::setSetectedType(unsigned char type)
{
  rects[s_rect_it].type = type;
}

void GUI::insert(Botao *bt)
{
  this->botoes.emplace_back(bt);
}
void GUI::insert(Slider *sl)
{
  this->sliders.emplace_back(sl);
}
void GUI::insert(Switch *sw)
{
  this->switches.emplace_back(sw);
}

void GUI::mostrar()
{
  for (unsigned i = 0; i < botoes.size(); i++)
  {
    this->botoes[i]->mostrar();
  }
  for (unsigned i = 0; i < sliders.size(); i++)
  {
    this->sliders[i]->mostrar();
  }
  for (unsigned i = 0; i < switches.size(); i++)
  {
    this->switches[i]->mostrar();
  }
  for (unsigned i = 0; i < rects.size(); i++)
  {
    rectangle(botoes[0]->tela, *(rects[i].rect),
              rects[i].type == 0 ? Scalar(0, 20, 230) : rects[i].type == 1 ? Scalar(255, 20, 0) : rects[i].type == 2 ? Scalar(255, 100, 0) : Scalar(255, 255, 255));
  }
  if (c_rect)
    rectangle(botoes[0]->tela, *c_rect, Scalar(120, 120, 120));
  if (s_rect_it != -1)
    rectangle(botoes[0]->tela, *rects[s_rect_it].rect, rects[s_rect_it].type == 0 ? Scalar(0, 20, 230) : rects[s_rect_it].type == 1 ? Scalar(255, 20, 0) : rects[s_rect_it].type == 2 ? Scalar(255, 100, 0) : Scalar(255, 255, 255), 2);

  for (unsigned i = 0; i < rects.size(); i++)
  {
    if (rects[i].contemPeixe && rects[i].type == 1)
    {
      cout << "rect[" << i << "]: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - rects[i].t0 + rects[i].t).count() << endl;
    }
    else
    {
      cout << "rect[" << i << "]: " << std::chrono::duration_cast<std::chrono::milliseconds>(rects[i].t).count() << endl;
    }
  }
}

void GUI::exe()
{
  for (unsigned i = 0; i < botoes.size(); i++)
  {
    this->botoes[i]->exe();
  }
  for (unsigned i = 0; i < sliders.size(); i++)
  {
    this->sliders[i]->exe();
  }
  for (unsigned i = 0; i < switches.size(); i++)
  {
    this->switches[i]->exe();
  }
}

void GUI::zeraCont()
{
  if (s_rect_it >= 0)
  {
    rects[s_rect_it].t = std::chrono::nanoseconds(0);
    rects[s_rect_it].t0 = std::chrono::high_resolution_clock::now();
  }
  else
  {
    cerr << "nenhum rect selecionado\n";
  }
}

void GUI::computaTempo(const Point &p)
{
  for (unsigned i = 0; i < rects.size(); i++)
  {
    if (rects[i].rect->contains(p + Point(200, 0)) && !rects[i].type)
    {
      for (unsigned i = 0; i < rects.size(); i++)
      {
        if (rects[i].contemPeixe)
        {
          rects[i].contemPeixe = false;
          rects[i].t += std::chrono::high_resolution_clock::now() - rects[i].t0;
          rects[i].t0 = std::chrono::high_resolution_clock::now();
        }
      }
      return;
    }
  }

  for (unsigned i = 0; i < rects.size(); i++)
  {
    if (rects[i].type != 1)
      continue;
    // cout << "p: " << p.x << p.y << endl;
    // cout << "rect xywh:" << rects[i].rect->tl().x << " " << rects[i].rect->tl().y << " " << rects[i].rect->width << " " << rects[i].rect->height << endl;
    if (rects[i].rect->contains(p + Point(200, 0)) && !rects[i].contemPeixe)
    {
      rects[i].contemPeixe = true;
      rects[i].t0 = std::chrono::high_resolution_clock::now();
    }
    else if (!rects[i].rect->contains(p + Point(200, 0)) && rects[i].contemPeixe)
    {
      rects[i].contemPeixe = false;
      rects[i].t += std::chrono::high_resolution_clock::now() - rects[i].t0;
      rects[i].t0 = std::chrono::high_resolution_clock::now();
    }
  }
}

void GUI::aperta_botao(int x, int y)
{
  for (unsigned i = 0; i < botoes.size(); i++)
  {
    this->botoes[i]->aperta(x, y);
  }
}

void GUI::solta_botao()
{
  for (unsigned i = 0; i < botoes.size(); i++)
  {
    this->botoes[i]->solta();
  }
}

void GUI::aperta_switch(int x, int y)
{
  for (unsigned i = 0; i < switches.size(); i++)
  {
    this->switches[i]->aperta(x, y);
  }
}

void GUI::aperta_slider(int x, int y)
{
  for (unsigned i = 0; i < sliders.size(); i++)
  {
    this->sliders[i]->aperta(x, y);
  }
}

void GUI::solta_slider()
{
  for (unsigned i = 0; i < sliders.size(); i++)
  {
    this->sliders[i]->solta();
  }
}

Slider *GUI::getApertado_slider()
{
  for (unsigned i = 0; i < sliders.size(); i++)
  {
    if (this->sliders[i]->apertado)
      return this->sliders[i];
  }
  return NULL;
}

void gui_func(int event, int x, int y, int flags, void *userdata)
{
  GUI *gui = (GUI *)userdata;

  if (event == EVENT_LBUTTONDOWN)
  {
    cout << "aperta\n";
    gui->aperta_botao(x, y);
    gui->aperta_slider(x, y);
    gui->aperta_switch(x, y);
    if (gui->click_mode == CRIA_RECT)
      gui->create_rect(x, y);
    else if (gui->click_mode == SELECT_RECT)
      gui->select_rect(x, y);
    else if (gui->click_mode == POINT_PEIXE)
    {
      gui->p.x = x - 200;
      gui->p.y = y;
      int n_regX = (gui->p.x - det_tam / 2) < 0 ? 0 : gui->p.x + det_tam / 2 >= gui->botoes[0]->tela.cols ? gui->botoes[0]->tela.cols - det_tam - 1 : (gui->p.x - det_tam / 2);
      int n_regY = (gui->p.y - det_tam / 2) < 0 ? 0 : gui->p.y + det_tam / 2 >= gui->botoes[0]->tela.rows ? gui->botoes[0]->tela.rows - det_tam - 1 : (gui->p.y - det_tam / 2);
      gui->reg.x = n_regX;
      gui->reg.y = n_regY;
      gui->reg.width = det_tam;
      gui->reg.height = det_tam;
    }
  }

  if (event == EVENT_LBUTTONUP)
  {
    cout << "solta\n";
    gui->solta_botao();
    gui->solta_slider();
    gui->save_rect();
  }

  if (event == EVENT_MOUSEMOVE)
  {
    if (gui->c_rect)
    {
      gui->scale_rect(x, y);
    }

    if (gui->getApertado_slider())
    {
      gui->getApertado_slider()->aperta(x, y);
    }
  }
}
