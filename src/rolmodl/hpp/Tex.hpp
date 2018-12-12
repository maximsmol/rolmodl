#pragma once
#include <cstdint>

#include <SDL.h>

namespace rolmodl {
  class Tex;
  class StaticTex;
  class LockTex;
  class RenTex;
  class TexLock;
}

#include "Base.hpp"
#include "PixelFmt.hpp"
#include "Geom.hpp"
#include "Ren.hpp"

namespace rolmodl {
  class Tex {
    public:
      ~Tex() noexcept;

      Tex(const Tex& that) = delete;
      Tex(Tex&& that) noexcept;

      Tex& operator=(const Tex& that) = delete;
      Tex& operator=(Tex&& that) noexcept;

      friend void swap(Tex& a, Tex& b) noexcept;

      pixelfmt::Id format() const noexcept;
      BlendMode getBlendMode();
      void setBlendMode(const BlendMode m);

      uint8_t getAlphaMod();
      void setAlphaMod(const uint8_t i);

      RGB getRGBMod();
      void setRGBMod(const RGB i);

      SDL_Texture* unsafeRaw() noexcept;
      const SDL_Texture* unsafeRaw() const noexcept;

    protected:
      Tex() noexcept;
      Tex(Ren& r, const pixelfmt::Id fmt, const int access, const geom::Size s);

      SDL_Texture* h_;
      pixelfmt::Id format_;
  };

  class StaticTex : public Tex {
    public:
      StaticTex(Ren& r, const pixelfmt::Id fmt, const geom::Size s);
  };

  class LockTex : public Tex {
    public:
      LockTex(Ren& r, const pixelfmt::Id fmt, const geom::Size s);
  };

  class RenTex : public Tex {
    public:
      RenTex(Ren& r, const pixelfmt::Id fmt, const geom::Size s);
  };


  class TexLock {
    public:
      explicit TexLock(LockTex& tex);
      TexLock(LockTex& tex, const geom::RectWH r);
      TexLock(LockTex& tex, const geom::RectXY r);
      ~TexLock() noexcept;

      TexLock(const TexLock& that) = delete;
      TexLock(TexLock&& that) noexcept;

      TexLock& operator=(const TexLock& that) = delete;
      TexLock& operator=(TexLock&& that) noexcept;

      friend void swap(TexLock& a, TexLock& b) noexcept;

      TexLock& drawPoint(const RGBA c, const geom::Pos p) noexcept;
      RGBA getPoint(const geom::Pos p) const noexcept;

      uint32_t& unsafePoint(const geom::Pos p) noexcept;
      const uint32_t& unsafePoint(const geom::Pos p) const noexcept;

      void* unsafeRaw() noexcept;
      const void* unsafeRaw() const noexcept;

    private:
      TexLock() noexcept;
      TexLock(LockTex& tex, const SDL_Rect* r);
      TexLock(LockTex& tex, const SDL_Rect r);

      LockTex* t_;
      void* h_;
      unsigned int pitch_;
  };
}
