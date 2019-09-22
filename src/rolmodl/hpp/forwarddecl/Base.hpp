#pragma once

#include <exception>
#include <optional>

#include <SDL.h>

#include "Geom.hpp"

namespace rolmodl {
  namespace sys {
    enum class Id;

    const char* name() noexcept;
    Id id() noexcept;

    unsigned int ram() noexcept;
    unsigned int logicalCores() noexcept;

    namespace cpu {
      unsigned int l1Size() noexcept;

      bool has3DNow() noexcept;
      bool hasAVX() noexcept;
      bool hasAVX2() noexcept;
      bool hasAltiVec() noexcept;
      bool hasMMX() noexcept;
      bool hasRDTSC() noexcept;
      bool hasSSE() noexcept;
      bool hasSSE2() noexcept;
      bool hasSSE3() noexcept;
      bool hasSSE41() noexcept;
      bool hasSSE42() noexcept;
    }

    namespace pwr {
      enum class State;
      struct Status;
      Status status() noexcept;
    }
    namespace clipboard {
      bool hasText() noexcept;
      // SDLString getText();
      void setText(const char* x);
    }
    namespace screensaver {
      void enable() noexcept;
      void disable() noexcept;
      bool enabled() noexcept;
    }
    namespace driver {
      unsigned int count();
      const char* name(const unsigned int i);
      std::optional<const char*> current() noexcept;
    }

    namespace display::unsafe {
      // Display byIndex(unsigned int i);
      unsigned int count();
      const char* name(const unsigned int i);
      geom::RectWH bounds(const unsigned int i);
      geom::RectWH usableBounds(const unsigned int i);
    }
    struct Display;

    namespace display::mode::unsafe {
      unsigned int countForDisplayN(unsigned int n);

      // DisplayMode desktopForDisplayN(unsigned int n);
      // DisplayMode currentForDisplayN(unsigned int n);
      // DisplayMode forDisplayNByIndexI(unsigned int n, unsigned int i);
      // DisplayMode closestForDisplayN(unsigned int n, const DisplayMode ideal);
    }
    struct DisplayMode;
  }

  struct RGB;
  struct RGBA;

  struct sdlexception;
  namespace detail {
    int throwOnErr(const int code);
  }
  class SDLString;
}
