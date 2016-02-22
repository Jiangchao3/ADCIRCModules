/*-------------------------------GPL-------------------------------------//
//
// ADCMESH - A library for working with ADCIRC models
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
/**
 * \class adcirc_mesh
 *
 * \addtogroup ADCMESH
 *
 * \brief Class that describes an ADCIRC mesh
 *
 * \author Zachary Cobell
 * \version Version 0.1
 * \date 02/21/2016
 *
 * Contact: zcobell@gmail.com
 *
 * Created on: 02/21/2016
 *
 */
#ifndef ADCIRC_MESH_H
#define ADCIRC_MESH_H


#include <QObject>
#include <QVector>
#include <QFile>
#include <QMap>

#include "QADCModules_global.h"
#include "adcirc_node.h"
#include "adcirc_element.h"
#include "adcirc_boundary.h"


//----------------------------------------------------//
//...Define a set of errors for use with this
//   class
#define ADCMESH_NOERROR         -999900000
#define ADCMESH_FILEOPENERR     -999901001
#define ADCMESH_NULLFILENAM     -999901002
#define ADCMESH_FILENOEXIST     -999901003
#define ADCMESH_MESHREAD_HEADER -999902001
#define ADCMESH_MESHREAD_NODERR -999902002
#define ADCMESH_MESHREAD_ELEMER -999902003
#define ADCMESH_MESHREAD_BNDERR -999902004
#define ADCMESH_MESHREAD_NODNUM -999902005
#define ADCMESH_MESHREAD_ELENUM -999902006
#define ADCMESH_MESHREAD_BNDUNK -999902007
//----------------------------------------------------//


class QADCMODULESSHARED_EXPORT adcirc_mesh : public QObject
{
    Q_OBJECT
public:

    /** \brief Constructor
     *  Takes QObject reference as input
     * @param *parent [in] reference to QObject. Enables automatic memory management to avoid memory leaks
     **/
    explicit adcirc_mesh(QObject *parent = 0);


    //...Public variables...//

    ///Filename for this ADCIRC mesh
    QString                            filename;

    ///Title, or header, from the mesh file
    QString                            title;

    ///Number of elements contained within the mesh file
    int                                numElements;

    ///Number of nodes contained with the mesh file
    int                                numNodes;

    ///Vector made up of the class adcirc_node with x, y, and z information for nodes in this mesh
    QVector<adcirc_node*>              nodes;

    ///Vector made up of the class adcirc_element with connectivity information
    QVector<adcirc_element*>           elements;

    ///Number of open boundary conditions contained within the mesh
    int                                numOpenBoundaries;

    ///Total number of open boundary nodes in this mesh
    int                                totNumOpenBoundaryNodes;

    ///Number of land boundary conditions within the mesh
    int                                numLandBonudaries;

    ///Total number of land boundary nodes in this mesh
    int                                totNumLandBoundaryNodes;

    ///Vector made up of the class adcirc_boundary with the open boundary information
    QVector<adcirc_boundary*>          openBC;

    ///Vector made up of the class adcirc_boundary with the land boundary information
    QVector<adcirc_boundary*>          landBC;



    //...Public functions...///

    ///Public function to trigger the reading of an ADCIRC mesh into this class
    int read();

    ///Public function to allow user to trigger writing of the ADCIRC mesh contained within this class
    int write(QString outputFile);

    ///Public function to allow user to get the current error code
    int getErrorCode();

    ///Public function to allow the user to return the description for the current error code
    QString getErrorString();

    ///Public function that sets ignoreMeshNumbering. This allows the code to continue even if mesh numbering is non-sequential
    int setIgnoreMeshNumbering(bool value);

private:

    //...PRIVATE VARIABLES...//

    ///Error code generated by some function in this class
    int errorCode;

    ///Map function between error codes and their descriptions
    QMap<int,QString> errorMapping;

    ///Variable that is checked to decide if non-sequential mesh numbering is a fatal error. Default = false
    bool ignoreMeshNumbering;

    //...PRIVATE FUNCTIONS...//
    /// \brief Private function that initializes the error mapping on startup
    int initializeErrors();

    /// \brief Private function to read an ADCIRC mesh from the current filename
    int readMesh();

    /// \brief Private function to allocate a vector of pointers and create new adcirc_nodes on the heap
    int allocateNodes();

    /// \brief Private function to allocate a vector of pointers and create new adcirc_elements on the heap
    int allocateElements();

    /** \brief Private function to parse the string from an ADCIRC mesh file containing nodal information
     * @param line  [in]    The QString with the information read from the file to be parsed into an ADCIRC node
     * @param index [in]    An integer for the current node position in the ADCIRC file
     * @param *node [inout] The pointer to the adcirc_node that should be created with this information **/
    int readNode(QString line, int index, adcirc_node *node);

    /** \brief Private function to parse the string from an ADCIRC mesh file containing elemental connectivity information
     * @param line     [in]    The QString with the information read from the file to be parsed into an ADCIRC element
     * @param index    [in]    An integer for the current element position in the ADCIRC file
     * @param *element [inout] The pointer to the adcirc_element that should be created with this information **/
    int readElement(QString line, int index, adcirc_element *element);

};

#endif // ADCIRC_MESH_H
