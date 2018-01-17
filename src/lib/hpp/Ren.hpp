#pragma once

#include <cstdint>
#include <optional>

#include <SDL.h>

#include "Base.hpp"
#include "Geom.hpp"
#include "Win.hpp"

namespace rolmodl {
  namespace ren {
    // todo: some flags might be mutually exclusive
    // fixme: how does CreateSoftwareRenderer relate
    struct Flags {
      public:
        constexpr Flags() noexcept :
          data_(0)
        {}

        // set
        constexpr Flags software() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_RENDERER_SOFTWARE));
        }
        constexpr Flags accelerated() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_RENDERER_ACCELERATED));
        }
        constexpr Flags vsync() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_RENDERER_PRESENTVSYNC));
        }
        constexpr Flags toTexture() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_RENDERER_TARGETTEXTURE));
        }

        // unset
        constexpr Flags unSoftware() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_RENDERER_SOFTWARE));
        }
        constexpr Flags unAccelerated() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_RENDERER_ACCELERATED));
        }
        constexpr Flags unVsync() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_RENDERER_PRESENTVSYNC));
        }
        constexpr Flags unToTexture() const noexcept {
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

      void drawLine(const geom::Pos a, const geom::Pos b);
      // void drawLines(?); // fixme: implement this generically

      void drawPoint(const geom::Pos p);
      // void drawPoints(?) noexcept;  // fixme: implement this generically

      void drawRect(const geom::RectWH r);
      void drawRect(const geom::RectXY r);
      // void drawRects(?) noexcept;  // fixme: implement this generically

      void fillRect(const geom::RectWH r);
      void fillRect(const geom::RectXY r);
      // void fillRects(?);  // fixme: implement this generically

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


      Color color();
      void setColor(const Color c);

    private:
      Ren() noexcept;

      SDL_Renderer* h_;
  };
}
