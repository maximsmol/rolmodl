#include "hpp/Kb.hpp"

#include <stdexcept>

namespace rolmodl::kb {
  namespace key {
    /*static*/ void Name::query(const Key k) noexcept {
      instance().data_ = SDL_GetKeyName(key::unsafe::toSDLEnum(k));
    }
    /*static*/ const char* Name::lastQueryRes() noexcept {
      return instance().data_;
    }

    /*static*/ Name& Name::instance() noexcept {
      static Name res{};
      return res;
    }

    Name::Name() :
      data_(nullptr)
    {}
  }

  /*static*/ bool State::down(const Scancode s) {
    const unsigned int i = scancode::unsafe::toSDLEnum(s);
    if (i > unsafeRawL())
      throw std::out_of_range("rolmodl::kb::State::down");
    return static_cast<bool>(unsafeRaw()[i]);
  }
  /*static*/ bool State::up(const Scancode s) {
    return !down(s);
  }

  /*static*/ bool State::down(const Key k) {
    return down(key::scancode(k));
  }
  /*static*/ bool State::up(const Key k) {
    return !down(k);
  }

  /*static*/ State& State::instance() noexcept {
    static State res{};
    return res;
  }

  /*static*/ const uint8_t* State::unsafeRaw() noexcept {
    return instance().data_;
  }
  /*static*/ unsigned int State::unsafeRawL() noexcept {
    return instance().l_;
  }

  State::State() noexcept :
    data_(nullptr),
    l_(0)
  {
    int tmp = 0;
    data_ = SDL_GetKeyboardState(&tmp);
    l_ = static_cast<unsigned int>(tmp);
  }
}
