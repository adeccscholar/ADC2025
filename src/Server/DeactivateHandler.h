/*
 * DeactivateHandler.h
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

#include <tao/PortableServer/PortableServer.h>
#include <ace/Event_Handler.h>
#include <ace/Reactor.h>
#include <iostream>
#include <print>

// Hilfstruktur, um mit Hilfe der ACE Freigabe der Objekte zu verzögern und zu verschieben
class DeactivateHandler : public ACE_Event_Handler {
public:
  DeactivateHandler(PortableServer::POA_ptr poa,
                    const PortableServer::ObjectId& oid)
    : poa_(PortableServer::POA::_duplicate(poa)),
      oid_(oid) {}

  int handle_timeout(const ACE_Time_Value&, const void*) override {
    try {
      std::println(std::cerr, "[DeactivateHandler] Deactivating servant...");
      poa_->deactivate_object(oid_);
    } catch (const CORBA::Exception& ex) {
      std::cerr << "[DeactivateHandler] Exception: " << ex << std::endl;
    }
    return 0; // nur einmal ausführen
  }

private:
  PortableServer::POA_var      poa_;
  PortableServer::ObjectId_var oid_;
};
