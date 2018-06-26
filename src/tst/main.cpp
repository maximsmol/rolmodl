#include <cstdio>
#include <optional>

#include "../lib/hpp/Rolmodl.hpp"

using namespace std;
using namespace rolmodl;
using namespace geom;

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

  Win w("test", Size{800, 600});
  Ren r(w);

  bool running = true;

  const int btnDim = 30;
  Button b(Pos{100, 100}, Size{btnDim, btnDim});
  Button b1(Pos{100+2*btnDim, 100}, Size{btnDim, btnDim});
  Button b2(Pos{100+4*btnDim, 100}, Size{btnDim, btnDim});
  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
        case SDL_QUIT:
          running = false;
          break;
      }
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
