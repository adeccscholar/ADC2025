# Maschinenkommunikation: Warum CORBA/TAO HTTP/3 und gRPC technologisch überlegen ist

## Einleitung

In einer Ära verteilter Systeme, in der Mikroservices, Cloud-Infrastrukturen und plattformübergreifende Anwendungen 
den Alltag prägen, dominieren moderne Protokolle wie **gRPC**, meist betrieben über **HTTP/3**, die Diskussion um 
effiziente Kommunikationsarchitekturen. 

> In der Realität werden viele Schnittstellen aber noch über REST- Services 
betrieben, die entweder mit HTTP/1.1 oder den jeweils folgenden Erweiterungen HTTP/2 oder HTTP/3 betrieben werden.
Dabei spielt kurioser Weise die Lesbarkeit (human readable) eine Rolle.

Dabei wird oft übersehen, dass diese Technologien vor allem darauf ausgelegt sind, die Schwächen des zugrunde 
liegenden **HTTP-Protokolls** zu kompensieren – und nicht auf technologische Eleganz und Effizienz für echte 
Maschine-zu-Maschine-Kommunikation. In scharfem Kontrast dazu steht das bereits in den 1990er Jahren entwickelte 
und bis heute weiterentwickelte **CORBA (Common Object Request Broker Architecture)**, insbesondere in Form 
der Echtzeit-Implementierung **TAO (The ACE ORB)**.

Dieser Artikel vergleicht die technischen Grundlagen, zeigt Latenz- und Architekturunterschiede auf und 
beleuchtet, warum sich CORBA/TAO nicht aus technischen, sondern aus marktstrategischen Gründen aus dem Fokus der 
Öffentlichkeit zurückgezogen hat.

---

## Architekturvergleich: CORBA/TAO vs. HTTP/3 + gRPC

CORBA wurde von Beginn an als Plattform für performante und standardisierte Objektkommunikation entwickelt – mit einem klaren Fokus auf Stabilität, Effizienz und Echtzeitfähigkeit. HTTP/3 und gRPC hingegen entstanden als Antwort auf die strukturellen Schwächen von HTTP/1.1, insbesondere im Kontext verteilter Webanwendungen.

| Kriterium | **CORBA/TAO** | **HTTP/3 + gRPC** |
|:--|:--|:--|
| **Zweck** | Maschine-zu-Maschine Kommunikation (RPC) | Maschinen- und Browser-Kommunikation, API-Orientierung |
| **Verbindungskonzept** | Stabile, langlebige TCP-Verbindung mit dedizierten Ports | UDP-basiertes QUIC-Protokoll, viele Layer, zustandsbehaftetes Management |
| **Handshake** | Einmalig (TCP, optional TLS) | Komplex: QUIC-Handshake, TLS 1.3 integriert |
| **Effizienz** | Minimaler Overhead, direkte Binärkommunikation | Hoher Overhead durch QUIC, HTTP/3 Layer, Multiplexing |
| **Echtzeitfähigkeit** | Echtzeitfähig (TAO Echtzeit-CORBA) | Eingeschränkt durch Layering und Pufferstrategien |
| **Standardisierung** | OMG-Standard, offen, dokumentiert | gRPC proprietär (Google), abhängig von Anbieter-Ökosystem |
| **Komplexität** | Klar definierte, standardisierte IDL-Komplexität | Proprietäre Komplexität durch Protobuf, komplexe Toolchains |
| **Interoperabilität** | Sprach- und plattformübergreifend durch IDL | Eingeschränkt auf gRPC-Ökosysteme mit Protobuf-Support |

---

## Latenzvergleich: TAO vs. HTTP/3 + gRPC

Benchmarks zeigen, dass CORBA/TAO im direkten Vergleich insbesondere in stabilen Netzwerkumgebungen deutlich besser performt:

| Messwert | **CORBA/TAO** | **gRPC über HTTP/3** |
|:--|:--|:--|
| **Verbindungslatenz (initial)** | 1 TCP-Handshake (~1 RTT) | QUIC + TLS 1.3 (~1 RTT, aber komplexer Stack) |
| **Aufruflatenz (bestehende Verbindung)** | ~1–2 ms netto | 3–5x höher durch Layering und Serialisierung |
| **Recovery bei Paketverlust** | TCP-optimiert, einfach gehalten | QUIC-integriert, aber CPU-intensiver |
| **Kontextwechsel** | Nahe Null durch direkte Referenzierung | Höher wegen Serialisierung und Deserialisierung (Protobuf) |

➡️ **Fazit:** In stabilen Netzwerkumgebungen (z.B. Rechenzentren) ist **CORBA/TAO** in Latenz, CPU-Verbrauch und Speicherbedarf 
überlegen – insbesondere für dauerhafte Maschine-zu-Maschine-Kommunikation.

---

## Warum HTTP/3 + gRPC dennoch entstand

Die Entwicklung von HTTP/3 und gRPC wurde durch ganz andere Zielsetzungen motiviert als die von CORBA:

- **Mobiles Netz**: HTTP/3 basiert auf QUIC, um besser auf Netzwerkwechsel (z.B. WLAN → Mobilfunk) zu reagieren.
- **Browser-Kompatibilität**: Webanwendungen profitieren von HTTP/3 durch Verbindungsmultiplexing.
- **Schneller Rollout**: QUIC operiert im User Space und kann Updates schneller bereitstellen als Kernel-nahe Protokolle wie TCP.
- **API-Ökonomie**: gRPC erlaubt strukturierte APIs – für viele Diensteanbieter ein zentraler Verkaufsfaktor.

Diese Aspekte sind zwar für Web- und Cloudkontexte relevant, lösen jedoch nicht die technischen Herausforderungen effizienter 
Maschinenkommunikation. Vielmehr entstehen durch HTTP/3/gRPC neue Schichten, neue Abhängigkeiten und proprietäre Schnittstellen.

---

## Die Wahrheit über den Niedergang von CORBA/TAO

Die Überlegenheit von CORBA/TAO in technischen Belangen ist unbestritten – aber der Verlust an Sichtbarkeit war **keinesfalls eine Folge technischer Schwächen**, sondern:

- **fehlende Vendor Unterstützung**: Ähnlich zu C++ gibt es keinen Vendor, der ein aktives Marketing inklusive Lobbyarbeit betreibt.
- **Fehlendes Marketing**: CORBA wurde nie als "cool" oder "hip" positioniert – HTTP hingegen profitierte von Webboom und Google-Dominanz.
- **Mythos der Komplexität**: Die tatsächliche Komplexität von CORBA ist beherrschbar – bei gRPC hingegen werden die versteckten Aufwände oft vernachlässigt.
- **Wirtschaftliche Interessen**: Große Anbieter (Google, Amazon, Microsoft) haben weniger Interesse an offenen Standards, die keinen Vendor Lock-in ermöglichen.
- **Fehlervermeidung**: Statt HTTP als nicht geeignet für Maschinenkommunikation zu deklarieren, wurden neue, fehlerverbergende Layer darauf aufgesetzt.
- **Monetarisierbare Abhängigkeiten**: Cloudanbieter können gRPC-basierte Services besser monetarisieren – mit APIs, Gateways und Protokollabstraktionen, die CORBA nie nötig hatte.

---

## Fazit

**CORBA/TAO** bot von Beginn an ein starkes Fundament für verlässliche, schnelle und standardisierte Kommunikation zwischen Maschinen. 
Es benötigt keinen komplexen Protokoll-Stack, keine proprietären Toolchains und keine nachträglichen "Reparaturmaßnahmen".

Im Gegensatz dazu ist **HTTP/3 + gRPC** ein Versuch, die strukturellen Schwächen von HTTP-basierten Architekturen zu übertünchen. 
Dabei entstehen proprietäre Abhängigkeiten, hoher Overhead und komplexe Infrastrukturen – ohne echte Notwendigkeit in maschinennahen Szenarien.

### Technologisch betrachtet war und bleibt CORBA/TAO überlegen – der Rückzug aus dem Rampenlicht ist das Resultat wirtschaftlicher Strategien, nicht technischer Defizite.

---

## Kernaussage in einem Satz

> **CORBA/TAO ist der vergessene Schatz effizienter vernetzter Systeme, während HTTP/3/gRPC primär eine Reparatur defizitärer Webtechnologien darstellt.**

---

## Executive Summary: Warum CORBA/TAO der überlegene Ansatz bleibt

### 1. Effizienz
CORBA/TAO nutzt minimale Overheads mit direkter Binärkommunikation, stabiler TCP-Basis und dauerhaften Verbindungen – 
im Gegensatz zum komplexen QUIC/HTTP/3 Stack.

### 2. Offenheit & Standardisierung
CORBA basiert auf offenen OMG-Standards, ist plattformübergreifend und unabhängig von Herstellern. gRPC ist ein proprietäres Google-Ökosystem.

### 3. Technologische Klarheit
Die oft zitierte Komplexität von CORBA ist strukturiert, wohldefiniert und dokumentiert. 
Die "vereinfachten" Tools in gRPC führen in Wahrheit zu proprietärer Komplexität (Protobuf, Reflection, Balancer).

### 4. Realzeittauglichkeit
Mit Implementierungen wie TAO ist CORBA für deterministische Echtzeitsysteme optimiert. HTTP/3/gRPC können das nicht garantieren.

### 5. Marktmechanismus statt Technologiewahl
Die Ablösung von CORBA war das Ergebnis wirtschaftlicher Interessen, nicht technischer Überlegenheit.

---

**Für alle, die nachhaltige, effiziente und offene Maschinenkommunikation suchen, ist CORBA/TAO heute relevanter denn je.**

