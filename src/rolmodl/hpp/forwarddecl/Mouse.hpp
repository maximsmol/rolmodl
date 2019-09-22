#pragma once

#include <cstdint>

#include <SDL.h>

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
