/*
 * File: Organization.h
 * Servant-Implementierung für die Company (persistent)
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

#include "../Tools/Tools.h"

#include "OrganizationS.h" // Skeleton Header

#include "Employee_i.h"

#include <iostream>
#include <string>
#include <map>
#include <print>
#include <ranges>


using namespace std::string_literals;

// Servant-Implementierung für das Company-Interface
// Dieses Objekt ist PERSISTENT.
class Company_i : public virtual POA_Organization::Company {
private:
   std::string strName = "Pfefferminzia AG"s;
   // Simulieren einer "Datenbank" der Angestellten für das Beispiel
   std::map<CORBA::Long, EmployeeData> employee_database_;

   // Referenz auf den POA, der Employee-Objekte verwaltet (TRANSIENT)
   PortableServer::POA_var employee_poa_;

   // Referenz auf den POA, der dieses Company-Objekt verwaltet (PERSISTENT)
   // Wird nicht direkt gebraucht, aber gut zu haben.
   PortableServer::POA_var company_poa_;

   // Methode zum Initialisieren der Testdaten
   void initializeDatabase();

public:
   // Konstruktor benötigt die Referenzen auf die relevanten POAs
   Company_i(PortableServer::POA_ptr company_poa, PortableServer::POA_ptr employee_poa);
   virtual ~Company_i() = default;

   // --- IDL Operationen Implementierung ---
   virtual char* nameCompany();
   virtual Organization::TimePoint getTimeStamp(void);

   virtual Organization::EmployeeSeq* getEmployees();
   virtual Organization::EmployeeSeq* getActiveEmployees();
   virtual Organization::Employee*    getEmployee(CORBA::Long personId);

   virtual Organization::EmployeeData* getEmployeeData(CORBA::Long personId);

   virtual double  getSumSalary();

private:

   template <std::ranges::input_range Range>
   Organization::EmployeeSeq* buildEmployeeSequenceFromRange(Range&& range) {
      Organization::EmployeeSeq_var employees_seq = new Organization::EmployeeSeq;
      CORBA::ULong current_index = 0;

      for (const auto& data : range) {
         try {
            Employee_i* employee_servant = new Employee_i(data, employee_poa_.in());
            PortableServer::ObjectId_var oid = employee_poa_->activate_object(employee_servant);
            CORBA::Object_var obj_ref = employee_poa_->id_to_reference(oid.in());
            Organization::Employee_var employee_ref = Organization::Employee::_narrow(obj_ref.in());

            if (CORBA::is_nil(employee_ref)) {
               std::println(std::cerr, "[Servant Company {}] Fehler beim Narrown der Employee-Referenz für ID {}",
                                    ::getTimeStamp(), employee_servant->personId());
               continue;
               }

            employee_servant->set_oid(oid);

            employees_seq->length(current_index + 1);
            (*employees_seq)[current_index++] = employee_ref._retn();
            }
         catch (CORBA::Exception const& ex) {
            std::println(std::cerr, "[Servant Company {}] CORBA Exception für Employee: {}", ::getTimeStamp(), toString(ex));
            }
         catch (std::exception const& stdex) {
            std::println(std::cerr, "[Servant Company {}] C++ Exception für Employee: {}", ::getTimeStamp(), stdex.what());
            }
         }

      std::println(std::cout, "[Servant Company {}] Returning {} employee references.", ::getTimeStamp(), employees_seq->length());
      return employees_seq._retn();
      }

};
