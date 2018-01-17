#pragma once

#include <SDL.h>

namespace rolmodl::geom {
  struct Pos {
    public:
      int x, y;

    private:
  };
  struct Size {
    public:
      int w, h;

    private:
  };

  struct RectXY;
  struct RectWH {
    public:
      int x, y, w, h;

      constexpr RectWH(const int argX, const int argY, const int argW, const int argH) noexcept :
        x(argX), y(argY), w(argW), h(argH)
      {}
      constexpr explicit RectWH(const SDL_Rect r) noexcept :
        x(r.x), y(r.y), w(r.w), h(r.h)
      {}

      constexpr RectXY xy() const noexcept;
      constexpr SDL_Rect sdl() const noexcept {
        return SDL_Rect{x, y, w, h};
      }

    private:
  };
  struct RectXY {
    public:
      int x, y, x1, y1;

      constexpr RectXY(const int argX, const int argY, const int argX1, const int argY1) noexcept :
        x(argX), y(argY), x1(argX1), y1(argY1)
      {}
      constexpr explicit RectXY(const SDL_Rect r) noexcept :
        RectXY(RectWH(r).xy())
      {}

      constexpr RectWH wh() const noexcept;
      constexpr SDL_Rect sdl() const noexcept {
        return wh().sdl();
      }

    private:
  };

  constexpr RectXY RectWH::xy() const noexcept {
      return RectXY(x, y, x+w, y+h);
  }
  constexpr RectWH RectXY::wh() const noexcept {
    return RectWH(x, y, x1-x, y1-y);
  }
}
