ARCHS=armv7 armv7s arm64
TARGET = iphone:clang:latest:6.0

include theos/makefiles/common.mk

TOOL_NAME = jetsamctl
jetsamctl_CFLAGS = -Wall -Wpedantic -Wextra
jetsamctl_FILES = main.c

include $(THEOS_MAKE_PATH)/tool.mk

before-all::
	@if [ ! -f "$(THEOS_INCLUDE_PATH)/sys/kern_memorystatus.h" ]; then \
		mkdir -p "$(THEOS_INCLUDE_PATH)/sys"; \
		curl -s -o "$(THEOS_INCLUDE_PATH)/sys/kern_memorystatus.h" -L "http://www.opensource.apple.com/source/xnu/xnu-2782.1.97/bsd/sys/kern_memorystatus.h?txt"; \
	fi
