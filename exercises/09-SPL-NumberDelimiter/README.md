# Aufgabe: Number Delimiter

Beim Schreiben von besonders großen Zahlen ist es üblich ein Tausender-Trennzeichen zu benutzen, um die Ziffernfolge in Dreiergruppen zu unterteilen. Dieses Trennzeichen kann ein Leerzeichen, ein Komma oder ein Punkt sein.

Eine Million würde zum Beispiel folgendermaßen geschrieben:

    1,000,000
    1.000.000
    1 000 000

Um die Darstellung solcher Zahlen zu vereinfachen sollen Sie nun eine Funktion

    addSeparatorToNumericString(char number[], char formattedNumber[],
                                char separator)

implementieren, die den String aus **char number[]** Zeichen für Zeichen prüft und in **char formattedNumber[]** den neuen korrekt formatierten String speichert. Achtung: Diese Aufgabe ist möglicherweise komplexer als sie auf den ersten Blick scheint. Unter Umständen müssen Sie eines der beiden Arrays umdrehen, um ein korrektes Ergebnis zu erzeugen.

**Tipp:** Eventuell kann Ihnen der Debugger gute Dienste bei der Lösung dieser Aufgabe leisten.
Ihr Programm sollte bei korrekter Implementierung folgende Ergebnisse liefern:

    Zahl eingeben: 10
    Formatierte Zahl: 10

    Zahl eingeben: 1000
    Formatierte Zahl: 1.000
    
    Zahl eingeben: 100345
    Formatierte Zahl: 100.345
    
    Zahl eingeben: 35000000
    Formatierte Zahl: 35.000.000

**Tipp 2:** Entfernen Sie am besten nach dem Einlesen mit **fgets()** das Newline, das bei **fgets()** mit ins Zeichenarray eingelesen wird.

# SPL

Documentation to SPL can be found here:

https://cs50.github.io/spl/

