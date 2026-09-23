# Karel and SPL

## Prerequisites

### Install libraries on Linux (dpkg-based, e.g. Debian/Ubuntu)

    sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev

### Install on Windows 64-bit

Libraries are already bundled in lib/

### Install brew and libraries on MacOSX

If you don't have brew installed yet:

    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

Then:

    brew install sdl sdl2_image sdl2_ttf

## Add new targets

Additional targets can be added in **CMakeLists.txt**

Two example targets already exist: Pacman and LivingRoomKarel

For additional targets using the SPL interface add a line

    add_spl_target(name src1.c src2.c ...)

with //name// being the resulting executable and src1.c src2.c ... being the source files of the project.

Similarly, for Karel the Robot projects, add a line

    add_karel_target(name src1.c src2.c ...)

with //name// being the resulting executable and src1.c src2.c ... being the source files of the project.

## Method 1: Compile and run on command line

    cmake .
    make
    ./target

## Method 2: Compile and run with CLion

First, checkout the project on CLion startup from the following GIT URL:

    https://github.com/fwheinz/karelspl

Then, choose the correct target from the dropdown list at the top of the window.

Finally, press the green play symbol at the top of the window.
