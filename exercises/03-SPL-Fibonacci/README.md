# Fibonacci-Sequenz

Im 13. Jahrhundert hat der italienische Mathematiker Leonardo da Pisa (Fibonacci) eine
Möglichkeit gefunden, um das Wachstum einer Population von Hasen als mathematische
Sequenz auszudrücken. Die ersten beiden Terme in der Fibonaccireihe Fib(0) und Fib(1)
sind 0 und 1, und jeder folgende Term ist die Summe der beiden vorherigen Terme. Demnach lauten die ersten Terme der Fibonaccireihe wie folgt:

    Fib(0) = 0
    Fib(1) = 1
    Fib(2) = 1 (0 + 1)
    Fib(3) = 2 (1 + 1)
    Fib(4) = 3 (1 + 2)
    Fib(5) = 5 (2 + 3)

Schreiben Sie ein Programm, das die Terme der Fibonaccireihe, beginnend bei Fib(0)
ausgibt solange die Terme kleiner als 10000 sind.

Die Ausgabe sollte also wie folgt sein:

    0
    1
    1
    2
    3
    5
    8
    13
    21
    34
    ...

Dieses Programm läuft weiter, solange der Wert des Terms kleiner dem Maximalwert ist. Sie benötigen dementsprechend eine while-Schleife, die wie folgt ausssehen könnte:

     while ( term < MAX_VALUE )

Hinweis: Legen Sie den Maximalwert mit #define als Konstante fest.

# SPL

Documentation to SPL can be found here:

https://cs50.github.io/spl/

