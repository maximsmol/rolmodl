#pragma once

#include <cstdint>

#include <SDL.h>

namespace rolmodl::pixelfmt {
  enum class Id;

  // constexpr size_t unique_count = 48;
  namespace id::unsafe {
    constexpr Id fromSDLEnum(const uint32_t val) noexcept;
    constexpr uint32_t toSDLEnum(const Id i) noexcept;
  }

  namespace id {
    constexpr const char* toString(const Id id) noexcept;
  }
  constexpr unsigned int bitSizeOf(const Id id) noexcept;
  constexpr unsigned int byteSizeOf(const Id id) noexcept;
  constexpr bool isIndexed(const Id id) noexcept;
  constexpr bool hasAlpha(const Id id) noexcept;
  constexpr bool isUnique(const Id id) noexcept;

  enum class Type;
  namespace type::unsafe {
    constexpr Type fromSDLEnum(const uint32_t val) noexcept;
    constexpr uint32_t toSDLEnum(const Type t) noexcept;
  }
  constexpr Type typeOf(const Id id) noexcept;

  enum class Order;
  namespace order::unsafe {
    constexpr Order fromSDLEnum(const uint32_t val) noexcept;
    constexpr uint32_t toSDLEnum(const Order o) noexcept;
  }
  constexpr Order orderOf(const Id id) noexcept;

  enum class Layout;
  namespace layout::unsafe {
    constexpr Layout fromSDLEnum(const uint32_t val) noexcept;
    constexpr uint32_t toSDLEnum(const Layout l) noexcept;
  }
  constexpr Layout layoutOf(const Id id) noexcept;

  class PixelFmtStorage;
  namespace id::unsafe {
    constexpr Id fromSDLEnum(const uint32_t val) noexcept;
    constexpr uint32_t toSDLEnum(const Id i) noexcept;
  }

  namespace type::unsafe {
    constexpr Type fromSDLEnum(const uint32_t val) noexcept;
    constexpr uint32_t toSDLEnum(const Type t) noexcept;
  }

  namespace order::unsafe {
    constexpr Order fromSDLEnum(const uint32_t val) noexcept;
    constexpr uint32_t toSDLEnum(const Order o) noexcept;
  }

  namespace layout::unsafe {
    constexpr Layout fromSDLEnum(const uint32_t val) noexcept;
    constexpr uint32_t toSDLEnum(const Layout l) noexcept;
  }
}

