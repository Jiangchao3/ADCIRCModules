#include "adcirc_global_output.h"
#include "netcdf.h"
#include <QDebug>



adcirc_global_output::adcirc_global_output(QString filename, QObject *parent) : QObject(parent)
{
    this->filename = filename;
    this->isMeshInitialized = false;
    this->initializeNetcdfVariables();
    this->error = new QADCModules_errors(this);
}



int adcirc_global_output::read(int record)
{
    int ierr = this->readAdcircGlobalOutputNetCDF(record);
    this->error->setError(ierr);
    return this->error->getError();
}



int adcirc_global_output::initializeNetcdfVariables()
{
    this->netCDFVariables.clear();
    this->netCDFVariables.append("sigmat");
    this->netCDFVariables.append("salinity");
    this->netCDFVariables.append("temperature");
    this->netCDFVariables.append("u-vel3D");
    this->netCDFVariables.append("v-vel3D");
    this->netCDFVariables.append("w-vel3D");
    this->netCDFVariables.append("q20");
    this->netCDFVariables.append("l");
    this->netCDFVariables.append("ev");
    this->netCDFVariables.append("qsurfkp1");
    this->netCDFVariables.append("zeta");
    this->netCDFVariables.append("zeta_max");
    this->netCDFVariables.append("u-vel");
    this->netCDFVariables.append("v-vel");
    this->netCDFVariables.append("vel_max");
    this->netCDFVariables.append("pressure");
    this->netCDFVariables.append("pressure_min");
    this->netCDFVariables.append("windx");
    this->netCDFVariables.append("windy");
    this->netCDFVariables.append("wind_max");
    this->netCDFVariables.append("radstress_x");
    this->netCDFVariables.append("radstress_y");
    this->netCDFVariables.append("radstress_max");
    this->netCDFVariables.append("swan_HS");
    this->netCDFVariables.append("swan_HS_max");
    this->netCDFVariables.append("swan_DIR");
    this->netCDFVariables.append("swan_DIR_max");
    this->netCDFVariables.append("swan_TM01");
    this->netCDFVariables.append("swan_TM01_max");
    this->netCDFVariables.append("swan_TPS");
    this->netCDFVariables.append("swan_TPS_max");
    this->netCDFVariables.append("swan_windx");
    this->netCDFVariables.append("swan_windy");
    this->netCDFVariables.append("swan_wind_max");
    this->netCDFVariables.append("swan_TM02");
    this->netCDFVariables.append("swan_TM02_max");
    this->netCDFVariables.append("swan_TMM10");
    this->netCDFVariables.append("swan_TMM10_max");
    return ERROR_NOERROR;
}



int adcirc_global_output::findNetCDFVariable(int &ncid, int &numVariables, int &variable1, int &variable2)
{
    QString varname;
    int i,ierr;

    for(i=0;i<this->netCDFVariables.size();i++)
    {
        varname = this->netCDFVariables.at(i);
        ierr = nc_inq_varid(ncid,varname.toStdString().c_str(),&variable1);
        if(ierr==NC_NOERR)
        {
            numVariables = 1;
            if(varname=="u-vel")
            {
                numVariables = 2;
                ierr = nc_inq_varid(ncid,"v-vel",&variable2);
                if(ierr!=NC_NOERR)
                    return ERROR_ADCIRCOUTPUT_NOVARIABLE;
            }
            else if(varname=="windx")
            {
                numVariables = 2;
                ierr = nc_inq_varid(ncid,"windy",&variable2);
                if(ierr!=NC_NOERR)
                    return ERROR_ADCIRCOUTPUT_NOVARIABLE;
            }
            else if(varname=="radstress_x")
            {
                numVariables = 2;
                ierr = nc_inq_varid(ncid,"radstress_y",&variable2);
                if(ierr!=NC_NOERR)
                    return ERROR_ADCIRCOUTPUT_NOVARIABLE;
            }
            else if(varname=="swan_windx")
            {
                numVariables = 2;
                ierr = nc_inq_varid(ncid,"swan_windy",&variable2);
                if(ierr!=NC_NOERR)
                    return ERROR_ADCIRCOUTPUT_NOVARIABLE;
            }
            return ERROR_NOERROR;
        }
    }
    return ERROR_ADCIRCOUTPUT_NOVARIABLE;
}



int adcirc_global_output::readAdcircGlobalOutputNetCDF(int record)
{
    int i,ierr,numColumns;
    int ncid,varid1,varid2,dimid_time,varid_time;
    int start_int,count_int;
    double *timeList,*column1,*column2;
    size_t nSnaps;

    //...Check if the file exists
    QFile thisRegFile(this->filename);
    if(!thisRegFile.exists())
    {
        this->error->setError(ERROR_FILENOEXIST);
        return this->error->getError();
    }

    //...First time here, we need to read the ADCIRC mesh data (nodes, elements only)
    if(!this->isMeshInitialized)
    {
        this->mesh = new adcirc_mesh(this);
        ierr = this->mesh->readNetCDF(this->filename);
        this->numNodes = this->mesh->numNodes;
        if(ierr == ERROR_NOERROR)
            this->isMeshInitialized = true;
        else
        {
            this->error->setError(ierr);
            return this->error->getError();
        }
    }

    //...Open the netCDF file
    ierr = nc_open(this->filename.toStdString().c_str(),NC_NOWRITE,&ncid);

    //...Get the dimid for time
    ierr = nc_inq_dimid(ncid,"time",&dimid_time);
    if(ierr!=NC_NOERR)
    {
        this->error->setCustomDescription(nc_strerror(ierr));
        this->error->setError(ERROR_NETCDF_GENERIC);
        return this->error->getError();
    }

    //...Get the varid for time
    ierr = nc_inq_varid(ncid,"time",&varid_time);
    if(ierr!=NC_NOERR)
    {
        this->error->setCustomDescription(nc_strerror(ierr));
        this->error->setError(ERROR_NETCDF_GENERIC);
        return this->error->getError();
    }

    //...Check if the record requested is valid
    ierr = nc_inq_dimlen(ncid,dimid_time,&nSnaps);
    if(ierr!=NC_NOERR)
    {
        this->error->setCustomDescription(nc_strerror(ierr));
        this->error->setError(ERROR_NETCDF_GENERIC);
        return this->error->getError();
    }
    if(record > static_cast<int>(nSnaps))
    {
        this->error->setError(ERROR_ADCIRCOUTPUT_EXCEEDEDSDIM);
        return this->error->getError();
    }

    //...Find the netCDF variable that is present in this file
    ierr = this->findNetCDFVariable(ncid,numColumns,varid1,varid2);
    if(ierr!=ERROR_NOERROR)
    {
        this->error->setError(ierr);
        return this->error->getError();
    }

    start_int             = (record-1)*this->mesh->numNodes;
    count_int             =  this->mesh->numNodes;
    static size_t nNode   =  static_cast<size_t>(count_int);
    static size_t start[] = {static_cast<size_t>(start_int)};
    static size_t count[] = {nNode};

    //...Allocate memory
    timeList = (double*)malloc(sizeof(double)*nSnaps);
    column1  = (double*)malloc(sizeof(double)*nNode);
    if(numColumns==2)
        column2 = (double*)malloc(sizeof(double)*nNode);

    //...Read data from netCDF file
    ierr = nc_get_var(ncid,varid_time,timeList);
    if(ierr!=NC_NOERR)
    {
        this->error->setCustomDescription(nc_strerror(ierr));
        this->error->setError(ERROR_NETCDF_GENERIC);
        return this->error->getError();
    }

    ierr = nc_get_vara_double(ncid,varid1,start,count,column1);
    if(ierr!=NC_NOERR)
    {
        this->error->setCustomDescription(nc_strerror(ierr));
        this->error->setError(ERROR_NETCDF_GENERIC);
        return this->error->getError();
    }

    if(numColumns==2)
    {
        ierr = nc_get_vara_double(ncid,varid2,start,count,column2);
        if(ierr!=NC_NOERR)
        {
            this->error->setCustomDescription(nc_strerror(ierr));
            this->error->setError(ERROR_NETCDF_GENERIC);
            return this->error->getError();
        }
    }

    //...Save the data into the output variable
    this->outputData.clear();
    this->outputData.resize(1);
    this->outputData[0] = new adcirc_output_record(this->mesh->numNodes,this);
    this->outputData[0]->modelTime = timeList[record-1];
    this->numColumns = numColumns;
    if(numColumns==1)
    {
        this->outputData[0]->scalar.resize(this->mesh->numNodes);
        for(i=0;i<this->mesh->numNodes;i++)
            this->outputData[0]->scalar[i] = column1[i];
    }
    else if(numColumns==2)
    {
        this->outputData[0]->vector_u.resize(this->mesh->numNodes);
        this->outputData[0]->vector_v.resize(this->mesh->numNodes);
        for(i=0;i<this->mesh->numNodes;i++)
        {
            this->outputData[0]->vector_u[i] = column1[i];
            this->outputData[0]->vector_v[i] = column2[i];
        }
    }

    //...Deallocate memory
    free(timeList);
    free(column1);
    if(numColumns==2)
        free(column2);

    return ERROR_NOERROR;
}
