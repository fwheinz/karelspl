# Hailstone Puzzle

Douglas Hofstadters Pulitzer-Preis prämiertes Buch Gödel, Escher, Bach beinhaltet viele interessante mathematische Rätsel, von denen viele durch Computer-Programme gelöst werden können. In Kapitel 12 beschreibt Hofstadter ein Problem, welches gut zum aktuellen Stoff passt. Das Problem kann folgendermaßen beschrieben werden:

Man nehme eine positive Ganzzahl und nenne sie n.  
Falls n gerade ist, halbiere man sie.  
Ist n ungerade, multipliziere man sie mit drei und zähle eins dazu.  
Dann setze man diesen Prozess solange fort, bis n den Wert eins angenommen hat.  
Auf Seite 401 veranschaulicht Hofstadter diesen Prozess mit folgendem Beispiel, beginnend mit der Nummer 15:  

    15 ist ungerade, also nehme man 3n+1: 46.
    46 ist gerade, also halbiere man: 23.
    23 ist ungerade, also nehme man 3n+1: 70.
    70 ist gerade, also halbiere man: 35.
    35 ist ungerade, also nehme man 3n+1: 106.
    106 ist gerade, also halbiere man: 53.
    53 ist ungerade, also nehme man 3n+1: 160.
    160 ist gerade, also halbiere man: 80.
    80 ist gerade, also halbiere man: 40.
    40 ist gerade, also halbiere man: 20.
    20 ist gerade, also halbiere man: 10.
    10 ist gerade, also halbiere man: 5.
    5 ist ungerade, also nehme man 3n+1: 16.
    16 ist gerade, also halbiere man: 8.
    8 ist gerade, also halbiere man: 4.
    4 ist gerade, also halbiere man: 2.
    2 ist gerade, also halbiere man: 1.
    
Aus dem Beispiel wird ersichtlich, dass die Zahl zwar steigt und sinkt, aber irgendwann - zumindest für alle Zahlen, die jemals probiert wurden - wird die Zahl 1 erreicht. In gewisser Hinsicht ist dieser Prozess ähnlich dem Entstehen eines Hagelkorns (engl. Hailstone) oder einer Schneeflocke, die im Wind immer wieder nach oben und unten fliegen, bevor sie zu Boden fallen. Aufgrund dieser Analogie wird diese Sequenz normalerweise Hailstone sequence genannt, obwohl es dafür auch viele andere Namen gibt. Schreiben Sie ein Programm, welches eine Nummer von der Konsole einliest und die dazugehörige Hailstone
sequence ausgibt, wie in Hofstadters Buch beschrieben. Anschließend wird ausgegeben, wie viele Schritte benötigt wurden die eins zu erreichen.

**Tipp:**

Mit dem folgenden Code können Sie prüfen, ob eine Zahl gerade oder ungerade ist:

    if ( num % 2 == 0) {
      // num is even
    } else {
      // num is not even
    }

# SPL

Documentation to SPL can be found here:

https://cs50.github.io/spl/

