#include <include/vendor.h>

std::string timePointAsString(const std::chrono::high_resolution_clock::time_point &t)
{
  std::time_t tt = std::chrono::high_resolution_clock::to_time_t(t);
  std::string ts = std::ctime(&tt);
  ts.resize(ts.size() - 1);
  return ts;
}