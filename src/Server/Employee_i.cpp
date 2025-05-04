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
 
#include "Employee_i.h"
#include "DeactivateHandler.h"

#include <print>

#include <tao/ORB_Core.h>      // für TAO_ORB_Core_instance()
#include <tao/PortableServer/PortableServer.h>

// --- Implementierung der Employee_i Klasse ---

Employee_i::Employee_i(EmployeeData const& data, PortableServer::POA_ptr poa) :
                            data_(data), poa_(PortableServer::POA::_duplicate(poa)) { 
   std::println(std::cerr, "Employee_i::Employee_i - Objekt erzeugt für ID: {}", personId());
   }

Employee_i::~Employee_i() {
   std::println(std::cerr, "Employee_i::~Employee_i - Employee-Objekt zerstört für ID: {}", personId());
   }

CORBA::Long Employee_i::personId() {
   return data_.personId;
   }

char* Employee_i::firstName() {
   return CORBA::string_dup(data_.firstName.c_str());
   }

char* Employee_i::name() {
   return CORBA::string_dup(data_.name.c_str());
   }

Organization::EGender Employee_i::gender() {
   return data_.gender;
   }

char* Employee_i::getFullName() {
   std::string fullName = data_.firstName + " " + data_.name;
   return CORBA::string_dup(fullName.c_str());
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
   std::cout << "[Employee_i] destroy() called for ID " << personId() << std::endl;

   try {
      poa_->deactivate_object(oid_);  // Objekt deregistrieren
      }
   catch (const CORBA::Exception& ex) {
      std::cerr << "[Employee_i] Exception during deactivate_object: " << ex._name() << std::endl;
      }

   _remove_ref();  // Führt zu delete this bei RefCount 0
}

/*
std::cout << "[Employee_i] destroy() called for ID " << personId() << std::endl;

// Triggert die Freigabe, wenn keine weiteren Referenzen da sind
_remove_ref();
*/

/*
void Employee_i::destroy() {
   std::cout << "[Employee_i] destroy() called for ID " << personId() << std::endl;

   // 1) POA + OID besorgen
   PortableServer::POA_var poa = this->_default_POA();
   PortableServer::ObjectId_var oid =
      poa->reference_to_id(this->_this());

   // 2) in TAO's Reactor einen sofort‑Timer planen (0s,0µs)
   ACE_Reactor* reactor = TAO_ORB_Core_instance()->reactor();
   reactor->schedule_timer(new DeactivateHandler(poa.in(), oid.in()),
                           nullptr,                  // act
                           ACE_Time_Value(0, 50000), // delay
                           ACE_Time_Value::zero      // interval
                          );


   // KEIN Timer, KEIN Reactor, KEIN std::thread nötig!
   // Und weil es oneway ist, erwartet der ORB keinen Reply mehr,
   // also gibt es keinen MARSHAL‑Error.

}
*/
/*
// --- Lifecycle Callback ---
// Diese Methode wird vom POA aufgerufen, wenn der Referenzzähler 0 erreicht
// UND die Policy TRANSIENT ist (oder bei expliziter Deaktivierung).
// Der POA wird danach `delete this;` aufrufen.
void Employee_i::_remove_ref () {
   std::println(std::cerr, "Employee_i::_remove_ref - Referenz entfernt für Employee-Objekt mit ID: {}", personId());
  // Da dies ein TRANSIENTEs Objekt ist, löschen wir es, sobald die Referenzen auf Null fallen.
   delete this;
   }
*/ 

// Setter für die ObjectId, beim Aktivieren setzen
void Employee_i::set_oid(PortableServer::ObjectId const& oid) {
   oid_ = oid;
   }