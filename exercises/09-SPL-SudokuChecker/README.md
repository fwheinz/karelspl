# Aufgabe: Number Delimiter

Die Aufgabe in einem Sudoku-Spiel besteht darin eine Matrix aus 9x9 Feldern, die bereits teilweise mit Zahlen zwischen 1 bis 9 befüllt ist, so zu ergänzen, dass in jeder Zeile, jeder Spalte und in allen Teilfeldern dieser Matrix, die aus jeweils 3x3 Feldern bestehen, jede Ziffer nur einmal vorkommt.

In dieser Aufgabe sollen Sie ein vereinfachtes Problem lösen, das sich nur mit einem Teilfeld (bestehend aus 3x3 Feldern) des Sudoku-Spiels beschäftigt. Die folgenden Abbildung zeigt Beispiele für diese Teilfelder. Lösung (a) ist korrekt, da die Ziffern im Bereich 1 bis 9 liegen und nur einmal vorkommen, Lösung (b) ist inkorrekt, da bestimmte Zahlen doppelt vorkommen (hier: 1 und 8), Lösung (c) ist inkorrekt, da die Zahlen -1 und 10 nicht dem zulässigen Wertebereich entsprechen.

Beispiele ausgefüllter Sudokus finden sich in der beiliegenden main.c / main-Funktion, drei Arrays.

| 1 | 2 | 3 |
|---|---|---|
| 4 | 5 | 6 |
| 7 | 8 | 9 |

korrekt

| 1 | 1 | 3 |
|---|---|---|
| 8 | 8 | 9 |
| 4 | 5 | 6 |

inkorrekt - wiederholende Ziffern

| 10 | 2 | 3 |
|---|---|---|
| 7 | -1 | 9 |
| 4 | 5 | 6 |

inkorrekt - ungültige Ziffern

Implementieren Sie die folgende Funktion:
```
int checkSudokuSquare(int sudokuSquare[][3])
```
Diese Methode erhält als Parameter ein zwei-dimensionales Array aus Ganzzahlen, mit drei Spalten und drei Zeilen, das bereits mit Ziffern aus einem Sudoku-Lösungsversuch befüllt ist. Diese Methode gibt 1 zurück, wenn das übergebene Array jede Ziffer zwischen 1 und 9 genau einmal enthält. Die Methode gibt 0 zurück, wenn die Ziffern außerhalb dieses Be- reichs liegen oder wenn das Array Duplikate (d.h. identische Ziffern) enthält.

# SPL

Documentation to SPL can be found here:

https://cs50.github.io/spl/

