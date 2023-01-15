# Zoxel Makefile!
# can add --num-callers=60 to valgrind
# \todo atm: put all platform builds into this one file
#	make web, or make android, or make windows, or make dev, make is for make linux

# Declare compiler tools and flags
NAME := Zoxel
TARGET = zoxel
TARGET_DEV = dev
TARGET_WEB = zoxel.js
# used for cleaning
RM = rm
TARGET_WEB_WASM = zoxel.wasm
TARGET_WEB_DATA = zoxel.data
# our compilers
CC = cc						# Defines the compiler, cc for C code
CC_WEB = emcc				# the web compiler
# OBJS defines all the files used to compile the final Zoxel binary.
OBJS = include/flecs/flecs.c src/main.c
# This collects all c and h files in the directory
SRCS = $(shell find src/ -type f -name *.c)
SRCS += $(shell find src/ -type f -name *.h)
# our compiler properties
CFLAGS = -std=c99			# Specificies c99 Standard
CFLAGS += -D_DEFAULT_SOURCE	# Needed for a few functions, will be fixed in the future
CFLAGS += -fPIC				# Position Independent Code https://stackoverflow.com/questions/5311515/gcc-fpic-option
# Add libraries
LDLIBS = -lGL				# OpenGL library
LDLIBS += -lSDL2			# SDL2 Library
LDLIBS += -lSDL2_image		# SDL2 Image Library
# LDLIBS += -s SDL2_IMAGE_FORMATS='["png"]' # "bmp",
LDLIBS += -lSDL2_mixer		# SDL2 Audio Library
LDLIBS += -lm				# for math.h
LDLIBS += -lpthread			# For threading
# used for manual sdl compiling on systems that don't have sdl lib in their package managers
LDLIBS += -L/usr/local/lib
LDLIBS += -Wl,-rpath=/usr/local/lib
# FOR RELEASE
CFLAGS_RELEASE = -O3				# Optimization level 3
CFLAGS_RELEASE += -flto=auto		# fuse linker plugin
CFLAGS_RELEASE += -D NDEBUG			# No Debugging
CFLAGS_RELEASE += -s				# strip - removes 70kb atm
# FOR DEBUG
CFLAGS_DEBUG = -Wall				# For Warnings
CFLAGS_DEBUG += -g					# Adds debugging info to executeable
# CFLAGS_DEBUG += -fsanitize=address # detects memory leaks as well
# web build flags
CFLAGS_WEB = --preload-file resources
CFLAGS_WEB += -s WASM=1
CFLAGS_WEB += -s FULL_ES3=1
CFLAGS_WEB += -s USE_WEBGL2=1
CFLAGS_WEB += -s MIN_WEBGL_VERSION=2
CFLAGS_WEB += -s MAX_WEBGL_VERSION=2
CFLAGS_WEB += -s ALLOW_MEMORY_GROWTH
CFLAGS_WEB += -s STACK_SIZE=365536
# libraries used in web build
LDLIBS_WEB = -lGL
LDLIBS_WEB += -lSDL
LDLIBS_WEB += -s USE_SDL=2
LDLIBS_WEB += -s USE_SDL_IMAGE=2
LDLIBS_WEB += -s SDL2_IMAGE_FORMATS='["png"]' # "bmp",
LDLIBS_WEB += -s USE_SDL_MIXER=2
# Completed build commands
MAKE_RELEASE = $(CC) $(CFLAGS) $(CFLAGS_RELEASE) -o $(TARGET) $(OBJS) $(LDLIBS)
MAKE_DEV = $(CC) $(CFLAGS) $(CFLAGS_DEBUG) -o $(TARGET_DEV) $(OBJS) $(LDLIBS)
MAKE_WEB_RELEASE = $(CC_WEB) $(CFLAGS) $(CFLAGS_WEB) -o $(TARGET_WEB) $(OBJS) $(LDLIBS_WEB)

# release
$(TARGET): $(SRCS) ## builds zoxel
	cd bash/flecs && ./install_flecs.sh
	$(MAKE_RELEASE)

# dev
$(TARGET_DEV): $(SRCS) ## builds zoxel
	cd bash/flecs && ./install_flecs.sh
	$(MAKE_DEV)

# web
$(TARGET_WEB): $(SRCS) ## builds zoxel
	cd bash/flecs && ./install_flecs.sh
	$(MAKE_WEB_RELEASE)

# builds for all platforms - this rebuilds everytime tho
all: $(SRCS)
	@echo "Begin Making All"
	cd bash/flecs && ./install_flecs.sh
	@echo "Making Native Release Build [$(TARGET)]"
	$(MAKE_RELEASE)
	@echo "Making Native Dev Build [$(TARGET_DEV)]"
	$(MAKE_DEV)
	@echo "Making Webasm Release Build [$(TARGET_WEB)]"
	$(MAKE_WEB_RELEASE)
	@echo "Finished Making All"

# Removes all build files
clean:
	@echo "Cleaning All Build Files"
	$(RM) $(TARGET)
	$(RM) $(TARGET_DEV)
	cd bash/flecs && ./remove_flecs.sh

# used to remove web?
# $(RM) $(TARGET_WEB)
# $(RM) $(TARGET_WEB_WASM)
# $(RM) $(TARGET_WEB_DATA)

# Runs zoxel release build
run:
	./$(TARGET)

# Runs zoxel dev build
run-dev:
	./$(TARGET_DEV)

# Runs zoxel dev build with valgrind
run-dev-debug:
	valgrind ./$(TARGET_DEV)

# Runs zoxel web release build
run-web:
	emrun --browser firefox web/index.html

android:
	bash/android/install.sh

android-dev:
	bash/android/install_debug.sh

android-dev-debug:
	bash/android/install_debug.sh
	bash/android/read_logcat.sh
	
packages:
	@echo "Installing Libaries: make gcc libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev"
	sudo apt install make gcc libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev

install: ## installs zoxel into /usr/games directory
	cd bash/install && ./install.sh

uninstall: ## uninstalls zoxel into /usr/games directory
	bash/install/uninstall.sh

install-web-builder:
	bash/web/install_emcc.sh

install-flecs:
	cd bash/flecs && ./install_flecs.sh

remove-flecs:
	cd bash/flecs && ./remove_flecs.sh

git-push: ## installs zoxel into /usr/games directory
	cd bash/git && ./git_push.sh

git-pull: ## installs zoxel into /usr/games directory
	cd bash/git && ./git_pull.sh

help:
	@echo "zoxel -> an open source voxel engine"
	@echo "  make			builds zoxel"
	@echo "  make <target>"
	@echo "    $(TARGET_DEV)	builds dev"
	@echo "    $(TARGET_WEB)	builds zoxel-web"
	@echo "    run			runs $(TARGET)"
	@echo "    run-dev		runs $(TARGET_DEV)"
	@echo "    run-dev-debug	runs valgrind $(TARGET_DEV)"
	@echo "    run-web		runs $(TARGET_WEB)"
	@echo "    clean		removes all build files"
	@echo "    install		installs zoxel"
	@echo "    uninstall		inuninstalls zoxel"
	@echo "    install-web-builder	installs emcc for web build"
	@echo "    android		builds & runs android release"
	@echo "    android-dev		builds & runs android debug"
	@echo "    android-dev-debug	builds & runs android debug with logcat"
	@echo "    packages		installs required libraries with apt-get"
	@echo "    git-pull		pulls latest git"
	@echo "    git-push		pushes git updates - if has ssh access"
	@echo "find latest at -> https://codeberg.org/deus/zoxel"

# CFLAGS += -D_POSIX_C_SOURCE=200112L
# dCFLAGS += -fsanitize=undefined
# LDLIBS += -flto-partition=none ?
# https://stackoverflow.com/questions/66350310/link-time-optimizations-in-cuda-11-what-are-they-and-how-to-use-them
# LDLIBS += -dlto	# Dynamic Link-Time Optimization
# @echo " Inside Directory: $(CURDIR)"
# . $(CURDIR)/bash/web/emsdk_env.sh
# web build threading
# LDLIBS += -s USE_PTHREADS=1
# LDLIBS += -s PTHREAD_POOL_SIZE=2
