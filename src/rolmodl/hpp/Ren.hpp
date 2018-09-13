#pragma once

#include <cstdint>
#include <optional>
#include <iterator>

#include <SDL.h>

namespace rolmodl {
  class Ren;
  struct RenScale;

  struct SrcRectWH;
  struct SrcRectXY;

  struct DstRectWH;
  struct DstRectXY;

  namespace ren {
    struct Flags;
  }
}

#include "Base.hpp"
#include "Geom.hpp"
#include "Win.hpp"
#include "Tex.hpp"

namespace rolmodl {
  namespace ren {
    // todo: some flags might be mutually exclusive
    // fixme: how does CreateSoftwareRenderer relate
    struct Flags {
      public:
        constexpr Flags() noexcept :
          data_(0)
        {}

        // test
        constexpr Flags isSoftware() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_RENDERER_SOFTWARE));
        }
        constexpr Flags isAccelerated() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_RENDERER_ACCELERATED));
        }
        constexpr Flags isVsync() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_RENDERER_PRESENTVSYNC));
        }
        constexpr Flags isToTexture() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_RENDERER_TARGETTEXTURE));
        }

        // set
        constexpr Flags withSoftware() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_RENDERER_SOFTWARE));
        }
        constexpr Flags withAccelerated() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_RENDERER_ACCELERATED));
        }
        constexpr Flags withVsync() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_RENDERER_PRESENTVSYNC));
        }
        constexpr Flags withToTexture() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_RENDERER_TARGETTEXTURE));
        }

        // unset
        constexpr Flags withoutSoftware() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_RENDERER_SOFTWARE));
        }
        constexpr Flags withoutAccelerated() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_RENDERER_ACCELERATED));
        }
        constexpr Flags withoutVsync() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_RENDERER_PRESENTVSYNC));
        }
        constexpr Flags withoutToTexture() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_RENDERER_TARGETTEXTURE));
        }

        constexpr uint32_t raw() const noexcept {
          return data_;
        }
      private:
        explicit constexpr Flags(const uint32_t data) noexcept :
          data_(data)
        {}

        uint32_t data_;
    };
  }

  struct RenScale {
    public:
      float x, y;

    private:
  };

  struct SrcRectWH : public geom::RectWH {
    using geom::RectWH::RectWH;
    constexpr SrcRectXY xy() const noexcept;
  };
  struct SrcRectXY : public geom::RectXY {
    using geom::RectXY::RectXY;
    constexpr SrcRectWH wh() const noexcept;
  };
  constexpr SrcRectXY SrcRectWH::xy() const noexcept {
    geom::RectXY tmp = geom::RectWH::xy();
    return SrcRectXY(tmp.x, tmp.y, tmp.x1, tmp.y1);
  }
  constexpr SrcRectWH SrcRectXY::wh() const noexcept {
    geom::RectWH tmp = geom::RectXY::wh();
    return SrcRectWH(tmp.x, tmp.y, tmp.w, tmp.h);
  }

  struct DstRectWH : public geom::RectWH {
    using geom::RectWH::RectWH;
    constexpr DstRectXY xy() const noexcept;
  };
  struct DstRectXY : public geom::RectXY {
    using geom::RectXY::RectXY;
    constexpr DstRectWH wh() const noexcept;
  };
  constexpr DstRectXY DstRectWH::xy() const noexcept {
    geom::RectXY tmp = geom::RectWH::xy();
    return DstRectXY(tmp.x, tmp.y, tmp.x1, tmp.y1);
  }
  constexpr DstRectWH DstRectXY::wh() const noexcept {
    geom::RectWH tmp = geom::RectXY::wh();
    return DstRectWH(tmp.x, tmp.y, tmp.w, tmp.h);
  }

  class Ren {
    public:
      Ren(Win& win, int i, ren::Flags flags);
      Ren(Win& win,        ren::Flags flags);
      Ren(Win& win, int i);
      explicit Ren(Win& win);
      ~Ren() noexcept;

      Ren(const Ren& that) = delete;
      Ren(Ren&& that) noexcept;

      Ren& operator=(const Ren& that) = delete;
      Ren& operator=(Ren&& that) noexcept;

      friend void swap(Ren& a, Ren& b) noexcept;

      SDL_Renderer* unsafeRaw() noexcept;
      const SDL_Renderer* unsafeRaw() const noexcept;


      void clear();
      void present() noexcept;

      void drawTex(Tex& tex);

      void drawTex(Tex& tex, const SrcRectWH src);
      void drawTex(Tex& tex, const SrcRectXY src);

      void drawTex(Tex& tex, const DstRectWH dst);
      void drawTex(Tex& tex, const DstRectXY dst);

      void drawTex(Tex& tex, const SrcRectWH src, const DstRectWH dst);
      void drawTex(Tex& tex, const SrcRectWH src, const DstRectXY dst);
      void drawTex(Tex& tex, const SrcRectXY src, const DstRectWH dst);
      void drawTex(Tex& tex, const SrcRectXY src, const DstRectXY dst);

      // todo:
      // needs ContigousIterators, for which no check can be done at compile-time
      // also needs SDL_Point structs, or equivalen
      //   reinterpret_cast geom::Point?
      void drawLine(const geom::Pos a, const geom::Pos b);
      // void drawLines(?) noexcept;  // todo: implement this generically

      void drawPoint(const geom::Pos p);
      // void drawPoints(?) noexcept;  // todo: implement this generically

      void drawRect(const geom::RectWH r);
      void drawRect(const geom::RectXY r);
      // void drawRects(?) noexcept;  // todo: implement this generically

      void fillRect(const geom::RectWH r);
      void fillRect(const geom::RectXY r);
      // void fillRects(?);  // todo: implement this generically

      void outlineScreen();
      void fillScreen();

      std::optional<geom::RectWH> clipRect() noexcept;
      void setClipRect(const geom::RectWH r);
      void setClipRect(const geom::RectXY r);
      void disableClip();

      std::optional<geom::Size> logicalSize() noexcept;
      void setLogicalSize(const geom::Size s);

      RenScale scale() noexcept;
      void setScale(const RenScale s);

      geom::RectWH viewport() noexcept;
      void setViewport(const geom::RectWH r);


      RGBA color();
      void setColor(const RGBA c);

    private:
      Ren() noexcept;

      SDL_Renderer* h_;
  };
}
