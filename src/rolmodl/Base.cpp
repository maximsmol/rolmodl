#include "hpp/Base.hpp"

#include <cstring>
#include <stdexcept>
#include <utility>

namespace rolmodl {
  namespace sys {
    const char* name() noexcept {
      return SDL_GetPlatform();
    }

    // data from SDL.c
    Id id() noexcept {
      const char* str = name();

      if (strcmp(str, "AIX") == 0)
        return Id::AIX;
      if (strcmp(str, "Android") == 0)
        return Id::android;
      if (strcmp(str, "BSDI") == 0)
        return Id::BSDI;
      if (strcmp(str, "Dreamcast") == 0)
        return Id::dreamcast;
      if (strcmp(str, "Emscripten") == 0)
        return Id::emscripten;
      if (strcmp(str, "FreeBSD") == 0)
        return Id::freeBSD;
      if (strcmp(str, "Haiku") == 0)
        return Id::haiku;
      if (strcmp(str, "HP-UX") == 0)
        return Id::HPUX;
      if (strcmp(str, "Irix") == 0)
        return Id::irix;
      if (strcmp(str, "Linux") == 0)
        return Id::linux;
      if (strcmp(str, "Atari MiNT") == 0)
        return Id::MiNT;
      if (strcmp(str, "MacOS Classic") == 0)
        return Id::macOSClassic;
      if (strcmp(str, "Mac OS X") == 0)
        return Id::macOSX;
      if (strcmp(str, "NaCl") == 0)
        return Id::NaCl;
      if (strcmp(str, "NetBSD") == 0)
        return Id::netBSD;
      if (strcmp(str, "OpenBSD") == 0)
        return Id::openBSD;
      if (strcmp(str, "OS/2") == 0)
        return Id::OS2;
      if (strcmp(str, "OSF/1") == 0)
        return Id::OSF;
      if (strcmp(str, "QNX Neutrino") == 0)
        return Id::QNXNeutrino;
      if (strcmp(str, "RISC OS") == 0)
        return Id::RISCOS;
      if (strcmp(str, "Solaris") == 0)
        return Id::solaris;
      if (strcmp(str, "Windows") == 0)
        return Id::windows;
      if (strcmp(str, "WinRT") == 0)
        return Id::winRT;
      if (strcmp(str, "tvOS") == 0)
        return Id::tvOS;
      if (strcmp(str, "iOS") == 0)
        return Id::iOS;
      if (strcmp(str, "PlayStation Portable") == 0)
        return Id::PSP;
      return Id::unknown;
    }


    unsigned int ram() noexcept {
      return static_cast<unsigned int>(SDL_GetSystemRAM());
    }
    unsigned int logicalCores() noexcept {
      return static_cast<unsigned int>(SDL_GetCPUCount());
    }

    namespace cpu {
      unsigned int l1Size() noexcept {
        return static_cast<unsigned int>(SDL_GetCPUCacheLineSize());
      }

      bool has3DNow() noexcept {
        return SDL_Has3DNow() == SDL_TRUE;
      }
      bool hasAVX() noexcept {
        return SDL_HasAVX() == SDL_TRUE;
      }
      bool hasAVX2() noexcept {
        return SDL_HasAVX2() == SDL_TRUE;
      }
      bool hasAltiVec() noexcept {
        return SDL_HasAltiVec() == SDL_TRUE;
      }
      bool hasMMX() noexcept {
        return SDL_HasMMX() == SDL_TRUE;
      }
      bool hasRDTSC() noexcept {
        return SDL_HasRDTSC() == SDL_TRUE;
      }
      bool hasSSE() noexcept {
        return SDL_HasSSE() == SDL_TRUE;
      }
      bool hasSSE2() noexcept {
        return SDL_HasSSE2() == SDL_TRUE;
      }
      bool hasSSE3() noexcept {
        return SDL_HasSSE3() == SDL_TRUE;
      }
      bool hasSSE41() noexcept {
        return SDL_HasSSE41() == SDL_TRUE;
      }
      bool hasSSE42() noexcept {
        return SDL_HasSSE42() == SDL_TRUE;
      }
    }

    namespace pwr {
      Status::Status() noexcept :
        state_(State::unknown),
        time_(),
        percentage_()
      {
        int t = -1; int p = -1;
        int state = SDL_GetPowerInfo(&t, &p);
        if (t != -1)
          time_ = t;
        if (p != -1)
          percentage_ = p;

        if (state == SDL_POWERSTATE_ON_BATTERY)
          state_ = State::onBattery;
        else if (state == SDL_POWERSTATE_NO_BATTERY)
          state_ = State::noBattery;
        else if (state == SDL_POWERSTATE_CHARGING)
          state_ = State::charging;
        else if (state == SDL_POWERSTATE_CHARGED)
          state_ = State::charged;
        else if (state == SDL_POWERSTATE_UNKNOWN)
          state_ = State::unknown;
      }

      State Status::state() const noexcept {
        return state_;
      }
      std::optional<int> Status::time() const noexcept {
        return time_;
      }
      std::optional<int> Status::percentage() const noexcept {
        return percentage_;
      }

      Status status() noexcept {
        return Status();
      }
    }

    namespace clipboard {
      bool hasText() noexcept {
        return SDL_HasClipboardText() == SDL_TRUE;
      }
      SDLString getText() {
        char* x = SDL_GetClipboardText();
        if (x == nullptr)
          throw sdlexception();

        return SDLString(x);
      }
      void setText(const char* x) {
        detail::throwOnErr(SDL_SetClipboardText(x));
      }
    }

    namespace screensaver {
      void enable() noexcept {
        SDL_EnableScreenSaver();
      }
      void disable() noexcept {
        SDL_DisableScreenSaver();
      }
      bool enabled() noexcept {
        return SDL_IsScreenSaverEnabled() == SDL_TRUE;
      }
    }

    namespace driver {
      unsigned int count() {
        int res = SDL_GetNumVideoDrivers();
        if (res < 1)
          throw sdlexception(res);

        return static_cast<unsigned int>(res);
      }
      const char* name(const unsigned int i) {
        if (i >= count())
          throw std::out_of_range("rolmodl::sys::driver::name");
        return SDL_GetVideoDriver(static_cast<int>(i));
      }
      std::optional<const char*> current() noexcept {
        const char* p = SDL_GetCurrentVideoDriver();
        if (p != nullptr)
          return p;
        return {};
      }
    }

    namespace display::unsafe {
      Display byIndex(unsigned int i) {
        return Display(i);
      }

      unsigned int count() {
        int res = SDL_GetNumVideoDisplays();
        if (res < 1)
          throw sdlexception(res);

        return static_cast<unsigned int>(res);
      }
      const char* name(const unsigned int i) {
        if (i >= count())
          throw std::out_of_range("rolmodl::sys::display::unsafe::name");
        return SDL_GetDisplayName(static_cast<int>(i));
      }

      geom::RectWH bounds(const unsigned int i) {
        if (i >= count())
          throw std::out_of_range("rolmodl::sys::display::unsafe::bounds");

        SDL_Rect tmp{};
        detail::throwOnErr(SDL_GetDisplayBounds(static_cast<int>(i), &tmp));

        return geom::RectWH(tmp);
      }
      geom::RectWH usableBounds(const unsigned int i) {
        if (i >= count())
          throw std::out_of_range("rolmodl::sys::display::unsafe::usableBounds");

        SDL_Rect tmp{};
        detail::throwOnErr(SDL_GetDisplayUsableBounds(static_cast<int>(i), &tmp));

        return geom::RectWH(tmp);
      }
    }


    /*explicit*/ Display::Display(const unsigned int i) :
      name_(display::unsafe::name(i)),
      bounds_(display::unsafe::bounds(i)), usableBounds_(display::unsafe::usableBounds(i)),
      ddpi_(0),
      hdpi_(0),
      vdpi_(0)
    {
      if (i >= display::unsafe::count())
        throw std::out_of_range("rolmodl::sys::Display::Display");

      detail::throwOnErr(SDL_GetDisplayDPI(static_cast<int>(i), &ddpi_, &hdpi_, &vdpi_));
    }

    const char* Display::name() const noexcept {
      return name_;
    }

    geom::RectWH Display::bounds() const noexcept {
      return bounds_;
    }
    geom::RectWH Display::usableBounds() const noexcept {
      return usableBounds_;
    }

    float Display::ddpi() const noexcept {
      return ddpi_;
    }
    float Display::hdpi() const noexcept {
      return hdpi_;
    }
    float Display::vdpi() const noexcept {
      return vdpi_;
    }


    namespace display::mode::unsafe {
      unsigned int countForDisplayN(unsigned int n) {
        if (n >= display::unsafe::count())
          throw std::out_of_range("rolmodl::sys::display::mode::unsafe::countForDisplayN");

        int res = SDL_GetNumDisplayModes(static_cast<int>(n));
        if (res < 1)
          throw sdlexception(res);
        return static_cast<unsigned int>(res);
      }

      DisplayMode fromSDL_DisplayMode(const SDL_DisplayMode x);
      DisplayMode fromSDL_DisplayMode(const SDL_DisplayMode x) {
        return DisplayMode{
          .fmt = pixelfmt::id::unsafe::fromSDLEnum(x.format),
          .size = geom::Size{x.w, x.h},
          .refreshRate = x.refresh_rate == 0 ? std::nullopt : std::optional<unsigned int>(static_cast<unsigned int>(x.refresh_rate)),
          .unsafeDriverData = x.driverdata
        };
      }

      DisplayMode desktopForDisplayN(unsigned int n) {
        if (n >= display::unsafe::count())
          throw std::out_of_range("rolmodl::sys::display::mode::unsafe::desktopForDisplayN");

        SDL_DisplayMode tmp{};
        detail::throwOnErr(SDL_GetDesktopDisplayMode(static_cast<int>(n), &tmp));

        return fromSDL_DisplayMode(tmp);
      }
      DisplayMode currentForDisplayN(unsigned int n) {
        if (n >= display::unsafe::count())
          throw std::out_of_range("rolmodl::sys::display::mode::unsafe::currentForDisplayN");

        SDL_DisplayMode tmp{};
        detail::throwOnErr(SDL_GetCurrentDisplayMode(static_cast<int>(n), &tmp));

        return fromSDL_DisplayMode(tmp);
      }
      DisplayMode forDisplayNByIndexI(unsigned int n, unsigned int i) {
        if (n >= display::unsafe::count())
          throw std::out_of_range("rolmodl::sys::display::mode::unsafe::forDisplayNByIndexI");

        if (i >= countForDisplayN(n))
          throw std::out_of_range("rolmodl::sys::display::mode::unsafe::forDisplayNByIndexI");

        SDL_DisplayMode tmp{};
        detail::throwOnErr(SDL_GetDisplayMode(static_cast<int>(n), static_cast<int>(i), &tmp));

        return fromSDL_DisplayMode(tmp);
      }

      DisplayMode closestForDisplayN(unsigned int n, const DisplayMode ideal) {
        if (n >= display::unsafe::count())
          throw std::out_of_range("rolmodl::sys::display::mode::unsafe::closestForDisplayN");

        const SDL_DisplayMode sdl_ideal{
          .format = pixelfmt::id::unsafe::toSDLEnum(ideal.fmt),
          .w = ideal.size.w, .h = ideal.size.h,
          .refresh_rate = ideal.refreshRate.has_value() ? static_cast<int>(*ideal.refreshRate) : 0,
          .driverdata = ideal.unsafeDriverData
        };

        SDL_DisplayMode tmp{};
        if (SDL_GetClosestDisplayMode(static_cast<int>(n), &sdl_ideal, &tmp) == nullptr)
          throw sdlexception();

        return fromSDL_DisplayMode(tmp);
      }
    }
  }

  /*explicit*/ sdlexception::sdlexception(const int code) noexcept :
    code_(code),
    msg_(SDL_GetError())
  {}
  sdlexception::sdlexception() noexcept :
    sdlexception(0)
  {}

  int sdlexception::code() const noexcept {
    return code_;
  }
  const char* sdlexception::what() const noexcept /*override*/ {
    return msg_;
  }

  int detail::throwOnErr(const int code) {
    if (code < 0)
      throw sdlexception(code);
    return code;
  }


  SDLString::SDLString() noexcept :
    raw_(nullptr)
  {}

  /*explicit*/ SDLString::SDLString(char* x) noexcept :
    raw_(x)
  {
    assert(raw_ != nullptr);
  }

  SDLString::~SDLString() noexcept {
    if (raw_ != nullptr)
      SDL_free(raw_);
    raw_ = nullptr;
  }

  // SDLString::SDLString(const SDLString& that) = delete;
  SDLString::SDLString(SDLString&& that) noexcept :
    SDLString()
  {
    std::swap(*this, that);
  }

  // SDLString& SDLString::operator=(const SDLString& that) = delete;
  SDLString& SDLString::operator=(SDLString&& that) noexcept {
    std::swap(*this, that);
    return *this;
  }

  void swap(SDLString& a, SDLString& b) noexcept {
    using std::swap;
    swap(a.raw_, b.raw_);
  }

  char* SDLString::unsafeRaw() noexcept {
    assert(raw_ != nullptr);
    return raw_;
  }
  const char* SDLString::raw() const noexcept {
    assert(raw_ != nullptr);
    return raw_;
  }
}
