#include "hpp/Mouse.hpp"

namespace rolmodl::mouse {
  State::State() noexcept :
    btnState_(SDL_GetMouseState(&p_.x, &p_.y))
  {}
  const geom::Pos& State::pos() const noexcept {
    return p_;
  }
  const BtnState& State::btnState() const noexcept {
    return btnState_;
  }
}
