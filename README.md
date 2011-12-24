Overview
========

The goal of the NeuroJet project is a technology suitable for studying the biological basis of cognition. The software helps neuroscientists produce quantitative predictions at a variety of levels;
from the firing patterns of individual neurons to the cognition and behavior of individual organisms. NeuroJet is a neural network simulator initially designed for simulations of the mammalian
hippocampus.

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
