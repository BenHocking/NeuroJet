#if !defined(NEURONTYPE_HPP)
#  include "NeuronType.hpp"
#endif

#if !defined(CALC_HPP)
#  include "Calc.hpp"
#endif
#if !defined(OUTPUT_HPP)
#  include "Output.hpp"
#endif
#if !defined(SYSTEMVAR_HPP)
#  include "SystemVar.hpp"
#endif

std::map<std::string, NeuronType> NeuronType::Member;

NeuronType::NeuronType(const NeuronType& n): m_dendriteToSomaFilter(n.m_dendriteToSomaFilter),
   m_convolvedFilter(n.m_convolvedFilter), m_synapseType(n.m_synapseType),
   m_parameter(n.m_parameter), m_isExc(n.m_isExc), m_isInhDiv(n.m_isInhDiv),
   m_thresholdType(n.m_thresholdType), m_name(n.m_name) {
}

NeuronType& NeuronType::operator=(const NeuronType& n) {
   if (this != &n) {  // make sure not same object
      m_dendriteToSomaFilter = n.m_dendriteToSomaFilter;
      m_convolvedFilter = n.m_convolvedFilter;
      m_synapseType = n.m_synapseType;
      m_parameter = n.m_parameter;
      m_isExc = n.m_isExc;
      m_isInhDiv = n.m_isInhDiv;
      m_thresholdType = n.m_thresholdType;
      m_name = n.m_name;
   }
   return *this;    // Return ref for multiple assignment
}

void NeuronType::addMember(const std::string& name, bool isExc, bool isInhDiv, const ThresholdType thresholdType) {
   if (Member.find(name) == Member.end()) {
      Member[name] = NeuronType(isExc, isInhDiv, thresholdType, name);
   } else {
      CALL_ERROR << "NeuronType " << name << " already exists" << ERR_WHERE;
      exit(EXIT_FAILURE);
   }
}

void NeuronType::convolveFilters() {
   const float deltaT = SystemVar::GetFloatVar("deltaT");
   Filter* synapticFilter = m_synapseType.getFilter();
   if (deltaT > verySmallFloat) {
      if ((m_dendriteToSomaFilter.size() > 1) && (synapticFilter->size() > 1)) {
         m_convolvedFilter.setFilter(Calc::convolve(
                                         m_dendriteToSomaFilter.getFilter(), synapticFilter->getFilter(), deltaT));
      } else if (m_dendriteToSomaFilter.size() > 1) {
	m_convolvedFilter.setFilter(m_dendriteToSomaFilter.getFilter());
      } else {
	m_convolvedFilter.setFilter(synapticFilter->getFilter());
      }
   }
}

bool NeuronType::forceExt() const {
   return ((getParameter("ExtExc", SystemVar::GetFloatVar("ExtExc")) < verySmallFloat) &&
          (getParameter("DGstrength", SystemVar::GetFloatVar("DGstrength")) < verySmallFloat));
}

bool NeuronType::useIzh() const {
   return (hasParameter("IzhA"));
}
