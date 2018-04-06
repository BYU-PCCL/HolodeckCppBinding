#include <gtest/gtest.h>

#include "core/shmem.h"

namespace {
TEST(BasicTest, Basic) {
  holodeck::Shmem shmem("test", 0, "TEST1");
}

}  //namespace
