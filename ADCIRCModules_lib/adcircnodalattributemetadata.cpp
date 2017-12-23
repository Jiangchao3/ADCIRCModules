//------------------------------GPL---------------------------------------//
// This file is part of ADCIRCModules.
//
// (c) 2015-2018 Zachary Cobell
//
// ADCIRCModules is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ADCIRCModules is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ADCIRCModules.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------//
#include "adcircnodalattributemetadata.h"

AdcircNodalAttributeMetadata::AdcircNodalAttributeMetadata(std::string name, std::string units,
                                           int numValues) {
  this->setName(name);
  this->setUnits(units);
  this->setNumValues(numValues);
}

std::string AdcircNodalAttributeMetadata::name() const { return this->m_name; }

void AdcircNodalAttributeMetadata::setName(const std::string &name) {
  this->m_name = name;
}

std::string AdcircNodalAttributeMetadata::units() const { return this->m_units; }

void AdcircNodalAttributeMetadata::setUnits(const std::string &units) {
  this->m_units = units;
}

int AdcircNodalAttributeMetadata::numValues() const { return this->m_numValues; }

void AdcircNodalAttributeMetadata::setNumValues(int numValues) {
  this->m_numValues = numValues;
}

std::vector<double> AdcircNodalAttributeMetadata::getDefaultValue() const {
  return this->m_defaultValue;
}

void AdcircNodalAttributeMetadata::setDefaultValue(const std::vector<double> &value) {
  this->m_defaultValue = value;
}
