#pragma once

#include "forwarddecl/Event.hpp"

#include "Base.hpp"
#include "Geom.hpp"
#include "Kb.hpp"
#include "Mouse.hpp"

namespace rolmodl {
  enum class ButtonState {
    Up, Down
  };
  namespace button_state::unsafe {
    constexpr ButtonState fromSDLEnum(const uint8_t s) noexcept {
      if (s == SDL_RELEASED)
        return ButtonState::Up;
      // if (s == SDL_PRESSED) // unsafe for a reason
        return ButtonState::Down;
    }
    constexpr uint8_t toSDLEnum(const ButtonState s) noexcept {
      if (s == ButtonState::Up)
        return SDL_RELEASED;
      // if (s == ButtonState::Down)
        return SDL_PRESSED;
    }
  }

  struct Keysym {
    public:
      kb::Key key;
      kb::Scancode scancode;
      uint16_t mod; // todo
    private:
  };

  struct HatState {
    public:
      enum class Position {
         TopLeft,    Top,    TopRight,
           Left,    Center,    Right,
        BottomLeft, Bottom, BottomRight
      };

      constexpr explicit HatState(const uint8_t v) noexcept : // fixme:: unsafe! arbitrary default if the SDL enum is invalid
        data_(unsafe_fromSDLEnum_(v))
      {}
      constexpr uint8_t unsafe_toSDLEnum() const {
        if (data_ == Position::TopLeft)
          return SDL_HAT_LEFTUP;
        if (data_ == Position::Top)
          return SDL_HAT_UP;
        if (data_ == Position::TopRight)
          return SDL_HAT_RIGHTUP;

        if (data_ == Position::Left)
          return SDL_HAT_LEFT;
        if (data_ == Position::Center)
          return SDL_HAT_CENTERED;
        if (data_ == Position::Right)
          return SDL_HAT_RIGHTUP;

        if (data_ == Position::BottomLeft)
          return SDL_HAT_LEFTDOWN;
        if (data_ == Position::Bottom)
          return SDL_HAT_DOWN;
        // if (data_ == Position::BottomRight)
          return SDL_HAT_RIGHTDOWN;
      }

      constexpr bool pointsLeft() const {
        return data_ == Position::TopLeft || data_ == Position::Left || data_ == Position::BottomLeft;
      }
      constexpr bool pointsRight() const {
        return data_ == Position::TopRight || data_ == Position::Right || data_ == Position::BottomRight;
      }

      constexpr bool pointsUp() const {
        return data_ == Position::TopLeft || data_ == Position::Top || data_ == Position::TopRight;
      }
      constexpr bool pointsDown() const {
        return data_ == Position::BottomLeft || data_ == Position::Bottom || data_ == Position::BottomRight;
      }

      constexpr Position pos() const {
        return data_;
      }

    private:
      Position data_;

      constexpr Position unsafe_fromSDLEnum_(const uint8_t v) noexcept {
        if (v == SDL_HAT_LEFTUP)
          return Position::TopLeft;
        else if (v == SDL_HAT_UP)
          return Position::Top;
        else if (v == SDL_HAT_RIGHTUP)
          return Position::TopRight;

        else if (v == SDL_HAT_LEFT)
          return Position::Left;
        else if (v == SDL_HAT_CENTERED)
          return Position::Center;
        else if (v == SDL_HAT_RIGHT)
          return Position::Right;

        else if (v == SDL_HAT_LEFTDOWN)
          return Position::BottomLeft;
        else if (v == SDL_HAT_DOWN)
          return Position::Bottom;
        // else if (v == SDL_HAT_RIGHTDOWN)
          return Position::BottomRight;
      }
  };
  namespace hatState::position {
    constexpr const char* toString(const HatState::Position p) {
      if (p == HatState::Position::TopLeft)
        return "top-left";
      if (p == HatState::Position::Top)
        return "top";
      if (p == HatState::Position::TopRight)
        return "top-right";

      if (p == HatState::Position::Left)
        return "left";
      if (p == HatState::Position::Center)
        return "center";
      if (p == HatState::Position::Right)
        return "right";

      if (p == HatState::Position::BottomLeft)
        return "bottom-left";
      if (p == HatState::Position::Bottom)
        return "bottom";
      // if (p == HatState::Position::BottomRight)
        return "bottom-right";
    }
  }

  namespace event {
    struct Timestamped {
      uint32_t timestamp;
    };

    struct WindowSpecific {
      uint32_t unsafe_winId;
    };

    struct Quit : public Timestamped {};

    // todo: type from SDL_RegisterEvents()
    struct User : public Timestamped, WindowSpecific {
      int32_t code;
      void* data1;
      void* data2;
    };

    struct Edit : public Timestamped, WindowSpecific {
      char text[32];
      int32_t start, len;
    };
    struct Text : public Timestamped, WindowSpecific {
      char text[32];
    };

    namespace touch {
      struct Motion : public Timestamped {
        SDL_TouchID touchId; // todo
        SDL_FingerID fingerId; // todo

        geom::XYFloats pos;
        geom::XYFloats dpos;
        float pressure;
      };
      struct Up : public Timestamped {
        SDL_TouchID touchId; // todo
        SDL_FingerID fingerId; // todo

        geom::XYFloats pos;
        // geom::XYFloats dpos; // todo: is this always (0, 0)?
        float pressure;
      };
      struct Down : public Timestamped {
        SDL_TouchID touchId; // todo
        SDL_FingerID fingerId; // todo

        geom::XYFloats pos;
        // geom::XYFloats dpos; // todo: is this always (0, 0)?
        float pressure;
      };
    }

    namespace key {
      struct Up : public Timestamped, WindowSpecific {
        ButtonState state;
        bool repeat;

        Keysym sym;
      };
      struct Down : public Timestamped, WindowSpecific {
        ButtonState state;
        bool repeat;

        Keysym sym;
      };
    }

    namespace mouse {
      enum class WheelDirection {
        Normal, Reverse
      };
      namespace wheel_direction::unsafe {
        constexpr WheelDirection fromSDLEnum(const uint32_t v) noexcept {
          if (v == SDL_MOUSEWHEEL_NORMAL)
            return WheelDirection::Normal;
          // if (v == SDL_MOUSEWHEEL_FLIPPED) // unsafe for a reason
            return WheelDirection::Reverse;
        }
        constexpr uint32_t toSDLEnum(const WheelDirection v) noexcept {
          if (v == WheelDirection::Normal)
            return SDL_MOUSEWHEEL_NORMAL;
          // if (v == WheelDirection::Reverse)
            return SDL_MOUSEWHEEL_FLIPPED;
        }
      }

      namespace button {
        struct Up : public Timestamped, WindowSpecific {
          uint32_t mouseId; // todo: support SDL_TouchMouseID elegantly
          ::rolmodl::mouse::Btn button;
          ButtonState state;
          uint8_t clicks;

          geom::XYInt32 pos;
        };
        struct Down : public Timestamped, WindowSpecific {
          uint32_t mouseId; // todo: support SDL_TouchMouseID elegantly
          ::rolmodl::mouse::Btn button;
          ButtonState state;
          uint8_t clicks;

          geom::XYInt32 pos;
        };
      }
      struct Motion : public Timestamped, WindowSpecific {
        uint32_t mouseId; // todo: support SDL_TouchMouseID elegantly
        ::rolmodl::mouse::BtnState state;

        geom::XYInt32 pos;
        geom::XYInt32 dpos;
      };
      struct Wheel : public Timestamped, WindowSpecific {
        uint32_t mouseId; // todo: support SDL_TouchMouseID elegantly

        geom::XYInt32 dpos;
        WheelDirection direction;
      };
    }

    namespace joystick {
      struct Axis : public Timestamped {
        SDL_JoystickID joystickId; // todo
        uint8_t axisN;
        int16_t x;
      };
      struct Ball : public Timestamped {
        SDL_JoystickID joystickId; // todo
        uint8_t ballN;
        int16_t dx, dy;
      };
      struct Hat : public Timestamped {
        SDL_JoystickID joystickId; // todo
        uint8_t hatN;
        HatState x;
      };
      namespace button {
        struct Up : public Timestamped {
          SDL_JoystickID joystickId; // todo
          uint8_t buttonN;
          ButtonState state;
        };
        struct Down : public Timestamped {
          SDL_JoystickID joystickId; // todo
          uint8_t buttonN;
          ButtonState state;
        };
      }
      namespace device {
        struct Added : public Timestamped {
          int32_t joystickId; // todo
        };
        struct Removed : public Timestamped {
          int32_t joystickId; // todo
        };
      }
    }

    namespace controller {
      struct Axis : public Timestamped {
        SDL_JoystickID controllerId; // todo
        uint8_t axisN; // todo: should be enum
        int16_t x;
      };
      namespace button {
        struct Up : public Timestamped {
          SDL_JoystickID controllerId; // todo
          uint8_t buttonN; // todo: should be enum
          ButtonState state;
        };
        struct Down : public Timestamped {
          SDL_JoystickID controllerId; // todo
          uint8_t buttonN; // todo: should be enum
          ButtonState state;
        };
      }
      namespace device {
        struct Added : public Timestamped {
          SDL_JoystickID controllerId; // todo
        };
        struct Removed : public Timestamped {
          SDL_JoystickID controllerId; // todo
        };
        struct Remapped : public Timestamped {
          SDL_JoystickID controllerId; // todo
        };
      }
    }

    namespace gesture {
      struct Builtin : public Timestamped {
        SDL_TouchID touchId; // todo

        float dRotation;
        float dPinch;

        uint16_t nFingers;
        geom::XYFloats pos;
      };
      namespace custom {
        struct Recorded : public Timestamped {
          SDL_TouchID touchId; // todo
          SDL_GestureID gestureId; // todo

          uint32_t nFingers;
          float error;
          geom::XYFloats pos;
        };
        struct Detected : public Timestamped {
          SDL_TouchID touchId; // todo
          SDL_GestureID gestureId; // todo

          uint32_t nFingers;
          float error;
          geom::XYFloats pos;
        };
      }
    }

    namespace window {
      struct Close : public Timestamped, WindowSpecific {};
      struct HitTest : public Timestamped, WindowSpecific {};

      struct Shown : public Timestamped, WindowSpecific {};
      struct Hidden : public Timestamped, WindowSpecific {};
      struct Exposed : public Timestamped, WindowSpecific {};

      struct Moved : public Timestamped, WindowSpecific {
        geom::Pos loc;
      };
      struct Resized : public Timestamped, WindowSpecific {
        geom::Size size;
      };
      struct SizeChanged : public Timestamped, WindowSpecific {
        geom::Size size;
      };

      struct Minimized : public Timestamped, WindowSpecific {};
      struct Maximized : public Timestamped, WindowSpecific {};
      struct Restored : public Timestamped, WindowSpecific {};

      namespace mouse {
        struct Entered : public Timestamped, WindowSpecific {};
        struct Left : public Timestamped, WindowSpecific {};
      }

      namespace focus {
        struct Gained : public Timestamped, WindowSpecific {};
        struct Lost : public Timestamped, WindowSpecific {};
        struct Offered : public Timestamped, WindowSpecific {};
      }
    }
    struct SystemWindow : public Timestamped {
      SDL_SysWMmsg* msg; // todo?
    };

    namespace drag_n_drop {
      struct File : public Timestamped, WindowSpecific {
        SDLString path;
      };
      struct Text : public Timestamped, WindowSpecific {
        SDLString x;
      };
      struct Begin : public Timestamped, WindowSpecific {};
      struct Complete : public Timestamped, WindowSpecific {};
    }

    namespace audio_device {
      struct Added : public Timestamped {
        uint32_t index; // todo
        bool isCapture;
      };
      struct Removed : public Timestamped {
        uint32_t id; // todo
        bool isCapture;
      };
    }
  }
}
