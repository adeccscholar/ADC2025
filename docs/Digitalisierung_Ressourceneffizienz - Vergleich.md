# Digitalisierung und Ressourceneffizienz in verteilten Systemen

## 1. Preamble und Aufgabenstellung

Die fortschreitende Digitalisierung öffentlicher und privater Dienste erfordert heute mehr denn je effiziente, 
skalierbare und sichere Lösungen für den Datenaustausch. Insbesondere Behörden und Unternehmen stehen vor der 
Herausforderung, wachsende Datenmengen zu verarbeiten, ohne den Ressourcen- und Energieverbrauch unverhältnismäßig 
in die Höhe zu treiben.

**Aufgabenstellung:**

Wir betrachten zwei in einer _is-a_-Beziehung verknüpfte Tabellen:

- **Person**: Stammdaten wie Name, Geburtstag, Anrede, Familienstand, Notizen.
- **Employees**: Beschäftigtendaten wie Gehalt, Eintrittsdatum, Jobposition, Urlaubstage.

Ziel ist es, diese Datensätze (z.B. 10.000 Datensätze) über drei verschiedene Schnittstellen zu übertragen und 
dabei folgende Aspekte zu analysieren:

1. **Typsicherheit**: Binäre CORBA-Interfaces vs. textbasierte Formate JSON/XML.
2. **Overhead und Datenvolumen**: Feldnamen, Tags, Trennzeichen, HTTP-Protokoll.
3. **Performance**: Parsing, Umwandlungstempo, Server- vs. Client-Seitiges Processing.
4. **Verteilte Businesslogik**: Nur CORBA ermöglicht serverseitige Methodenaufrufe und _Lazy Loading_.

In der Betrachtung fließen auch die Herausforderungen öffentlicher Verwaltungen mit ein: Statt echter 
Machine-to-Machine-Kommunikation dominieren Minimalkompromisse und Insellösungen.

---

## 2. Vergleich der Technologien

### 2.1 CORBA (Common Object Request Broker Architecture)
- **Übertragungsformat:** Binär (CDR – Common Data Representation).
- **Typsicherheit:** Stark typisiert dank Interface-Definitionen (IDL).
- **Verarbeitungsaufwand:**
  - _Marshalling/Unmarshalling_ ist extrem effizient – Daten liegen bereits in geeigneter Form vor.
  - Serverseitige Instanziierung ermöglicht _Lazy Loading_: Clients fordern nur die tatsächlich benötigten Felder an.
- **Overhead:** Sehr gering (nur Header + Daten) – keine Feldnamen oder Tags.
- **Erweiterte Möglichkeiten:** Integrierte Businesslogik und Methodenaufrufe direkt im ORB.

### 2.2 JSON (JavaScript Object Notation)
- **Übertragungsformat:** Textbasiert (UTF 8), human‑readable.
- **Typsicherheit:** Schwach – Typumwandlung und Validierung auf Clientseite erforderlich.
- **Verarbeitungsaufwand:**
  - Serialisierung/Deserialisierung: Text→Binär (Zahlen, Datumswerte).
  - Parsing: Analyse von Klammern, Anführungszeichen, Kommas.
- **Overhead:** Feldnamen und Strukturzeichen vergrößern Datengröße.
- **HTTP-Overhead:** Zusätzliche Headerfelder und Protokollinformationen.

### 2.3 XML (eXtensible Markup Language)
- **Übertragungsformat:** Textbasiert (UTF‑8) mit ausführlichen Start- und End-Tags.
- **Typsicherheit:** Wie JSON, jedoch können XML-Schemata Typsicherheit erzwingen.
- **Verarbeitungsaufwand:**
  - _DOM/SAX_-Parsing ist speicher- und CPU-intensiv.
  - Baumstruktur-Scans zur Variablenzuordnung.
- **Overhead:** Umfangreiche Tags und optionale Attribute (z.B. `xsi:nil`).
- **HTTP-Overhead:** Analog zu JSON.

---

## 3. Detaillierte Analyse und Vergleich

### A. Arbeitsspeicher und Datenvolumen

| Technologie | Volumen pro Datensatz | Volumen für 10.000 Datensätze |
|-------------|-----------------------|-------------------------------|
| **CORBA**   | ca. 325–350 Byte      | ca. 3,3 MB                    |
| **JSON**    | ca. 700 Byte          | ca. 7 MB                      |
| **XML**     | ca. 950 Byte          | ca. 9,5 MB                    |

> **Beispielrechnung CORBA:** 277 Byte Rohdaten (Person+Employee) + Header ≈ 350 Byte → 10.000×350Byte≈3,3 MB.

### B. Performance und Overhead

- **CORBA**: Binär → kein Parsen, minimale CPU-Last, serverseitige Businesslogik (Methodenaufrufe, Validierung).
- **JSON**: Text→Binär-Konvertierung erhöht Latenz; Client speichert und parst komplette JSON-Strings.
- **XML**: DOM-Baum erzeugen kostet Speicher; XSD-Validierung für Typsicherheit weiter zeitintensiv.

---

## 4. Beispiele für Schnittstellen

### 4.1 CORBA-IDL Definition (Vererbung)

```idl
module Company {
  interface Person {
    attribute long ID;
    attribute string Name;
    attribute string Firstname;
    attribute string Birthname;
    attribute short FormOfAddress;
    attribute short FamilyStatus;
    attribute string FamilyStatusSince;
    attribute string Birthday;
    attribute string Notes;
    attribute string FullName;
  };

  interface Employee : Person {
    attribute string PersonNumber;
    attribute double Salary;
    attribute short SalaryType;
    attribute short TaxClass;
    attribute string StartOfJob;
    attribute string EndOfJob;
    attribute short ReasonDeparture;
    attribute short JobPosition;
    attribute string JobSpec;
    attribute long VacationDays;
    attribute short Department;
    attribute string TaxID;
    attribute string SocialNumber;
    attribute boolean Active;
    attribute short UnitID;
  };

  interface Persons {
    Person getPerson(in long id);
    void updatePerson(in Person p);
  };

  interface Employees {
    Employee getEmployee(in long employID);
    void updateEmployee(in Employee e);
  };
};
```

### 4.2 JSON-Datensatz

```json
{
  "meta": { "dateFormat": "YYYY-MM-DD" },
  "ID": 123,
  "Name": "Doe",
  "Firstname": "John",
  "Birthname": "Doe",
  "FormOfAddress": 1,
  "FamilyStatus": 2,
  "FamilyStatusSince": "2010-01-01",
  "Birthday": "1980-06-15",
  "Notes": "...",
  "FullName": "John Doe",
  "Employee": {
    "PersonNumber": "P123456",
    "Salary": 50000.00,
    "SalaryType": 1,
    "TaxClass": 3,
    "StartOfJob": "2005-05-01",
    "EndOfJob": "",
    "ReasonDeparture": 0,
    "JobPosition": 2,
    "JobSpec": "Software Developer",
    "VacationDays": 25,
    "Department": 4,
    "TaxID": "12345678901",
    "SocialNumber": "9876543210",
    "Active": true,
    "UnitID": 7
  }
}
```

### 4.3 XML-Datensatz

```xml
<Person dateFormat="YYYY-MM-DD" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <ID>123</ID>
  <Name>Doe</Name>
  <Firstname>John</Firstname>
  ...
  <Employee>
    <PersonNumber>P123456</PersonNumber>
    <Salary>50000.00</Salary>
    <EndOfJob xsi:nil="true"/>
    ...
  </Employee>
</Person>
```

---

## 5. Fazit

Für hochdynamische, datenintensive Systeme in Behörden und Unternehmen ist **CORBA** mit seiner binären 
Typsicherheit, dem geringen Overhead und der Unterstützung verteilter Businesslogik deutlich effizienter 
als JSON oder XML. Während einfache Web-APIs (Google, REST-Dienste) eine große Entwickler-Community 
erreichen, sind sie für interne _Machine-to-Machine_-Szenarien aufgrund von Parsing- und Validierungsaufwand 
ungeeignet. Eine nachhaltige IT-Strategie setzt auf Qualität statt Quantität, spart Energie und reduziert 
den CO₂-Fußabdruck – und genau hier kann CORBA heute mit Open-Source-Implementierungen wie TAO punkten.
