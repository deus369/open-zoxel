#!/bin/bash

# Copies:
#   - [f] local.properties
#   - [f] app/build.gradle
#   - [f] app/src/main/AndroidManifest.xml
#   - [d] app/src/main/res
#   - [f] app/jni/Application.mk
#   - [f] app/jni/CMakeLists.txt
#   - [f] app/jni/src/Android.mk

# \todo copy resources into app/src/main/resources/ & rename to android-resources sub folder

source $PWD/bash/android/gradle_set_paths.sh

# copy settings files over

# copy local.properties
local_properties_source=$android_bash_directory/local.properties
local_properties_target=$android_directory/local.properties
echo "  > copying $local_properties_source to $local_properties_target"
rm $local_properties_target
cp $local_properties_source $local_properties_target

# copy app/build.gradle
android_gradle_file_source=$android_bash_directory/app/build.gradle
android_gradle_file_target=$android_directory/app/build.gradle
echo "  > copying $android_gradle_file_source to $android_gradle_file_target"
rm $android_gradle_file_target
cp $android_gradle_file_source $android_gradle_file_target

# copy manifesto
android_manifesto_file=$android_bash_directory/app/src/main/AndroidManifest.xml
android_manifesto_file_target=$android_directory/app/src/main/AndroidManifest.xml
echo "  > copying $android_manifesto_file to $android_manifesto_file_target"
rm $android_manifesto_file_target
cp $android_manifesto_file $android_manifesto_file_target

# copy res folder
android_res_directory=$android_bash_directory/app/src/main/res
android_res_directory_target=$android_directory/app/src/main/res
echo "  > copying $android_res_directory to $android_res_directory_target"
rm -R $android_res_directory_target
cp -R $android_res_directory $android_res_directory_target

# copy Application.mk, CMakeLists.txt, src/Android.mk
jni_directory=$android_bash_directory/app/jni
jni_directory_target=$android_directory/app/jni

file_1_name=Application.mk
file_1=$jni_directory/$file_1_name
file_1_target=$jni_directory_target/$file_1_name
echo "  > copying $file_1 to $file_1_target"
rm $file_1_target
cp $file_1 $file_1_target

file_2_name=CMakeLists.txt
file_2=$jni_directory/$file_2_name
file_2_target=$jni_directory_target/$file_2_name
echo "  > copying $file_2 to $file_2_target"
rm $file_2_target
cp $file_2 $file_2_target

file_3_name=src/Android.mk
file_3=$jni_directory/$file_3_name
file_3_target=$jni_directory_target/$file_3_name
echo "  > copying $file_3 to $file_3_target"
rm $file_3_target
cp $file_3 $file_3_target

echo "Finished copying settings files."
# sleep 3

# new_source_directory0=$android_directory/app/jni/src
# # our settings files
# old_source_directory1=$android_bash_directory/src
# old_source_directory2=$zoxel_directory/include

# our new locations
# old_source_file1=$zoxel_directory/Android.mk
# old_source_file2=$zoxel_directory/CMakeLists.txt
# new_source_file1=$new_source_directory0/Android.mk
# new_source_file2=$new_source_directory0/CMakeLists.txt