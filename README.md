[![Status ostatniego builda](https://github.com/WojciechKosierkiewicz/Systemy-operacyjne-projekt-drugi/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/WojciechKosierkiewicz/Systemy-operacyjne-projekt-drugi/actions/workflows/cmake-single-platform.yml)
# Systemy-operacyjne-projekt-drugi
![Alt text](https://media.tenor.com/YQ-r_mFzlm0AAAAM/typing-cat-typing.gif)

Celem naszego projektu było stworzenie prostej aplikacji do chatu. System pozwala na wybranie nicku a potem na odbiór na bierząco komunikacji między innymi użytkownikami. Jedynym działaniem serwera jest utrzymywanie aktualnej listy połączonych urzytkowników i przy wysłaniu przez któregokolwiek z użytkowników wiadomości rozesłanie jej innym.

Celem naszego projektu było stworzenie komunikatora lub chatu. System umożliwia użytkownikowi wybór własnego nicku, a następnie pozwala na bieżące odbieranie i wysyłanie wiadomości do innych uczestników rozmowy. Serwer odpowiada jedynie za utrzymywanie aktualnej listy połączonych użytkowników oraz za rozsyłanie otrzymywanych wiadomości do wszystkich pozostałych osób zalogowanych w systemie. Wysłana wiadomość przez Clienta Zwiera w sobie timestamp wysłania przez użytkownika wiadomości oraz jego nazwe.

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
Serwer jest bardzo prostą aplikacją wielowątkową gdzie dla każdego nowego połączenia jest tworzony nowy thread odpowiedzialny za obsługe tego połączenia. Sekcja krytyczna pojawia się w 3 momentach przy tworzeniu nowego threadu dla połączenia przy stracie połączenia oraz przy broadcascie. By uniknąc problemów z tym zwiazanym użyłem mutexów co skutecznie ubezpiecza liste clientów.
### Podsumowanie działania:
1. Użytkownik podaje numer portu, na którym ma działać serwer.
2. Serwer czeka na połączenia klientów.
3. Każdy klient jest obsługiwany w oddzielnym wątku.
4. Każda otrzymana wiadomość jest rozsyłana wszystkim pozostałym klientom.
5. Po rozłączeniu klienta, jest on usuwany z listy aktywnych połączeń.

## Logika działania clienta
Klient natomiast unika problemów związanych z wielowątkowościa i jedyne co robi to odczytuje z serwera wiadomości i zwraca ja na wiersz polecenia oraz pozwala na wysłanie swoich wiadomości do serwerowi.

---
