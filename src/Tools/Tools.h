/*
 * Copyright (c) 2025 adecc Systemhaus GmbH
 *
 * Dieses Programm ist freie Software: Sie können es unter den Bedingungen der
 * GNU General Public License, Version 3, wie von der Free Software Foundation
 * veröffentlicht, weitergeben und/oder modifizieren.
 *
 * Dieses Programm wird in der Hoffnung bereitgestellt, dass es nützlich ist,
 * aber OHNE JEDE GEWÄHRLEISTUNG – sogar ohne die implizite Gewährleistung der
 * MARKTGÄNGIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
 *
 * Sie sollten eine Kopie der GNU General Public License zusammen mit diesem Programm erhalten haben.
 * Falls nicht, siehe <https://www.gnu.org/licenses/>.
 *
 * Teil des Projekts: adecc Scholar – Freies Lernmaterial für modernes C++
 */
 
#pragma once

#include "OrganizationC.h"

#include <string>
#include <sstream>
#include <chrono>
#include <format>

#include <tao/corba.h>

inline std::string getTimeStamp(std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now()) {
   auto const cz_ts = std::chrono::current_zone()->to_local(now);
   auto const millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
   return std::format("{:%d.%m.%Y %X},{:03d}", cz_ts, millis.count());
   }

inline std::string toString(CORBA::Exception const& ex) {
   std::ostringstream os;
   os << ex;
   return os.str();
   }
   
inline std::chrono::system_clock::time_point from_timepoint(Organization::TimePoint const& tp) {
   using namespace std::chrono;
   return system_clock::time_point(milliseconds(tp.milliseconds_since_epoch));
   }
   
inline std::chrono::year_month_day convertToChrono(Organization::YearMonthDay const& ymd) {
   return std::chrono::year_month_day{ std::chrono::year { ymd.year },
                                       std::chrono::month { ymd.month },
                                       std::chrono::day { ymd.day } };
   }
   