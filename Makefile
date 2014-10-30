

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CXX = clang++
endif

GH_PATH=/opt/oblong/greenhouse
PATH:=/opt/oblong/deps-64-9/bin:$(PATH)
GH_PKG_CONFIG=$(PKG_CONFIG_PATH):$(GH_PATH)/lib/pkgconfig pkg-config

CXXFLAGS = -Wempty-body \
  -fdiagnostics-show-option \
  -Wall \
  -Wpointer-arith \
  -Wno-shadow \
  -Wwrite-strings \
  -Wno-overloaded-virtual \
  -Wno-strict-overflow \
  -Wno-error=unused-variable \
  -Werror \
  `PKG_CONFIG_PATH=$(GH_PKG_CONFIG) --cflags --static libGreenhouse` \
  -g -O3 -std=gnu++0x

LDFLAGS=`PKG_CONFIG_PATH=$(GH_PKG_CONFIG) --libs --static libGreenhouse`

esc=`echo "\033"`
NO_COLOR=${esc}[0m
OK_COLOR=${esc}[36;21m

all: robot
clean:
	-rm robot

robot: robot.cpp Robot.hpp
	${CXX} $< -lGreenhouse ${LDFLAGS} ${CXXFLAGS} -lboost_thread -lboost_system -lboost_filesystem -o $@
	@echo	"$(OK_COLOR)$$EXP_STR$(NO_COLOR)"

