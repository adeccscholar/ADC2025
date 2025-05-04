/** \file
    \brief Demonstrationsbeispiel für CORBA mit TAO: Trennung von persistenten und transienten Objekten
    \authors Volker Hillmann
    \date    05.05.2025
    \version 1.0
    \details Dieses Beispiel demonstriert:
              - die Trennung von persistenten (Company) und transienten (Employee) Objekten mithilfe verschiedener POAs,
              - die korrekte Handhabung von Objektlebenszyklen,
              - die Verwendung einer benutzerdefinierten Exception,
              - sowie das Ausführen des ORBs in einem separaten Thread für einen reaktionsfähigeren Server.
 
    \copyright Copyright (c) 2025 adecc Systemhaus GmbH
    \copyright Dieses Programm ist freie Software: Sie können es unter den Bedingungen der
               GNU General Public License, Version 3, wie von der Free Software Foundation
               veröffentlicht, weitergeben und/oder modifizieren.
 
    \copyright Dieses Programm wird in der Hoffnung bereitgestellt, dass es nützlich ist,
               aber OHNE JEDE GEWÄHRLEISTUNG – sogar ohne die implizite Gewährleistung der
               MARKTGÄNGIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
 
    \copyright Sie sollten eine Kopie der GNU General Public License zusammen mit diesem Programm erhalten haben.
               Falls nicht, siehe <https://www.gnu.org/licenses/>.
 
    \note Teil des Projekts: adecc Scholar – Freies Lernmaterial für modernes C++
*/
 
#include "../Tools/Tools.h"

#include "EmployeeData.h"

#include "Organization_i.h"

#include <tao/corba.h>
#include <tao/PortableServer/PortableServer.h>
#include "tao/PortableServer/LifespanPolicyC.h"
#include "tao/PortableServer/LifespanPolicyA.h"
#include <orbsvcs/CosNamingC.h> 

#include <iostream>
#include <thread>     
#include <chrono>     
#include <csignal>    
#include <atomic>     
#include <print>

#ifdef _WIN32
   #include <windows.h>
#endif

// Globale Variable für den ORB (hier global, in Produktion besser in einer Klasse kapseln)
CORBA::ORB_var orb_glob;

// Atomares Flag für sauberen Shutdown, wird in Signal- Funktion verwendet
std::atomic<bool> shutdown_requested = false;

// Signal Handler Funktion
void signal_handler(int signum) {
   std::println(std::cout);
   std::println(std::cout, "[Signal Handler] Signal {} received. Requesting shutdown...", signum);
   shutdown_requested = true;
   }

// Parameter für ORB- Initialisierung notwendig, falls ACEMain ist ein Rückgabewert erforderlich
int main(int argc, char *argv[]) {
    
   // Signal Handler registrieren (optional, für Strg+C)
   signal(SIGINT,  signal_handler);
   signal(SIGTERM, signal_handler);

#ifdef _WIN32
   SetConsoleOutputCP(CP_UTF8);
#endif

   std::println(std::cout, "[Server Main {}] Test- Server ADC 2025", getTimeStamp());
   try {
      // 1. ORB initialisieren, Achtung, diese Methode assimiliert die Parameter 
      orb_glob = CORBA::ORB_init(argc, argv);
      std::println(std::cout, "[Server Main {}] ORB initialized.", getTimeStamp());

      // 2. RootPOA Referenz holen und POA Manager aktivieren
      CORBA::Object_var poa_object = orb_glob->resolve_initial_references("RootPOA");
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow(poa_object.in());
      if (CORBA::is_nil(root_poa.in())) throw std::runtime_error("Failed to narrow RootPOA");

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager();
      poa_manager->activate();
      std::println(std::cout, "[Server Main {}] RootPOA obtained and POAManager activated.", getTimeStamp());

      // 3. Policies für die beiden POAs erstellen     
      //    Policy für PERSISTENTen POA (für Company)
      CORBA::PolicyList comp_pol;
      comp_pol.length(1);
      comp_pol[0] = root_poa->create_lifespan_policy(PortableServer::PERSISTENT);

      //    Policy für TRANSIENTen POA (für Employee)
      CORBA::PolicyList empl_pol;
      empl_pol.length(2);
      empl_pol[0] = root_poa->create_lifespan_policy(PortableServer::LifespanPolicyValue::TRANSIENT);
      empl_pol[1] = root_poa->create_servant_retention_policy(PortableServer::ServantRetentionPolicyValue::RETAIN);
      // Alternativ NO_RETAIN: aber dann wird locator verwendet und Daten bei jedem Aufruf gelesen
      //empl_pol[1] = root_poa->create_servant_retention_policy(PortableServer::ServantRetentionPolicyValue::NON_RETAIN);
      //empl_pol[2] = root_poa->create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);
      
      // 4. Die beiden POAs als Kinder des RootPOA erstellen
      PortableServer::POA_var company_poa  = root_poa->create_POA("CompanyPOA", poa_manager.in(), comp_pol);
      PortableServer::POA_var employee_poa = root_poa->create_POA("EmployeePOA", poa_manager.in(), empl_pol);

      // Policies nach Gebrauch zerstören
      for (uint32_t i = 0; i < 1; ++i) comp_pol[i]->destroy();
      for (uint32_t i = 0; i < 2; ++i) empl_pol[i]->destroy();

      std::println(std::cout, "[Server Main {}] Persistent CompanyPOA and Transient EmployeePOA created.", getTimeStamp());

      // 5. Company Servant erstellen (mit beiden POAs, da dieser später die Employees erstellt)
      auto company_servant = std::make_unique<Company_i>(company_poa.in(), employee_poa.in());

      // 6. Company Servant mit dem *persistenten* POA aktivieren
      PortableServer::ObjectId_var company_oid = company_poa->activate_object(company_servant.get());
        
      // Referenz für Naming Service holen (Ownership verbleibt beim POA)
      CORBA::Object_var company_ref_obj = company_poa->id_to_reference(company_oid.in());
      Organization::Company_var company_ref = Organization::Company::_narrow(company_ref_obj.in());
      if (CORBA::is_nil(company_ref.in())) 
         throw std::runtime_error("Failed to narrow Company reference after activation");
      
      std::println(std::cout, "[Server Main {}] Company servant activated under CompanyPOA.", getTimeStamp());

      // 7. Naming Service Referenz holen und Company Objekt registrieren
      CORBA::Object_var naming_obj = orb_glob->resolve_initial_references("NameService");
      CosNaming::NamingContext_var naming_context = CosNaming::NamingContext::_narrow(naming_obj.in());
      if (CORBA::is_nil(naming_context.in())) throw std::runtime_error("Failed to narrow Naming Context");

      std::string strName = "GlobalCorp/CompanyService";
      CosNaming::Name name;
      name.length(1);
      name[0].id   = CORBA::string_dup(strName.c_str()); // Beispiel für Pfad im Namen
      name[0].kind = CORBA::string_dup("Object");

      // Ggf. Kontextpfad erstellen (vereinfacht: rebind nutzen)
      // naming_context->bind_context(...);
      naming_context->rebind(name, company_ref.in());
      std::println(std::cout, "[Server Main {}] Company object registered with NameService under '{}'.", getTimeStamp(), strName);
      
      // 8. ORB Event Loop in einem separaten Thread starten (std::jthread für autom. join)
      std::println(std::cout, "[Server Main {}] Starting ORB event loop in a separate thread...", getTimeStamp());

      std::thread orb_thread([&]() {
            try {
               orb_glob->run();
               std::println(std::cout, "[ORB Thread {}] orb->run() finished.", getTimeStamp());
               } 
            catch (CORBA::Exception const& ex) {
               std::println(std::cerr, "[ORB Thread {}] CORBA Exception in orb->run(): ", getTimeStamp(), toString(ex));
               } 
            catch (...) {
               std::println(std::cerr, "[ORB Thread {}] Unknown exception in orb->run().", getTimeStamp());
               }
            });

      // 9. Hauptthread wartet auf Shutdown-Signal
      std::println(std::cout, "[Server Main {}] Server ready. Waiting for shutdown signal (e.g., Ctrl+C)...", getTimeStamp());
 
      while (!shutdown_requested) {
         std::this_thread::sleep_for(std::chrono::milliseconds(200));
         }

      // 10. Aufräumen
      std::println(std::cout, "[Server Main {}] Unbinding from NameService...", getTimeStamp());
      try {
        naming_context->unbind(name);
        }
      catch (CORBA::Exception const& ex) {
         std::println(std::cerr, "[Server Main {}] Error unbinding from NameService (maybe already gone): {}", getTimeStamp(), toString(ex));
         }

      // 11. ORB beenden – weckt orb->run() im orb_thread auf
      std::println(std::cout, "[Server Main {}] Shutdown requested, calling orb->shutdown(false) …", getTimeStamp());
      orb_glob->shutdown(false); // false = nicht blockieren, beende aber sofort run()
  
      // 12. ORB wurde gestoppt, warte auf Thread-Ende ===
      if (orb_thread.joinable()) {
         orb_thread.join();
         }

      std::println(std::cout, "[Server Main{}] Destroying POAs...", getTimeStamp());
      // 13. Zerstöre POAs (true = warte auf laufende Requests, true = entferne Objekte)
      // Wichtig: Kinder zuerst (oder RootPOA mit 'true' für Kaskadierung, falls unterstützt)
      // Hier zerstören wir explizit beide, da sie Geschwister sind
      employee_poa->destroy(true, true);
      company_poa->destroy(true, true);

      // Root POA auch, falls man eigene Objekte darin hätte (hier nicht der Fall)
      root_poa->destroy(true, true);
      std::println(std::cout, "[Server Main {}] POAs destroyed.", getTimeStamp());

      }
   catch (Organization::EmployeeNotFound const& ex) {
      // Sollte hier nicht auftreten, aber zur Vollständigkeit
      std::println(std::cerr, "[Server Main {}] Unexpected EmployeeNotFound Exception: ID {}", getTimeStamp(), ex.requestedId);
      return 1;
      } 
   catch (CORBA::Exception const& ex) {
      std::println(std::cerr, "[Server Main {}] CORBA Exception: {}", getTimeStamp(), toString(ex));
      return 1;
      } 
   catch (std::exception const& ex) {
      std::println(std::cerr, "[Server Main {}] Standard Exception: {}", getTimeStamp(), ex.what());
       return 1;
       } 
   catch (...) {
      std::println(std::cerr, "[Server Main {}] Unknown exception caught.", getTimeStamp());
      return 1;
      }

   // 14. ORB Zerstörung nach dem Join des Threads
   if (!CORBA::is_nil(orb_glob.in())) {
      try {
         std::println(std::cout, "[Server Main {}] Destroying ORB...", getTimeStamp());
         orb_glob->destroy();
         std::println(std::cout, "[Server Main {}] ORB destroyed.", getTimeStamp());
         } 
      catch (const CORBA::Exception& ex) {
         std::println(std::cerr, "[Server Main {}] Exception while destroying ORB: {}", getTimeStamp(), toString(ex));
         }
      }

   std::println(std::cout, "[Server Main {}] Server exited successfully.", getTimeStamp());
   return 0;
   }
