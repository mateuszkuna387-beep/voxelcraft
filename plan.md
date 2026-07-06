# Projekt: VoxelCraft — gra blokowa w C++/OpenGL

## Opis
Gra typu sandbox w stylu Minecrafta, napisana w C++ z wykorzystaniem OpenGL (GLFW + GLAD).
Świat złożony z bloków (voxeli), renderowanych w trybie 3D z perspektywą pierwszej osoby.
Silnik wykorzystuje shadery, system chunków, detekcję kolizji oraz fizykę bloków.

## Architektura (moduły)

| Moduł | Opis |
|-------|------|
| `engine/` | Główna pętla, zarządzanie oknem (GLFW), input, delta time |
| `rendering/` | OpenGL: VAO/VBO/EBO, shadery, tekstury, kamera, oświetlenie |
| `world/` | Świat: chunk, blok, generator terenu, system zapisu |
| `player/` | Gracz: ruch, kolizje, ekwipunek, niszczenie/stawianie bloków |
| `core/` | Wspólne typy, math (vec3, AABB), stałe |
| `tests/` | Testy jednostkowe (GoogleTest) |

## Stos technologiczny

- **Język**: C++17/20
- **Biblioteka graficzna**: OpenGL 3.3+ (GLFW + GLAD)
- **Matematyka**: glm (OpenGL Mathematics)
- **Testy**: GoogleTest (CMake FetchContent)
- **Build**: CMake

## Struktura katalogów

```
.
├── CMakeLists.txt          # główny plik build
├── src/
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── engine/
│   │   ├── Engine.h / .cpp
│   │   ├── Window.h / .cpp
│   │   └── Input.h / .cpp
│   ├── rendering/
│   │   ├── Renderer.h / .cpp
│   │   ├── Shader.h / .cpp
│   │   ├── Texture.h / .cpp
│   │   ├── Camera.h / .cpp
│   │   └── Mesh.h / .cpp
│   ├── world/
│   │   ├── World.h / .cpp
│   │   ├── Chunk.h / .cpp
│   │   ├── Block.h / .cpp
│   │   ├── BlockRegistry.h / .cpp
│   │   └── TerrainGenerator.h / .cpp
│   ├── player/
│   │   ├── Player.h / .cpp
│   │   └── Inventory.h / .cpp
│   └── core/
│       ├── Types.h
│       ├── Constants.h
│       └── Math.h
├── tests/
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── world/
│   │   ├── ChunkTest.cpp
│   │   ├── BlockTest.cpp
│   │   └── TerrainGeneratorTest.cpp
│   ├── rendering/
│   │   └── CameraTest.cpp
│   └── player/
│       └── PlayerTest.cpp
├── assets/
│   ├── shaders/
│   │   ├── block.vert
│   │   └── block.frag
│   └── textures/
├── plan.md
├── AGENTS.md
└── .gitignore
```

## Kamienie milowe (milestones)

### M1 — Podstawa (okno + shadery + render)
- [x] Inicjalizacja repozytorium
- [ ] GLFW — okno 3.3+, obsługa inputu
- [ ] GLAD — załadowanie OpenGL
- [ ] Shader — kompilacja, linkowanie, uniformy
- [ ] Renderer — VAO, VBO, EBO, czyszczenie ekranu
- [ ] Kamera — ruch WASD, myszka (FPS)

### M2 — Świat bloków
- [ ] Block — enum typów, dane bloku, Registry
- [ ] Chunk — siatka 16x64x16, generacja meshu (greedy meshing)
- [ ] World — zarządzanie chunkami, LOD
- [ ] TerrainGenerator — Perlin/Simplex noise, biomy
- [ ] Testy: Block, Chunk, TerrainGenerator

### M3 — Gracz i interakcja
- [ ] Player — pozycja, prędkość, grawitacja, skok
- [ ] Kolizje — AABB vs bloki, przesuwanie po osiach
- [ ] Raycast — zaznaczanie/niszczenie/stawianie bloków
- [ ] Ekwipunek — wybór bloku, quickbar
- [ ] Testy: Player (ruch), kolizje

### M4 — Dopracowanie
- [ ] Tekstury — atlas bloków, mipmapy
- [ ] Oświetlenie — ambient + directional (słońce)
- [ ] Zapisywanie/wczytywanie świata (binarny format)
- [ ] Optymalizacja — frustum culling, face culling

## Zasady commitów
- `init:` pierwsza konfiguracja
- `feat:` nowa funkcjonalność
- `fix:` naprawa błędu
- `test:` dodanie / zmiana testów
- `refactor:` refaktoring
- `docs:` zmiana w dokumentacji
- `chore:` build, zależności, CI

## Testy
- GoogleTest (CMake FetchContent)
- Każdy moduł ma osobny plik testowy
- `build_and_test.bat` / `cmake --build build --target run_tests`
