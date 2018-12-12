#include "hpp/Tex.hpp"

#include <utility>
#include <cassert>

namespace rolmodl {
  using detail::throwOnErr;

  Tex::Tex() noexcept :
    h_(nullptr)
  {}
  Tex::Tex(Ren& r, const pixelfmt::Id fmt, const int access, const geom::Size s) :
    h_(SDL_CreateTexture(r.unsafeRaw(), pixelfmt::id::unsafe::toSDLEnum(fmt), access, s.w, s.h))
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

  BlendMode Tex::getBlendMode() {
    SDL_BlendMode tmp = SDL_BLENDMODE_NONE;
    throwOnErr(SDL_GetTextureBlendMode(unsafeRaw(), &tmp));
    return blendMode::unsafe::fromSDLEnum(tmp);
  }
  void Tex::setBlendMode(const BlendMode m) {
    throwOnErr(SDL_SetTextureBlendMode(unsafeRaw(), blendMode::unsafe::toSDLEnum(m)));
  }

  uint8_t Tex::getAlphaMod() {
    uint8_t res = 0;
    throwOnErr(SDL_GetTextureAlphaMod(unsafeRaw(), &res));
    return res;
  }
  void Tex::setAlphaMod(const uint8_t i) {
    throwOnErr(SDL_SetTextureAlphaMod(unsafeRaw(), i));
  }

  RGB Tex::getRGBMod() {
    RGB res{};
    throwOnErr(SDL_GetTextureColorMod(unsafeRaw(), &res.r, &res.g, &res.b));
    return res;
  }
  void Tex::setRGBMod(const RGB i) {
    throwOnErr(SDL_SetTextureColorMod(unsafeRaw(), i.r, i.g, i.b));
  }

  TextureInfo Tex::query() {
    uint32_t format = 0;
    int access = 0;

    TextureInfo res{};
    throwOnErr(SDL_QueryTexture(unsafeRaw(), &format, &access, &res.size.w, &res.size.h));

    res.fmt = pixelfmt::id::unsafe::fromSDLEnum(format);
    res.type = textureType::unsafe::fromSDLEnum(access);
    return res;
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


  LockTex::LockTex() noexcept :
    Tex(),
    format_(pixelfmt::Id::unknown)
  {}
  LockTex::LockTex(Ren& r, const pixelfmt::Id fmt, const geom::Size s) :
    Tex(r, fmt, SDL_TEXTUREACCESS_STREAMING, s),
    format_(fmt)
  {}

  pixelfmt::Id LockTex::format() const noexcept {
    assert(h_ != nullptr); // catch errors early
    return format_;
  }


  RenTex::RenTex(Ren& r, const pixelfmt::Id fmt, const geom::Size s) :
    Tex(r, fmt, SDL_TEXTUREACCESS_TARGET, s) {}


  TexLock::TexLock() noexcept :
    t_(nullptr),
    h_(nullptr),
    pitch_(0)
  {}
  TexLock::TexLock(LockTex& tex, const SDL_Rect* r) :
    t_(&tex),
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
    if (h_ != nullptr) {
      assert(t_ != nullptr);
      SDL_UnlockTexture(t_->unsafeRaw());
    }

    t_ = nullptr;
    h_ = nullptr;
    pitch_ = 0;
  }

  TexLock::TexLock(TexLock&& that) noexcept :
    TexLock()
  {
    std::swap(*this, that);
  }
  TexLock& TexLock::operator=(TexLock&& that) noexcept {
    std::swap(*this, that);
    return *this;
  }

  void swap(TexLock& a, TexLock& b) noexcept {
    using std::swap;
    swap(a.t_, b.t_);
    swap(a.h_, b.h_);
    swap(a.pitch_, b.pitch_);
  }

  TexLock& TexLock::drawPoint(const RGBA c, const geom::Pos p) noexcept {
    const SDL_PixelFormat f = pixelfmt::PixelFmtStorage::instance().get(t_->format());

    // fixme: assumes little endian
    const uint32_t mask = static_cast<uint32_t>((~0 << f.BitsPerPixel));

    uint32_t& val = unsafePoint(p);
    val = (val & mask) | SDL_MapRGBA(&f, c.r, c.g, c.b, c.a);

    return *this;
  }
  RGBA TexLock::getPoint(const geom::Pos p) const noexcept {
    const SDL_PixelFormat f = pixelfmt::PixelFmtStorage::instance().get(t_->format());

    RGBA c{};
    SDL_GetRGBA(unsafePoint(p), &f, &c.r, &c.g, &c.b, &c.a);

    return c;
  }

  uint32_t& TexLock::unsafePoint(const geom::Pos p) noexcept {
    return *reinterpret_cast<uint32_t*>(
      static_cast<char*>(unsafeRaw()) +
        static_cast<unsigned int>(p.y)*pitch_ +
        static_cast<unsigned int>(p.x)*pixelfmt::byteSizeOf(t_->format())
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
