#include <gtest/gtest.h>
#include "Rendering/Surface.h"
using namespace Sourcehold::Rendering;
using namespace Sourcehold::SDL;

TEST(Surface, Constructor) {
  Surface surface({10, 10});
}

TEST(Surface, Set) {
  Surface surface({10, 10});
  Color color = {0xF, 0xF, 0xF};
  surface.Set({0, 0}, color);
  ASSERT_EQ(At<uint32_t>(surface.Ptr(), {}), AsPixel(color));
}
