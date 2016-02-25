/*-------------------------------GPL-------------------------------------//
//
// QADCModules - A library for working with ADCIRC models
// Copyright (C) 2016  Zach Cobell
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------*/
#include "adcirc_element.h"

adcirc_element::adcirc_element(QObject *parent) : QObject(parent)
{
    this->numConnections = 3;
    this->connections.resize(3);
}

//-----------------------------------------------------------------------------------------//
//...Parses a line from the ADCIRC mesh that contains an element into
//   its connectivity information. Note that if the mesh needs re-numbering,
//   you will need to set ignoreMeshNumbering to true;
//-----------------------------------------------------------------------------------------//
/** \brief Protected function to parse the string from an ADCIRC mesh file containing
 *  elemental connectivity information
 *
 * \author Zach Cobell
 *
 * @param line     [in]    The QString with the information read from the file to be parsed into an ADCIRC element
 * @param index    [in]    An integer for the current element position in the ADCIRC file
 * @param *element [inout] The pointer to the adcirc_element that should be created with this information
 *
 * This function parses the string from the element section of the ADCIRC file
 **/
//-----------------------------------------------------------------------------------------//
int adcirc_element::readElement(QString line, QVector<adcirc_node *> &nodes, QMap<int, int> &nodeMapping)
{
    QStringList tempList;
    QString     tempString;
    int         tempInt;
    bool        err;

    tempList   = line.simplified().split(" ");
    tempString = tempList.value(0);
    tempInt    = tempString.toInt(&err);
    if(!err)
        return ADCMESH_MESHREAD_ELEMER;

    this->id = tempInt;

    tempString = tempList.value(2);
    tempInt    = tempString.simplified().toInt(&err);
    if(!err)
        return ADCMESH_MESHREAD_ELEMER;
    this->connections[0] = nodes[nodeMapping[tempInt]];

    tempString = tempList.value(3);
    tempInt    = tempString.simplified().toInt(&err);
    if(!err)
        return ADCMESH_MESHREAD_ELEMER;
    this->connections[1] = nodes[nodeMapping[tempInt]];

    tempString = tempList.value(4);
    tempInt    = tempString.simplified().toInt(&err);
    if(!err)
        return ADCMESH_MESHREAD_ELEMER;
    this->connections[2] = nodes[nodeMapping[tempInt]];

    return ADCMESH_NOERROR;
}
//-----------------------------------------------------------------------------------------//
