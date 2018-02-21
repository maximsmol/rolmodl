#include "hpp/Mouse.hpp"

#include "hpp/Base.hpp"

using rolmodl::detail::throwOnErr;
namespace rolmodl::mouse {
  namespace detail {
    /*explicit*/ BaseState::BaseState(const uint32_t b) noexcept :
      btnState_(b)
    {}
    const geom::Pos& BaseState::pos() const noexcept {
      return p_;
    }
    const BtnState& BaseState::btnState() const noexcept {
      return btnState_;
    }
  }

  State::State() noexcept :
    BaseState(SDL_GetMouseState(&p_.x, &p_.y))
  {}
  GlobalState::GlobalState() noexcept :
    BaseState(SDL_GetGlobalMouseState(&p_.x, &p_.y))
  {}

  void capture() {
    throwOnErr(SDL_CaptureMouse(SDL_TRUE));
  }
  void release() {
    throwOnErr(SDL_CaptureMouse(SDL_FALSE));
  }

  void move(const geom::Pos p) {
    throwOnErr(SDL_WarpMouseGlobal(p.x, p.y));
  }
}
