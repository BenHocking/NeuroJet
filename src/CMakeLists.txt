#  Copyright (c) 2012 Ben hocking
#  All rights reserved.
#
#  @author <a href="mailto:benjaminhocking@gmail.com">Ashlie B. Hocking</a>
#
#  This file is part of NeuroJet.
#
#  NeuroJet is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published
#  by the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  NeuroJet is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with NeuroJet.  If not, see <http://www.gnu.org/licenses/lgpl.txt>.

add_subdirectory(main)
if(NOT SKIPTESTS)
  enable_testing()
  add_subdirectory(test)
endif()
