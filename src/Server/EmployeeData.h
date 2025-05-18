/*
 * File: EmployeeData.h
 * Hilfstrukturen zur einfachen Speicherung der Rohdaten einer Person oder Angestellten
 *
 * > Diese würde normalerweise aus einer Datenbank gelesen werden.
 * > Alternativ wäre auch struct innerhalb von CORBA möglich, zusätzliche Eigenschaften, aber kein std::string
 * 
 * ----------------------------------------------------------------------------------------------------
 *
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
/*
 * File: EmployeeData.h
 * interne Datenstruktur
 * 
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

#include "OrganizationC.h" // Stub aus CORBA workflow, für EGender, YearMonthDay aus der IDL
#include <string>
#include <print>

#include <tao/ORB_Core.h>     
#include <tao/PortableServer/PortableServer.h>


using namespace std::string_literals;

// Struktur zur einfachen Speicherung der Rohdaten einer Person
// Diese würde normalerweise aus einer Datenbank gelesen werden.
// könnte auch als struct in CORBA Idl definiert werden, nur dann ohne std::string
struct PersonData {
    CORBA::Long   personId       = -1;
    std::string   firstName      = ""s;
    std::string   name           = ""s;
    Organization::EGender gender = Organization::OTHER; // Default
};

// Struktur zur einfachen Speicherung der Rohdaten eines Angestellten
// Diese würde normalerweise aus einer Datenbank gelesen werden.
// Verzicht auf Vererbung, also Daten der Person enthalten
struct EmployeeData : public PersonData {
  // CORBA::Long   personId               = -1;
  // std::string   firstName              = ""s;
  // std::string   name                   = ""s;
  // Organization::EGender gender         = Organization::OTHER; // Default
   double        salary                 = 0.0;    
   Organization::YearMonthDay startDate = {0, 0, 0};
   CORBA::Boolean isActive              = false;
};


