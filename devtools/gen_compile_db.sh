#!/bin/sh

rm -f ./compile_commands.json
cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
cd .. && ln -s build/compile_commands.json .
