# TARGET #

TARGET := 3DS
LIBRARY := 0

ifeq ($(TARGET),$(filter $(TARGET),3DS WIIU))
    ifeq ($(strip $(DEVKITPRO)),)
        $(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro")
    endif
endif

# COMMON CONFIGURATION #

NAME := tangerine

BUILD_DIR := build
OUTPUT_DIR := output
INCLUDE_DIRS := include
SOURCE_DIRS := source source/pp2d source/formats

LIBRARY_DIRS := $(DEVKITPRO)/portlibs/armv6k
LIBRARIES := mpg123 citro3d ctru m

BUILD_FLAGS := -Wno-misleading-indentation -Wno-strict-aliasing
BUILD_FLAGS_CC :=
BUILD_FLAGS_CXX :=
RUN_FLAGS :=

VERSION_PARTS := $(subst ., ,$(shell git describe --tags --abbrev=0))

VERSION_MAJOR := $(word 1, $(VERSION_PARTS))
VERSION_MINOR := $(word 2, $(VERSION_PARTS))
VERSION_MICRO := $(word 3, $(VERSION_PARTS))

TITLE := $(NAME)
DESCRIPTION := An audio player
AUTHOR := tesnos6921

# 3DS CONFIGURATION #

ifeq ($(TARGET),3DS)
    LIBRARY_DIRS += $(DEVKITPRO)/libctru

    PRODUCT_CODE := CTR-P-TANG
    UNIQUE_ID := 0xFAF5A

    CATEGORY := Application
    USE_ON_SD := true

    MEMORY_TYPE := Application
    SYSTEM_MODE := 64MB
    SYSTEM_MODE_EXT := Legacy
    CPU_SPEED := 268MHz
    ENABLE_L2_CACHE := true

    ICON_FLAGS := --flags visible,ratingrequired,recordusage --cero 128 --esrb 128 --usk 128 --pegigen 128 --pegiptr 128 --pegibbfc 128 --cob 128 --grb 128 --cgsrr 128

    ROMFS_DIR := romfs
    BANNER_AUDIO := buildfiles/jingle.wav
    BANNER_IMAGE := buildfiles/model.cgfx
    ICON := buildfiles/logo.png
endif

# INTERNAL #

include buildtools/make_base