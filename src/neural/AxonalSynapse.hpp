/***************************************************************************
 * Synapse.hpp
 *
 *  Copyright 2005, 2009, 2011 Informed Simplifications, LLC
 *  This file is part of NeuroJet.
 *
 *  NeuroJet is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  NeuroJet is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with NeuroJet.  If not, see <http://www.gnu.org/licenses/lgpl.txt>.
 ****************************************************************************/

#if !defined(AXONALSYNAPSE_HPP)
#  define AXONALSYNAPSE_HPP

#  include <deque>
#  include <vector>

#  if !defined(ARGFUNCTS_HPP)
#    include "ArgFuncts.hpp"
#  endif
#  if !defined(SYNAPSETYPE_HPP)
#    include "SynapseType.hpp"
#  endif
#  if !defined(DENDRITICSYNAPSE_HPP)
#    include "DendriticSynapse.hpp"
#  endif

// AxonalSynapse = Synapse on axon
class AxonalSynapse {
 public:
  inline AxonalSynapse(): synapse(NULL) {}
  inline void connectSynapse(unsigned int destNeuron,
                             DendriticSynapse &dendritic,
                             SynapseType const* synType,
                             bool isExc = true, bool isInhDiv = false) {
    synapse = &dendritic;
    synapse->connectNeuron(destNeuron, synType, isExc, isInhDiv);
  }
  inline bool connectsTo(const DendriticSynapse &dendritic) const {
    return synapse == &dendritic;
  }
  inline void activate(DataList &bus, DataList &bus_inhdiv,
                       DataList &bus_inhsub, const int timeStep) {
    synapse->activate(bus, bus_inhdiv, bus_inhsub, timeStep);
  }
  inline float getWeight() const { return synapse->getWeight(); }
  inline void setWeight(const float toSet) { synapse->setWeight(toSet); }
  inline int getLastActivate() const { return synapse->getLastActivate(); }
  inline void setLastActivate(const int toSet) {
    synapse->setLastActivate(toSet);
  }

 private:
  DendriticSynapse *synapse;  // Points to corresponding DendriticSynapse
};

typedef AxonalSynapse * AxonalSegment;
typedef AxonalSegment * Axon;
typedef AxonalSegment const * const AxonConst;

#endif  // AXONALSYNAPSE_HPP
