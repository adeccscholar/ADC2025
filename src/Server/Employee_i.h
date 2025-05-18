/*
 * File: Employee.h
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

#pragma once

#include "OrganizationS.h" // Skeleton Header
#include "EmployeeData.h"

// --- Servant-Implementierung für das Person-Interface  ---
// Dieses Objekt ist TRANSIENT. , als erstes der Versuch, ohne Deaktivierung
class Person_i : public virtual PortableServer::RefCountServantBase,
   public virtual POA_Organization::Person {
private:
   PersonData data_;

   // Referenz auf den POA, der dieses Objekt verwaltet (nur für Debugging/Info)
   PortableServer::POA_var poa_;
   PortableServer::ObjectId_var oid_;
public:
   Person_i(PersonData const& data, PortableServer::POA_ptr poa);
   // Wichtig für transiente Objekte: Loggen, wann sie zerstört werden.
   virtual ~Person_i();

   // --- IDL Attribute Implementierung (readonly) ---
   virtual CORBA::Long    personId();
   virtual char* firstName();
   virtual char* name();
   virtual Organization::EGender gender();

   // --- IDL Operation Implementierung ---
   virtual char* getFullName();

   // helper for deactivation Setter für die ObjectId, beim Aktivieren setzen
   void set_oid(PortableServer::ObjectId const& oid);

   virtual void destroy() override;
};

// --- Servant-Implementierung für das Employee-Interface (erbt von Person) ---
// Dieses Objekt ist TRANSIENT.
class Employee_i : public virtual PortableServer::RefCountServantBase,
                   public virtual Person_i,
                   public virtual POA_Organization::Employee {
private:
    EmployeeData data_;
	
public:
    Employee_i(const EmployeeData& data, PortableServer::POA_ptr poa);
    // Wichtig für transiente Objekte: Loggen, wann sie zerstört werden.
    virtual ~Employee_i();

    virtual CORBA::Double salary();
    virtual Organization::YearMonthDay startDate();
    virtual CORBA::Boolean isActive();

    virtual void destroy() override;
    // --- Lifecycle Callback (optional, aber nützlich) ---
    // Wird vom POA aufgerufen kurz bevor der Servant gelöscht wird.
    //virtual void _remove_ref ();
};
