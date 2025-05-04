# Voraussetzungen

- TAO ORB: Installiert und konfiguriert.
- IDL-Datei: Die Organization.idl (wie im vorherigen Schritt definiert) muss vorhanden sein.
- IDL Kompilierung: Führe den TAO IDL-Compiler aus:
  ```Bash
  tao_idl Organization.idl
  ```
  > Dies generiert u.a. OrganizationC.h, OrganizationC.cpp, OrganizationS.h, OrganizationS.cpp.
- Naming Service: Ein Naming Service muss laufen, damit Client und Server das Company-Objekt finden/registrieren können. 

- tao_idl in CMake

---

# Lebensdauer der Sequenz (EmployeeSeq_var) auf dem Client:

1. Rückgabe von GetEmployees(): Die Methode GetEmployees() auf dem Server erstellt eine Sequenz (Organization::EmployeeSeq*), 
   füllt sie mit Objektreferenzen (Organization::Employee_ptr) auf die (auf dem Server aktivierten) Employee-Servants und gibt 
   einen Pointer auf diese Sequenz zurück. Das Ownership dieses Pointers geht an den Client-Stub über.

2. Speicherung in _var: Auf dem Client speicherst du diesen Pointer typischerweise in einer Organization::EmployeeSeq_var. 
   Diese _var-Variable übernimmt das Ownership.

3. Gültigkeitsbereich (Scope): Die EmployeeSeq_var (und damit die Sequenzstruktur im Speicher des Clients) existiert so lange, 
   wie die Variable im Gültigkeitsbereich ist.

4. Ende des Scopes: Wenn die EmployeeSeq_var den Gültigkeitsbereich verlässt (z.B. am Ende einer Funktion oder eines {...}-Blocks), 
   passiert Folgendes:
   - Der Destruktor der EmployeeSeq_var wird aufgerufen.
   - Der Destruktor gibt den Speicher frei, der für die Sequenzstruktur selbst auf dem Client belegt wurde.
   - **Wichtig**: Der Destruktor iteriert auch durch alle Organization::Employee_ptr-Referenzen, die in der Sequenz gespeichert sind, 
     und ruft für jede einzelne Referenz CORBA::release() auf.
	 
## Auswirkungen auf den Server (bei CORBA::release()):

1. Benachrichtigung: Für jede Employee_ptr-Referenz, für die der Client CORBA::release() aufruft (weil die EmployeeSeq_var zerstört wird), 
   sendet der Client-ORB (implizit) eine Nachricht an den Server-ORB.

2. Referenzzählung: Der Server-ORB leitet diese Information an den POA weiter, der den entsprechenden Employee_i-Servant verwaltet. 
   Der POA dekrementiert den Referenzzähler für diesen spezifischen Servant.
   
3. Entscheidung über Deaktivierung/Zerstörung: Nun kommt die POA Policy ins Spiel, insbesondere die LifespanPolicy:
   - TRANSIENT Lifespan Policy: Wenn der Employee_i-Servant mit einem POA aktiviert wurde, der die TRANSIENT-Policy hat, 
     und der Referenzzähler dieses Servants auf Null fällt (d.h., der Client mit der Sequenz war der letzte, der eine Referenz hielt), 
	 dann wird der POA den Servant automatisch deaktivieren und anschließend typischerweise den Speicher des Servant-Objekts freigeben 
	 (delete servant;). Das ist das Verhalten, das du wünschst.
   - PERSISTENT Lifespan Policy: Wenn der POA die PERSISTENT-Policy hat, passiert bei einem Referenzzähler von Null nichts automatisch. 
     Der Servant bleibt aktiv, bis er explizit vom Server-Code deaktiviert wird (z.B. über poa->deactivate_object()) oder der 
	 Server herunterfährt.
	 
**Bedingung:** Der Server muss die Employee_i-Servants, deren Referenzen er in die Sequenz packt, mit einem POA aktivieren, 
	der die TRANSIENT Lifespan Policy verwendet.
	
- Die EmployeeSeq_var auf dem Client verwaltet die Lebensdauer der Sequenzstruktur und löst CORBA::release() für alle enthaltenen 
  Referenzen aus, wenn sie zerstört wird.
- Ob die zugehörigen Servants auf dem Server dann auch zerstört werden, hängt ausschließlich von der LifespanPolicy des POAs ab, 
  mit dem sie auf dem Server aktiviert wurden.
- Mit der TRANSIENT-Policy erreichst du genau das gewünschte Verhalten: Die Server-Objekte leben nur so lange, wie mindestens ein 
  Client eine Referenz darauf hält.
  
## Methode um Daten direkt abzurufen

### Erklärung der Methode:

1. **Datenbankabfrage:**
Die Methode sucht nach dem Mitarbeiter mit der angegebenen personId in der internen employee_database_. Diese employee_database_ ist eine Map (oder eine ähnliche Datenstruktur), die Mitarbeiterinformationen anhand der personId speichert.

2. **Fehlerbehandlung:**
Falls der Mitarbeiter mit der gegebenen ID nicht gefunden wird, wird eine benutzerdefinierte Ausnahme EmployeeNotFound geworfen. Diese Exception enthält die requestedId, die die ID des nicht gefundenen Mitarbeiters darstellt.

3. **Datenextraktion:**
Wenn der Mitarbeiter gefunden wird, wird das zugehörige EmployeeData aus der Datenbank extrahiert.

4. **Rückgabeobjekt erstellen:**
Ein neues Organization::EmployeeData-Objekt wird erstellt und mit den extrahierten Werten aus der internen Datenbank gefüllt. Diese Werte sind die grundlegenden Informationen des Mitarbeiters wie Name, Gehalt, Geburtsdatum usw.

5. **Rückgabe an den Client:**
Das employee_data-Objekt wird zurückgegeben, sodass es an den Client übertragen wird. CORBA sorgt dafür, dass dieses Objekt korrekt serialisiert und an den Client weitergeleitet wird.

### Wichtige Punkte:

- **Speicherverwaltung:** Die Methode erstellt ein neues Organization::EmployeeData-Objekt mit new, das an den Client zurückgegeben wird. Dies bedeutet, dass die Speicherverwaltung des Objekts an CORBA und den Client übergeben wird. Es ist daher wichtig, dass der Client dieses Objekt später freigibt, wenn es nicht mehr benötigt wird.

- **Fehlerbehandlung:** Falls der Mitarbeiter nicht gefunden wird, wird eine benutzerdefinierte Ausnahme (EmployeeNotFound) geworfen. Diese Exception kann vom Client abgefangen und behandelt werden.

- **Rückgabe an den Client:** CORBA sorgt dafür, dass das zurückgegebene Organization::EmployeeData-Objekt korrekt an den Client übertragen wird. Da EmployeeData eine einfache Struktur ohne komplexe Ressourcenverwaltung ist, kann sie direkt übertragen werden, ohne dass auf dem Server ein persistent gespeichertes Objekt erforderlich ist.
  
### Beispielaufruf

```cpp
try {
   std::unique_ptr<Organization::EmployeeData> employee_data = company_servant->getEmployeeData(12345); // Beispiel-ID
   // Verarbeiten des employee_data...
   }
catch (const Organization::EmployeeNotFound& ex) {
    std::println(std::cerr, "Mitarbeiter mit ID {} nicht gefunden!", ex.requestedId);
   }
```

