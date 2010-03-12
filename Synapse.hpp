/***************************************************************************
*            Synapse.hpp
*
*  Mon Apr 18 16:50:23 2005
*  Copyright  2005-2009
****************************************************************************/

#if !defined(SYNAPSE_HPP)
#  define SYNAPSE_HPP
#  if !defined(ARGFUNCTS_HPP)
#    include "ArgFuncts.hpp"
#  endif
#  if !defined(NOISE_HPP)
#    include "Noise.hpp"
#  endif
#  if !defined(SYNAPSETYPE_HPP)
#    include "SynapseType.hpp"
#  endif
#  if defined(RNG_BUCKET) && !defined(PARALLELRAND_HPP)
#    include "ParallelRand.hpp"
#  endif
#  include <vector>
class AxonalSynapse;

// DendriticSynapse = Synapse on dendritic tree
class DendriticSynapse {
public:
   inline DendriticSynapse():
     m_isExc(true), m_isInhDiv(false), m_srcNeuron(0), m_destNeuron(0), m_weight(0.0f), m_riseUntil(0),
     m_lastActivate(NEVER_ACTIVATED), m_prevLastActivate(NEVER_ACTIVATED), m_mvgAvg(0.0f),
     m_synType(NULL) {
   };
   DendriticSynapse(const DendriticSynapse& d) {
      m_isExc = d.m_isExc;
      m_isInhDiv = d.m_isInhDiv;
      m_srcNeuron = d.m_srcNeuron;
      m_destNeuron = d.m_destNeuron;
      m_weight = d.m_weight;
      m_lastActivate = d.m_lastActivate;
      m_prevLastActivate = d.m_prevLastActivate;
      m_mvgAvg = d.m_mvgAvg;
      m_synType = d.m_synType;
   }
   ~DendriticSynapse() {
      // Don't do anything
      // ESPECIALLY don't destroy m_synType!!
   }
   DendriticSynapse& operator=(const DendriticSynapse& d) {
      if (this != &d) {
         m_isExc = d.m_isExc;
         m_isInhDiv = d.m_isInhDiv;
         m_srcNeuron = d.m_srcNeuron;
         m_destNeuron = d.m_destNeuron;
         m_weight = d.m_weight;
         m_lastActivate = d.m_lastActivate;
         m_prevLastActivate = d.m_prevLastActivate;
         m_mvgAvg = d.m_mvgAvg;
         m_synType = d.m_synType;
      }
      return *this;
   }
   // activate happens prior to ++timeStep
   inline void activate(DataList &bus, DataList &bus_inhdiv, DataList &bus_inhsub, const int timeStep) {
      // throw the coin
      // Whatever you do, don't do this:
      // const SynapseType mySynType = *m_synType;
#if defined(RNG_BUCKET)
      bool result = ParallelRand::RandComm.RandBernoulli();
#else                     // not RNG_BUCKET
                    // relies on programmer to invoke chkNoiseInit in code before use
      bool result = SynNoise.Bernoulli(m_synType->getSynSuccRate());
#endif                    // RNG_BUCKET
      if (result) {
         const bool useMvgAvg = (m_synType->getLearningRule() == LRT_MvgAvg);
//         const bool useMultiAct = (m_synType->getLearningRule() == LRT_MultiActPS);
         const unsigned int NMDArise = m_synType->getNMDArise();
         const unsigned int timeDiff = timeStep - m_lastActivate;
         const unsigned int maxTimeStep = (m_synType->getMaxTimeStep());
         if (useMvgAvg) {
			if (timeDiff < NMDArise) {
            // Zeroth element is 0, first element is 1, 2nd element is alpha,
            // nth element is alpha^(n-1)
			   m_mvgAvg = (timeStep - m_lastActivate) < static_cast<int>(SynapseType::MAX_TIME_STEP) ?
			   m_synType->alphaRiseArray[m_oldZbar][timeDiff+1] * m_mvgAvg + (1-m_synType->getAlpha()) :
			   1-m_synType->getAlpha();
			}
			else {
			   m_mvgAvg = (timeStep - m_lastActivate) < static_cast<int>(SynapseType::MAX_TIME_STEP) ?
			   m_synType->alphaFallArray[timeDiff+1-NMDArise] * m_mvgAvg + (1-m_synType->getAlpha()) :
               1-m_synType->getAlpha();
            }
         }
         // Calculates net excitation
         //FLEX: Could add AMPA-like biology/dendritic capacitance(with lookup table)
 		 if (m_isExc)
 		    bus[m_destNeuron] += m_synType->getKsyn() * m_weight;
 		 else if (m_isInhDiv)
 		    bus_inhdiv[m_destNeuron] += m_synType->getKsyn() * m_weight;
 		 else
 			bus_inhsub[m_destNeuron] += m_synType->getKsyn() * m_weight;
         //FLEX: Could add more biology to this functionality(see NMDArise.ps for
         // an explanation)
         // NEVER_ACTIVATED is set so that if m_lastActivate == NEVER_ACTIVATED,
         // then timeStep - m_lastActivate >= MAX_TIME_STEP (because timeStep >= 0)
 		 //NOT USING THIS ANYMORE: m_lastActivate = ((NMDArise<=1) || (timeDiff>=static_cast<int>(SynapseType::MAX_TIME_STEP)) || useMvgAvg)
 		 //                                        ? timeStep : timeStep - iround(m_synType->alphaArray[timeDiff] * NMDArise);
 		 if (m_lastActivate == NEVER_ACTIVATED) {
 			 m_oldZbar = 0;
 			 m_riseUntil = 0;
         }
 		 else if (timeDiff < NMDArise) { //Activation during a Rise
 			if (timeStep < static_cast<int>(m_riseUntil)) { //previously had fired on a rise, i.e. at least the second fire on this rise
 				m_riseUntil = timeStep + NMDArise - 1;
 			}
 			else { //first fire on this rise
 				m_riseUntil = timeStep + NMDArise - 1;
 				m_riseActivate = m_lastActivate;
 			}
 		 }
 		 else {		//Activation during a Fall
 			m_oldZbar = static_cast<int>(1000.0f * m_synType->alphaFallArray[timeDiff-NMDArise]);
 			m_riseUntil = 0;
		 }
 		 m_prevLastActivate = m_lastActivate;
 		 m_lastActivate = timeStep;
 		 if (m_oldZbar < 0)
 			m_oldZbar = 0;
 		 else if (m_oldZbar >= 1000)
 			m_oldZbar = 1000;
         while (m_actHistory.size() > 0 && (timeStep - m_actHistory.front()) > maxTimeStep) {
			m_actHistory.pop_front();
         }
         m_actHistory.push_back(static_cast<unsigned int>(timeStep));
      }
   }
   inline void connectNeuron(unsigned int destNeuron, SynapseType const* synType, bool isExc, bool isInhDiv) {
      m_destNeuron = destNeuron;
      m_isExc = isExc;
      m_isInhDiv = isInhDiv;
      m_synType = synType;
   }
   inline unsigned int getSrcNeuron() const { return m_srcNeuron; }
   inline void setSrcNeuron(const unsigned int toSet) { m_srcNeuron = toSet; }
   inline float getWeight() const { return m_weight; }
   inline void setWeight(const float toSet) { m_weight = toSet; }
   inline float calcZBar(int timeStep, int lastActivate, int NMDArise, LearningRuleType learningRule) {
        const int timeDiff = timeStep - lastActivate;
        const int fallDiff = timeDiff - NMDArise;
        float zBar = 0;
        if (timeStep <= static_cast<int> (m_riseUntil)) {
            if (timeStep - m_riseActivate > NMDArise) { //zBar is saturated at maximum
                zBar = 1;
            }
            else {
                zBar = m_synType->alphaRiseArray[m_oldZbar][timeStep - m_riseActivate]; //zBar is still rising from the first fire
            }
        }
        else if (timeDiff < NMDArise) {
            zBar = m_synType->alphaRiseArray[m_oldZbar][timeDiff];
            if ((learningRule == LRT_PostSynB) && !((m_synType->alphaRiseArray[m_oldZbar][timeStep
                    - lastActivate]) > 0)) {
                zBar = m_synType->alphaRiseArray[m_oldZbar][timeStep - m_prevLastActivate];
            }
        }
        else {
            zBar = m_synType->alphaFallArray[fallDiff];
        }
	return zBar;
    }
   inline void updateWeight(int timeStep) {
      // Whatever you do, don't do this:
      // const SynapseType mySynType = *m_synType;
      // NEVER_ACTIVATED is set so that if m_lastActivate == NEVER_ACTIVATED,
      // then timeStep - m_lastActivate >= MAX_TIME_STEP (because timeStep >= 0)
      const float synModRate = m_synType->getSynModRate();
	  const int timeDiff = timeStep - m_lastActivate;
	  const int NMDArise = m_synType->getNMDArise();
	  const int fallDiff = timeDiff - NMDArise;
      const unsigned int maxTimeStep = m_synType->getMaxTimeStep();
	  if (fallDiff >= static_cast<int>(maxTimeStep)) {	// Never_Activated or really old
         m_weight -= synModRate * m_weight;
	  }
	  else {
    	 LearningRuleType learningRule = m_synType->getLearningRule();
    	 if (learningRule == LRT_MultiActPS) {
             for (unsigned int i=0; i<m_actHistory.size(); i++) {
                 float zBar = calcZBar(timeStep, m_actHistory[i], NMDArise, learningRule);
                 m_weight += static_cast<float>(synModRate * (zBar - m_weight));
             }
    	 }
    	 else {
    	     float zBar = calcZBar(timeStep, m_lastActivate, NMDArise, learningRule);
    	     if (learningRule == LRT_MvgAvg) {
                 // For moving averager, it only gets updated when synapse is
                 // activated. Need to do calculations since that's happened (all zeros)
                 m_weight += static_cast<float>(synModRate * (zBar * m_mvgAvg - m_weight));
    	     }
    	     else if (learningRule == LRT_PostSyn) {
                 m_weight += static_cast<float>(synModRate * (zBar - m_weight));
    	     }
    	 }
      }
   }
   inline int getLastActivate() const { return m_lastActivate; }
   inline SynapseType const* getSynapseType() const { return m_synType; }
   inline void setLastActivate(const int toSet) {
      m_prevLastActivate = m_lastActivate;
      m_lastActivate = toSet;
   }
   inline void resetLastActivate() {
      m_prevLastActivate = m_lastActivate;
      m_lastActivate = NEVER_ACTIVATED;
   }
   inline void setOldZbar(const float toSet) { m_oldZbar = static_cast<int>(toSet*1000); }
   static Noise SynNoise;                 // rng for syn failure
private:
   bool m_isExc;
   bool m_isInhDiv;
   unsigned int m_srcNeuron;
   unsigned int m_destNeuron;
   float m_weight;
   int m_oldZbar;
   unsigned int m_riseUntil;
   int m_riseActivate;
   int m_lastActivate; // actual last activation time - must be signed since NEVER_ACTIVATED is negative
   int m_prevLastActivate; // MEGHAN: adjusted activation time before last
   float m_mvgAvg;
   std::deque<unsigned int> m_actHistory;
   SynapseType const* m_synType;
   // Static variables
   // NEVER_ACTIVATED is set so that if m_lastActivate == NEVER_ACTIVATED,
   // then timeStep - m_lastActivate >= MAX_TIME_STEP (because timeStep >= 0)
   const static int NEVER_ACTIVATED = -1 - static_cast<int>(SynapseType::MAX_TIME_STEP);
};

typedef DendriticSynapse * Dendrite;
typedef DendriticSynapse const * const DendriteConst;

// AxonalSynapse = Synapse on axon
class AxonalSynapse {
public:
   inline AxonalSynapse(): synapse(NULL) {};
   inline void connectSynapse(unsigned int destNeuron, DendriticSynapse &dendritic,
                        SynapseType const* synType, bool isExc=true, bool isInhDiv=false) {
      synapse = &dendritic;
      synapse->connectNeuron(destNeuron, synType, isExc, isInhDiv);
   }
   inline bool connectsTo(const DendriticSynapse &dendritic) const {
      return synapse == &dendritic;
   }
   inline void activate(DataList &bus, DataList &bus_inhdiv, DataList &bus_inhsub, const int timeStep) {
      synapse->activate(bus, bus_inhdiv, bus_inhsub, timeStep);
   }
   inline float getWeight() const { return synapse->getWeight(); }
   inline void setWeight(const float toSet) { synapse->setWeight(toSet); }
   inline int getLastActivate() const { return synapse->getLastActivate(); }
   inline void setLastActivate(const int toSet) { synapse->setLastActivate(toSet); }
private:
   DendriticSynapse *synapse; // Points to corresponding DendriticSynapse
};

typedef AxonalSynapse * AxonalSegment;
typedef AxonalSegment * Axon;
typedef AxonalSegment const * const AxonConst;

#endif //SYNAPSE_HPP
