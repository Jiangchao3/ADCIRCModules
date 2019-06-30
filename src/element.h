/*------------------------------GPL---------------------------------------//
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
//------------------------------------------------------------------------*/
#ifndef ADCMOD_ELEMENT_H
#define ADCMOD_ELEMENT_H

#include <cmath>
#include <string>
#include <utility>
#include <vector>
#include "adcircmodules_global.h"
#include "node.h"

using Point = std::pair<double, double>;

namespace Adcirc {
namespace Geometry {

/**
 * @class Element
 * @author Zachary Cobell
 * @copyright Copyright 2018 Zachary Cobell. All Rights Reserved. This project
 * is released under the terms of the GNU General Public License v3
 * @brief The Element class describes an Element as an array
 * of Node pointers
 *
 */

class Element {
 public:
  ADCIRCMODULES_EXPORT Element();
  ADCIRCMODULES_EXPORT Element(size_t id, Adcirc::Geometry::Node *n1,
                               Adcirc::Geometry::Node *n2,
                               Adcirc::Geometry::Node *n3);
  ADCIRCMODULES_EXPORT Element(size_t id, Adcirc::Geometry::Node *n1,
                               Adcirc::Geometry::Node *n2,
                               Adcirc::Geometry::Node *n3,
                               Adcirc::Geometry::Node *n4);

  void ADCIRCMODULES_EXPORT setElement(size_t id, Adcirc::Geometry::Node *n1,
                                       Adcirc::Geometry::Node *n2,
                                       Adcirc::Geometry::Node *n3);
  void ADCIRCMODULES_EXPORT setElement(size_t id, Adcirc::Geometry::Node *n1,
                                       Adcirc::Geometry::Node *n2,
                                       Adcirc::Geometry::Node *n3,
                                       Adcirc::Geometry::Node *n4);

  size_t n() const;
  void ADCIRCMODULES_EXPORT resize(size_t nVertex);

  Adcirc::Geometry::Node ADCIRCMODULES_EXPORT *node(size_t i);
  void ADCIRCMODULES_EXPORT setNode(size_t i, Adcirc::Geometry::Node *node);

  size_t ADCIRCMODULES_EXPORT id() const;
  void ADCIRCMODULES_EXPORT setId(size_t id);

  std::string ADCIRCMODULES_EXPORT toAdcircString();
  std::string ADCIRCMODULES_EXPORT to2dmString();

  bool ADCIRCMODULES_EXPORT isInside(Point location);
  bool ADCIRCMODULES_EXPORT isInside(double x, double y);

  double ADCIRCMODULES_EXPORT elementSize(bool geodesic = true);

  void ADCIRCMODULES_EXPORT sortVerticiesAboutCenter();

  std::pair<Adcirc::Geometry::Node *, Adcirc::Geometry::Node *>
      ADCIRCMODULES_EXPORT elementLeg(size_t i);

  void ADCIRCMODULES_EXPORT getElementCenter(double &xc, double &yc);

  double ADCIRCMODULES_EXPORT area();

  void ADCIRCMODULES_EXPORT interpolationWeights(double x, double y,
                                                 std::vector<double> &weights);

  std::string ADCIRCMODULES_EXPORT hash();

 private:
  size_t m_id;
  std::vector<Adcirc::Geometry::Node *> m_nodes;
  std::string m_hash;

  void generateHash();

  void triangularInterpolation(double x, double y,
                               std::vector<double> &weights);
  void polygonInterpolation(double x, double y, std::vector<double> &weights);
};
}  // namespace Geometry
}  // namespace Adcirc

#endif  // ADCMOD_ELEMENT_H
