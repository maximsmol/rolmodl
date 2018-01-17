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
        return Id::Android;
      if (strcmp(str, "BSDI") == 0)
        return Id::BSDI;
      if (strcmp(str, "Dreamcast") == 0)
        return Id::Dreamcast;
      if (strcmp(str, "Emscripten") == 0)
        return Id::Emscripten;
      if (strcmp(str, "FreeBSD") == 0)
        return Id::FreeBSD;
      if (strcmp(str, "Haiku") == 0)
        return Id::Haiku;
      if (strcmp(str, "HP-UX") == 0)
        return Id::HPUX;
      if (strcmp(str, "Irix") == 0)
        return Id::Irix;
      if (strcmp(str, "Linux") == 0)
        return Id::Linux;
      if (strcmp(str, "Atari MiNT") == 0)
        return Id::MiNT;
      if (strcmp(str, "MacOS Classic") == 0)
        return Id::MacOSClassic;
      if (strcmp(str, "Mac OS X") == 0)
        return Id::MacOSX;
      if (strcmp(str, "NaCl") == 0)
        return Id::NaCl;
      if (strcmp(str, "NetBSD") == 0)
        return Id::NetBSD;
      if (strcmp(str, "OpenBSD") == 0)
        return Id::OpenBSD;
      if (strcmp(str, "OS/2") == 0)
        return Id::OS2;
      if (strcmp(str, "OSF/1") == 0)
        return Id::OSF;
      if (strcmp(str, "QNX Neutrino") == 0)
        return Id::QNXNeutrino;
      if (strcmp(str, "RISC OS") == 0)
        return Id::RISCOS;
      if (strcmp(str, "Solaris") == 0)
        return Id::Solaris;
      if (strcmp(str, "Windows") == 0)
        return Id::Windows;
      if (strcmp(str, "WinRT") == 0)
        return Id::WinRT;
      if (strcmp(str, "tvOS") == 0)
        return Id::tvOS;
      if (strcmp(str, "iOS") == 0)
        return Id::iOS;
      if (strcmp(str, "PlayStation Portable") == 0)
        return Id::PSP;
      return Id::Unknown;
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
        state_(State::Unknown),
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
          state_ = State::OnBattery;
        else if (state == SDL_POWERSTATE_NO_BATTERY)
          state_ = State::NoBattery;
        else if (state == SDL_POWERSTATE_CHARGING)
          state_ = State::Charging;
        else if (state == SDL_POWERSTATE_CHARGED)
          state_ = State::Charged;
        else if (state == SDL_POWERSTATE_UNKNOWN)
          state_ = State::Unknown;
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
}
