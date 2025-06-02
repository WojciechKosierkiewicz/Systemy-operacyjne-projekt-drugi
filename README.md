[![Status ostatniego builda](https://github.com/WojciechKosierkiewicz/Systemy-operacyjne-projekt-drugi/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/WojciechKosierkiewicz/Systemy-operacyjne-projekt-drugi/actions/workflows/cmake-single-platform.yml)
# Systemy-operacyjne-projekt-drugi
![Alt text](https://media.tenor.com/YQ-r_mFzlm0AAAAM/typing-cat-typing.gif)

Celem projektu było stworzenie prostej aplikacji czatu w architekturze klient-serwer.
System umożliwia:
- wybór własnego 'nicku' przez użytkownika,
- bieżące odbieranie i wysyłanie wiadomości do innych uczestników,
- serwer zajmuje się utrzymywaniem aktualnej listy połączonych użytkowników oraz rozsyłaniem otrzymywanych wiadomości do wszystkich pozostałych klientów,
- historia czatu jest zapisywana za pomocą serwera do pliku tekstowego, i rozsyłana każdemu nowo dołączającemu użytkownikowi.

Każda wiadomość wysyłana przez klienta zawiera znacznik czasu (timestamp) oraz nazwę użytkownika.

## Kompilacja projektu
> **Uwaga:**  
> Aplikacja została stworzona i przetestowana w środowisku Linux.  
> Poprawne działanie jest gwarantowane wyłącznie na tym systemie operacyjnym.  
> Projekt nie jest kompatybilny z systemem Windows.

Zalecamy kompilację naszego projektu przy użyciu GitHub Actions lub za pomocą kontenera deweloperskiego. Dzieki kontenerowi deweloperskiemu możemy bez problemu programować na każdym systemie operacyjnym oraz wersji linuxa ponieważ kontener posiada wszystkie narzędzia konieczne do kompilacji gwarantując ich kompatybilność między developerami.
Projekt został zbudowany w oparciu o CMake, dlatego aby go skompilować, wystarczy użyć poniższych poleceń:
```bash
cmake -S . -B build
cmake --build build
```
Polecenie to buduje automatycznie binary Clientu i Serweru.

## Logika działania serwera czatu
Serwer jest bardzo prostą aplikacją wielowątkową gdzie dla każdego nowego połączenia jest tworzony nowy wątek (thread) odpowiedzialny za obsługe tego połączenia. Sekcja krytyczna pojawia się w 4 momentach:
- przy tworzeniu nowego threadu dla połączenia,
- przy stracie połączenia,
- przy broadcascie,
- przy zapisie wiadomości z czatu do pliku tekstowego.

By uniknąc problemów z tym zwiazanych użyliśmy mutexów, co skutecznie zabezpiecza listę klientów, jak i stronę serwera.
### Podsumowanie działania:
1. Przy uruchomieniu serwera, należy podać numer portu na którym ma on działać.
2. Użytkownik podaje numer portu, na którym ma działać serwer.
3. Serwer czeka na połączenia klientów.
4. Każdy klient jest obsługiwany w oddzielnym wątku.
5. Po dołączeniu klienta jest mu wysyłana historia czatu.
6. Każda otrzymana wiadomość jest rozsyłana wszystkim pozostałym klientom, oraz wypisywana po stronie serwera w celu monitorowania.
7. Po rozłączeniu klienta, jest on usuwany z listy aktywnych połączeń.

## Logika działania clienta
Klient natomiast unika problemów związanych z wielowątkowością i jedyne co robi, to odczytuje z serwera wiadomości i zwraca ja na wiersz polecenia. Pozwala również na wysłanie swoich wiadomości do serwerowi.

---
