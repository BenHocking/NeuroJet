/***************************************************************************
 * NeuronType.hpp
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
#  define NEURONTYPE_HPP

#  include <map>
#  include <string>

#  if !defined(DATATYPES_HPP)
#    include "DataTypes.hpp"
#  endif
#  if !defined(FILTER_HPP)
#    include "Filter.hpp"
#  endif
#  if !defined(SYNAPSETYPE_HPP)
#    include "SynapseType.hpp"
#  endif
#  if !defined(SYSTEMVAR_HPP)
#    include "SystemVar.hpp"
#  endif

enum ThresholdType { TT_Undef, TT_Simple, TT_E, TT_Log, TT_Rational };

class NeuronType {
 public:
  static const char* DEFAULT;
  explicit NeuronType(const std::string& name = DEFAULT) throw()
    : m_synapseType(name), m_isExc(true), m_isInhDiv(false),
      m_thresholdType(TT_Simple), m_name(name) {}
  NeuronType(const NeuronType& n) throw();
  NeuronType(bool isExc, bool isInhDiv, const ThresholdType thresholdType,
             const std::string& name) throw()
    : m_synapseType(name), m_isExc(isExc), m_isInhDiv(isInhDiv),
      m_thresholdType(thresholdType), m_name(name) {}
  NeuronType& operator=(const NeuronType& n) throw();
  void convolveFilters();
  bool forceExt() const;
  unsigned int getFilterSize() const throw() { return m_convolvedFilter.size(); }
  Filter getFilter() const throw() { return m_convolvedFilter; }
  std::string getName() const throw() { return m_name; }
  template<class T>
  T getParameter(const std::string& param, const T defValue) const {
    // m_parameter[param] isn't const friendly, hence this:
    return (hasParameter(param)) ?
      from_string<T>(m_parameter.find(param)->second) : defValue;
  }
  ThresholdType getThresholdType() const throw() { return m_thresholdType; }
  bool hasParameter(const std::string& param) const throw() {
    return (m_parameter.find(param) != m_parameter.end());
  }
  bool isExcType() const throw() { return m_isExc; }
  bool isInhDivType() const throw() { return m_isInhDiv; }
  void loadDTSFilterValues(const DataList &filterVals) {
    m_dendriteToSomaFilter.setFilter(filterVals);
    convolveFilters();
  }
  void loadSynapseFilterValues(const DataList &filterVals) {
    m_synapseType.setFilter(filterVals);
    convolveFilters();
  }
  void setParameter(const std::string& param, const std::string& val) {
    m_parameter[param] = val;
  }
  bool useIzh() const;
  void setThresholdType(const ThresholdType thresholdType) {
    m_thresholdType = thresholdType;
  }
  ////////////////////
  // Static members //
  ////////////////////
  // This map will only be accessed during CreateNetwork
  static std::map<std::string, NeuronType> Member;
  static void addMember(const std::string& name, bool isExc, bool isInhDiv,
                        const ThresholdType thresholdType);

 private:
  Filter m_dendriteToSomaFilter;
  // dendrite-to-soma filter convolved with synapse filter
  Filter m_convolvedFilter;
  SynapseType m_synapseType;
  std::map<const std::string, std::string> m_parameter;
  bool m_isExc;
  bool m_isInhDiv;
  ThresholdType m_thresholdType;
  std::string m_name;
};

typedef std::map<std::string, NeuronType>::const_iterator NeuronTypeMapCIt;
typedef std::map<std::string, NeuronType>::iterator NeuronTypeMapIt;

#endif  // NEURONTYPE_HPP
