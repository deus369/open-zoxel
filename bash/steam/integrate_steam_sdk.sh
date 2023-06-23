#!/bin/bash

include_path="include/steam"
steam_sdk_path="~/steam_sdk"

if [ ! -d "$steam_sdk_path" ]; then
    echo "  ! steam sdk directory [$steam_sdk_path]"
    exit 1
fi

if [ ! -d "$include_path" ]; then
    echo "  > created steam sdk include path [$include_path]"
    mkdir $include_path
fi