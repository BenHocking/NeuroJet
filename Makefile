# Copyright (c) 2010 Informed Simplifications, LLC
# All rights reserved.
#
# @since Jan 27, 2010
# @author <a href="mailto:benjaminhocking@gmail.com">Ashlie B. Hocking</a>

ifeq ($(TERM),msys)
	CMAKE_FLAGS = -G "MSYS Makefiles"
endif
ROOT_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
BLD := $(ROOT_DIR)/build
CACHE := $(BLD)/CMakeCache.txt
DBLD := $(ROOT_DIR)/debug.build
MBLD := $(ROOT_DIR)/mpi.build
DOC := $(ROOT_DIR)/doc
XBLD := $(ROOT_DIR)/build.xcode
VSBLD := $(ROOT_DIR)/build.vs

.PHONY: test clean xcode xcode2 vs

compile:
	if [ ! -d $(BLD) ]; then mkdir $(BLD); fi
	(cd $(BLD); cmake $(CMAKE_FLAGS) ..; make)
	rm -f NeuroJet; ln -s $(BLD)/NeuroJet NeuroJet

debug:
	if [ ! -d $(DBLD) ]; then mkdir $(DBLD); fi
	(cd $(DBLD); cmake -DDEBUG=true -DCMAKE_VERBOSE_MAKEFILE=true $(CMAKE_FLAGS) ..; make)
	rm -f NeuroJet_d; ln -s $(DBLD)/NeuroJet NeuroJet_d

mpi:
	if [ ! -d $(MBLD) ]; then mkdir $(MBLD); fi
	(cd $(MBLD); cmake -DMULTIPROC=true $(CMAKE_FLAGS) ..; make)

test: debug
	$(MAKE) -C $(DBLD) test

coverage: test
	rm -rf $(DOC)
	lcov --directory $(DBLD) --zerocounters
	(cd $(DBLD); ./ArgFunctsTest; ./NoiseTest; ./StringTest;)
	lcov --directory $(DBLD) --capture --output-file $(DBLD)/app.info
	mkdir $(DOC)
	(cd $(DOC); genhtml ../debug.build/app.info; open index.html)

clean:
	if [ -d $(BLD) ]; then rm -rf $(BLD); fi
	if [ -d $(DBLD) ]; then rm -rf $(DBLD); fi
	if [ -d $(MBLD) ]; then rm -rf $(MBLD); fi
	if [ -d $(DOC) ]; then rm -rf $(DOC); fi
	if [ -d $(XBLD) ]; then rm -rf $(XBLD); fi
	if [ -d $(VSBLD) ]; then rm -rf $(VSBLD); fi

xcode:
	rm -rf $(XBLD)
	mkdir $(XBLD)
	(cd $(XBLD); cmake -G"Xcode" ..; open NeuroJet.xcodeproj)

xcode2: compile
	if [ -e $(CACHE) ]; then rm $(CACHE); fi
	(cd $(BLD); cmake -Wno-dev -G"Xcode" ..)

vs:
	rm -rf $(VSBLD)
	mkdir $(VSBLD)
	(cd $(VSBLD); cmake -G"Visual Studio 9 2008" ..)