Overview
========

The goal of the NeuroJet project is a technology suitable for studying the biological basis of cognition. The software helps neuroscientists produce quantitative predictions at a variety of levels;
from the firing patterns of individual neurons to the cognition and behavior of individual organisms. NeuroJet is a neural network simulator initially designed for simulations of the mammalian
hippocampus.

Compiling
=========

In order to compile this code, you need to install gtest, and have the GTEST_ROOT environment variable point to its installation directory. It can be downloaded via SVN at
http://googletest.googlecode.com/svn/trunk, via:

  svn co http://googletest.googlecode.com/svn/trunk gtest-svn

Currently, there are many steps to compiling, and there might be some unknown dependencies with the test code. If you run into an error during compilation of the tests, check to see if
build/src/NeuroJet (or build.xcode/src/Debug/NeuroJet, etc.) exists. That file should be enough to get you started.

In order to run coverage, you will also need to install lcov and make sure it is in your path. For directions on installing lcov, see:

  http://ltp.sourceforge.net/coverage/lcov/readme.php

Authors
-------
* Per Sederberg
* Matthew Harrison
* Scott Witherell
* Aaron Shon
* Paul Rodriguez
* Dave Sullivan
* Ben Hocking
* Aprotim Sanyal
* Aleksandr Gershaft
* Andrew Howe
* Blake Thomas
* (et al.)

Code Coverage
=============

In order to check how well the tests cover the code, you first need to install lcov:
http://ltp.sourceforge.net/coverage/lcov.php
Once this is installed (and in your path), "make coverage" should generate and open an html file displaying coverage. This has currently only been tested on Mac OS X (Lion). For Jenkins/Hudson, lcov
can also be used to generate XML files compatible with the Cobertura plug-in.
