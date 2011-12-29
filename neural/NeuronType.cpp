/***************************************************************************
 * NeuronType.cpp
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
#if !defined(NEURONTYPE_HPP)
#  include "NeuronType.hpp"
#endif

#include <map>
#include <string>

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

NeuronType::NeuronType(const NeuronType& n)
  : m_dendriteToSomaFilter(n.m_dendriteToSomaFilter),
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

void NeuronType::addMember(const std::string& name, bool isExc, bool isInhDiv,
                           const ThresholdType thresholdType) {
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
      DataList& convolved =
        Calc::convolve(m_dendriteToSomaFilter.getFilter(),
                       synapticFilter->getFilter(), deltaT);
      m_convolvedFilter.setFilter(convolved);
    } else if (m_dendriteToSomaFilter.size() > 1) {
      m_convolvedFilter.setFilter(m_dendriteToSomaFilter.getFilter());
    } else {
      m_convolvedFilter.setFilter(synapticFilter->getFilter());
    }
  }
}

bool NeuronType::forceExt() const {
  return ((getParameter("ExtExc", SystemVar::GetFloatVar("ExtExc"))
           < verySmallFloat) &&
          (getParameter("DGstrength", SystemVar::GetFloatVar("DGstrength"))
           < verySmallFloat));
}

bool NeuronType::useIzh() const {
  return (hasParameter("IzhA"));
}
