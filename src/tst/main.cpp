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

  LockTex t(r, pixelfmt::Id::rgba32, Size{800, 600});

  bool running = true;

  int col_r = 255;
  int col_g = 0;
  int col_b = 255;
  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
        case SDL_QUIT:
          running = false;
          break;
        case SDL_KEYDOWN:
          kb::key::Name::query(kb::key::unsafe::fromSDLEnum(e.key.keysym.sym));
          printf("%s\n", kb::key::Name::lastQueryRes());
          break;
      }
    }

    if (kb::State::down(kb::Key::r))
      col_r += 4;
    if (kb::State::down(kb::Key::g))
      col_g += 4;
    if (kb::State::down(kb::Key::b))
      col_b += 4;

    w.setTitle(titles[titleI++]);
    titleI %= sizeof(titles)/sizeof(titles[0]);
    SDL_Delay(50);

    r.setColor(RGBA{0, 0, 0});
    r.clear();
    // r.setColor(RGBA{255, 255, 255});
    // r.drawLine(Pos{10, 10}, Pos{100, 100});
    // r.drawRect(RectXY{110, 110, 200, 110});

    {
      TexLock l(t);
      // for (int x = 300; x < 500; ++x)
      //   for (int y = 200; y < 500; ++y)
      //     l.drawPoint(
      //       RGBA{
      //         static_cast<uint8_t>((col_r -= 3)%256),
      //         static_cast<uint8_t>((col_g -= 7)%256),
      //         static_cast<uint8_t>((col_b -= 5)%256)
      //       },
      //       Pos{x, y});
      for (int x = 0; x < 100; ++x)
        l.drawPoint(
          RGBA{
            static_cast<uint8_t>(col_r%256),
            static_cast<uint8_t>(col_g%256),
            static_cast<uint8_t>(col_b%256)
          },
          Pos{300+5*(x%10), 300+5*(x/10)});
    }
    r.drawTex(t);

    r.present();
  }

  return 0;
}
