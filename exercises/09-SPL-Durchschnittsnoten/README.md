# Aufgabe Durchschnittsnoten

In dieser Aufgabe sollen Sie ein Programm erstellen, das eine Durchschnittsnote berechnen soll. Die Noten sollen Sie vom Nutzer einlesen. Bevor der Nutzer die Noten eingibt, fragt das Programm nach der Anzahl der Noten, aus denen der Durchschnitt berechnet werden soll.

Verwenden Sie zur Ablage der Noten eine dynamische Speicherstruktur. Sie müssen die Größe mithilfe dynamischer Speicherallokation festlegen, nachdem der Nutzer die Anzahl der Noten eingegeben hat. Verschwenden Sie in dieser Aufgabe keinen unnötigen Speicherplatz!

So könnte die Ausgabe des Programmes ungefähr aussehen:

    Please enter number of grades: 4
    Enter grade: 1
    Enter grade: 4
    Enter grade: 2
    Enter grade: 5
    Average grade: 3.250000

# Erweiterung

Erweitern Sie das Programm, so dass der Nutzer nach der Ausgabe der Durchschnittsnote gefragt wird, wie viele weitere Noten er eingeben will. Der reservierte Speicherbereich soll dann erweitert werden (mit **realloc()**), die zusätzlichen Noten eingegeben und die neue Durchschnittsnote (aus *allen* bisher eingegebenen Noten) errechnet werden. Dies soll in einer Schleife beliebig oft passieren (bis zur Eingabe von 0)

So könnte die Ausgabe des erweiterten Programmes dann ungefähr aussehen:

    Please enter number of grades: 2
    Enter grade: 1
    Enter grade: 4
    Average grade: 2.500000
    Please enter number of grades: 1
    Enter grade: 2
    Average grade: 2.333333
    Please enter number of grades: 2
    Enter grade: 1
    Enter grade: 5
    Average grade: 2.600000
    Please enter number of grades: 0
    Bye!

Im Beispiel wird also erst ein Speicherbereich für zwei Noten erzeugt, anschliessend um eine Note auf drei Noten erweitert und schliesslich nochmals um zwei Noten auf fünf Noten insgesamt vergrößert.

**Tipp:** Sie können sich dabei - um den Spezialfall der ersten Allokation mit *malloc()* zu vermeiden - auch zu Nutze machen, dass 

    realloc(NULL, size);

mit

    malloc(size);

identisch ist.

# SPL

Documentation to SPL can be found here:

https://cs50.github.io/spl/

