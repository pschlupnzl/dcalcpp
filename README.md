# DysCalculator - C++

This is an implementation of DysCalculator aimed at embedded systems, e.g.
Espressif ESP32.

## Build: Docker

Because build the tooling requires a complete C++ compiler, the Dockerfile
creates a minimal Ubuntu container that can be used to build the project.

1. Build the container with

   ```
   $ docker build -t dcalcpp .
   ```

2. Run the container and finish on the bash prompt in the contianer. We mount
   the current directory to _/app_ in the container so it can be
   built with any changes made on the local machine.

   ```
   $ docker run --rm -it -v .:/app dcalcpp bash
   ```

3. Inside the container, build all _\*.cpp_ files, including tests.
   If you're using the _curses_ library for display, link it with

   ```
   # g++ *.cpp **/*.cpp -lncurses
   ```

4. Run the built application. You can do this either in the build container, or
   on the host command line after it has been built.

   ```
   # ./a.out
   ```

## Tests

We have copied as many tests from the React Native DysCalculator project as
possible, plus added a few new ones. The main difference is the display
precision on floating point numbers.

To run the tests, build as above and run with the `-t` option:

```
$ ./a.out -t
344 passed
```

## Platformio

There are [docker-compose.yml](./docker/docker-compose.yml) and
[webtop-compose.yml](./docker/webtop-compose.yml) compose files but I have not
yet been able to run Platformio in a container. The issues lie with Platformio
Home assuming that requests come from the same host (hence Webtop experiment)
and difficulties passing the `/dev/ttyUSB0` or `/dev/tty.usbserial-2130` devices
into the container.

# Flash to device

For this we currently use Arduino IDE running on a Linux machine that physically
connects to the device. Use the [dcalcpp.ino](dcalcpp.ino) file to launch.

# Emscripten

Use this to create WebAssembly suitable for browsers.

- Modify the [dcalcpp-wasm.cpp](./webassembly/dcalcpp-wasm.cpp) file to update
   the Web Assembly interface to the calculator.
- Modify the [dcalcpp.htm](./webassembly/dcalcpp.htm) template to update the
   JavaScript code or HTML layout.

We can run the Emscripten SDK in a container to build the Web Assembly files.

1. Run from the root directory of the project so the build has access to all
   files:

   ```
   $ docker run --rm -it -v $(pwd):/src -u $(id -u):$(id -g)  emscripten/emsdk bash
   ```

2. Inside the container, change to the _webassembly_ directory:

   ```
   # cd webassembly
   ```

3. Build all requisite files (this is also [build.sh](./webassembly/build.sh))

   ```
   # emcc -o dist.html --shell-file dcalcpp.htm -s NO_EXIT_RUNTIME=1 -s "EXPORTED_RUNTIME_METHODS=['ccall']" dcalcpp.cpp ../calc/*.cpp
   ```

4. In a separate process on the host, run a server from the _webassembly_
   folder

   ```
   $ cd webassembly
   $ python3 -m http.server
   ```

5. Now use a **Chrome** style browser that supports Web Assembly to load the
   calculator at http://localhost:8000/dist.html

Then inside the container, change direct
emcc helloworld.cpp -o helloworld.js
