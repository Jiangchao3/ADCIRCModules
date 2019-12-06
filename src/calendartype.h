#ifndef ADCMODULES_CALENDAR_H
#define ADCMODULES_CALENDAR_H
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

namespace Adcirc {

namespace Calendar {

namespace Primitives {
using milliseconds = std::chrono::milliseconds;
using seconds = std::chrono::seconds;
using minutes = std::chrono::minutes;
using hours = std::chrono::hours;
using days = std::chrono::duration<
    int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;
using weeks =
    std::chrono::duration<int,
                          std::ratio_multiply<std::ratio<7>, days::period>>;
using years = std::chrono::duration<
    int, std::ratio_multiply<std::ratio<146097, 400>, days::period>>;
using months =
    std::chrono::duration<int,
                          std::ratio_divide<years::period, std::ratio<12>>>;
}  // namespace Primitives

template <class T>
class CalendarType {
 public:
  CalendarType(const T &value) : m_data(value) {}
  CalendarType(const long &value) : m_data(value) {}

  Adcirc::Calendar::Primitives::milliseconds nmilliseconds() const {
    return std::chrono::duration_cast<Calendar::Primitives::milliseconds>(
        this->m_data);
  }

  Adcirc::Calendar::Primitives::seconds nseconds() const {
    return std::chrono::duration_cast<Calendar::Primitives::seconds>(
        this->m_data);
  }

  Adcirc::Calendar::Primitives::minutes nminutes() const {
    return std::chrono::duration_cast<Calendar::Primitives::minutes>(
        this->m_data);
  }

  Adcirc::Calendar::Primitives::hours nhours() const {
    return std::chrono::duration_cast<Calendar::Primitives::hours>(
        this->m_data);
  }

  Adcirc::Calendar::Primitives::days ndays() const {
    return std::chrono::duration_cast<Calendar::Primitives::days>(this->m_data);
  }

  Adcirc::Calendar::Primitives::weeks nweeks() const {
    return std::chrono::duration_cast<Calendar::Primitives::weeks>(
        this->m_data);
  }

  Adcirc::Calendar::Primitives::months nmonths() const {
    return std::chrono::duration_cast<Calendar::Primitives::months>(
        this->m_data);
  }

  Adcirc::Calendar::Primitives::years nyears() const {
    return std::chrono::duration_cast<Calendar::Primitives::years>(
        this->m_data);
  }

 protected:
  T m_data;
};

class Milliseconds : public CalendarType<Primitives::milliseconds> {
 public:
  Milliseconds(const Primitives::seconds &value)
      : CalendarType<Primitives::milliseconds>(value) {}
  Milliseconds(const int &value)
      : CalendarType<Primitives::milliseconds>(value) {}
  Milliseconds(const long &value)
      : CalendarType<Primitives::milliseconds>(value) {}
};

class Seconds : public CalendarType<Primitives::seconds> {
 public:
  Seconds(const Primitives::seconds &value)
      : CalendarType<Primitives::seconds>(value) {}
  Seconds(const long &value) : CalendarType<Primitives::seconds>(value) {}
};

class Minutes : public CalendarType<Primitives::minutes> {
 public:
  Minutes(const Primitives::minutes &value)
      : CalendarType<Primitives::minutes>(value) {}
  Minutes(const long &value) : CalendarType<Primitives::minutes>(value) {}
};

class Hours : public CalendarType<Primitives::hours> {
 public:
  Hours(const Primitives::hours &value)
      : CalendarType<Primitives::hours>(value) {}
  Hours(const long &value) : CalendarType<Primitives::hours>(value) {}
};

class Days : public CalendarType<Primitives::days> {
 public:
  Days(const Primitives::days &value) : CalendarType<Primitives::days>(value) {}
  Days(const long &value) : CalendarType<Primitives::days>(value) {}
};

class Weeks : public CalendarType<Primitives::weeks> {
 public:
  Weeks(const Primitives::weeks &value)
      : CalendarType<Primitives::weeks>(value) {}
  Weeks(const long &value) : CalendarType<Primitives::weeks>(value) {}
};

class Months : public CalendarType<Primitives::months> {
 public:
  Months(const Primitives::months &value)
      : CalendarType<Primitives::months>(value) {}
  Months(const long &value) : CalendarType<Primitives::months>(value) {}
};

class Years : public CalendarType<Primitives::years> {
 public:
  Years(const Primitives::years &value)
      : CalendarType<Primitives::years>(value) {}
  Years(const long &value) : CalendarType<Primitives::years>(value) {}
};

}  // namespace Calendar
}  // namespace Adcirc

#endif  // ADCMODULES_CALENDAR_H
