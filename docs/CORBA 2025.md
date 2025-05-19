# Umfassende Übersicht über CORBA-Implementierungen im Jahr 2025

Diese detaillierte Übersicht wurde erstellt, um die aktuelle Landschaft der verfügbaren CORBA-Implementierungen im Jahr 2025 zu beleuchten. 
Dabei habe ich sowohl ChatGPT als auch Gemini mit Deep Research verwendet. So baut diese Übersicht auf einer ersten Zusammenstellung von ChatGPT 
auf und erweitert diese durch tiefergehende Recherche von Gemini, um eine präzisere und umfassendere Darstellung zu bieten. Dabei werden sowohl 
Open-Source- als auch kommerzielle Produkte berücksichtigt und zusätzliche, in der ursprünglichen Übersicht fehlende Implementierungen analysiert.

## Verfeinerte Übersicht über Open-Source-CORBA-Implementierungen

Die Welt der Open-Source-CORBA-Implementierungen bietet eine Reihe von Optionen, die sich in Bezug auf Aktivität, unterstützte Sprachen und 
spezifische Funktionen unterscheiden.

* **TAO (The ACE ORB):** Diese mächtige C++-ORB ist bekannt für ihre Verwendung des POA (Portable Object Adapter) und die Unterstützung von 
  RT-CORBA (Real-Time CORBA). Die Entwicklung von TAO ist weiterhin aktiv, wie die Veröffentlichung von ACE 8.0.2 und TAO 4.0.2 am 6. Dezember 2024 
  zeigt. Ältere Versionen wie ACE 7.1.0 und TAO 3.1.0 wurden am 3. März 2023 veröffentlicht. TAO erfordert nun C++17 oder neuer. Die fortlaufende 
  Entwicklung und die Unterstützung für Echtzeitanwendungen machen TAO zu einer relevanten Wahl für anspruchsvolle C++-Projekte.

* **TAOX11:** Als moderne C++-Implementierung unterstützt TAOX11 C++11/17 und basiert auf TAO. Die letzte Version 2.5.0 wurde am 8. November 2023 
  veröffentlicht. TAOX11 ist konform mit dem IDL-zu-C++11-Mapping v1.4 und CORBA v3.3 und bietet volle Unterstützung für das IDL-Typsystem, lokale 
  und Remote-CORBA-Schnittstellen sowie POA. Experimentelle Unterstützung für IDL 4.2-Erweiterungen ist ebenfalls vorhanden. Die aktive Entwicklung 
  und die Unterstützung moderner C++-Standards positionieren TAOX11 als eine technisch fortschrittliche Lösung für C++-basierte CORBA-Systeme.

* **omniORB:** Diese bekannte und performante ORB unterstützt C++ und Python (via omniORBpy). Die neuesten Versionen 4.3.3 für omniORB und omniORBpy 
  wurden am 31. März 2025 veröffentlicht. Zuvor gab es Versionen 4.3.2 und 4.2.6, die am 15. Januar 2024 erschienen sind. omniORB ist weitgehend 
  CORBA 2.6-konform und unter der GNU Lesser General Public License verfügbar. Die kontinuierlichen Releases unterstreichen die aktive Weiterentwicklung 
  und die Relevanz von omniORB für Projekte, die C++- oder Python-Integration benötigen.

* **Eclipse ORB (früher GlassFish CORBA):** Als Teil von Eclipse GlassFish und der Jakarta EE-Plattform wird Eclipse ORB aktiv weiterentwickelt. 
  Die neueste Hauptversion 5.0.0 wurde am 5. März 2025 veröffentlicht und ist mit Jakarta EE kompatibel. Diese Version beinhaltet signifikante 
  API-Änderungen und unterstützt nun JPMS, wodurch ältere Java-Versionen nicht mehr unterstützt werden. Eclipse ORB bietet Standard-CORBA-Funktionen 
  für Java-Anwendungen wie POA und IIOP und wird in Jakarta EE für die EJB RMI/IIOP-Kommunikation verwendet. Die Integration in die Jakarta EE-Plattform 
  sichert die fortlaufende Pflege und Relevanz für Java-basierte CORBA-Anwendungen.

* **JacORB:** Obwohl JacORB als bekannt und ausgereift gilt, wird es nicht mehr aktiv entwickelt. Die letzte veröffentlichte Version 3.9 stammt vom 
  31. August 2017. Trotz seiner Inaktivität ist JacORB weiterhin verfügbar und wird in einigen Kontexten möglicherweise noch eingesetzt. Es ist auch 
  auf GitHub verfügbar.

* **R2CORBA:** Diese Implementierung bietet Ruby-CORBA-Bindungen und ist interoperabel mit TAO und JacORB. Die letzte Version 1.7.1 wurde am 
  1. November 2023 veröffentlicht. R2CORBA zielt darauf ab, CORBA-Funktionalität durch eine Ruby-Erweiterung bereitzustellen, die die DII- und 
  DSI-Schnittstellen bestehender CORBA-Implementierungen wie TAO und JacORB nutzt. Dies ermöglicht die Integration von CORBA-Funktionen in 
  Ruby-Anwendungen.

* **MICO (MICO Is COrba):** Dieses Projekt zielt darauf ab, eine frei verfügbare und vollständig konforme Implementierung des CORBA-Standards 
  bereitzustellen. Die letzte Aktualisierung auf SourceForge erfolgte am 5. Juni 2013. MICO unterstützt C++ und bietet Funktionen wie IDL-zu-C++-Mapping, 
  DII, DSI, POA und SSL-Unterstützung. Obwohl die letzte Aktualisierung einige Zeit zurückliegt, wird MICO weiterhin in einigen Produktionsumgebungen 
  eingesetzt.

Die Landschaft der Open-Source-CORBA-Implementierungen zeigt eine Mischung aus aktiv gepflegten Projekten wie TAO, TAOX11, omniORB, Eclipse ORB und 
R2CORBA sowie älteren Projekten, deren Entwicklung wahrscheinlich eingestellt wurde. Dies deutet darauf hin, dass CORBA zwar als ältere Technologie 
betrachtet werden mag, aber dennoch einige lebendige Open-Source-Optionen für Entwickler bietet.

## Erweiterte Übersicht über kommerzielle CORBA-Implementierungen

Der Markt für kommerzielle CORBA-Implementierungen wird von etablierten Anbietern dominiert, die umfassende Unterstützung und Funktionen für 
Unternehmenskunden bieten.

* **Rocket Orbix (ehemals IONA Orbix, Progress Orbix, Micro Focus Orbix, OpenText Orbix):** Orbix ist eine der weltweit am häufigsten eingesetzten ORBs, 
  insbesondere in Branchen wie Telekommunikation und Bankwesen. Die 6.x-Serie wird weiterhin aktiv unterstützt, und die Dokumentation wurde zuletzt 
  am 28. April 2025 aktualisiert. Orbix bietet hohe Verfügbarkeit, Replikation, Skalierbarkeit und flexible Bereitstellungsoptionen. Es ist sowohl für 
  POA- als auch für BOA-basierte ORBs verfügbar und bietet auch eine Version für Mainframe-Umgebungen (z/OS). Die lange Geschichte und die fortlaufende 
  Unterstützung durch Rocket Software machen Orbix zu einer stabilen Wahl für Unternehmen mit anspruchsvollen Anforderungen.

* **Rocket VisiBroker (ehemals Visigenic VisiBroker, Borland VisiBroker, Micro Focus VisiBroker):** VisiBroker liefert Enterprise-Performance mit 
  Echtzeit-CORBA-Funktionen und umfangreichen Tools. Die Dokumentation für Version 8.5.9 wurde zuletzt am 15. Mai 2025 aktualisiert. VisiBroker unterstützt 
  C++, Java und.NET und kann sowohl BOA- als auch POA-basierte ORBs ausführen. Funktionen wie Real-Time CORBA (RT-CORBA) und eine einfache Einrichtung 
  machen VisiBroker zu einer attraktiven Option für verschiedene Anwendungsfälle.

* **ORBexpress:** Diese kommerzielle ORB von Objective Interface Systems (OIS) ist echtzeitfähig und für Embedded-Systeme optimiert. ORBexpress 
  unterstützt die Sprachen Ada, C++, Java und C#. Während ChatGPT eine Version 4.x aus dem Jahr 2024 erwähnte, deuten Nachrichtenartikel auf Releases 
  im Jahr 2025 hin. ORBexpress zeichnet sich durch hohe Leistung und einen geringen Speicherbedarf aus und eignet sich daher gut für 
  ressourcenbeschränkte Umgebungen.

* **Orbacus (ehemals IONA Orbacus, Micro Focus Orbacus):** Orbacus, jetzt von Micro Focus angeboten, ist eine schlanke ORB, die auch für Embedded-Systeme 
  geeignet ist und kommerziellen Support bietet. Die letzte in den Snippets gefundene Version ist 4.3.6, mit Dokumentationsupdates bis ins Jahr 2022. 
  Orbacus ist als Sourcecode verfügbar, was eine hohe Kontrolle über das CORBA-System ermöglicht. Es wurde in verschiedenen kritischen Systemen in 
  Branchen wie Telekommunikation und Finanzen eingesetzt.

* **Tuxedo CORBA:** Als Bestandteil der Oracle-Tuxedo-Middleware wird Tuxedo CORBA aktiv gepflegt. Die neueste Version ist 22c (22.1.0.0.0), veröffentlicht 
  im Oktober 2022. Tuxedo CORBA unterstützt eine breite Palette von Sprachen, darunter C++, C, COBOL, Java, Python, Ruby und PHP. Es ist primär für 
  existierende Oracle-Installationen und als Teil eines umfassenden Transaction-Processing-Systems relevant.

* **OpenFusion (ehemals PrismTech OpenFusion, Micro Focus OpenFusion, Rocket OpenFusion):** OpenFusion, jetzt unter Rocket Software, bietet eine 
  umfassende Suite von CORBA-Produkten und -Diensten, einschließlich Implementierungen der wichtigsten OMG CORBA Services. Es unterstützt Java, 
  C++ und Ada und ist für große Unternehmenssysteme konzipiert, die hohe Leistung, Skalierbarkeit und Verfügbarkeit erfordern. OpenFusion umfasst 
  auch Echtzeitversionen (RTOrb Java und RTOrb Ada) für anspruchsvolle Anwendungen.

Der kommerzielle CORBA-Markt wird von etablierten Anbietern wie Rocket Software und Oracle dominiert, deren Angebote auf spezifische Bedürfnisse 
wie Echtzeitsysteme, hochverfügbare Unternehmensanwendungen und die Integration mit bestimmten Middleware-Plattformen zugeschnitten sind. 
Die Historie von Akquisitionen in diesem Bereich deutet auf eine Konsolidierung des Marktes hin.

## Detaillierte Analyse spezifischer Implementierungen

Um die CORBA-Landschaft weiter zu erhellen, ist eine detailliertere Betrachtung bestimmter Implementierungen hilfreich.

### OpenText

OpenText war historisch durch die Übernahme des Geschäftsbereichs Application Modernization and Connectivity (AMC) von Micro Focus im Jahr 2023 in 
den CORBA-Markt involviert. Dieser Geschäftsbereich umfasste auch Orbix. Im Jahr 2024 erwarb jedoch Rocket Software den AMC-Geschäftsbereich von 
OpenText, einschließlich Orbix Mainframe. Somit war die Beteiligung von OpenText an Orbix eher übergangsweise. Die Hauptanbieter, die mit Orbix 
in Verbindung gebracht werden, sind nun Rocket Software sowie zuvor Micro Focus und IONA Technologies.

### IBM und zOS/CICS

IBM bietet CORBA-Lösungen im Kontext seines Mainframe-Betriebssystems z/OS und des CICS (Customer Information Control System) Transaction Servers an. 
CICS unterstützt CORBA-Server und ermöglicht es Mainframe-Anwendungen, mit verteilten CORBA-Objekten zu interagieren. Eine Schlüsseltechnologie in 
diesem Zusammenhang ist das IBM System Object Model (SOM). SOM ist eine Technologie zur Objektaktivierung, die entwickelt wurde, um die Verwendung 
von Objektklassenbibliotheken zu erleichtern. Es ist mit der CORBA-Spezifikation der OMG konform und unterstützt IDL sowie CORBA-Datentypen. 
DSOM (Distributed SOM) ist ein CORBA-konformer Object Request Broker, der die Kommunikation über Prozesse und Netzwerke hinweg ermöglicht. 
SOM ist sprachneutral, sodass in verschiedenen Sprachen implementierte Objekte miteinander interagieren können. Es wurde als "Softwarebus" konzipiert, 
der Betriebssysteme und Compiler verbindet. IBM App Connect Enterprise unterstützt CORBA ebenfalls über den Object Request Integration Node (ORB). 
IBM hat eine lange Geschichte der CORBA-Unterstützung, insbesondere in seiner Mainframe-Umgebung. SOM spielte eine entscheidende Rolle bei der 
Ermöglichung objektorientierter Programmierung und CORBA-Konformität auf IBM-Plattformen. Obwohl SOM als ältere Technologie betrachtet werden mag, 
sind sein Einfluss und die fortlaufende Unterstützung von CORBA in zOS/CICS für Organisationen mit Mainframe-Infrastruktur weiterhin wichtig.

### MinCor.NET

Der Benutzer erwähnte "MinCor.NET", aber die Recherche ergab hauptsächlich Informationen über DotNetOrb, eine CORBA-Standardbibliothek für C#. 
DotNetOrb zielt darauf ab, eine nahtlose Integration mit CORBA in.NET-Anwendungen zu ermöglichen und die Kommunikation zwischen verteilten Objekten 
unabhängig von den implementierenden Programmiersprachen zu erleichtern. Es basiert teilweise auf JacORB und enthält einen von Grund auf neu 
entwickelten C#-IDL-Compiler. Zu den wichtigsten Funktionen gehören die Unterstützung verschiedener Codesets, SSL-Sicherheit, POA-Konfiguration und 
die Integration mit einem Implementation Repository. Es gibt auch andere.NET-CORBA-Implementierungen wie IIOP.NET (obwohl wahrscheinlich inaktiv) 
und den Orbacus.NET Connector. Middsols MinCor.NET wurde auch zur Verbindung von.NET Compact Framework-Mobilgeräten mit Java-Backends verwendet. 
Während der Benutzer möglicherweise eine leichte Namensabweichung aufwies oder sich auf ältere Lösungen bezog, scheint DotNetOrb die aktuell relevante 
Open-Source-CORBA-Implementierung für.NET zu sein. Die Existenz mehrerer.NET-CORBA-Bridges unterstreicht den Bedarf an Interoperabilität 
zwischen.NET und anderen Plattformen.

## Überlegungen zur Auswahl einer CORBA-Implementierung

Die Auswahl der richtigen CORBA-Implementierung für ein Projekt hängt stark von den spezifischen Anforderungen ab. Es gibt keine Universallösung. 
Wichtige Faktoren, die bei der Entscheidung berücksichtigt werden sollten, sind:

* **Sprachunterstützung:** Stellen Sie sicher, dass die Implementierung die für die Client- und Serverkomponenten erforderlichen 
  Programmiersprachen unterstützt.
* **Plattformkompatibilität:** Überprüfen Sie die Kompatibilität mit den Zielbetriebssystemen und Hardwarearchitekturen.
* **Lizenzierung:** Verstehen Sie die Lizenzbedingungen (Open-Source vs. kommerziell) und deren Auswirkungen auf Nutzung und Weitergabe.
* **Kommerzieller Support:** Bewerten Sie die Verfügbarkeit und Qualität des kommerziellen Supports, insbesondere für unternehmenskritische Anwendungen.
* **Community-Aktivität:** Bei Open-Source-Projekten sollten Sie die Aktivität und Reaktionsfähigkeit der Community in Bezug auf Fehlerbehebungen 
  und Support bewerten.
* **Funktionsanforderungen:** Ermitteln Sie, ob die Implementierung die erforderlichen Funktionen bietet, wie z. B. Echtzeiterweiterungen, 
  Sicherheitsfunktionen (SSL/TLS) und Unterstützung relevanter CORBA-Dienste.
* **Leistung:** Berücksichtigen Sie die Leistungsmerkmale der ORB, insbesondere für Anwendungen mit hohem Durchsatz oder geringer Latenz.
* **Reife und Stabilität:** Bewerten Sie die Reife und Stabilität der Implementierung anhand ihrer Release-Historie und des Einsatzes in 
  Produktionsumgebungen.

## Schlussfolgerung

Die Analyse der verfügbaren CORBA-Implementierungen im Jahr 2025 zeigt, dass sowohl Open-Source- als auch kommerzielle Optionen weiterhin existieren 
und gepflegt werden. Während CORBA oft als ältere Technologie betrachtet wird, spielt sie immer noch eine wichtige Rolle in vielen bestehenden Systemen, 
insbesondere in Unternehmensumgebungen und spezialisierten Bereichen wie Echtzeitsystemen und der Mainframe-Integration. Die Wahl der geeigneten 
CORBA-Implementierung erfordert eine sorgfältige Abwägung der spezifischen Projektanforderungen in Bezug auf Sprachunterstützung, Plattformkompatibilität, 
Lizenzierung, Support und benötigte Funktionen. Die fortlaufende Entwicklung und Unterstützung wichtiger Implementierungen wie TAO, TAOX11, omniORB und 
Eclipse ORB sowie die kommerziellen Angebote von Anbietern wie Rocket Software und Oracle deuten darauf hin, dass CORBA auch in Zukunft in bestimmten 
Kontexten relevant bleiben wird.