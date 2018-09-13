#pragma once

#include <cstdint>

#include <SDL.h>

namespace rolmodl::pixelfmt {
  enum class Id {
    unknown,
    index1lsb,
    index1msb,
    index4lsb,
    index4msb,
    index8,
    rgb332,
    rgb444,
    rgb555,
    bgr555,
    argb4444,
    rgba4444,
    abgr4444,
    bgra4444,
    argb1555,
    rgba5551,
    abgr1555,
    bgra5551,
    rgb565,
    bgr565,
    rgb24,
    bgr24,
    rgb888,
    rgbx8888,
    bgr888,
    bgrx8888,
    argb8888,
    rgba8888,
    abgr8888,
    bgra8888,
    argb2101010,
    // 31 lines

    yv12,
    iyuv,
    yuy2,
    uyvy,
    yvyu,
    nv12,
    nv21,
    // 7 lines

    // = 48 total

    // aliases
    rgba32,
    argb32,
    bgra32,
    abgr32
  };
  constexpr size_t unique_count = 48;

  namespace id::unsafe {
    constexpr Id fromSDLEnum(const uint32_t val) noexcept;
    constexpr uint32_t toSDLEnum(const Id i) noexcept;
  }

  constexpr unsigned int bitSizeOf(const Id id) noexcept {
    return SDL_BITSPERPIXEL(id::unsafe::toSDLEnum(id));
  }
  constexpr unsigned int byteSizeOf(const Id id) noexcept {
    return SDL_BYTESPERPIXEL(id::unsafe::toSDLEnum(id));
  }

  constexpr bool isIndexed(const Id id) noexcept {
    return SDL_ISPIXELFORMAT_INDEXED(id::unsafe::toSDLEnum(id)) == SDL_TRUE;
  }
  constexpr bool hasAlpha(const Id id) noexcept {
    return SDL_ISPIXELFORMAT_ALPHA(id::unsafe::toSDLEnum(id)) == SDL_TRUE;
  }
  constexpr bool isUnique(const Id id) noexcept {
    return SDL_ISPIXELFORMAT_FOURCC(id::unsafe::toSDLEnum(id)) == SDL_TRUE;
  }

  enum class Type {
    unknown,

    index_1,
    index_4,
    index_8,

    packed_8,
    packed_16,
    packed_32,

    array_u8,
    array_u16,
    array_u32,
    array_f16,
    array_f32
  };
  namespace type::unsafe {
    constexpr Type fromSDLEnum(const uint32_t val) noexcept;
    constexpr uint32_t toSDLEnum(const Type t) noexcept;
  }
  constexpr Type typeOf(const Id id) noexcept {
    return type::unsafe::fromSDLEnum(SDL_PIXELTYPE(id::unsafe::toSDLEnum(id)));
  }

  enum class Order {
    bitmap_none,
    bitmap_4321,
    bitmap_1234,

    packed_none,
    packed_xrgb,
    packed_rgbx,
    packed_argb,
    packed_rgba,
    packed_xbgr,
    packed_bgrx,
    packed_abgr,
    packed_bgra,

    array_none,
    array_rgb,
    array_rgba,
    array_argb,
    array_bgr,
    array_bgra,
    array_abgr
  };
  namespace order::unsafe {
    constexpr Order fromSDLEnum(const uint32_t val) noexcept;
    constexpr uint32_t toSDLEnum(const Order o) noexcept;
  }
  constexpr Order orderOf(const Id id) noexcept {
    return order::unsafe::fromSDLEnum(SDL_PIXELORDER(id::unsafe::toSDLEnum(id)));
  }

  enum class Layout {
    none,
    bits_332,
    bits_4444,
    bits_1555,
    bits_5551,
    bits_565,
    bits_8888,
    bits_2101010,
    bits_1010102
  };
  namespace layout::unsafe {
    constexpr Layout fromSDLEnum(const uint32_t val) noexcept;
    constexpr uint32_t toSDLEnum(const Layout l) noexcept;
  }
  constexpr Layout layoutOf(const Id id) noexcept {
    return layout::unsafe::fromSDLEnum(SDL_PIXELLAYOUT(id::unsafe::toSDLEnum(id)));
  }

  class PixelFmtStorage {
    public:
      PixelFmtStorage(const PixelFmtStorage& that) = delete;
      PixelFmtStorage(PixelFmtStorage&& that) = delete;

      PixelFmtStorage& operator=(const PixelFmtStorage& that) = delete;
      PixelFmtStorage& operator=(PixelFmtStorage&& that) = delete;

      static const SDL_PixelFormat& get(const Id id);
      static PixelFmtStorage& instance() noexcept;

      static SDL_PixelFormat** unsafeRaw() noexcept;

    private:
      PixelFmtStorage() noexcept;
      ~PixelFmtStorage() noexcept;

      SDL_PixelFormat** data_;
  };

  namespace id::unsafe {
    constexpr Id fromSDLEnum(const uint32_t val) noexcept {
      if (val == SDL_PIXELFORMAT_UNKNOWN)
        return Id::unknown;
      if (val == SDL_PIXELFORMAT_INDEX1LSB)
        return Id::index1lsb;
      if (val == SDL_PIXELFORMAT_INDEX1MSB)
        return Id::index1msb;
      if (val == SDL_PIXELFORMAT_INDEX4LSB)
        return Id::index4lsb;
      if (val == SDL_PIXELFORMAT_INDEX4MSB)
        return Id::index4msb;
      if (val == SDL_PIXELFORMAT_INDEX8)
        return Id::index8;
      if (val == SDL_PIXELFORMAT_RGB332)
        return Id::rgb332;
      if (val == SDL_PIXELFORMAT_RGB444)
        return Id::rgb444;
      if (val == SDL_PIXELFORMAT_RGB555)
        return Id::rgb555;
      if (val == SDL_PIXELFORMAT_BGR555)
        return Id::bgr555;
      if (val == SDL_PIXELFORMAT_ARGB4444)
        return Id::argb4444;
      if (val == SDL_PIXELFORMAT_RGBA4444)
        return Id::rgba4444;
      if (val == SDL_PIXELFORMAT_ABGR4444)
        return Id::abgr4444;
      if (val == SDL_PIXELFORMAT_BGRA4444)
        return Id::bgra4444;
      if (val == SDL_PIXELFORMAT_ARGB1555)
        return Id::argb1555;
      if (val == SDL_PIXELFORMAT_RGBA5551)
        return Id::rgba5551;
      if (val == SDL_PIXELFORMAT_ABGR1555)
        return Id::abgr1555;
      if (val == SDL_PIXELFORMAT_BGRA5551)
        return Id::bgra5551;
      if (val == SDL_PIXELFORMAT_RGB565)
        return Id::rgb565;
      if (val == SDL_PIXELFORMAT_BGR565)
        return Id::bgr565;
      if (val == SDL_PIXELFORMAT_RGB24)
        return Id::rgb24;
      if (val == SDL_PIXELFORMAT_BGR24)
        return Id::bgr24;
      if (val == SDL_PIXELFORMAT_RGB888)
        return Id::rgb888;
      if (val == SDL_PIXELFORMAT_RGBX8888)
        return Id::rgbx8888;
      if (val == SDL_PIXELFORMAT_BGR888)
        return Id::bgr888;
      if (val == SDL_PIXELFORMAT_BGRX8888)
        return Id::bgrx8888;
      if (val == SDL_PIXELFORMAT_ARGB8888)
        return Id::argb8888;
      if (val == SDL_PIXELFORMAT_RGBA8888)
        return Id::rgba8888;
      if (val == SDL_PIXELFORMAT_ABGR8888)
        return Id::abgr8888;
      if (val == SDL_PIXELFORMAT_BGRA8888)
        return Id::bgra8888;
      if (val == SDL_PIXELFORMAT_ARGB2101010)
        return Id::argb2101010;

      if (val == SDL_PIXELFORMAT_YV12)
        return Id::yv12;
      if (val == SDL_PIXELFORMAT_IYUV)
        return Id::iyuv;
      if (val == SDL_PIXELFORMAT_YUY2)
        return Id::yuy2;
      if (val == SDL_PIXELFORMAT_UYVY)
        return Id::uyvy;
      if (val == SDL_PIXELFORMAT_YVYU)
        return Id::yvyu;
      if (val == SDL_PIXELFORMAT_NV12)
        return Id::nv12;
      if (val == SDL_PIXELFORMAT_NV21)
        return Id::nv21;

      // aliases for one of the above
      // should never get here
      if (val == SDL_PIXELFORMAT_RGBA32)
        return Id::rgba32;
      if (val == SDL_PIXELFORMAT_ARGB32)
        return Id::argb32;
      if (val == SDL_PIXELFORMAT_BGRA32)
        return Id::bgra32;
      if (val == SDL_PIXELFORMAT_ABGR32)
        return Id::abgr32;

      return Id::unknown;
    }
    constexpr uint32_t toSDLEnum(const Id i) noexcept {
      if (i == Id::unknown)
        return SDL_PIXELFORMAT_UNKNOWN;
      if (i == Id::index1lsb)
        return SDL_PIXELFORMAT_INDEX1LSB;
      if (i == Id::index1msb)
        return SDL_PIXELFORMAT_INDEX1MSB;
      if (i == Id::index4lsb)
        return SDL_PIXELFORMAT_INDEX4LSB;
      if (i == Id::index4msb)
        return SDL_PIXELFORMAT_INDEX4MSB;
      if (i == Id::index8)
        return SDL_PIXELFORMAT_INDEX8;
      if (i == Id::rgb332)
        return SDL_PIXELFORMAT_RGB332;
      if (i == Id::rgb444)
        return SDL_PIXELFORMAT_RGB444;
      if (i == Id::rgb555)
        return SDL_PIXELFORMAT_RGB555;
      if (i == Id::bgr555)
        return SDL_PIXELFORMAT_BGR555;
      if (i == Id::argb4444)
        return SDL_PIXELFORMAT_ARGB4444;
      if (i == Id::rgba4444)
        return SDL_PIXELFORMAT_RGBA4444;
      if (i == Id::abgr4444)
        return SDL_PIXELFORMAT_ABGR4444;
      if (i == Id::bgra4444)
        return SDL_PIXELFORMAT_BGRA4444;
      if (i == Id::argb1555)
        return SDL_PIXELFORMAT_ARGB1555;
      if (i == Id::rgba5551)
        return SDL_PIXELFORMAT_RGBA5551;
      if (i == Id::abgr1555)
        return SDL_PIXELFORMAT_ABGR1555;
      if (i == Id::bgra5551)
        return SDL_PIXELFORMAT_BGRA5551;
      if (i == Id::rgb565)
        return SDL_PIXELFORMAT_RGB565;
      if (i == Id::bgr565)
        return SDL_PIXELFORMAT_BGR565;
      if (i == Id::rgb24)
        return SDL_PIXELFORMAT_RGB24;
      if (i == Id::bgr24)
        return SDL_PIXELFORMAT_BGR24;
      if (i == Id::rgb888)
        return SDL_PIXELFORMAT_RGB888;
      if (i == Id::rgbx8888)
        return SDL_PIXELFORMAT_RGBX8888;
      if (i == Id::bgr888)
        return SDL_PIXELFORMAT_BGR888;
      if (i == Id::bgrx8888)
        return SDL_PIXELFORMAT_BGRX8888;
      if (i == Id::argb8888)
        return SDL_PIXELFORMAT_ARGB8888;
      if (i == Id::rgba8888)
        return SDL_PIXELFORMAT_RGBA8888;
      if (i == Id::abgr8888)
        return SDL_PIXELFORMAT_ABGR8888;
      if (i == Id::bgra8888)
        return SDL_PIXELFORMAT_BGRA8888;
      if (i == Id::argb2101010)
        return SDL_PIXELFORMAT_ARGB2101010;

      if (i == Id::yv12)
        return SDL_PIXELFORMAT_YV12;
      if (i == Id::iyuv)
        return SDL_PIXELFORMAT_IYUV;
      if (i == Id::yuy2)
        return SDL_PIXELFORMAT_YUY2;
      if (i == Id::uyvy)
        return SDL_PIXELFORMAT_UYVY;
      if (i == Id::yvyu)
        return SDL_PIXELFORMAT_YVYU;
      if (i == Id::nv12)
        return SDL_PIXELFORMAT_NV12;
      if (i == Id::nv21)
        return SDL_PIXELFORMAT_NV21;

      if (i == Id::rgba32)
        return SDL_PIXELFORMAT_RGBA32;
      if (i == Id::argb32)
        return SDL_PIXELFORMAT_ARGB32;
      if (i == Id::bgra32)
        return SDL_PIXELFORMAT_BGRA32;
      if (i == Id::abgr32)
        return SDL_PIXELFORMAT_ABGR32;

      return SDL_PIXELFORMAT_UNKNOWN;
    }
  }

  namespace type::unsafe {
    constexpr Type fromSDLEnum(const uint32_t val) noexcept {
      if (val == SDL_PIXELTYPE_UNKNOWN)
        return Type::unknown;

      if (val == SDL_PIXELTYPE_INDEX1)
        return Type::index_1;
      if (val == SDL_PIXELTYPE_INDEX4)
        return Type::index_4;
      if (val == SDL_PIXELTYPE_INDEX8)
        return Type::index_8;

      if (val == SDL_PIXELTYPE_PACKED8)
        return Type::packed_8;
      if (val == SDL_PIXELTYPE_PACKED16)
        return Type::packed_16;
      if (val == SDL_PIXELTYPE_PACKED32)
        return Type::packed_32;

      if (val == SDL_PIXELTYPE_ARRAYU8)
        return Type::array_u8;
      if (val == SDL_PIXELTYPE_ARRAYU16)
        return Type::array_u16;
      if (val == SDL_PIXELTYPE_ARRAYU32)
        return Type::array_u32;
      if (val == SDL_PIXELTYPE_ARRAYF16)
        return Type::array_f16;
      if (val == SDL_PIXELTYPE_ARRAYF32)
        return Type::array_f32;

      return Type::unknown;
    }
    constexpr uint32_t toSDLEnum(const Type t) noexcept {
      if (t == Type::unknown)
        return SDL_PIXELTYPE_UNKNOWN;

      if (t == Type::index_1)
        return SDL_PIXELTYPE_INDEX1;
      if (t == Type::index_4)
        return SDL_PIXELTYPE_INDEX4;
      if (t == Type::index_8)
        return SDL_PIXELTYPE_INDEX8;

      if (t == Type::packed_8)
        return SDL_PIXELTYPE_PACKED8;
      if (t == Type::packed_16)
        return SDL_PIXELTYPE_PACKED16;
      if (t == Type::packed_32)
        return SDL_PIXELTYPE_PACKED32;

      if (t == Type::array_u8)
        return SDL_PIXELTYPE_ARRAYU8;
      if (t == Type::array_u16)
        return SDL_PIXELTYPE_ARRAYU16;
      if (t == Type::array_u32)
        return SDL_PIXELTYPE_ARRAYU32;
      if (t == Type::array_f16)
        return SDL_PIXELTYPE_ARRAYF16;
      if (t == Type::array_f32)
        return SDL_PIXELTYPE_ARRAYF32;

      return SDL_PIXELTYPE_UNKNOWN;
    }
  }

  namespace order::unsafe {
    constexpr Order fromSDLEnum(const uint32_t val) noexcept {
      if (val == SDL_BITMAPORDER_NONE)
        return Order::bitmap_none;
      if (val == SDL_BITMAPORDER_4321)
        return Order::bitmap_4321;
      if (val == SDL_BITMAPORDER_1234)
        return Order::bitmap_1234;

      if (val == SDL_PACKEDORDER_NONE)
        return Order::packed_none;
      if (val == SDL_PACKEDORDER_XRGB)
        return Order::packed_xrgb;
      if (val == SDL_PACKEDORDER_RGBX)
        return Order::packed_rgbx;
      if (val == SDL_PACKEDORDER_ARGB)
        return Order::packed_argb;
      if (val == SDL_PACKEDORDER_RGBA)
        return Order::packed_rgba;
      if (val == SDL_PACKEDORDER_XBGR)
        return Order::packed_xbgr;
      if (val == SDL_PACKEDORDER_BGRX)
        return Order::packed_bgrx;
      if (val == SDL_PACKEDORDER_ABGR)
        return Order::packed_abgr;
      if (val == SDL_PACKEDORDER_BGRA)
        return Order::packed_bgra;

      if (val == SDL_ARRAYORDER_NONE)
        return Order::array_none;
      if (val == SDL_ARRAYORDER_RGB)
        return Order::array_rgb;
      if (val == SDL_ARRAYORDER_RGBA)
        return Order::array_rgba;
      if (val == SDL_ARRAYORDER_ARGB)
        return Order::array_argb;
      if (val == SDL_ARRAYORDER_BGR)
        return Order::array_bgr;
      if (val == SDL_ARRAYORDER_BGRA)
        return Order::array_bgra;
      if (val == SDL_ARRAYORDER_ABGR)
        return Order::array_abgr;

      return Order::bitmap_none;
    }
    constexpr uint32_t toSDLEnum(const Order o) noexcept {
      if (o == Order::bitmap_none)
        return SDL_BITMAPORDER_NONE;
      if (o == Order::bitmap_4321)
        return SDL_BITMAPORDER_4321;
      if (o == Order::bitmap_1234)
        return SDL_BITMAPORDER_1234;

      if (o == Order::packed_none)
        return SDL_PACKEDORDER_NONE;
      if (o == Order::packed_xrgb)
        return SDL_PACKEDORDER_XRGB;
      if (o == Order::packed_rgbx)
        return SDL_PACKEDORDER_RGBX;
      if (o == Order::packed_argb)
        return SDL_PACKEDORDER_ARGB;
      if (o == Order::packed_rgba)
        return SDL_PACKEDORDER_RGBA;
      if (o == Order::packed_xbgr)
        return SDL_PACKEDORDER_XBGR;
      if (o == Order::packed_bgrx)
        return SDL_PACKEDORDER_BGRX;
      if (o == Order::packed_abgr)
        return SDL_PACKEDORDER_ABGR;
      if (o == Order::packed_bgra)
        return SDL_PACKEDORDER_BGRA;

      if (o == Order::array_none)
        return SDL_ARRAYORDER_NONE;
      if (o == Order::array_rgb)
        return SDL_ARRAYORDER_RGB;
      if (o == Order::array_rgba)
        return SDL_ARRAYORDER_RGBA;
      if (o == Order::array_argb)
        return SDL_ARRAYORDER_ARGB;
      if (o == Order::array_bgr)
        return SDL_ARRAYORDER_BGR;
      if (o == Order::array_bgra)
        return SDL_ARRAYORDER_BGRA;
      if (o == Order::array_abgr)
        return SDL_ARRAYORDER_ABGR;

      return SDL_BITMAPORDER_NONE;
    }
  }

  namespace layout::unsafe {
    constexpr Layout fromSDLEnum(const uint32_t val) noexcept {
      if (val == SDL_PACKEDLAYOUT_NONE)
        return Layout::none;
      if (val == SDL_PACKEDLAYOUT_332)
        return Layout::bits_332;
      if (val == SDL_PACKEDLAYOUT_4444)
        return Layout::bits_4444;
      if (val == SDL_PACKEDLAYOUT_1555)
        return Layout::bits_1555;
      if (val == SDL_PACKEDLAYOUT_5551)
        return Layout::bits_5551;
      if (val == SDL_PACKEDLAYOUT_565)
        return Layout::bits_565;
      if (val == SDL_PACKEDLAYOUT_8888)
        return Layout::bits_8888;
      if (val == SDL_PACKEDLAYOUT_2101010)
        return Layout::bits_2101010;
      if (val == SDL_PACKEDLAYOUT_1010102)
        return Layout::bits_1010102;

      return Layout::none;
    }
    constexpr uint32_t toSDLEnum(const Layout l) noexcept {
      if (l == Layout::none)
        return SDL_PACKEDLAYOUT_NONE;
      if (l == Layout::bits_332)
        return SDL_PACKEDLAYOUT_332;
      if (l == Layout::bits_4444)
        return SDL_PACKEDLAYOUT_4444;
      if (l == Layout::bits_1555)
        return SDL_PACKEDLAYOUT_1555;
      if (l == Layout::bits_5551)
        return SDL_PACKEDLAYOUT_5551;
      if (l == Layout::bits_565)
        return SDL_PACKEDLAYOUT_565;
      if (l == Layout::bits_8888)
        return SDL_PACKEDLAYOUT_8888;
      if (l == Layout::bits_2101010)
        return SDL_PACKEDLAYOUT_2101010;
      if (l == Layout::bits_1010102)
        return SDL_PACKEDLAYOUT_1010102;

      return SDL_PACKEDLAYOUT_NONE;
    }
  }
}
