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
#include "elementtable.h"
#include "default_values.h"
#include "logging.h"
#include "mesh.h"
#include "mesh_private.h"

using namespace Adcirc::Geometry;

namespace Adcirc {
namespace Geometry {

/**
 * @brief Default constructor
 */
ElementTable::ElementTable()
    : m_mesh(nullptr), m_initialized(false), m_maxElementsAroundNode(0), m_maxNodesAroundNode(0) {}

/**
 * @brief Constructor with smart pointer meshimpl as a parameter
 * @param[in] mesh sets the mesh used to generate the ElementTable
 */
ElementTable::ElementTable(std::unique_ptr<Adcirc::Private::MeshPrivate> *mesh)
    : m_mesh(mesh->get()), m_initialized(false), m_maxElementsAroundNode(0), m_maxNodesAroundNode(0) {}

/**
 * @brief Constructor with mesh as a parameter
 * @param[in] mesh sets the mesh used to generate the ElementTable
 */
ElementTable::ElementTable(Adcirc::Geometry::Mesh *mesh)
    : m_mesh(mesh->m_impl.get()), m_initialized(false), m_maxElementsAroundNode(0), m_maxNodesAroundNode(0) {}

/**
 * @brief Constructor with meshimpl as a parameter
 * @param[in] mesh sets the mesh used to generate the ElementTable
 */
ElementTable::ElementTable(Adcirc::Private::MeshPrivate *mesh)
    : m_mesh(mesh), m_initialized(false) {}

/**
 * @brief Returns the pointer to the mesh used to build the table
 * @return pointer to mesh used in this table
 */
Adcirc::Private::MeshPrivate *ElementTable::mesh() const { return this->m_mesh; }

/**
 * @brief Sets the mesh used in the element table
 * @param[in] mesh to use to build the table
 */
void ElementTable::setMesh(Adcirc::Private::MeshPrivate *mesh) { this->m_mesh = mesh; }

/**
 * @brief Begin building the table
 */
void ElementTable::build() {
  if (this->m_mesh == nullptr) {
    return;
  }
  this->m_elementTable.reserve(this->m_mesh->numNodes());
  for (size_t i = 0; i < this->m_mesh->numElements(); ++i) {
    for (size_t j = 0; j < this->m_mesh->element(i)->n(); ++j) {
      auto n = this->m_mesh->nodeIndexById(this->m_mesh->element(i)->node(j)->id());
      this->m_elementTable[n].push_back(this->m_mesh->element(i));
    }
  }

  this->m_nodeTable.reserve(this->m_mesh->numNodes());
  for (size_t i = 0; i < this->m_mesh->numNodes(); ++i) {
    m_maxElementsAroundNode = std::max(m_maxElementsAroundNode, m_elementTable[i].size());
    auto elmList = this->elementList(i);
    std::vector<size_t> nid;
    nid.reserve(elmList.size() * 3);
    for (auto &e : elmList) {
      for (size_t j = 0; j < e->n(); ++j) {
        nid.push_back(m_mesh->nodeIndexById(e->node(j)->id()));
      }
    }
    std::sort(nid.begin(), nid.end());
    auto p = std::unique(nid.begin(), nid.end());
    this->m_nodeTable[i].reserve(p - nid.begin());
    for (auto j = nid.begin(); j != p; ++j) {
      this->m_nodeTable[i].push_back(m_mesh->node(*(j)));
    }
    m_maxNodesAroundNode = std::max(m_maxNodesAroundNode, m_nodeTable[i].size());
  }

  this->m_initialized = true;
}

/**
 * @brief Returns the list of elements around a specified node
 * @param[in] n node to return the element table for
 * @return vector of element pointers around the node
 */
std::vector<Element *> ElementTable::elementList(size_t nodeIndex) const {
  if (nodeIndex < m_mesh->numNodes())
    return this->m_elementTable[nodeIndex];
  else
    adcircmodules_throw_exception("Node Index " + std::to_string(nodeIndex) +
        " greater than number of nodes");
  return std::vector<Element *>();
}

/**
 * @param[in] nodeIndex node index in the mesh to return the number of elements for
 * @return number of elements around a specified node
 */
size_t ElementTable::numElementsAroundNode(size_t nodeIndex) const {
  if (nodeIndex < this->mesh()->numNodes()) {
    return this->m_elementTable[nodeIndex].size();
  } else {
    adcircmodules_throw_exception("Out of bounds node request");
  }
  return adcircmodules_default_value<size_t>();
}

/**
 * @param[in] nodeIndex node index in the mesh to return the number of elements for
 * @param[in] listIndex index in list of elements
 * @return pointer to element
 */
Adcirc::Geometry::Element *ElementTable::elementTable(size_t nodeIndex,
                                                      size_t listIndex) const {
  if (nodeIndex < this->mesh()->numNodes()) {
    if (listIndex < this->m_elementTable[nodeIndex].size()) {
      return this->m_elementTable[nodeIndex].at(listIndex);
    } else {
      adcircmodules_throw_exception("Out of element table request");
    }
  } else {
    adcircmodules_throw_exception("Out of bounds node request");
  }
  return nullptr;
}

size_t ElementTable::numNodesAroundNode(size_t nodeIndex) const {
  if (nodeIndex < m_mesh->numNodes()) {
    return this->m_nodeTable[nodeIndex].size();
  }
  return 0;
}

Adcirc::Geometry::Node *ElementTable::nodeTable(size_t nodeIndex, size_t listIndex) const {
  if (nodeIndex < m_mesh->numNodes()) {
    if (listIndex < m_nodeTable[nodeIndex].size()) return m_nodeTable[nodeIndex][listIndex];
  }
  return nullptr;
}

std::vector<Adcirc::Geometry::Node *> ElementTable::nodeList(size_t nodeIndex) const {
  if (nodeIndex < m_mesh->numNodes()) {
    return m_nodeTable[nodeIndex];
  }
  return std::vector<Adcirc::Geometry::Node *>();
}

std::vector<std::vector<size_t>> ElementTable::fullNodeTable() const {
  std::vector<std::vector<size_t>> t;
  t.resize(m_mesh->numNodes());
  for (size_t i = 0; i < m_mesh->numNodes(); ++i) {
    t[i] = std::vector<size_t>(m_maxNodesAroundNode, 0);
    for (size_t j = 0; j < m_nodeTable[i].size(); ++j) {
      t[i][j] = m_mesh->nodeIndexById(m_nodeTable[i][j]->id());
    }
  }
  return t;
}

std::vector<std::vector<size_t>> ElementTable::fullElementTable() const {
  std::vector<std::vector<size_t>> t;
  t.resize(m_mesh->numNodes());
  for (size_t i = 0; i < m_mesh->numNodes(); ++i) {
    t[i] = std::vector<size_t>(m_maxElementsAroundNode, 0);
    for (size_t j = 0; j < m_elementTable[i].size(); ++j) {
      t[i][j] = m_mesh->nodeIndexById(m_elementTable[i][j]->id());
    }
  }
  return t;
}

size_t ElementTable::maxElementsAroundNode() const { return m_maxElementsAroundNode; }

size_t ElementTable::maxNodesAroundNode() const { return m_maxNodesAroundNode; }

bool ElementTable::initialized() const { return this->m_initialized; }

}  // namespace Geometry
}  // namespace Adcirc
