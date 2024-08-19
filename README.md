The Sensor Watch
================

The [Sensor Watch](https://www.sensorwatch.net) is a board replacement for the classic Casio F-91W wristwatch. It is powered by a Microchip SAM L22 microcontroller with built-in segment LCD controller. You can write your own programs for the watch using the provided watch library, program the watch over USB using the built-in UF2 bootloader, and then install the board in your existing watch case to run your own software on your wrist.

![image](/images/sensor-watch.jpg)

Casiopeia
---------

This repository is a fork from the original sensorwatch repository that adds a complication (aka watch face) that only displays quotes from the novel [Momo](https://en.wikipedia.org/wiki/Momo_(novel)). The selected quotes are all messages from the turtle Kassiopeia.

The code for the complication can be found in the [casiopeia_face.h](/movement/watch_faces/complication/casiopeia_face.h) and [casiopeia_face.c](/movement/watch_faces/complication/casiopeia_face.c) files.

Development
--------

You will need to install [the GNU Arm Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads/) to build projects for the watch. The watch library has been tested with the `9-2019-q4-major` version and the `10.3-2021.07` versions. If you're using Debian or Ubuntu, it should be sufficient to `apt install gcc-arm-none-eabi`.

To build your project, open your terminal and navigate to the project's `make` folder, then type `make`.

To install the project onto your Sensor Watch board, plug the watch into your USB port and double tap the tiny Reset button on the back of the board. You should see the LED light up red and begin pulsing. (If it does not, make sure you didn’t plug the board in upside down). Once you see the “WATCHBOOT” drive appear on your desktop, type `make install`. This will convert your compiled program to a UF2 file, and copy it over to the watch.

Using the Movement framework
----------------------------
If you just want to make minor modifications and use existing code, start with the `movement` directory. You can build the default watch firmware with:

```
cd movement/make
make
```

Then copy `movement/make/build/watch.uf2` to your watch. If you'd like to modify which faces are built, see `movement_config.h`.

You may want to test out changes in the emulator first. To do this, you'll need to install [emscripten](https://emscripten.org/), then run:

```
cd movement/make
emmake make
python3 -m http.server -d build-sim
```

Finally, visit [watch.html](http://localhost:8000/watch.html) to see your work.

License
-------
Different components of the project are licensed differently, see [LICENSE.md](https://github.com/joeycastillo/Sensor-Watch/blob/main/LICENSE.md).
