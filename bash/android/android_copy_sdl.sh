#!/bin/bash

# copies SDL2 and SDL_image src, include and Application into android project

source gradle_set_paths.sh

# sdl - base
sdl2_directory=$HOME/SDL/SDL2
sdl2_1=$sdl2_directory/src
sdl2_2=$sdl2_directory/include
sdl2_3=$sdl2_directory/Android.mk
# image
sdl2_image_directory=$HOME/SDL/SDL2_image
# mixer
sdl2_mixer_directory=$HOME/SDL/SDL2_mixer
sdl2_mixer_directory_src=$sdl2_mixer_directory/src
sdl2_mixer_directory_include=$sdl2_mixer_directory/include
# destination - where to copy to
destination_directory=$android_directory/app/jni # /src
destination_directory1=$destination_directory/SDL2
destination_directory2=$destination_directory/SDL2_image
destination_directory3=$destination_directory/SDL2_mixer

# first remove source files from new directory
echo Removing old SDL2 and SDL2_image
rm -R $destination_directory1
rm -R $destination_directory2
rm -R $destination_directory3

echo Making directory 1 $destination_directory1
mkdir $destination_directory1
echo Copying $sdl2_directory to $destination_directory1
cp -R $sdl2_1 $destination_directory1
cp -R $sdl2_2 $destination_directory1
cp -R $sdl2_3 $destination_directory1

echo Making directory 2 $destination_directory2
mkdir $destination_directory2
echo Copying $sdl2_image_directory to $destination_directory2
cp $sdl2_image_directory/* $destination_directory2

echo Making directory 3 $destination_directory3
mkdir $destination_directory3
echo Copying $sdl2_mixer_directory to $destination_directory3
cp $sdl2_mixer_directory/* $destination_directory3
echo Copying $sdl2_mixer_directory_src to $destination_directory3
cp -R $sdl2_mixer_directory_src $destination_directory3
echo Copying $sdl2_mixer_directory_include to $destination_directory3
cp -R $sdl2_mixer_directory_include $destination_directory3

echo Finished copying SDL files
# sleep 2
# echo Make sure to set AndroidBuild in include file at $new_source_directory0/src
sleep 3 # 15
