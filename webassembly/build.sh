# !/bin/bash
emcc \
    -o dist.html \
    --shell-file dcalcpp.htm \
    -s NO_EXIT_RUNTIME=1 \
    -s "EXPORTED_RUNTIME_METHODS=['ccall']" \
    dcalcpp.cpp ../calc/*.cpp