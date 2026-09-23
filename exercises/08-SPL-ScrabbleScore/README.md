# Aufgabe 1 : Scrabble Score

In Scrabble ist jedem Buchstaben eine Punktzahl zugeordnet, die von seiner Häufigkeit in
der entsprechenden Sprache abhängt.  
Für die englische Version gelten die folgenden Punktzahlen:

    Punkte Buchstaben
     1      A, E, I, L, N, O, R, S, T, U
     2      D, G
     3      B, C, M, P
     4      F, H, V, W, Y
     5      K
     8      J, X
    10      Q, Z
    
Das Wort FARM ist folglich 9 Punkte wert: 4 für das F, jeweils einen für A und R und 3 für das M.
Schreiben Sie eine Funktion int getWordScore(char word[]) welche die Punkte für ein
übergebenes Wort zurückgibt.
Um zu überprüfen, wie viele Punkte ein einzelner char bringt können Sie die einzelnen
Buchstaben in jeweils einem einzelnen char Arrays pro Punktegruppe abspeichern (z.B.:
char onePointChars[] = "AEILNORSTU";). Jetzt können Sie eine neue Funktion erstellen,
die prüft, ob ein bestimmter Buchstabe darin vorkommt und die entsprechende Punktzahl
zurückgibt.
In einer ersten Version des Programms reicht es, wenn Sie die Punktzahl für Wörter aus
Großbuchstaben errechnen.
Ihr Programm soll am Ende folgende Ausgabe produzieren können:

    Enter word: FARM
    Scrabble score fuer FARM: 9

    Enter word: JAVA
    Scrabble score fuer JAVA: 14
    
    Enter word: REGENSBURG
    Scrabble score fuer REGENSBURG: 14

    Enter word: XYLOPHONE
    Scrabble score fuer XYLOPHONE: 24

# SPL

Documentation to SPL can be found here:

https://cs50.github.io/spl/

