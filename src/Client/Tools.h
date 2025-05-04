/*
 * Copyright (c) 2025 adecc Systemhaus GmbH
 *
 * Dieses Programm ist freie Software: Sie k�nnen es unter den Bedingungen der
 * GNU General Public License, Version 3, wie von der Free Software Foundation
 * ver�ffentlicht, weitergeben und/oder modifizieren.
 *
 * Dieses Programm wird in der Hoffnung bereitgestellt, dass es n�tzlich ist,
 * aber OHNE JEDE GEW�HRLEISTUNG � sogar ohne die implizite Gew�hrleistung der
 * MARKTG�NGIGKEIT oder EIGNUNG F�R EINEN BESTIMMTEN ZWECK.
 *
 * Sie sollten eine Kopie der GNU General Public License zusammen mit diesem Programm erhalten haben.
 * Falls nicht, siehe <https://www.gnu.org/licenses/>.
 *
 * Teil des Projekts: adecc Scholar � Freies Lernmaterial f�r modernes C++
 */
#pragma once

#include "OrganizationC.h"

#include <string>
#include <sstream>
#include <chrono>
#include <format>

#include <tao/corba.h>

inline std::string getTimeStamp(std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now()) {
   auto const millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
   return std::format("{:%d.%m.%Y %X},{:03d}", now, millis.count());
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
   