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
 
#include "Tools.h"

#include "OrganizationC.h" // Client Stubs
#include <tao/corba.h>
#include <orbsvcs/CosNamingC.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept> // Für Exceptions
#include <chrono>    // Für sleep
#include <thread>    // Für sleep

#ifdef _WIN32
   #include <windows.h>
#endif

inline std::string toString(CORBA::String_var const& s) {
   return std::string { s.in() };  // safe: in() returns const char*, ownership retained
   }

inline std::string toString(char* s) {
   std::string result = s ? std::string { s } : std::string {};
   CORBA::string_free(s);  // important for CORBA::String type return values, the memory would allocated from orb!
   return result;
   }

// === Beispiel: getEmployee(personId) ===
void testReadEmployee(Organization::Company_ptr comp_in, CORBA::Long seekId) {
   Organization::Company_var company_var = Organization::Company::_duplicate(comp_in);
   std::println(std::cout, "[Client {}] Requesting employee with ID: {}", 
         getTimeStamp(from_timepoint(company_var->getTimeStamp())), seekId);
   Organization::Employee_var employee_var = Organization::Employee::_nil();
   try {
      std::println(std::cout, "   Entering scope for Employee_var (ID: {}) ...", seekId);
      employee_var = company_var->getEmployee(seekId);

      if (!CORBA::is_nil(employee_var.in())) {
         CORBA::String_var fullName = employee_var->getFullName();
         CORBA::Boolean active = employee_var->isActive();
         std::println(std::cout, "    Found Employee - Name: {}, Active: {}, Salary: {:.2f} ",
                                        employee_var->getFullName(), 
                                        (employee_var->isActive() ? "Yes" : "No"), 
                                        employee_var->salary());
         }
      else {
         std::println(std::cerr, "    WARNING: getEmployee({}) returned nil unexpectedly.", seekId);
         }
      std::println(std::cout, "  ...leaving scope for Employee_var (ID: {}). Reference released.", seekId);
      }
   catch (Organization::EmployeeNotFound const& ex) { 
      std::println(std::cerr, "  ERROR: Caught EmployeeNotFound for ID {}", ex.requestedId);
      }
   catch (CORBA::Exception const& ex) {
      std::ostringstream os;
      os << ex; 
      std::println(std::cerr, "  ERROR: CORBA Exception during getEmployee({}): {}", seekId, os.str());
      }
   catch(std::exception const& ex) {
      std::println(std::cerr, "  ERROR: C++ Exception during getEmployee({}): {}", seekId, ex.what());
      }

   // HIER zerstören wir employee_var, so dass die Referenz auf dem Server freigegeben wird.
   // Server zerstört den transienten Servant.
   if (!CORBA::is_nil(employee_var.in())) {
      employee_var->destroy();
      employee_var = Organization::Employee::_nil();
      }
   }

// === Beispiel: getEmployees() ===
void testReadEmployees(Organization::Company_ptr comp_in) {
   Organization::Company_var company_var = Organization::Company::_duplicate(comp_in);
   std::println(std::cout, "[Client {}] Requesting employees ", 
                 getTimeStamp(from_timepoint(company_var->getTimeStamp())));

   Organization::EmployeeSeq_var employees_seq = company_var->getEmployees();
   std::println(std::cout, "[Client] Received sequence with {} employee references.", employees_seq->length());

   for (CORBA::ULong i = 0; i < employees_seq->length(); ++i) {
      Organization::Employee_ptr current_employee = employees_seq[i];
      if (CORBA::is_nil(current_employee)) {
         std::println(std::cerr, "  WARNING: Nil reference encountered in sequence at index {}", i);
         continue;
         }

      try {
         CORBA::Long id = current_employee->personId(); // Readonly Attribut
         //CORBA::String_var fullName = current_employee->getFullName();
         auto fullName = toString(current_employee->getFullName());
         CORBA::Double salary = current_employee->salary();
         CORBA::Boolean active = current_employee->isActive();
         std::println(std::cout, "  Employee [{:>3}] - ID: {:>4}, Name: {:<25}, Salary: {:>10.2f}, Status: {} ",
                                 i, id, fullName, salary, (active ? "aktiv" : "ausgeschieden"));

         }
      catch (CORBA::Exception const& ex) {
         std::ostringstream os;
         os << ex;
         std::println(std::cerr, "  Error accessing employee data at index {}: {}", i, os.str());
         }
      }

   std::println(std::cout, "[Client] End of scope for EmployeeSeq_var. Releasing references in sequence...");

   for (CORBA::ULong i = 0; i < employees_seq->length(); ++i) {
      if (!CORBA::is_nil(employees_seq[i].in())) {
         employees_seq[i]->destroy();
         employees_seq[i] = Organization::Employee::_nil();
         }
      }
   } 


int main(int argc, char *argv[]) {
#ifdef _WIN32
   SetConsoleOutputCP(CP_UTF8);
#endif
   std::println(std::cout, "[Client] Client Testprogramm für die ADC2025.");
   std::unique_ptr<Organization::EmployeeData> empData;
   try {
      // 1. ORB initialisieren
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
      std::println(std::cout, "[Client] ORB initialized.");

      // 2. Naming Service Referenz holen
      CORBA::Object_var naming_obj = orb->resolve_initial_references("NameService");
      CosNaming::NamingContext_var naming_context = CosNaming::NamingContext::_narrow(naming_obj.in());
      if (CORBA::is_nil(naming_context.in())) throw std::runtime_error("Failed to narrow Naming Context.");
      std::println(std::cout, "[Client] Naming Service context obtained.");

      // 3. Company Objekt Referenz vom Naming Service holen
      CosNaming::Name name;
      name.length(1);
      name[0].id = CORBA::string_dup("GlobalCorp/CompanyService"); 
      name[0].kind = CORBA::string_dup("Object");

      std::println(std::cout, "[Client] Resolving 'GlobalCorp/CompanyService'...");
      CORBA::Object_var company_obj = naming_context->resolve(name);
      Organization::Company_var company_var = Organization::Company::_narrow(company_obj.in());
      if (CORBA::is_nil(company_var.in())) throw std::runtime_error("Failed to narrow Company reference.");
      std::println(std::cout, "[Client] Successfully obtained reference to Company object.");

      // Firmenname und Gehaltsummer bestimmen
      std::println(std::cout, "Firma: {}, zu zahlende Gehälter: {:.2f}", company_var->nameCompany(), company_var->getSumSalary());

      // Direkter Transfer, Daten werden direkt übertragen und dann nur lokal gespeichert
      empData.reset(company_var->getEmployeeData(110));

      // === Beispiel: getEmployees() ===
      std::println(std::cout);
      std::println(std::cout, "--- Calling getEmployees() ---");
      testReadEmployees(company_var.in());
      
      // Kurze Pause, um Server-Logs zu beobachten (optional)
      std::this_thread::sleep_for(std::chrono::seconds(2));
 
      // === Beispiel: getEmployee(personId) ===
      // --- Fall 1: Existierender Angestellter ---
      std::println(std::cout);
      std::println(std::cout, " --- Calling getEmployee() ---");

      CORBA::Long existingId = 101;
      std::println(std::cout);
      testReadEmployee(company_var.in(), existingId);
      
      // Kurze Pause (optional)
      std::this_thread::sleep_for(std::chrono::seconds(2));

      // --- Fall 2: Nicht existierender Angestellter ---
      CORBA::Long nonExistingId = 999;
      std::println(std::cout);
      testReadEmployee(company_var.in(), nonExistingId);
      
      // Kurze Pause (optional)
      std::this_thread::sleep_for(std::chrono::seconds(2));

      // 4. Aufräumen
      std::println(std::cout);
      std::println(std::cout, "[Client] Shutting down.");
        
      while (orb->work_pending()) orb->perform_work();
      orb->destroy();

      std::println(std::cout, "[Client] ORB destroyed.");
      } 
   catch (Organization::EmployeeNotFound const& ex) {
      // Falls die Exception außerhalb des spezifischen try-catch auftritt
      std::println(std::cerr, "[Client] Unhandled EmployeeNotFound Exception: ID {}", ex.requestedId);
      return 1;
      } 
   catch (CORBA::COMM_FAILURE const& ex) {
      std::cerr << "[Client] CORBA Communication Failure: " << ex << std::endl;
      std::cerr << "           Is the server running and reachable?" << std::endl;
      std::cerr << "           Is the NameService running and reachable via ORBInitRef?" << std::endl;
      return 1;
      }
   catch (CORBA::TRANSIENT const& ex) {
      std::cerr << "[Client] CORBA Transient Exception: " << ex << std::endl;
      std::cerr << "           The server might be starting up, shutting down, or busy." << std::endl;
      std::cerr << "           Try again later." << std::endl;
      return 1;
      }
   catch (CORBA::Exception const& ex) {
      std::println(std::cerr, "[Client] CORBA Exception: {}", toString(ex));
      return 1;
      }
   catch (std::exception const& stdex) {
      std::println(std::cerr, "[Client] Standard Exception: {}",stdex.what());
      return 1;
      }
   catch (...) {
      std::println(std::cerr, "[Client] Unknown exception caught.");
      return 1;
      }

   std::println(std::cout);
   std::println(std::cout, "[Client] Client exited gracefully.");

   if(empData) 
      std::cout << "\nvorher eingelesene Daten, lokal:\n"
                << "Mitarbeiter " << empData->personId << ": "
                << empData->firstName << " " << empData->name << ", "
                << "Gehalt: " << empData->salary << ", "
                << "Aktiv: " << (empData->isActive ? "Ja" : "Nein") << ", "
                << "Startdatum: " << empData->startDate.day << "."
                                  << empData->startDate.month << "." << empData->startDate.year
                << std::endl;

    return 0;
   }
