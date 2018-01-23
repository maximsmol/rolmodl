#pragma once

#include <SDL.h>

namespace rolmodl::kb {
  class State;
  enum class Key;
  enum class Scancode;

  namespace key {
    struct Name;

    constexpr Scancode scancode(const Key k) noexcept;
    constexpr Key fromScancode(const Scancode s) noexcept;
  }
  namespace scancode {
    constexpr Key key(const Scancode s) noexcept;
    constexpr Scancode fromKey(const Key k) noexcept;
  }

  namespace key::unsafe {
    constexpr Key fromSDLEnum(const SDL_Keycode k) noexcept;
    constexpr SDL_Keycode toSDLEnum(const Key k) noexcept;
  }
  namespace scancode::unsafe {
    constexpr Scancode fromSDLEnum(const SDL_Scancode k) noexcept;
    constexpr SDL_Scancode toSDLEnum(const Scancode k) noexcept;
  }
}

namespace rolmodl::kb {
  class State {
    public:
      State(const State& that) = delete;
      State(State&& that) = delete;

      State& operator=(const State& that) = delete;
      State& operator=(State&& that) = delete;

      // fixme: SDL_PumpEvents required before calls
      static bool down(const Scancode s);
      static bool up(const Scancode s);

      static bool down(const Key k);
      static bool up(const Key k);

      static State& instance() noexcept;

      static const uint8_t* unsafeRaw() noexcept;
      static unsigned int unsafeRawL() noexcept;

    private:
      State() noexcept;

      const uint8_t* data_;
      unsigned int l_;
  };

  namespace key {
    struct Name {
      public:
        Name(const Name& that) = delete;
        Name(Name&& that) = delete;

        Name& operator=(const Name& that) = delete;
        Name& operator=(Name&& that) = delete;

        static void query(const Key k) noexcept;
        static void query(const Scancode s) noexcept;
        static const char* lastQueryRes() noexcept;

        static Name& instance() noexcept;

      private:
        Name();

        const char* data_;
    };

    constexpr Scancode scancode(const Key k) noexcept {
      return scancode::unsafe::fromSDLEnum(SDL_GetScancodeFromKey(key::unsafe::toSDLEnum(k)));
    }
    constexpr Key fromScancode(const Scancode s) noexcept {
      return key::unsafe::fromSDLEnum(SDL_GetKeyFromScancode(scancode::unsafe::toSDLEnum(s)));
    }
  }
  namespace scancode {
    constexpr Key key(const Scancode s) noexcept {
      return key::fromScancode(s);
    }
    constexpr Scancode fromKey(const Key k) noexcept {
      return key::scancode(k);
    }
  }

  enum class Key {
    unknown,

    // row 0
    escape,

    f1,
    f2,
    f3,
    f4,
    f5,
    f6,
    f7,
    f8,
    f9,
    f10,
    f11,
    f12,

    printscreen,
    scrolllock,
    pause,

    // row 1
    backquote,

    num_1,
    num_2,
    num_3,
    num_4,
    num_5,
    num_6,
    num_7,
    num_8,
    num_9,
    num_0,

    exclamationPoint,
    at,
    hash,
    dollar,
    percent,
    caret,
    ampersand,
    asterisk,
    leftParen,
    rightParen,
    minus,
    underscore,
    eq,
    plus,
    backspace,

    insert,
    home,
    pageup,

    numlock,
    kp_divide,
    kp_multiply,
    kp_minus,

    // row2
    tab,
    q, w, e, r, t, y, u, i, o, p,
    leftBracket,
    // leftBrace, // todo: not in sdl
    rightBracket,
    // rightBrace, // todo: not in sdl
    backslash,
    // vbar, // todo: not is sdl

    del,
    end,
    pagedown,

    kp_7, kp_8, kp_9,

    // row 3
    capslock,
    a, s, d, f, g, h, j, k, l,
    semicolon,
    colon,
    apostrophe,
    quote,
    enter,
    enter2,

    kp_4, kp_5, kp_6,
    kp_plus,

    //row 4
    lshift,
    z, x, c, v, b, n, m,
    comma,
    less,
    period,
    greater,
    slash,
    questionMark,
    rshift,

    kp_1, kp_2, kp_3,

    //row 5
    lctrl,
    lgui, // todo: better name?
    lalt,
    space,
    ralt,
    rgui,
    app,
    rctrl,

    left,
    up,
    right,
    down,

    kp_0,
    kp_period,
    kp_enter,

    // etc.

    f13,
    f14,
    f15,
    f16,
    f17,
    f18,
    f19,
    f20,
    f21,
    f22,
    f23,
    f24,

    mute,
    volumeUp,
    volumeDown,

    cut,
    copy,
    paste,

    brightnessDown,
    brightnessUp,
    displaySwitch,

    kb_illum_toggle,
    kb_illum_down,
    kb_illum_up,

    thousandsSeparator,
    decimalSeparator,

    currency_unit,
    currency_subunit,

    kp_comma,
    kp_eq,
    kp_eqas400,
    kp_00,
    kp_000,
    kp_leftParen,
    kp_rightParen,
    kp_leftBrace,
    kp_rightBrace,
    kp_tab,
    kp_backspace,
    kp_a,
    kp_b,
    kp_c,
    kp_d,
    kp_e,
    kp_f,
    kp_xor,
    kp_power,
    kp_percent,
    kp_less,
    kp_greater,
    kp_ampersand,
    kp_doubleAmpersand,
    kp_vbar,
    kp_doubleVbar,
    kp_colon,
    kp_hash,
    kp_space,
    kp_at,
    kp_exclamationPoint,
    kp_mem_store,
    kp_mem_recall,
    kp_mem_clear,
    kp_mem_add,
    kp_mem_subtract,
    kp_mem_multiply,
    kp_mem_divide,
    kp_plusMinus,
    kp_clear,
    kp_clearEntry,
    kp_bin,
    kp_octal,
    kp_decimal,
    kp_hex,

    audio_next,
    audio_prev,
    audio_stop,
    audio_play,
    audio_mute,
    audio_rewind,
    audio_fastforward,
    mediaSelect,

    sleep,
    power,
    eject,
    exec,
    help,
    menu,
    select,
    stop,
    redo,
    undo,
    find,

    www,
    mail,
    calculator,
    computer,
    ac_search,
    ac_home,
    ac_back,
    ac_fwd,
    ac_stop,
    ac_refresh,
    ac_bookmarks,

    altErase,
    mode,
    sysrq,
    cancel,
    clear,
    prior,
    separator,
    out,
    oper,
    clearAgain,
    crsel,
    exsel,

    app1,
    app2
  };
  namespace key::unsafe {
    constexpr Key fromSDLEnum(const SDL_Keycode k) noexcept;
    constexpr SDL_Keycode toSDLEnum(const Key k) noexcept;
  }

  enum class Scancode {
    unknown,

    // row 0
    escape,

    f1,
    f2,
    f3,
    f4,
    f5,
    f6,
    f7,
    f8,
    f9,
    f10,
    f11,
    f12,

    printscreen,
    scrolllock,
    pause,

    // row 1
    backquote,

    num_1,
    num_2,
    num_3,
    num_4,
    num_5,
    num_6,
    num_7,
    num_8,
    num_9,
    num_0,

    exclamationPoint,
    at,
    hash,
    dollar,
    percent,
    caret,
    ampersand,
    asterisk,
    leftParen,
    rightParen,
    minus,
    underscore,
    eq,
    plus,
    backspace,

    insert,
    home,
    pageup,

    numlock,
    kp_divide,
    kp_multiply,
    kp_minus,

    // row2
    tab,
    q, w, e, r, t, y, u, i, o, p,
    leftBracket,
    // leftBrace, // todo: not in sdl
    rightBracket,
    // rightBrace, // todo: not in sdl
    backslash,
    // vbar, // todo: not is sdl

    del,
    end,
    pagedown,

    kp_7, kp_8, kp_9,

    // row 3
    capslock,
    a, s, d, f, g, h, j, k, l,
    semicolon,
    colon,
    apostrophe,
    quote,
    enter,
    enter2,

    kp_4, kp_5, kp_6,
    kp_plus,

    //row 4
    lshift,
    z, x, c, v, b, n, m,
    comma,
    less,
    period,
    greater,
    slash,
    questionMark,
    rshift,

    kp_1, kp_2, kp_3,

    //row 5
    lctrl,
    lgui, // todo: better name?
    lalt,
    space,
    ralt,
    rgui,
    app,
    rctrl,

    left,
    up,
    right,
    down,

    kp_0,
    kp_period,
    kp_enter,

    // etc.

    f13,
    f14,
    f15,
    f16,
    f17,
    f18,
    f19,
    f20,
    f21,
    f22,
    f23,
    f24,

    mute,
    volumeUp,
    volumeDown,

    cut,
    copy,
    paste,

    brightnessDown,
    brightnessUp,
    displaySwitch,

    kb_illum_toggle,
    kb_illum_down,
    kb_illum_up,

    thousandsSeparator,
    decimalSeparator,

    currency_unit,
    currency_subunit,

    kp_comma,
    kp_eq,
    kp_eqas400,
    kp_00,
    kp_000,
    kp_leftParen,
    kp_rightParen,
    kp_leftBrace,
    kp_rightBrace,
    kp_tab,
    kp_backspace,
    kp_a,
    kp_b,
    kp_c,
    kp_d,
    kp_e,
    kp_f,
    kp_xor,
    kp_power,
    kp_percent,
    kp_less,
    kp_greater,
    kp_ampersand,
    kp_doubleAmpersand,
    kp_vbar,
    kp_doubleVbar,
    kp_colon,
    kp_hash,
    kp_space,
    kp_at,
    kp_exclamationPoint,
    kp_mem_store,
    kp_mem_recall,
    kp_mem_clear,
    kp_mem_add,
    kp_mem_subtract,
    kp_mem_multiply,
    kp_mem_divide,
    kp_plusMinus,
    kp_clear,
    kp_clearEntry,
    kp_bin,
    kp_octal,
    kp_decimal,
    kp_hex,

    audio_next,
    audio_prev,
    audio_stop,
    audio_play,
    audio_mute,
    audio_rewind,
    audio_fastforward,
    mediaSelect,

    sleep,
    power,
    eject,
    exec,
    help,
    menu,
    select,
    stop,
    redo,
    undo,
    find,

    www,
    mail,
    calculator,
    computer,
    ac_search,
    ac_home,
    ac_back,
    ac_fwd,
    ac_stop,
    ac_refresh,
    ac_bookmarks,

    altErase,
    mode,
    sysrq,
    cancel,
    clear,
    prior,
    separator,
    out,
    oper,
    clearAgain,
    crsel,
    exsel,

    app1,
    app2,

    nonUsHash,
    nonUsBackslash,

    internat_1,
    internat_2,
    internat_3,
    internat_4,
    internat_5,
    internat_6,
    internat_7,
    internat_8,
    internat_9,

    lang_1,
    lang_2,
    lang_3,
    lang_4,
    lang_5,
    lang_6,
    lang_7,
    lang_8,
    lang_9
  };
  namespace scancode::unsafe {
    constexpr Scancode fromSDLEnum(const SDL_Scancode k) noexcept;
    constexpr SDL_Scancode toSDLEnum(const Scancode k) noexcept;
  }

  namespace key::unsafe {
    constexpr Key fromSDLEnum(const SDL_Keycode k) noexcept {
      if (k == SDLK_UNKNOWN)
        return Key::unknown;

      if (k == SDLK_RETURN)
        return Key::enter;
      if (k == SDLK_ESCAPE)
        return Key::escape;
      if (k == SDLK_BACKSPACE)
        return Key::backspace;
      if (k == SDLK_TAB)
        return Key::tab;
      if (k == SDLK_SPACE)
        return Key::space;
      if (k == SDLK_EXCLAIM)
        return Key::exclamationPoint;
      if (k == SDLK_QUOTEDBL)
        return Key::quote;
      if (k == SDLK_HASH)
        return Key::hash;
      if (k == SDLK_PERCENT)
        return Key::percent;
      if (k == SDLK_DOLLAR)
        return Key::dollar;
      if (k == SDLK_AMPERSAND)
        return Key::ampersand;
      if (k == SDLK_QUOTE)
        return Key::apostrophe;
      if (k == SDLK_LEFTPAREN)
        return Key::leftParen;
      if (k == SDLK_RIGHTPAREN)
        return Key::rightParen;
      if (k == SDLK_ASTERISK)
        return Key::asterisk;
      if (k == SDLK_PLUS)
        return Key::plus;
      if (k == SDLK_COMMA)
        return Key::comma;
      if (k == SDLK_MINUS)
        return Key::minus;
      if (k == SDLK_PERIOD)
        return Key::period;
      if (k == SDLK_SLASH)
        return Key::slash;
      if (k == SDLK_0)
        return Key::num_0;
      if (k == SDLK_1)
        return Key::num_1;
      if (k == SDLK_2)
        return Key::num_2;
      if (k == SDLK_3)
        return Key::num_3;
      if (k == SDLK_4)
        return Key::num_4;
      if (k == SDLK_5)
        return Key::num_5;
      if (k == SDLK_6)
        return Key::num_6;
      if (k == SDLK_7)
        return Key::num_7;
      if (k == SDLK_8)
        return Key::num_8;
      if (k == SDLK_9)
        return Key::num_9;
      if (k == SDLK_COLON)
        return Key::colon;
      if (k == SDLK_SEMICOLON)
        return Key::semicolon;
      if (k == SDLK_LESS)
        return Key::less;
      if (k == SDLK_EQUALS)
        return Key::eq;
      if (k == SDLK_GREATER)
        return Key::greater;
      if (k == SDLK_QUESTION)
        return Key::questionMark;
      if (k == SDLK_AT)
        return Key::at;

      if (k == SDLK_LEFTBRACKET)
        return Key::leftBracket;
      if (k == SDLK_BACKSLASH)
        return Key::backslash;
      if (k == SDLK_RIGHTBRACKET)
        return Key::rightBracket;
      if (k == SDLK_CARET)
        return Key::caret;
      if (k == SDLK_UNDERSCORE)
        return Key::underscore;
      if (k == SDLK_BACKQUOTE)
        return Key::backquote;
      if (k == SDLK_a)
        return Key::a;
      if (k == SDLK_b)
        return Key::b;
      if (k == SDLK_c)
        return Key::c;
      if (k == SDLK_d)
        return Key::d;
      if (k == SDLK_e)
        return Key::e;
      if (k == SDLK_f)
        return Key::f;
      if (k == SDLK_g)
        return Key::g;
      if (k == SDLK_h)
        return Key::h;
      if (k == SDLK_i)
        return Key::i;
      if (k == SDLK_j)
        return Key::j;
      if (k == SDLK_k)
        return Key::k;
      if (k == SDLK_l)
        return Key::l;
      if (k == SDLK_m)
        return Key::m;
      if (k == SDLK_n)
        return Key::n;
      if (k == SDLK_o)
        return Key::o;
      if (k == SDLK_p)
        return Key::p;
      if (k == SDLK_q)
        return Key::q;
      if (k == SDLK_r)
        return Key::r;
      if (k == SDLK_s)
        return Key::s;
      if (k == SDLK_t)
        return Key::t;
      if (k == SDLK_u)
        return Key::u;
      if (k == SDLK_v)
        return Key::v;
      if (k == SDLK_w)
        return Key::w;
      if (k == SDLK_x)
        return Key::x;
      if (k == SDLK_y)
        return Key::y;
      if (k == SDLK_z)
        return Key::z;

      if (k == SDLK_CAPSLOCK)
        return Key::capslock;

      if (k == SDLK_F1)
        return Key::f1;
      if (k == SDLK_F2)
        return Key::f2;
      if (k == SDLK_F3)
        return Key::f3;
      if (k == SDLK_F4)
        return Key::f4;
      if (k == SDLK_F5)
        return Key::f5;
      if (k == SDLK_F6)
        return Key::f6;
      if (k == SDLK_F7)
        return Key::f7;
      if (k == SDLK_F8)
        return Key::f8;
      if (k == SDLK_F9)
        return Key::f9;
      if (k == SDLK_F10)
        return Key::f10;
      if (k == SDLK_F11)
        return Key::f11;
      if (k == SDLK_F12)
        return Key::f12;

      if (k == SDLK_PRINTSCREEN)
        return Key::printscreen;
      if (k == SDLK_SCROLLLOCK)
        return Key::scrolllock;
      if (k == SDLK_PAUSE)
        return Key::pause;
      if (k == SDLK_INSERT)
        return Key::insert;
      if (k == SDLK_HOME)
        return Key::home;
      if (k == SDLK_PAGEUP)
        return Key::pageup;
      if (k == SDLK_DELETE)
        return Key::del;
      if (k == SDLK_END)
        return Key::end;
      if (k == SDLK_PAGEDOWN)
        return Key::pagedown;
      if (k == SDLK_RIGHT)
        return Key::right;
      if (k == SDLK_LEFT)
        return Key::left;
      if (k == SDLK_DOWN)
        return Key::down;
      if (k == SDLK_UP)
        return Key::up;

      if (k == SDLK_NUMLOCKCLEAR)
        return Key::numlock;
      if (k == SDLK_KP_DIVIDE)
        return Key::kp_divide;
      if (k == SDLK_KP_MULTIPLY)
        return Key::kp_multiply;
      if (k == SDLK_KP_MINUS)
        return Key::kp_minus;
      if (k == SDLK_KP_PLUS)
        return Key::kp_plus;
      if (k == SDLK_KP_ENTER)
        return Key::kp_enter;
      if (k == SDLK_KP_1)
        return Key::kp_1;
      if (k == SDLK_KP_2)
        return Key::kp_2;
      if (k == SDLK_KP_3)
        return Key::kp_3;
      if (k == SDLK_KP_4)
        return Key::kp_4;
      if (k == SDLK_KP_5)
        return Key::kp_5;
      if (k == SDLK_KP_6)
        return Key::kp_6;
      if (k == SDLK_KP_7)
        return Key::kp_7;
      if (k == SDLK_KP_8)
        return Key::kp_8;
      if (k == SDLK_KP_9)
        return Key::kp_9;
      if (k == SDLK_KP_0)
        return Key::kp_0;
      if (k == SDLK_KP_PERIOD)
        return Key::kp_period;

      if (k == SDLK_APPLICATION)
        return Key::app;
      if (k == SDLK_POWER)
        return Key::power;
      if (k == SDLK_KP_EQUALS)
        return Key::kp_eq;
      if (k == SDLK_F13)
        return Key::f13;
      if (k == SDLK_F14)
        return Key::f14;
      if (k == SDLK_F15)
        return Key::f15;
      if (k == SDLK_F16)
        return Key::f16;
      if (k == SDLK_F17)
        return Key::f17;
      if (k == SDLK_F18)
        return Key::f18;
      if (k == SDLK_F19)
        return Key::f19;
      if (k == SDLK_F20)
        return Key::f20;
      if (k == SDLK_F21)
        return Key::f21;
      if (k == SDLK_F22)
        return Key::f22;
      if (k == SDLK_F23)
        return Key::f23;
      if (k == SDLK_F24)
        return Key::f24;
      if (k == SDLK_EXECUTE)
        return Key::exec;
      if (k == SDLK_HELP)
        return Key::help;
      if (k == SDLK_MENU)
        return Key::menu;
      if (k == SDLK_SELECT)
        return Key::select;
      if (k == SDLK_STOP)
        return Key::stop;
      if (k == SDLK_AGAIN)
        return Key::redo;
      if (k == SDLK_UNDO)
        return Key::undo;
      if (k == SDLK_CUT)
        return Key::cut;
      if (k == SDLK_COPY)
        return Key::copy;
      if (k == SDLK_PASTE)
        return Key::paste;
      if (k == SDLK_FIND)
        return Key::find;
      if (k == SDLK_MUTE)
        return Key::mute;
      if (k == SDLK_VOLUMEUP)
        return Key::volumeUp;
      if (k == SDLK_VOLUMEDOWN)
        return Key::volumeDown;
      if (k == SDLK_KP_COMMA)
        return Key::kp_comma;
      if (k == SDLK_KP_EQUALSAS400)
        return Key::kp_eqas400;

      if (k == SDLK_ALTERASE)
        return Key::altErase;
      if (k == SDLK_SYSREQ)
        return Key::sysrq;
      if (k == SDLK_CANCEL)
        return Key::cancel;
      if (k == SDLK_CLEAR)
        return Key::clear;
      if (k == SDLK_PRIOR)
        return Key::prior;
      if (k == SDLK_RETURN2)
        return Key::enter2;
      if (k == SDLK_SEPARATOR)
        return Key::separator;
      if (k == SDLK_OUT)
        return Key::out;
      if (k == SDLK_OPER)
        return Key::oper;
      if (k == SDLK_CLEARAGAIN)
        return Key::clearAgain;
      if (k == SDLK_CRSEL)
        return Key::crsel;
      if (k == SDLK_EXSEL)
        return Key::exsel;

      if (k == SDLK_KP_00)
        return Key::kp_00;
      if (k == SDLK_KP_000)
        return Key::kp_000;
      if (k == SDLK_THOUSANDSSEPARATOR)
        return Key::thousandsSeparator;
      if (k == SDLK_DECIMALSEPARATOR)
        return Key::decimalSeparator;
      if (k == SDLK_CURRENCYUNIT)
        return Key::currency_unit;
      if (k == SDLK_CURRENCYSUBUNIT)
        return Key::currency_subunit;
      if (k == SDLK_KP_LEFTPAREN)
        return Key::kp_leftParen;
      if (k == SDLK_KP_RIGHTPAREN)
        return Key::kp_rightParen;
      if (k == SDLK_KP_LEFTBRACE)
        return Key::kp_leftBrace;
      if (k == SDLK_KP_RIGHTBRACE)
        return Key::kp_rightBrace;
      if (k == SDLK_KP_TAB)
        return Key::kp_tab;
      if (k == SDLK_KP_BACKSPACE)
        return Key::kp_backspace;
      if (k == SDLK_KP_A)
        return Key::kp_a;
      if (k == SDLK_KP_B)
        return Key::kp_b;
      if (k == SDLK_KP_C)
        return Key::kp_c;
      if (k == SDLK_KP_D)
        return Key::kp_d;
      if (k == SDLK_KP_E)
        return Key::kp_e;
      if (k == SDLK_KP_F)
        return Key::kp_f;
      if (k == SDLK_KP_XOR)
        return Key::kp_xor;
      if (k == SDLK_KP_POWER)
        return Key::kp_power;
      if (k == SDLK_KP_PERCENT)
        return Key::kp_percent;
      if (k == SDLK_KP_LESS)
        return Key::kp_less;
      if (k == SDLK_KP_GREATER)
        return Key::kp_greater;
      if (k == SDLK_KP_AMPERSAND)
        return Key::kp_ampersand;
      if (k == SDLK_KP_DBLAMPERSAND)
        return Key::kp_doubleAmpersand;
      if (k == SDLK_KP_VERTICALBAR)
        return Key::kp_vbar;
      if (k == SDLK_KP_DBLVERTICALBAR)
        return Key::kp_doubleVbar;
      if (k == SDLK_KP_COLON)
        return Key::kp_colon;
      if (k == SDLK_KP_HASH)
        return Key::kp_hash;
      if (k == SDLK_KP_SPACE)
        return Key::kp_space;
      if (k == SDLK_KP_AT)
        return Key::kp_at;
      if (k == SDLK_KP_EXCLAM)
        return Key::kp_exclamationPoint;
      if (k == SDLK_KP_MEMSTORE)
        return Key::kp_mem_store;
      if (k == SDLK_KP_MEMRECALL)
        return Key::kp_mem_recall;
      if (k == SDLK_KP_MEMCLEAR)
        return Key::kp_mem_clear;
      if (k == SDLK_KP_MEMADD)
        return Key::kp_mem_add;
      if (k == SDLK_KP_MEMSUBTRACT)
        return Key::kp_mem_subtract;
      if (k == SDLK_KP_MEMMULTIPLY)
        return Key::kp_mem_multiply;
      if (k == SDLK_KP_MEMDIVIDE)
        return Key::kp_mem_divide;
      if (k == SDLK_KP_PLUSMINUS)
        return Key::kp_plusMinus;
      if (k == SDLK_KP_CLEAR)
        return Key::kp_clear;
      if (k == SDLK_KP_CLEARENTRY)
        return Key::kp_clearEntry;
      if (k == SDLK_KP_BINARY)
        return Key::kp_bin;
      if (k == SDLK_KP_OCTAL)
        return Key::kp_octal;
      if (k == SDLK_KP_DECIMAL)
        return Key::kp_decimal;
      if (k == SDLK_KP_HEXADECIMAL)
        return Key::kp_hex;

      if (k == SDLK_LCTRL)
        return Key::lctrl;
      if (k == SDLK_LSHIFT)
        return Key::lshift;
      if (k == SDLK_LALT)
        return Key::lalt;
      if (k == SDLK_LGUI)
        return Key::lgui;
      if (k == SDLK_RCTRL)
        return Key::rctrl;
      if (k == SDLK_RSHIFT)
        return Key::rshift;
      if (k == SDLK_RALT)
        return Key::ralt;
      if (k == SDLK_RGUI)
        return Key::rgui;

      if (k == SDLK_MODE)
        return Key::mode;

      if (k == SDLK_AUDIONEXT)
        return Key::audio_next;
      if (k == SDLK_AUDIOPREV)
        return Key::audio_prev;
      if (k == SDLK_AUDIOSTOP)
        return Key::audio_stop;
      if (k == SDLK_AUDIOPLAY)
        return Key::audio_play;
      if (k == SDLK_AUDIOMUTE)
        return Key::audio_mute;
      if (k == SDLK_MEDIASELECT)
        return Key::mediaSelect;
      if (k == SDLK_WWW)
        return Key::www;
      if (k == SDLK_MAIL)
        return Key::mail;
      if (k == SDLK_CALCULATOR)
        return Key::calculator;
      if (k == SDLK_COMPUTER)
        return Key::computer;
      if (k == SDLK_AC_SEARCH)
        return Key::ac_search;
      if (k == SDLK_AC_HOME)
        return Key::ac_home;
      if (k == SDLK_AC_BACK)
        return Key::ac_back;
      if (k == SDLK_AC_FORWARD)
        return Key::ac_fwd;
      if (k == SDLK_AC_STOP)
        return Key::ac_stop;
      if (k == SDLK_AC_REFRESH)
        return Key::ac_refresh;
      if (k == SDLK_AC_BOOKMARKS)
        return Key::ac_bookmarks;

      if (k == SDLK_BRIGHTNESSDOWN)
        return Key::brightnessDown;
      if (k == SDLK_BRIGHTNESSUP)
        return Key::brightnessUp;
      if (k == SDLK_DISPLAYSWITCH)
        return Key::displaySwitch;
      if (k == SDLK_KBDILLUMTOGGLE)
        return Key::kb_illum_toggle;
      if (k == SDLK_KBDILLUMDOWN)
        return Key::kb_illum_down;
      if (k == SDLK_KBDILLUMUP)
        return Key::kb_illum_up;
      if (k == SDLK_EJECT)
        return Key::eject;
      if (k == SDLK_SLEEP)
        return Key::sleep;
      if (k == SDLK_APP1)
        return Key::app1;
      if (k == SDLK_APP2)
        return Key::app2;

      if (k == SDLK_AUDIOREWIND)
        return Key::audio_rewind;
      if (k == SDLK_AUDIOFASTFORWARD)
        return Key::audio_fastforward;

      return Key::unknown;
    }

    constexpr SDL_Keycode toSDLEnum(const Key k) noexcept {
      if (k == Key::unknown)
        return SDLK_UNKNOWN;

      if (k == Key::enter)
        return SDLK_RETURN;
      if (k == Key::escape)
        return SDLK_ESCAPE;
      if (k == Key::backspace)
        return SDLK_BACKSPACE;
      if (k == Key::tab)
        return SDLK_TAB;
      if (k == Key::space)
        return SDLK_SPACE;
      if (k == Key::exclamationPoint)
        return SDLK_EXCLAIM;
      if (k == Key::quote)
        return SDLK_QUOTEDBL;
      if (k == Key::hash)
        return SDLK_HASH;
      if (k == Key::percent)
        return SDLK_PERCENT;
      if (k == Key::dollar)
        return SDLK_DOLLAR;
      if (k == Key::ampersand)
        return SDLK_AMPERSAND;
      if (k == Key::apostrophe)
        return SDLK_QUOTE;
      if (k == Key::leftParen)
        return SDLK_LEFTPAREN;
      if (k == Key::rightParen)
        return SDLK_RIGHTPAREN;
      if (k == Key::asterisk)
        return SDLK_ASTERISK;
      if (k == Key::plus)
        return SDLK_PLUS;
      if (k == Key::comma)
        return SDLK_COMMA;
      if (k == Key::minus)
        return SDLK_MINUS;
      if (k == Key::period)
        return SDLK_PERIOD;
      if (k == Key::slash)
        return SDLK_SLASH;
      if (k == Key::num_0)
        return SDLK_0;
      if (k == Key::num_1)
        return SDLK_1;
      if (k == Key::num_2)
        return SDLK_2;
      if (k == Key::num_3)
        return SDLK_3;
      if (k == Key::num_4)
        return SDLK_4;
      if (k == Key::num_5)
        return SDLK_5;
      if (k == Key::num_6)
        return SDLK_6;
      if (k == Key::num_7)
        return SDLK_7;
      if (k == Key::num_8)
        return SDLK_8;
      if (k == Key::num_9)
        return SDLK_9;
      if (k == Key::colon)
        return SDLK_COLON;
      if (k == Key::semicolon)
        return SDLK_SEMICOLON;
      if (k == Key::less)
        return SDLK_LESS;
      if (k == Key::eq)
        return SDLK_EQUALS;
      if (k == Key::greater)
        return SDLK_GREATER;
      if (k == Key::questionMark)
        return SDLK_QUESTION;
      if (k == Key::at)
        return SDLK_AT;

      if (k == Key::leftBracket)
        return SDLK_LEFTBRACKET;
      if (k == Key::backslash)
        return SDLK_BACKSLASH;
      if (k == Key::rightBracket)
        return SDLK_RIGHTBRACKET;
      if (k == Key::caret)
        return SDLK_CARET;
      if (k == Key::underscore)
        return SDLK_UNDERSCORE;
      if (k == Key::backquote)
        return SDLK_BACKQUOTE;
      if (k == Key::a)
        return SDLK_a;
      if (k == Key::b)
        return SDLK_b;
      if (k == Key::c)
        return SDLK_c;
      if (k == Key::d)
        return SDLK_d;
      if (k == Key::e)
        return SDLK_e;
      if (k == Key::f)
        return SDLK_f;
      if (k == Key::g)
        return SDLK_g;
      if (k == Key::h)
        return SDLK_h;
      if (k == Key::i)
        return SDLK_i;
      if (k == Key::j)
        return SDLK_j;
      if (k == Key::k)
        return SDLK_k;
      if (k == Key::l)
        return SDLK_l;
      if (k == Key::m)
        return SDLK_m;
      if (k == Key::n)
        return SDLK_n;
      if (k == Key::o)
        return SDLK_o;
      if (k == Key::p)
        return SDLK_p;
      if (k == Key::q)
        return SDLK_q;
      if (k == Key::r)
        return SDLK_r;
      if (k == Key::s)
        return SDLK_s;
      if (k == Key::t)
        return SDLK_t;
      if (k == Key::u)
        return SDLK_u;
      if (k == Key::v)
        return SDLK_v;
      if (k == Key::w)
        return SDLK_w;
      if (k == Key::x)
        return SDLK_x;
      if (k == Key::y)
        return SDLK_y;
      if (k == Key::z)
        return SDLK_z;

      if (k == Key::capslock)
        return SDLK_CAPSLOCK;

      if (k == Key::f1)
        return SDLK_F1;
      if (k == Key::f2)
        return SDLK_F2;
      if (k == Key::f3)
        return SDLK_F3;
      if (k == Key::f4)
        return SDLK_F4;
      if (k == Key::f5)
        return SDLK_F5;
      if (k == Key::f6)
        return SDLK_F6;
      if (k == Key::f7)
        return SDLK_F7;
      if (k == Key::f8)
        return SDLK_F8;
      if (k == Key::f9)
        return SDLK_F9;
      if (k == Key::f10)
        return SDLK_F10;
      if (k == Key::f11)
        return SDLK_F11;
      if (k == Key::f12)
        return SDLK_F12;

      if (k == Key::printscreen)
        return SDLK_PRINTSCREEN;
      if (k == Key::scrolllock)
        return SDLK_SCROLLLOCK;
      if (k == Key::pause)
        return SDLK_PAUSE;
      if (k == Key::insert)
        return SDLK_INSERT;
      if (k == Key::home)
        return SDLK_HOME;
      if (k == Key::pageup)
        return SDLK_PAGEUP;
      if (k == Key::del)
        return SDLK_DELETE;
      if (k == Key::end)
        return SDLK_END;
      if (k == Key::pagedown)
        return SDLK_PAGEDOWN;
      if (k == Key::right)
        return SDLK_RIGHT;
      if (k == Key::left)
        return SDLK_LEFT;
      if (k == Key::down)
        return SDLK_DOWN;
      if (k == Key::up)
        return SDLK_UP;

      if (k == Key::numlock)
        return SDLK_NUMLOCKCLEAR;
      if (k == Key::kp_divide)
        return SDLK_KP_DIVIDE;
      if (k == Key::kp_multiply)
        return SDLK_KP_MULTIPLY;
      if (k == Key::kp_minus)
        return SDLK_KP_MINUS;
      if (k == Key::kp_plus)
        return SDLK_KP_PLUS;
      if (k == Key::kp_enter)
        return SDLK_KP_ENTER;
      if (k == Key::kp_1)
        return SDLK_KP_1;
      if (k == Key::kp_2)
        return SDLK_KP_2;
      if (k == Key::kp_3)
        return SDLK_KP_3;
      if (k == Key::kp_4)
        return SDLK_KP_4;
      if (k == Key::kp_5)
        return SDLK_KP_5;
      if (k == Key::kp_6)
        return SDLK_KP_6;
      if (k == Key::kp_7)
        return SDLK_KP_7;
      if (k == Key::kp_8)
        return SDLK_KP_8;
      if (k == Key::kp_9)
        return SDLK_KP_9;
      if (k == Key::kp_0)
        return SDLK_KP_0;
      if (k == Key::kp_period)
        return SDLK_KP_PERIOD;

      if (k == Key::app)
        return SDLK_APPLICATION;
      if (k == Key::power)
        return SDLK_POWER;
      if (k == Key::kp_eq)
        return SDLK_KP_EQUALS;
      if (k == Key::f13)
        return SDLK_F13;
      if (k == Key::f14)
        return SDLK_F14;
      if (k == Key::f15)
        return SDLK_F15;
      if (k == Key::f16)
        return SDLK_F16;
      if (k == Key::f17)
        return SDLK_F17;
      if (k == Key::f18)
        return SDLK_F18;
      if (k == Key::f19)
        return SDLK_F19;
      if (k == Key::f20)
        return SDLK_F20;
      if (k == Key::f21)
        return SDLK_F21;
      if (k == Key::f22)
        return SDLK_F22;
      if (k == Key::f23)
        return SDLK_F23;
      if (k == Key::f24)
        return SDLK_F24;
      if (k == Key::exec)
        return SDLK_EXECUTE;
      if (k == Key::help)
        return SDLK_HELP;
      if (k == Key::menu)
        return SDLK_MENU;
      if (k == Key::select)
        return SDLK_SELECT;
      if (k == Key::stop)
        return SDLK_STOP;
      if (k == Key::redo)
        return SDLK_AGAIN;
      if (k == Key::undo)
        return SDLK_UNDO;
      if (k == Key::cut)
        return SDLK_CUT;
      if (k == Key::copy)
        return SDLK_COPY;
      if (k == Key::paste)
        return SDLK_PASTE;
      if (k == Key::find)
        return SDLK_FIND;
      if (k == Key::mute)
        return SDLK_MUTE;
      if (k == Key::volumeUp)
        return SDLK_VOLUMEUP;
      if (k == Key::volumeDown)
        return SDLK_VOLUMEDOWN;
      if (k == Key::kp_comma)
        return SDLK_KP_COMMA;
      if (k == Key::kp_eqas400)
        return SDLK_KP_EQUALSAS400;

      if (k == Key::altErase)
        return SDLK_ALTERASE;
      if (k == Key::sysrq)
        return SDLK_SYSREQ;
      if (k == Key::cancel)
        return SDLK_CANCEL;
      if (k == Key::clear)
        return SDLK_CLEAR;
      if (k == Key::prior)
        return SDLK_PRIOR;
      if (k == Key::enter2)
        return SDLK_RETURN2;
      if (k == Key::separator)
        return SDLK_SEPARATOR;
      if (k == Key::out)
        return SDLK_OUT;
      if (k == Key::oper)
        return SDLK_OPER;
      if (k == Key::clearAgain)
        return SDLK_CLEARAGAIN;
      if (k == Key::crsel)
        return SDLK_CRSEL;
      if (k == Key::exsel)
        return SDLK_EXSEL;

      if (k == Key::kp_00)
        return SDLK_KP_00;
      if (k == Key::kp_000)
        return SDLK_KP_000;
      if (k == Key::thousandsSeparator)
        return SDLK_THOUSANDSSEPARATOR;
      if (k == Key::decimalSeparator)
        return SDLK_DECIMALSEPARATOR;
      if (k == Key::currency_unit)
        return SDLK_CURRENCYUNIT;
      if (k == Key::currency_subunit)
        return SDLK_CURRENCYSUBUNIT;
      if (k == Key::kp_leftParen)
        return SDLK_KP_LEFTPAREN;
      if (k == Key::kp_rightParen)
        return SDLK_KP_RIGHTPAREN;
      if (k == Key::kp_leftBrace)
        return SDLK_KP_LEFTBRACE;
      if (k == Key::kp_rightBrace)
        return SDLK_KP_RIGHTBRACE;
      if (k == Key::kp_tab)
        return SDLK_KP_TAB;
      if (k == Key::kp_backspace)
        return SDLK_KP_BACKSPACE;
      if (k == Key::kp_a)
        return SDLK_KP_A;
      if (k == Key::kp_b)
        return SDLK_KP_B;
      if (k == Key::kp_c)
        return SDLK_KP_C;
      if (k == Key::kp_d)
        return SDLK_KP_D;
      if (k == Key::kp_e)
        return SDLK_KP_E;
      if (k == Key::kp_f)
        return SDLK_KP_F;
      if (k == Key::kp_xor)
        return SDLK_KP_XOR;
      if (k == Key::kp_power)
        return SDLK_KP_POWER;
      if (k == Key::kp_percent)
        return SDLK_KP_PERCENT;
      if (k == Key::kp_less)
        return SDLK_KP_LESS;
      if (k == Key::kp_greater)
        return SDLK_KP_GREATER;
      if (k == Key::kp_ampersand)
        return SDLK_KP_AMPERSAND;
      if (k == Key::kp_doubleAmpersand)
        return SDLK_KP_DBLAMPERSAND;
      if (k == Key::kp_vbar)
        return SDLK_KP_VERTICALBAR;
      if (k == Key::kp_doubleVbar)
        return SDLK_KP_DBLVERTICALBAR;
      if (k == Key::kp_colon)
        return SDLK_KP_COLON;
      if (k == Key::kp_hash)
        return SDLK_KP_HASH;
      if (k == Key::kp_space)
        return SDLK_KP_SPACE;
      if (k == Key::kp_at)
        return SDLK_KP_AT;
      if (k == Key::kp_exclamationPoint)
        return SDLK_KP_EXCLAM;
      if (k == Key::kp_mem_store)
        return SDLK_KP_MEMSTORE;
      if (k == Key::kp_mem_recall)
        return SDLK_KP_MEMRECALL;
      if (k == Key::kp_mem_clear)
        return SDLK_KP_MEMCLEAR;
      if (k == Key::kp_mem_add)
        return SDLK_KP_MEMADD;
      if (k == Key::kp_mem_subtract)
        return SDLK_KP_MEMSUBTRACT;
      if (k == Key::kp_mem_multiply)
        return SDLK_KP_MEMMULTIPLY;
      if (k == Key::kp_mem_divide)
        return SDLK_KP_MEMDIVIDE;
      if (k == Key::kp_plusMinus)
        return SDLK_KP_PLUSMINUS;
      if (k == Key::kp_clear)
        return SDLK_KP_CLEAR;
      if (k == Key::kp_clearEntry)
        return SDLK_KP_CLEARENTRY;
      if (k == Key::kp_bin)
        return SDLK_KP_BINARY;
      if (k == Key::kp_octal)
        return SDLK_KP_OCTAL;
      if (k == Key::kp_decimal)
        return SDLK_KP_DECIMAL;
      if (k == Key::kp_hex)
        return SDLK_KP_HEXADECIMAL;

      if (k == Key::lctrl)
        return SDLK_LCTRL;
      if (k == Key::lshift)
        return SDLK_LSHIFT;
      if (k == Key::lalt)
        return SDLK_LALT;
      if (k == Key::lgui)
        return SDLK_LGUI;
      if (k == Key::rctrl)
        return SDLK_RCTRL;
      if (k == Key::rshift)
        return SDLK_RSHIFT;
      if (k == Key::ralt)
        return SDLK_RALT;
      if (k == Key::rgui)
        return SDLK_RGUI;

      if (k == Key::mode)
        return SDLK_MODE;

      if (k == Key::audio_next)
        return SDLK_AUDIONEXT;
      if (k == Key::audio_prev)
        return SDLK_AUDIOPREV;
      if (k == Key::audio_stop)
        return SDLK_AUDIOSTOP;
      if (k == Key::audio_play)
        return SDLK_AUDIOPLAY;
      if (k == Key::audio_mute)
        return SDLK_AUDIOMUTE;
      if (k == Key::mediaSelect)
        return SDLK_MEDIASELECT;
      if (k == Key::www)
        return SDLK_WWW;
      if (k == Key::mail)
        return SDLK_MAIL;
      if (k == Key::calculator)
        return SDLK_CALCULATOR;
      if (k == Key::computer)
        return SDLK_COMPUTER;
      if (k == Key::ac_search)
        return SDLK_AC_SEARCH;
      if (k == Key::ac_home)
        return SDLK_AC_HOME;
      if (k == Key::ac_back)
        return SDLK_AC_BACK;
      if (k == Key::ac_fwd)
        return SDLK_AC_FORWARD;
      if (k == Key::ac_stop)
        return SDLK_AC_STOP;
      if (k == Key::ac_refresh)
        return SDLK_AC_REFRESH;
      if (k == Key::ac_bookmarks)
        return SDLK_AC_BOOKMARKS;

      if (k == Key::brightnessDown)
        return SDLK_BRIGHTNESSDOWN;
      if (k == Key::brightnessUp)
        return SDLK_BRIGHTNESSUP;
      if (k == Key::displaySwitch)
        return SDLK_DISPLAYSWITCH;
      if (k == Key::kb_illum_toggle)
        return SDLK_KBDILLUMTOGGLE;
      if (k == Key::kb_illum_down)
        return SDLK_KBDILLUMDOWN;
      if (k == Key::kb_illum_up)
        return SDLK_KBDILLUMUP;
      if (k == Key::eject)
        return SDLK_EJECT;
      if (k == Key::sleep)
        return SDLK_SLEEP;
      if (k == Key::app1)
        return SDLK_APP1;
      if (k == Key::app2)
        return SDLK_APP2;

      if (k == Key::audio_rewind)
        return SDLK_AUDIOREWIND;
      if (k == Key::audio_fastforward)
        return SDLK_AUDIOFASTFORWARD;

      return SDLK_UNKNOWN;
    }
  }

  namespace scancode::unsafe {
    constexpr Scancode fromSDLEnum(const SDL_Scancode k) noexcept {
      if (k == SDL_SCANCODE_UNKNOWN)
        return Scancode::unknown;

      if (k == SDL_SCANCODE_A)
        return Scancode::a;
      if (k == SDL_SCANCODE_B)
        return Scancode::b;
      if (k == SDL_SCANCODE_C)
        return Scancode::c;
      if (k == SDL_SCANCODE_D)
        return Scancode::d;
      if (k == SDL_SCANCODE_E)
        return Scancode::e;
      if (k == SDL_SCANCODE_F)
        return Scancode::f;
      if (k == SDL_SCANCODE_G)
        return Scancode::g;
      if (k == SDL_SCANCODE_H)
        return Scancode::h;
      if (k == SDL_SCANCODE_I)
        return Scancode::i;
      if (k == SDL_SCANCODE_J)
        return Scancode::j;
      if (k == SDL_SCANCODE_K)
        return Scancode::k;
      if (k == SDL_SCANCODE_L)
        return Scancode::l;
      if (k == SDL_SCANCODE_M)
        return Scancode::m;
      if (k == SDL_SCANCODE_N)
        return Scancode::n;
      if (k == SDL_SCANCODE_O)
        return Scancode::o;
      if (k == SDL_SCANCODE_P)
        return Scancode::p;
      if (k == SDL_SCANCODE_Q)
        return Scancode::q;
      if (k == SDL_SCANCODE_R)
        return Scancode::r;
      if (k == SDL_SCANCODE_S)
        return Scancode::s;
      if (k == SDL_SCANCODE_T)
        return Scancode::t;
      if (k == SDL_SCANCODE_U)
        return Scancode::u;
      if (k == SDL_SCANCODE_V)
        return Scancode::v;
      if (k == SDL_SCANCODE_W)
        return Scancode::w;
      if (k == SDL_SCANCODE_X)
        return Scancode::x;
      if (k == SDL_SCANCODE_Y)
        return Scancode::y;
      if (k == SDL_SCANCODE_Z)
        return Scancode::z;

      if (k == SDL_SCANCODE_1)
        return Scancode::num_1;
      if (k == SDL_SCANCODE_2)
        return Scancode::num_2;
      if (k == SDL_SCANCODE_3)
        return Scancode::num_3;
      if (k == SDL_SCANCODE_4)
        return Scancode::num_4;
      if (k == SDL_SCANCODE_5)
        return Scancode::num_5;
      if (k == SDL_SCANCODE_6)
        return Scancode::num_6;
      if (k == SDL_SCANCODE_7)
        return Scancode::num_7;
      if (k == SDL_SCANCODE_8)
        return Scancode::num_8;
      if (k == SDL_SCANCODE_9)
        return Scancode::num_9;
      if (k == SDL_SCANCODE_0)
        return Scancode::num_0;

      if (k == SDL_SCANCODE_RETURN)
        return Scancode::enter;
      if (k == SDL_SCANCODE_ESCAPE)
        return Scancode::escape;
      if (k == SDL_SCANCODE_BACKSPACE)
        return Scancode::backspace;
      if (k == SDL_SCANCODE_TAB)
        return Scancode::tab;
      if (k == SDL_SCANCODE_SPACE)
        return Scancode::space;

      if (k == SDL_SCANCODE_MINUS)
        return Scancode::minus;
      if (k == SDL_SCANCODE_EQUALS)
        return Scancode::eq;
      if (k == SDL_SCANCODE_LEFTBRACKET)
        return Scancode::leftBracket;
      if (k == SDL_SCANCODE_RIGHTBRACKET)
        return Scancode::rightBracket;
      if (k == SDL_SCANCODE_BACKSLASH)
        return Scancode::backslash;
      if (k == SDL_SCANCODE_NONUSHASH)
        return Scancode::nonUsHash;
      if (k == SDL_SCANCODE_SEMICOLON)
        return Scancode::semicolon;
      if (k == SDL_SCANCODE_APOSTROPHE)
        return Scancode::apostrophe;
      if (k == SDL_SCANCODE_GRAVE)
        return Scancode::backquote;
      if (k == SDL_SCANCODE_COMMA)
        return Scancode::comma;
      if (k == SDL_SCANCODE_PERIOD)
        return Scancode::period;
      if (k == SDL_SCANCODE_SLASH)
        return Scancode::slash;

      if (k == SDL_SCANCODE_CAPSLOCK)
        return Scancode::capslock;

      if (k == SDL_SCANCODE_F1)
        return Scancode::f1;
      if (k == SDL_SCANCODE_F2)
        return Scancode::f2;
      if (k == SDL_SCANCODE_F3)
        return Scancode::f3;
      if (k == SDL_SCANCODE_F4)
        return Scancode::f4;
      if (k == SDL_SCANCODE_F5)
        return Scancode::f5;
      if (k == SDL_SCANCODE_F6)
        return Scancode::f6;
      if (k == SDL_SCANCODE_F7)
        return Scancode::f7;
      if (k == SDL_SCANCODE_F8)
        return Scancode::f8;
      if (k == SDL_SCANCODE_F9)
        return Scancode::f9;
      if (k == SDL_SCANCODE_F10)
        return Scancode::f10;
      if (k == SDL_SCANCODE_F11)
        return Scancode::f11;
      if (k == SDL_SCANCODE_F12)
        return Scancode::f12;

      if (k == SDL_SCANCODE_PRINTSCREEN)
        return Scancode::printscreen;
      if (k == SDL_SCANCODE_SCROLLLOCK)
        return Scancode::scrolllock;
      if (k == SDL_SCANCODE_PAUSE)
        return Scancode::pause;
      if (k == SDL_SCANCODE_INSERT)
        return Scancode::insert;
      if (k == SDL_SCANCODE_HOME)
        return Scancode::home;
      if (k == SDL_SCANCODE_PAGEUP)
        return Scancode::pageup;
      if (k == SDL_SCANCODE_DELETE)
        return Scancode::del;
      if (k == SDL_SCANCODE_END)
        return Scancode::end;
      if (k == SDL_SCANCODE_PAGEDOWN)
        return Scancode::pagedown;
      if (k == SDL_SCANCODE_RIGHT)
        return Scancode::right;
      if (k == SDL_SCANCODE_LEFT)
        return Scancode::left;
      if (k == SDL_SCANCODE_DOWN)
        return Scancode::down;
      if (k == SDL_SCANCODE_UP)
        return Scancode::up;

      if (k == SDL_SCANCODE_NUMLOCKCLEAR)
        return Scancode::numlock;
      if (k == SDL_SCANCODE_KP_DIVIDE)
        return Scancode::kp_divide;
      if (k == SDL_SCANCODE_KP_MULTIPLY)
        return Scancode::kp_multiply;
      if (k == SDL_SCANCODE_KP_MINUS)
        return Scancode::kp_minus;
      if (k == SDL_SCANCODE_KP_PLUS)
        return Scancode::kp_plus;
      if (k == SDL_SCANCODE_KP_ENTER)
        return Scancode::kp_enter;
      if (k == SDL_SCANCODE_KP_1)
        return Scancode::kp_1;
      if (k == SDL_SCANCODE_KP_2)
        return Scancode::kp_2;
      if (k == SDL_SCANCODE_KP_3)
        return Scancode::kp_3;
      if (k == SDL_SCANCODE_KP_4)
        return Scancode::kp_4;
      if (k == SDL_SCANCODE_KP_5)
        return Scancode::kp_5;
      if (k == SDL_SCANCODE_KP_6)
        return Scancode::kp_6;
      if (k == SDL_SCANCODE_KP_7)
        return Scancode::kp_7;
      if (k == SDL_SCANCODE_KP_8)
        return Scancode::kp_8;
      if (k == SDL_SCANCODE_KP_9)
        return Scancode::kp_9;
      if (k == SDL_SCANCODE_KP_0)
        return Scancode::kp_0;
      if (k == SDL_SCANCODE_KP_PERIOD)
        return Scancode::kp_period;

      if (k == SDL_SCANCODE_NONUSBACKSLASH)
        return Scancode::nonUsBackslash;
      if (k == SDL_SCANCODE_APPLICATION)
        return Scancode::app;
      if (k == SDL_SCANCODE_POWER)
        return Scancode::power;
      if (k == SDL_SCANCODE_KP_EQUALS)
        return Scancode::kp_eq;
      if (k == SDL_SCANCODE_F13)
        return Scancode::f13;
      if (k == SDL_SCANCODE_F14)
        return Scancode::f14;
      if (k == SDL_SCANCODE_F15)
        return Scancode::f15;
      if (k == SDL_SCANCODE_F16)
        return Scancode::f16;
      if (k == SDL_SCANCODE_F17)
        return Scancode::f17;
      if (k == SDL_SCANCODE_F18)
        return Scancode::f18;
      if (k == SDL_SCANCODE_F19)
        return Scancode::f19;
      if (k == SDL_SCANCODE_F20)
        return Scancode::f20;
      if (k == SDL_SCANCODE_F21)
        return Scancode::f21;
      if (k == SDL_SCANCODE_F22)
        return Scancode::f22;
      if (k == SDL_SCANCODE_F23)
        return Scancode::f23;
      if (k == SDL_SCANCODE_F24)
        return Scancode::f24;
      if (k == SDL_SCANCODE_EXECUTE)
        return Scancode::exec;
      if (k == SDL_SCANCODE_HELP)
        return Scancode::help;
      if (k == SDL_SCANCODE_MENU)
        return Scancode::menu;
      if (k == SDL_SCANCODE_SELECT)
        return Scancode::select;
      if (k == SDL_SCANCODE_STOP)
        return Scancode::stop;
      if (k == SDL_SCANCODE_AGAIN)
        return Scancode::redo;
      if (k == SDL_SCANCODE_UNDO)
        return Scancode::undo;
      if (k == SDL_SCANCODE_CUT)
        return Scancode::cut;
      if (k == SDL_SCANCODE_COPY)
        return Scancode::copy;
      if (k == SDL_SCANCODE_PASTE)
        return Scancode::paste;
      if (k == SDL_SCANCODE_FIND)
        return Scancode::find;
      if (k == SDL_SCANCODE_MUTE)
        return Scancode::mute;
      if (k == SDL_SCANCODE_VOLUMEUP)
        return Scancode::volumeUp;
      if (k == SDL_SCANCODE_VOLUMEDOWN)
        return Scancode::volumeDown;
      if (k == SDL_SCANCODE_KP_COMMA)
        return Scancode::kp_comma;
      if (k == SDL_SCANCODE_KP_EQUALSAS400)
        return Scancode::kp_eqas400;

      if (k == SDL_SCANCODE_INTERNATIONAL1)
        return Scancode::internat_1;
      if (k == SDL_SCANCODE_INTERNATIONAL2)
        return Scancode::internat_2;
      if (k == SDL_SCANCODE_INTERNATIONAL3)
        return Scancode::internat_3;
      if (k == SDL_SCANCODE_INTERNATIONAL4)
        return Scancode::internat_4;
      if (k == SDL_SCANCODE_INTERNATIONAL5)
        return Scancode::internat_5;
      if (k == SDL_SCANCODE_INTERNATIONAL6)
        return Scancode::internat_6;
      if (k == SDL_SCANCODE_INTERNATIONAL7)
        return Scancode::internat_7;
      if (k == SDL_SCANCODE_INTERNATIONAL8)
        return Scancode::internat_8;
      if (k == SDL_SCANCODE_INTERNATIONAL9)
        return Scancode::internat_9;
      if (k == SDL_SCANCODE_LANG1)
        return Scancode::lang_1;
      if (k == SDL_SCANCODE_LANG2)
        return Scancode::lang_2;
      if (k == SDL_SCANCODE_LANG3)
        return Scancode::lang_3;
      if (k == SDL_SCANCODE_LANG4)
        return Scancode::lang_4;
      if (k == SDL_SCANCODE_LANG5)
        return Scancode::lang_5;
      if (k == SDL_SCANCODE_LANG6)
        return Scancode::lang_6;
      if (k == SDL_SCANCODE_LANG7)
        return Scancode::lang_7;
      if (k == SDL_SCANCODE_LANG8)
        return Scancode::lang_8;
      if (k == SDL_SCANCODE_LANG9)
        return Scancode::lang_9;

      if (k == SDL_SCANCODE_ALTERASE)
        return Scancode::altErase;
      if (k == SDL_SCANCODE_SYSREQ)
        return Scancode::sysrq;
      if (k == SDL_SCANCODE_CANCEL)
        return Scancode::cancel;
      if (k == SDL_SCANCODE_CLEAR)
        return Scancode::clear;
      if (k == SDL_SCANCODE_PRIOR)
        return Scancode::prior;
      if (k == SDL_SCANCODE_RETURN2)
        return Scancode::enter2;
      if (k == SDL_SCANCODE_SEPARATOR)
        return Scancode::separator;
      if (k == SDL_SCANCODE_OUT)
        return Scancode::out;
      if (k == SDL_SCANCODE_OPER)
        return Scancode::oper;
      if (k == SDL_SCANCODE_CLEARAGAIN)
        return Scancode::clearAgain;
      if (k == SDL_SCANCODE_CRSEL)
        return Scancode::crsel;
      if (k == SDL_SCANCODE_EXSEL)
        return Scancode::exsel;

      if (k == SDL_SCANCODE_KP_00)
        return Scancode::kp_00;
      if (k == SDL_SCANCODE_KP_000)
        return Scancode::kp_000;
      if (k == SDL_SCANCODE_THOUSANDSSEPARATOR)
        return Scancode::thousandsSeparator;
      if (k == SDL_SCANCODE_DECIMALSEPARATOR)
        return Scancode::decimalSeparator;
      if (k == SDL_SCANCODE_CURRENCYUNIT)
        return Scancode::currency_unit;
      if (k == SDL_SCANCODE_CURRENCYSUBUNIT)
        return Scancode::currency_subunit;
      if (k == SDL_SCANCODE_KP_LEFTPAREN)
        return Scancode::kp_leftParen;
      if (k == SDL_SCANCODE_KP_RIGHTPAREN)
        return Scancode::kp_rightParen;
      if (k == SDL_SCANCODE_KP_LEFTBRACE)
        return Scancode::kp_leftBrace;
      if (k == SDL_SCANCODE_KP_RIGHTBRACE)
        return Scancode::kp_rightBrace;
      if (k == SDL_SCANCODE_KP_TAB)
        return Scancode::kp_tab;
      if (k == SDL_SCANCODE_KP_BACKSPACE)
        return Scancode::kp_backspace;
      if (k == SDL_SCANCODE_KP_A)
        return Scancode::kp_a;
      if (k == SDL_SCANCODE_KP_B)
        return Scancode::kp_b;
      if (k == SDL_SCANCODE_KP_C)
        return Scancode::kp_c;
      if (k == SDL_SCANCODE_KP_D)
        return Scancode::kp_d;
      if (k == SDL_SCANCODE_KP_E)
        return Scancode::kp_e;
      if (k == SDL_SCANCODE_KP_F)
        return Scancode::kp_f;
      if (k == SDL_SCANCODE_KP_XOR)
        return Scancode::kp_xor;
      if (k == SDL_SCANCODE_KP_POWER)
        return Scancode::kp_power;
      if (k == SDL_SCANCODE_KP_PERCENT)
        return Scancode::kp_percent;
      if (k == SDL_SCANCODE_KP_LESS)
        return Scancode::kp_less;
      if (k == SDL_SCANCODE_KP_GREATER)
        return Scancode::kp_greater;
      if (k == SDL_SCANCODE_KP_AMPERSAND)
        return Scancode::kp_ampersand;
      if (k == SDL_SCANCODE_KP_DBLAMPERSAND)
        return Scancode::kp_doubleAmpersand;
      if (k == SDL_SCANCODE_KP_VERTICALBAR)
        return Scancode::kp_vbar;
      if (k == SDL_SCANCODE_KP_DBLVERTICALBAR)
        return Scancode::kp_doubleVbar;
      if (k == SDL_SCANCODE_KP_COLON)
        return Scancode::kp_colon;
      if (k == SDL_SCANCODE_KP_HASH)
        return Scancode::kp_hash;
      if (k == SDL_SCANCODE_KP_SPACE)
        return Scancode::kp_space;
      if (k == SDL_SCANCODE_KP_AT)
        return Scancode::kp_at;
      if (k == SDL_SCANCODE_KP_EXCLAM)
        return Scancode::kp_exclamationPoint;
      if (k == SDL_SCANCODE_KP_MEMSTORE)
        return Scancode::kp_mem_store;
      if (k == SDL_SCANCODE_KP_MEMRECALL)
        return Scancode::kp_mem_recall;
      if (k == SDL_SCANCODE_KP_MEMCLEAR)
        return Scancode::kp_mem_clear;
      if (k == SDL_SCANCODE_KP_MEMADD)
        return Scancode::kp_mem_add;
      if (k == SDL_SCANCODE_KP_MEMSUBTRACT)
        return Scancode::kp_mem_subtract;
      if (k == SDL_SCANCODE_KP_MEMMULTIPLY)
        return Scancode::kp_mem_multiply;
      if (k == SDL_SCANCODE_KP_MEMDIVIDE)
        return Scancode::kp_mem_divide;
      if (k == SDL_SCANCODE_KP_PLUSMINUS)
        return Scancode::kp_plusMinus;
      if (k == SDL_SCANCODE_KP_CLEAR)
        return Scancode::kp_mem_clear;
      if (k == SDL_SCANCODE_KP_CLEARENTRY)
        return Scancode::kp_clearEntry;
      if (k == SDL_SCANCODE_KP_BINARY)
        return Scancode::kp_bin;
      if (k == SDL_SCANCODE_KP_OCTAL)
        return Scancode::kp_octal;
      if (k == SDL_SCANCODE_KP_DECIMAL)
        return Scancode::kp_decimal;
      if (k == SDL_SCANCODE_KP_HEXADECIMAL)
        return Scancode::kp_hex;

      if (k == SDL_SCANCODE_LCTRL)
        return Scancode::lctrl;
      if (k == SDL_SCANCODE_LSHIFT)
        return Scancode::lshift;
      if (k == SDL_SCANCODE_LALT)
        return Scancode::lalt;
      if (k == SDL_SCANCODE_LGUI)
        return Scancode::lgui;
      if (k == SDL_SCANCODE_RCTRL)
        return Scancode::rctrl;
      if (k == SDL_SCANCODE_RSHIFT)
        return Scancode::rshift;
      if (k == SDL_SCANCODE_RALT)
        return Scancode::ralt;
      if (k == SDL_SCANCODE_RGUI)
        return Scancode::rgui;

      if (k == SDL_SCANCODE_MODE)
        return Scancode::mode;

      if (k == SDL_SCANCODE_AUDIONEXT)
        return Scancode::audio_next;
      if (k == SDL_SCANCODE_AUDIOPREV)
        return Scancode::audio_prev;
      if (k == SDL_SCANCODE_AUDIOSTOP)
        return Scancode::audio_stop;
      if (k == SDL_SCANCODE_AUDIOPLAY)
        return Scancode::audio_play;
      if (k == SDL_SCANCODE_AUDIOMUTE)
        return Scancode::audio_mute;
      if (k == SDL_SCANCODE_MEDIASELECT)
        return Scancode::mediaSelect;
      if (k == SDL_SCANCODE_WWW)
        return Scancode::www;
      if (k == SDL_SCANCODE_MAIL)
        return Scancode::mail;
      if (k == SDL_SCANCODE_CALCULATOR)
        return Scancode::calculator;
      if (k == SDL_SCANCODE_COMPUTER)
        return Scancode::computer;
      if (k == SDL_SCANCODE_AC_SEARCH)
        return Scancode::ac_search;
      if (k == SDL_SCANCODE_AC_HOME)
        return Scancode::ac_home;
      if (k == SDL_SCANCODE_AC_BACK)
        return Scancode::ac_back;
      if (k == SDL_SCANCODE_AC_FORWARD)
        return Scancode::ac_fwd;
      if (k == SDL_SCANCODE_AC_STOP)
        return Scancode::ac_stop;
      if (k == SDL_SCANCODE_AC_REFRESH)
        return Scancode::ac_refresh;
      if (k == SDL_SCANCODE_AC_BOOKMARKS)
        return Scancode::ac_bookmarks;

      if (k == SDL_SCANCODE_BRIGHTNESSDOWN)
        return Scancode::brightnessDown;
      if (k == SDL_SCANCODE_BRIGHTNESSUP)
        return Scancode::brightnessUp;
      if (k == SDL_SCANCODE_DISPLAYSWITCH)
        return Scancode::displaySwitch;
      if (k == SDL_SCANCODE_KBDILLUMTOGGLE)
        return Scancode::kb_illum_toggle;
      if (k == SDL_SCANCODE_KBDILLUMDOWN)
        return Scancode::kb_illum_down;
      if (k == SDL_SCANCODE_KBDILLUMUP)
        return Scancode::kb_illum_up;
      if (k == SDL_SCANCODE_EJECT)
        return Scancode::eject;
      if (k == SDL_SCANCODE_SLEEP)
        return Scancode::sleep;

      if (k == SDL_SCANCODE_APP1)
        return Scancode::app1;
      if (k == SDL_SCANCODE_APP2)
        return Scancode::app2;

      if (k == SDL_SCANCODE_AUDIOREWIND)
        return Scancode::audio_rewind;
      if (k == SDL_SCANCODE_AUDIOFASTFORWARD)
        return Scancode::audio_fastforward;

      return Scancode::unknown;
    }
    constexpr SDL_Scancode toSDLEnum(const Scancode k) noexcept {
      if (k == Scancode::unknown)
        return SDL_SCANCODE_UNKNOWN;

      if (k == Scancode::a)
        return SDL_SCANCODE_A;
      if (k == Scancode::b)
        return SDL_SCANCODE_B;
      if (k == Scancode::c)
        return SDL_SCANCODE_C;
      if (k == Scancode::d)
        return SDL_SCANCODE_D;
      if (k == Scancode::e)
        return SDL_SCANCODE_E;
      if (k == Scancode::f)
        return SDL_SCANCODE_F;
      if (k == Scancode::g)
        return SDL_SCANCODE_G;
      if (k == Scancode::h)
        return SDL_SCANCODE_H;
      if (k == Scancode::i)
        return SDL_SCANCODE_I;
      if (k == Scancode::j)
        return SDL_SCANCODE_J;
      if (k == Scancode::k)
        return SDL_SCANCODE_K;
      if (k == Scancode::l)
        return SDL_SCANCODE_L;
      if (k == Scancode::m)
        return SDL_SCANCODE_M;
      if (k == Scancode::n)
        return SDL_SCANCODE_N;
      if (k == Scancode::o)
        return SDL_SCANCODE_O;
      if (k == Scancode::p)
        return SDL_SCANCODE_P;
      if (k == Scancode::q)
        return SDL_SCANCODE_Q;
      if (k == Scancode::r)
        return SDL_SCANCODE_R;
      if (k == Scancode::s)
        return SDL_SCANCODE_S;
      if (k == Scancode::t)
        return SDL_SCANCODE_T;
      if (k == Scancode::u)
        return SDL_SCANCODE_U;
      if (k == Scancode::v)
        return SDL_SCANCODE_V;
      if (k == Scancode::w)
        return SDL_SCANCODE_W;
      if (k == Scancode::x)
        return SDL_SCANCODE_X;
      if (k == Scancode::y)
        return SDL_SCANCODE_Y;
      if (k == Scancode::z)
        return SDL_SCANCODE_Z;

      if (k == Scancode::num_1)
        return SDL_SCANCODE_1;
      if (k == Scancode::num_2)
        return SDL_SCANCODE_2;
      if (k == Scancode::num_3)
        return SDL_SCANCODE_3;
      if (k == Scancode::num_4)
        return SDL_SCANCODE_4;
      if (k == Scancode::num_5)
        return SDL_SCANCODE_5;
      if (k == Scancode::num_6)
        return SDL_SCANCODE_6;
      if (k == Scancode::num_7)
        return SDL_SCANCODE_7;
      if (k == Scancode::num_8)
        return SDL_SCANCODE_8;
      if (k == Scancode::num_9)
        return SDL_SCANCODE_9;
      if (k == Scancode::num_0)
        return SDL_SCANCODE_0;

      if (k == Scancode::enter)
        return SDL_SCANCODE_RETURN;
      if (k == Scancode::escape)
        return SDL_SCANCODE_ESCAPE;
      if (k == Scancode::backspace)
        return SDL_SCANCODE_BACKSPACE;
      if (k == Scancode::tab)
        return SDL_SCANCODE_TAB;
      if (k == Scancode::space)
        return SDL_SCANCODE_SPACE;

      if (k == Scancode::minus)
        return SDL_SCANCODE_MINUS;
      if (k == Scancode::eq)
        return SDL_SCANCODE_EQUALS;
      if (k == Scancode::leftBracket)
        return SDL_SCANCODE_LEFTBRACKET;
      if (k == Scancode::rightBracket)
        return SDL_SCANCODE_RIGHTBRACKET;
      if (k == Scancode::backslash)
        return SDL_SCANCODE_BACKSLASH;
      if (k == Scancode::nonUsHash)
        return SDL_SCANCODE_NONUSHASH;
      if (k == Scancode::semicolon)
        return SDL_SCANCODE_SEMICOLON;
      if (k == Scancode::apostrophe)
        return SDL_SCANCODE_APOSTROPHE;
      if (k == Scancode::backquote)
        return SDL_SCANCODE_GRAVE;
      if (k == Scancode::comma)
        return SDL_SCANCODE_COMMA;
      if (k == Scancode::period)
        return SDL_SCANCODE_PERIOD;
      if (k == Scancode::slash)
        return SDL_SCANCODE_SLASH;

      if (k == Scancode::capslock)
        return SDL_SCANCODE_CAPSLOCK;

      if (k == Scancode::f1)
        return SDL_SCANCODE_F1;
      if (k == Scancode::f2)
        return SDL_SCANCODE_F2;
      if (k == Scancode::f3)
        return SDL_SCANCODE_F3;
      if (k == Scancode::f4)
        return SDL_SCANCODE_F4;
      if (k == Scancode::f5)
        return SDL_SCANCODE_F5;
      if (k == Scancode::f6)
        return SDL_SCANCODE_F6;
      if (k == Scancode::f7)
        return SDL_SCANCODE_F7;
      if (k == Scancode::f8)
        return SDL_SCANCODE_F8;
      if (k == Scancode::f9)
        return SDL_SCANCODE_F9;
      if (k == Scancode::f10)
        return SDL_SCANCODE_F10;
      if (k == Scancode::f11)
        return SDL_SCANCODE_F11;
      if (k == Scancode::f12)
        return SDL_SCANCODE_F12;

      if (k == Scancode::printscreen)
        return SDL_SCANCODE_PRINTSCREEN;
      if (k == Scancode::scrolllock)
        return SDL_SCANCODE_SCROLLLOCK;
      if (k == Scancode::pause)
        return SDL_SCANCODE_PAUSE;
      if (k == Scancode::insert)
        return SDL_SCANCODE_INSERT;
      if (k == Scancode::home)
        return SDL_SCANCODE_HOME;
      if (k == Scancode::pageup)
        return SDL_SCANCODE_PAGEUP;
      if (k == Scancode::del)
        return SDL_SCANCODE_DELETE;
      if (k == Scancode::end)
        return SDL_SCANCODE_END;
      if (k == Scancode::pagedown)
        return SDL_SCANCODE_PAGEDOWN;
      if (k == Scancode::right)
        return SDL_SCANCODE_RIGHT;
      if (k == Scancode::left)
        return SDL_SCANCODE_LEFT;
      if (k == Scancode::down)
        return SDL_SCANCODE_DOWN;
      if (k == Scancode::up)
        return SDL_SCANCODE_UP;

      if (k == Scancode::numlock)
        return SDL_SCANCODE_NUMLOCKCLEAR;
      if (k == Scancode::kp_divide)
        return SDL_SCANCODE_KP_DIVIDE;
      if (k == Scancode::kp_multiply)
        return SDL_SCANCODE_KP_MULTIPLY;
      if (k == Scancode::kp_minus)
        return SDL_SCANCODE_KP_MINUS;
      if (k == Scancode::kp_plus)
        return SDL_SCANCODE_KP_PLUS;
      if (k == Scancode::kp_enter)
        return SDL_SCANCODE_KP_ENTER;
      if (k == Scancode::kp_1)
        return SDL_SCANCODE_KP_1;
      if (k == Scancode::kp_2)
        return SDL_SCANCODE_KP_2;
      if (k == Scancode::kp_3)
        return SDL_SCANCODE_KP_3;
      if (k == Scancode::kp_4)
        return SDL_SCANCODE_KP_4;
      if (k == Scancode::kp_5)
        return SDL_SCANCODE_KP_5;
      if (k == Scancode::kp_6)
        return SDL_SCANCODE_KP_6;
      if (k == Scancode::kp_7)
        return SDL_SCANCODE_KP_7;
      if (k == Scancode::kp_8)
        return SDL_SCANCODE_KP_8;
      if (k == Scancode::kp_9)
        return SDL_SCANCODE_KP_9;
      if (k == Scancode::kp_0)
        return SDL_SCANCODE_KP_0;
      if (k == Scancode::kp_period)
        return SDL_SCANCODE_KP_PERIOD;

      if (k == Scancode::nonUsBackslash)
        return SDL_SCANCODE_NONUSBACKSLASH;
      if (k == Scancode::app)
        return SDL_SCANCODE_APPLICATION;
      if (k == Scancode::power)
        return SDL_SCANCODE_POWER;
      if (k == Scancode::kp_eq)
        return SDL_SCANCODE_KP_EQUALS;
      if (k == Scancode::f13)
        return SDL_SCANCODE_F13;
      if (k == Scancode::f14)
        return SDL_SCANCODE_F14;
      if (k == Scancode::f15)
        return SDL_SCANCODE_F15;
      if (k == Scancode::f16)
        return SDL_SCANCODE_F16;
      if (k == Scancode::f17)
        return SDL_SCANCODE_F17;
      if (k == Scancode::f18)
        return SDL_SCANCODE_F18;
      if (k == Scancode::f19)
        return SDL_SCANCODE_F19;
      if (k == Scancode::f20)
        return SDL_SCANCODE_F20;
      if (k == Scancode::f21)
        return SDL_SCANCODE_F21;
      if (k == Scancode::f22)
        return SDL_SCANCODE_F22;
      if (k == Scancode::f23)
        return SDL_SCANCODE_F23;
      if (k == Scancode::f24)
        return SDL_SCANCODE_F24;
      if (k == Scancode::exec)
        return SDL_SCANCODE_EXECUTE;
      if (k == Scancode::help)
        return SDL_SCANCODE_HELP;
      if (k == Scancode::menu)
        return SDL_SCANCODE_MENU;
      if (k == Scancode::select)
        return SDL_SCANCODE_SELECT;
      if (k == Scancode::stop)
        return SDL_SCANCODE_STOP;
      if (k == Scancode::redo)
        return SDL_SCANCODE_AGAIN;
      if (k == Scancode::undo)
        return SDL_SCANCODE_UNDO;
      if (k == Scancode::cut)
        return SDL_SCANCODE_CUT;
      if (k == Scancode::copy)
        return SDL_SCANCODE_COPY;
      if (k == Scancode::paste)
        return SDL_SCANCODE_PASTE;
      if (k == Scancode::find)
        return SDL_SCANCODE_FIND;
      if (k == Scancode::mute)
        return SDL_SCANCODE_MUTE;
      if (k == Scancode::volumeUp)
        return SDL_SCANCODE_VOLUMEUP;
      if (k == Scancode::volumeDown)
        return SDL_SCANCODE_VOLUMEDOWN;
      if (k == Scancode::kp_comma)
        return SDL_SCANCODE_KP_COMMA;
      if (k == Scancode::kp_eqas400)
        return SDL_SCANCODE_KP_EQUALSAS400;

      if (k == Scancode::internat_1)
        return SDL_SCANCODE_INTERNATIONAL1;
      if (k == Scancode::internat_2)
        return SDL_SCANCODE_INTERNATIONAL2;
      if (k == Scancode::internat_3)
        return SDL_SCANCODE_INTERNATIONAL3;
      if (k == Scancode::internat_4)
        return SDL_SCANCODE_INTERNATIONAL4;
      if (k == Scancode::internat_5)
        return SDL_SCANCODE_INTERNATIONAL5;
      if (k == Scancode::internat_6)
        return SDL_SCANCODE_INTERNATIONAL6;
      if (k == Scancode::internat_7)
        return SDL_SCANCODE_INTERNATIONAL7;
      if (k == Scancode::internat_8)
        return SDL_SCANCODE_INTERNATIONAL8;
      if (k == Scancode::internat_9)
        return SDL_SCANCODE_INTERNATIONAL9;
      if (k == Scancode::lang_1)
        return SDL_SCANCODE_LANG1;
      if (k == Scancode::lang_2)
        return SDL_SCANCODE_LANG2;
      if (k == Scancode::lang_3)
        return SDL_SCANCODE_LANG3;
      if (k == Scancode::lang_4)
        return SDL_SCANCODE_LANG4;
      if (k == Scancode::lang_5)
        return SDL_SCANCODE_LANG5;
      if (k == Scancode::lang_6)
        return SDL_SCANCODE_LANG6;
      if (k == Scancode::lang_7)
        return SDL_SCANCODE_LANG7;
      if (k == Scancode::lang_8)
        return SDL_SCANCODE_LANG8;
      if (k == Scancode::lang_9)
        return SDL_SCANCODE_LANG9;

      if (k == Scancode::altErase)
        return SDL_SCANCODE_ALTERASE;
      if (k == Scancode::sysrq)
        return SDL_SCANCODE_SYSREQ;
      if (k == Scancode::cancel)
        return SDL_SCANCODE_CANCEL;
      if (k == Scancode::clear)
        return SDL_SCANCODE_CLEAR;
      if (k == Scancode::prior)
        return SDL_SCANCODE_PRIOR;
      if (k == Scancode::enter2)
        return SDL_SCANCODE_RETURN2;
      if (k == Scancode::separator)
        return SDL_SCANCODE_SEPARATOR;
      if (k == Scancode::out)
        return SDL_SCANCODE_OUT;
      if (k == Scancode::oper)
        return SDL_SCANCODE_OPER;
      if (k == Scancode::clearAgain)
        return SDL_SCANCODE_CLEARAGAIN;
      if (k == Scancode::crsel)
        return SDL_SCANCODE_CRSEL;
      if (k == Scancode::exsel)
        return SDL_SCANCODE_EXSEL;

      if (k == Scancode::kp_00)
        return SDL_SCANCODE_KP_00;
      if (k == Scancode::kp_000)
        return SDL_SCANCODE_KP_000;
      if (k == Scancode::thousandsSeparator)
        return SDL_SCANCODE_THOUSANDSSEPARATOR;
      if (k == Scancode::decimalSeparator)
        return SDL_SCANCODE_DECIMALSEPARATOR;
      if (k == Scancode::currency_unit)
        return SDL_SCANCODE_CURRENCYUNIT;
      if (k == Scancode::currency_subunit)
        return SDL_SCANCODE_CURRENCYSUBUNIT;
      if (k == Scancode::kp_leftParen)
        return SDL_SCANCODE_KP_LEFTPAREN;
      if (k == Scancode::kp_rightParen)
        return SDL_SCANCODE_KP_RIGHTPAREN;
      if (k == Scancode::kp_leftBrace)
        return SDL_SCANCODE_KP_LEFTBRACE;
      if (k == Scancode::kp_rightBrace)
        return SDL_SCANCODE_KP_RIGHTBRACE;
      if (k == Scancode::kp_tab)
        return SDL_SCANCODE_KP_TAB;
      if (k == Scancode::kp_backspace)
        return SDL_SCANCODE_KP_BACKSPACE;
      if (k == Scancode::kp_a)
        return SDL_SCANCODE_KP_A;
      if (k == Scancode::kp_b)
        return SDL_SCANCODE_KP_B;
      if (k == Scancode::kp_c)
        return SDL_SCANCODE_KP_C;
      if (k == Scancode::kp_d)
        return SDL_SCANCODE_KP_D;
      if (k == Scancode::kp_e)
        return SDL_SCANCODE_KP_E;
      if (k == Scancode::kp_f)
        return SDL_SCANCODE_KP_F;
      if (k == Scancode::kp_xor)
        return SDL_SCANCODE_KP_XOR;
      if (k == Scancode::kp_power)
        return SDL_SCANCODE_KP_POWER;
      if (k == Scancode::kp_percent)
        return SDL_SCANCODE_KP_PERCENT;
      if (k == Scancode::kp_less)
        return SDL_SCANCODE_KP_LESS;
      if (k == Scancode::kp_greater)
        return SDL_SCANCODE_KP_GREATER;
      if (k == Scancode::kp_ampersand)
        return SDL_SCANCODE_KP_AMPERSAND;
      if (k == Scancode::kp_doubleAmpersand)
        return SDL_SCANCODE_KP_DBLAMPERSAND;
      if (k == Scancode::kp_vbar)
        return SDL_SCANCODE_KP_VERTICALBAR;
      if (k == Scancode::kp_doubleVbar)
        return SDL_SCANCODE_KP_DBLVERTICALBAR;
      if (k == Scancode::kp_colon)
        return SDL_SCANCODE_KP_COLON;
      if (k == Scancode::kp_hash)
        return SDL_SCANCODE_KP_HASH;
      if (k == Scancode::kp_space)
        return SDL_SCANCODE_KP_SPACE;
      if (k == Scancode::kp_at)
        return SDL_SCANCODE_KP_AT;
      if (k == Scancode::kp_exclamationPoint)
        return SDL_SCANCODE_KP_EXCLAM;
      if (k == Scancode::kp_mem_store)
        return SDL_SCANCODE_KP_MEMSTORE;
      if (k == Scancode::kp_mem_recall)
        return SDL_SCANCODE_KP_MEMRECALL;
      if (k == Scancode::kp_mem_clear)
        return SDL_SCANCODE_KP_MEMCLEAR;
      if (k == Scancode::kp_mem_add)
        return SDL_SCANCODE_KP_MEMADD;
      if (k == Scancode::kp_mem_subtract)
        return SDL_SCANCODE_KP_MEMSUBTRACT;
      if (k == Scancode::kp_mem_multiply)
        return SDL_SCANCODE_KP_MEMMULTIPLY;
      if (k == Scancode::kp_mem_divide)
        return SDL_SCANCODE_KP_MEMDIVIDE;
      if (k == Scancode::kp_plusMinus)
        return SDL_SCANCODE_KP_PLUSMINUS;
      if (k == Scancode::kp_mem_clear)
        return SDL_SCANCODE_KP_CLEAR;
      if (k == Scancode::kp_clearEntry)
        return SDL_SCANCODE_KP_CLEARENTRY;
      if (k == Scancode::kp_bin)
        return SDL_SCANCODE_KP_BINARY;
      if (k == Scancode::kp_octal)
        return SDL_SCANCODE_KP_OCTAL;
      if (k == Scancode::kp_decimal)
        return SDL_SCANCODE_KP_DECIMAL;
      if (k == Scancode::kp_hex)
        return SDL_SCANCODE_KP_HEXADECIMAL;

      if (k == Scancode::lctrl)
        return SDL_SCANCODE_LCTRL;
      if (k == Scancode::lshift)
        return SDL_SCANCODE_LSHIFT;
      if (k == Scancode::lalt)
        return SDL_SCANCODE_LALT;
      if (k == Scancode::lgui)
        return SDL_SCANCODE_LGUI;
      if (k == Scancode::rctrl)
        return SDL_SCANCODE_RCTRL;
      if (k == Scancode::rshift)
        return SDL_SCANCODE_RSHIFT;
      if (k == Scancode::ralt)
        return SDL_SCANCODE_RALT;
      if (k == Scancode::rgui)
        return SDL_SCANCODE_RGUI;

      if (k == Scancode::mode)
        return SDL_SCANCODE_MODE;

      if (k == Scancode::audio_next)
        return SDL_SCANCODE_AUDIONEXT;
      if (k == Scancode::audio_prev)
        return SDL_SCANCODE_AUDIOPREV;
      if (k == Scancode::audio_stop)
        return SDL_SCANCODE_AUDIOSTOP;
      if (k == Scancode::audio_play)
        return SDL_SCANCODE_AUDIOPLAY;
      if (k == Scancode::audio_mute)
        return SDL_SCANCODE_AUDIOMUTE;
      if (k == Scancode::mediaSelect)
        return SDL_SCANCODE_MEDIASELECT;
      if (k == Scancode::www)
        return SDL_SCANCODE_WWW;
      if (k == Scancode::mail)
        return SDL_SCANCODE_MAIL;
      if (k == Scancode::calculator)
        return SDL_SCANCODE_CALCULATOR;
      if (k == Scancode::computer)
        return SDL_SCANCODE_COMPUTER;
      if (k == Scancode::ac_search)
        return SDL_SCANCODE_AC_SEARCH;
      if (k == Scancode::ac_home)
        return SDL_SCANCODE_AC_HOME;
      if (k == Scancode::ac_back)
        return SDL_SCANCODE_AC_BACK;
      if (k == Scancode::ac_fwd)
        return SDL_SCANCODE_AC_FORWARD;
      if (k == Scancode::ac_stop)
        return SDL_SCANCODE_AC_STOP;
      if (k == Scancode::ac_refresh)
        return SDL_SCANCODE_AC_REFRESH;
      if (k == Scancode::ac_bookmarks)
        return SDL_SCANCODE_AC_BOOKMARKS;

      if (k == Scancode::brightnessDown)
        return SDL_SCANCODE_BRIGHTNESSDOWN;
      if (k == Scancode::brightnessUp)
        return SDL_SCANCODE_BRIGHTNESSUP;
      if (k == Scancode::displaySwitch)
        return SDL_SCANCODE_DISPLAYSWITCH;
      if (k == Scancode::kb_illum_toggle)
        return SDL_SCANCODE_KBDILLUMTOGGLE;
      if (k == Scancode::kb_illum_down)
        return SDL_SCANCODE_KBDILLUMDOWN;
      if (k == Scancode::kb_illum_up)
        return SDL_SCANCODE_KBDILLUMUP;
      if (k == Scancode::eject)
        return SDL_SCANCODE_EJECT;
      if (k == Scancode::sleep)
        return SDL_SCANCODE_SLEEP;

      if (k == Scancode::app1)
        return SDL_SCANCODE_APP1;
      if (k == Scancode::app2)
        return SDL_SCANCODE_APP2;

      if (k == Scancode::audio_rewind)
        return SDL_SCANCODE_AUDIOREWIND;
      if (k == Scancode::audio_fastforward)
        return SDL_SCANCODE_AUDIOFASTFORWARD;

      return SDL_SCANCODE_UNKNOWN;
    }
  }
}
