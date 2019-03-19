#pragma once

#include <cstdint>

#include <SDL.h>

namespace rolmodl {
  class Win_Base;
  class Win;
  class Win_SW;
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

        // test
        constexpr bool isFullscreen() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_FULLSCREEN)) != 0;
        }
        constexpr bool isFullscreenDesktop() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_FULLSCREEN_DESKTOP)) != 0;
        }

        constexpr bool isMinimized() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_MINIMIZED)) != 0;
        }
        constexpr bool isMaximized() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_MAXIMIZED)) != 0;
        }

        constexpr bool isOpengl() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_OPENGL)) != 0;
        }
        constexpr bool isHidden() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_HIDDEN)) != 0;
        }
        constexpr bool isBorderless() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_BORDERLESS)) != 0;
        }
        constexpr bool isResizable() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_RESIZABLE)) != 0;
        }
        constexpr bool isInputGrabbed() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_INPUT_GRABBED)) != 0;
        }
        constexpr bool isHighDPI() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_ALLOW_HIGHDPI)) != 0;
        }

        // set
        constexpr Flags withFullscreen() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_FULLSCREEN));
        }
        constexpr Flags withFullscreenDesktop() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_FULLSCREEN_DESKTOP));
        }

        constexpr Flags withMinimized() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_MINIMIZED));
        }
        constexpr Flags withMaximized() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_MAXIMIZED));
        }

        constexpr Flags withOpengl() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_OPENGL));
        }
        constexpr Flags withHidden() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_HIDDEN));
        }
        constexpr Flags withBorderless() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_BORDERLESS));
        }
        constexpr Flags withResizable() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_RESIZABLE));
        }
        constexpr Flags withInputGrabbed() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_INPUT_GRABBED));
        }
        constexpr Flags withHighDPI() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_ALLOW_HIGHDPI));
        }

        // unset
        constexpr Flags withoutFullscreen() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_FULLSCREEN));
        }
        constexpr Flags withoutFullscreenDesktop() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_FULLSCREEN_DESKTOP));
        }

        constexpr Flags withoutMinimized() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_MINIMIZED));
        }
        constexpr Flags withoutMaximized() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_MAXIMIZED));
        }

        constexpr Flags withoutOpengl() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_OPENGL));
        }
        constexpr Flags withoutHidden() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_HIDDEN));
        }
        constexpr Flags withoutBorderless() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_BORDERLESS));
        }
        constexpr Flags withoutResizable() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_RESIZABLE));
        }
        constexpr Flags withoutInputGrabbed() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_INPUT_GRABBED));
        }
        constexpr Flags withoutHighDPI() const noexcept {
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
      // Win_Base fromId();
    }
  }

  class Win_Base;
  struct BorderSizes {
    public:
      explicit BorderSizes(Win_Base& w);

      int top() const noexcept;
      int l() const noexcept;
      int bot() const noexcept;
      int r() const noexcept;

    private:
      int top_, l_, bot_, r_;
  };

  class Win_Base {
    public:
      Win_Base(const char* title, const geom::Pos p, const geom::Size s, const win::Flags flags);
      Win_Base(const char* title,                    const geom::Size s, const win::Flags flags);
      Win_Base(const char* title, const geom::Pos p, const geom::Size s);
      Win_Base(const char* title,                    const geom::Size s);
      ~Win_Base() noexcept;

      Win_Base(const Win_Base& that) = delete;
      Win_Base(Win_Base&& that) noexcept;

      Win_Base& operator=(const Win_Base& that) = delete;
      Win_Base& operator=(Win_Base&& that) noexcept;

      friend void swap(Win_Base& a, Win_Base& b) noexcept;

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
      Win_Base() noexcept;

      SDL_Window* h_;
  };

  class Win : public Win_Base {
    public:
      using Win_Base::Win_Base;
  };
}
