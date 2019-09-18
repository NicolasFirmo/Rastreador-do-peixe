#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <string>
#include <iomanip>
#include <cmath>
#include "GUI.h"

GUI::GUI() : c_rect(nullptr), s_rect_it(-1), cria_rect(false)
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
    if (rects[i]->contains({x, y}))
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
  cria_rect = false;
}

void GUI::pop_rect()
{
  if (rects.empty())
    return;
  if (s_rect_it != -1)
  {
    delete rects[s_rect_it];
    rects.erase(rects.begin() + s_rect_it);
    s_rect_it = -1;
    return;
  }
  delete rects.back();
  rects.pop_back();
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
    rectangle(botoes[0]->tela, *(this->rects[i]), Scalar(255, 255, 255));
  }
  if (c_rect)
    rectangle(botoes[0]->tela, *c_rect, Scalar(0, 0, 255));
  if (s_rect_it != -1)
    rectangle(botoes[0]->tela, *rects[s_rect_it], Scalar(255, 0, 0), 2);
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
    if (gui->cria_rect)
      gui->create_rect(x, y);
    else
      gui->select_rect(x, y);
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
