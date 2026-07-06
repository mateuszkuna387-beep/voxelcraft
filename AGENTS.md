# AGENTS.md — instrukcje dla asystenta AI

## Projekt
VoxelCraft — gra blokowa w C++/OpenGL (Minecraft-like).

## Konwencje kodu
- **Język**: C++17/20
- **Format**: 4 spacje, brak tabsów
- **Nazewnictwo**:
  - Klasy: PascalCase (`ChunkManager`)
  - Funkcje/metody: camelCase (`generateMesh`)
  - Zmienne: camelCase (`chunkSize`)
  - Stałe: UPPER_SNAKE (`CHUNK_HEIGHT`)
  - Pliki: PascalCase dla klas (`Chunk.h`), camelCase dla innych
- **Nagłówki**: `#pragma once`
- **Include**: własne przed bibliotecznymi, oddzielone pustą linią
- ** const** używamy wszędzie gdzie to możliwe
- **Smart pointers** zamiast gołych `new`/`delete`
- **Brak** `using namespace std;` w nagłówkach

## Backend konfiguracja
- CMake + FetchContent (GoogleTest, glm)
- OpenGL 3.3 Core Profile
- GLFW + GLAD

## Komendy build

```bash
# Konfiguracja
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Budowanie
cmake --build build

# Testy
cmake --build build --target run_tests

# Release
cmake -B build_release -DCMAKE_BUILD_TYPE=Release
cmake --build build_release
```

## Testy
- GoogleTest
- Pliki testowe: `tests/<moduł>/<Klasa>Test.cpp`
- Każda klasa/funkcja publiczna powinna mieć testy
- Testy uruchamiają się przez `run_tests` target
- Pisz testy równolegle z implementacją (TDD w miarę możliwości)

## Commit rules
- Commity tylko na wyraźne żądanie użytkownika
- NIGDY nie commitować bez pytania
- NIGDY nie używać `git commit --amend` na pushniętych commitach
- Prefixy: `init:`, `feat:`, `fix:`, `test:`, `refactor:`, `docs:`, `chore:`

## Zasady pracy
1. Przed zmianą pliku — przeczytaj go w całości
2. Używaj istniejących wzorców i konwencji kodu
3. Nie dodawaj komentarzy do kodu (chyba że poproszono)
4. Nie twórz plików dokumentacji na własną rękę
5. Jeśli nie wiesz jak działa fragment — szukaj w sąsiednich plikach
6. Po każdej większej zmianie uruchom testy

## Struktura katalogów
```
src/
  main.cpp
  engine/       # pętla, okno, input
  rendering/    # OpenGL, shadery, kamera
  world/        # bloki, chunk, generator
  player/       # gracz, kolizje
  core/         # typy, stałe, math
tests/
  world/
  rendering/
  player/
assets/
  shaders/
  textures/
```
