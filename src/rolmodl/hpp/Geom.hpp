#pragma once

/// \file Geom.hpp
/// `rolmodl` geometric primitives.
///
/// \sa https://wiki.libsdl.org/CategoryRect

#include "forwarddecl/Geom.hpp"

namespace rolmodl::geom {
  /// \brief `int` point data type. The value is in pixels. Semantically different from \link rolmodl::geom::Size \endlink.
  /// \sa https://wiki.libsdl.org/SDL_Point
  struct Pos {
    public:
      int x, y;

    private:
  };
  /// \brief
  /// `int` dimensions data type. The value is in pixels. Semantically different from \link rolmodl::geom::Pos \endlink.
  ///
  /// Should be used in conjunction with \link rolmodl::geom::Pos \endlink
  /// to indicate the semantic difference between position and size.
  ///
  /// Has no SDL equivalent as SDL passes width and height as separate `int`s.
  struct Size {
    public:
      int w, h;

    private:
  };

  /// \brief `float` point data type. Used by touch events.
  /// \todo what are the units here?
  /// \todo any other uses?
  struct XYFloats {
    public:
      float x, y;

    private:
  };
  /// \brief `int32` point data type. The value is in pixels. Used by mouse events.
  /// \todo any other uses?
  struct XYInt32 {
    public:
      int32_t x, y;

    private:
  };

  /// \brief Rectangle represented by its top left corner coordinates, width, and height.
  /// \sa https://wiki.libsdl.org/SDL_Rect
  /// \sa rolmodl::geom::RectXY::wh
  struct RectWH {
    public:
      int x, y, w, h;

      /// \brief Initialize with the given top left corner x and y, width, and height.
      /// \todo Replace `int`s with \link rolmodl::geom::Pos \endlink and \link rolmodl::geom::Size \endlink? It's less convenient to initialize.
      constexpr RectWH(const int argX, const int argY, const int argW, const int argH) noexcept :
        x(argX), y(argY), w(argW), h(argH)
      {}
      /// \brief Initialize from the given <a href="https://wiki.libsdl.org/SDL_Rect">`SDL_Rect`</a>.
      constexpr explicit RectWH(const SDL_Rect r) noexcept :
        x(r.x), y(r.y), w(r.w), h(r.h)
      {}

      /// \brief Convert to an equivalent \link rolmodl::geom::RectXY \endlink.
      constexpr RectXY xy() const noexcept;
      /// \brief Convert to an <a href="https://wiki.libsdl.org/SDL_Rect">`SDL_Rect`</a>.
      constexpr SDL_Rect sdl() const noexcept {
        return SDL_Rect{x, y, w, h};
      }

    private:
  };
  /// \brief Rectangle represented by its top left corner coordinates and its bottom right corner coordinates.
  ///
  /// Has no SDL equivalent.
  ///
  /// \sa rolmodl::geom::RectWH::xy
  struct RectXY {
    public:
      int x, y, x1, y1;

      /// \brief Initialize with the given top left corner x and y, and the bottom right corner x and y.
      /// \todo Replace `int`s with \link rolmodl::geom::Pos \endlink? It's less convenient to initialize
      constexpr RectXY(const int argX, const int argY, const int argX1, const int argY1) noexcept :
        x(argX), y(argY), x1(argX1), y1(argY1)
      {}
      /// \brief Initialize from the given <a href="https://wiki.libsdl.org/SDL_Rect">`SDL_Rect`</a> converted from a width-height rectangle.
      constexpr explicit RectXY(const SDL_Rect r) noexcept :
        RectXY(RectWH(r).xy())
      {}

      /// \brief Convert to an equivalent \link rolmodl::geom::RectWH \endlink.
      constexpr RectWH wh() const noexcept;
      /// \brief Convert to an <a href="https://wiki.libsdl.org/SDL_Rect">`SDL_Rect`</a> width-height rectangle.
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
