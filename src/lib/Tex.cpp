#include "hpp/Tex.hpp"

namespace rolmodl {
  using detail::throwOnErr;

  Tex::Tex() noexcept :
    h_(nullptr),
    format_(pixelfmt::Id::unknown)
  {}
  Tex::Tex(Ren& r, const pixelfmt::Id fmt, const int access, const geom::Size s) :
    h_(SDL_CreateTexture(r.unsafeRaw(), pixelfmt::id::unsafe::toSDLEnum(fmt), access, s.w, s.h)),
    format_(fmt)
  {
    if (h_ == nullptr)
      throw sdlexception();
  }

  Tex::~Tex() noexcept {
    if (h_ != nullptr)
      SDL_DestroyTexture(h_);
    h_ = nullptr;
  }

  Tex::Tex(Tex&& that) noexcept :
    Tex()
  {
    std::swap(*this, that);
  }
  Tex& Tex::operator=(Tex&& that) noexcept {
    std::swap(*this, that);
    return *this;
  }

  void swap(Tex& a, Tex& b) noexcept {
    using std::swap;
    swap(a.h_, b.h_);
  }

  pixelfmt::Id Tex::format() const noexcept {
    assert(h_ != nullptr); // catch errors early
    return format_;
  }

  // h_ can be nullptr from
  //   the private default constructor
  //     => its a rolmodl bug
  //   SDL
  //     => user should have handled the exception and not tried to use the texture
  SDL_Texture* Tex::unsafeRaw() noexcept {
    assert(h_ != nullptr);
    return h_;
  }
  const SDL_Texture* Tex::unsafeRaw() const noexcept {
    assert(h_ != nullptr);
    return h_;
  }

  StaticTex::StaticTex(Ren& r, const pixelfmt::Id fmt, const geom::Size s) :
    Tex(r, fmt, SDL_TEXTUREACCESS_STATIC, s) {}


  LockTex::LockTex(Ren& r, const pixelfmt::Id fmt, const geom::Size s) :
    Tex(r, fmt, SDL_TEXTUREACCESS_STREAMING, s) {}


  RenTex::RenTex(Ren& r, const pixelfmt::Id fmt, const geom::Size s) :
    Tex(r, fmt, SDL_TEXTUREACCESS_TARGET, s) {}


  TexLock::TexLock(LockTex& tex, const SDL_Rect* r) :
    t_(tex),
    h_(nullptr),
    pitch_(0)
  {
    int pitch = 0;
    throwOnErr(SDL_LockTexture(tex.unsafeRaw(), r, &h_, &pitch));
    pitch_ = static_cast<unsigned int>(pitch);
  }
  TexLock::TexLock(LockTex& tex, const SDL_Rect r) :
    TexLock(tex, &r) {}

  /*explicit*/ TexLock::TexLock(LockTex& tex) :
    TexLock(tex, nullptr) {}
  TexLock::TexLock(LockTex& tex, const geom::RectWH r) :
    TexLock(tex, r.sdl()) {}
  TexLock::TexLock(LockTex& tex, const geom::RectXY r) :
    TexLock(tex, r.wh()) {}

  TexLock::~TexLock() noexcept {
    if (h_ != nullptr)
      SDL_UnlockTexture(t_.unsafeRaw());

    h_ = nullptr;
    pitch_ = 0;
  }

  TexLock& TexLock::drawPoint(const RGBA c, const geom::Pos p) noexcept {
    SDL_PixelFormat f = pixelfmt::PixelFmtStorage::instance().get(t_.format());

    uint32_t mask = static_cast<unsigned int>((~0 >> (32 - f.BitsPerPixel)));

    uint32_t& val = unsafePoint(p);
    val = (val & ~mask) | SDL_MapRGBA(&f, c.r, c.g, c.b, c.a);

    return *this;
  }
  RGBA TexLock::getPoint(const geom::Pos p) const noexcept {
    SDL_PixelFormat f = pixelfmt::PixelFmtStorage::instance().get(t_.format());

    RGBA c{};
    SDL_GetRGBA(unsafePoint(p), &f, &c.r, &c.g, &c.b, &c.a);

    return c;
  }

  uint32_t& TexLock::unsafePoint(const geom::Pos p) noexcept {
    return *reinterpret_cast<uint32_t*>(
      static_cast<char*>(unsafeRaw()) +
        static_cast<unsigned int>(p.y)*pitch_ +
        static_cast<unsigned int>(p.x)*pixelfmt::byteSizeOf(t_.format())
    );
  }
  const uint32_t& TexLock::unsafePoint(const geom::Pos p) const noexcept {
    return *reinterpret_cast<const uint32_t*>(static_cast<const char*>(unsafeRaw()) + static_cast<unsigned int>(p.y)*pitch_ + p.x);
  }

  // h_ can be nullptr from
  //   the private default constructor
  //     => its a rolmodl bug
  //   SDL
  //     => user should have handled the exception and not tried to use the lock
  void* TexLock::unsafeRaw() noexcept {
    assert(h_ != nullptr);
    return h_;
  }
  const void* TexLock::unsafeRaw() const noexcept {
    assert(h_ != nullptr);
    return h_;
  }
}
