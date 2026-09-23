# Aufgabe: Ermitteln der Buchstabenhäufigkeit in einem Text

Erstellen Sie ein Programm welches eine Folge von Kleinbuchstaben bis max 1000 Zeichen in ein char-Array einliest. Ermitteln Sie für alle Kleinbuchstaben des Alphabets, wie oft der jeweilige Buchstabe in der Eingabe vorkommt. Geben Sie das Ergebnis tabellarisch aus.

Verwenden Sie dafür die Information, dass die Kleinbuchstaben in der ASCII Tabelle
in einer Reihe stehen. Erweitern Sie das Programm nun so, dass die Anzahl der gefunde-
nen Buchstaben nun grafisch über einen Balken visualisiert wird. Die Größe des Balkens
entspricht der Anzahl des Auftretens.

Eine Ausgabe des Programms für den String **"c macht spass"** könnte wie folgt aussehen:

    Enter sentence: c macht spass
    Raw results for your sentence:
    a:2
    c:2
    h:1
    m:1
    p:1
    s:3
    t:1
    histogram for your sentence:
    a:**
    c:**
    h:*
    m:*
    p:*
    s:***
    t:*

**Optionale Erweiterung:**  
Erweitern Sie Ihr Programm so, dass die Buchstabenhäufigkeit
anhand eines größeren Textes bestimmt werden kann.
Verwenden Sie dazu beispielsweise den Text von
Alice im Wunderland aus der Datei **alice13a.txt** (in data/)
und lesen diesen für die Auswertung ein.
Achtung, in diesem Fall müssen Sie für das
Histogramm relative Häufigkeiten verwenden,
um nicht zu viele Sterne zu produzieren.
Beispielsweise könnten Sie die höchste absolute
Häufigkeit auf 30 Sterne setzen und die Häufigkeiten
auf diese Weise skalieren.

# SPL

Documentation to SPL can be found here:

https://cs50.github.io/spl/

