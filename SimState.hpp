/***************************************************************************
 * SimState.hpp
 *
 *  Copyright 2011 Informed Simplifications, LLC
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
#if !defined(SIMSTATE_HPP)
#  define SIMSTATE_HPP

#  include <vector>

#  if !defined(DATATYPES_HPP)
#    include "DataTypes.hpp"
#  endif
#  if !defined(NOISE_HPP)
#    include "Noise.hpp"
#  endif
#  if !defined(INTERNEURON_HPP)
#    include "neural/Interneuron.hpp"
#  endif

class SimState {
 private:
  UIVectorDeque Fired;  // Not just last time step, but enough for
  // axonal delays
  Interneuron feedbackInterneuron;
  Interneuron feedforwardInterneuron;
  float* IzhV;    // Only necessary for Izh model
  float* IzhU;    // Only necessary for Izh model
  std::vector<DataList> lastActivate;  // Time that synapse was last activated
                                       // (Necessary for zbar)
  int timeStep;   // Don't want to mess up periodic functions
                  // or lastActivate!
  // Dendritic queues are only necessary with dendritic filter
  std::vector<float*> dendriteQueue;
  std::vector<float*> dendriteQueue_inhdiv;
  std::vector<float*> dendriteQueue_inhsub;
  DataList somaExc;             // Only necessary for old I&F model
  // I.e., when (fabs(yDecay) > verySmallFloat)
  float* VarKConductanceArray;  // Only necessary for old I&F model
#if defined(MULTIPROC)
  Noise LocalSynNoise;
#endif
  Noise ExternalNoise;
  Noise PickNoise;
  Noise ResetNoise;
  Noise SynNoise;
  Noise TieBreakNoise;
  // Not include ConnectNoise, ShuffleNoise, WeightNoise
  // These Noises should affect initial setup only
};

#endif
