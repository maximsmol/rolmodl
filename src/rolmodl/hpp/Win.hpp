#pragma once

/// \file Win.hpp
/// Support for SDL windows.
///
/// \sa https://wiki.libsdl.org/CategoryVideo

#include "forwarddecl/Win.hpp"

#include "forwarddecl/Geom.hpp"
#include "forwarddecl/PixelFmt.hpp"
#include "forwarddecl/Base.hpp"

namespace rolmodl {
  namespace win {
    /// \brief Window configuration (flags) container.
    ///
    /// \todo
    /// fixme: no support for some window flags i dont understand
    /// todo: some flags are mutually exclusive
    ///
    /// \sa https://wiki.libsdl.org/SDL_WindowFlags
    struct Flags {
      public:
        /// \brief Create a configuration corresponding to a default window.
        constexpr Flags() noexcept :
          data_(0)
        {}

        // test
        /// \brief Test whether this configuration corresponds to a fullscreen window. The SDL flag equivalent is `SDL_WINDOW_FULLSCREEN`.
        constexpr bool isFullscreen() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_FULLSCREEN)) != 0;
        }
        /// \brief Test whether this configuration corresponds to a desktop-resolution fullscreen window. The SDL flag equivalent is `SDL_WINDOW_FULLSCREEN_DESKTOP`.
        constexpr bool isFullscreenDesktop() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_FULLSCREEN_DESKTOP)) != 0;
        }

        /// \brief Test whether this configuration corresponds to a minimized window. The SDL flag equivalent is `SDL_WINDOW_MINIMIZED`.
        constexpr bool isMinimized() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_MINIMIZED)) != 0;
        }
        /// \brief Test whether this configuration corresponds to a maximized window. The SDL flag equivalent is `SDL_WINDOW_MAXIMIZED`.
        constexpr bool isMaximized() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_MAXIMIZED)) != 0;
        }

        /// \brief Test whether this configuration corresponds to a window usable with OpenGL. The SDL flag equivalent is `SDL_WINDOW_OPENGL`.
        /// \todo add a "see also" on something about OpenGL.
        constexpr bool isOpengl() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_OPENGL)) != 0;
        }
        /// \brief Test whether this configuration corresponds to a hidden window. The SDL flag equivalent is `SDL_WINDOW_HIDDEN`.
        constexpr bool isHidden() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_HIDDEN)) != 0;
        }
        /// \brief Test whether this configuration corresponds to a borderless window. The SDL flag equivalent is `SDL_WINDOW_BORDERLESS`.
        constexpr bool isBorderless() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_BORDERLESS)) != 0;
        }
        /// \brief Test whether this configuration corresponds to a resizable window. The SDL flag equivalent is `SDL_WINDOW_RESIZABLE`.
        constexpr bool isResizable() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_RESIZABLE)) != 0;
        }
        /// \brief Test whether this configuration corresponds to a window that has grabbed input focus. The SDL flag equivalent is `SDL_WINDOW_INPUT_GRABBED`.
        constexpr bool isInputGrabbed() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_INPUT_GRABBED)) != 0;
        }
        /// \brief Test whether this configuration corresponds to a window with preference for high DPI display. The SDL flag equivalent is `SDL_WINDOW_ALLOW_HIGHDPI`.
        constexpr bool isHighDPI() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_WINDOW_ALLOW_HIGHDPI)) != 0;
        }

        // set
        /// \brief Create a version of this configuration that corresponds to a fullscreen window. The SDL flag equivalent is `SDL_WINDOW_FULLSCREEN`.
        constexpr Flags withFullscreen() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_FULLSCREEN));
        }
        /// \brief Create a version of this configuration that corresponds to a desktop-resolution fullscreen window. The SDL flag equivalent is `SDL_WINDOW_FULLSCREEN_DESKTOP`.
        constexpr Flags withFullscreenDesktop() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_FULLSCREEN_DESKTOP));
        }

        /// \brief Create a version of this configuration that corresponds to a minimized window. The SDL flag equivalent is `SDL_WINDOW_MINIMIZED`.
        constexpr Flags withMinimized() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_MINIMIZED));
        }
        /// \brief Create a version of this configuration that corresponds to a maximized window. The SDL flag equivalent is `SDL_WINDOW_MAXIMIZED`.
        constexpr Flags withMaximized() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_MAXIMIZED));
        }

        /// \brief Create a version of this configuration that corresponds to a window usable with OpenGL. The SDL flag equivalent is `SDL_WINDOW_OPENGL`.
        /// \todo add a "see also" on something about OpenGL.
        constexpr Flags withOpengl() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_OPENGL));
        }
        /// \brief Create a version of this configuration that corresponds to a hidden window. The SDL flag equivalent is `SDL_WINDOW_HIDDEN`.
        constexpr Flags withHidden() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_HIDDEN));
        }
        /// \brief Create a version of this configuration that corresponds to a borderless window. The SDL flag equivalent is `SDL_WINDOW_BORDERLESS`.
        constexpr Flags withBorderless() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_BORDERLESS));
        }
        /// \brief Create a version of this configuration that corresponds to a resizable window. The SDL flag equivalent is `SDL_WINDOW_RESIZABLE`.
        constexpr Flags withResizable() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_RESIZABLE));
        }
        /// \brief Create a version of this configuration that corresponds to a window that has grabbed input focus. The SDL flag equivalent is `SDL_WINDOW_INPUT_GRABBED`.
        constexpr Flags withInputGrabbed() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_INPUT_GRABBED));
        }
        /// \brief Create a version of this configuration that corresponds to a window with preference for high DPI display. The SDL flag equivalent is `SDL_WINDOW_ALLOW_HIGHDPI`.
        constexpr Flags withHighDPI() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_WINDOW_ALLOW_HIGHDPI));
        }

        // unset
        /// \brief Create a version of this configuration that corresponds to a window that is not fullscreen. The SDL flag equivalent is `~SDL_WINDOW_FULLSCREEN`.
        constexpr Flags withoutFullscreen() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_FULLSCREEN));
        }
        /// \brief Create a version of this configuration that corresponds to a window that is not display-resolution fullscreen. The SDL flag equivalent is `~SDL_WINDOW_FULLSCREEN_DESKTOP`.
        constexpr Flags withoutFullscreenDesktop() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_FULLSCREEN_DESKTOP));
        }

        /// \brief Create a version of this configuration that corresponds to a window that is not minimized. The SDL flag equivalent is `~SDL_WINDOW_MINIMIZED`.
        constexpr Flags withoutMinimized() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_MINIMIZED));
        }
        /// \brief Create a version of this configuration that corresponds to a window that is not maximized. The SDL flag equivalent is `~SDL_WINDOW_MAXIMIZED`.
        constexpr Flags withoutMaximized() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_MAXIMIZED));
        }

        /// \brief Create a version of this configuration that corresponds to a window that is not usable with OpenGL. The SDL flag equivalent is `~SDL_WINDOW_OPENGL`.
        constexpr Flags withoutOpengl() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_OPENGL));
        }
        /// \brief Create a version of this configuration that corresponds to a window that is not hidden. The SDL flag equivalent is `~SDL_WINDOW_HIDDEN`.
        constexpr Flags withoutHidden() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_HIDDEN));
        }
        /// \brief Create a version of this configuration that corresponds to a window that is not borderless. The SDL flag equivalent is `~SDL_WINDOW_BORDERLESS`.
        constexpr Flags withoutBorderless() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_BORDERLESS));
        }
        /// \brief Create a version of this configuration that corresponds to a window that is not resizable. The SDL flag equivalent is `~SDL_WINDOW_RESIZABLE`.
        constexpr Flags withoutResizable() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_RESIZABLE));
        }
        /// \brief Create a version of this configuration that corresponds to a window that has not grabbed input focus. The SDL flag equivalent is `~SDL_WINDOW_INPUT_GRABBED`.
        constexpr Flags withoutInputGrabbed() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_INPUT_GRABBED));
        }
        /// \brief Create a version of this configuration that corresponds to a window with no preference for high DPI display. The SDL flag equivalent is `~SDL_WINDOW_ALLOW_HIGHDPI`.
        constexpr Flags withoutHighDPI() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_WINDOW_ALLOW_HIGHDPI));
        }

        /// \brief Return the underlying bitfield representation of this configuration.
        constexpr uint32_t raw() const noexcept {
          return data_;
        }
        // fixme: missing unsafeFromRaw
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

  /// \brief Window decoration (border) sizes snapshot.
  /// \sa https://wiki.libsdl.org/SDL_GetWindowBordersSize
  struct BorderSizes {
    public:
      /// \brief Initialize from the current decoration (border) sizes for window `w`.
      explicit BorderSizes(Win_Base& w);

      /// \brief Get the recorded top border size.
      int top() const noexcept;
      /// \brief Get the recorded left border size.
      int l() const noexcept;
      /// \brief Get the recorded bottom border size.
      int bot() const noexcept;
      /// \brief Get the recorded right border size.
      int r() const noexcept;

    private:
      int top_, l_, bot_, r_;
  };

  /// \brief Common window class.
  /// Use \link rolmodl::Win \endlink for use with accelerated rendering (\link rolmodl::Ren \endlink)
  /// and  \link rolmodl::Win_SW \endlink for use with software rendering (\link rolmodl::SWTex \endlink).
  ///
  /// Wrapper around `SDL_Window`.
  ///
  /// \sa https://wiki.libsdl.org/SDL_CreateWindow
  class Win_Base {
    public:
      /// \brief Initialize a window with title `title`, at the screen position `p`, of size `s`, with configuration `flags`.
      /// \sa https://wiki.libsdl.org/SDL_CreateWindow
      Win_Base(const char* title, const geom::Pos p, const geom::Size s, const win::Flags flags);
      /// \brief Initialize a window with title `title`, at the center of the screen, of size `s`, with configuration `flags`.
      /// \sa https://wiki.libsdl.org/SDL_CreateWindow
      Win_Base(const char* title,                    const geom::Size s, const win::Flags flags);
      /// \brief Initialize a window with title `title`, at the screen position `p`, of size `s`, with the default configuration.
      /// \sa https://wiki.libsdl.org/SDL_CreateWindow
      Win_Base(const char* title, const geom::Pos p, const geom::Size s);
      /// \brief Initialize a window with title `title`, at the center of the screen, of size `s`, with the default configuration.
      /// \sa https://wiki.libsdl.org/SDL_CreateWindow
      Win_Base(const char* title,                    const geom::Size s);
      /// \brief Free the underlying `SDL_Window`.
      /// \sa https://wiki.libsdl.org/SDL_DestroyWindow
      ~Win_Base() noexcept;

      /// \brief Copying rolmodl windows is not allowed because their lifetime is tied to the underlying `SDL_Window`'s. Use references or some other pointer type instead.
      Win_Base(const Win_Base& that) = delete;
      /// \brief Take over the underlying `SDL_Window` of `that`.
      Win_Base(Win_Base&& that) noexcept;

      /// \brief Copying rolmodl windows is not allowed because their lifetime is tied to the underlying `SDL_Window`'s. Use references or some other pointer type instead.
      Win_Base& operator=(const Win_Base& that) = delete;
      /// \brief Take over the underlying `SDL_Window` of `that`.
      Win_Base& operator=(Win_Base&& that) noexcept;

      /// \brief Exchange the underlying `SDL_Window` with `that`.
      friend void swap(Win_Base& a, Win_Base& b) noexcept;

      /// \brief Get the underlying `SDL_Window*`. Unsafe because this value might be nullptr and using it with some SDL functions can lead to rolmodl invariant violation.
      SDL_Window* unsafeRaw() noexcept;
      /// \brief Get the underlying `SDL_Window*` in a `const` context. Unsafe because this value might be nullptr and using it with some SDL functions can lead to rolmodl invariant violation.
      const SDL_Window* unsafeRaw() const noexcept;


      /// \brief Hide the window.
      /// \sa https://wiki.libsdl.org/SDL_HideWindow
      void hide() noexcept;
      /// \brief Show the window.
      /// \sa https://wiki.libsdl.org/SDL_ShowWindow
      void show() noexcept;
      /// \brief Place this window above all other windows and grab input focus.
      /// \sa https://wiki.libsdl.org/SDL_RaiseWindow
      void raise() noexcept;

      /// \brief Maximize the window setting its size to the largest that will fit on screen.
      /// \sa https://wiki.libsdl.org/SDL_MaximizeWindow
      void maximize() noexcept;
      /// \brief Minimize the window hiding it in the start menu or dock or an equivalent.
      /// \sa https://wiki.libsdl.org/SDL_MaximizeWindow
      void minimize() noexcept;
      /// \brief Restore the window to the size and screen position it had before being maximized or minimized.
      /// \sa https://wiki.libsdl.org/SDL_RestoreWindow
      void restore() noexcept;

      /// \brief Get the index of the display that contains this window's center. Unsafe because storing this index can lead to an out-of-bounds error if the display is later disconnected.
      /// \sa https://wiki.libsdl.org/SDL_GetWindowDisplayIndex
      unsigned int unsafeDisplayIndex();
      /// \brief Get the display information snapshot for the display that contains this window's center.
      /// \sa https://wiki.libsdl.org/SDL_GetWindowDisplayIndex
      sys::Display display();

      /// \brief Get the window opacity.
      /// \sa https://wiki.libsdl.org/SDL_GetWindowOpacity
      float opacity();
      /// \brief Set the window opacity.
      /// \sa https://wiki.libsdl.org/SDL_SetWindowOpacity
      void setOpacity(const float v);

      /// \brief Get the window screen position.
      /// \sa https://wiki.libsdl.org/SDL_GetWindowPosition
      geom::Pos pos() noexcept;
      /// \brief Set the window screen position.
      /// \sa https://wiki.libsdl.org/SDL_SetWindowPosition
      void setPos(const geom::Pos p) noexcept;

      /// \brief Get the window title in UTF-8. Will be an empty string if there is no title
      /// \sa https://wiki.libsdl.org/SDL_GetWindowTitle
      const char* title() noexcept;
      /// \brief Set the window title. `str` is in UTF-8.
      /// \sa https://wiki.libsdl.org/SDL_SetWindowTitle
      void setTitle(const char* str) noexcept;


      /// \brief Get the current window size.
      /// \sa https://wiki.libsdl.org/SDL_GetWindowSize
      geom::Size size() noexcept;
      /// \brief Set the window size.
      /// \sa https://wiki.libsdl.org/SDL_SetWindowSize
      void setSize(const geom::Size s) noexcept;

      /// \brief Get the current maximum window size.
      /// \sa https://wiki.libsdl.org/SDL_GetWindowMaximumSize
      geom::Size maxSize() noexcept;
      /// \brief Set the maximum window size.
      /// \sa https://wiki.libsdl.org/SDL_SetWindowMaximumSize
      void setMaxSize(const geom::Size s) noexcept;

      /// \brief Get the current minimum window size.
      /// \sa https://wiki.libsdl.org/SDL_GetWindowMinimumSize
      geom::Size minSize() noexcept;
      /// \brief Set the minimum window size.
      /// \sa https://wiki.libsdl.org/SDL_SetWindowMinimumSize
      void setMinSize(const geom::Size s) noexcept;


      /// \brief Get the underlying SDL window id. Unsafe because this can be used to make a pointer to the underlying window that escapes the window's lifetime.
      /// \sa https://wiki.libsdl.org/SDL_GetWindowID
      uint32_t unsafeId();
      /// \brief Get the window pixel format.
      /// \sa https://wiki.libsdl.org/SDL_GetWindowPixelFormat
      pixelfmt::Id pixelFmt();
      /// \brief Move the mouse pointer to a position relative to this window.
      /// \sa https://wiki.libsdl.org/SDL_WarpMouseInWindow
      void moveMouseIn(const geom::Pos p) noexcept;

    private:
      Win_Base() noexcept;

      SDL_Window* h_;
  };

  /// \brief Window class for use with accelerated rendering (\link rolmodl::Ren \endlink).
  class Win : public Win_Base {
    public:
      using Win_Base::Win_Base;
  };
}
