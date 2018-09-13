#ifndef RASTERDATA_H
#define RASTERDATA_H

#include <string>
#include <vector>
#include "pixel.h"
#include "point.h"

class GDALDataset;
class GDALRasterBand;

class Rasterdata {
 public:
  Rasterdata();

  Rasterdata(std::string filename);

  ~Rasterdata();

  enum RasterTypes {
    Bool,
    Unsigned,
    Float,
    Double,
    Integer,
    ComplexInt,
    ComplexDouble,
    Unknown
  };

  bool open();
  bool close();

  size_t nx() const;

  size_t ny() const;

  double dx() const;

  double dy() const;

  double xmin() const;

  double xmax() const;

  double ymin() const;

  double ymax() const;

  double nodata() const;

  Point pixelToCoordinate(size_t i, size_t j);
  Point pixelToCoordinate(Pixel &p);

  Pixel coordinateToPixel(double x, double y);
  Pixel coordinateToPixel(Point &p);

  std::string projectionString() const;

  int searchBoxAroundPoint(double x, double y, double width, Pixel &upperLeft,
                           Pixel &lowerRight);

  std::string filename() const;
  void setFilename(const std::string &filename);

  double pixelValueDouble(Pixel &p);
  double pixelValueDouble(size_t i, size_t j);
  int pixelValueInt(Pixel p);
  int pixelValueInt(size_t i, size_t j);

  int pixelValues(size_t ibegin, size_t jbegin, size_t iend, size_t jend,
                  std::vector<double> &x, std::vector<double> &y,
                  std::vector<int> &z);

  int pixelValues(size_t ibegin, size_t jbegin, size_t iend, size_t jend,
                  std::vector<double> &x, std::vector<double> &y,
                  std::vector<double> &z);

  int rasterType() const;

  int epsg() const;
  void setEpsg(int epsg);

  bool isOpen() const;

  int nodataint() const;

private:
  void init();
  bool getRasterMetadata();
  RasterTypes selectRasterType(int d);

  GDALDataset *m_file;
  GDALRasterBand *m_band;

  bool m_isOpen;
  size_t m_nx, m_ny;
  int m_epsg;
  double m_dx, m_dy;
  double m_xmin, m_xmax, m_ymin, m_ymax;
  double m_nodata;
  int m_nodataint;
  RasterTypes m_rasterType;
  std::string m_projectionReference;
  std::string m_filename;
};

#endif  // RASTERDATA_H
