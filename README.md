rolmodl is a C++ API for the [Simple Directmedia Library 2 (SDL2)](https://www.libsdl.org/). The overhead is minimized, as long as this does not compromise other design goals. In general, the abstraction layer varies in depth quite a lot.

### Design goals
1. rolmodl exposes risks to the consumer and makes it non-trivial to make mistakes
    * Type safety
    * Compile-time checks

1. rolmodl does not add new features
1. rolmodl implements all features of SDL2

## Status

The doxygen documentation is available at https://maximsmol.github.io/rolmodl/.

An [example program](src/tst/main.cpp) is available that showcases some of the features and their inteded use.

The [todo files](src/rolmodl/hpp/todo) list the features that were left unimplemented in the meantime. The [project todo](todo.txt) is a developer reference of porject goals and shorcomings

### Features:
- Window class
- Renderer class
- Texture class
- Semantic geometric classes
- C++ class enums in place of C enums and defines
- Pixel format factory
- Mouse state polling
- Keyboard state polling
- Event polling
- Mouse cursor control
- System information polling
- Exception-based error handling

### Planned Features:
- OpenGL support
- Surface class
- Clipboard support
- Vulkan support
- SDL initialization and finalization support
- Hint support
- Log support
- SDL assertion support
- Version query support
- $ gestures support
- Text input support
- toString for enums
- Joystick support
- Game controller support
- Haptic support
- Audio support
- SDL thread support
- SDL synchronization support
- SDL atomics support
- SDL timers support
- SDL filesystem abstraction support
- SDL file I/O support
- SDL shared object tools support
- SDL byte tools support
- SDL bit order tools support
- Platform-specific functions support
- SDL stdlib aliases
- See the [todo files](src/rolmodl/hpp/todo)

## Building
rolmodl uses [Shake](https://shakebuild.com/). It depends on [SDL2](https://www.libsdl.org/).

After installing the dependencies, simply run `./build.sh` from the project folder.

## Manual
Include the rolmodl header: [`./src/rolmodl/hpp/Rolmodl.hpp`](src/rolmodl/hpp/Rolmodl.hpp). Link with the rolmodl shared library `./dist/dbg/librolmodl.dylib` (or your OS equivalent).

Use `./run.sh` to run the test application.
