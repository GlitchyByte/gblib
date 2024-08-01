# GlitchyByte C++ General Purpose Library

**WORK IN PROGRESS** - **WORK IN PROGRESS** - **WORK IN PROGRESS**

![Version](https://img.shields.io/badge/Version-0.1.0-blue)
![Version](https://img.shields.io/badge/20-white?logo=cplusplus&logoColor=00599C)

Converting embedded gblib I was using in other projects to a header-only library.

Include in your `CMakeLists.txt`:
```cmake
include(FetchContent)

# GBLib.
FetchContent_Declare(
    gblib
    GIT_REPOSITORY https://github.com/glitchybyte/gblib
    GIT_TAG main
)
FetchContent_MakeAvailable(gblib)

# ...and then add it to your `target_link_libraries`:

# Use PUBLIC or PRIVATE if it's a final binary.
target_link_libraries(${PROJECT_NAME} PRIVATE gblib)

# Use INTERFACE if it's a library.
target_link_libraries(${PROJECT_NAME} INTERFACE gblib)
```
