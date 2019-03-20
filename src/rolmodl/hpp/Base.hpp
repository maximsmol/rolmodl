#pragma once

/// \file Base.hpp
/// Support for base SDL features. `rolmodl` utilities.
///
/// \sa https://wiki.libsdl.org/CategoryPower
/// \sa https://wiki.libsdl.org/CategoryPlatform
/// \sa https://wiki.libsdl.org/CategoryCPU
/// \sa https://wiki.libsdl.org/CategoryVideo

#include <exception>
#include <optional>

#include <SDL.h>

namespace rolmodl {
  namespace sys {
    namespace pwr {
      struct Status;
    }
    struct Display;
    struct DisplayMode;
  }

  struct RGB;
  struct RGBA;

  struct sdlexception;
}

#include "Geom.hpp"
#include "PixelFmt.hpp"

namespace rolmodl {
  namespace sys {
    /// \brief OS type enum.
    ///
    /// Values taken from `SDL.c`.
    ///
    /// \sa https://github.com/spurious/SDL-mirror/blob/master/src/SDL.c
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

    /// \brief Human-readable OS name. Use \link rolmodl::sys::id \endlink for OS identification purposes instead of this method.
    /// \sa https://wiki.libsdl.org/SDL_GetPlatform
    const char* name() noexcept;
    /// \brief Get id of the OS on which the code is running. May differ from whatever operating system the code was compiled on.
    Id id() noexcept;

    /// \brief Get the amount of system RAM in MB.
    /// \sa https://wiki.libsdl.org/SDL_GetSystemRAM
    unsigned int ram() noexcept;
    /// \brief Get the number of available logical CPU cores.
    /// \sa https://wiki.libsdl.org/SDL_GetCPUCount
    unsigned int logicalCores() noexcept;
    namespace cpu {
      /// \brief Get the CPU L1 cache line size in bytes.
      /// \sa https://wiki.libsdl.org/SDL_GetCPUCacheLineSize
      unsigned int l1Size() noexcept;

      // todo: put in a namespace/rename?
      /// \brief Query whether the CPU supports the 3DNow! extension.
      ///
      /// \sa https://wiki.libsdl.org/SDL_Has3DNow
      /// \sa https://en.wikipedia.org/wiki/3DNow!
      bool has3DNow() noexcept;
      /// \brief Query whether the CPU supports the AVX1 extension.
      ///
      /// \sa https://wiki.libsdl.org/SDL_HasAVX
      /// \sa https://en.wikipedia.org/wiki/Advanced_Vector_Extensions#AVX1
      bool hasAVX() noexcept;
      /// \brief Query whether the CPU supports the AVX2 extension.
      ///
      /// \sa https://wiki.libsdl.org/SDL_HasAVX2
      /// \sa https://en.wikipedia.org/wiki/Advanced_Vector_Extensions#AVX2
      bool hasAVX2() noexcept;
      /// \brief Query whether the CPU supports the AltiVec instruction set.
      ///
      /// \sa https://wiki.libsdl.org/SDL_HasAltiVec
      /// \sa https://en.wikipedia.org/wiki/AltiVec
      bool hasAltiVec() noexcept;
      /// \brief Query whether the CPU supports the MMX instruction set.
      ///
      /// \sa https://wiki.libsdl.org/SDL_HasMMX
      /// \sa https://en.wikipedia.org/wiki/MMX_(instruction_set)
      bool hasMMX() noexcept;
      /// \brief Query whether the CPU supports the `RDTSC` instruction.
      ///
      /// \sa https://wiki.libsdl.org/SDL_HasRDTSC
      /// \sa https://en.wikipedia.org/wiki/Time_Stamp_Counter
      bool hasRDTSC() noexcept;
      /// \brief Query whether the CPU supports the SSE1 extenstion.
      ///
      /// \sa https://wiki.libsdl.org/SDL_HasSSE
      /// \sa https://en.wikipedia.org/wiki/Streaming_SIMD_Extensions
      bool hasSSE() noexcept;
      /// \brief Query whether the CPU supports the SSE2 extenstion.
      ///
      /// \sa https://wiki.libsdl.org/SDL_HasSSE2
      /// \sa https://en.wikipedia.org/wiki/SSE2
      bool hasSSE2() noexcept;
      /// \brief Query whether the CPU supports the SSE3 extenstion.
      ///
      /// \sa https://wiki.libsdl.org/SDL_HasSSE3
      /// \sa https://en.wikipedia.org/wiki/SSE3
      bool hasSSE3() noexcept;
      /// \brief Query whether the CPU supports the SSE4.1 extenstion.
      ///
      /// \sa https://wiki.libsdl.org/SDL_HasSSE41
      /// \sa https://en.wikipedia.org/wiki/SSE4#SSE4.1
      bool hasSSE41() noexcept;
      /// \brief Query whether the CPU supports the SSE4.2 (Streaming SIMD Extensions) extenstion.
      ///
      /// \sa https://wiki.libsdl.org/SDL_HasSSE42
      /// \sa https://en.wikipedia.org/wiki/SSE4#SSE4.2
      bool hasSSE42() noexcept;
    }

    namespace pwr {
      /// \brief Indicates battery status a.k.a. the system power state.
      /// \sa https://wiki.libsdl.org/SDL_PowerState
      enum class State {
        onBattery,
        noBattery,
        charging,
        charged,
        unknown
      };

      /// \brief System power status snapshot.
      ///
      /// \todo: explain why a snapshot is used instead of separate functions
      ///
      /// \sa https://wiki.libsdl.org/SDL_GetPowerInfo
      struct Status {
        public:
          Status() noexcept;

          /// \brief Get the recorded battery status/power state.
          State state() const noexcept;
          /// \brief Get the recorded estimated remaining battery charge life time.
          std::optional<int> time() const noexcept;
          /// \brief Get the recorded remaining battery charge percentage.
          std::optional<int> percentage() const noexcept;

        private:
          State state_;
          std::optional<int> time_, percentage_;
      };
      /// \brief Get a system power status snapshot.
      Status status() noexcept;
    }

    namespace screensaver {
      /// \brief Enable system screensaver.
      /// \sa https://wiki.libsdl.org/SDL_EnableScreenSaver
      void enable() noexcept;
      /// \brief Disable system screensaver.
      /// \sa https://wiki.libsdl.org/SDL_DisableScreenSaver
      void disable() noexcept;
      /// \brief Query whether the system screensaver is enabled.
      /// \sa https://wiki.libsdl.org/SDL_IsScreenSaverEnabled
      bool enabled() noexcept;
    }

    namespace driver {
      /// \brief Query the amount of available video drivers.
      /// \sa https://wiki.libsdl.org/SDL_GetNumVideoDrivers
      unsigned int count();
      /// \brief Get the name of the `i`th video driver.
      ///
      /// \note The video drivers are presented in the order in which they are normally checked during initialization.
      ///
      /// \sa https://wiki.libsdl.org/SDL_GetVideoDriver
      const char* name(const unsigned int i);
      /// \brief Query the current video driver name.
      /// \sa https://wiki.libsdl.org/SDL_GetCurrentVideoDriver
      std::optional<const char*> current() noexcept;
    }

    namespace display::unsafe {
      /// \brief Get display information for the `i`th display. Unsafe because there is no bounds checking on the index.
      ///
      /// Equivalent to \link rolmodl::sys::Display::Display \endlink.
      Display byIndex(unsigned int i);

      /// \brief Get the number of displays. Unsafe because the return value is unreliable and may lead to out-of-bounds errors if its cached and used for loop iteration.
      /// \sa https://wiki.libsdl.org/SDL_GetNumVideoDisplays
      unsigned int count();
      /// \brief Get the name of the `i`th display. Sample output: `XF250Q` for my Acer XF250Q. Unsafe because there is no bounds checking on the index.
      ///
      /// In addition to being marked unsafe, the use of this function is further discouraged.
      /// Display information should only be acquired all together as a snapshot as done by \link rolmodl::sys::Display \endlink.
      ///
      /// \sa https://wiki.libsdl.org/SDL_GetDisplayName
      const char* name(const unsigned int i);

      /// \brief Get the resolution of the `i`th display. Unsafe because there is no bounds checking on the index.
      ///
      /// In addition to being marked unsafe, the use of this function is further discouraged.
      /// Display information should only be acquired all together as a snapshot as done by \link rolmodl::sys::Display \endlink.
      ///
      /// \sa https://wiki.libsdl.org/SDL_GetDisplayBounds
      geom::RectWH bounds(const unsigned int i);
      /// \brief Get the bounds of the usable space of the `i`th display. Excludes system decorations such as the menu bar and the dock. Unsafe because there is no bounds checking on the index.
      ///
      /// In addition to being marked unsafe, the use of this function is further discouraged.
      /// Display information should only be acquired all together as a snapshot as done by \link rolmodl::sys::Display \endlink.
      ///
      /// \sa https://wiki.libsdl.org/SDL_GetDisplayUsableBounds
      geom::RectWH usableBounds(const unsigned int i);
    }

    /// \brief Display information snapshot.
    ///
    /// \todo: explain why a snapshot is used instead of separate functions
    ///
    /// \sa rolmodl::sys:display::unsafe::name
    /// \sa rolmodl::sys:display::unsafe::bounds
    /// \sa rolmodl::sys:display::unsafe::usableBounds
    struct Display {
      public:
        /// \brief Get recorded display name.
        ///
        /// \sa rolmodl::sys:display::unsafe::name
        const char* name() const noexcept;

        /// \brief Get recorded display resolution.
        /// \sa rolmodl::sys:display::unsafe::bounds
        geom::RectWH bounds() const noexcept;
        /// \brief Get recorded display usable bounds. Excludes system decorations such as the menu bar and the dock.
        /// \sa rolmodl::sys:display::unsafe::usableBounds
        geom::RectWH usableBounds() const noexcept;

        /// \brief Get recorded display diagonal DPI (dots per inch).
        /// \sa https://wiki.libsdl.org/SDL_GetDisplayDPI
        float ddpi() const noexcept;
        /// \brief Get recorded display horizontal DPI (dots per inch).
        /// \sa https://wiki.libsdl.org/SDL_GetDisplayDPI
        float hdpi() const noexcept;
        /// \brief Get recorded display vertical DPI (dots per inch).
        /// \sa https://wiki.libsdl.org/SDL_GetDisplayDPI
        float vdpi() const noexcept;

      private:
        explicit Display(const unsigned int i);

        const char* name_;
        geom::RectWH bounds_, usableBounds_;

        float ddpi_, hdpi_, vdpi_;

        /// \link rolmodl::sys::display::unsafe::byIndex \endlink
        friend Display display::unsafe::byIndex(unsigned int i);
    };

    namespace display::mode::unsafe {
      /// \brief Get the amount of display modes available for the `n`th display.
      ///
      /// Unsafe because there is no bounds checking on the index and because the return value is unreliable and may lead to out-of-bounds errors if its cached and used for loop iteration.
      ///
      /// \sa https://wiki.libsdl.org/SDL_GetNumDisplayModes
      unsigned int countForDisplayN(unsigned int n);
      /// \brief Get the display mode used by the system desktop for the `n`th display. Unsafe because there is no bounds checking on the index.
      /// \sa https://wiki.libsdl.org/SDL_GetDesktopDisplayMode
      DisplayMode desktopForDisplayN(unsigned int n);
      /// \brief Get the current display mode for the `n`th display. Unsafe because there is no bounds checking on the index.
      /// \sa https://wiki.libsdl.org/SDL_GetCurrentDisplayMode
      DisplayMode currentForDisplayN(unsigned int n);
      /// \brief Get the `i`th display mode for the `n`th display. Unsafe because there is no bounds checking on either index.
      /// \sa https://wiki.libsdl.org/SDL_GetDisplayMode
      DisplayMode forDisplayNByIndexI(unsigned int n, unsigned int i);

      /// \brief Get the display mode closest to the one specified for the `n`th display. Unsafe because there is no bounds checking on the index and because it will throw if no display is found.
      ///
      /// Pixel format and refresh rate default to the system desktop's display mode's if they are not set. \todo: what does that equal in terms of pixelfmt::Id, the doc said it needs to be 0
      ///
      /// Parameter priority for what is considered "closest": size, pixel format, refresh rate.
      /// If all available display modes are too small, returns nullptr
      ///
      /// \sa https://wiki.libsdl.org/SDL_GetClosestDisplayMode
      DisplayMode closestForDisplayN(unsigned int n, const DisplayMode ideal);
    }

    /// \brief Display mode parameter plain old struct.
    /// \sa https://wiki.libsdl.org/SDL_DisplayMode
    struct DisplayMode {
      public:
        /// \brief Display mode format.
        pixelfmt::Id fmt;
        /// \brief Display mode size.
        geom::Size size;
        /// \brief Display mode refresh rate, might be unspecified.
        std::optional<unsigned int> refreshRate = std::nullopt;
        /// \brief Unspecified driver-specific data.
        void* unsafeDriverData = nullptr;
      private:
    };
  }

  /// \brief RGB color type. No alpha component.
  ///
  /// The default color is pure black: `RGB(0, 0, 0)` = `#000000`.
  ///
  /// \sa https://wiki.libsdl.org/SDL_Color
  struct RGB {
    public:
      uint8_t r, g, b;

      /// \brief Initialize as the default pure black color: `RGB(0, 0, 0)` = `#000000`.
      constexpr RGB() :
        r(0), g(0), b(0)
      {}
      /// \brief Initialize with the given red, green, and blue values
      constexpr RGB(const uint8_t arg_r, const uint8_t arg_g, const uint8_t arg_b) :
        r(arg_r), g(arg_g), b(arg_b)
      {}

    private:
  };

  /// \brief RGBA color type. Has an alpha component.
  ///
  /// The default color is pure opaque black: `RGBA(0, 0, 0, 255)` = `#000000ff`.
  /// The default alpha value is `255`.
  ///
  /// \sa https://wiki.libsdl.org/SDL_Color
  struct RGBA : public RGB {
    public:
      uint8_t a;

      /// \brief Initialize as the default pure opaque black color: `RGBA(0, 0, 0, 255)` = `#000000ff`.
      constexpr RGBA() :
        RGB(), a(255)
      {}
      /// \brief Initialize with the given red, green, blue values and the default alpha of `255`.
      constexpr RGBA(const uint8_t arg_r, const uint8_t arg_g, const uint8_t arg_b) :
        RGB(arg_r, arg_g, arg_b), a(255)
      {}
      /// \brief Initialize with the given red, green, blue, and alpha values.
      constexpr RGBA(const uint8_t arg_r, const uint8_t arg_g, const uint8_t arg_b, const uint8_t arg_a) :
        RGB(arg_r, arg_g, arg_b), a(arg_a)
      {}

    private:
  };

  /// \brief Exception type containing an error code and the last SDL error at the moment of creation.
  /// \sa https://wiki.libsdl.org/SDL_GetError
  struct sdlexception : public std::exception {
    public:
      /// \brief Initialize with the given error code and the last SDL error message.
      /// \sa https://wiki.libsdl.org/SDL_GetError
      explicit sdlexception(const int code) noexcept;
      /// \brief Initialize with the error code arbitrarily set to `0` if one isn't available and the last SDL error message.
      /// \sa https://wiki.libsdl.org/SDL_GetError
      sdlexception() noexcept;
      /// \brief Get the recorded error code.
      int code() const noexcept;
      /// \brief Get the recorded error message.
      /// \sa https://wiki.libsdl.org/SDL_GetError
      const char* what() const noexcept override;

    private:
      int code_;
      const char* msg_;
  };

  namespace detail {
    /// \brief Throw a \link rolmodl::sdlexception \endlink if `code < 0`.
    int throwOnErr(const int code);
  }
}
