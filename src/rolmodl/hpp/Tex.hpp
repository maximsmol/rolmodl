#pragma once

#include "forwarddecl/Tex.hpp"

#include "forwarddecl/Ren.hpp"
#include "Base.hpp"
#include "PixelFmt.hpp"
#include "Geom.hpp"

namespace rolmodl {
  enum class TextureType {
    Static, Lock, Ren
  };
  namespace textureType::unsafe {
    constexpr TextureType fromSDLEnum(const int a) noexcept {
      if (a == SDL_TEXTUREACCESS_STATIC)
        return TextureType::Static;
      if (a == SDL_TEXTUREACCESS_STREAMING)
        return TextureType::Lock;
      // if (a == SDL_TEXTUREACCESS_TARGET)
        return TextureType::Ren;
    }
    constexpr int toSDLEnum(const TextureType a) noexcept {
      if (a == TextureType::Static)
        return SDL_TEXTUREACCESS_STATIC;
      if (a == TextureType::Lock)
        return SDL_TEXTUREACCESS_STREAMING;
      // if (a == TextureType::Ren)
        return SDL_TEXTUREACCESS_TARGET;
    }
  }

  struct TextureInfo {
    public:
      pixelfmt::Id fmt;
      TextureType type;
      geom::Size size;
  };

  class Tex {
    public:
      ~Tex() noexcept;

      Tex(const Tex& that) = delete;
      Tex(Tex&& that) noexcept;

      Tex& operator=(const Tex& that) = delete;
      Tex& operator=(Tex&& that) noexcept;

      friend void swap(Tex& a, Tex& b) noexcept;

      BlendMode getBlendMode();
      void setBlendMode(const BlendMode m);

      uint8_t getAlphaMod();
      void setAlphaMod(const uint8_t i);

      RGB getRGBMod();
      void setRGBMod(const RGB i);

      TextureInfo query();

      SDL_Texture* unsafeRaw() noexcept;
      const SDL_Texture* unsafeRaw() const noexcept;

    protected:
      Tex() noexcept;
      Tex(Ren& r, const pixelfmt::Id fmt, const int access, const geom::Size s);

      SDL_Texture* h_;
  };

  class StaticTex : public Tex {
    public:
      StaticTex(Ren& r, const pixelfmt::Id fmt, const geom::Size s);
  };

  class LockTex : public Tex {
    public:
      LockTex(Ren& r, const pixelfmt::Id fmt, const geom::Size s);

      pixelfmt::Id format() const noexcept;

    private:
      LockTex() noexcept;

      pixelfmt::Id format_;
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
