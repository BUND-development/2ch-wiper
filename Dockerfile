FROM debian:buster

### Prepare environment
RUN apt update \
  && apt upgrade -y \
  && apt install -y \
    autoconf \
    automake \
    autopoint \
    bash \
    bison \
    bzip2 \
    flex \
    g++ \
    g++-multilib \
    gettext \
    git \
    gperf \
    intltool \
    libc6-dev-i386 \
    libgdk-pixbuf2.0-dev \
    libltdl-dev \
    libssl-dev \
    libtool-bin \
    libxml-parser-perl \
    lzip \
    make \
    openssl \
    p7zip-full \
    patch \
    perl \
    pkg-config \
    python \
    ruby \
    sed \
    unzip \
    wget \
    xz-utils \
    build-essential \
    libx11-dev \
    libx11-xcb-dev \
    libxcursor-dev \
    libxrender-dev \
    libxrandr-dev \
    libxext-dev \
    libxi-dev \
    libxss-dev \
    libxt-dev \
    libxv-dev \
    libxxf86vm-dev \
    libxinerama-dev \
    libxkbcommon-dev \
    libfontconfig1-dev \
    libharfbuzz-dev \
    libasound2-dev \
    libpulse-dev \
    libdbus-1-dev \
    udev \
    mtdev-tools \
    webp \
    libudev-dev \
    libglm-dev \
    libwayland-dev \
    libegl1-mesa-dev \
    mesa-common-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libgles2-mesa \
    libgles2-mesa-dev \
    libproxy-dev \
    libgtk2.0-dev \
    libgtk-3-dev \
    libcups2-dev

### Building MXE toolchain
RUN cd /opt \
  && git clone https://github.com/mxe/mxe.git \
  && cd mxe \
  && git checkout tags/build-2019-06-02 \
  && make -j$(nproc --all) \
    qtbase \
    qtmultimedia

### Building static Qt toolchain
RUN cd /opt \
  && wget http://download.qt.io/archive/qt/5.13/5.13.0/single/qt-everywhere-src-5.13.0.tar.xz \
  && mv $(ls | grep qt-everywhere-src) qt.tar.xz \
  && tar xvf qt.tar.xz \
  && rm qt.tar.xz \
  && mv $(ls | grep qt-everywhere) qt-everywhere \
  && cd qt-everywhere \
  && ./configure \
    -static \
    -release \
    -silent \
    -prefix /opt/qt-static \
    -opensource \
    -confirm-license \
    -opengl \
    -nomake examples \
    -skip wayland \
    -skip purchasing \
    -skip serialbus \
    -skip qtserialport \
    -skip script \
    -skip scxml \
    -skip speech \
    -skip qtconnectivity \
    -qt-xcb \
    -qt-libpng \
    -no-libjpeg \
    -qt-zlib \
    -qt-pcre \
    -gtk \
    -qt-freetype \
    -qt-harfbuzz \
    -pulseaudio \
    -alsa \
  && make -r -j$(nproc --all) \
  && cd /opt/qt-everywhere \
  && make install \
  && rm -rf /opt/qt-everywhere


### Build application for Windows x86 & Linux X86_64
COPY /src /tmp/build
ENV PATH=/opt/mxe/usr/bin:$PATH
RUN cd /tmp/build \
  && /opt/mxe/usr/i686-w64-mingw32.static/qt5/bin/qmake \
  && make \
  && /opt/qt-static/bin/qmake \
  && make

### Finishing and cleanup
RUN cd /tmp/build \
  && mkdir /tmp/release \
  && cp release/wiper.exe /tmp/release/wiper_win32.exe \
  && cp wiper /tmp/release/wiper_linux64 \
  && cp -rf gui /tmp/release/gui \
  && cp -rf engine /tmp/release/engine \
  && touch /tmp/release/.config \
  && touch /tmp/release/proxies.cfg \
  && touch /tmp/release/texts.txt \
  && rm -rf /tmp/build \
  && echo "\n\n\033[1;33mBuilding complete. Don't forget to run \033[0;32m'docker cp qtbuilder:/tmp/release ./src'\033[1;33m in order to copy your files." \
  && echo "Have a very nice day~\033[0m\n"

### Complete!