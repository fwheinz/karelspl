# Aufgabe 1 : Histogramme und Dateien: Notenhäufigkeit
Schreiben Sie ein Programm, das eine Liste von Klausurergebnissen (in Punkten) aus der Datei `examscores.txt` ausliest. Diese Datei enthält pro Zeile immer ein Klausurergebnis. Geben Sie ein Histogramm dieser Punktzahlen aus, unterteilt in die folgenden Bereiche: 0–9, 10–19, 20–29, und so weiter, bis zu einem einzigen Bereich, der nur den Wert 100 enthält.

Der folgende Code zeigt das Öffnen und zeilenweise Auslesen einer Datei:

    FILE *infile = fopen("filename", "r");
    while (1) {
      char * line = readLine ( infile ) ;
      if ( line == NULL ) {
        break ;
      }
      int num = atoi ( line );
    }
    fclose(infile);

Mit der ebenfalls dort angegebenen Funktion **atoi()** können Sie die ausgelesene Zeile in eine Ganzzahl umwandeln.

Das Ergebnis sollte in etwa wie folgt aussehen:

    00-09: ***
    10-19: ******
    20-29: ****
    30-39: 
    40-49: *****
    50-59: **
    60-69: ***********
    70-79: ******
    80-89: ***
    90-99: *****
      100: *

# SPL

Documentation to SPL can be found here:

https://cs50.github.io/spl/

