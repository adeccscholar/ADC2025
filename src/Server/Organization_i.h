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

#include "OrganizationS.h" // Skeleton Header

#include "Employee_i.h"
#include <string>
#include <map>


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
    virtual Organization::TimePoint getTimeStamp(void);

    virtual Organization::EmployeeSeq* getEmployees();
    virtual Organization::Employee* getEmployee(CORBA::Long personId);

    virtual Organization::EmployeeData* getEmployeeData(CORBA::Long personId);
};
