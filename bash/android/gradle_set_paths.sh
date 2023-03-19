#!/bin/bash
# called from zoxel directory

zoxel_directory="$PWD"
echo "Gradle set paths at zoxel directory [$zoxel_directory]"
# zoxel_directory="$HOME/zoxel"
export zoxel_directory="$zoxel_directory"
android_directory="$zoxel_directory/build/android-build"
android_bash_directory="$zoxel_directory/bash/android"

# ndk installation path
android_sdk_directory=$zoxel_directory/build/android # "$HOME/android"
ANDROID_NDK_HOME="$android_sdk_directory/ndk"
export ANDROID_NDK_HOME="$android_sdk_directory/ndk"
# Android installation path
ANDROID_HOME="$android_sdk_directory/sdk" # "/usr/lib/android-sdk"
export ANDROID_HOME="$android_sdk_directory/sdk" # "/usr/lib/android-sdk"
ANDROID_SDK_ROOT="$ANDROID_HOME" # "/usr/lib/android-sdk"
export ANDROID_SDK_ROOT="$ANDROID_HOME" #"/usr/lib/android-sdk"

# SDL
sdl_directory="$zoxel_directory/build/sdl/sdl" # "$HOME/SDL/SDL2"
export sdl_directory="$zoxel_directory/build/sdl/sdl" # "$HOME/SDL/SDL2"
sdl_android_project_directory="$sdl_directory/android-project"
sdl_image_directory="$zoxel_directory/build/sdl/sdl_image"  # $HOME/SDL/SDL2_image
sdl_mixer_directory="$zoxel_directory/build/sdl/sdl_mixer"  # $HOME/SDL/SDL2_mixer