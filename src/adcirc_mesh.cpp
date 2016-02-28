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
#include "adcirc_mesh.h"

//-----------------------------------------------------------------------------------------//
// Initializer
//-----------------------------------------------------------------------------------------//
/** \brief Constructor for the ADCIRC mesh class
 *
 * \author Zach Cobell
 *
 * @param *parent [in] reference to QObject. Enables automatic memory management to avoid memory leaks
 *
 * Constructs an adcirc_mesh object, takes QObject reference as input
 *
 **/
//-----------------------------------------------------------------------------------------//
adcirc_mesh::adcirc_mesh(QObject *parent) : QObject(parent)
{
    //...Initialize the errors so we can
    //   describe what went wrong to the
    //   user later    
    this->error = new adcirc_errors(this);

    //...By default, we will assume that the mesh numbering should be sequential
    this->ignoreMeshNumbering = false;
    this->meshNeedsNumbering  = false;

    //...Assume this is a geographic coordinate system by default. After
    //   the mesh is read, this will be checked to be sure
    this->isGCS = true;

    return;
}
//-----------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------//
//
//
//   P U B L I C
//             F U N C T I O N S
//
//
//-----------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------//
//...Public function to read an ADCIRC mesh. Assumes filename already specified
//-----------------------------------------------------------------------------------------//
/** \brief Read an ADCIRC mesh into the class
 *
 * \author Zach Cobell
 *
 * Function used to read the ADCIRC mesh into the class. Assumes the filename has already
 * been specified. The mesh file will be read in after checking for some simple errors. Error codes
 * will be returned upon any error. Any return besides ERROR_NOERROR is a fatal
 * error
 **/
//-----------------------------------------------------------------------------------------//
int adcirc_mesh::read()
{
    //...Check for a null string
    if(this->filename==QString())
    {
        this->error->errorCode = ERROR_NULLFILENAM;
        return this->error->errorCode;
    }

    //...Check for file exists
    QFile thisFile(this->filename);
    if(!thisFile.exists())
    {
        this->error->errorCode = ERROR_FILENOEXIST;
        return this->error->errorCode;
    }

    //...Assuming these two checks passed, we can call
    //   the main routine
    int ierr = this->readMesh();

    return ierr;
}
//-----------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------//
//...Public function to read an ADCIRC mesh. Assumes filename specified in input
//-----------------------------------------------------------------------------------------//
/** \brief  Read an ADCIRC mesh into the class and set the filename
 *
 *  \author Zach Cobell
 *
 *   @param inputFile [in] specifies the mesh file to be read
 *
 * Function used to read the ADCIRC mesh into the class. Assumes the filename has not already
 * been specified. The mesh file will be read in after checking for some simple errors.
 * Error codes will be returned upon any error. Any return besides ERROR_NOERROR is a
 * fatal error
 **/
//-----------------------------------------------------------------------------------------//
int adcirc_mesh::read(QString inputFile)
{
    //...Set the filename
    this->filename = inputFile;

    //...Check for a null string
    if(this->filename==QString())
    {
        this->error->errorCode = ERROR_NULLFILENAM;
        return this->error->errorCode;
    }

    //...Check for file exists
    QFile thisFile(this->filename);
    if(!thisFile.exists())
    {
        this->error->errorCode = ERROR_FILENOEXIST;
        return this->error->errorCode;
    }

    //...Assuming these two checks passed, we can call
    //   the main routine
    int ierr = this->readMesh();

    return ierr;
}
//-----------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------//
//...The publicly exposed function to write an ADCIRC mesh
//-----------------------------------------------------------------------------------------//
/** \brief  Public function to allow user to trigger writing
 * of the ADCIRC mesh contained within this class
 *
 *  \author Zach Cobell
 *
 *  @param  outputFile [in] Name of the file to write
 *
 * Function used by the user to write the ADCIRC mesh currently contained within the class.
 * If the file already exists, it will be overwritten. Tread carefully.
 **/
//-----------------------------------------------------------------------------------------//
int adcirc_mesh::write(QString outputFile)
{
    if(outputFile==QString())
    {
        this->error->errorCode = ERROR_NULLFILENAM;
        return this->error->errorCode;
    }

    //...Assuming the filename is valid, write the mesh
    int ierr = this->writeMesh(outputFile);

    return ierr;
}
//-----------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------//
//...The publicly exposed function to set the ignoreMeshnumbering variable
//-----------------------------------------------------------------------------------------//
/** \brief Public function to determine if non-sequential mesh numbering is a fatal error
 *
 * \author Zach Cobell
 *
 * @param  value [in] if true, mesh numbering is ignored.
 * If false, mesh numbering that is nonsequential is fatal
 *
 * Function used by the user to set if the code will determine that non-sequantial
 * mesh numbering is a fatal error. This applies to both the node portion of the file
 * as well as the element lists. This code can handle misnumbered meshes, however, ADCIRC
 * itself cannot.
 **/
//-----------------------------------------------------------------------------------------//
int adcirc_mesh::setIgnoreMeshNumbering(bool value)
{
    this->ignoreMeshNumbering = value;
    this->error->errorCode = ERROR_NOERROR;
    return this->error->errorCode;
}
//-----------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------//
//
//
//   P R O T E C T E D
//             F U N C T I O N S
//
//
//-----------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------//
//...Function to read an ADCIRC mesh
//-----------------------------------------------------------------------------------------//
/** \brief This function is used internally to read an ADCIRC mesh
 *
 * \author Zach Cobell
 *
 * This function is used internally to read an ADCIRC mesh. The mesh will have its filename
 * specified in the adcirc_mesh::filename variable.
 **/
//-----------------------------------------------------------------------------------------//
int adcirc_mesh::readMesh()
{

    QFile       meshFile(this->filename);
    QString     meshStringData,tempString;
    QStringList meshFileList,tempList;
    QByteArray  meshByteData;
    int         i,ierr,filePosition;
    bool        err;

    if(!meshFile.open(QIODevice::ReadOnly))
    {
        this->error->errorCode = ERROR_FILEOPENERR;
        return this->error->errorCode;
    }

    //...Read the entire mesh file at once
    meshByteData   = meshFile.readAll();
    meshStringData = QString(meshByteData);
    meshFileList   = meshStringData.split("\n");
    meshByteData   = QByteArray();
    meshStringData = QString();
    meshFile.close();

    //...Grab the header
    tempString = meshFileList.value(0);
    this->title = tempString.simplified();

    //...Grab the number of elements,nodes
    tempString = meshFileList.value(1);
    tempList   = tempString.simplified().split(" ");
    tempString = tempList.value(1);
    this->numNodes = tempString.toInt(&err);
    if(!err)
    {
        this->error->errorCode = ERROR_MESHREAD_HEADER;
        return this->error->errorCode;
    }
    tempString = tempList.value(0);
    this->numElements = tempString.toInt(&err);
    if(!err)
    {
        this->error->errorCode = ERROR_MESHREAD_HEADER;
        return this->error->errorCode;
    }

    //...Do a quick check on the length of the file
    if(meshFileList.length()<this->numNodes+this->numElements+2)
    {
        this->error->errorCode = ERROR_MESHREAD_UNEXPECTEDEND;
        return this->error->errorCode;
    }

    //...Allocate the nodes
    this->allocateNodes();

    //...Allocate the elements
    this->allocateElements();

    //...Loop over the nodes
    for(i=0;i<this->numNodes;i++)
    {

        ierr = this->nodes[i]->fromString(meshFileList.value(i+2));

        if(ierr!=ERROR_NOERROR)
        {
            this->error->errorCode = ierr;
            return this->error->errorCode;
        }

        //...Check that the numbering is acceptable
        if(this->nodes[i]->id != i+1)
        {
            if(this->ignoreMeshNumbering)
                this->meshNeedsNumbering = true;
            else
            {
                this->error->errorCode = ERROR_MESHREAD_NODNUM;
                return this->error->errorCode;
            }
        }

        //...Save the mapping. This is used to prevent
        //   issues with meshes that are not numbered sequentially
        this->nodeMapping[this->nodes[i]->id] = i;
    }

    //...Loop over the elements
    for(i=0;i<this->numElements;i++)
    {
        ierr = this->elements[i]->fromString(meshFileList.value(this->numNodes+2+i),this->nodes,this->nodeMapping);
        if(ierr!=ERROR_NOERROR)
        {
            this->error->errorCode = ierr;
            return this->error->errorCode;
        }

        //...Check that the numbering is acceptable
        if(this->elements[i]->id != i+1)
        {
            if(this->ignoreMeshNumbering)
                this->meshNeedsNumbering = true;
            else
            {
                this->error->errorCode = ERROR_MESHREAD_ELENUM;
                return this->error->errorCode;
            }
        }

        //...Save the mapping. This is used to prevent
        //   issues with meshes that are not numbered sequentially
        this->elementMapping[this->elements[i]->id] = i;
    }

    //...Read the open boundary
    filePosition = this->numNodes + this->numElements + 2;
    ierr = this->readOpenBoundaries(filePosition,meshFileList);
    if(ierr!=ERROR_NOERROR)
        return ierr;

    //...Read the land boundaries
    this->readLandBoundaries(filePosition,meshFileList);
    if(ierr!=ERROR_NOERROR)
        return ierr;

    //...Determine how we are going to write back the coordinates
    this->senseCoordinateSystem();

    //...The mesh read is now complete. We're done.

    return ERROR_NOERROR;
}
//-----------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------//
//...Allocates an array of pointers in the ADCIRC mesh
//-----------------------------------------------------------------------------------------//
/** \brief Creates a number of adcirc_node variables on the heap
 *
 * \author Zach Cobell
 *
 * This function creates a set of ADCIRC nodes on the heap. All nodes that area
 * created are done so with a QObject reference to enable automatic memory management
 * to avoid memory leaks
 **/
//-----------------------------------------------------------------------------------------//
int adcirc_mesh::allocateNodes()
{
    this->nodes.resize(this->numNodes);
    for(int i=0;i<this->numNodes;i++)
        this->nodes[i] = new adcirc_node(this);
    return ERROR_NOERROR;
}
//-----------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------//
//...Allocates an array of pointers in the ADCIRC mesh
//-----------------------------------------------------------------------------------------//
/** \brief Creates a number of adcirc_element variables on the heap
 *
 * \author Zach Cobell
 *
 * This function creates a set of ADCIRC elements on the heap. All elements that area
 * created are done so with a QObject reference to enable automatic memory management
 * to avoid memory leaks
 **/
//-----------------------------------------------------------------------------------------//
int adcirc_mesh::allocateElements()
{
    this->elements.resize(this->numElements);
    for(int i=0;i<this->numElements;i++)
        this->elements[i] = new adcirc_element(this);
    return ERROR_NOERROR;
}
//-----------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------//
// Function to read the open boundary segmenets of the adcirc mesh file
//-----------------------------------------------------------------------------------------//
/** \brief Protected function to read the entire set of open boundary conditions
 *
 * \author Zach Cobell
 *
 * @param position [inout] The current file position. Returned as the new file position when the open boundary read is complete
 * @param fileData [in]    Reference to the data read from the ADCIRC mesh file
 *
 * Protected function to read the entire set of open boundary conditions
 */
//-----------------------------------------------------------------------------------------//
int adcirc_mesh::readOpenBoundaries(int &position, QStringList &fileData)
{
    QString tempString;
    int i,j,ierr;
    int boundaryCode,boundarySize;
    bool err;

    if(position>fileData.length()-1)
        return ERROR_MESHREAD_UNEXPECTEDEND;

    //...Read the header
    tempString = fileData[position];

    position = position + 1;

    tempString = tempString.simplified().split(" ").value(0);
    this->numOpenBoundaries = tempString.toInt(&err);
    if(!err)
    {
        this->error->errorCode = ERROR_MESHREAD_BNDERR;
        return this->error->errorCode;
    }

    if(position>fileData.length()-1)
        return ERROR_MESHREAD_UNEXPECTEDEND;

    tempString = fileData[position];
    position = position + 1;
    tempString = tempString.simplified().split(" ").value(0);
    this->totNumOpenBoundaryNodes = tempString.toInt(&err);
    if(!err)
    {
        this->error->errorCode = ERROR_MESHREAD_BNDERR;
        return this->error->errorCode;
    }

    //...Allocate the boundary array
    this->openBC.resize(this->numOpenBoundaries);

    //...Read the boundaries
    for(i=0;i<this->numOpenBoundaries;i++)
    {
        if(position>fileData.length()-1)
            return ERROR_MESHREAD_UNEXPECTEDEND;

        //...Read the number of nodes in the boundary
        tempString = fileData[position];
        position = position + 1;
        tempString = tempString.simplified().split(" ").value(0);

        //...Set a default code so we know it is an open boundary
        //   if for some reason we don't realize it
        boundaryCode = -9999;

        //...Get the size of this open boundary
        boundarySize = tempString.toInt(&err);
        if(!err)
        {
            this->error->errorCode = ERROR_MESHREAD_BNDERR;
            return this->error->errorCode;
        }

        //...Create a new adcirc_boundary object on the heap
        this->openBC[i] = new adcirc_boundary(boundaryCode,boundarySize,this);

        //...Loop over the length of the boundary
        for(j=0;j<this->openBC[i]->numNodes;j++)
        {
            if(position>fileData.length()-1)
                return ERROR_MESHREAD_UNEXPECTEDEND;

            tempString = fileData[position];
            position = position + 1;
            ierr = this->openBC[i]->fromString(tempString,j,this->nodes,this->nodeMapping);
            if(ierr!=ERROR_NOERROR)
            {
                this->error->errorCode = ierr;
                return this->error->errorCode;
            }
        }
    }

    return ERROR_NOERROR;

}
//-----------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------//
// Function to read the land boundary segments from the ADCIRC mesh file
//-----------------------------------------------------------------------------------------//
/** \brief Protected function to read the entire set of land boundary conditions
 *
 * \author Zach Cobell
 *
 * @param position [inout] The current file position. Returned as the new file position
 *                         when the land boundary read is complete
 * @param fileData [in]    Reference to the data read from the ADCIRC mesh file
 *
 * Protected function to read the entire set of land boundary conditions
 */
//-----------------------------------------------------------------------------------------//
int adcirc_mesh::readLandBoundaries(int &position, QStringList &fileData)
{
    QString tempString,tempString2;
    int ierr,i,j;
    int boundaryCode,boundarySize;
    bool err;

    if(position>fileData.length()-1)
        return ERROR_MESHREAD_UNEXPECTEDEND;

    //...Read the header
    tempString = fileData[position];
    position = position + 1;
    tempString = tempString.simplified().split(" ").value(0);
    this->numLandBoundaries = tempString.toInt(&err);
    if(!err)
    {
        this->error->errorCode = ERROR_MESHREAD_BNDERR;
        return this->error->errorCode;
    }

    if(position>fileData.length()-1)
        return ERROR_MESHREAD_UNEXPECTEDEND;

    tempString = fileData[position];
    position = position + 1;
    tempString = tempString.simplified().split(" ").value(0);
    this->totNumLandBoundaryNodes = tempString.toInt(&err);
    if(!err)
    {
        this->error->errorCode = ERROR_MESHREAD_BNDERR;
        return this->error->errorCode;
    }

    //...Allocate the boundary array
    this->landBC.resize(this->numLandBoundaries);

    //...Read the boundaries
    for(i=0;i<this->numLandBoundaries;i++)
    {
        if(position>fileData.length()-1)
            return ERROR_MESHREAD_UNEXPECTEDEND;

        //...Read the number of nodes in the boundary
        tempString = fileData[position];
        position   = position + 1;

        //...Set a default code so we know it is an open boundary
        //   if for some reason we don't realize it
        tempString2 = tempString.simplified().split(" ").value(1);
        boundaryCode = tempString2.toInt(&err);
        if(!err)
        {
            this->error->errorCode = ERROR_MESHREAD_BNDERR;
            return this->error->errorCode;
        }

        //...Get the size of this land boundary
        tempString2 = tempString.simplified().split(" ").value(0);
        boundarySize = tempString2.toInt(&err);
        if(!err)
        {
            this->error->errorCode = ERROR_MESHREAD_BNDERR;
            return this->error->errorCode;
        }

        //...Create a new adcirc_boundary object on the heap
        this->landBC[i] = new adcirc_boundary(boundaryCode,boundarySize,this);

        //...Read the boundary string depending on its type
        for(j=0;j<this->landBC[i]->numNodes;j++)
        {
            if(position>fileData.length()-1)
                return ERROR_MESHREAD_UNEXPECTEDEND;

            tempString = fileData[position];
            position = position + 1;
            ierr = this->landBC[i]->fromString(tempString,j,this->nodes,this->nodeMapping);
            if(ierr!=ERROR_NOERROR)
            {
                this->error->errorCode = ierr;
                return this->error->errorCode;
            }
        }
    }

    return ERROR_NOERROR;
}
//-----------------------------------------------------------------------------------------//



//-----------------------------------------------------------------------------------------//
//...Function to write an ADCIRC mesh
//-----------------------------------------------------------------------------------------//
/** \brief This function is used internally to write an ADCIRC mesh
 *
 * \author Zach Cobell
 *
 * @param filename [in] Name of the output mesh file
 *
 * This function is used internally to write an ADCIRC mesh.
 **/
//-----------------------------------------------------------------------------------------//
int adcirc_mesh::writeMesh(QString filename)
{
    int i,j;
    QString tempString;
    QStringList boundaryList;

    QFile outputFile(filename);

    //...Open the output file. Return with error
    //   if any issues occur
    if(!outputFile.open(QIODevice::WriteOnly))
        return ERROR_FILEOPENERR;

    //...Create a stream
    QTextStream out(&outputFile);

    //...Write the header
    out << this->title << "\n";
    out << tempString.sprintf("%11i  %11i",this->numElements,this->numNodes) << "\n";

    //...Write the mesh nodes
    for(i=0;i<this->numNodes;i++)
        out << this->nodes[i]->toString(this->isGCS) << "\n";

    //...Write the mesh elements
    for(i=0;i<this->numElements;i++)
        out << this->elements[i]->toString() << "\n";

    //...Write the open boundary header
    out << this->numOpenBoundaries << "\n";
    out << this->totNumOpenBoundaryNodes << "\n";

    //...Write the open boundaries
    for(i=0;i<this->numOpenBoundaries;i++)
    {
        boundaryList = this->openBC[i]->toStringList(true);
        for(j=0;j<boundaryList.length();j++)
            out << boundaryList.value(j) << "\n";
    }

    //...Write the land boundary header
    out << this->numLandBoundaries << "\n";
    out << this->totNumLandBoundaryNodes << "\n";

    //...Write the land boundaries
    for(i=0;i<this->numLandBoundaries;i++)
    {
        boundaryList = this->landBC[i]->toStringList(false);
        for(j=0;j<boundaryList.length();j++)
            out << boundaryList.value(j) << "\n";
    }

    //...Close the file
    outputFile.close();

    return ERROR_NOERROR;

}

//-----------------------------------------------------------------------------------------//
//...Function to attempt to sense the coordinate system type
//-----------------------------------------------------------------------------------------//
/** \brief This function is used internally sense the type of coordinate system that is used
 *
 * \author Zach Cobell
 *
 * This function is used internally sense the type of coordinate system that is used.
 * Ultimately, it is used to format the coordinates when writing an output file so that
 * precision is maintained. UTM or other simliar coordinate systems will have fewer decimal
 * places than what appears to be a geographic coordinate system
 **/
//-----------------------------------------------------------------------------------------//
int adcirc_mesh::senseCoordinateSystem()
{
    int i;
    qreal avgX,avgY,m1,m2,mag,nn;

    avgX = 0.0;
    avgY = 0.0;

    nn = static_cast<qreal>(this->numNodes);

    for(i=0;i<this->numNodes;i++)
    {
        avgX = avgX + this->nodes[i]->x;
        avgY = avgY + this->nodes[i]->y;
    }

    avgX = qAbs(avgX / nn);
    avgY = qAbs(avgY / nn);

    m1 = qAbs(qLn(avgX)/qLn(10.0));
    m2 = qAbs(qLn(avgY)/qLn(10.0));

    mag = ( m1 + m2 ) / 2.0;

    if(mag>3.0)
        this->isGCS = false;
    else
        this->isGCS = true;

    return ERROR_NOERROR;
}
