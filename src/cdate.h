#ifndef ADCMODULES_CDATE_H
#define ADCMODULES_CDATE_H
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
#include <chrono>
#include <cmath>
#include <iostream>
#include <ratio>
#include <string>
#include <type_traits>
#include <vector>
#include "calendartype.h"
#include "logging.h"

namespace Adcirc {

class CDate {
 public:
  CDate();
  CDate(const std::chrono::system_clock::time_point &t);
  CDate(const std::vector<int> &v);
  CDate(const CDate &d);
  CDate(int year, int month = 1, int day = 1, int hour = 0, int minute = 0,
        int second = 0, int millisecond = 0);

  //...operator overloads
  bool operator<(const CDate &d) const;
  bool operator==(const CDate &d) const;
  bool operator!=(const CDate &d) const;

  template <
      class T,
      typename std::enable_if<
          !std::is_integral<T>::value && !std::is_floating_point<T>::value &&
          !std::is_same<T, Adcirc::Calendar::Years>::value &&
          !std::is_same<T, Adcirc::Calendar::Months>::value>::type * = nullptr>
  CDate &operator+=(const T &rhs) {
    this->m_datetime += std::chrono::seconds(10);
    return *this;
  }

  template <
      class T,
      typename std::enable_if<
          !std::is_integral<T>::value && !std::is_floating_point<T>::value &&
          !std::is_same<T, Adcirc::Calendar::Years>::value &&
          !std::is_same<T, Adcirc::Calendar::Months>::value>::type * = nullptr>
  CDate &operator-=(const T &rhs) {
    this->m_datetime -= rhs.nseconds();
    return *this;
  }

  template <class T, typename std::enable_if<std::is_integral<T>::value>::type
                         * = nullptr>
  CDate &operator+=(const T &rhs) {
    this->m_datetime += Adcirc::Calendar::Primitives::seconds(rhs);
    return *this;
  }

  template <class T, typename std::enable_if<
                         std::is_floating_point<T>::value>::type * = nullptr>
  CDate &operator+=(const T &rhs) {
    this->m_datetime += Adcirc::Calendar::Primitives::milliseconds(
        static_cast<long>(std::floor(rhs * 1000.0)));

    return *this;
  }

  CDate &operator+=(const Adcirc::Calendar::Years &rhs);
  CDate &operator+=(const Adcirc::Calendar::Months &rhs);

  template <class T, typename std::enable_if<std::is_integral<T>::value>::type
                         * = nullptr>
  CDate &operator-=(const T &rhs) {
    this->m_datetime -= Adcirc::Calendar::Primitives::seconds(rhs);
    return *this;
  }

  template <class T, typename std::enable_if<
                         std::is_floating_point<T>::value>::type * = nullptr>
  CDate &operator-=(const T &rhs) {
    this->m_datetime -= Adcirc::Calendar::Primitives::milliseconds(
        static_cast<long>(std::floor(rhs * 1000.0)));
    return *this;
  }

  CDate &operator-=(const Adcirc::Calendar::Years &rhs);
  CDate &operator-=(const Adcirc::Calendar::Months &rhs);

  friend std::ostream &operator<<(std::ostream &os, const Adcirc::CDate &dt) {
    os << dt.toString();
    return os;
  }

  // end operator overloads

  static CDate maxDate() { return CDate(3000, 1, 1, 0, 0, 0); }
  static CDate minDate() { return CDate(1900, 1, 1, 0, 0, 0); }

  std::vector<int> get() const;

  void set(const std::vector<int> &v);
  void set(const std::chrono::system_clock::time_point &t);
  void set(const CDate &v);
  void set(int year, int month = 1, int day = 1, int hour = 0, int minute = 0,
           int second = 0, int millisecond = 0);

  void fromSeconds(long seconds);

  void fromMSeconds(long long mseconds);

  long toSeconds() const;

  long long toMSeconds() const;

  int year() const;
  void setYear(int year);
  int month() const;
  void setMonth(int month);

  int day() const;
  void setDay(int day);

  int hour() const;
  void setHour(int hour);

  int minute() const;
  void setMinute(int month);

  int second() const;
  void setSecond(int second);

  int millisecond() const;
  void setMillisecond(int milliseconds);

  void fromString(const std::string &datestr);

  std::string toString() const;

  std::chrono::system_clock::time_point time_point() const;

  static CDate now();

 private:
  std::chrono::system_clock::time_point m_datetime;
};

}  // namespace Adcirc

template <typename T>
Adcirc::CDate operator+(Adcirc::CDate lhs, const T &rhs) {
  lhs += rhs;
  return lhs;
}

template <typename T>
Adcirc::CDate operator-(Adcirc::CDate lhs, const T &rhs) {
  lhs -= rhs;
  return lhs;
}

#ifndef SWIG
template Adcirc::CDate operator+(Adcirc::CDate, const short &);
template Adcirc::CDate operator+(Adcirc::CDate, const int &);
template Adcirc::CDate operator+(Adcirc::CDate, const long &);
template Adcirc::CDate operator+(Adcirc::CDate, const unsigned short &);
template Adcirc::CDate operator+(Adcirc::CDate, const unsigned int &);
template Adcirc::CDate operator+(Adcirc::CDate, const unsigned long &);
template Adcirc::CDate operator+(Adcirc::CDate, const float &);
template Adcirc::CDate operator+(Adcirc::CDate, const double &);
template Adcirc::CDate operator+(Adcirc::CDate,
                                 const Adcirc::Calendar::Milliseconds &);
template Adcirc::CDate operator+(Adcirc::CDate,
                                 const Adcirc::Calendar::Seconds &);
template Adcirc::CDate operator+(Adcirc::CDate,
                                 const Adcirc::Calendar::Minutes &);
template Adcirc::CDate operator+(Adcirc::CDate,
                                 const Adcirc::Calendar::Hours &);
template Adcirc::CDate operator+(Adcirc::CDate, const Adcirc::Calendar::Days &);
template Adcirc::CDate operator+(Adcirc::CDate,
                                 const Adcirc::Calendar::Months &);
template Adcirc::CDate operator+(Adcirc::CDate,
                                 const Adcirc::Calendar::Weeks &);
template Adcirc::CDate operator+(Adcirc::CDate,
                                 const Adcirc::Calendar::Years &);
template Adcirc::CDate operator-(Adcirc::CDate, const short &);
template Adcirc::CDate operator-(Adcirc::CDate, const int &);
template Adcirc::CDate operator-(Adcirc::CDate, const long &);
template Adcirc::CDate operator-(Adcirc::CDate, const unsigned short &);
template Adcirc::CDate operator-(Adcirc::CDate, const unsigned int &);
template Adcirc::CDate operator-(Adcirc::CDate, const unsigned long &);
template Adcirc::CDate operator-(Adcirc::CDate, const float &);
template Adcirc::CDate operator-(Adcirc::CDate, const double &);
template Adcirc::CDate operator-(Adcirc::CDate,
                                 const Adcirc::Calendar::Milliseconds &);
template Adcirc::CDate operator-(Adcirc::CDate,
                                 const Adcirc::Calendar::Seconds &);
template Adcirc::CDate operator-(Adcirc::CDate,
                                 const Adcirc::Calendar::Minutes &);
template Adcirc::CDate operator-(Adcirc::CDate,
                                 const Adcirc::Calendar::Hours &);
template Adcirc::CDate operator-(Adcirc::CDate, const Adcirc::Calendar::Days &);
template Adcirc::CDate operator-(Adcirc::CDate,
                                 const Adcirc::Calendar::Months &);
template Adcirc::CDate operator-(Adcirc::CDate,
                                 const Adcirc::Calendar::Weeks &);
template Adcirc::CDate operator-(Adcirc::CDate,
                                 const Adcirc::Calendar::Years &);
#endif

#endif  // ADCMODULES_CDATE_H
