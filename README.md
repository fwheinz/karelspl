# Karel and SPL

## Install on Linux (dpkg-based, e.g. Debian/Ubuntu)

    sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
    cmake .
    make

## Install on Windows 64-bit

    cmake .
    make

## Install on MacOSX

If you don't have brew installed yet:

    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

Then:

    brew install sdl sdl2_image sdl2_ttf
    cmake .
    make

