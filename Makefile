# Copyright (c) 2010 Informed Simplifications, LLC
# All rights reserved.
#
# @since Jan 27, 2010
# @author <a href="mailto:benjaminhocking@gmail.com">Ashlie B. Hocking</a>

ifeq ($(TERM),msys)
	CMAKE_FLAGS = -G "MSYS Makefiles"
endif
BLD=build
CACHE=build/CMakeCache.txt

compile:
	if [ ! -d $(BLD) ]; then mkdir $(BLD); fi
	(cd $(BLD); cmake $(CMAKE_FLAGS) ..; make)
	rm NeuroJet; ln -s $(BLD)/NeuroJet NeuroJet

DBLD=debug.build
debug:
	if [ ! -d $(DBLD) ]; then mkdir $(DBLD); fi
	(cd $(DBLD); cmake -DDEBUG=true -DCMAKE_VERBOSE_MAKEFILE=true $(CMAKE_FLAGS) ..; make)
	rm NeuroJet_d; ln -s $(DBLD)/NeuroJet NeuroJet_d

MBLD=mpi.build
mpi:
	if [ ! -d $(MBLD) ]; then mkdir $(MBLD); fi
	(cd $(MBLD); cmake -DMULTIPROC=true $(CMAKE_FLAGS) ..; make)

test:
	if [ ! -d $(BLD) ]; then $(MAKE) compile; fi
	$(MAKE) -C $(BLD) test

clean:
	if [ -d $(MBLD) ]; then rm -rf $(MBLD); fi
	if [ -d $(DBLD) ]; then rm -rf $(DBLD); fi
	if [ -d $(BLD) ]; then rm -rf $(BLD); fi

XBLD=build.xcode
xcode:
	rm -rf $(XBLD)
	mkdir $(XBLD)
	(cd $(XBLD); cmake -G"Xcode" ..; open NeuroJet.xcodeproj)

xcode2: 
	if [ ! -d $(BLD) ]; then $(MAKE) compile; fi
	if [ -e $(CACHE) ]; then rm $(CACHE); fi
	(cd $(BLD); cmake -Wno-dev -G"Xcode" ..)

VSBLD=build.vs
vs:
	rm -rf $(VSBLD)
	mkdir $(VSBLD)
	(cd $(VSBLD); cmake -G"Visual Studio 9 2008" ..)