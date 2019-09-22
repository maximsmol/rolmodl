#pragma once
#include <cstdint>

#include <SDL.h>

namespace rolmodl {
  enum class TextureType;

  namespace textureType::unsafe {
    constexpr TextureType fromSDLEnum(const int a) noexcept;
    constexpr int toSDLEnum(const TextureType a) noexcept;
  }

  struct TextureInfo;

  class Tex;
  class StaticTex;
  class LockTex;
  class RenTex;
  class TexLock;
}
