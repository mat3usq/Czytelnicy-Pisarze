# Czytelnicy i pisarze
Z czytelni korzysta na okrągło pewna ilość czytelników i
pisarzy, przy czym jednocześnie może w niej znajdować się albo dowolna ilość
czytelników, albo jeden pisarz, albo nikt - nigdy inaczej. 
Problem ten ma trzy rozwiązania:
- z możliwością zagłodzenia pisarzy
- z możliwością zagłodzenia czytelników
- wykluczające zagłodzenie

# Kryteria
A) dwa programy symulujące dwa różne rozwiązania tego problemu, bez
korzystania ze zmiennych warunkowych [17p].
B) dwa programy symulujące dwa różne rozwiązania tego problemu, przy czym
jeden z nich musi korzystać ze zmiennych warunkowych (condition variable) [27p].
C) trzy programy symulujące trzy różne rozwiązania tego problemu, przy czym
przynajmniej jeden z nich musi korzystać ze zmiennych warunkowych [34p].

# Dodatkowe Wymagania
- Ilość wątków pisarzy R i czytelników W można przekazać jako argumenty linii
poleceń. 
- Zarówno czytelnicy jak i pisarze wkrótce po opuszczeniu czytelni próbują
znów się do niej dostać.
- Program powinien wypisywać komunikaty według poniższego przykładu: 
          ReaderQ: 11 WriterQ: 10 [in: R:0 W:1]
          
Oznacza to, że w kolejce przed czytelnią czeka 10 pisarzy i 11 czytelników a sama
czytelnia zajęta jest przez jednego pisarza. 
Komunikat należy wypisywać w momencie zmiany którejkolwiek z tych wartości. 
