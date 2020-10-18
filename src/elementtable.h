/*------------------------------GPL---------------------------------------//
// This file is part of ADCIRCModules.
//
// (c) 2015-2019 Zachary Cobell
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
#ifndef ADCMOD_ELEMENTTABLE_H
#define ADCMOD_ELEMENTTABLE_H

#include <memory>
#include <vector>
#include "element.h"
namespace Adcirc {

namespace Private {
// Forward declaration of Pimpl class
class MeshPrivate;
}  // namespace Private

namespace Geometry {

// Forward declaration of mesh class
class Mesh;

/**
 * @class ElementTable
 * @author Zachary Cobell
 * @copyright Copyright 2015-2019 Zachary Cobell. All Rights Reserved. This project
 * is released under the terms of the GNU General Public License v3
 * @brief The ElementTable class is used to develop a list of the elements that
 * surround each node
 *
 */

class ElementTable {
 public:
  ElementTable();
  explicit ElementTable(Adcirc::Private::MeshPrivate *mesh);
  explicit ElementTable(std::unique_ptr<Adcirc::Private::MeshPrivate> *mesh);
  explicit ElementTable(Adcirc::Geometry::Mesh *mesh);

  size_t numNodesAroundNode(size_t nodeIndex) const;
  Adcirc::Geometry::Node *nodeTable(size_t nodeIndex, size_t listIndex) const;
  std::vector<Adcirc::Geometry::Node *> nodeList(size_t nodeIndex) const;
  std::vector<std::vector<size_t>> fullNodeTable() const;

  size_t numElementsAroundNode(size_t nodeIndex) const;
  Adcirc::Geometry::Element *elementTable(size_t nodeIndex, size_t listIndex) const;
  std::vector<Adcirc::Geometry::Element *> elementList(size_t nodeIndex) const;
  std::vector<std::vector<size_t>> fullElementTable() const;

  size_t maxElementsAroundNode() const;
  size_t maxNodesAroundNode() const;

  void build();

  bool initialized() const;

  Adcirc::Private::MeshPrivate *mesh() const;
  void setMesh(Adcirc::Private::MeshPrivate *mesh);

 private:
  std::vector<std::vector<Adcirc::Geometry::Element *>> m_elementTable;
  std::vector<std::vector<Adcirc::Geometry::Node *>> m_nodeTable;

  Adcirc::Private::MeshPrivate *m_mesh;

  size_t m_maxElementsAroundNode;
  size_t m_maxNodesAroundNode;

  bool m_initialized;
};
}  // namespace Geometry
}  // namespace Adcirc

#endif  // ADCMOD_ELEMENTTABLE_H
