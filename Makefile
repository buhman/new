.SUFFIXES:
.INTERMEDIATE:
.SECONDARY:
.PHONY: all clean

ifndef GLFW
$(error GLFW undefined)
endif

MAKEFILE_PATH := $(patsubst %/,%,$(dir $(abspath $(firstword $(MAKEFILE_LIST)))))

DEBUG = -g

CSTD += -std=gnu23
CXXSTD += -std=gnu++23

CFLAGS += -Wall -Werror -Wfatal-errors
CFLAGS += -Wno-error=unused-function
CFLAGS += -Wno-error=unused-variable
CFLAGS += -I$(MAKEFILE_PATH)/include
CFLAGS += -I$(dir $(GLFW))../include
CXXFLAGS += -fno-exceptions
LDFLAGS += -nostdlib++ -lm -Wl,-z noexecstack
ifeq ($(shell uname),Linux)
LDFLAGS += -static-libgcc
endif
ifeq ($(OS),Windows_NT)
LDFLAGS += -Wl,--subsystem,windows -mwindows
endif
ifeq ($(shell uname),Darwin)
LDFLAGS += -framework Foundation -framework IOKit -framework AppKit
endif

DEPFLAGS = -MMD -MP

OPT = -Og

all: main

makefile_path := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
as_obj_binary = $(subst -,_,$(subst .,_,$(subst /,_,$(subst .h,,$(1)))))
as_obj_binary_p = _binary_$(call as_obj_binary,$(1))

define BUILD_BINARY_S
	@echo build_binary_s $@
	@echo '.section .rodata.$(call as_obj_binary_p,$<)' > $@
	@echo '.global $(call as_obj_binary_p,$<)_start' >> $@
	@echo '.global $(call as_obj_binary_p,$<)_end' >> $@
	@echo '$(call as_obj_binary_p,$<)_start:' >> $@
	@printf '\t.incbin "$<"\n' >> $@
	@echo '$(call as_obj_binary_p,$<)_end:' >> $@
endef

define BUILD_BINARY_H
	@echo build_binary_h $@
	@echo '#pragma once' > $@
	@echo '' >> $@
	@echo '#include <stdint.h>' >> $@
	@echo '' >> $@
	@echo '#ifdef __cplusplus' >> $@
	@echo 'extern "C" {' >> $@
	@echo '#endif' >> $@
	@echo '' >> $@
	@echo 'extern uint32_t $(call as_obj_binary_p,$<)_start __asm("$(call as_obj_binary_p,$<)_start");' >> $@
	@echo 'extern uint32_t $(call as_obj_binary_p,$<)_end __asm("$(call as_obj_binary_p,$<)_end");' >> $@
	@echo '' >> $@
	@echo '#define $(call as_obj_binary,$<)_start ((const char *)&$(call as_obj_binary_p,$<)_start)' >> $@
	@echo '#define $(call as_obj_binary,$<)_end ((const char *)&$(call as_obj_binary_p,$<)_end)' >> $@
	@echo '#define $(call as_obj_binary,$<)_size ((uintptr_t)$(call as_obj_binary,$<)_end - (uintptr_t)$(call as_obj_binary,$<)_start)' >> $@
	@echo '' >> $@
	@echo '#ifdef __cplusplus' >> $@
	@echo '}' >> $@
	@echo '#endif' >> $@
endef

%.glsl.s: %.glsl
	$(BUILD_BINARY_S)

%.data.s: %.data
	$(BUILD_BINARY_S)

%.data.pal.s: %.data.pal
	$(BUILD_BINARY_S)

include/%.glsl.h: src/%.glsl
	$(BUILD_BINARY_H)

include/%.data.h: %.data
	$(BUILD_BINARY_H)

include/%.data.pal.h: %.data.pal
	$(BUILD_BINARY_H)

clean:
	rm -f *.o *.d *.gch
	rm -f main

%.o: %.s
	$(AS) $(AFLAGS) $< -o $@ $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXSTD) $(ARCH) $(CFLAGS) $(OPT) $(DEBUG) $(DEPFLAGS) -MF ${<}.d -c $< -o $@

%.o: %.c
	$(CC) $(CSTD) $(ARCH) $(CFLAGS) $(CXXFLAGS) $(OPT) $(DEBUG) $(DEPFLAGS) -MF ${<}.d -c $< -o $@

SHADERS = $(shell find src/ -type f -name '*.glsl')
SHADER_OBJS = $(patsubst %,%.o,$(SHADERS))
SHADER_HEADERS = $(subst src/,include/,$(patsubst %,%.h,$(SHADERS)))

shaders: $(SHADER_HEADERS)

MAIN_OBJS = \
	src/main.o \
	src/gl.o \
	src/make.o \
	src/shader.o \
	src/render.o \
	src/state.o \
	src/input.o \
	model/test_scene_color.data.o \
	$(SHADER_OBJS) \
	$(GLFW)

main: $(MAIN_OBJS) | shaders
	$(CXX) $^ $(CXXFLAGS) $(ARCH) -o $@ $(LDFLAGS)

-include $(shell find src/ -type f -name '*.d')

.SUFFIXES:
.INTERMEDIATE:
.SECONDARY:
.PHONY: all clean shaders

%: RCS/%,v
%: RCS/%
%: %,v
%: s.%
%: SCCS/s.%
