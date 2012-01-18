# Copyright (c) 2010-2012 Informed Simplifications, LLC
# All rights reserved.
#
# @since Jan 27, 2010
# @author <a href="mailto:benjaminhocking@gmail.com">Ashlie B. Hocking</a>
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

project(NeuroJet CXX)

include_directories(${NeuroJet_SOURCE_DIR})

set(FILES ${SRC} ${INCLUDES})

# Main executable
add_executable(NeuroJet ${FILES} ${NeuroJet_SOURCE_DIR}/neurojet_driver.cpp)
