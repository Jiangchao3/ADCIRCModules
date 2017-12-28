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
#include "adcircnode.h"
#include "boost/format.hpp"

AdcircNode::AdcircNode() {
  this->m_id = 0;
  this->m_x = -9999.0;
  this->m_y = -9999.0;
  this->m_z = -9999.0;
}

AdcircNode::AdcircNode(int id, double x, double y, double z) {
  this->m_id = id;
  this->m_x = x;
  this->m_y = y;
  this->m_z = z;
}

void AdcircNode::setNode(int id, double x, double y, double z) {
  this->m_id = id;
  this->m_x = x;
  this->m_y = y;
  this->m_z = z;
  return;
}

double AdcircNode::x() const { return this->m_x; }

void AdcircNode::setX(double x) { this->m_x = x; }

double AdcircNode::y() const { return this->m_y; }

void AdcircNode::setY(double y) { this->m_y = y; }

double AdcircNode::z() const { return this->m_z; }

void AdcircNode::setZ(double z) { this->m_z = z; }

int AdcircNode::id() const { return this->m_id; }

void AdcircNode::setId(int id) { this->m_id = id; }

std::string AdcircNode::toString(bool geographicCoordinates) {
  if (geographicCoordinates)
    return boost::str(boost::format("%11i   %14.10f   %14.10f  %14.10f") %
                      this->id() % this->x() % this->y() % this->z());
  else
    return boost::str(boost::format("%11i   %14.4f   %14.4f  %14.4f") %
                      this->id() % this->x() % this->y() % this->z());
}
