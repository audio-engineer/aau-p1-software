# AAU P1 Software

## Setup

### Prerequisites

You need to have the following software installed on your development system:

- [`Clang`](https://clang.llvm.org/)
- [`CMake`](https://cmake.org/)
- [`Ninja`](https://ninja-build.org/)
- [`pkg-config`](https://www.freedesktop.org/wiki/Software/pkg-config/)
- [`OpenSSL`](https://www.openssl.org/)

If you are on Windows and use Windows Subsystem for Linux, consider running
the [`wsl-setup`](https://github.com/audio-engineer/wsl-setup) script to install all necessary and some additional
software.

### Editors/IDEs

#### Visual Studio Code

Create a `.vscode/cmake-kits.json` file and replace the values for `name`, `C` and `CXX` with a name for the
configuration and the paths to the Clang binaries on your system respectively.

```json
[
  {
    "name": "Some Clang Configuration",
    "compilers": {
      "C": "/some/path/to/clang",
      "CXX": "/some/path/to/clang++"
    },
    "preferredGenerator": {
      "name": "Ninja"
    }
  }
]

```
