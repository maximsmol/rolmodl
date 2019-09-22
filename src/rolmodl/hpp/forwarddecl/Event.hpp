#pragma once

#include <cstdint>
#include <variant>
#include <optional>

#include <SDL.h>

namespace rolmodl {
  enum class ButtonState;
  namespace button_state::unsafe {
    constexpr ButtonState fromSDLEnum(const uint8_t s) noexcept;
    constexpr uint8_t toSDLEnum(const ButtonState s) noexcept;
  }

  struct Keysym;

  struct HatState;
  namespace hatState::position {
  //   constexpr const char* toString(const HatState::Position p); // todo?
  }

  namespace event {
    struct Timestamped;
    struct WindowSpecific;

    struct Quit;
    struct User;

    struct Edit;
    struct Text;

    namespace touch {
      struct Motion;
      struct Up;
      struct Down;
    }

    namespace key {
      struct Up;
      struct Down;
    }

    namespace mouse {
      enum class WheelDirection;
      namespace wheel_direction::unsafe {
        constexpr WheelDirection fromSDLEnum(const uint32_t v) noexcept;
        constexpr uint32_t toSDLEnum(const WheelDirection v) noexcept;
      }

      namespace button {
        struct Up;
        struct Down;
      }
      struct Motion;
      struct Wheel;
    }

    namespace joystick {
      struct Axis;
      struct Ball;
      struct Hat;
      namespace button {
        struct Up;
        struct Down;
      }
      namespace device {
        struct Added;
        struct Removed;
      }
    }

    namespace controller {
      struct Axis;
      namespace button {
        struct Up;
        struct Down;
      }
      namespace device {
        struct Added;
        struct Removed;
        struct Remapped;
      }
    }

    namespace gesture {
      struct Builtin;
      namespace custom {
        struct Recorded;
        struct Detected;
      }
    }

    namespace window {
      struct Close;
      struct HitTest;

      struct Shown;
      struct Hidden;
      struct Exposed;

      struct Moved;
      struct Resized;
      struct SizeChanged;

      struct Minimized;
      struct Maximized;
      struct Restored;

      namespace mouse {
        struct Entered;
        struct Left;
      }

      namespace focus {
        struct Gained;
        struct Lost;
        struct Offered;
      }
    }
    struct SystemWindow;

    namespace drag_n_drop {
      struct File;
      struct Text;
      struct Begin;
      struct Complete;
    }

    namespace audio_device {
      struct Added;
      struct Removed;
    }
  }

  using Event = std::variant<
    event::Quit,
    event::User,

    event::Edit,
    event::Text,

    event::touch::Motion,
    event::touch::Up,
    event::touch::Down,

    event::key::Up,
    event::key::Down,

    event::mouse::button::Up,
    event::mouse::button::Down,
    event::mouse::Motion,
    event::mouse::Wheel,

    event::joystick::Axis,
    event::joystick::Ball,
    event::joystick::Hat,
    event::joystick::button::Up,
    event::joystick::button::Down,
    event::joystick::device::Added,
    event::joystick::device::Removed,

    event::controller::Axis,
    event::controller::button::Up,
    event::controller::button::Down,
    event::controller::device::Added,
    event::controller::device::Removed,
    event::controller::device::Remapped,

    event::gesture::Builtin,
    event::gesture::custom::Recorded,
    event::gesture::custom::Detected,

    // window
    event::window::Close,
    event::window::HitTest,

    event::window::Shown,
    event::window::Hidden,
    event::window::Exposed,

    event::window::Moved,
    event::window::Resized,
    event::window::SizeChanged,

    event::window::Minimized,
    event::window::Maximized,
    event::window::Restored,

    event::window::mouse::Entered,
    event::window::mouse::Left,

    event::window::focus::Gained,
    event::window::focus::Lost,
    event::window::focus::Offered,

    event::SystemWindow,
    //

    event::drag_n_drop::File,
    event::drag_n_drop::Text,
    event::drag_n_drop::Begin,
    event::drag_n_drop::Complete,

    event::audio_device::Added,
    event::audio_device::Removed>;
  namespace event {
    std::optional<Event> poll() noexcept;
  }
}
