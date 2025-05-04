/*
 * File: Organization.cpp
 * Servant-Implementierung für die Company (persistent)
 * 
 * 
 * ----------------------------------------------------------------------------------------------------
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
 
#include "Organization_i.h"
#include "Employee_i.h"

#include "OrganizationS.h" // Skeleton Header

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <memory> // Für unique_ptr etc.
#include <thread>
#include <print>

#include <tao/ORB_Core.h>      // für TAO_ORB_Core_instance()
#include <tao/PortableServer/PortableServer.h>

 
Company_i::Company_i(PortableServer::POA_ptr company_poa, PortableServer::POA_ptr employee_poa)
   : employee_poa_(PortableServer::POA::_duplicate(employee_poa)),
     company_poa_(PortableServer::POA::_duplicate(company_poa))
{
   initializeDatabase();
   std::cout << "[Servant Company] Company_i servant CREATED (Company POA: "
      << reinterpret_cast<void*>(company_poa_.in()) << ", Employee POA: "
      << reinterpret_cast<void*>(employee_poa_.in()) << ")." << std::endl;
}

void Company_i::initializeDatabase() {
   CORBA::Long emp_no = 99;
   employee_database_[emp_no] = { ++emp_no, "Max",      "Muster",  Organization::MALE,   55'000.00, {2020,  5, 1}, true  };
   employee_database_[emp_no] = { ++emp_no, "Petra",    "Power",   Organization::FEMALE, 62'000.00, {2019,  3, 1}, true  };
   employee_database_[emp_no] = { ++emp_no, "Klaus",    "Klein",   Organization::MALE,   48'000.00, {2022, 11, 1}, false  };
   employee_database_[emp_no] = { ++emp_no, "Johannes", "Gerlach", Organization::MALE,   63'230.00, {2020,  5, 1}, true  };
   employee_database_[emp_no] = { ++emp_no, "Matthias", "Fehse",   Organization::MALE,   65'500.00, {2020,  5, 1}, true  };
   employee_database_[emp_no] = { ++emp_no, "Gabriele", "Sommer",  Organization::FEMALE, 70'320.50, {2017, 11, 1}, true  };
   employee_database_[emp_no] = { ++emp_no, "Sandra",   "Mayer",   Organization::FEMALE, 55'100.00, {2020,  5, 1}, true  };
   employee_database_[emp_no] = { ++emp_no, "Vanessa",  "Schmitt", Organization::FEMALE, 45'500.25, {2020,  5, 1}, false };
   employee_database_[emp_no] = { ++emp_no, "Christel", "Rau",     Organization::FEMALE, 52'300.00, {2020,  5, 1}, true  };
   employee_database_[emp_no] = { ++emp_no, "Torsten",  "Gutmann", Organization::MALE,   73'500.00, {2016, 12, 1}, true  };
   employee_database_[emp_no] = { ++emp_no, "Stefanie", "Berger",  Organization::FEMALE, 63'352.25, {2020,  5, 1}, true  };
   employee_database_[emp_no] = { ++emp_no, "Hans",     "Mayer",   Organization::MALE,   66'360.10, {2020,  5, 1}, false };
   employee_database_[emp_no] = { ++emp_no, "Reinhard", "Schmidt", Organization::MALE,   61'200.00, {2019, 10, 1}, true  };
   employee_database_[emp_no] = { ++emp_no, "Petra",    "Winther", Organization::FEMALE, 72'650.00, {2017,  4, 1}, true  };
   employee_database_[emp_no] = { ++emp_no, "Julia",    "Schmidt", Organization::FEMALE, 68'250.00, {2020,  5, 1}, true  };
   employee_database_[emp_no] = { ++emp_no, "Mark",     "Krämer",  Organization::MALE,   46'700.20, {2020,  5, 1}, true  };


   std::cout << "[Servant Company] Database initialized with " << employee_database_.size() << " employees." << std::endl;
}

Organization::TimePoint Company_i::getTimeStamp(void) {
   auto now = std::chrono::system_clock::now();
   auto value_milliseconds = duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
   Organization::TimePoint tp { .milliseconds_since_epoch = value_milliseconds };
   return tp;
   }

// --- Implementierung von getEmployees ---
Organization::EmployeeSeq* Company_i::getEmployees() {
   std::cout << "[Servant Company] getEmployees() called by client." << std::endl;

   // Erzeuge die Sequenz für die Rückgabe (Ownership geht an Client)
   //auto employees_seq = std::make_unique<Organization::EmployeeSeq>();
   Organization::EmployeeSeq_var employees_seq = new Organization::EmployeeSeq;
   
   CORBA::ULong current_index = 0;
   for (const auto& [id, data] : employee_database_) { // C++17 structured binding
      try {
         // 1. Erzeuge einen *neuen* Employee Servant für jeden Eintrag
        //    Wichtig: Aktiviere mit dem *transienten* POA!
         Employee_i* employee_servant = new Employee_i(data, employee_poa_.in());

         // 2. *** HIER AKTIVIEREN WIR DEN SERVANT BEIM employee_poa_ ***
         PortableServer::ObjectId_var oid = employee_poa_->activate_object(employee_servant);

         // 3. Erhalte die Referenz vom employee_poa_ (jetzt, nachdem der Servant aktiv ist)
         //    Der employee_poa_ verwaltet nun die Lebensdauer des Servants (bei transienten POAs).
         CORBA::Object_var obj_ref = employee_poa_->id_to_reference(oid.in());
         //Organization::Employee_ptr employee_ref = Organization::Employee::_narrow(obj_ref.in());
         Organization::Employee_var employee_ref = Organization::Employee::_narrow(obj_ref.in());

         if (CORBA::is_nil(employee_ref)) {
            std::cerr << "[Servant Company] CORBA Fehler beim Narrown der Employee-Referenz für ID " << employee_servant->personId() << std::endl;
            // Hier ggf. Fehlerbehandlung
            continue; // Oder break, je nach gewünschtem Verhalten
            }
         employee_servant->set_oid(oid);
         employees_seq->length(current_index + 1);
         (*employees_seq)[current_index++] = employee_ref._retn();
         }
      catch (const CORBA::Exception& ex) {
         std::cerr << "[Servant Company] CORBA Exception creating employee ref for ID "
            << id << ": " << ex << std::endl;
         // Im Fehlerfall wird dieser Eintrag übersprungen
         }
      catch (const std::exception& stdex) {
         std::cerr << "[Servant Company] C++ Exception creating employee ref for ID "
            << id << ": " << stdex.what() << std::endl;
         }
      }

   std::cout << "[Servant Company] Returning " << employees_seq->length() << " employee references." << std::endl;
   // Gib den Besitz des unique_ptr an den Aufrufer (Stub) weiter
   //return employees_seq.release();
   return employees_seq._retn();
}


// --- Implementierung von getEmployee ---
Organization::Employee* Company_i::getEmployee(CORBA::Long personId) {
   std::cout << "[Servant Company] getEmployee(" << personId << ") called by client." << std::endl;

   // 1. Suche in der "Datenbank"
   auto it = employee_database_.find(personId);

   if (it == employee_database_.end()) {
      // 2. NICHT GEFUNDEN: Wirf die benutzerdefinierte Exception
      std::cout << "[Servant Company] Employee ID " << personId << " not found. Throwing EmployeeNotFound." << std::endl;
      Organization::EmployeeNotFound ex;
      ex.requestedId = personId; // Setze das Feld der Exception
      throw ex;
   }

   // 3. GEFUNDEN: Erzeuge dynamisch einen neuen Servant
   EmployeeData const& data = it->second;
   try {
      // Wichtig: Aktivierung wieder mit dem *transienten* POA!
      Employee_i* employee_servant = new Employee_i(data, employee_poa_.in());

      // 2. *** HIER AKTIVIEREN WIR DEN SERVANT BEIM POA ***
      PortableServer::ObjectId_var oid = employee_poa_->activate_object(employee_servant);

      employee_servant->set_oid(oid);

      // 3. Erhalte die Referenz vom POA (jetzt, nachdem der Servant aktiv ist)
      //    Der POA verwaltet nun die Lebensdauer des Servants (bei transienten POAs).
      CORBA::Object_var obj_ref = employee_poa_->id_to_reference(oid.in());
      Organization::Employee_var employee_ref = Organization::Employee::_narrow(obj_ref.in());

      //Organization::Employee_var employee_ref = employee_servant->_this();

      if (CORBA::is_nil(employee_ref)) {
         std::cerr << "[Servant Company] CORBA Fehler beim Narrown der Employee-Referenz für ID " << personId << std::endl;
         // Hier ggf. Fehlerbehandlung (z.B. nullptr zurückgeben oder Exception werfen)
         return nullptr; // Oder eine andere Fehlerbehandlung
         }

      std::cout << "[Servant Company] Found Employee ID " << personId << ". Returning transient reference." << std::endl;
      // Gib Besitz der Referenz an Aufrufer (Stub) weiter
      return employee_ref._retn();
      }
   catch (const CORBA::Exception& ex) {
      std::cerr << "[Servant Company] CORBA Exception creating dynamic employee ref for ID "
         << personId << ": " << ex << std::endl;
      throw CORBA::INTERNAL(); // Unerwarteter Fehler
      }
   catch (const std::exception& stdex) {
      std::cerr << "[Servant Company] C++ Exception creating dynamic employee ref for ID "
         << personId << ": " << stdex.what() << std::endl;
      throw CORBA::INTERNAL(); // Unerwarteter Fehler
      }
   } 

// Implementierung der Methode getEmployeeData
Organization::EmployeeData* Company_i::getEmployeeData(CORBA::Long personId) {
   std::cout << "[Servant Company] getEmployeeData(" << personId << ") called by client." << std::endl;

   // 1. Suche in der "Datenbank" nach der Mitarbeiter-ID
   auto it = employee_database_.find(personId);

   if (it == employee_database_.end()) {
      // 2. Wenn Mitarbeiter nicht gefunden, werfe die benutzerdefinierte Exception
      std::cout << "[Servant Company] Employee ID " << personId << " not found. Throwing EmployeeNotFound." << std::endl;

      // Werfe die Exception und setze die angeforderte ID
      Organization::EmployeeNotFound ex;
      ex.requestedId = personId;
      throw ex;
      }

   // 3. Wenn der Mitarbeiter gefunden wird, bereite die Rückgabe der EmployeeData vor
   const EmployeeData& data = it->second; // Zugriff auf das EmployeeData aus der "Datenbank"

   // Erstelle das Rückgabeobjekt (EmployeeData)
   Organization::EmployeeData* employee_data = new Organization::EmployeeData();

   employee_data->personId  = data.personId;
   employee_data->firstName = CORBA::string_dup(data.firstName.c_str());
   employee_data->name      = CORBA::string_dup(data.name.c_str());
   employee_data->gender    = data.gender;
   employee_data->salary    = data.salary;
   employee_data->startDate = data.startDate;
   employee_data->isActive  = data.isActive;

   std::cout << "[Servant Company] Returning EmployeeData for ID " << personId << std::endl;

   // Rückgabe des EmployeeData
   return employee_data;
}