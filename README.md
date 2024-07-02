
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

    hmm...ako kaze da ne zna sta je komanda g++, probaj da instaliras g++ sa sudo apt-get install build-essential a onda ponovo g++ ...to prethodno

    // kad se kompajlira, pokrenes ga
    ./hTT    obrati paznju na ./

Dalje sve pise :-)

Unesi hex npr ovako  5a 4f 4b 49

Ili napravi tekstualni fajl sa pojedinim redovima kao sto bi ukucavao, pa ga pozovi ovako

./hTT < fajl.txt    


sto znaci, citaj ulaz iz tekstualnog fajla

    
Za sada ne vraca nikakav odgovor, napravicemo i to.

Konektuje se samo na /dev/ttyS0, to mozes lagano da promenis na pocetku TT.cpp fajla.

Srecno!


https://www.sciencebuddies.org/cdn/references/ascii-table.png

![ascii-table](https://github.com/nbgsmk/hTT/assets/60575976/3c2747f2-7f34-4800-bd51-9817a3f7b76e)



--------- podsetnik --------

https://stackoverflow.com/questions/76299250/how-to-use-getline-in-a-separate-thread-in-c
https://stackoverflow.com/a/76334993

https://www.onlinegdb.com/E9Gwgje7T

https://stackoverflow.com/a/42264216



