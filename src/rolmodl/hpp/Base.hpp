#pragma once

#include <exception>
#include <optional>

#include <SDL.h>

#include "Geom.hpp"

namespace rolmodl {
  namespace sys {
    // data from SDL.c
    enum class Id {
      AIX,
      android,
      BSDI,
      dreamcast,
      emscripten,
      freeBSD,
      haiku,
      HPUX,
      irix,
      linux,
      MiNT,
      macOSClassic,
      macOSX,
      NaCl,
      netBSD,
      openBSD,
      OS2,
      OSF,
      QNXNeutrino,
      RISCOS,
      solaris,
      windows,
      winRT,
      tvOS,
      iOS,
      PSP,
      unknown
    };

    const char* name() noexcept;
    Id id() noexcept;

    unsigned int ram() noexcept;
    unsigned int logicalCores() noexcept;
    namespace cpu {
      unsigned int l1Size() noexcept;

      // todo: put in a namespace/rename?
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
      enum class State {
        onBattery,
        noBattery,
        charging,
        charged,
        unknown
      };

      struct Status {
        public:
          Status() noexcept;

          State state() const noexcept;
          std::optional<int> time() const noexcept;
          std::optional<int> percentage() const noexcept;

        private:
          State state_;
          std::optional<int> time_, percentage_;
      };
      Status status() noexcept;
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

    struct Display;
    namespace display::unsafe {
      Display byIndex(unsigned int i);

      unsigned int count();
      const char* name(const unsigned int i);

      geom::RectWH bounds(const unsigned int i);
      geom::RectWH usableBounds(const unsigned int i);
    }

    struct Display {
      public:
        const char* name() const noexcept;

        geom::RectWH bounds() const noexcept;
        geom::RectWH usableBounds() const noexcept;

        float ddpi() const noexcept;
        float hdpi() const noexcept;
        float vdpi() const noexcept;

      private:
        explicit Display(const unsigned int i);

        const char* name_;
        geom::RectWH bounds_, usableBounds_;

        float ddpi_, hdpi_, vdpi_;

        friend Display display::unsafe::byIndex(unsigned int i);
    };
  }

  struct RGB {
    public:
      uint8_t r, g, b;

      constexpr RGB() :
        r(0), g(0), b(0)
      {}
      constexpr RGB(const uint8_t arg_r, const uint8_t arg_g, const uint8_t arg_b) :
        r(arg_r), g(arg_g), b(arg_b)
      {}

    private:
  };

  struct RGBA : public RGB {
    public:
      uint8_t a;

      constexpr RGBA() :
        RGB(), a(255)
      {}
      constexpr RGBA(const uint8_t arg_r, const uint8_t arg_g, const uint8_t arg_b) :
        RGB(arg_r, arg_g, arg_b), a(255)
      {}
      constexpr RGBA(const uint8_t arg_r, const uint8_t arg_g, const uint8_t arg_b, const uint8_t arg_a) :
        RGB(arg_r, arg_g, arg_b), a(arg_a)
      {}

    private:
  };

  struct sdlexception : public std::exception {
    public:
      explicit sdlexception(const int code) noexcept;
      sdlexception() noexcept;
      int code() const noexcept;
      const char* what() const noexcept override;

    private:
      int code_;
      const char* msg_;
  };

  namespace detail {
    int throwOnErr(const int code);
  }
}
