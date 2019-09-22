#include <cstdio>
#include <optional>
#include <variant>
#include <functional>

#include "Rolmodl.hpp"

using namespace std;
using namespace rolmodl;
using namespace geom;

template<class T, class... Types>
void varact(function<void(T&)> f, variant<Types...>& v) {
  if (!holds_alternative<T>(v))
    return;

  f(get<T>(v));
}

template<class R, class T, class... Types>
optional<R> varact(function<R(T&)> f, variant<Types...>& v) {
  if (!holds_alternative<T>(v))
    return nullopt;

  return f(get<T>(v));
}

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);

  Size size{800, 600};
  Win_SW w("test", size);
  SWTex& screenTex = w.swtex();

  bool running = true;
  while (running) {
    while (true) {
      optional polled = event::poll();
      if (!polled)
        break;

      Event& e = polled.value();
      varact<event::Quit>([&](event::Quit x) {
        printf("%6d: Quit\n", x.timestamp);
        running = false;
      }, e);
    }

    SDL_Delay(1000/24);
  }

  return 0;
}
