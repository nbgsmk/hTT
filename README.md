
Pozdrav!

    ssh -> zero.local
    git clone https://github.com/nbgsmk/hTT.git
    
    // repo se downloaduje na rpi

    // udjes u folder gde je source
    cd hTT/src
    pwd
    // treba da kaze npr /home/pi/hTT/src

    g++ *.cpp -o hTT
    // i sacekas malo
    // dobije se izvrsni fajl hTT

    // pokrenes ga
    ./hTT    obrati paznju na ./

Dalje sve pise :-)

Unesi hex npr ovako  5a 4f 4b 49

Ili napravi tekstualni fajl sa pojedinim redovima kao sto bi ukucavao, pa ga pozovi ovako

./hTT < fajl.txt    // sto znaci, citaj ulaz iz tekstualnog fajla

    
Za sada ne vraca nikakav odgovor, napravicemo i to.

Konektuje se samo na /dev/ttyS0, to mozes lagano da promenis na pocetku TT.cpp fajla.

Srecno!



