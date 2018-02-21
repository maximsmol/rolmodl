#pragma once

#include <cstdint>

#include <SDL.h>

#include "Geom.hpp"

namespace rolmodl::mouse {
  enum class Btn;

  namespace btn::unsafe {
    constexpr uint32_t toSDLEnum(const Btn b) noexcept;
    constexpr Btn fromSDLEnum(const uint32_t b) noexcept;
  }

  struct BtnState;
  class State;
}

namespace rolmodl::mouse {
  enum class Btn {
    left, right, middle, x1, x2,
    l = left, r = right, m = middle
  };

  namespace btn::unsafe {
    constexpr uint32_t toSDLEnum(const Btn b) noexcept {
      if (b == Btn::left)
        return SDL_BUTTON_LEFT;
      else if (b == Btn::right)
        return SDL_BUTTON_RIGHT;
      else if (b == Btn::middle)
        return SDL_BUTTON_MIDDLE;
      else if (b == Btn::x1)
        return SDL_BUTTON_X1;
      // else if (b == Btn::x2)
        return SDL_BUTTON_X2;
    }

    constexpr Btn fromSDLEnum(const uint32_t b) noexcept {
      if (b == SDL_BUTTON_LEFT)
        return Btn::left;
      else if (b == SDL_BUTTON_RIGHT)
        return Btn::right;
      else if (b == SDL_BUTTON_MIDDLE)
        return Btn::middle;
      else if (b == SDL_BUTTON_X1)
        return Btn::x1;
      // else if (b == SDL_BUTTON_X2)
        return Btn::x2;
    }
  }

  struct BtnState {
    public:
      explicit constexpr BtnState(const uint32_t raw) noexcept :
        raw_(raw)
      {}

      constexpr bool left() const noexcept {
        return get(Btn::left);
      }
      constexpr bool right() const noexcept {
        return get(Btn::right);
      }
      constexpr bool middle() const noexcept {
        return get(Btn::middle);
      }
      constexpr bool x1() const noexcept {
        return get(Btn::x1);
      }
      constexpr bool x2() const noexcept {
        return get(Btn::x2);
      }

      constexpr bool l() const noexcept {return left();}
      constexpr bool r() const noexcept {return right();}
      constexpr bool m() const noexcept {return middle();}

      constexpr bool get(const Btn b) const noexcept {
        return raw_ & SDL_BUTTON(btn::unsafe::toSDLEnum(b));
      }

    private:
      uint32_t raw_;
  };

  class State {
    public:
      // fixme: SDL_PumpEvents required before calls
      State() noexcept;

      const geom::Pos& pos() const noexcept;
      const BtnState& btnState() const noexcept;

    private:
      geom::Pos p_;
      BtnState btnState_;
  };
}
