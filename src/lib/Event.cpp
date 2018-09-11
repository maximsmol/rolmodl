#include "hpp/Event.hpp"

#include "hpp/Geom.hpp"

#include <cassert>
#include <algorithm>
#include <iterator>

namespace rolmodl::event {
  // fixme: only allow running in the main thread
  std::optional<Event> poll() noexcept {
    using std::variant;
    using std::copy;
    using std::begin;
    using std::end;

    using namespace geom;

    SDL_Event e;
    if (SDL_PollEvent(&e) == 0)
      return std::nullopt;

    if (e.type == SDL_QUIT)
      return Quit{{e.quit.timestamp}};
    // if (e.type == SDL_USER) // todo: any non-recognized is a user event? keep track of registered events?
    //   return User{
    //     {e.user.timestamp},
    //     {e.user.windowID},
    //     .code = e.user.code,
    //     .data1 = e.user.data1,
    //     .data2 = e.user.data2
    //   };

    if (e.type == SDL_TEXTEDITING) {
      Edit res{
        {e.edit.timestamp},
        {e.edit.windowID},
        .start = e.edit.start,
        .len = e.edit.length
      };
      copy(begin(e.edit.text), end(e.edit.text), begin(res.text));

      return res;
    }
    if (e.type == SDL_TEXTINPUT) {
      #pragma clang diagnostic push
      #pragma clang diagnostic ignored "-Wmissing-field-initializers"
      Text res{
        {e.text.timestamp},
        {e.text.windowID},
      };
      copy(begin(e.text.text), end(e.text.text), begin(res.text));
      #pragma clang diagnostic pop

      return res;
    }

    if (e.type == SDL_FINGERMOTION)
      return touch::Motion{
        {e.tfinger.timestamp},
        .touchId = e.tfinger.touchId,
        .fingerId = e.tfinger.fingerId,

        .pos = geom::XYFloats{e.tfinger.x, e.tfinger.y},
        .dpos = geom::XYFloats{e.tfinger.dx, e.tfinger.dy},
        .pressure = e.tfinger.pressure
      };
    if (e.type == SDL_FINGERUP)
      return touch::Up{
        {e.tfinger.timestamp},
        .touchId = e.tfinger.touchId,
        .fingerId = e.tfinger.fingerId,

        .pos = geom::XYFloats{e.tfinger.x, e.tfinger.y},
        .pressure = e.tfinger.pressure
      };
    if (e.type == SDL_FINGERDOWN)
      return touch::Down{
        {e.tfinger.timestamp},
        .touchId = e.tfinger.touchId,
        .fingerId = e.tfinger.fingerId,

        .pos = geom::XYFloats{e.tfinger.x, e.tfinger.y},
        .pressure = e.tfinger.pressure
      };

    if (e.type == SDL_KEYUP)
      return key::Up{
        {e.key.timestamp},
        {e.key.windowID},
        .state = button_state::unsafe::fromSDLEnum(e.key.state),
        .repeat = e.key.repeat != 0,
        .sym = e.key.keysym
      };
    if (e.type == SDL_KEYDOWN)
      return key::Down{
        {e.key.timestamp},
        {e.key.windowID},
        .state = button_state::unsafe::fromSDLEnum(e.key.state),
        .repeat = e.key.repeat != 0,
        .sym = e.key.keysym
      };

    if (e.type == SDL_MOUSEBUTTONUP)
      return mouse::button::Up{
        {e.button.timestamp},
        {e.button.windowID},
        .mouseId = e.button.which,
        .button = ::rolmodl::mouse::btn::unsafe::fromSDLEnum(e.button.button),
        .state = button_state::unsafe::fromSDLEnum(e.button.state),
        .clicks = e.button.clicks,
        .pos = geom::XYInt32{e.button.x, e.button.y}
      };
    if (e.type == SDL_MOUSEBUTTONDOWN)
      return mouse::button::Down{
        {e.button.timestamp},
        {e.button.windowID},
        .mouseId = e.button.which,
        .button = ::rolmodl::mouse::btn::unsafe::fromSDLEnum(e.button.button),
        .state = button_state::unsafe::fromSDLEnum(e.button.state),
        .clicks = e.button.clicks,
        .pos = geom::XYInt32{e.button.x, e.button.y}
      };
    if (e.type == SDL_MOUSEMOTION)
      return mouse::Motion{
        {e.motion.timestamp},
        {e.motion.windowID},
        .mouseId = e.motion.which,
        .state = ::rolmodl::mouse::BtnState(e.motion.state),
        .pos = geom::XYInt32{e.motion.x, e.motion.y},
        .dpos = geom::XYInt32{e.motion.xrel, e.motion.yrel}
      };
    if (e.type == SDL_MOUSEWHEEL)
      return mouse::Wheel{
        {e.wheel.timestamp},
        {e.wheel.windowID},
        .mouseId = e.wheel.which,
        .dpos = geom::XYInt32{e.wheel.x, e.wheel.y},
        .direction = mouse::wheel_direction::unsafe::fromSDLEnum(e.wheel.direction)
      };

    if (e.type == SDL_JOYAXISMOTION)
      return joystick::Axis{
        {e.jaxis.timestamp},
        .joystickId = e.jaxis.which,
        .axisN = e.jaxis.axis,
        .x = e.jaxis.value
      };
    if (e.type == SDL_JOYBALLMOTION)
      return joystick::Ball{
        {e.jball.timestamp},
        .joystickId = e.jball.which,
        .ballN = e.jball.ball,
        .dx = e.jball.xrel, .dy = e.jball.yrel
      };
    if (e.type == SDL_JOYHATMOTION)
      return joystick::Hat{
        {e.jhat.timestamp},
        .joystickId = e.jhat.which,
        .hatN = e.jhat.hat,
        .x = HatState(e.jhat.value)
      };
    if (e.type == SDL_JOYBUTTONUP)
      return joystick::button::Up{
        {e.jbutton.timestamp},
        .joystickId = e.jbutton.which,
        .buttonN = e.jbutton.button,
        .state = button_state::unsafe::fromSDLEnum(e.jbutton.state)
      };
    if (e.type == SDL_JOYBUTTONDOWN)
      return joystick::button::Down{
        {e.jbutton.timestamp},
        .joystickId = e.jbutton.which,
        .buttonN = e.jbutton.button,
        .state = button_state::unsafe::fromSDLEnum(e.jbutton.state)
      };
    if (e.type == SDL_JOYDEVICEADDED)
      return joystick::device::Added{
        {e.jdevice.timestamp},
        .joystickId = e.jdevice.which,
      };
    if (e.type == SDL_JOYDEVICEREMOVED)
      return joystick::device::Removed{
        {e.jdevice.timestamp},
        .joystickId = e.jdevice.which,
      };

    if (e.type == SDL_CONTROLLERAXISMOTION)
      return controller::Axis{
        {e.caxis.timestamp},
        .controllerId = e.caxis.which,
        .axisN = e.caxis.axis,
        .x = e.caxis.value
      };
    if (e.type == SDL_CONTROLLERBUTTONUP)
      return controller::button::Up{
        {e.cbutton.timestamp},
        .controllerId = e.cbutton.which,
        .buttonN = e.cbutton.button,
        .state = button_state::unsafe::fromSDLEnum(e.cbutton.state)
      };
    if (e.type == SDL_CONTROLLERBUTTONDOWN)
      return controller::button::Down{
        {e.cbutton.timestamp},
        .controllerId = e.cbutton.which,
        .buttonN = e.cbutton.button,
        .state = button_state::unsafe::fromSDLEnum(e.cbutton.state)
      };
    if (e.type == SDL_CONTROLLERDEVICEADDED)
      return controller::device::Added{
        {e.cdevice.timestamp},
        .controllerId = e.cdevice.which,
      };
    if (e.type == SDL_CONTROLLERDEVICEREMOVED)
      return controller::device::Removed{
        {e.cdevice.timestamp},
        .controllerId = e.cdevice.which,
      };
    if (e.type == SDL_CONTROLLERDEVICEREMAPPED)
      return controller::device::Remapped{
        {e.cdevice.timestamp},
        .controllerId = e.cdevice.which,
      };

    if (e.type == SDL_MULTIGESTURE)
      return gesture::Builtin{
        {e.mgesture.timestamp},
        .touchId = e.mgesture.touchId,

        .dRotation = e.mgesture.dTheta,
        .dPinch = e.mgesture.dDist,

        .nFingers = e.mgesture.numFingers,
        .pos = geom::XYFloats{e.mgesture.x, e.mgesture.y}
      };
    if (e.type == SDL_DOLLARRECORD)
      return gesture::custom::Recorded{
        {e.dgesture.timestamp},
        .touchId = e.dgesture.touchId,
        .gestureId = e.dgesture.gestureId,

        .nFingers = e.dgesture.numFingers,
        .error = e.dgesture.error,
        .pos = geom::XYFloats{e.mgesture.x, e.mgesture.y}
      };
    if (e.type == SDL_DOLLARGESTURE)
      return gesture::custom::Detected{
        {e.dgesture.timestamp},
        .touchId = e.dgesture.touchId,
        .gestureId = e.dgesture.gestureId,

        .nFingers = e.dgesture.numFingers,
        .error = e.dgesture.error,
        .pos = geom::XYFloats{e.mgesture.x, e.mgesture.y}
      };

    if (e.type == SDL_WINDOWEVENT) {
      if (e.window.event == SDL_WINDOWEVENT_CLOSE)
        return window::Close{{e.window.timestamp}, {e.window.windowID}};
      if (e.window.event == SDL_WINDOWEVENT_HIT_TEST)
        return window::HitTest{{e.window.timestamp}, {e.window.windowID}};

      if (e.window.event == SDL_WINDOWEVENT_SHOWN)
        return window::Shown{{e.window.timestamp}, {e.window.windowID}};
      if (e.window.event == SDL_WINDOWEVENT_HIDDEN)
        return window::Hidden{{e.window.timestamp}, {e.window.windowID}};
      if (e.window.event == SDL_WINDOWEVENT_EXPOSED)
        return window::Exposed{{e.window.timestamp}, {e.window.windowID}};

      if (e.window.event == SDL_WINDOWEVENT_MOVED)
        return window::Moved{
          {e.window.timestamp}, {e.window.windowID},
          .loc = Pos{static_cast<int>(e.window.data1), static_cast<int>(e.window.data2)} // todo: make sure we want to cast
        };
      if (e.window.event == SDL_WINDOWEVENT_RESIZED)
        return window::Resized{
          {e.window.timestamp}, {e.window.windowID},
          .size = Size{static_cast<int>(e.window.data1), static_cast<int>(e.window.data2)} // todo: make sure we want to cast
        };
      if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        return window::SizeChanged{
          {e.window.timestamp}, {e.window.windowID},
          .size = Size{static_cast<int>(e.window.data1), static_cast<int>(e.window.data2)} // todo: make sure we want to cast
        };

      if (e.window.event == SDL_WINDOWEVENT_MINIMIZED)
        return window::Minimized{{e.window.timestamp}, {e.window.windowID}};
      if (e.window.event == SDL_WINDOWEVENT_MAXIMIZED)
        return window::Maximized{{e.window.timestamp}, {e.window.windowID}};
      if (e.window.event == SDL_WINDOWEVENT_RESTORED)
        return window::Restored{{e.window.timestamp}, {e.window.windowID}};

      if (e.window.event == SDL_WINDOWEVENT_ENTER)
        return window::mouse::Entered{{e.window.timestamp}, {e.window.windowID}};
      if (e.window.event == SDL_WINDOWEVENT_LEAVE)
        return window::mouse::Left{{e.window.timestamp}, {e.window.windowID}};

      if (e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
        return window::focus::Gained{{e.window.timestamp}, {e.window.windowID}};
      if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
        return window::focus::Lost{{e.window.timestamp}, {e.window.windowID}};
      if (e.window.event == SDL_WINDOWEVENT_TAKE_FOCUS)
        return window::focus::Offered{{e.window.timestamp}, {e.window.windowID}};
    }

    if (e.type == SDL_SYSWMEVENT)
      return SystemWindow{
        {e.syswm.timestamp},
        .msg = e.syswm.msg
      };

    if (e.type == SDL_DROPFILE)
      return drag_n_drop::File{
        {e.drop.timestamp},
        {e.drop.windowID},
        .path = e.drop.file
      };
    if (e.type == SDL_DROPTEXT)
      return drag_n_drop::Text{
        {e.drop.timestamp},
        {e.drop.windowID},
        .x = e.drop.file
      };
    if (e.type == SDL_DROPBEGIN)
      return drag_n_drop::Begin{{e.drop.timestamp}, {e.drop.windowID}};
    if (e.type == SDL_DROPCOMPLETE)
      return drag_n_drop::Complete{{e.drop.timestamp}, {e.drop.windowID}};

    if (e.type == SDL_AUDIODEVICEADDED)
      return audio_device::Added{
        {e.adevice.timestamp},
        .index = e.adevice.which,
        .isCapture = e.adevice.iscapture != 0
      };
    if (e.type == SDL_AUDIODEVICEREMOVED)
      return audio_device::Removed{
        {e.adevice.timestamp},
        .id = e.adevice.which,
        .isCapture = e.adevice.iscapture != 0
      };

    return std::nullopt;
  }
}
