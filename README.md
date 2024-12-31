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

3. Inside the container, build all _\*.cpp_ files, e.g.

   ```
   # g++ *.cpp
   ```

   If you're using the _curses_ library for display, link it with

   ```
   # g++ *.cpp -lncurses
   ```

4. Run the built application

   ```
   # ./a.out
   ```

## Flash to device

For this we currently use Arduino IDE. We may also be able to use
PlatformIO or the Espressif ESP-IDF. Not sure yet.
