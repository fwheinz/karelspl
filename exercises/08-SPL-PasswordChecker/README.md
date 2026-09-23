# Aufgabe: Password Checker

Gute Passwörter sollten so aufgebaut sein, dass sie nicht leicht durch massives Ausprobieren ("Brute force") mithilfe von Wörterbüchern und Zahlenfolgen erraten werden können.

Schreiben Sie eine Funktion `int isStrongPassword(char word[])` welche die Zeichen
eines übergebenen Strings überprüft und nur dann 1 zurückgibt, wenn alle folgenden Be-
dingungen erfüllt sind.

Das Passwort besteht aus...
- ...mindestens 8 Zeichen,
- mindestens zwei Buchstaben,
- mindestens zwei Ziffern,
- mindestens einem Groß- und einem Kleinbuchstaben,
- mindestens einem Sonderzeichen (als Sonderzeichen gelten alle Zeichen außer Zah-
len und Buchstaben).

Die folgenden Zeilen zeigen die Ausgabe des Passwortcheckers:

    Passwortchecker.
    Bitte Passwort eingeben: 12345678
    Achtung - Passwort unsicher
    
    Passwortchecker.
    Bitte Passwort eingeben: hallo123
    Achtung - Passwort unsicher

    Passwortchecker.
    Bitte Passwort eingeben: Foo1234#
    OK - Passwort sicher

# SPL

Documentation to SPL can be found here:

https://cs50.github.io/spl/

