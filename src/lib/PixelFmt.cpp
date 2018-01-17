#include "hpp/PixelFmt.hpp"

#include "hpp/Base.hpp"

namespace rolmodl::pixelfmt {
  const SDL_PixelFormat& PixelFmtStorage::get(const Id id) const {
    const size_t i = static_cast<size_t>(id);

    if (data_[i] == nullptr) {
      SDL_PixelFormat* tmp = SDL_AllocFormat(id::unsafe::toSDLEnum(id));
      if (tmp == nullptr)
        throw sdlexception();

      data_[i] = tmp;
    }

    return *data_[i];
  }

  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wexit-time-destructors"
  /*static*/ PixelFmtStorage& PixelFmtStorage::instance() noexcept {
    static PixelFmtStorage res{};
    return res;
  }
  #pragma clang diagnostic pop

  PixelFmtStorage::PixelFmtStorage() noexcept :
    data_(new SDL_PixelFormat*[unique_count])
  {
    for (size_t i = 0; i < unique_count; ++i)
      data_[i] = nullptr;
  }
  PixelFmtStorage::~PixelFmtStorage() noexcept {
    if (data_ != nullptr) {
      for (size_t i = 0; i < unique_count; ++i)
        if (data_[i] != nullptr)
          SDL_FreeFormat(data_[i]);

      delete[] data_;
    }
  }
}
