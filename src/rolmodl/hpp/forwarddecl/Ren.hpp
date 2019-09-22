#pragma once

#include <cstdint>
#include <optional>
#include <iterator>
#include <vector>

#include <SDL.h>

namespace rolmodl {
  enum class BlendMode;
  namespace blendMode::unsafe {
    constexpr BlendMode fromSDLEnum(const SDL_BlendMode m) noexcept;
    constexpr SDL_BlendMode toSDLEnum(const BlendMode m) noexcept;
  }

  struct Flip;

  namespace ren {
    struct Flags;
    struct Info;

    namespace driver {
      unsigned int count();
      Info info(const unsigned int i);
    }
  }

  struct RenScale;

  struct SrcRectWH;
  struct SrcRectXY;

  struct DstRectWH;
  struct DstRectXY;

  class Ren;
  class TexRen;
}
