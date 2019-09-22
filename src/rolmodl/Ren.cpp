#include "hpp/Ren.hpp"

#include <algorithm>
#include <utility>
#include <cassert>

#include "hpp/Base.hpp"
#include "hpp/Win.hpp"
#include "hpp/Tex.hpp"

namespace rolmodl {
  using detail::throwOnErr;

  namespace ren::driver {
    unsigned int count() {
      int res = SDL_GetNumRenderDrivers();
      throwOnErr(res);
      return static_cast<unsigned int>(res);
    }

    Info info(const unsigned int i) {
      if (i >= count())
        throw std::out_of_range("rolmodl::ren::driver::info");
      SDL_RendererInfo tmp{};
      throwOnErr(SDL_GetRenderDriverInfo(static_cast<int>(i), &tmp));

      Info res{};
      res.name = tmp.name;
      res.flags = Flags::unsafeFromRaw(tmp.flags);

      res.pixelFmts = std::vector<pixelfmt::Id>(tmp.num_texture_formats);
      for (unsigned int j = 0; j < tmp.num_texture_formats; ++j)
        res.pixelFmts[j] = pixelfmt::id::unsafe::fromSDLEnum(tmp.texture_formats[j]);

      res.maxTexSize = geom::Size{tmp.max_texture_width, tmp.max_texture_height};

      return res;
    }
  }

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
    swap(*this, that);
  }

  // Ren& Ren::operator=(const Ren& that) = delete;
  Ren& Ren::operator=(Ren&& that) noexcept {
    swap(*this, that);
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

  // copy
  void Ren::drawTex(Tex& tex) {
    throwOnErr(SDL_RenderCopy(unsafeRaw(), tex.unsafeRaw(), nullptr, nullptr));
  }

  void Ren::drawTex(Tex& tex, const SrcRectWH src) {
    SDL_Rect tmp = src.sdl();
    throwOnErr(SDL_RenderCopy(unsafeRaw(), tex.unsafeRaw(), &tmp, nullptr));
  }
  void Ren::drawTex(Tex& tex, const SrcRectXY src) {
    drawTex(tex, src.wh());
  }

  void Ren::drawTex(Tex& tex, const DstRectWH dst) {
    SDL_Rect tmp = dst.sdl();
    throwOnErr(SDL_RenderCopy(unsafeRaw(), tex.unsafeRaw(), nullptr, &tmp));
  }
  void Ren::drawTex(Tex& tex, const DstRectXY dst) {
    drawTex(tex, dst.wh());
  }

  void Ren::drawTex(Tex& tex, const SrcRectWH src, const DstRectWH dst) {
    SDL_Rect srcTmp = src.sdl();
    SDL_Rect dstTmp = dst.sdl();
    throwOnErr(SDL_RenderCopy(unsafeRaw(), tex.unsafeRaw(), &srcTmp, &dstTmp));
  }
  void Ren::drawTex(Tex& tex, const SrcRectWH src, const DstRectXY dst) {
    drawTex(tex, src, dst.wh());
  }
  void Ren::drawTex(Tex& tex, const SrcRectXY src, const DstRectWH dst) {
    drawTex(tex, src.wh(), dst);
  }
  void Ren::drawTex(Tex& tex, const SrcRectXY src, const DstRectXY dst) {
    drawTex(tex, src.wh(), dst.wh());
  }

  // copyEx
  void Ren::drawTex(Tex& tex, const Flip flip) {
    drawTex(tex, 0, flip);
  }
  void Ren::drawTex(Tex& tex, const double rot, const Flip flip/* = Flip()*/) {
    throwOnErr(SDL_RenderCopyEx(unsafeRaw(), tex.unsafeRaw(), nullptr, nullptr, rot, nullptr, flip.toSDLEnum()));
  }
  void Ren::drawTex(Tex& tex, const double rot, const geom::Pos rotCenter, const Flip flip/* = Flip()*/) {
    SDL_Point p{rotCenter.x, rotCenter.y};
    throwOnErr(SDL_RenderCopyEx(unsafeRaw(), tex.unsafeRaw(), nullptr, nullptr, rot, &p, flip.toSDLEnum()));
  }

  void Ren::drawTex(Tex& tex, const SrcRectWH src, const Flip flip) {
    drawTex(tex, src, 0, flip);
  }
  void Ren::drawTex(Tex& tex, const SrcRectXY src, const Flip flip) {
    drawTex(tex, src.wh(), flip);
  }
  void Ren::drawTex(Tex& tex, const SrcRectWH src, const double rot, const Flip flip/* = Flip()*/) {
    SDL_Rect srcTmp = src.sdl();
    throwOnErr(SDL_RenderCopyEx(unsafeRaw(), tex.unsafeRaw(), &srcTmp, nullptr, rot, nullptr, flip.toSDLEnum()));
  }
  void Ren::drawTex(Tex& tex, const SrcRectXY src, const double rot, const Flip flip/* = Flip()*/) {
    drawTex(tex, src.wh(), rot, flip);
  }
  void Ren::drawTex(Tex& tex, const SrcRectWH src, const double rot, const geom::Pos rotCenter, const Flip flip/* = Flip()*/) {
    SDL_Rect srcTmp = src.sdl();
    SDL_Point p{rotCenter.x, rotCenter.y};
    throwOnErr(SDL_RenderCopyEx(unsafeRaw(), tex.unsafeRaw(), &srcTmp, nullptr, rot, &p, flip.toSDLEnum()));
  }
  void Ren::drawTex(Tex& tex, const SrcRectXY src, const double rot, const geom::Pos rotCenter, const Flip flip/* = Flip()*/) {
    drawTex(tex, src.wh(), rot, rotCenter, flip);
  }

  void Ren::drawTex(Tex& tex, const DstRectWH dst, const Flip flip) {
    drawTex(tex, dst, 0, flip);
  }
  void Ren::drawTex(Tex& tex, const DstRectXY dst, const Flip flip) {
    drawTex(tex, dst.wh(), flip);
  }
  void Ren::drawTex(Tex& tex, const DstRectWH dst, const double rot, const Flip flip/* = Flip()*/) {
    SDL_Rect dstTmp = dst.sdl();
    throwOnErr(SDL_RenderCopyEx(unsafeRaw(), tex.unsafeRaw(), nullptr, &dstTmp, rot, nullptr, flip.toSDLEnum()));
  }
  void Ren::drawTex(Tex& tex, const DstRectXY dst, const double rot, const Flip flip/* = Flip()*/) {
    drawTex(tex, dst.wh(), rot, flip);
  }
  void Ren::drawTex(Tex& tex, const DstRectWH dst, const double rot, const geom::Pos rotCenter, const Flip flip/* = Flip()*/) {
    SDL_Rect dstTmp = dst.sdl();
    SDL_Point p{rotCenter.x, rotCenter.y};
    throwOnErr(SDL_RenderCopyEx(unsafeRaw(), tex.unsafeRaw(), nullptr, &dstTmp, rot, &p, flip.toSDLEnum()));
  }
  void Ren::drawTex(Tex& tex, const DstRectXY dst, const double rot, const geom::Pos rotCenter, const Flip flip/* = Flip()*/) {
    drawTex(tex, dst.wh(), rot, rotCenter, flip);
  }

  void Ren::drawTex(Tex& tex, const SrcRectWH src, const DstRectWH dst, const Flip flip) {
    drawTex(tex, src, dst, 0, flip);
  }
  void Ren::drawTex(Tex& tex, const SrcRectWH src, const DstRectXY dst, const Flip flip) {
    drawTex(tex, src, dst.wh(), flip);
  }
  void Ren::drawTex(Tex& tex, const SrcRectXY src, const DstRectWH dst, const Flip flip) {
    drawTex(tex, src.wh(), dst, flip);
  }
  void Ren::drawTex(Tex& tex, const SrcRectXY src, const DstRectXY dst, const Flip flip) {
    drawTex(tex, src.wh(), dst.wh(), flip);
  }
  void Ren::drawTex(Tex& tex, const SrcRectWH src, const DstRectWH dst, const double rot, const Flip flip/* = Flip()*/) {
    SDL_Rect srcTmp = src.sdl();
    SDL_Rect dstTmp = dst.sdl();
    throwOnErr(SDL_RenderCopyEx(unsafeRaw(), tex.unsafeRaw(), &srcTmp, &dstTmp, rot, nullptr, flip.toSDLEnum()));
  }
  void Ren::drawTex(Tex& tex, const SrcRectWH src, const DstRectXY dst, const double rot, const Flip flip/* = Flip()*/) {
    drawTex(tex, src, dst.wh(), rot, flip);
  }
  void Ren::drawTex(Tex& tex, const SrcRectXY src, const DstRectWH dst, const double rot, const Flip flip/* = Flip()*/) {
    drawTex(tex, src.wh(), dst, rot, flip);
  }
  void Ren::drawTex(Tex& tex, const SrcRectXY src, const DstRectXY dst, const double rot, const Flip flip/* = Flip()*/) {
    drawTex(tex, src.wh(), dst.wh(), rot, flip);
  }
  void Ren::drawTex(Tex& tex, const SrcRectWH src, const DstRectWH dst, const double rot, const geom::Pos rotCenter, const Flip flip/* = Flip()*/) {
    SDL_Rect srcTmp = src.sdl();
    SDL_Rect dstTmp = dst.sdl();
    SDL_Point p{rotCenter.x, rotCenter.y};
    throwOnErr(SDL_RenderCopyEx(unsafeRaw(), tex.unsafeRaw(), &srcTmp, &dstTmp, rot, &p, flip.toSDLEnum()));
  }
  void Ren::drawTex(Tex& tex, const SrcRectWH src, const DstRectXY dst, const double rot, const geom::Pos rotCenter, const Flip flip/* = Flip()*/) {
    drawTex(tex, src, dst.wh(), rot, rotCenter, flip);
  }
  void Ren::drawTex(Tex& tex, const SrcRectXY src, const DstRectWH dst, const double rot, const geom::Pos rotCenter, const Flip flip/* = Flip()*/) {
    drawTex(tex, src.wh(), dst, rot, rotCenter, flip);
  }
  void Ren::drawTex(Tex& tex, const SrcRectXY src, const DstRectXY dst, const double rot, const geom::Pos rotCenter, const Flip flip/* = Flip()*/) {
    drawTex(tex, src.wh(), dst.wh(), rot, rotCenter, flip);
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
    return std::nullopt;
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
  bool Ren::isClipOn() noexcept {
    return SDL_RenderIsClipEnabled(unsafeRaw()) == SDL_TRUE;
  }

  std::optional<geom::Size> Ren::logicalSize() noexcept {
    geom::Size tmp{};
    SDL_RenderGetLogicalSize(unsafeRaw(), &tmp.w, &tmp.h);
    if (tmp.w != 0 || tmp.h != 0)
      return tmp;
    return std::nullopt;
  }
  void Ren::setLogicalSize(const geom::Size s) {
    throwOnErr(SDL_RenderSetLogicalSize(unsafeRaw(), s.w, s.h));
  }

  geom::Size Ren::getRealSize() {
    geom::Size res{};
    throwOnErr(SDL_GetRendererOutputSize(unsafeRaw(), &res.w, &res.h));
    return res;
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

  BlendMode Ren::getBlendMode() {
    SDL_BlendMode tmp = SDL_BLENDMODE_NONE;
    throwOnErr(SDL_GetRenderDrawBlendMode(unsafeRaw(), &tmp));
    return blendMode::unsafe::fromSDLEnum(tmp);
  }
  void Ren::setBlendMode(const BlendMode m) {
    throwOnErr(SDL_SetRenderDrawBlendMode(unsafeRaw(), blendMode::unsafe::toSDLEnum(m)));
  }


  RGBA Ren::color() {
    RGBA res{};
    throwOnErr(SDL_GetRenderDrawColor(unsafeRaw(), &res.r, &res.g, &res.b, &res.a));
    return res;
  }

  void Ren::setColor(const RGBA c) {
    throwOnErr(SDL_SetRenderDrawColor(unsafeRaw(), c.r, c.g, c.b, c.a));
  }



  TexRen::TexRen(Win& win, int i, ren::Flags flags) :
    Ren(win, i, flags.withToTexture()) {}
  TexRen::TexRen(Win& win,        ren::Flags flags) :
    Ren(win, flags.withToTexture()) {}
  TexRen::TexRen(Win& win, int i) :
    Ren(win, i, ren::Flags().withToTexture()) {}
  /*explicit*/ TexRen::TexRen(Win& win) :
    Ren(win, ren::Flags().withToTexture()) {}

  void TexRen::setTarget(RenTex& tex) {
    throwOnErr(SDL_SetRenderTarget(unsafeRaw(), tex.unsafeRaw()));
  }
  void TexRen::setDefaultTarget() {
    throwOnErr(SDL_SetRenderTarget(unsafeRaw(), nullptr));
  }

  SDL_Texture* TexRen::unsafeGetTarget() noexcept {
    return SDL_GetRenderTarget(unsafeRaw());
  }
}
