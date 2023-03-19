#!/bin/bash
# installs android project if it doesn't exist, and then copies latest source files over to it
source bash/android/gradle_set_paths.sh

if [[ -d $android_sdk_directory ]]; then
    echo "Android SDK Directory [$android_sdk_directory] exists. Nice work."
else
    echo "Please install android sdk & ndk to [$android_sdk_directory] directory."
    exit
fi
if [[ -d $android_directory ]]; then
    echo "[$android_directory] already exists. No need to create android project."
else
    echo "$android_directory does not exist. Building android project from SDL project."
    # copy sdl android project
    echo "Copying SDL androind project folder to zoxel directory."
    # cd $android_bash_directory
    source bash/android/android_copy_android_project.sh

    # copy sdl source files to android folder
    echo "Copying SDL source files to android build folder."
    # cd $android_bash_directory
    source bash/android/android_copy_sdl.sh

    echo "Copying zoxel settings to build directory."
    # cd $android_bash_directory
    source bash/android/android_copy_settings.sh
fi

echo "Copying zoxel source files to build directory."
# cd $android_bash_directory
source bash/android/android_copy_source.sh

# checks if android build directory exists, if it doesn't exist it will build it using sdl files.
# updates source files after that.
# remove last zoxel-android
# zoxel_directory=$PWD # "$HOME/zoxel"
# cd $zoxel_directory/bash/android