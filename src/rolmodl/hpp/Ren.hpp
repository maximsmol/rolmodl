#pragma once

/// \file
/// Common SDL rendering structs and support for SDL accelerated rendering.
///
/// \sa https://wiki.libsdl.org/CategoryRender

#include "forwarddecl/Ren.hpp"

#include "forwarddecl/Win.hpp"
#include "forwarddecl/Tex.hpp"
#include "forwarddecl/Base.hpp"
#include "Geom.hpp"
#include "PixelFmt.hpp"

/// Main namespace
namespace rolmodl {
  /// \brief Describes how colors are combined when rendering twice to the same place.
  ///
  /// \sa https://wiki.libsdl.org/SDL_BlendMode
  enum class BlendMode {
    /// \brief Do not blend. Equivalent SDL enum is `SDL_BLENDMODE_NONE`.
    ///
    /// `output RGBA = source RGBA`
    none,
    /// \brief Alpha-blending. Treats the alpha value as transparency. Equivalent SDL enum is `SDL_BLENDMODE_BLEND`.
    ///
    /// `output RGB = source RGB * source A + output RGB * (1 - source A)`
    /// `output A = source A + output A * (1 - source A)`
    blend,
    /// \brief Additive blending. Equivalent SDL enum is `SDL_BLENDMODE_ADD`.
    ///
    /// `output RGB = source RGB * source A + output RGB`
    /// `output A = output A`
    add,
    /// \brief Color modulation blending. Equivalent SDL enum is `SDL_BLENDMODE_MOD`.
    ///
    /// `output RGB = source RGB * output RGB`
    /// `output A = output A`
    mod
  };
  /// Unsafe \link rolmodl::BlendMode \endlink methods
  namespace blendMode::unsafe {
    /// \brief Convert an <a href="https://wiki.libsdl.org/SDL_BlendMode">`SDL_BlendMode`</a> value to a \link rolmodl::BlendMode \endlink value.
    constexpr BlendMode fromSDLEnum(const SDL_BlendMode m) noexcept {
      if (m == SDL_BLENDMODE_NONE)
        return BlendMode::none;
      if (m == SDL_BLENDMODE_BLEND)
        return BlendMode::blend;
      if (m == SDL_BLENDMODE_ADD)
        return BlendMode::add;
      // if (m == SDL_BLENDMODE_MOD)
        return BlendMode::mod;
    }
    /// \brief Convert a \link rolmodl::BlendMode \endlink value to an <a href="https://wiki.libsdl.org/SDL_BlendMode">`SDL_BlendMode`</a> value.
    constexpr SDL_BlendMode toSDLEnum(const BlendMode m) noexcept {
      if (m == BlendMode::none)
        return SDL_BLENDMODE_NONE;
      if (m == BlendMode::blend)
        return SDL_BLENDMODE_BLEND;
      if (m == BlendMode::add)
        return SDL_BLENDMODE_ADD;
      // if (m == BlendMode::mod)
        return SDL_BLENDMODE_MOD;
    }
  }

  /// \brief Rendering flip configuration container.
  /// \sa https://wiki.libsdl.org/SDL_RendererFlip
  struct Flip {
    public:
      /// \brief Create a configuration corresponding to no flipping. The SDL flag equivalent is `SDL_FLIP_NONE`.
      constexpr Flip() noexcept :
        data_(SDL_FLIP_NONE) // todo: assumes that SDL_FLIP_NONE = 0
      {}

      // test
      /// \brief Test whether this configuration has horizontal flipping enabled. The SDL flag equivalent is `SDL_FLIP_HORIZONTAL`.
      constexpr Flip isHorizontal() const noexcept {
        return Flip(data_ | static_cast<uint32_t>(SDL_FLIP_HORIZONTAL));
      }
      /// \brief Test whether this configuration has vertical flipping enabled. The SDL flag equivalent is `SDL_FLIP_VERTICAL`.
      constexpr Flip isVertical() const noexcept {
        return Flip(data_ | static_cast<uint32_t>(SDL_FLIP_VERTICAL));
      }

      // set
      /// \brief Create a version of this configuration that has horizontal flipping enabled. The SDL flag equivalent is `SDL_FLIP_HORIZONTAL`.
      constexpr Flip withHorizontal() const noexcept {
        return Flip(data_ | static_cast<uint32_t>(SDL_FLIP_HORIZONTAL));
      }
      /// \brief Create a version of this configuration that has vertical flipping enabled. The SDL flag equivalent is `SDL_FLIP_VERTICAL`.
      constexpr Flip withVertical() const noexcept {
        return Flip(data_ | static_cast<uint32_t>(SDL_FLIP_VERTICAL));
      }

      // unset
      /// \brief Create a version of this configuration that does not have horizontal flipping enabled. The SDL flag equivalent is `~SDL_FLIP_HORIZONTAL`.
      constexpr Flip withoutHorizontal() const noexcept {
        return Flip(data_ & ~static_cast<uint32_t>(SDL_FLIP_HORIZONTAL));
      }
      /// \brief Create a version of this configuration that does not have vertical flipping enabled. The SDL flag equivalent is `~SDL_FLIP_VERTICAL`.
      constexpr Flip withoutVertical() const noexcept {
        return Flip(data_ & ~static_cast<uint32_t>(SDL_FLIP_VERTICAL));
      }

      /// \brief Return the underlying bitfield representation of this configuration as the SDL enum type `SDL_RendererFlip`.
      constexpr SDL_RendererFlip toSDLEnum() const noexcept {
        return static_cast<SDL_RendererFlip>(data_);
      }
      /// \brief Return the underlying bitfield representation of this configuration.
      constexpr uint32_t raw() const noexcept {
        return data_;
      }
    private:
      explicit constexpr Flip(const uint32_t data) noexcept :
        data_(data)
      {}

      uint32_t data_;
  };

  /// \link rolmodl::Ren \endlink -related members
  namespace ren {
    /// \brief Renderer configuration (flags) container.
    ///
    /// \todo
    /// todo: some flags might be mutually exclusive
    /// fixme: how does CreateSoftwareRenderer relate
    ///
    /// \sa https://wiki.libsdl.org/SDL_RendererFlags
    struct Flags {
      public:
        /// \brief Create a configuration corresponding to a default renderer.
        constexpr Flags() noexcept :
          data_(0)
        {}

        // test
        /// \brief Test whether this configuration corresponds to a software renderer. The SDL flag equivalent is `SDL_RENDERER_SOFTWARE`.
        constexpr bool isSoftware() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_RENDERER_SOFTWARE)) != 0;
        }
        /// \brief Test whether this configuration corresponds to an accelerated renderer. The SDL flag equivalent is `SDL_RENDERER_ACCELERATED`.
        constexpr bool isAccelerated() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_RENDERER_ACCELERATED)) != 0;
        }
        /// \brief Test whether this configuration corresponds to a VSync renderer. The SDL flag equivalent is `SDL_RENDERER_PRESENTVSYNC`.
        constexpr bool isVsync() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_RENDERER_PRESENTVSYNC)) != 0;
        }
        /// \brief Test whether this configuration corresponds to a renderer capable of rendering to texture. The SDL flag equivalent is `SDL_RENDERER_TARGETTEXTURE`.
        constexpr bool isToTexture() const noexcept {
          return (data_ & static_cast<uint32_t>(SDL_RENDERER_TARGETTEXTURE)) != 0;
        }

        // set
        /// \brief Create a version of this configuration that corresponds to a software renderer. The SDL flag equivalent is `SDL_RENDERER_SOFTWARE`.
        constexpr Flags withSoftware() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_RENDERER_SOFTWARE));
        }
        /// \brief Create a version of this configuration that corresponds to an accelerated renderer. The SDL flag equivalent is `SDL_RENDERER_ACCELERATED`.
        constexpr Flags withAccelerated() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_RENDERER_ACCELERATED));
        }
        /// \brief Create a version of this configuration that corresponds to a VSync renderer. The SDL flag equivalent is `SDL_RENDERER_PRESENTVSYNC`.
        constexpr Flags withVsync() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_RENDERER_PRESENTVSYNC));
        }
        /// \brief Create a version of this configuration that corresponds to a renderer capable of rendering to texture. The SDL flag equivalent is `SDL_RENDERER_TARGETTEXTURE`.
        constexpr Flags withToTexture() const noexcept {
          return Flags(data_ | static_cast<uint32_t>(SDL_RENDERER_TARGETTEXTURE));
        }

        // unset
        /// \brief Create a version of this configuration that corresponds to a renderer that is not software. The SDL flag equivalent is `~SDL_RENDERER_SOFTWARE`.
        constexpr Flags withoutSoftware() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_RENDERER_SOFTWARE));
        }
        /// \brief Create a version of this configuration that corresponds to a renderer that is not accelerated. The SDL flag equivalent is `~SDL_RENDERER_ACCELERATED`.
        constexpr Flags withoutAccelerated() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_RENDERER_ACCELERATED));
        }
        /// \brief Create a version of this configuration that corresponds to a renderer that is not VSync. The SDL flag equivalent is `~SDL_RENDERER_PRESENTVSYNC`.
        constexpr Flags withoutVsync() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_RENDERER_PRESENTVSYNC));
        }
        /// \brief Create a version of this configuration that corresponds to a renderer that is not capable of rendering to texture. The SDL flag equivalent is `~SDL_RENDERER_TARGETTEXTURE`.
        constexpr Flags withoutToTexture() const noexcept {
          return Flags(data_ & ~static_cast<uint32_t>(SDL_RENDERER_TARGETTEXTURE));
        }

        /// \brief Return the underlying bitfield representation of this configuration.
        constexpr uint32_t raw() const noexcept {
          return data_;
        }
        /// \brief Create a configuration representing the bitfield `data`. Unsafe because `data` is not verified as representing a valid configuration.
        constexpr static Flags unsafeFromRaw(const uint32_t data) noexcept {
          return Flags(data);
        }
      private:
        explicit constexpr Flags(const uint32_t data) noexcept :
          data_(data)
        {}

        uint32_t data_;
    };

    /// \brief Information about a rendering driver.
    /// \sa https://wiki.libsdl.org/SDL_RendererInfo
    struct Info {
      /// \brief Name of the driver. \todo add examples
      const char* name;
      /// \brief Maximal renderer configuration supported by the driver.
      Flags flags;
      /// \brief Pixel formats supported by the driver.
      std::vector<pixelfmt::Id> pixelFmts;
      /// \brief Maximum texture size supported by the driver.
      geom::Size maxTexSize;
    };

    /// Renderer driver information functions
    namespace driver {
      // fixme: there is something off with this and exception guarantees, not sure if its good design

      /// \brief Query the amount of available rendering drivers.
      ///
      /// \exception rolmodl::sdlexception SDL reported an error code.
      ///
      /// \sa https://wiki.libsdl.org/SDL_GetNumRenderDrivers
      unsigned int count();
      /// \brief Query the driver info for the `i`th rendering driver.
      ///
      /// \exception std::out_of_range Driver index is out of range.
      ///
      /// \sa https://wiki.libsdl.org/SDL_GetRenderDriverInfo
      Info info(const unsigned int i);
    }
  }

  /// \brief Rendering scaling factors.
  /// \sa https://wiki.libsdl.org/SDL_RenderGetScale
  /// \sa https://wiki.libsdl.org/SDL_RenderSetScale
  struct RenScale {
    public:
      float x, y;

    private:
  };

  /// \brief Rectangle specifying the source rectangle for a rendering operation by its top left corner coordinates, width, and height.
  struct SrcRectWH : public geom::RectWH {
    using geom::RectWH::RectWH;
    constexpr SrcRectXY xy() const noexcept;
  };
  /// \brief Rectangle specifying the source rectangle for a rendering operation by its top left and bottom right corner coordinates.
  struct SrcRectXY : public geom::RectXY {
    using geom::RectXY::RectXY;
    constexpr SrcRectWH wh() const noexcept;
  };
  /// \brief Convert to an equivalent \link rolmodl::SrcRectXY \endlink.
  constexpr SrcRectXY SrcRectWH::xy() const noexcept {
    geom::RectXY tmp = geom::RectWH::xy();
    return SrcRectXY(tmp.x, tmp.y, tmp.x1, tmp.y1);
  }
  /// \brief Convert to an equivalent \link rolmodl::SrcRectWH \endlink.
  constexpr SrcRectWH SrcRectXY::wh() const noexcept {
    geom::RectWH tmp = geom::RectXY::wh();
    return SrcRectWH(tmp.x, tmp.y, tmp.w, tmp.h);
  }

  /// \brief Rectangle specifying the destination rectangle for a rendering operation by its top left corner coordinates, width, and height.
  struct DstRectWH : public geom::RectWH {
    using geom::RectWH::RectWH;
    constexpr DstRectXY xy() const noexcept;
  };
  /// \brief Rectangle specifying the destination rectangle for a rendering operation by its top left and bottom right corner coordinates.
  struct DstRectXY : public geom::RectXY {
    using geom::RectXY::RectXY;
    constexpr DstRectWH wh() const noexcept;
  };
  /// \brief Convert to an equivalent \link rolmodl::DstRectXY \endlink.
  constexpr DstRectXY DstRectWH::xy() const noexcept {
    geom::RectXY tmp = geom::RectWH::xy();
    return DstRectXY(tmp.x, tmp.y, tmp.x1, tmp.y1);
  }
  /// \brief Convert to an equivalent \link rolmodl::DstRectWH \endlink.
  constexpr DstRectWH DstRectXY::wh() const noexcept {
    geom::RectWH tmp = geom::RectXY::wh();
    return DstRectWH(tmp.x, tmp.y, tmp.w, tmp.h);
  }

  /// \brief Renderer class that does not support rendering to texture. Use \link rolmodl::TexRen \endlink for rendering to texture support.
  ///
  /// Wrapper around `SDL_Renderer`.
  ///
  /// The renderer managed by this class may actually support rendering to texture, based on the configuration with which it is created.
  /// However, it does not provide any methods to use this ability because no effort is made to ensure that the support was actually enabled.
  ///
  /// \sa https://wiki.libsdl.org/SDL_Renderer
  class Ren {
    public:
      // fixme: the index is unsafe here probably, i don't like that this api relies on SDL to catch out-of-bounds errors

      /// \brief Initialize a renderer for window `win`, using the `i`th rendering driver, with configuration `flags`.
      /// \exception rolmodl::sdlexception SDL reported an error.
      /// \sa https://wiki.libsdl.org/SDL_CreateRenderer
      Ren(Win& win, int i, ren::Flags flags);
      /// \brief Initialize a renderer for window `win`, with configuration `flags`, using the first rendering driver that supports it.
      /// \exception rolmodl::sdlexception SDL reported an error.
      /// \sa https://wiki.libsdl.org/SDL_CreateRenderer
      Ren(Win& win,        ren::Flags flags);
      /// \brief Initialize a renderer for window `win`, using the `i`th rendering driver, with the default configuration.
      /// \exception rolmodl::sdlexception SDL reported an error.
      /// \sa https://wiki.libsdl.org/SDL_CreateRenderer
      Ren(Win& win, int i);
      /// \brief Initialize a renderer for window `win`, with the default configuration, using the first rendering driver that supports it.
      /// \exception rolmodl::sdlexception SDL reported an error.
      /// \sa https://wiki.libsdl.org/SDL_CreateRenderer
      explicit Ren(Win& win);
      /// \brief Free the underlying `SDL_Renderer`.
      /// \sa https://wiki.libsdl.org/SDL_DestroyRenderer
      ~Ren() noexcept;

      /// \brief Copying rolmodl renderers is not allowed because their lifetime is tied to the underlying `SDL_Renderer`'s. Use references or some other pointer type instead.
      Ren(const Ren& that) = delete;
      /// \brief Take over the underlying `SDL_Renderer` of `that`.
      Ren(Ren&& that) noexcept;

      /// \brief Copying rolmodl renderers is not allowed because their lifetime is tied to the underlying `SDL_Renderer`'s. Use references or some other pointer type instead.
      Ren& operator=(const Ren& that) = delete;
      /// \brief Take over the underlying `SDL_Renderer` of `that`.
      Ren& operator=(Ren&& that) noexcept;

      /// \brief Exchange the underlying `SDL_Renderer` with `that`.
      friend void swap(Ren& a, Ren& b) noexcept;

      /// \brief Get the underlying `SDL_Renderer*`. Unsafe because this value might be nullptr and using it with some SDL functions can lead to rolmodl invariant violation.
      SDL_Renderer* unsafeRaw() noexcept;
      /// \brief Get the underlying `SDL_Renderer*` in a `const` context. Unsafe because this value might be nullptr and using it with some SDL functions can lead to rolmodl invariant violation.
      const SDL_Renderer* unsafeRaw() const noexcept;


      /// \brief Paint over the entire rendering target with the current color.
      /// \sa https://wiki.libsdl.org/SDL_RenderClear
      void clear();
      /// \brief Push the backbuffer to screen, displaying any changes made since the last `present`. This operation invalidates the rendering buffer and it must be cleared with \link rolmodl::Ren::clear \endlink.
      ///
      /// \todo How does this work with non-window rendering targets?
      ///
      /// \sa https://wiki.libsdl.org/SDL_RenderPresent
      void present() noexcept;

      // copy
      /// \brief Copy the entire texture `tex` to fill the entire rendering target.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopy
      void drawTex(Tex& tex);

      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the entire rendering target.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopy
      void drawTex(Tex& tex, const SrcRectWH src);
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the entire rendering target.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopy
      void drawTex(Tex& tex, const SrcRectXY src);

      /// \brief Copy the entire texture `tex` to fill the part of the rendering target specified by the destination rectangle `dst`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopy
      void drawTex(Tex& tex, const DstRectWH dst);
      /// \brief Copy the entire texture `tex` to fill the part of the rendering target specified by the destination rectangle `dst`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopy
      void drawTex(Tex& tex, const DstRectXY dst);

      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the part of the rendering target specified by the destination rectangle `dst`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopy
      void drawTex(Tex& tex, const SrcRectWH src, const DstRectWH dst);
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the part of the rendering target specified by the destination rectangle `dst`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopy
      void drawTex(Tex& tex, const SrcRectWH src, const DstRectXY dst);
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the part of the rendering target specified by the destination rectangle `dst`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopy
      void drawTex(Tex& tex, const SrcRectXY src, const DstRectWH dst);
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the part of the rendering target specified by the destination rectangle `dst`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopy
      void drawTex(Tex& tex, const SrcRectXY src, const DstRectXY dst);

      // copyEx
      /// \brief Copy the entire texture `tex` to fill the entire rendering target, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const Flip flip);
      /// \brief Copy the entire texture `tex` to fill the entire rendering target, rotating it by `rot` radian around its center, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const double rot, const Flip flip = Flip());
      /// \brief Copy the entire texture `tex` to fill the entire rendering target, rotating it by `rot` radian around `rotCenter`, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const double rot, const geom::Pos rotCenter, const Flip flip = Flip());

      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the entire rendering target, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectWH src, const Flip flip);
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the entire rendering target, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectXY src, const Flip flip);
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the entire rendering target, rotating it by `rot` radian around its center, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectWH src, const double rot, const Flip flip = Flip());
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the entire rendering target, rotating it by `rot` radian around its center, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectXY src, const double rot, const Flip flip = Flip());
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the entire rendering target, rotating it by `rot` radian around `rotCenter`, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectWH src, const double rot, const geom::Pos rotCenter, const Flip flip = Flip());
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the entire rendering target, rotating it by `rot` radian around `rotCenter`, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectXY src, const double rot, const geom::Pos rotCenter, const Flip flip = Flip());

      /// \brief Copy the entire texture `tex` to fill the part of the rendering target specified by the destination rectangle `dst`, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const DstRectWH dst, const Flip flip);
      /// \brief Copy the entire texture `tex` to fill the part of the rendering target specified by the destination rectangle `dst`, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const DstRectXY dst, const Flip flip);
      /// \brief Copy the entire texture `tex` to fill the part of the rendering target specified by the destination rectangle `dst`, rotating it by `rot` radian around its center, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const DstRectWH dst, const double rot, const Flip flip = Flip());
      /// \brief Copy the entire texture `tex` to fill the part of the rendering target specified by the destination rectangle `dst`, rotating it by `rot` radian around its center, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const DstRectXY dst, const double rot, const Flip flip = Flip());
      /// \brief Copy the entire texture `tex` to fill the part of the rendering target specified by the destination rectangle `dst`, rotating it by `rot` radian around `rotCenter`, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const DstRectWH dst, const double rot, const geom::Pos rotCenter, const Flip flip = Flip());
      /// \brief Copy the entire texture `tex` to fill the part of the rendering target specified by the destination rectangle `dst`, rotating it by `rot` radian around `rotCenter`, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const DstRectXY dst, const double rot, const geom::Pos rotCenter, const Flip flip = Flip());

      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the part of the rendering target specified by the destination rectangle `dst`, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectWH src, const DstRectWH dst, const Flip flip);
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the part of the rendering target specified by the destination rectangle `dst`, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectWH src, const DstRectXY dst, const Flip flip);
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the part of the rendering target specified by the destination rectangle `dst`, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectXY src, const DstRectWH dst, const Flip flip);
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the part of the rendering target specified by the destination rectangle `dst`, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectXY src, const DstRectXY dst, const Flip flip);
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the part of the rendering target specified by the destination rectangle `dst`, rotating it by `rot` radian around its center, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectWH src, const DstRectWH dst, const double rot, const Flip flip = Flip());
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the part of the rendering target specified by the destination rectangle `dst`, rotating it by `rot` radian around its center, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectWH src, const DstRectXY dst, const double rot, const Flip flip = Flip());
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the part of the rendering target specified by the destination rectangle `dst`, rotating it by `rot` radian around its center, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectXY src, const DstRectWH dst, const double rot, const Flip flip = Flip());
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the part of the rendering target specified by the destination rectangle `dst`, rotating it by `rot` radian around its center, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectXY src, const DstRectXY dst, const double rot, const Flip flip = Flip());
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the part of the rendering target specified by the destination rectangle `dst`, rotating it by `rot` radian around `rotCenter`, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectWH src, const DstRectWH dst, const double rot, const geom::Pos rotCenter, const Flip flip = Flip());
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the part of the rendering target specified by the destination rectangle `dst`, rotating it by `rot` radian around `rotCenter`, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectWH src, const DstRectXY dst, const double rot, const geom::Pos rotCenter, const Flip flip = Flip());
      /// \brief Copy the part of texture `tex` specified by the source rectangle `src` to fill the part of the rendering target specified by the destination rectangle `dst`, rotating it by `rot` radian around `rotCenter`, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectXY src, const DstRectWH dst, const double rot, const geom::Pos rotCenter, const Flip flip = Flip());
      /// \brief Copy the part of texture \p tex specified by the source rectangle `src` to fill the part of the rendering target specified by the destination rectangle `dst`, rotating it by `rot` radian around `rotCenter`, respecting the flipping configuration `flip`.
      /// \sa https://wiki.libsdl.org/SDL_RenderCopyEx
      void drawTex(Tex& tex, const SrcRectXY src, const DstRectXY dst, const double rot, const geom::Pos rotCenter, const Flip flip = Flip());

      /// \brief Draw a line from point `a` to point `b` with the drawing color.
      ///
      /// \todo
      /// needs ContigousIterators, for which no check can be done at compile-time
      /// also needs SDL_Point structs, or byte-equivalent
      ///   reinterpret_cast geom::Point?
      ///
      /// \sa https://wiki.libsdl.org/SDL_RenderDrawLine
      void drawLine(const geom::Pos a, const geom::Pos b);
      // void drawLines(?) noexcept;  // todo: implement this generically

      /// \brief Set the pixel at point `p` to the drawing color.
      /// \sa https://wiki.libsdl.org/SDL_RenderDrawPoint
      void drawPoint(const geom::Pos p);
      // void drawPoints(?) noexcept;  // todo: implement this generically

      /// \brief Draw the outline of the width-height rect `r` with the drawing color.
      /// \sa https://wiki.libsdl.org/SDL_RenderDrawRect
      void drawRect(const geom::RectWH r);
      /// \brief Draw the outline of the xy-rect rect `r` with the drawing color.
      /// \sa https://wiki.libsdl.org/SDL_RenderDrawRect
      void drawRect(const geom::RectXY r);
      // void drawRects(?) noexcept;  // todo: implement this generically

      /// \brief Fill the width-height rect `r` with the drawing color.
      /// \sa https://wiki.libsdl.org/SDL_RenderFillRect
      void fillRect(const geom::RectWH r);
      /// \brief Fill the xy-rect rect `r` with the drawing color.
      /// \sa https://wiki.libsdl.org/SDL_RenderFillRect
      void fillRect(const geom::RectXY r);
      // void fillRects(?);  // todo: implement this generically

      /// \brief Draw an outline around the rendering target with the drawing color.
      /// \todo Rename to outlineTarget
      /// \sa https://wiki.libsdl.org/SDL_RenderDrawRect
      void outlineScreen();
      /// \brief Fill the rendering target with the drawing color.
      /// \todo Rename to fillTarget
      /// \todo How is this functionally different from clear()?
      /// \sa https://wiki.libsdl.org/SDL_RenderFillRect
      void fillScreen();

      /// \brief Get the clipping rectangle. \link std::nullopt \endlink is returned if the clipping rectangle has not been set.
      /// \todo What is the difference between clipping being disabled and the rectangle not being set yet.
      /// \sa https://wiki.libsdl.org/SDL_RenderGetClipRect
      std::optional<geom::RectWH> clipRect() noexcept;
      /// \brief Set the clipping rectangle to `r`.
      /// \sa https://wiki.libsdl.org/SDL_RenderSetClipRect
      void setClipRect(const geom::RectWH r);
      /// \brief Set the clipping rectangle to `r`.
      /// \sa https://wiki.libsdl.org/SDL_RenderSetClipRect
      void setClipRect(const geom::RectXY r);
      /// \brief Unset the clipping rectangle.
      /// \sa https://wiki.libsdl.org/SDL_RenderSetClipRect
      void disableClip();
      /// \brief Query whether the clipping rectangle has been set.
      /// \todo What is the difference between clipping being disabled and the rectangle not being set yet.
      /// \todo not clear if false is an error state because the docs suggest "call `SDL_GetError()` for more information." if `SDL_FALSE` is returned.
      /// \sa https://wiki.libsdl.org/SDL_RenderIsClipEnabled
      bool isClipOn() noexcept;

      /// \brief Query the device-independent resolution.
      /// \todo Explain how to use this with resolution-independent rendering.
      /// \sa https://wiki.libsdl.org/SDL_RenderGetLogicalSize
      std::optional<geom::Size> logicalSize() noexcept;
      /// \brief Set the device-independent resolution to `s`.
      /// \todo Explain how to use this with resolution-independent rendering.
      /// \sa https://wiki.libsdl.org/SDL_RenderGetLogicalSize
      void setLogicalSize(const geom::Size s);

      // void setIntegralLogicalScale();

      /// \brief Query the target resolution in device pixels.
      /// \todo Explain how to use this with resolution-independent rendering.
      /// \sa https://wiki.libsdl.org/SDL_GetRendererOutputSize
      geom::Size getRealSize();

      /// \brief Query the rendering scaling factors.
      /// \todo Explain how to use this, including with resolution-independent rendering.
      /// \sa https://wiki.libsdl.org/SDL_RenderGetScale
      RenScale scale() noexcept;
      /// \brief Set the rendering scaling factors to `s`.
      /// \todo Explain how to use this, including with resolution-independent rendering.
      /// \sa https://wiki.libsdl.org/SDL_RenderSetScale
      void setScale(const RenScale s);

      /// \brief Query the subarea of the target used for drawing.
      /// \todo How does this work? Does it scale? Clip?
      /// \todo Explain how to use this, including with resolution-independent rendering.
      /// \sa https://wiki.libsdl.org/SDL_RenderGetViewport
      geom::RectWH viewport() noexcept;
      /// \brief Set the subarea of the target used for drawing to the rectangle `r`.
      /// \sa https://wiki.libsdl.org/SDL_RenderSetViewport
      void setViewport(const geom::RectWH r);

      /// \brief Get the blending mode used for `draw` operations. Textures have their own separate belnding mode.
      /// \todo Verify that textures indeed ignore this.
      /// \sa https://wiki.libsdl.org/SDL_SetRenderDrawBlendMode
      BlendMode getBlendMode();
      /// \brief Set the blending mode used for `draw` operations to `m`. Does not effect how textures are blended.
      /// \todo Verify that textures indeed ignore this.
      /// \sa https://wiki.libsdl.org/SDL_GetRenderDrawBlendMode
      void setBlendMode(const BlendMode m);

      /// \brief Query the current drawing color.
      /// \sa https://wiki.libsdl.org/SDL_GetRenderDrawColor
      RGBA color();
      /// \brief Set the drawing color to `c`.
      /// \sa https://wiki.libsdl.org/SDL_SetRenderDrawColor
      void setColor(const RGBA c);

    private:
      Ren() noexcept;

      SDL_Renderer* h_;
  };

  // fixme: if the renderer driver fails to create a renderer that supports target textures, this class is invalid
  /// \brief Renderer class that supports rendering to texture. Use \link rolmodl::Ren \endlink if you do not need support for rendering to texture.
  ///
  /// Wrapper around `SDL_Renderer`.
  ///
  /// The renderer managed by this class always supports rendering to texture, or the constructor will throw an \link rolmodl::sdlexception \endlink.
  ///
  /// \sa https://wiki.libsdl.org/SDL_Renderer
  class TexRen : public Ren { // todo: not all target-supporting renderers are TexRen
    public:
      /// \brief Initialize a renderer for window `win`, using the `i`th rendering driver, with a version of configuration `flags` that requires support for rendering to texture.
      /// \exception rolmodl::sdlexception SDL reported an error.
      /// \sa https://wiki.libsdl.org/SDL_CreateRenderer
      TexRen(Win& win, int i, ren::Flags flags);
      /// \brief Initialize a renderer for window `win`, with a version of configuration `flags` that requires support for rendering to texture, using the first rendering driver that supports it.
      /// \exception rolmodl::sdlexception SDL reported an error.
      /// \sa https://wiki.libsdl.org/SDL_CreateRenderer
      TexRen(Win& win,        ren::Flags flags);
      /// \brief Initialize a renderer for window `win`, using the `i`th rendering driver, with a version of the default configuration that requires support for rendering to texture.
      /// \exception rolmodl::sdlexception SDL reported an error.
      /// \sa https://wiki.libsdl.org/SDL_CreateRenderer
      TexRen(Win& win, int i);
      /// \brief Initialize a renderer for window `win`, with a version of the default configuration that requires support for rendering to texture, using the first rendering driver that supports it.
      /// \exception rolmodl::sdlexception SDL reported an error.
      /// \sa https://wiki.libsdl.org/SDL_CreateRenderer
      explicit TexRen(Win& win);

      /// \brief Set the rendering target.
      /// \sa https://wiki.libsdl.org/SDL_SetRenderTarget
      void setTarget(RenTex& tex);
      /// \brief Reset the rendering target to the default one. Usually it is the window for which the renderer was created.
      /// \todo What specifically is the default rendering target?
      /// \sa https://wiki.libsdl.org/SDL_SetRenderTarget
      void setDefaultTarget();

      /// \brief Query the current rendering target. Unsafe because it returns a raw non-owning pointer to the texture.
      /// \todo Implement a class that does this safely?
      /// \sa https://wiki.libsdl.org/SDL_GetRenderTarget
      SDL_Texture* unsafeGetTarget() noexcept;

    private:
  };
}
