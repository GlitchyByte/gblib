# GlitchyByte C++ General Purpose Library

**WORK IN PROGRESS** - **WORK IN PROGRESS** - **WORK IN PROGRESS**

![Version](https://img.shields.io/badge/Version-0.1.0-blue)
![Version](https://img.shields.io/badge/20-white?logo=cplusplus&logoColor=00599C)

Converting embedded gblib I was using in other projects to a header-only library.

Include in your `CMakeLists.txt`:
```cmake
# GBLib.
FetchContent_Declare(
    gblib
    GIT_REPOSITORY https://github.com/glitchybyte/gblib
    GIT_TAG main
)
FetchContent_MakeAvailable(gblib)

# ...and in then add it to your `target_link_libraries`:

target_link_libraries(${PROJECT_NAME} PRIVATE gblib)
```
