#include <iostream>
#include <thread>
#include <functional>

void teste(int& i)
{
  i++;
}

int main(int argc, char const *argv[])
{
  std::thread bora;
  int i = 0;
  std::cout << "d boas" << std::endl;
  bora = std::thread(teste, std::ref(i));
  bora.join();
  std::cout << "valor de i: " << i << std::endl;

  return 0;
}
