#include "hpp/Mouse.hpp"

#include <cassert>
#include <utility>

#include "hpp/Base.hpp"

using rolmodl::detail::throwOnErr;
namespace rolmodl::mouse {
  namespace cursor {
    namespace unsafe {
      void useRaw(Cursor* c) noexcept { // todo: weird API
        using detail::active_cursor;

        if (active_cursor != nullptr)
          active_cursor->unused();
        SDL_SetCursor(c == nullptr ? SDL_GetDefaultCursor() : c->unsafeRaw());
      }
    }

    void useDefault() noexcept {
      unsafe::useRaw(nullptr);
    }

    namespace system {
      Cursor create(const Type t) {
        SDL_Cursor* c = SDL_CreateSystemCursor(type::unsafe::toSDLEnum(t));
        if (c == nullptr)
          throw sdlexception();
        return Cursor(c);
      }

      Cursor arrow() {
        return create(Type::arrow);
      }
      Cursor iBeam() {
        return create(Type::iBeam);
      }
      Cursor wait() {
        return create(Type::wait);
      }
      Cursor crosshair() {
        return create(Type::crosshair);
      }
      Cursor waitArrow() {
        return create(Type::waitArrow);
      }
      Cursor forbidden() {
        return create(Type::forbidden);
      }
      Cursor hand() {
        return create(Type::hand);
      }

      namespace resize {
        Cursor nw_se() {
        return create(Type::resize_nw_se);
        }
        Cursor ne_sw() {
        return create(Type::resize_ne_sw);
        }
        Cursor we() {
        return create(Type::resize_we);
        }
        Cursor ns() {
        return create(Type::resize_ns);
        }
        Cursor all() {
        return create(Type::resize_all);
        }
      }
    }
  }

  Cursor::Cursor() noexcept : Cursor(nullptr)
  {}
  /*explicit*/ Cursor::Cursor(SDL_Cursor* data) noexcept :
    data_(data),
    destructed_(false)
  {}
  Cursor::Cursor(Cursor&& that) noexcept :
    Cursor()
  {
    std::swap(*this, that);
  }
  Cursor::~Cursor() {
    if (cursor::detail::active_cursor == this) {
      destructed_ = true;
      return;
    }

    free();
  }

  Cursor& Cursor::operator=(Cursor&& that) noexcept {
    std::swap(*this, that);
    return *this;
  }

  void swap(Cursor& a, Cursor& b) noexcept {
    using std::swap;
    swap(a.data_, b.data_);
    swap(a.destructed_, b.destructed_);
  }

  void Cursor::use() noexcept {
    cursor::unsafe::useRaw(this);
  }
  void Cursor::unused() noexcept {
    if (!destructed_)
      return;
    free();
  }
  void Cursor::free() noexcept {
    if (data_ != nullptr)
      SDL_FreeCursor(data_);
  }

  SDL_Cursor* Cursor::unsafeRaw() noexcept {
    assert(data_ != nullptr);
    return data_;
  }

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
