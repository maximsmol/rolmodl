#pragma once

#include "forwarddecl/SWTex.hpp"

#include "forwarddecl/Geom.hpp"
#include "forwarddecl/PixelFmt.hpp"
#include "forwarddecl/Base.hpp"
#include "Win.hpp"

namespace rolmodl {
  class SWTex_Base {
    public:
      SWTex_Base(const geom::Size s, const pixelfmt::Id fmt);
      ~SWTex_Base() noexcept;

      SWTex_Base(const SWTex_Base& that) = delete;
      SWTex_Base(SWTex_Base&& that) noexcept;

      SWTex_Base& operator=(const SWTex_Base& that) = delete;
      SWTex_Base& operator=(SWTex_Base&& that) noexcept;

      friend void swap(SWTex_Base& a, SWTex_Base& b) noexcept;

      pixelfmt::Id fmtId() noexcept;
      pixelfmt::Id fmtId() const noexcept;

      SDL_PixelFormat& fmt() noexcept;
      SDL_PixelFormat& fmt() const noexcept;

      unsigned int pitch() noexcept;
      unsigned int pitch() const noexcept;

      int w() noexcept;
      int w() const noexcept;
      int h() noexcept;
      int h() const noexcept;

      bool mustLock() noexcept;
      bool mustLock() const noexcept;

      SDL_Surface* unsafeRaw() noexcept;
      const SDL_Surface* unsafeRaw() const noexcept;

      uint32_t& unsafePoint(const geom::Pos p) noexcept;
      const uint32_t& unsafePoint(const geom::Pos p) const noexcept;

      void* unsafeRawPixels() noexcept;
      const void* unsafeRawPixels() const noexcept;

    protected:
      SWTex_Base();

      friend class Win_SW;
      friend class SWTex;
      friend class SWTex_RLE;
      SDL_Surface* h_;

      friend class SWTex_RLELock;
      SWTex_Base& drawPoint(const RGBA c, const geom::Pos p) noexcept;
      RGBA getPoint(const geom::Pos p) const noexcept;
  };

  class SWTex : public SWTex_Base {
    public:
      SWTex(const geom::Size s, const pixelfmt::Id fmt);
      explicit SWTex(SWTex_RLE&& rle) noexcept;

      SWTex& drawPoint(const RGBA c, const geom::Pos p) noexcept;
      RGBA getPoint(const geom::Pos p) const noexcept;

    private:
      friend class Win_SW;
      SWTex();
  };

  class SWTex_RLE : public SWTex_Base {
    public:
      SWTex_RLE(const geom::Size s, const pixelfmt::Id fmt);
      explicit SWTex_RLE(SWTex&& nonRle) noexcept;
  };

  class SWTex_RLELock {
    public:
      explicit SWTex_RLELock(SWTex_RLE& tex);
      ~SWTex_RLELock() noexcept;

      SWTex_RLELock(const SWTex_RLELock& that) = delete;
      SWTex_RLELock(SWTex_RLELock&& that) noexcept;

      SWTex_RLELock& operator=(const SWTex_RLELock& that) = delete;
      SWTex_RLELock& operator=(SWTex_RLELock&& that) noexcept;

      friend void swap(SWTex_RLELock& a, SWTex_RLELock& b) noexcept;

      SWTex_RLELock& drawPoint(const RGBA c, const geom::Pos p) noexcept;
      RGBA getPoint(const geom::Pos p) const noexcept;

    private:
      SWTex_RLELock() noexcept;

      SWTex_RLE* h_;
  };

  class Win_SW : public Win_Base {
    public:
      using Win_Base::Win_Base;
      ~Win_SW() noexcept; // todo: we could rely on the SDL_Surface do not free flag instead

      SWTex& swtex();

    private:
      SWTex tex;
  };
}
