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
ElementTable::ElementTable() : m_mesh(nullptr), m_initialized(false) {}

/**
 * @brief Constructor with smart pointer meshimpl as a parameter
 * @param[in] mesh sets the mesh used to generate the ElementTable
 */
ElementTable::ElementTable(std::unique_ptr<Adcirc::Private::MeshPrivate> *mesh)
    : m_mesh(mesh->get()), m_initialized(false) {}

/**
 * @brief Constructor with mesh as a parameter
 * @param[in] mesh sets the mesh used to generate the ElementTable
 */
ElementTable::ElementTable(Adcirc::Geometry::Mesh *mesh)
    : m_mesh(mesh->m_impl.get()), m_initialized(false) {}

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
      Node *n = this->m_mesh->element(i)->node(j);
      this->m_elementTable[n].push_back(this->m_mesh->element(i));
    }
  }
  this->m_initialized = true;
  return;
}

/**
 * @brief Returns the list of elements around a specified node
 * @param[in] n node to return the element table for
 * @return vector of element pointers around the node
 */
std::vector<Element *> ElementTable::elementList(Node *n) {
  if (this->m_elementTable.find(n) != this->m_elementTable.end())
    return this->m_elementTable[n];
  else
    adcircmodules_throw_exception("Node " + std::to_string(n->id()) +
                                  " not part of mesh");
  return std::vector<Element *>();
}

/**
 * @brief Returns the number of elements around a specified node pointer
 * @param[in] n pointer to node to return the number of elements for
 * @return number of elements around a specified node
 */
size_t ElementTable::numElementsAroundNode(Adcirc::Geometry::Node *n) {
  if (this->m_elementTable.find(n) != this->m_elementTable.end())
    return this->m_elementTable[n].size();
  else
    return adcircmodules_default_value<size_t>();
}

/**
 * @param[in] nodeIndex node index in the mesh to return the number of elements for
 * @return number of elements around a specified node
 */
size_t ElementTable::numElementsAroundNode(size_t nodeIndex) {
  if (nodeIndex < this->mesh()->numNodes()) {
    return this->m_elementTable[this->mesh()->node(nodeIndex)].size();
  } else {
    adcircmodules_throw_exception("Out of bounds node request");
  }
  return adcircmodules_default_value<size_t>();
}

/**
 * @brief Returns a pointer to the element at a position in the list for the
 * specified node
 * @param[in] n pointer to node
 * @param[in] listIndex index in list of elements
 * @return pointer to element
 */
Adcirc::Geometry::Element *ElementTable::elementTable(Adcirc::Geometry::Node *n,
                                                      size_t listIndex) {
  if (this->m_elementTable.find(n) != this->m_elementTable.end()) {
    if (listIndex < this->m_elementTable[n].size()) {
      return this->m_elementTable[n].at(listIndex);
    } else {
      adcircmodules_throw_exception("Out of element table request");
    }
  }
  return nullptr;
}

/**
 * @param[in] nodeIndex node index in the mesh to return the number of elements for
 * @param[in] listIndex index in list of elements
 * @return pointer to element
 */
Adcirc::Geometry::Element *ElementTable::elementTable(size_t nodeIndex,
                                                      size_t listIndex) {
  if (nodeIndex < this->mesh()->numNodes()) {
    if (listIndex <
        this->m_elementTable[this->mesh()->node(nodeIndex)].size()) {
      return this->m_elementTable[this->mesh()->node(nodeIndex)].at(listIndex);
    } else {
      adcircmodules_throw_exception("Out of element table request");
    }
  } else {
    adcircmodules_throw_exception("Out of bounds node request");
  }
  return nullptr;
}

bool ElementTable::initialized() { return this->m_initialized; }

}  // namespace Geometry
}  // namespace Adcirc
