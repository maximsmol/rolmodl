#include "hpp/Win.hpp"

#include <utility>
#include <cassert>

#include "hpp/Base.hpp"

namespace rolmodl {
  using detail::throwOnErr;

  /*explicit*/ BorderSizes::BorderSizes(Win_Base& w) :
    top_(0),
    l_(0),
    bot_(0),
    r_(0)
  {
    throwOnErr(SDL_GetWindowBordersSize(w.unsafeRaw(), &top_, &l_, &bot_, &r_));
  }

  int BorderSizes::top() const noexcept {
    return top_;
  }
  int BorderSizes::l() const noexcept {
    return l_;
  }
  int BorderSizes::bot() const noexcept {
    return bot_;
  }
  int BorderSizes::r() const noexcept {
    return r_;
  }


  Win_Base::Win_Base() noexcept :
    h_(nullptr)
  {}
  Win_Base::Win_Base(const char* title, const geom::Pos p, const geom::Size s, const win::Flags flags) :
    h_(SDL_CreateWindow(title, p.x, p.y, s.w, s.h, flags.raw()))
  {
    assert(0 <= s.w); assert(s.w <= 16384);
    assert(0 <= s.h); assert(s.h <= 16384);
    if (h_ == nullptr)
      throw sdlexception();
  }
  Win_Base::Win_Base(const char* title, const geom::Size s, const win::Flags flags) :
    Win_Base(title, geom::Pos{SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED}, s, flags) {}
  Win_Base::Win_Base(const char* title, const geom::Pos p, const geom::Size s) :
    Win_Base(title, p, s, win::Flags()) {}
  Win_Base::Win_Base(const char* title, const geom::Size s) :
    Win_Base(title, s, win::Flags()) {}

  Win_Base::~Win_Base() noexcept {
    if (h_ != nullptr)
      SDL_DestroyWindow(h_);
    h_ = nullptr;
  }

  // Win_Base::Win_Base(const Win_Base& that) = delete;
  Win_Base::Win_Base(Win_Base&& that) noexcept :
    Win_Base()
  {
    std::swap(*this, that);
  }

  // Win_Base& Win_Base::operator=(const Win_Base& that) = delete;
  Win_Base& Win_Base::operator=(Win_Base&& that) noexcept {
    std::swap(*this, that);
    return *this;
  }

  void swap(Win_Base& a, Win_Base& b) noexcept {
    using std::swap;
    swap(a.h_, b.h_);
  }

  // h_ can be nullptr from
  //   the private default constructor
  //     => its a rolmodl bug
  //   SDL
  //     => user should have handled the exception and not tried to use the window
  SDL_Window* Win_Base::unsafeRaw() noexcept {
    assert(h_ != nullptr);
    return h_;
  }
  const SDL_Window* Win_Base::unsafeRaw() const noexcept {
    assert(h_ != nullptr);
    return h_;
  }

  uint32_t Win_Base::unsafeId() {
    const uint32_t res = SDL_GetWindowID(unsafeRaw());
    if (res == 0)
      throw sdlexception();
    return res;
  }
  pixelfmt::Id Win_Base::pixelFmt() {
    const uint32_t f = SDL_GetWindowPixelFormat(unsafeRaw());
    if (f == SDL_PIXELFORMAT_UNKNOWN)
      throw sdlexception();
    return pixelfmt::id::unsafe::fromSDLEnum(f);
  }
  void Win_Base::moveMouseIn(const geom::Pos p) noexcept {
    SDL_WarpMouseInWindow(unsafeRaw(), p.x, p.y);
  }


  void Win_Base::hide() noexcept {
    SDL_HideWindow(unsafeRaw());
  }
  void Win_Base::show() noexcept {
    SDL_ShowWindow(unsafeRaw());
  }
  void Win_Base::raise() noexcept {
    SDL_RaiseWindow(unsafeRaw());
  }

  void Win_Base::maximize() noexcept {
    SDL_MaximizeWindow(unsafeRaw());
  }
  void Win_Base::minimize() noexcept {
    SDL_MinimizeWindow(unsafeRaw());
  }
  void Win_Base::restore() noexcept {
    SDL_RestoreWindow(unsafeRaw());
  }

  unsigned int Win_Base::unsafeDisplayIndex() {
    return static_cast<unsigned int>(throwOnErr(SDL_GetWindowDisplayIndex(unsafeRaw())));
  }
  sys::Display Win_Base::display() {
    return sys::display::unsafe::byIndex(unsafeDisplayIndex());
  }

  float Win_Base::opacity() {
    float res = 0;
    throwOnErr(SDL_GetWindowOpacity(unsafeRaw(), &res));
    return res;
  }
  void Win_Base::setOpacity(const float v) {
    throwOnErr(SDL_SetWindowOpacity(unsafeRaw(), v));
  }

  geom::Pos Win_Base::pos() noexcept {
    geom::Pos res{};
    SDL_GetWindowPosition(unsafeRaw(), &res.x, &res.y);
    return res;
  }
  void Win_Base::setPos(const geom::Pos p) noexcept {
    SDL_SetWindowPosition(unsafeRaw(), p.x, p.y);
  }

  const char* Win_Base::title() noexcept {
    return SDL_GetWindowTitle(unsafeRaw());
  }
  void Win_Base::setTitle(const char* str) noexcept {
    SDL_SetWindowTitle(unsafeRaw(), str);
  }


  geom::Size Win_Base::size() noexcept {
    geom::Size res{};
    SDL_GetWindowSize(unsafeRaw(), &res.w, &res.h);
    return res;
  }
  void Win_Base::setSize(const geom::Size s) noexcept {
    SDL_SetWindowSize(unsafeRaw(), s.w, s.h);
  }

  geom::Size Win_Base::maxSize() noexcept {
    geom::Size res{};
    SDL_GetWindowMaximumSize(unsafeRaw(), &res.w, &res.h);
    return res;
  }
  void Win_Base::setMaxSize(const geom::Size s) noexcept {
    SDL_SetWindowMaximumSize(unsafeRaw(), s.w, s.h);
  }

  geom::Size Win_Base::minSize() noexcept {
    geom::Size res{};
    SDL_GetWindowMinimumSize(unsafeRaw(), &res.w, &res.h);
    return res;
  }
  void Win_Base::setMinSize(const geom::Size s) noexcept {
    SDL_SetWindowMinimumSize(unsafeRaw(), s.w, s.h);
  }
}
