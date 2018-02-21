#pragma once

#include <cstdint>

#include <SDL.h>

namespace rolmodl {
  class Win;
}

#include "Base.hpp"
#include "Geom.hpp"
#include "PixelFmt.hpp"

namespace rolmodl {
  namespace win {
    // fixme: no support for some window flags i dont understand
    // https://wiki.libsdl.org/SDL_WindowFlags
    // todo: some flags are mutually exclusive
    struct Flags {
      public:
        constexpr Flags() noexcept :
          data_(0)
        {}

        // set
        constexpr Flags fullscreen() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_FULLSCREEN));
        }
        constexpr Flags fullscreenDesktop() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_FULLSCREEN_DESKTOP));
        }

        constexpr Flags minimized() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_MINIMIZED));
        }
        constexpr Flags maximized() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_MAXIMIZED));
        }

        constexpr Flags opengl() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_OPENGL));
        }
        constexpr Flags hidden() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_HIDDEN));
        }
        constexpr Flags borderless() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_BORDERLESS));
        }
        constexpr Flags resizable() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_RESIZABLE));
        }
        constexpr Flags inputGrabbed() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_INPUT_GRABBED));
        }
        constexpr Flags highDPI() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_ALLOW_HIGHDPI));
        }

        // unset
        constexpr Flags unFullscreen() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_FULLSCREEN));
        }
        constexpr Flags unFullscreenDesktop() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_FULLSCREEN_DESKTOP));
        }

        constexpr Flags unMinimized() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_MINIMIZED));
        }
        constexpr Flags unMaximized() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_MAXIMIZED));
        }

        constexpr Flags unOpengl() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_OPENGL));
        }
        constexpr Flags unHidden() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_HIDDEN));
        }
        constexpr Flags unBorderless() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_BORDERLESS));
        }
        constexpr Flags unResizable() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_RESIZABLE));
        }
        constexpr Flags unInputGrabbed() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_INPUT_GRABBED));
        }
        constexpr Flags unHighDPI() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_ALLOW_HIGHDPI));
        }

        constexpr uint32_t raw() const noexcept {
          return data_;
        }
      private:
        explicit constexpr Flags(const uint32_t data) noexcept :
          data_(data)
        {}

        uint32_t data_;
    };

    namespace unsafe {
      // Win fromId();
    }
  }

  class Win;
  struct BorderSizes {
    public:
      explicit BorderSizes(Win& w);

      int top() const noexcept;
      int l() const noexcept;
      int bot() const noexcept;
      int r() const noexcept;

    private:
      int top_, l_, bot_, r_;
  };

  class Win {
    public:
      Win(const char* title, const geom::Pos p, const geom::Size s, const win::Flags flags);
      Win(const char* title,                    const geom::Size s, const win::Flags flags);
      Win(const char* title, const geom::Pos p, const geom::Size s);
      Win(const char* title,                    const geom::Size s);
      ~Win() noexcept;

      Win(const Win& that) = delete;
      Win(Win&& that) noexcept;

      Win& operator=(const Win& that) = delete;
      Win& operator=(Win&& that) noexcept;

      friend void swap(Win& a, Win& b) noexcept;

      SDL_Window* unsafeRaw() noexcept;
      const SDL_Window* unsafeRaw() const noexcept;

      uint32_t unsafeId();
      pixelfmt::Id pixelFmt();
      void moveMouseIn(const geom::Pos p) noexcept;


      void hide() noexcept;
      void show() noexcept;
      void raise() noexcept;

      void maximize() noexcept;
      void minimize() noexcept;
      void restore() noexcept;

      unsigned int unsafeDisplayIndex();
      sys::Display display();

      float opacity();
      void setOpacity(const float v);

      geom::Pos pos() noexcept;
      void setPos(const geom::Pos p) noexcept;

      const char* title() noexcept;
      void setTitle(const char* str) noexcept;


      geom::Size size() noexcept;
      void setSize(const geom::Size s) noexcept;

      geom::Size maxSize() noexcept;
      void setMaxSize(const geom::Size s) noexcept;

      geom::Size minSize() noexcept;
      void setMinSize(const geom::Size s) noexcept;

    private:
      Win() noexcept;

      SDL_Window* h_;
  };
}
