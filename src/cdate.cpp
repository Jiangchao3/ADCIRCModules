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
#include "cdate.h"

#include <time.h>

#include <chrono>
#include <iostream>

#include "boost/format.hpp"

#ifdef _WIN32
#define timegm _mkgmtime
#endif

Date::Date() { this->init(); }

Date::Date(int year, int month, int day, int hour, int minute, int second) {
  this->init();
  this->set(year, month, day, hour, minute, second);
}

bool Date::operator<(const Date &d) const {
  return this->toMSeconds() < d.toMSeconds();
}

bool Date::operator==(const Date &d) const {
  return this->toMSeconds() == d.toMSeconds();
}

bool Date::operator!=(const Date &d) const {
  return this->toMSeconds() != d.toMSeconds();
}

Date Date::maxDate() {
  std::time_t mx = std::numeric_limits<time_t>::max();
  std::tm mx_tm = *gmtime(&mx);
  return Date(mx_tm.tm_year - 1900, mx_tm.tm_mon - 1, mx_tm.tm_mday,
              mx_tm.tm_hour, mx_tm.tm_min, mx_tm.tm_sec);
}

Date Date::minDate() {
  std::time_t mn = std::numeric_limits<time_t>::min();
  std::tm mn_tm = *gmtime(&mn);
  return Date(mn_tm.tm_year - 1900, mn_tm.tm_mon - 1, mn_tm.tm_mday,
              mn_tm.tm_hour, mn_tm.tm_min, mn_tm.tm_sec);
}

void Date::init() {
  struct tm defaultTime = {0};
  this->m_date = timegm(&defaultTime);
}

void Date::add(long seconds) {
  this->m_date = this->m_date + seconds;
  this->buildTm();
  this->buildDate();
  return;
}

void Date::buildTm() {
  this->m_tm = *(gmtime(&this->m_date));
  return;
}

void Date::buildDate(int year, int month, int day, int hour, int minute,
                     int second) {
  this->m_tm.tm_sec = second;
  this->m_tm.tm_min = minute;
  this->m_tm.tm_hour = hour;
  this->m_tm.tm_mday = day;
  this->m_tm.tm_mon = month - 1;
  this->m_tm.tm_year = year - 1900;
  this->m_date = timegm(&this->m_tm);
  return;
}

void Date::buildDate() {
  this->m_date = timegm(&this->m_tm);
  return;
}

void Date::set(int year, int month, int day, int hour, int minute, int second) {
  this->buildDate(year, month, day, hour, minute, second);
  this->buildTm();
  return;
}

void Date::get(int &year, int &month, int &day, int &hour, int &minute,
               int &second) {
  year = this->year();
  month = this->month();
  day = this->day();
  hour = this->hour();
  minute = this->minute();
  second = this->second();
  return;
}

void Date::fromSeconds(long long seconds) {
  this->m_date = seconds;
  this->buildTm();
  this->buildDate();
  return;
}

void Date::fromMSeconds(long long mseconds) {
  this->fromSeconds(mseconds / 1000);
}

long long Date::toSeconds() const {
  return this->m_tm.tm_sec + this->m_tm.tm_min * 60 +
         this->m_tm.tm_hour * 3600 + this->m_tm.tm_yday * 86400 +
         (this->m_tm.tm_year - 70) * 31536000 +
         ((this->m_tm.tm_year - 69) / 4) * 86400 -
         ((this->m_tm.tm_year - 1) / 100) * 86400 +
         ((this->m_tm.tm_year + 299) / 400) * 86400;
}

long long Date::toMSeconds() const { return this->toSeconds() * 1000; }

int Date::year() const { return this->m_tm.tm_year + 1900; }

void Date::setYear(int year) {
  this->m_tm.tm_year = year - 1900;
  this->buildDate();
  this->buildTm();
  return;
}

int Date::month() const { return this->m_tm.tm_mon + 1; }

void Date::setMonth(int month) {
  this->m_tm.tm_mon = month - 1;
  this->buildDate();
  this->buildTm();
  return;
}

int Date::day() const { return this->m_tm.tm_mday; }

void Date::setDay(int day) {
  this->m_tm.tm_mday = day;
  this->buildDate();
  this->buildTm();
  return;
}

int Date::hour() const { return this->m_tm.tm_hour; }

void Date::setHour(int hour) {
  this->m_tm.tm_hour = hour;
  this->buildDate();
  this->buildTm();
  return;
}

int Date::minute() const { return this->m_tm.tm_min; }

void Date::setMinute(int minute) {
  this->m_tm.tm_min = minute;
  this->buildDate();
  this->buildTm();
  return;
}

int Date::second() const { return this->m_tm.tm_sec; }

void Date::setSecond(int second) {
  this->m_tm.tm_sec = second;
  this->buildDate();
  this->buildTm();
  return;
}

void Date::fromString(const std::string &datestr) {
  int year = stoi(datestr.substr(0, 4));
  int month = stoi(datestr.substr(5, 2));
  int day = stoi(datestr.substr(8, 2));
  int hour = stoi(datestr.substr(11, 2));
  int minute = stoi(datestr.substr(14, 2));
  int second = stoi(datestr.substr(17, 2));
  this->set(year, month, day, hour, minute, second);
}

std::string Date::toString() const {
  return boost::str(boost::format("%04.4i-%02.2i-%02.2i %02.2i:%02.2i:%02.2i") %
                    this->year() % this->month() % this->day() % this->hour() %
                    this->minute() % this->second());
}

Date Date::now() {
  auto now = std::chrono::system_clock::now();
  const std::time_t t = std::chrono::system_clock::to_time_t(now);
  std::tm *t2 = gmtime(&t);
  Date d(t2->tm_year + 1900, t2->tm_mon + 1, t2->tm_mday, t2->tm_hour,
         t2->tm_min, t2->tm_sec);
  return d;
}
