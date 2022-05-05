TV4Mac is a proof-of-concept that demonstrates how support for USB TV receivers
on MacOS could be implemented with minimal effort by leveraging existing open
source solutions.

It consists of two parts:
1. A series of patches against webcamd 3.13.0.5. Webcamd is a compatibility
   framework that enables various drivers for webcams and TV receivers from the
   Linux kernel to be used as user-space drivers on FreeBSD. My patches allow
   webcamd to be built on MacOS as a shared library.
2. A series of patches against tvheadend (git revision d9dd70b1). Tvheadend is a
   TV server that exposes a standardized interface for tuning, streaming, and
   recording of live TV. My patches port tvheadend to MacOS (at the time of
   development, an official port was not yet available) and make it use webcamd
   for communication with the hardware.

The patches are distributed under the same license as the code that is being
modified.

In theory, TV4Mac supports all TV receiver hardware for which a driver is
available in the mainline Linux kernel, provided that it is connected via USB
and supports at least one digital TV standard. In practice, TV4Mac has only been
tested with a generic DVB-T stick using the RTL2832U chipset. The code has not
been tested on any recent version of MacOS and may not even compile any more.

Build instructions:
1. Install dependencies (assuming MacPorts):
   `sudo port install bsdmake gpatch libusb`
2. Download the webcamd source code from
   http://www.selasky.org/hans_petter/distfiles/webcamd-3.13.0.5.tar.bz2
   and apply the patches.
3. Run bsdmake in the webcamd source directory:
   `bsdmake patch`
   ``HAVE_MACOS=1 HAVE_DEBUG=1 CFLAGS=`pkg-config --cflags libusb-1.0` LDFLAGS=`pkg-config --libs libusb-1.0` bsdmake``
4. Clone the tvheadend repository (https://github.com/tvheadend/tvheadend),
   check out revision d9dd70b1, and apply the patches.
5. In the tvheadend source directory, run:
   `CFLAGS="-I webcamd-3.13.0.5/api" LDFLAGS="-L webcamd-3.13.0.5" PLATFORM=macos ./configure --disable-epoll --enable-kqueue`

For testing, you need a media player that supports the HTSP protocol of
tvheadend. I used the VLC plugin written by BtbN, available at:
https://github.com/BtbN/vlc-htsp-plugin
