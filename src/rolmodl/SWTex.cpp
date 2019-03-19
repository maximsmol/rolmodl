#include "hpp/SWTex.hpp"

#include <utility>
#include <cassert>

namespace rolmodl {
  using detail::throwOnErr;

  SWTex_Base::SWTex_Base() :
    h_(nullptr)
  {};
  SWTex_Base::SWTex_Base(const geom::Size s, const pixelfmt::Id fmt) :
    h_(SDL_CreateRGBSurfaceWithFormat(0, s.w, s.h, static_cast<int>(pixelfmt::bitSizeOf(fmt)), pixelfmt::id::unsafe::toSDLEnum(fmt)))
  {
    if (h_ == nullptr)
      throw sdlexception();
  }

  SWTex_Base::~SWTex_Base() noexcept {
    if (h_ != nullptr)
      SDL_FreeSurface(h_);
    h_ = nullptr;
  }

  SWTex_Base::SWTex_Base(SWTex_Base&& that) noexcept :
    SWTex_Base()
  {
    std::swap(*this, that);
  }
  SWTex_Base& SWTex_Base::operator=(SWTex_Base&& that) noexcept {
    std::swap(*this, that);
    return *this;
  }

  void swap(SWTex_Base& a, SWTex_Base& b) noexcept {
    using std::swap;
    swap(a.h_, b.h_);
  }

  pixelfmt::Id SWTex_Base::fmtId() noexcept {
    return pixelfmt::id::unsafe::fromSDLEnum(fmt().format);
  }
  pixelfmt::Id SWTex_Base::fmtId() const noexcept {
    return pixelfmt::id::unsafe::fromSDLEnum(fmt().format);
  }

  SDL_PixelFormat& SWTex_Base::fmt() noexcept {
    return *unsafeRaw()->format;
  }
  SDL_PixelFormat& SWTex_Base::fmt() const noexcept {
    return *unsafeRaw()->format;
  }

  unsigned int SWTex_Base::pitch() noexcept {
    return static_cast<unsigned int>(unsafeRaw()->pitch);
  }
  unsigned int SWTex_Base::pitch() const noexcept {
    return static_cast<unsigned int>(unsafeRaw()->pitch);
  }

  int SWTex_Base::w() noexcept {
    return unsafeRaw()->w;
  }
  int SWTex_Base::w() const noexcept {
    return unsafeRaw()->w;
  }
  int SWTex_Base::h() noexcept {
    return unsafeRaw()->h;
  }
  int SWTex_Base::h() const noexcept {
    return unsafeRaw()->h;
  }

  bool SWTex_Base::mustLock() noexcept {
    return SDL_MUSTLOCK(unsafeRaw()) == SDL_TRUE;
  }
  bool SWTex_Base::mustLock() const noexcept {
    return SDL_MUSTLOCK(unsafeRaw()) == SDL_TRUE;
  }

  // h_ can be nullptr from
  //   the private default constructor
  //     => its a rolmodl bug
  //   SDL
  //     => user should have handled the exception and not tried to use the lock
  SDL_Surface* SWTex_Base::unsafeRaw() noexcept {
    assert(h_ != nullptr);
    return h_;
  }
  const SDL_Surface* SWTex_Base::unsafeRaw() const noexcept {
    assert(h_ != nullptr);
    return h_;
  }

  uint32_t& SWTex_Base::unsafePoint(const geom::Pos p) noexcept {
    return *reinterpret_cast<uint32_t*>(
      static_cast<char*>(unsafeRawPixels()) +
        static_cast<unsigned int>(p.y)*pitch() +
        static_cast<unsigned int>(p.x)*fmt().BytesPerPixel
    );
  }
  const uint32_t& SWTex_Base::unsafePoint(const geom::Pos p) const noexcept {
    return *reinterpret_cast<const uint32_t*>(static_cast<const char*>(unsafeRawPixels()) + static_cast<unsigned int>(p.y)*pitch() + p.x);
  }

  void* SWTex_Base::unsafeRawPixels() noexcept {
    return unsafeRaw()->pixels;
  }
  const void* SWTex_Base::unsafeRawPixels() const noexcept {
    return unsafeRaw()->pixels;
  }

  SWTex_Base& SWTex_Base::drawPoint(const RGBA c, const geom::Pos p) noexcept {
    // fixme: assumes little endian
    const uint32_t mask = static_cast<uint32_t>((~0 << fmt().BitsPerPixel));

    uint32_t& val = unsafePoint(p);
    val = (val & mask) | SDL_MapRGBA(&fmt(), c.r, c.g, c.b, c.a);

    return *this;
  }
  RGBA SWTex_Base::getPoint(const geom::Pos p) const noexcept {
    RGBA c{};
    SDL_GetRGBA(unsafePoint(p), &fmt(), &c.r, &c.g, &c.b, &c.a);

    return c;
  }


  SWTex::SWTex() :
    SWTex_Base()
  {}
  SWTex::SWTex(const geom::Size s, const pixelfmt::Id fmt) :
    SWTex_Base(s, fmt)
  {
    assert(!mustLock()); // fixme: assuming that new surfaces are always non-RLE
  }
  /*explicit*/ SWTex::SWTex(SWTex_RLE&& rle) noexcept :
    SWTex_Base()
  {
    std::swap(h_, rle.h_);
    throwOnErr(SDL_SetSurfaceRLE(unsafeRaw(), false));
  }

  SWTex& SWTex::drawPoint(const RGBA c, const geom::Pos p) noexcept {
    SWTex_Base::drawPoint(c, p);
    return *this;
  }
  RGBA SWTex::getPoint(const geom::Pos p) const noexcept {
    return SWTex_Base::getPoint(p);
  }


  SWTex_RLE::SWTex_RLE(const geom::Size s, const pixelfmt::Id fmt) :
    SWTex_Base(s, fmt)
  {
    assert(!mustLock()); // fixme: assuming that new surfaces are always non-RLE
    throwOnErr(SDL_SetSurfaceRLE(unsafeRaw(), true));
    assert(mustLock()); // fixme: assuming that new surfaces are always non-RLE
  }
  /*explicit*/ SWTex_RLE::SWTex_RLE(SWTex&& nonRle) noexcept :
    SWTex_Base()
  {
    std::swap(h_, nonRle.h_);
    throwOnErr(SDL_SetSurfaceRLE(unsafeRaw(), true));
  }


  SWTex_RLELock::SWTex_RLELock() noexcept :
    h_(nullptr)
  {}
  /*explicit*/ SWTex_RLELock::SWTex_RLELock(SWTex_RLE& t) :
    h_(&t)
  {
    throwOnErr(SDL_LockSurface(t.unsafeRaw()));
  }

  SWTex_RLELock::~SWTex_RLELock() noexcept {
    if (h_ != nullptr)
      SDL_UnlockSurface(h_->unsafeRaw());
    h_ = nullptr;
  }

  SWTex_RLELock::SWTex_RLELock(SWTex_RLELock&& that) noexcept :
    SWTex_RLELock()
  {
    std::swap(*this, that);
  }
  SWTex_RLELock& SWTex_RLELock::operator=(SWTex_RLELock&& that) noexcept {
    std::swap(*this, that);
    return *this;
  }

  void swap(SWTex_RLELock& a, SWTex_RLELock& b) noexcept {
    using std::swap;
    swap(a.h_, b.h_);
  }

  SWTex_RLELock& SWTex_RLELock::drawPoint(const RGBA c, const geom::Pos p) noexcept {
    h_->drawPoint(c, p);
    return *this;
  }
  RGBA SWTex_RLELock::getPoint(const geom::Pos p) const noexcept {
    return h_->getPoint(p);
  }


  SWTex& Win_SW::swtex() {
    if (tex.h_ != nullptr)
      return tex;

    SDL_Surface* res = SDL_GetWindowSurface(unsafeRaw());
    if (res == nullptr)
      throw sdlexception();
    tex.h_ = res;
    assert(!tex.mustLock()); // fixme

    return tex;
  }
  Win_SW::~Win_SW() noexcept {
    tex.h_ = nullptr; // don't free the SW tex
  }
}
