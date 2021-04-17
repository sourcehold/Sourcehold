#pragma once
#include <al.h>
#include <alc.h>
#include <string>

namespace Sourcehold {
namespace OpenAL {
class OpenAL {
 public:
  static void Init();
  static void Deinit() noexcept;

  static std::string GetError() noexcept;

  static bool& Muted();

 private:
  ~OpenAL();
};
}  // namespace OpenAL
}  // namespace Sourcehold
