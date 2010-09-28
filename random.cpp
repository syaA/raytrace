
#include "random.h"

namespace {

xorshift_engine rand_engine;
} // end of anonymus namespace

value_type rand_01()
{
  return rand_engine.generate() / static_cast<float>(0xffffffff);
}
