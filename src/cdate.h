#ifndef DATE_H
#define DATE_H
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
#include <ctime>
#include <string>

class Date {
 public:
  Date();
  Date(int year, int month, int day, int hour, int minute, int second);

  void add(long seconds);

  void get(int year, int month, int day, int hour, int minute,
           int second);

  void set(int year, int month, int day, int hour, int minute, int second);

  void fromSeconds(long long seconds);

  void fromMSeconds(long long mseconds);

  long long toSeconds();

  long long toMSeconds();

  int year();
  void setYear(int year);

  int month();
  void setMonth(int month);

  int day();
  void setDay(int day);

  int hour();
  void setHour(int hour);

  int minute();
  void setMinute(int month);

  int second();
  void setSecond(int second);

  void fromString(const std::string &datestr);

  std::string toString();

  static Date now();

 private:
  void buildTm();
  void buildDate(int year, int month, int day, int hour, int minute,
                 int second);
  void buildDate();

  std::time_t m_date;
  std::time_t m_epoch;
  struct std::tm m_tm;
  struct std::tm m_epoch_tm;
};
#endif
