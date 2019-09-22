#pragma once

#include <SDL.h>

namespace rolmodl::kb {
  class State;
  enum class Key;
  enum class Scancode;

  namespace key {
    struct Name;

    constexpr Scancode scancode(const Key k) noexcept;
    constexpr Key fromScancode(const Scancode s) noexcept;
  }
  namespace scancode {
    constexpr Key key(const Scancode s) noexcept;
    constexpr Scancode fromKey(const Key k) noexcept;
  }

  namespace key::unsafe {
    constexpr Key fromSDLEnum(const SDL_Keycode k) noexcept;
    constexpr SDL_Keycode toSDLEnum(const Key k) noexcept;
  }
  namespace scancode::unsafe {
    constexpr Scancode fromSDLEnum(const SDL_Scancode k) noexcept;
    constexpr SDL_Scancode toSDLEnum(const Scancode k) noexcept;
  }
}
