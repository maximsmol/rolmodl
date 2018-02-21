#include "hpp/Win.hpp"

#include <utility>
#include <cassert>

#include "hpp/Base.hpp"

namespace rolmodl {
  using detail::throwOnErr;

  /*explicit*/ BorderSizes::BorderSizes(Win& w) :
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


  Win::Win() noexcept :
    h_(nullptr)
  {}
  Win::Win(const char* title, const geom::Pos p, const geom::Size s, const win::Flags flags) :
    h_(SDL_CreateWindow(title, p.x, p.y, s.w, s.h, flags.raw()))
  {
    assert(0 <= s.w); assert(s.w <= 16384);
    assert(0 <= s.h); assert(s.h <= 16384);
    if (h_ == nullptr)
      throw sdlexception();
  }
  Win::Win(const char* title, const geom::Size s, const win::Flags flags) :
    Win(title, geom::Pos{SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED}, s, flags) {}
  Win::Win(const char* title, const geom::Pos p, const geom::Size s) :
    Win(title, p, s, win::Flags()) {}
  Win::Win(const char* title, const geom::Size s) :
    Win(title, s, win::Flags()) {}

  Win::~Win() noexcept {
    if (h_ != nullptr)
      SDL_DestroyWindow(h_);
    h_ = nullptr;
  }

  // Win::Win(const Win& that) = delete;
  Win::Win(Win&& that) noexcept :
    Win()
  {
    std::swap(*this, that);
  }

  // Win& Win::operator=(const Win& that) = delete;
  Win& Win::operator=(Win&& that) noexcept {
    std::swap(*this, that);
    return *this;
  }

  void swap(Win& a, Win& b) noexcept {
    using std::swap;
    swap(a.h_, b.h_);
  }

  // h_ can be nullptr from
  //   the private default constructor
  //     => its a rolmodl bug
  //   SDL
  //     => user should have handled the exception and not tried to use the window
  SDL_Window* Win::unsafeRaw() noexcept {
    assert(h_ != nullptr);
    return h_;
  }
  const SDL_Window* Win::unsafeRaw() const noexcept {
    assert(h_ != nullptr);
    return h_;
  }

  uint32_t Win::unsafeId() {
    const uint32_t res = SDL_GetWindowID(unsafeRaw());
    if (res == 0)
      throw sdlexception();
    return res;
  }
  pixelfmt::Id Win::pixelFmt() {
    const uint32_t f = SDL_GetWindowPixelFormat(unsafeRaw());
    if (f == SDL_PIXELFORMAT_UNKNOWN)
      throw sdlexception();
    return pixelfmt::id::unsafe::fromSDLEnum(f);
  }
  void Win::moveMouseIn(const geom::Pos p) noexcept {
    SDL_WarpMouseInWindow(unsafeRaw(), p.x, p.y);
  }


  void Win::hide() noexcept {
    SDL_HideWindow(unsafeRaw());
  }
  void Win::show() noexcept {
    SDL_ShowWindow(unsafeRaw());
  }
  void Win::raise() noexcept {
    SDL_RaiseWindow(unsafeRaw());
  }

  void Win::maximize() noexcept {
    SDL_MaximizeWindow(unsafeRaw());
  }
  void Win::minimize() noexcept {
    SDL_MinimizeWindow(unsafeRaw());
  }
  void Win::restore() noexcept {
    SDL_RestoreWindow(unsafeRaw());
  }

  unsigned int Win::unsafeDisplayIndex() {
    return static_cast<unsigned int>(throwOnErr(SDL_GetWindowDisplayIndex(unsafeRaw())));
  }
  sys::Display Win::display() {
    return sys::display::unsafe::byIndex(unsafeDisplayIndex());
  }

  float Win::opacity() {
    float res = 0;
    throwOnErr(SDL_GetWindowOpacity(unsafeRaw(), &res));
    return res;
  }
  void Win::setOpacity(const float v) {
    throwOnErr(SDL_SetWindowOpacity(unsafeRaw(), v));
  }

  geom::Pos Win::pos() noexcept {
    geom::Pos res{};
    SDL_GetWindowPosition(unsafeRaw(), &res.x, &res.y);
    return res;
  }
  void Win::setPos(const geom::Pos p) noexcept {
    SDL_SetWindowPosition(unsafeRaw(), p.x, p.y);
  }

  const char* Win::title() noexcept {
    return SDL_GetWindowTitle(unsafeRaw());
  }
  void Win::setTitle(const char* str) noexcept {
    SDL_SetWindowTitle(unsafeRaw(), str);
  }


  geom::Size Win::size() noexcept {
    geom::Size res{};
    SDL_GetWindowSize(unsafeRaw(), &res.w, &res.h);
    return res;
  }
  void Win::setSize(const geom::Size s) noexcept {
    SDL_SetWindowSize(unsafeRaw(), s.w, s.h);
  }

  geom::Size Win::maxSize() noexcept {
    geom::Size res{};
    SDL_GetWindowMaximumSize(unsafeRaw(), &res.w, &res.h);
    return res;
  }
  void Win::setMaxSize(const geom::Size s) noexcept {
    SDL_SetWindowMaximumSize(unsafeRaw(), s.w, s.h);
  }

  geom::Size Win::minSize() noexcept {
    geom::Size res{};
    SDL_GetWindowMinimumSize(unsafeRaw(), &res.w, &res.h);
    return res;
  }
  void Win::setMinSize(const geom::Size s) noexcept {
    SDL_SetWindowMinimumSize(unsafeRaw(), s.w, s.h);
  }
}
