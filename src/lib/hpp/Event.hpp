#pragma once

#include <variant>
#include <optional>
#include <cstdint>

#include <SDL.h>

#include "Geom.hpp"
#include "Kb.hpp"

namespace rolmodl {
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

namespace rolmodl {
  namespace event {
    // struct Typed {
    //   uint32_t sdl_type;
    // };
    struct Timestamped {
      uint32_t timestamp;
    };
    using Base = Timestamped;
    // struct Base : public Timestamped {};

    struct WindowSpecific {
      uint32_t unsafe_winId; // todo
    };

    struct Quit : public Base {};

    // todo: type from SDL_RegisterEvents()
    struct User : public Base, WindowSpecific {
      int32_t code;
      void* data1;
      void* data2;
    };

    struct Edit : public Base, WindowSpecific {
      char text[32];
      int32_t start, len;
    };
    struct Text : public Base, WindowSpecific {
      char text[32];
    };

    namespace touch {
      struct Motion : public Base {
        SDL_TouchID touchId; // todo
        SDL_FingerID fingerId; // todo

        float x, y; // todo: add geom:: type
        float dx, dy; // todo: add geom:: type
        float pressure;
      };
      struct Up : public Base {
        SDL_TouchID touchId; // todo
        SDL_FingerID fingerId; // todo

        float x, y; // todo: add geom:: type
        float dx, dy; // todo: add geom:: type
        float pressure;
      };
      struct Down : public Base {
        SDL_TouchID touchId; // todo
        SDL_FingerID fingerId; // todo

        float x, y; // todo: add geom:: type
        float dx, dy; // todo: add geom:: type
        float pressure;
      };
    }

    namespace key {
      struct Up : public Base, WindowSpecific {
        uint8_t state; // todo: should be enum
        bool repeat;

        SDL_Keysym sym; // todo: support in Kb.hpp
      };
      struct Down : public Base, WindowSpecific {
        uint8_t state; // todo: should be enum
        bool repeat;

        SDL_Keysym sym; // todo: support in Kb.hpp
      };
    }

    namespace mouse {
      namespace button {
        struct Up : public Base, WindowSpecific {
          uint32_t mouseId; // todo: support SDL_TouchMouseID elegantly
          uint8_t button; // todo: should be enum
          uint8_t state; // todo: should be enum
          uint8_t clicks;

          int32_t x, y; // todo: add geom:: type
        };
        struct Down : public Base, WindowSpecific {
          uint32_t mouseId; // todo: support SDL_TouchMouseID elegantly
          uint8_t button; // todo: should be enum
          uint8_t state; // todo: should be enum
          uint8_t clicks;

          int32_t x, y; // todo: add geom:: type
        };
      }
      struct Motion : public Base, WindowSpecific {
        uint32_t mouseId; // todo: support SDL_TouchMouseID elegantly
        uint32_t state; // todo: support state elegantly

        int32_t x, y; // todo: add geom:: type
        int32_t dx, dy; // todo: add geom:: type
      };
      struct Wheel : public Base, WindowSpecific {
        uint32_t mouseId; // todo: support SDL_TouchMouseID elegantly

        int32_t dx, dy; // todo: add geom:: type
        uint32_t direction; // todo: support direction elegantly
      };
    }

    namespace joystick {
      struct Axis : public Base {
        SDL_JoystickID joystickId;
        uint8_t axisN;
        int16_t x;
      };
      struct Ball : public Base {
        SDL_JoystickID joystickId;
        uint8_t ballN;
        int16_t dx, dy;
      };
      struct Hat : public Base {
        SDL_JoystickID joystickId;
        uint8_t hatN;
        uint8_t x; // todo: should be enum
      };
      namespace button {
        struct Up : public Base {
          SDL_JoystickID joystickId;
          uint8_t buttonN;
          uint8_t state; // todo: should be enum
        };
        struct Down : public Base {
          SDL_JoystickID joystickId;
          uint8_t buttonN;
          uint8_t state; // todo: should be enum
        };
      }
      namespace device {
        struct Added : public Base {
          int32_t joystickId;
        };
        struct Removed : public Base {
          int32_t joystickId;
        };
      }
    }

    namespace controller {
      struct Axis : public Base {
        SDL_JoystickID controllerId;
        uint8_t axisN; // todo: should be enum
        int16_t x;
      };
      namespace button {
        struct Up : public Base {
          SDL_JoystickID controllerId;
          uint8_t buttonN; // todo: should be enum
          int16_t state; // todo: should be enum
        };
        struct Down : public Base {
          SDL_JoystickID controllerId;
          uint8_t buttonN; // todo: should be enum
          int16_t state; // todo: should be enum
        };
      }
      namespace device {
        struct Added : public Base {
          SDL_JoystickID controllerId;
        };
        struct Removed : public Base {
          SDL_JoystickID controllerId;
        };
        struct Remapped : public Base {
          SDL_JoystickID controllerId;
        };
      }
    }

    namespace gesture {
      struct Builtin : public Base {
        SDL_TouchID touchId; // todo

        float dRotation;
        float dPinch;
        float x, y; // todo: add geom:: type
        uint16_t nFingers;
      };
      namespace custom {
        struct Recorded : public Base {
          SDL_TouchID touchId; // todo
          SDL_GestureID gestureId; // todo

          uint32_t nFingers;
          float error;
          float x, y; // todo: add geom:: type
        };
        struct Detected : public Base {
          SDL_TouchID touchId; // todo
          SDL_GestureID gestureId; // todo

          uint32_t nFingers;
          float error;
          float x, y; // todo: add geom:: type
        };
      }
    }

    namespace window {
      struct Close : public Base, WindowSpecific {};
      struct HitTest : public Base, WindowSpecific {};

      struct Shown : public Base, WindowSpecific {};
      struct Hidden : public Base, WindowSpecific {};
      struct Exposed : public Base, WindowSpecific {};

      struct Moved : public Base, WindowSpecific {
        geom::Pos loc;
      };
      struct Resized : public Base, WindowSpecific {
        geom::Size size;
      };
      struct SizeChanged : public Base, WindowSpecific {
        geom::Size size;
      };

      struct Minimized : public Base, WindowSpecific {};
      struct Maximized : public Base, WindowSpecific {};
      struct Restored : public Base, WindowSpecific {};

      namespace mouse {
        struct Entered : public Base, WindowSpecific {};
        struct Left : public Base, WindowSpecific {};
      }

      namespace focus {
        struct Gained : public Base, WindowSpecific {};
        struct Lost : public Base, WindowSpecific {};
        struct Offered : public Base, WindowSpecific {};
      }
    }
    struct SystemWindow : public Base {
      SDL_SysWMmsg* msg; // todo?
    };

    namespace drag_n_drop {
      struct File : public Base, WindowSpecific {
        char* path; // todo: manage the resource. free with SDL_Free()
      };
      struct Text : public Base, WindowSpecific {
        char* x; // todo: manage the resource. free with SDL_Free()
      };
      struct Begin : public Base, WindowSpecific {};
      struct Complete : public Base, WindowSpecific {};
    }

    namespace audio_device {
      struct Added : public Base {
        uint32_t index; // todo
        bool isCapture;
      };
      struct Removed : public Base {
        uint32_t id; // todo
        bool isCapture;
      };
    }
  }
}
