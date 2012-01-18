/***************************************************************************
 * Program.hpp
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
#if !defined(PROGRAM_HPP)
#   define PROGRAM_HPP

#  include <algorithm>
#  include <string>
#  include <vector>

#  if !defined(BINDLIST_HPP)
#    include "BindList.hpp"
#  endif
#  if !defined(CALC_HPP)
#    include "Calc.hpp"
#  endif
#  if !defined(FILTER_HPP)
#    include "Filter.hpp"
#  endif
#  if !defined(INTERNEURON_HPP)
#    include "neural/Interneuron.hpp"
#  endif
#  if !defined(NEURONTYPE_HPP)
#    include "neural/NeuronType.hpp"
#  endif
#  if !defined(PARALLELRAND_HPP)
#    include "ParallelRand.hpp"
#  endif
#  if defined(MULTIPROC) && !defined(POPULATION_HPP)
#    include "Population.hpp"
#  endif
#  if !defined(SYNAPSETYPE_HPP)
#    include "neural/SynapseType.hpp"
#  endif
#  if !defined(SYSTEMVAR_HPP)
#    include "SystemVar.hpp"
#  endif

/**************************************************************/
/* program class prototype */
/**************************************************************/
class program {
 public:
  program();
  ~program() { }
  static bool defaultsSet() { return areDefaultsSet; }
  // returns the number of set explicit Izhikevich variables
  int GetIzhExplicitCount() const;
  // returns maximum possible explicit Izhikevich variables
  int GetIzhExplicitMaxCount() const { return 6; }
  // TODO(bhocking): Get rid of magic number above

  void PrintHelp(const string& str = EMPTYSTR,
                 const string& func = EMPTYSTR) const;

  inline bool getNetworkCreated() const { return networkCreated; }
  inline void setNetworkCreated(bool nwCreated) { networkCreated = nwCreated; }

#   if defined(MULTIPROC)
  // Each node gets a seed based on its number
  // before we diverge te seeds, need to set up the shufflers
  inline void buildShuffleVectors(vector<unsigned int> &Shuffle,
                                  vector<unsigned int> &UnShuffle,
                                  unsigned int numNeurons) {
    chkNoiseInit();
    // create the shuffler
    for (unsigned int i = 0; i < numNeurons; i++) Shuffle.at(i) = i;
    // Shuffle needs to be within population
    for (PopulationCIt PCIt = Population::Member.begin();
         PCIt != Population::Member.end(); ++PCIt) {
      unsigned int lastNeuron = PCIt->getLastNeuron();
      for (unsigned int i = PCIt->getFirstNeuron(); i <= lastNeuron; ++i) {
        std::swap(Shuffle.at(i),
                  Shuffle.at(ShuffleNoise.RandInt(i, lastNeuron)));
      }
    }
    // create the unshuffler
    for (unsigned int i = 0; i < numNeurons; i++) {
      UnShuffle.at(Shuffle.at(i)) = i;
    }
  }
#   endif
  inline void chkNoiseInit() {
    if (!isNoiseInit) setAllSeeds();
  }
  inline unsigned int chooseItem(unsigned int listSize) {
    chkNoiseInit();
    return PickNoise.RandInt(0, listSize - 1);
  }
  inline double chooseWeightedItem() {
    chkNoiseInit();
    return PickNoise.Uniform(0.0L, 1.0L);
  }
  inline unsigned int getConnectNoise(unsigned int start, unsigned end) {
    chkNoiseInit();
    return ConnectNoise.RandInt(start, end);
  }
  inline unsigned int getExtRandInt(unsigned int start, unsigned end) {
    chkNoiseInit();
    return ExternalNoise.RandInt(start, end);
  }
  inline bool getExtBernoulli(double p) {
    chkNoiseInit();
    return ExternalNoise.Bernoulli(p);
  }
  inline unsigned int getResetNeuron(unsigned int numNeurons) {
    chkNoiseInit();
    return ResetNoise.RandInt(0, numNeurons - 1);
  }
  inline double getWeightNormal(double mu, double sigma) {
    chkNoiseInit();
    return WeightNoise.Normal(mu, sigma);
  }
  inline double getWeightUniform(double low, double high) {
    chkNoiseInit();
    return WeightNoise.Uniform(low, high);
  }
  inline unsigned int pickTie(unsigned int numTies) {
    chkNoiseInit();
    return TieBreakNoise.RandInt(0, numTies - 1);
  }
  void setAllSeeds();
  static LearningRuleType parseLearningRuleType(string lrt);
  static ThresholdType parseThresholdType(string tt);
  static void setDefaults(unsigned int ni);

  inline static program& Main() { return *mainPgm; }
  inline static void initMain() { mainPgm = new program(); }

 private:
  bool networkCreated;          // a flag indicating network creation
  static program *mainPgm;

  static bool areDefaultsSet;
  bool isNoiseInit;
  Noise ConnectNoise;             // rng for connectivity generation
  Noise ExternalNoise;            // rng for noise in the externals
  Noise PickNoise;                // rng for choosing between sequences
  Noise ResetNoise;               // rng for creating random Z0
#   if defined(MULTIPROC)
  Noise ShuffleNoise;             // rng for shuffling neurons
#   endif
  Noise TieBreakNoise;            // rng for tie breaking
  Noise WeightNoise;              // rng for generating weights
  // Synapse.hpp also has SynNoise

  void LoadLists();
};

/**************************************************************/
/* AtFunction Prototypes */
/**************************************************************/
string GetArg(string to_find, ArgListType &arg);
void Echo(ArgListType &arg);
void Print(ArgListType &arg);
void PrintVar(ArgListType &arg);
#if !defined(WIN32)
void System(ArgListType &arg);
#endif

#endif
