#include <chrono>
#include "time.h"

using namespace std::chrono;

unsigned long long timeSinceEpochMillisec() {
  return static_cast<unsigned long long>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
}

int millisecondsSinceX(unsigned long long X) {
  return static_cast<int>(timeSinceEpochMillisec() - X);
}