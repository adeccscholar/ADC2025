/*
 * File: Employee.cpp
 * Servant-Implementierung für das  Employee-Interface (transient)
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
 
#include "../Tools/Tools.h"

#include "Employee_i.h"
#include "DeactivateHandler.h"

#include <print>

#include <tao/ORB_Core.h>      // für TAO_ORB_Core_instance()
#include <tao/PortableServer/PortableServer.h>


 // --- Implementierung der Person_i Klasse ---

Person_i::Person_i(PersonData const& data, PortableServer::POA_ptr poa) : 
                data_(data), poa_(PortableServer::POA::_duplicate(poa)) {
   std::println(std::cerr, "[Person_i {}] Objekt erzeugt für ID: {}", ::getTimeStamp(), personId());
}

Person_i::~Person_i() {
   std::println(std::cerr, "[Person_i {}] Objekt zerstört für ID: {}", ::getTimeStamp(), personId());
}

CORBA::Long Person_i::personId() {
   return data_.personId;
}

char* Person_i::firstName() {
   return CORBA::string_dup(data_.firstName.c_str());
}

char* Person_i::name() {
   return CORBA::string_dup(data_.name.c_str());
}

Organization::EGender Person_i::gender() {
   return data_.gender;
}

char* Person_i::getFullName() {
   std::string fullName = data_.firstName + " " + data_.name;
   return CORBA::string_dup(fullName.c_str());
}

void Person_i::destroy() {
   std::println(std::cout, "[Person_i {}] destroy() called for ID {}", ::getTimeStamp(), personId());

   try {
      poa_->deactivate_object(oid_);  // Objekt deregistrieren
   }
   catch (CORBA::Exception const& ex) {
      std::println(std::cerr, "[Employee_i {}] Exception during deactivate_object: {}", ::getTimeStamp(), toString(ex));
   }

   _remove_ref();  // Führt zu delete this bei RefCount 0
}

// Setter für die ObjectId, beim Aktivieren setzen
void Person_i::set_oid(PortableServer::ObjectId const& oid) {
   oid_ = oid;
}

// --- Implementierung der Employee_i Klasse ---

Employee_i::Employee_i(EmployeeData const& data, PortableServer::POA_ptr poa) 
                           : Person_i(static_cast<PersonData const&>(data), poa),
                            data_(data) { 
   std::println(std::cerr, "[Employee_i {}] Objekt erzeugt für ID: {}", ::getTimeStamp(), personId());
   }

Employee_i::~Employee_i() {
   std::println(std::cerr, "[Employee_i {}] Objekt zerstört für ID: {}", ::getTimeStamp(), personId());
   }

CORBA::Double Employee_i::salary() {
   return data_.salary;
   }

Organization::YearMonthDay Employee_i::startDate() {
   return data_.startDate;
   }

CORBA::Boolean Employee_i::isActive() {
   return data_.isActive;
   }


void Employee_i::destroy() {
   std::println(std::cout, "[Employee_i {}] destroy() called for ID {}", ::getTimeStamp(), personId());
   Person_i::destroy();
   }
