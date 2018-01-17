#include <cstdio>
#include <optional>

#include "../lib/hpp/Rolmodl.hpp"

using namespace std;
using namespace rolmodl;
using namespace geom;

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);

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

  const char* titles[] = {
    "title title title title title title title ",
    "itle title title title title title title t",
    "tle title title title title title title ti",
    "le title title title title title title tit",
    "e title title title title title title titl"
  };
  int titleI = 0;

  bool running = true;
  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
        case SDL_QUIT:
          running = false;
          break;
      }
    }

    w.setTitle(titles[titleI++]);
    titleI %= sizeof(titles)/sizeof(titles[0]);
    SDL_Delay(50);

    r.setColor(Color{0, 0, 0});
    r.clear();
    r.setColor(Color{255, 255, 255});
    r.drawLine(Pos{10, 10}, Pos{100, 100});
    r.drawRect(RectXY{110, 110, 200, 110});
    r.present();
  }

  return 0;
}
