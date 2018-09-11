#include <cstdio>
#include <optional>
#include <variant>
#include <functional>

#include "../lib/hpp/Rolmodl.hpp"

using namespace std;
using namespace rolmodl;
using namespace geom;

template<class T, class... Types>
void varact(function<void(T)> f, variant<Types...> v) {
  if (!holds_alternative<T>(v))
    return;

  f(get<T>(v));
}

template<class R, class T, class... Types>
optional<R> varact(function<R(T)> f, variant<Types...> v) {
  if (!holds_alternative<T>(v))
    return nullopt;

  return f(get<T>(v));
}

static mouse::Cursor* hand = nullptr;
struct Button {
  public:
    Button(const Pos p, const Size s) :
      pos(p), size(s),
      col_(RGBA{100, 100, 100}),
      state_(0)
    {}

    void render(Ren& r) {
      RGBA oldC = r.color();

      r.setColor(col_);
      r.drawLine(pos, Pos{pos.x+size.w, pos.y});
      r.drawLine(pos, Pos{pos.x,        pos.y+size.h});

      r.setColor(RGBA{static_cast<uint8_t>(col_.r+100), static_cast<uint8_t>(col_.g+100), static_cast<uint8_t>(col_.b+100)});
      r.drawLine(Pos{pos.x+size.w, pos.y+size.h}, Pos{pos.x+size.w, pos.y});
      r.drawLine(Pos{pos.x+size.w, pos.y+size.h}, Pos{pos.x,        pos.y+size.h});

      r.setColor(oldC);
    }
    void tick(mouse::State ms) {
      Pos mp = ms.pos();

      if (mp.x < pos.x        || mp.y < pos.y ||
          mp.x > pos.x+size.w || mp.y > pos.y+size.h) {
        if (state_ == 0)
          return;

        col_ = RGBA{100, 100, 100};
        mouse::cursor::useDefault();
        state_ = 0;
        return;
      }

      hand->use();
      if (ms.btnState().l()) {
        if (state_ == 2)
          return;
        col_ = RGBA{100, 200, 100};
        state_ = 2;
      }
      else {
        if (state_ == 1)
          return;
        col_ = RGBA{200, 100, 100};
        state_ = 1;
      }
    }

    Pos pos;
    Size size;
  private:
    RGBA col_;
    int state_; // 0 - def, 1 - hover, 2 - click
};

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);

  mouse::Cursor hand1 = mouse::cursor::system::hand();
  hand = &hand1;

  printf("sys info\n");
  printf("name: %s\n", sys::name());
  printf("id: %d\n", sys::id());
  printf("ram: %dMiB ~ %d GiB\n", sys::ram(), sys::ram()/1024);
  printf("logical cores: %d\n", sys::logicalCores());
  printf("cpu:\n");
  printf("  l1 size: %d\n", sys::cpu::l1Size());
  printf("  feature support:\n");
  printf("    3DNow!  - %s\n", sys::cpu::has3DNow()   ? "ok" : "na");
  printf("    AVX     - %s\n", sys::cpu::hasAVX()     ? "ok" : "na");
  printf("    AVX2    - %s\n", sys::cpu::hasAVX2()    ? "ok" : "na");
  printf("    AltiVec - %s\n", sys::cpu::hasAltiVec() ? "ok" : "na");
  printf("    MMX     - %s\n", sys::cpu::hasMMX()     ? "ok" : "na");
  printf("    RDTSC   - %s\n", sys::cpu::hasRDTSC()   ? "ok" : "na");
  printf("    SSE     - %s\n", sys::cpu::hasSSE()     ? "ok" : "na");
  printf("    SSE2    - %s\n", sys::cpu::hasSSE2()    ? "ok" : "na");
  printf("    SSE3    - %s\n", sys::cpu::hasSSE3()    ? "ok" : "na");
  printf("    SSE41   - %s\n", sys::cpu::hasSSE41()   ? "ok" : "na");
  printf("    SSE42   - %s\n", sys::cpu::hasSSE42()   ? "ok" : "na");
  printf("screensaver: %s\n", sys::screensaver::enabled() ? "on" : "off");
  printf("\n");

  printf("driver:\n");
  if (auto drvr = sys::driver::current(); drvr.has_value())
    printf("  current: %s\n", *drvr);
  else
    printf("  not init\n");
  printf("  num: %d\n", sys::driver::count());
  printf("  available:\n");
  for (unsigned int i = 0; i < sys::driver::count(); ++i)
    printf("    %s\n", sys::driver::name(i));
  printf("\n");

  printf("displays:\n");
  printf("  num: %d\n", sys::display::unsafe::count());
  for (unsigned int i = 0; i < sys::display::unsafe::count(); ++i) {
    sys::Display d = sys::display::unsafe::byIndex(i);
    RectWH r = d.bounds();
    RectWH rUsable = d.usableBounds();
    printf("  %s:\n", d.name());
    printf("    all   : at (%d,%d), %d x %d\n", r.x, r.y, r.w, r.h);
    printf("    usable: at (%d,%d), %d x %d\n", rUsable.x, rUsable.y, rUsable.w, rUsable.h);
    printf("    %f ddpi, %f hdpi, %f vdpi\n", static_cast<double>(d.ddpi()), static_cast<double>(d.hdpi()), static_cast<double>(d.vdpi()));
  }
  printf("\n");

  sys::pwr::Status pwrStatus{};
  printf("pwr info\n");
  printf("time: ");
  if (auto time = pwrStatus.time(); time.has_value())
    printf("%d\n", *time);
  else
    printf("na\n");
  printf("%%: ");
  if (auto pct = pwrStatus.percentage(); pct)
    printf("%d\n", *pct);
  else
    printf("na\n");
  printf("state id: %d\n", pwrStatus.state());

  Win w("test", Size{800, 600}, win::Flags{}.withResizable());
  Ren r(w);

  bool running = true;

  const int btnDim = 30;
  Button b(Pos{100, 100}, Size{btnDim, btnDim});
  Button b1(Pos{100+2*btnDim, 100}, Size{btnDim, btnDim});
  Button b2(Pos{100+4*btnDim, 100}, Size{btnDim, btnDim});
  while (running) {
    while (true) {
      optional polled = event::poll();
      if (!polled)
        break;

      Event e = polled.value();
      varact<event::Quit>([&](event::Quit x) {
        printf("%6d: Quit\n", x.timestamp);
        running = false;
      }, e);

      varact<event::Edit>([&](event::Edit x) {
        printf("%6d: Edit\n\tunsafe_winId: %d\n\ttext: %s\n\tstart: %d len: %d\n", x.timestamp, x.unsafe_winId, x.text, x.start, x.len);
      }, e);
      varact<event::Text>([&](event::Text x) {
        printf("%6d: Text\n\tunsafe_winId: %d\n\ttext: %s\n", x.timestamp, x.unsafe_winId, x.text);
      }, e);

      varact<event::touch::Motion>([&](event::touch::Motion x) {
        // printf("%6d: Touch.motion\n\ttouchId: %lld\n\tfingerId: %lld\n\n\tx: %f y: %f\n\tdx: %f dy: %f\n\tpressure: %f\n", x.timestamp, x.touchId, x.fingerId, static_cast<double>(x.x), static_cast<double>(x.y), static_cast<double>(x.dx), static_cast<double>(x.dy), static_cast<double>(x.pressure));
      }, e);
      varact<event::touch::Up>([&](event::touch::Up x) {
        printf("%6d: Touch.up\n\ttouchId: %lld\n\tfingerId: %lld\n\n\tx: %f y: %f\n\tpressure: %f\n", x.timestamp, x.touchId, x.fingerId, static_cast<double>(x.pos.x), static_cast<double>(x.pos.y), static_cast<double>(x.pressure));
      }, e);
      varact<event::touch::Down>([&](event::touch::Down x) {
        printf("%6d: Touch.down\n\ttouchId: %lld\n\tfingerId: %lld\n\n\tx: %f y: %f\n\tpressure: %f\n", x.timestamp, x.touchId, x.fingerId, static_cast<double>(x.pos.x), static_cast<double>(x.pos.y), static_cast<double>(x.pressure));
      }, e);

      varact<event::key::Up>([&](event::key::Up x) {
        printf("%6d: Key.up\n\tunsafe_winId: %d\n\tstate: %d repeat: %s\n\tsym: %d\n", x.timestamp, x.unsafe_winId, x.state, x.repeat ? "true" : "false", x.sym.sym);
      }, e);
      varact<event::key::Down>([&](event::key::Down x) {
        printf("%6d: Key.down\n\tunsafe_winId: %d\n\tstate: %d repeat: %s\n\tsym: %d\n", x.timestamp, x.unsafe_winId, x.state, x.repeat ? "true" : "false", x.sym.sym);
      }, e);

      varact<event::mouse::button::Up>([&](event::mouse::button::Up x) {
        printf("%6d: Mouse.button.up\n\tunsafe_winId: %d\n\tmouseId: %d\n\tbutton: %d state: %d clicks: %d\n\tx: %d y: %d\n", x.timestamp, x.unsafe_winId, x.mouseId, x.button, x.state, x.clicks, x.pos.x, x.pos.y);
      }, e);
      varact<event::mouse::button::Down>([&](event::mouse::button::Down x) {
        printf("%6d: Mouse.button.down\n\tunsafe_winId: %d\n\tmouseId: %d\n\tbutton: %d state: %d clicks: %d\n\tx: %d y: %d\n", x.timestamp, x.unsafe_winId, x.mouseId, x.button, x.state, x.clicks, x.pos.x, x.pos.y);
      }, e);
      varact<event::mouse::Motion>([&](event::mouse::Motion x) {
        // printf("%6d: Mouse.motion\n\tunsafe_winId: %d\n\tmouseId: %d\n\tstate: %d\n\tx: %d y: %d\n\tdx: %d dy: %d\n", x.timestamp, x.unsafe_winId, x.mouseId, x.state, x.pos.x, x.pos.y, x.dpos.x, x.dpos.y);
      }, e);
      varact<event::mouse::Wheel>([&](event::mouse::Wheel x) {
        printf("%6d: Mouse.wheel\n\tunsafe_winId: %d\n\tmouseId: %d\n\tdx: %d dy: %d\n\tdirection: %d\n", x.timestamp, x.unsafe_winId, x.mouseId, x.dpos.x, x.dpos.y, x.direction);
      }, e);

      varact<event::joystick::Axis>([&](event::joystick::Axis x) {
        // printf("%6d: Joystick.axis\n\tjoystickId: %d\n\taxisN: %d x: %d\n", x.timestamp, x.joystickId, x.axisN, x.x);
      }, e);
      varact<event::joystick::Ball>([&](event::joystick::Ball x) {
        printf("%6d: Joystick.ball\n\tjoystickId: %d\n\tballN: %d\n\tdx: %d dy: %d\n", x.timestamp, x.joystickId, x.ballN, x.dx, x.dy);
      }, e);
      varact<event::joystick::Hat>([&](event::joystick::Hat x) {
        printf("%6d: Joystick.hat\n\tjoystickId: %d\n\thatN: %d x: %d\n", x.timestamp, x.joystickId, x.hatN, x.x);
      }, e);
      varact<event::joystick::button::Up>([&](event::joystick::button::Up x) {
        printf("%6d: Joystick.button.up\n\tjoystickId: %d\n\tbuttonN: %d state: %d\n", x.timestamp, x.joystickId, x.buttonN, x.state);
      }, e);
      varact<event::joystick::button::Down>([&](event::joystick::button::Down x) {
        printf("%6d: Joystick.button.down\n\tjoystickId: %d\n\tbuttonN: %d state: %d\n", x.timestamp, x.joystickId, x.buttonN, x.state);
      }, e);
      varact<event::joystick::device::Added>([&](event::joystick::device::Added x) {
        SDL_JoystickOpen(x.joystickId);
        printf("%6d: Joystick.device.added\n\tjoystickId: %d\n", x.timestamp, x.joystickId);
      }, e);
      varact<event::joystick::device::Removed>([&](event::joystick::device::Removed x) {
        printf("%6d: Joystick.device.removed\n\tjoystickId: %d\n", x.timestamp, x.joystickId);
      }, e);

      varact<event::controller::Axis>([&](event::controller::Axis x) {
        // printf("%6d: Controller.axis\n\tcontrollerId: %d\n\taxisN: %d x: %d\n", x.timestamp, x.controllerId, x.axisN, x.x);
      }, e);
      varact<event::controller::button::Up>([&](event::controller::button::Up x) {
        printf("%6d: Controller.button.up\n\tcontrollerId: %d\n\tbuttonN: %d state: %d\n", x.timestamp, x.controllerId, x.buttonN, x.state);
      }, e);
      varact<event::controller::button::Down>([&](event::controller::button::Down x) {
        printf("%6d: Controller.button.down\n\tcontrollerId: %d\n\tbuttonN: %d state: %d\n", x.timestamp, x.controllerId, x.buttonN, x.state);
      }, e);
      varact<event::controller::device::Added>([&](event::controller::device::Added x) {
        SDL_GameControllerOpen(x.controllerId);
        printf("%6d: Controller.device.added\n\tcontrollerId: %d\n", x.timestamp, x.controllerId);
      }, e);
      varact<event::controller::device::Removed>([&](event::controller::device::Removed x) {
        printf("%6d: Controller.device.removed\n\tcontrollerId: %d\n", x.timestamp, x.controllerId);
      }, e);
      varact<event::controller::device::Remapped>([&](event::controller::device::Remapped x) {
        printf("%6d: Controller.device.remapped\n\tcontrollerId: %d\n", x.timestamp, x.controllerId);
      }, e);

      varact<event::gesture::Builtin>([&](event::gesture::Builtin x) {
        printf("%6d: Gesture.builtin\n\ttouchId: %lld\n\tnFingers: %d dRotation: %f dPinch: %f\n\tx: %f y: %f\n", x.timestamp, x.touchId, x.nFingers, static_cast<double>(x.dRotation), static_cast<double>(x.dPinch), static_cast<double>(x.pos.x), static_cast<double>(x.pos.y));
      }, e);
      varact<event::gesture::custom::Recorded>([&](event::gesture::custom::Recorded x) {
        printf("%6d: Gesture.custom.recorded\n\ttouchId: %lld gestureId: %lld\n\tnFingers: %d error: %f\n\tx: %f y: %f\n", x.timestamp, x.touchId, x.gestureId, x.nFingers, static_cast<double>(x.error), static_cast<double>(x.pos.x), static_cast<double>(x.pos.y));
      }, e);
      varact<event::gesture::custom::Detected>([&](event::gesture::custom::Detected x) {
        printf("%6d: Gesture.custom.detected\n\ttouchId: %lld gestureId: %lld\n\tnFingers: %d error: %f\n\tx: %f y: %f\n", x.timestamp, x.touchId, x.gestureId, x.nFingers, static_cast<double>(x.error), static_cast<double>(x.pos.x), static_cast<double>(x.pos.y));
      }, e);

      varact<event::window::Close>([&](event::window::Close x) {
        printf("%6d: Window.close\n\tunsafe_winId: %d\n", x.timestamp, x.unsafe_winId);
      }, e);
      varact<event::window::HitTest>([&](event::window::HitTest x) {
        printf("%6d: Window.hittest\n\tunsafe_winId: %d\n", x.timestamp, x.unsafe_winId);
      }, e);

      varact<event::window::Shown>([&](event::window::Shown x) {
        printf("%6d: Window.shown\n\tunsafe_winId: %d\n", x.timestamp, x.unsafe_winId);
      }, e);
      varact<event::window::Hidden>([&](event::window::Hidden x) {
        printf("%6d: Window.hidden\n\tunsafe_winId: %d\n", x.timestamp, x.unsafe_winId);
      }, e);
      varact<event::window::Exposed>([&](event::window::Exposed x) {
        printf("%6d: Window.exposed\n\tunsafe_winId: %d\n", x.timestamp, x.unsafe_winId);
      }, e);

      varact<event::window::Moved>([&](event::window::Moved x) {
        printf("%6d: Window.moved\n\tunsafe_winId: %d\n\tx: %d y: %d\n", x.timestamp, x.unsafe_winId, x.loc.x, x.loc.y);
      }, e);
      varact<event::window::Resized>([&](event::window::Resized x) {
        printf("%6d: Window.resized\n\tunsafe_winId: %d\n\tw: %d h: %d\n", x.timestamp, x.unsafe_winId, x.size.w, x.size.h);
      }, e);
      varact<event::window::SizeChanged>([&](event::window::SizeChanged x) {
        printf("%6d: Window.sizechanged\n\tunsafe_winId: %d\n\tw: %d h: %d\n", x.timestamp, x.unsafe_winId, x.size.w, x.size.h);
      }, e);

      varact<event::window::Minimized>([&](event::window::Minimized x) {
        printf("%6d: Window.minimized\n\tunsafe_winId: %d\n", x.timestamp, x.unsafe_winId);
      }, e);
      varact<event::window::Maximized>([&](event::window::Maximized x) {
        printf("%6d: Window.maximized\n\tunsafe_winId: %d\n", x.timestamp, x.unsafe_winId);
      }, e);
      varact<event::window::Restored>([&](event::window::Restored x) {
        printf("%6d: Window.restored\n\tunsafe_winId: %d\n", x.timestamp, x.unsafe_winId);
      }, e);

      varact<event::window::mouse::Entered>([&](event::window::mouse::Entered x) {
        printf("%6d: Window.mouse.entered\n\tunsafe_winId: %d\n", x.timestamp, x.unsafe_winId);
      }, e);
      varact<event::window::mouse::Left>([&](event::window::mouse::Left x) {
        printf("%6d: Window.mouse.left\n\tunsafe_winId: %d\n", x.timestamp, x.unsafe_winId);
      }, e);

      varact<event::window::focus::Gained>([&](event::window::focus::Gained x) {
        printf("%6d: Window.focus.gained\n\tunsafe_winId: %d\n", x.timestamp, x.unsafe_winId);
      }, e);
      varact<event::window::focus::Lost>([&](event::window::focus::Lost x) {
        printf("%6d: Window.focus.lost\n\tunsafe_winId: %d\n", x.timestamp, x.unsafe_winId);
      }, e);
      varact<event::window::focus::Offered>([&](event::window::focus::Offered x) {
        printf("%6d: Window.focus.offered\n\tunsafe_winId: %d\n", x.timestamp, x.unsafe_winId);
      }, e);

      varact<event::drag_n_drop::File>([&](event::drag_n_drop::File x) {
        printf("%6d: Dragndrop.file\n\tunsafe_winId: %d\n\tpath: %s\n", x.timestamp, x.unsafe_winId, x.path);
      }, e);
      varact<event::drag_n_drop::Text>([&](event::drag_n_drop::Text x) {
        printf("%6d: Dragndrop.text\n\tunsafe_winId: %d\n\tstr: %s\n", x.timestamp, x.unsafe_winId, x.x);
      }, e);
      varact<event::drag_n_drop::Begin>([&](event::drag_n_drop::Begin x) {
        printf("%6d: Dragndrop.begin\n\tunsafe_winId: %d\n", x.timestamp, x.unsafe_winId);
      }, e);
      varact<event::drag_n_drop::Complete>([&](event::drag_n_drop::Complete x) {
        printf("%6d: Dragndrop.complete\n\tunsafe_winId: %d\n", x.timestamp, x.unsafe_winId);
      }, e);

      varact<event::audio_device::Added>([&](event::audio_device::Added x) {
        printf("%6d: Audiodevice.added\n\tindex: %d isCapture: %s\n", x.timestamp, x.index, x.isCapture ? "true" : "false");
      }, e);
      varact<event::audio_device::Removed>([&](event::audio_device::Removed x) {
        printf("%6d: Audiodevice.removed\n\tid: %d isCapture: %s\n", x.timestamp, x.id, x.isCapture ? "true" : "false");
      }, e);
    }

    r.setColor(RGBA{0, 0, 0});
    r.clear();

    mouse::State ms{};
    b.tick(ms);
    b1.tick(ms);
    b2.tick(ms);

    b.render(r);
    b1.render(r);
    b2.render(r);

    r.present();
  }

  return 0;
}
