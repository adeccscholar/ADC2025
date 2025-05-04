# 🧠 C++ & CORBA – Verteilte Systeme für Skalierung, Effizienz und Echtzeitfähigkeit
**Beispiel-Sourcen zum ADC 2025 Vortrag**

Willkommen im offiziellen Repository zum Vortrag auf der **Advanced Developers Conference (ADC) 2025**:

> **"C++ & CORBA – Verteilte Systeme für Skalierung, Effizienz und Echtzeitfähigkeit"**

Dieses Repository enthält die begleitenden Quelltexte zur Präsentation. Ziel ist es, die Leistungsfähigkeit von **CORBA** im 
Kontext moderner C++-Entwicklung zu demonstrieren und aufzuzeigen, warum CORBA auch heute noch richtungsweisend 
für **verteilte Systeme** ist – insbesondere in Bereichen mit Anforderungen an **Skalierbarkeit**, **Effizienz** 
und **Echtzeitfähigkeit**.

> Weitere Informationen im Artikel [Energieeffizienz in der IT](https://www.linkedin.com/pulse/energieeffizienz-der-volker-hillmann-eeiee)
mit der Betrachtung, warum Bequemlichkeit für Entwickler:innen die Umwelt unnötig belastet kann und ein Blick auf CORBA als Beispiel für 
vergessene Effizienz lohnenswert sein kann.

---

## Teil des Bildungsprojekts **adecc Scholar**

Dieses Repository ist Teil von **adecc Scholar** – einem freien Bildungsprojekt zur Förderung moderner C++-Kenntnisse.  
Das Projekt richtet sich an:

- Lernende & Studierende
- Berufstätige Entwickler:innen
- Technisch Interessierte
- Alle, die sich in **modernem C++ weiterbilden** möchten

### 📡 Plattformen rund um das Projekt:

- **GitHub**: [adeccScholar](https://github.com/adeccScholar)
- **YouTube**: Videos, Tutorials & Vorträge [adecc Scholar @ Youtube](https://www.youtube.com/@adeccScholar)
- **Twitch**: Live-Coding & Sessions [adecc Scholar @ Twitch](https://www.twitch.tv/volker_adecc)
- **Discord**: Community-Support, Lernhilfe, Diskussionen [adecc Scholar @ Twitch](https://discord.com/invite/E8tzbu2FTY) 
---

## TAO – Hintergrund, Bedeutung und Weiterentwicklung

### Allgemeiner Hintergrund

**TAO (The ACE ORB)** ist eine hochleistungsfähige CORBA-Implementierung, die aus jahrzehntelanger akademischer Forschung 
an verteilten Systemen hervorgegangen ist. Entwickelt wurde TAO unter der Leitung von **Prof. Dr. Douglas C. Schmidt**, 
einem der weltweit führenden Experten für Middleware und Echtzeitsysteme. 

Die Entwicklung begann an der **Washington University in St. Louis** und wurde später maßgeblich an 
der **Vanderbilt University** fortgeführt.

Ziel war von Anfang an die Schaffung einer **portablen, leistungsfähigen und echtzeitfähigen ORB-Plattform**, die sich 
sowohl für industrielle als auch sicherheitskritische Anwendungen eignet. TAO wurde in vielen national und international 
geförderten Forschungsprojekten verwendet und kontinuierlich verbessert. 

Die Plattform wurde in unterschiedlichsten Anwendungsbereichen produktiv eingesetzt – 
darunter **Luft- und Raumfahrt, Verteidigung, Telekommunikation, Medizin- und Automatisierungstechnik**.

Diese intensive und anwendungsorientierte Forschungsbasis, gepaart mit einer offenen Lizenz und aktiver Community, 
machen TAO zu einer der **stabilsten und am breitesten eingesetzten CORBA-Implementierungen weltweit**.

---

### ⚙️ Technische Informationen & Weiterentwicklung

**TAO** basiert auf dem **ACE (Adaptive Communication Environment)** Framework und bietet eine vollständige, hochgradig 
optimierte **CORBA-Implementierung** mit Fokus auf Echtzeitfähigkeit, Skalierbarkeit und Portabilität. Dabei wird es 
ständig weiterentwickelt, und dabei nicht nur an den neuen CORBA Standard, sondern auch aktuelles C++ angepaßt. 

Merkmale von TAO:

- Unterstützung für viele Plattformen (Linux, Windows, macOS, Echtzeitbetriebssysteme wie VxWorks oder QNX)
- Kompatibilität mit GCC, Clang, MSVC und Embarcadero C++Builder (benötigt C++17 oder höher)
- Echtzeitfähige Kommunikation durch deterministische Scheduling-Modelle
- IIOP-basierte Objektkommunikation mit hoher Effizienz
- Umfangreiche Service-Implementierungen (z.B. Naming, Event, Notification, Security)

#### 🧩 TAOX – modernes C++-Mapping

**TAOX** ist ein eigenständiges, modernes C++11/14/17-CORBA-Mapping, das **parallel zu TAO** entwickelt wird. 
Während TAO das etablierte CORBA-Backend darstellt, bietet TAOX eine **zeitgemäße und einfachere API** zur 
Interaktion mit CORBA-Diensten und zur Generierung von Client/Server-Code in modernem C++.

Ziele von TAOX:

- Vereinfachung des C++-MAPPINGS (keine Raw-Pointer, RAII, moderne Container etc.)
- Modularisierung und bessere Integration in moderne Build-Systeme
- Erleichterte Wartbarkeit und Lesbarkeit des generierten Codes
- Fokus auf Standardkonformität und Developer Experience

TAOX erweitert damit die bestehende TAO-Plattform um ein modernes Entwickler-Interface, bleibt aber kompatibel mit 
der TAO-Kernarchitektur. Diese Koexistenz ermöglicht sowohl die Pflege etablierter Systeme als auch die Entwicklung 
neuer CORBA-Anwendungen auf Basis aktueller C++-Standards.

**TAO und TAOX stehen weiterhin unter aktiver Pflege und Entwicklung** und sind insbesondere für Projekte interessant, 
die auf **offene, performante und langfristig wartbare Middleware-Lösungen** angewiesen sind.

> Hinweise zur Installation und Einbindung von TAO findest direkt bei [The ACE ORB (TAO)](https://www.dre.vanderbilt.edu/TAO/).
Das aktuelle Release findet sich im [ACE/TAO Github- Repository der DOCGroup](https://github.com/DOCGroup/ACE_TAO/releases).

---

## Inhalte

- `src/`: Beispielprogramme (Client, Server, IDL-Dateien)
- `docs/`: Build-Anleitungen, technische Hintergrundinfos
- `CMakeLists.txt`: Projektkonfiguration für CMake
- `LICENSE`: GPLv3-Lizenztext

---

## Lizenz: GNU General Public License v3.0 (GPL-3.0)

Dieses Projekt ist unter der [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.html) lizenziert.

Du darfst:
- den Code frei nutzen, verändern und weiterverbreiten
- das Projekt kommerziell oder nicht-kommerziell einsetzen

Du musst:
- den Quellcode bei Weitergabe offenlegen
- abgeleitete Werke ebenfalls unter der GPL v3.0 lizenzieren
- den Lizenztext beifügen

> Eine vollständige Lizenzdatei liegt unter `LICENSE` bei.

---

## Mitmachen & Feedback

Wir freuen uns über:
- Pull Requests
- Fehlerberichte (Issues)
- Anregungen und Ideen in der Community

> Verbinde dich auf [Discord](https://discord.com/invite/E8tzbu2FTY) oder kommentiere auf YouTube/Twitch!

---

**Viel Spaß beim Lernen & Entwickeln mit C++ und CORBA!**

---

Copyright (c) 2025 adecc Systemhaus GmbH

Dieses Programm ist freie Software: Sie können es unter den Bedingungen der
GNU General Public License, Version 3, wie von der Free Software Foundation
veröffentlicht, weitergeben und/oder modifizieren.

Dieses Programm wird in der Hoffnung bereitgestellt, dass es nützlich ist,
aber **OHNE JEDE GEWÄHRLEISTUNG** – sogar ohne die implizite Gewährleistung der
MARKTGÄNGIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK. Siehe die GNU General Public License für weitere Details.

Sie sollten eine Kopie der GNU General Public License zusammen mit diesem Programm erhalten haben.
Falls nicht, siehe <https://www.gnu.org/licenses/>.

Projekt: adecc Scholar – Freies Lernmaterial für modernes C++
Organisation: adecc Systemhaus GmbH
