#pragma once

#include <cstdint>

#include <SDL.h>

#include "Geom.hpp"

namespace rolmodl::mouse {
  class Cursor;
  namespace cursor {
    // namespace detail {
    //   static Cursor* active_cursor;
    // }
    namespace unsafe {
      void useRaw(Cursor* c) noexcept;
    }

    void useDefault() noexcept;
    namespace system {
      enum class Type;

      Cursor create(const Type t);

      Cursor arrow();
      Cursor iBeam();
      Cursor wait();
      Cursor crosshair();
      Cursor waitArrow();
      Cursor forbidden();
      Cursor hand();

      namespace resize {
        Cursor nw_se();
        Cursor ne_sw();
        Cursor we();
        Cursor ns();
        Cursor all();
      }
    }
  }

  enum class Btn;

  namespace btn::unsafe {
    constexpr uint32_t toSDLEnum(const Btn b) noexcept;
    constexpr Btn fromSDLEnum(const uint32_t b) noexcept;
  }

  struct BtnState;
  class State;
}

namespace rolmodl::mouse {
  class Cursor {
    public:
      Cursor(const Cursor& that) = delete;
      Cursor(Cursor&& that) noexcept;
      ~Cursor();

      Cursor& operator=(const Cursor& that) = delete;
      Cursor& operator=(Cursor&& that) noexcept;

      friend void swap(Cursor& a, Cursor& b) noexcept;

      void use() noexcept;
      SDL_Cursor* unsafeRaw() noexcept;

    private:
      Cursor() noexcept;
      explicit Cursor(SDL_Cursor* data) noexcept;

      friend Cursor cursor::system::create(const cursor::system::Type t);
      friend void cursor::unsafe::useRaw(Cursor* c) noexcept;

      void unused() noexcept;
      void free() noexcept;

      SDL_Cursor* data_;
      bool destructed_;
  };
  namespace cursor {
    namespace detail {
      static Cursor* active_cursor = nullptr; // todo: use optional?
    }
    namespace unsafe {
      void useRaw(Cursor* c) noexcept;
    }

    void useDefault() noexcept;
    namespace system {
      enum class Type {
        arrow, iBeam, wait, crosshair, waitArrow, forbidden, hand,
        resize_nw_se, resize_ne_sw, resize_we, resize_ns, resize_all
      };

      namespace type::unsafe {
        constexpr SDL_SystemCursor toSDLEnum(const Type t) noexcept {
          if (t == Type::arrow)
            return SDL_SYSTEM_CURSOR_ARROW;
          else if (t == Type::iBeam)
            return SDL_SYSTEM_CURSOR_IBEAM;
          else if (t == Type::wait)
            return SDL_SYSTEM_CURSOR_WAIT;
          else if (t == Type::crosshair)
            return SDL_SYSTEM_CURSOR_CROSSHAIR;
          else if (t == Type::waitArrow)
            return SDL_SYSTEM_CURSOR_WAITARROW;
          else if (t == Type::resize_nw_se)
            return SDL_SYSTEM_CURSOR_SIZENWSE;
          else if (t == Type::resize_ne_sw)
            return SDL_SYSTEM_CURSOR_SIZENESW;
          else if (t == Type::resize_we)
            return SDL_SYSTEM_CURSOR_SIZEWE;
          else if (t == Type::resize_ns)
            return SDL_SYSTEM_CURSOR_SIZENS;
          else if (t == Type::resize_all)
            return SDL_SYSTEM_CURSOR_SIZEALL;
          else if (t == Type::forbidden)
            return SDL_SYSTEM_CURSOR_NO;
          // else if (t == Type::hand)
            return SDL_SYSTEM_CURSOR_HAND;
        }

        constexpr Type fromSDLEnum(const SDL_SystemCursor t) noexcept {
          if (t == SDL_SYSTEM_CURSOR_ARROW)
            return Type::arrow;
          else if (t == SDL_SYSTEM_CURSOR_IBEAM)
            return Type::iBeam;
          else if (t == SDL_SYSTEM_CURSOR_WAIT)
            return Type::wait;
          else if (t == SDL_SYSTEM_CURSOR_CROSSHAIR)
            return Type::crosshair;
          else if (t == SDL_SYSTEM_CURSOR_WAITARROW)
            return Type::waitArrow;
          else if (t == SDL_SYSTEM_CURSOR_SIZENWSE)
            return Type::resize_nw_se;
          else if (t == SDL_SYSTEM_CURSOR_SIZENESW)
            return Type::resize_ne_sw;
          else if (t == SDL_SYSTEM_CURSOR_SIZEWE)
            return Type::resize_we;
          else if (t == SDL_SYSTEM_CURSOR_SIZENS)
            return Type::resize_ns;
          else if (t == SDL_SYSTEM_CURSOR_SIZEALL)
            return Type::resize_all;
          else if (t == SDL_SYSTEM_CURSOR_NO)
            return Type::forbidden;
          // else if (t == SDL_SYSTEM_CURSOR_HAND)
            return Type::hand;
        }
      }

      Cursor create(const Type t);

      Cursor arrow();
      Cursor iBeam();
      Cursor wait();
      Cursor crosshair();
      Cursor waitArrow();
      Cursor forbidden();
      Cursor hand();

      namespace resize {
        Cursor nw_se();
        Cursor ne_sw();
        Cursor we();
        Cursor ns();
        Cursor all();
      }
    }
  }

  enum class Btn {
    left, right, middle, x1, x2,
    l = left, r = right, m = middle
  };

  namespace btn::unsafe {
    constexpr uint32_t toSDLEnum(const Btn b) noexcept {
      if (b == Btn::left)
        return SDL_BUTTON_LEFT;
      else if (b == Btn::right)
        return SDL_BUTTON_RIGHT;
      else if (b == Btn::middle)
        return SDL_BUTTON_MIDDLE;
      else if (b == Btn::x1)
        return SDL_BUTTON_X1;
      // else if (b == Btn::x2)
        return SDL_BUTTON_X2;
    }

    constexpr Btn fromSDLEnum(const uint32_t b) noexcept {
      if (b == SDL_BUTTON_LEFT)
        return Btn::left;
      else if (b == SDL_BUTTON_RIGHT)
        return Btn::right;
      else if (b == SDL_BUTTON_MIDDLE)
        return Btn::middle;
      else if (b == SDL_BUTTON_X1)
        return Btn::x1;
      // else if (b == SDL_BUTTON_X2)
        return Btn::x2;
    }
  }

  struct BtnState {
    public:
      explicit constexpr BtnState(const uint32_t raw) noexcept :
        raw_(raw)
      {}

      constexpr bool left() const noexcept {
        return get(Btn::left);
      }
      constexpr bool right() const noexcept {
        return get(Btn::right);
      }
      constexpr bool middle() const noexcept {
        return get(Btn::middle);
      }
      constexpr bool x1() const noexcept {
        return get(Btn::x1);
      }
      constexpr bool x2() const noexcept {
        return get(Btn::x2);
      }

      constexpr bool l() const noexcept {return left();}
      constexpr bool r() const noexcept {return right();}
      constexpr bool m() const noexcept {return middle();}

      constexpr bool get(const Btn b) const noexcept {
        return raw_ & SDL_BUTTON(btn::unsafe::toSDLEnum(b));
      }

    private:
      uint32_t raw_;
  };

  namespace detail {
    class BaseState {
      public:
        const geom::Pos& pos() const noexcept;
        const BtnState& btnState() const noexcept;

      protected:
        explicit BaseState(const uint32_t b) noexcept;

        geom::Pos p_;
        BtnState btnState_;
    };
  }
  class State : public detail::BaseState {
    public:
      // fixme: SDL_PumpEvents required before calls
      State() noexcept;
  };
  class GlobalState : public detail::BaseState {
    public:
      // fixme: SDL_PumpEvents required before calls
      GlobalState() noexcept;
  };

  void capture();
  void release();

  void move(const geom::Pos p);
}
