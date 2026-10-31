[![C++20](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![License](https://img.shields.io/github/license/staromodny/windower)](https://github.com/staromodny/windower/blob/master/LICENSE)
[![Release](https://img.shields.io/github/v/release/staromodny/windower)](https://github.com/staromodny/windower/releases)
[![GitHub issues](https://img.shields.io/github/issues/staromodny/windower?color=purple)](https://github.com/staromodny/windower/issues)
# windower
Windower is a lightweight C++23 Win32 window wrapper designed to simplify native Windows window management.

# Requirements
- Windows (features dependent on versions).
- C++23 compatible compiler.
- [CMake 3.25](https://cmake.org/cmake/help/latest/release/3.25.html) or newer
- Windows SDK

# Installation
Windower can be included in your CMake project using FetchContent.
Add the following to your CMakeLists.txt:
```cmake
include(FetchContent)

FetchContent_Declare(
    Windower
    GIT_REPOSITORY https://github.com/staromodny/Windower.git
    GIT_TAG 1.0
)

FetchContent_MakeAvailable(Windower)

target_link_libraries(${PROJECT_NAME} PRIVATE Windower::Windower)
```

# Usage
To build the examples, enable the `WINDOWER_BUILD_EXAMPLES` CMake option. \
Example usage is in `examples/main.cpp`


# License
Windower is licensed under the **MIT** License.

See the [LICENSE](LICENSE) file for details.
