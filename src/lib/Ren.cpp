#include "hpp/Ren.hpp"

#include <utility>
#include <cassert>

namespace rolmodl {
  using detail::throwOnErr;

  Ren::Ren() noexcept :
    h_(nullptr)
  {}
  Ren::Ren(Win& win, int i, ren::Flags flags) :
    h_(SDL_CreateRenderer(win.unsafeRaw(), i, flags.raw()))
  {
    if (h_ == nullptr)
      throw sdlexception();
  }
  Ren::Ren(Win& win, ren::Flags flags) :
    Ren(win, -1, flags) {}
  Ren::Ren(Win& win, int i) :
    Ren(win, i, ren::Flags()) {}
  /*explicit*/ Ren::Ren(Win& win) :
    Ren(win, -1) {}

  Ren::~Ren() noexcept {
    if (h_ != nullptr)
      SDL_DestroyRenderer(h_);
    h_ = nullptr;
  }

  // Ren::Ren(const Ren& that) = delete;
  Ren::Ren(Ren&& that) noexcept :
    Ren()
  {
    std::swap(*this, that);
  }

  // Ren& Ren::operator=(const Ren& that) = delete;
  Ren& Ren::operator=(Ren&& that) noexcept {
    std::swap(*this, that);
    return *this;
  }

  void swap(Ren& a, Ren& b) noexcept {
    using std::swap;
    swap(a.h_, b.h_);
  }

  // h_ can be nullptr from
  //   the private default constructor
  //     => its a rolmodl bug
  //   SDL
  //     => user should have handled the exception and not tried to use the renderer
  SDL_Renderer* Ren::unsafeRaw() noexcept {
    assert(h_ != nullptr);
    return h_;
  }
  const SDL_Renderer* Ren::unsafeRaw() const noexcept {
    assert(h_ != nullptr);
    return h_;
  }


  void Ren::clear() {
    throwOnErr(SDL_RenderClear(unsafeRaw()));
  }
  void Ren::present() noexcept {
    SDL_RenderPresent(unsafeRaw());
  }

  void Ren::drawLine(const geom::Pos a, const geom::Pos b) {
    throwOnErr(SDL_RenderDrawLine(unsafeRaw(), a.x, a.y, b.x, b.y));
  }
  // void Ren::drawLines(?); // fixme: implement this generically

  void Ren::drawPoint(const geom::Pos p) {
    throwOnErr(SDL_RenderDrawPoint(unsafeRaw(), p.x, p.y));
  }
  // void Ren::drawPoints(?);  // fixme: implement this generically

  void Ren::drawRect(const geom::RectWH r) {
    SDL_Rect tmp = r.sdl();
    throwOnErr(SDL_RenderDrawRect(unsafeRaw(), &tmp));
  }
  void Ren::drawRect(const geom::RectXY r) {
    drawRect(r.wh());
  }
  // void Ren::drawRects(?);  // fixme: implement this generically

  void Ren::fillRect(const geom::RectWH r) {
    SDL_Rect tmp = r.sdl();
    throwOnErr(SDL_RenderFillRect(unsafeRaw(), &tmp));
  }
  void Ren::fillRect(const geom::RectXY r) {
    fillRect(r.wh());
  }
  // void Ren::fillRects(?);  // fixme: implement this generically

  void Ren::outlineScreen() {
    throwOnErr(SDL_RenderDrawRect(unsafeRaw(), nullptr));
  }
  void Ren::fillScreen() {
    throwOnErr(SDL_RenderFillRect(unsafeRaw(), nullptr));
  }

  std::optional<geom::RectWH> Ren::clipRect() noexcept {
    SDL_Rect tmp{};
    SDL_RenderGetClipRect(unsafeRaw(), &tmp);
    if (tmp.w != 0 || tmp.h != 0) // todo: what is an empty rectangle?
      return geom::RectWH(tmp);
    return {};
  }
  void Ren::setClipRect(const geom::RectWH r) {
    SDL_Rect tmp = r.sdl();
    throwOnErr(SDL_RenderSetClipRect(unsafeRaw(), &tmp));
  }
  void Ren::setClipRect(const geom::RectXY r) {
    setClipRect(r.wh());
  }
  void Ren::disableClip() {
    throwOnErr(SDL_RenderSetClipRect(unsafeRaw(), nullptr));
  }

  std::optional<geom::Size> Ren::logicalSize() noexcept {
    geom::Size tmp{};
    SDL_RenderGetLogicalSize(unsafeRaw(), &tmp.w, &tmp.h);
    if (tmp.w != 0 || tmp.h != 0)
      return tmp;
    return {};
  }
  void Ren::setLogicalSize(const geom::Size s) {
    throwOnErr(SDL_RenderSetLogicalSize(unsafeRaw(), s.w, s.h));
  }

  RenScale Ren::scale() noexcept {
    RenScale res{};
    SDL_RenderGetScale(unsafeRaw(), &res.x, &res.y);
    return res;
  }
  void Ren::setScale(const RenScale s) {
    throwOnErr(SDL_RenderSetScale(unsafeRaw(), s.x, s.y));
  }

  geom::RectWH Ren::viewport() noexcept {
    SDL_Rect tmp{};
    SDL_RenderGetViewport(unsafeRaw(), &tmp);
    return geom::RectWH(tmp);
  }
  void Ren::setViewport(const geom::RectWH r) {
    SDL_Rect tmp = r.sdl();
    throwOnErr(SDL_RenderSetViewport(unsafeRaw(), &tmp));
  }


  Color Ren::color() {
    Color res{};
    throwOnErr(SDL_GetRenderDrawColor(unsafeRaw(), &res.r, &res.g, &res.b, &res.a));
    return res;
  }

  void Ren::setColor(const Color c) {
    throwOnErr(SDL_SetRenderDrawColor(unsafeRaw(), c.r, c.g, c.b, c.a));
  }
}
