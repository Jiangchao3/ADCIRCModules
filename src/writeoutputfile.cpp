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
#include "writeoutputfile.h"
#include "adcirc_outputfiles.h"
#include "boost/format.hpp"
#include "hdf5.h"
#include "logging.h"
#include "netcdf.h"

using namespace Adcirc::Output;

static boost::format s_adcircAsciiScalar("%8i     %20.10e");
static boost::format s_adcircAsciiVector("%8i     %20.10e     %20.10e");
static boost::format s_adcircAscii3d("%8i     %20.10e     %20.10e     %20.10e");
static boost::format s_adcircRecordHeaderSparse(
    "%20.10e     %10i  %10i %20.10e");
static boost::format s_adcircRecordHeaderFull("%20.10e     %10i ");

WriteOutput::WriteOutput(const std::string &filename,
                         Adcirc::Output::ReadOutputFile *dataContainer,
                         Adcirc::Geometry::Mesh *mesh)
    : m_dataContainer(dataContainer),
      m_mesh(mesh),
      m_filename(filename),
      m_recordsWritten(0) {
  this->m_format = Adcirc::Output::getOutputFormatFromExtension(filename);
  this->m_isOpen = false;
}

WriteOutput::~WriteOutput() {
  if (this->m_isOpen) this->close();
}

void WriteOutput::open() {
  if (this->m_format == Adcirc::Output::OutputAsciiFull ||
      this->m_format == Adcirc::Output::OutputAsciiSparse) {
    this->openFileAscii();
  } else if (this->m_format == Adcirc::Output::OutputNetcdf4) {
    this->openFileNetCDF();
  } else if (this->m_format == Adcirc::Output::OutputHdf5) {
    this->openFileHdf5();
  }
  this->m_isOpen = true;
}

void WriteOutput::close() {
  if (this->m_format == Adcirc::Output::OutputAsciiFull ||
      this->m_format == Adcirc::Output::OutputAsciiSparse) {
    if (this->m_fid.is_open()) this->m_fid.close();
  } else if (this->m_format == Adcirc::Output::OutputNetcdf4 ||
             this->m_format == Adcirc::Output::OutputNetcdf3) {
    nc_close(this->m_ncid);
  } else if (this->m_format == Adcirc::Output::OutputHdf5) {
    H5Fclose(this->m_h5fid);
  }
  this->m_isOpen = false;
}

void WriteOutput::writeSparseAscii(bool s) {
  if (this->m_format == Adcirc::Output::OutputAsciiFull ||
      this->m_format == Adcirc::Output::OutputAsciiSparse) {
    if (s) {
      this->m_format = Adcirc::Output::OutputAsciiSparse;
    } else {
      this->m_format = Adcirc::Output::OutputAsciiFull;
    }
  }
}

void WriteOutput::write(const OutputRecord *record,
                        const OutputRecord *record2) {
  if (!this->m_isOpen) {
    adcircmodules_throw_exception("WriteOutput: File has not been opened.");
  }
  if (this->m_format == Adcirc::Output::OutputAsciiFull) {
    this->writeRecordAsciiFull(record);
  } else if (this->m_format == Adcirc::Output::OutputAsciiSparse) {
    this->writeRecordAsciiSparse(record);
  } else if (this->m_format == Adcirc::Output::OutputNetcdf3 ||
             this->m_format == Adcirc::Output::OutputNetcdf4) {
    this->writeRecordNetCDF(record);
  } else if (this->m_format == Adcirc::Output::OutputHdf5) {
    this->writeRecordHdf5(record, record2);
  }
  this->m_recordsWritten++;
  return;
}

void WriteOutput::openFileAscii() {
  this->m_fid.open(this->filename(), std::ios::out);
  this->m_fid << this->m_dataContainer->header() << std::endl;
  int fileformat = 1050624;
  std::string headerline = boost::str(
      boost::format("%6i %10i %10.6f %6i %6i FileFmtVersion: %10i") %
      this->m_dataContainer->numSnaps() % this->m_dataContainer->numNodes() %
      this->m_dataContainer->dt() % this->m_dataContainer->dIteration() %
      this->m_dataContainer->metadata()->dimension() % fileformat);
  this->m_fid << headerline << std::endl;
  return;
}

int WriteOutput::defineNetcdfVariable(int dimid_node, const int *dims,
                                      double fill, size_t index) {
  int varid_v, ierr = NC_NOERR;
  if (this->m_dataContainer->metadata()->isMax()) {
    ierr +=
        nc_def_var(this->m_ncid,
                   this->m_dataContainer->metadata()->variable(index).c_str(),
                   NC_DOUBLE, 1, &dimid_node, &varid_v);
  } else {
    ierr +=
        nc_def_var(this->m_ncid,
                   this->m_dataContainer->metadata()->variable(index).c_str(),
                   NC_DOUBLE, 2, dims, &varid_v);
  }
  ierr += nc_put_att_text(
      this->m_ncid, varid_v, "long_name",
      this->m_dataContainer->metadata()->longName(index).size(),
      this->m_dataContainer->metadata()->longName(index).c_str());
  ierr += nc_put_att_text(
      this->m_ncid, varid_v, "standard_name",
      this->m_dataContainer->metadata()->standardName(index).size(),
      this->m_dataContainer->metadata()->standardName(index).c_str());
  ierr +=
      nc_put_att_text(this->m_ncid, varid_v, "units",
                      this->m_dataContainer->metadata()->units(index).size(),
                      this->m_dataContainer->metadata()->units(index).c_str());
  ierr += nc_def_var_fill(this->m_ncid, varid_v, 0, &fill);
  ierr += nc_put_att_double(this->m_ncid, varid_v, "dry_value", NC_DOUBLE, 1,
                            &fill);
  ierr += nc_put_att_text(this->m_ncid, varid_v, "coordinates", 8, "time y x");
  ierr += nc_put_att_text(this->m_ncid, varid_v, "location", 4, "node");
  ierr += nc_put_att_text(this->m_ncid, varid_v, "mesh", 11, "adcirc_mesh");
  if (this->m_dataContainer->metadata()->convention(index) !=
      OutputMetadata::defaultConvention()) {
    ierr += nc_put_att_text(
        this->m_ncid, varid_v, "positive",
        this->m_dataContainer->metadata()->convention(index).size(),
        this->m_dataContainer->metadata()->convention(index).c_str());
  }

  if (this->m_format == Adcirc::Output::OutputNetcdf4) {
    ierr += nc_def_var_deflate(this->m_ncid, varid_v, 1, 1, 2);
  }
  return varid_v;
}

void WriteOutput::openFileNetCDF() {
  int ierr = nc_create(this->filename().c_str(), NC_NETCDF4, &this->m_ncid);
  int dimid_time, dimid_node, dimid_ele, dimid_nvertex, dimid_mesh;

  size_t nn, ne;
  if (this->m_mesh != nullptr) {
    nn = this->m_mesh->numNodes();
    ne = this->m_mesh->numElements();
  } else {
    nn = this->m_dataContainer->numNodes();
    ne = 0;
  }

  ierr += nc_def_dim(this->m_ncid, "time", NC_UNLIMITED, &dimid_time);
  ierr += nc_def_dim(this->m_ncid, "node", nn, &dimid_node);
  ierr += nc_def_dim(this->m_ncid, "ele", ne, &dimid_ele);
  ierr += nc_def_dim(this->m_ncid, "nvertex", 3, &dimid_nvertex);
  ierr += nc_def_dim(this->m_ncid, "mesh", 1, &dimid_mesh);

  const int dim_e[2] = {dimid_ele, dimid_nvertex};
  int varid_x, varid_y, varid_element, varid_mesh, varid_depth;

  ierr += nc_def_var(this->m_ncid, "time", NC_DOUBLE, 1, &dimid_time,
                     &this->m_varid_time);
  ierr += nc_def_var(this->m_ncid, "x", NC_DOUBLE, 1, &dimid_node, &varid_x);
  ierr += nc_def_var(this->m_ncid, "y", NC_DOUBLE, 1, &dimid_node, &varid_y);
  ierr += nc_def_var(this->m_ncid, "element", NC_INT, 2, dim_e, &varid_element);
  ierr += nc_def_var(this->m_ncid, "mesh", NC_INT, 1, &dimid_mesh, &varid_mesh);
  ierr += nc_def_var(this->m_ncid, "depth", NC_DOUBLE, 1, &dimid_node,
                     &varid_depth);

  if (this->m_format == Adcirc::Output::OutputNetcdf4) {
    ierr += nc_def_var_deflate(this->m_ncid, this->m_varid_time, 1, 1, 2);
    ierr += nc_def_var_deflate(this->m_ncid, varid_x, 1, 1, 2);
    ierr += nc_def_var_deflate(this->m_ncid, varid_y, 1, 1, 2);
    ierr += nc_def_var_deflate(this->m_ncid, varid_depth, 1, 1, 2);
    ierr += nc_def_var_deflate(this->m_ncid, varid_element, 1, 1, 2);
  }

  if (ierr != NC_NOERR) {
    adcircmodules_throw_exception(
        "WritOutput: Error initializing netCDF output file.");
    return;
  }

  double fill = this->m_dataContainer->defaultValue();
  const int dims[2] = {dimid_time, dimid_node};

  //...Check if there are variables to define in the metadata. If not,
  //   need to assume some defaults so the code can function
  if (this->m_dataContainer->metadata()->dimension() == 1) {
    if (this->m_dataContainer->metadata()->variable(0) == std::string()) {
      this->m_dataContainer->setMetadata(c_outputMetadata[4]);
    }
  } else if (this->m_dataContainer->metadata()->dimension() == 2) {
    if (this->m_dataContainer->metadata()->variable(1) == std::string()) {
      this->m_dataContainer->setMetadata(c_outputMetadata[7]);
    }
  } else if (this->m_dataContainer->metadata()->dimension() == 3) {
    if (this->m_dataContainer->metadata()->variable(2) == std::string()) {
      this->m_dataContainer->setMetadata(c_outputMetadata[1]);
    }
  }

  for (size_t i = 0; i < this->m_dataContainer->metadata()->dimension(); ++i) {
    this->m_varid.push_back(
        this->defineNetcdfVariable(dimid_node, dims, fill, i));
  }

  if (ierr != NC_NOERR) {
    adcircmodules_throw_exception(
        "WriteOutput: Error initializing variables in netCDF output file.");
    return;
  }

  double dt = this->m_dataContainer->modelDt();

  //...Global attribute section
  ierr += nc_put_att_double(this->m_ncid, NC_GLOBAL, "dt", NC_DOUBLE, 1, &dt);
  ierr += nc_put_att_double(this->m_ncid, NC_GLOBAL, "_FillValue", NC_DOUBLE, 1,
                            &fill);
  ierr += nc_put_att_text(this->m_ncid, NC_GLOBAL, "model", 6, "ADCIRC");
  ierr +=
      nc_put_att_text(this->m_ncid, NC_GLOBAL, "version", 13, "AdcircModules");
  ierr +=
      nc_put_att_text(this->m_ncid, NC_GLOBAL, "grid_type", 10, "triangular");
  ierr += nc_put_att_text(this->m_ncid, NC_GLOBAL, "conventions", 11,
                          "UGRID-0.9.0");
  ierr += nc_enddef(this->m_ncid);
  if (ierr != NC_NOERR) {
    adcircmodules_throw_exception("WriteOutput: Error defining attributes.");
    return;
  }

  if (this->m_mesh != nullptr) {
    ierr += nc_put_var(this->m_ncid, varid_x, &this->m_mesh->x()[0]);
    ierr += nc_put_var(this->m_ncid, varid_y, &this->m_mesh->y()[0]);
    ierr += nc_put_var(this->m_ncid, varid_depth, &this->m_mesh->z()[0]);
  }

  return;
}

void WriteOutput::hdf5_createDataset(const std::string &name, bool isVector) {
  //...Create metadata
  const hsize_t dims_tm[1] = {0};
  const hsize_t dims_val[3] = {0, this->m_dataContainer->numNodes(), 2};
  const hsize_t dims_tm_max[1] = {H5S_UNLIMITED};
  const hsize_t dims_val_max[3] = {H5S_UNLIMITED,
                                   this->m_dataContainer->numNodes(), 2};
  const hsize_t dims_tm_chunk[1] = {1};
  const hsize_t dims_val_chunk[3] = {1, this->m_dataContainer->numNodes(), 1};

  //...Storage chunking
  hid_t props_tm = H5Pcreate(H5P_DATASET_CREATE);
  hid_t props_vl = H5Pcreate(H5P_DATASET_CREATE);

  //...Create group
  hid_t gid = H5Gcreate2(this->m_h5fid, name.c_str(), H5P_DEFAULT, H5P_DEFAULT,
                         H5P_DEFAULT);

  //...Create variable space
  hid_t space_tm = H5Screate_simple(1, dims_tm, dims_tm_max);
  H5Pset_chunk(props_tm, 1, dims_tm_chunk);

  hid_t space_val = 0;
  if (isVector) {
    space_val = H5Screate_simple(3, dims_val, dims_val_max);
    H5Pset_chunk(props_vl, 3, dims_val_chunk);
  } else {
    space_val = H5Screate_simple(2, dims_val, dims_val_max);
    H5Pset_chunk(props_vl, 2, dims_val_chunk);
  }

  //...Compress storage
  H5Pset_deflate(props_tm, 2);
  H5Pset_deflate(props_vl, 2);

  //...Set fillvalue
  if (isVector) {
    float fill = 0.0f;
    H5Pset_fill_value(props_vl, H5T_IEEE_F32LE, &fill);
  } else {
    float fill = Adcirc::Output::defaultOutputValue();
    H5Pset_fill_value(props_vl, H5T_IEEE_F32LE, &fill);
  }

  //...Create datasets
  hid_t did_tm = H5Dcreate2(gid, "Times", H5T_IEEE_F64LE, space_tm, H5P_DEFAULT,
                            props_tm, H5P_DEFAULT);
  hid_t did_val = H5Dcreate2(gid, "Values", H5T_IEEE_F32LE, space_val,
                             H5P_DEFAULT, props_vl, H5P_DEFAULT);

  //...Close
  H5Sclose(space_tm);
  H5Sclose(space_val);
  H5Dclose(did_tm);
  H5Dclose(did_val);
  H5Gclose(gid);

  return;
}

void WriteOutput::openFileHdf5() {
  this->m_h5fid = H5Fcreate(this->m_filename.c_str(), H5F_ACC_TRUNC,
                            H5P_DEFAULT, H5P_DEFAULT);
  hid_t gid_dataset = H5Gcreate2(this->m_h5fid, "/Datasets", H5P_DEFAULT,
                                 H5P_DEFAULT, H5P_DEFAULT);
  this->hdf5_createDataset("/Datasets/Depth-averaged Velocity (64)", true);
  this->hdf5_createDataset("/Datasets/Water Surface Elevation (63)", false);

  H5Gclose(gid_dataset);

  return;
}

void WriteOutput::setFilename(const std::string &filename) {
  this->m_filename = filename;
}

std::string WriteOutput::filename() const { return this->m_filename; }

void WriteOutput::writeAsciiNodeRecord(size_t i, const OutputRecord *record) {
  std::string l;
  if (this->m_dataContainer->metadata()->dimension() == 1) {
    l = boost::str(s_adcircAsciiScalar % (i + 1) % record->z(i));
  } else if (this->m_dataContainer->metadata()->dimension() == 2) {
    if (this->m_dataContainer->metadata()->isMax()) {
      l = boost::str(s_adcircAsciiScalar % (i + 1) % record->z(i));
    } else {
      l = boost::str(s_adcircAsciiVector % (i + 1) % record->u(i) %
                     record->v(i));
    }
  } else if (this->m_dataContainer->metadata()->dimension() == 3) {
    l = boost::str(s_adcircAscii3d % (i + 1) % record->u(i) % record->v(i) %
                   record->w(i));
  }
  this->m_fid << l << std::endl;
  return;
}

void WriteOutput::writeRecordAsciiFull(const OutputRecord *record) {
  std::string header = boost::str(s_adcircRecordHeaderFull % record->time() %
                                  record->iteration());
  this->m_fid << header << std::endl;
  for (size_t i = 0; i < record->numNodes(); ++i) {
    this->writeAsciiNodeRecord(i, record);
  }
  if (this->m_dataContainer->metadata()->isMax() &&
      this->m_dataContainer->metadata()->dimension() == 2) {
    for (size_t i = 0; i < record->numNodes(); ++i) {
      this->m_fid << boost::str(s_adcircAsciiScalar % (i + 1) % record->v(i))
                  << std::endl;
    }
  }
  return;
}

void WriteOutput::writeRecordAsciiSparse(const OutputRecord *record) {
  std::string header = boost::str(
      s_adcircRecordHeaderSparse % record->time() % record->iteration() %
      record->numNonDefault() % record->defaultValue());

  this->m_fid << header << std::endl;
  for (size_t i = 0; i < record->numNodes(); ++i) {
    if (!record->isDefault(i)) {
      this->writeAsciiNodeRecord(i, record);
    }
  }
  if (this->m_dataContainer->metadata()->isMax() &&
      this->m_dataContainer->metadata()->dimension() == 2) {
    for (size_t i = 0; i < record->numNodes(); ++i) {
      if (!record->isDefault(i)) {
        this->m_fid << boost::str(s_adcircAsciiScalar % (i + 1) % record->v(i))
                    << std::endl;
      }
    }
  }
  return;
}

void WriteOutput::writeRecordNetCDF(const OutputRecord *record) {
  double t = record->time();
  nc_put_var1(this->m_ncid, this->m_varid_time, &this->m_recordsWritten, &t);

  const size_t start[2] = {this->m_recordsWritten, 0};
  const size_t count[2] = {1, record->numNodes()};

  if (this->m_dataContainer->metadata()->dimension() == 1) {
    double *z = new double[record->numNodes()];
    for (size_t i = 0; i < record->numNodes(); ++i) {
      z[i] = record->z(i);
    }
    nc_put_vara(this->m_ncid, this->m_varid[0], start, count, z);
    delete[] z;
  } else if (this->m_dataContainer->metadata()->dimension() == 2) {
    double *u = new double[record->numNodes()];
    double *v = new double[record->numNodes()];
    for (size_t i = 0; i < record->numNodes(); ++i) {
      u[i] = record->u(i);
      v[i] = record->v(i);
    }
    nc_put_vara(this->m_ncid, this->m_varid[0], start, count, u);
    nc_put_vara(this->m_ncid, this->m_varid[1], start, count, v);
    delete[] u;
    delete[] v;
  } else if (this->m_dataContainer->metadata()->dimension() == 3) {
    double *u = new double[record->numNodes()];
    double *v = new double[record->numNodes()];
    double *w = new double[record->numNodes()];
    for (size_t i = 0; i < record->numNodes(); ++i) {
      u[i] = record->u(i);
      v[i] = record->v(i);
      w[i] = record->w(i);
    }
    nc_put_vara(this->m_ncid, this->m_varid[0], start, count, u);
    nc_put_vara(this->m_ncid, this->m_varid[1], start, count, v);
    nc_put_vara(this->m_ncid, this->m_varid[2], start, count, w);
    delete[] u;
    delete[] v;
    delete[] w;
  }
  return;
}

void WriteOutput::hdf5_appendRecord(const std::string &name,
                                    const Adcirc::Output::OutputRecord *record,
                                    bool isVector) {
  //...Names
  const std::string name_time = name + "/Times";
  const std::string name_values = name + "/Values";

  //...Get the dataset ids
  hid_t did_tm = H5Dopen2(this->m_h5fid, name_time.c_str(), H5P_DEFAULT);
  hid_t did_val = H5Dopen2(this->m_h5fid, name_values.c_str(), H5P_DEFAULT);

  //...Get data spaces
  hid_t sid_tm = H5Dget_space(did_tm);
  hid_t sid_vl = H5Dget_space(did_val);

  //..Get extents
  hsize_t dimtm[1], dimval[3];
  H5Sget_simple_extent_dims(sid_tm, dimtm, NULL);
  H5Sget_simple_extent_dims(sid_vl, dimval, NULL);

  //...Create new extents
  hsize_t offset_tm[1] = {dimtm[0]};
  hsize_t offset_val[3] = {dimval[0], 0, 0};
  hsize_t dimtm_new[1] = {1};
  hsize_t dimval_new_sca[2] = {1, record->numNodes()};
  hsize_t dimval_new_vec[3] = {1, record->numNodes(), 2};
  dimtm[0]++;
  dimval[0]++;

  //...Extend the datasets
  H5Dset_extent(did_tm, dimtm);
  H5Dset_extent(did_val, dimval);

  //...Get a new hyperslab
  hid_t fs_tm = H5Dget_space(did_tm);
  hid_t fs_val = H5Dget_space(did_val);
  H5Sselect_hyperslab(fs_tm, H5S_SELECT_SET, offset_tm, NULL, dimtm_new, NULL);
  if (isVector) {
    H5Sselect_hyperslab(fs_val, H5S_SELECT_SET, offset_val, NULL,
                        dimval_new_vec, NULL);
  } else {
    H5Sselect_hyperslab(fs_val, H5S_SELECT_SET, offset_val, NULL,
                        dimval_new_sca, NULL);
  }

  //...Define the memory space
  hid_t ms_tm = H5Screate_simple(1, dimtm_new, NULL);
  hid_t ms_val = 0;
  if (isVector) {
    ms_val = H5Screate_simple(3, dimval_new_vec, NULL);
  } else {
    ms_val = H5Screate_simple(2, dimval_new_sca, NULL);
  }

  //...Write the new data
  double time[1] = {record->time()};
  H5Dwrite(did_tm, H5T_IEEE_F64LE, ms_tm, fs_tm, H5P_DEFAULT, time);

  if (isVector) {
    size_t idx = 0;
    float *uv = new float[record->numNodes() * 2];
    for (size_t i = 0; i < record->numNodes(); ++i) {
      float u = static_cast<float>(record->u(i));
      float v = static_cast<float>(record->v(i));
      if (u < -9990) u = 0.0f;
      if (v < -9990) v = 0.0f;
      uv[idx] = u;
      idx++;
      uv[idx] = v;
      idx++;
    }
    H5Dwrite(did_val, H5T_IEEE_F32LE, ms_val, fs_val, H5P_DEFAULT, uv);
  } else {
    float *wse = new float[record->numNodes()];
    for (size_t i = 0; i < record->numNodes(); ++i) {
      wse[i] = static_cast<float>(record->z(i));
    }
    H5Dwrite(did_val, H5T_IEEE_F32LE, ms_val, fs_val, H5P_DEFAULT, wse);
    delete[] wse;
  }

  //...Close datasets
  H5Sclose(ms_tm);
  H5Sclose(ms_val);

  H5Sclose(fs_tm);
  H5Sclose(fs_val);

  H5Sclose(sid_tm);
  H5Sclose(sid_vl);

  H5Dclose(did_tm);
  H5Dclose(did_val);

  return;
}

void WriteOutput::writeRecordHdf5(
    const Adcirc::Output::OutputRecord *recordElevation,
    const Adcirc::Output::OutputRecord *recordVelocity) {
  this->hdf5_appendRecord("/Datasets/Water Surface Elevation (63)",
                          recordElevation, false);
  if (recordVelocity != nullptr) {
    this->hdf5_appendRecord("/Datasets/Depth-averaged Velocity (64)",
                            recordVelocity, true);
  }
  return;
}
