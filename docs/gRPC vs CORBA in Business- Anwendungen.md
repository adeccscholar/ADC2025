# Argumentationspapier: Warum QUIC/HTTP/3 für klassische Businessanwendungen ungeeignet ist

## Zielsetzung

Bewertung des Einsatzes von QUIC/HTTP/3 (UDP-basiert) im Kontext klassischer transaktionsbasierter Businesssysteme, 
insbesondere bei interner Kommunikation, APIs, M2M-Systemen und ERP-nahen Anwendungen.

---

## 1. Anforderungen klassischer Businessanwendungen

| **Kriterium**               | **Beschreibung** |
|----------------------------|------------------|
| **Verbindungsmodell**      | Stabile, langlaufende TCP-Verbindungen (Pooling, Keep-Alive) |
| **Kommunikationssicherheit** | Transaktionsgarantie, deterministische Fehlerbehandlung |
| **Netzwerktopologie**      | Interne Netze / VPN / Rechenzentren, hohe Zuverlässigkeit |
| **Performance-Anforderung** | Effizienz, Vorhersehbarkeit, minimale Ressourcenlast |
| **Fehlersichtbarkeit**     | Vollständig analysierbar (Monitoring, IDS, Logging) |

➡️ **TCP (ggf. mit TLS) erfüllt alle Anforderungen stabil und seit Jahrzehnten erprobt.**

---

## 2. QUIC/UDP: Risiken und Nachteile

| **Aspekt**                      | **Problem** |
|--------------------------------|-------------|
| **Protokollkomplexität**       | QUIC repliziert TCP-Funktionalität in User Space → höhere CPU-Last, schwerer zu debuggen |
| **Verschlüsselung des Transports** | QUIC verschlüsselt selbst Header – verhindert tiefe Analyse durch Firewalls, Proxies, IDS |
| **Fehlende Trennung der Schichten** | Transport, TLS und Streams sind eng gekoppelt – keine isolierte Fehlerbehandlung |
| **Mangelnde Betriebsreife**    | Geringere Integration in Enterprise-Tools (Monitoring, APM, Security) |
| **Herstellerlastigkeit**       | Haupttreiber ist Google; viele QUIC-Implementierungen sind proprietär oder unvollständig |

➡️ Die „Vorteile“ von QUIC (0-RTT, Multiplexing, Verbindungswechsel) sind **im Businesskontext irrelevant oder sogar schädlich.**

---

## 3. Architektur- und Sicherheitsprinzipien

- **Modularität**: TCP + TLS + HTTP sind getrennt, auditierbar und isoliert analysierbar.
- **Unabhängige Standardisierung**: Verbreitet, interoperabel, unabhängig überprüfbar.
- **Forensik & Compliance**: Kommunikation auf allen Ebenen nachvollziehbar.
- **Betrieb & Integration**: Kompatibel mit Firewalls, Monitoring, SIEM, IDS.

➡️ QUIC untergräbt diese Prinzipien durch **Kopplung, Verschlüsselung und mangelnde Transparenz.**

---

## Vergleich: Latenz und Performanz (im Businesskontext)

| **Kriterium**             | **TCP (klassisch)** | **QUIC/HTTP/3 (UDP-basiert)** |
|--------------------------|---------------------|-------------------------------|
| **Verbindungsaufbau**    | 1x TCP-Handshake + TLS (optimiert mit TLS 1.3) | 0-RTT möglich, aber selten sinnvoll im Backend |
| **Stabilität**           | Hoch, bewährt, fehlerrobust | Weniger ausgereift, komplexeres Handling |
| **CPU-/Speicherbedarf**  | Niedrig, Kernel-optimiert | Höher, User-Space-Handling notwendig |
| **Monitoringfähigkeit**  | Vollständig (Netzwerk, TLS, Applikation) | Eingeschränkt durch Transportverschlüsselung |
| **Jitter / Paketverlust**| Robust durch TCP-Stack | Weniger kontrolliert, komplexe Recovery-Logik |
| **Relevanz im Businessumfeld** | Sehr hoch | Gering bis negativ |

---

## QUIC/HTTP/3 in diesem Umfeld

- Bringt **keinen messbaren Nutzen**
- Erhöht **Komplexität, Abhängigkeiten und Fehlermöglichkeiten**
- Schränkt **Sichtbarkeit und Steuerbarkeit** der Kommunikation ein
- Untergräbt **bewährte Betriebsmodelle** in sicherheitskritischen Infrastrukturen

---

## Empfehlung

### Keine Einführung von QUIC / HTTP/3 als Transportprotokoll für klassische Businesssysteme.

**Begründung:**

- Kein technischer oder betrieblicher Mehrwert  
- Erhöhte Komplexität und Betriebskosten  
- Verlust sicherheitskritischer Analyse- und Steuerungsmöglichkeiten  
- Kein Reifegrad für unternehmenskritische Kommunikation  
