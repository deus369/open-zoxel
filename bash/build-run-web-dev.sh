#!/bin/bash

cd ~/projects/open-zoxel

source ~/emsdk/emsdk_env.sh

make -f MakeWebDev -B -j24

emrun --browser firefox web-demo-dev/zoxel-dev.html

echo Done

sleep 30