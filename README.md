# OpenGL 선 그리기를 활용한 우주 탐사 장면 구현

OpenGL의 선 그리기 기능을 이용하여 로켓, 고리 달린 행성, UFO, 별자리/혜성으로 구성된 우주 탐사 장면을 구현하는 팀 프로젝트입니다.

---

## 좌표 기준

| 축 | 범위 |
|---|---|
| x | -1.0 ~ 1.0 |
| y | -1.0 ~ 1.0 |

---

## 담당 요소별 좌표 범위

| 요소 | x 범위 | y 범위 | 담당 파일 | 브랜치 |
|---|---|---|---|---|
| 로켓 | -0.25 ~ 0.25 | -0.40 ~ 0.70 | `src/draw_rocket.cpp` | `feature/rocket` |
| 고리 달린 행성 | -0.95 ~ -0.45 | 0.35 ~ 0.90 | `src/draw_ring_planet.cpp` | `feature/ring-planet` |
| UFO 우주선 | 0.45 ~ 0.95 | 0.40 ~ 0.85 | `src/draw_ufo.cpp` | `feature/ufo` |
| 별자리 + 혜성/운석 | -0.90 ~ 0.90 | -0.85 ~ 0.30 | `src/draw_constellation_comet.cpp` | `feature/constellation-comet` |

---

## 프로젝트 구조

```
OpenGL-Space-Scene/
├── CMakeLists.txt
├── Dependency.cmake
├── shader/
│   ├── simple.vs
│   └── simple.fs
└── src/
    ├── main.cpp                        # 진입점, GLFW/OpenGL 초기화 (공통)
    ├── context.cpp / context.h         # 렌더링 루프 및 draw 함수 호출 (공통)
    ├── draw_objects.h                  # 드로우 함수 선언 (공통)
    ├── draw_rocket.cpp                 # 로켓 담당
    ├── draw_ring_planet.cpp            # 고리 달린 행성 담당
    ├── draw_ufo.cpp                    # UFO 담당
    ├── draw_constellation_comet.cpp    # 별자리 + 혜성/운석 담당
    ├── common.cpp / common.h
    ├── shader.cpp / shader.h
    └── program.cpp / program.h
```

---

## 빌드 방법

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

---

## 협업 규칙

- `main` 브랜치에서 직접 작업하지 않기
- 각자 자기 브랜치에서 작업하기
- **각자 맡은 cpp 파일만 수정하기**
- 작업 전 main 최신 내용 pull 받기
- 작업 후 Pull Request로 병합하기

---

## 브랜치 이름

| 담당 요소 | 브랜치 이름 |
|---|---|
| 로켓 | `feature/rocket` |
| 고리 달린 행성 | `feature/ring-planet` |
| UFO 우주선 | `feature/ufo` |
| 별자리 + 혜성/운석 | `feature/constellation-comet` |

---

## 각 담당자가 수정할 파일

| 담당자 | 수정 파일 |
|---|---|
| 로켓 담당 | `src/draw_rocket.cpp` |
| 고리 달린 행성 담당 | `src/draw_ring_planet.cpp` |
| UFO 담당 | `src/draw_ufo.cpp` |
| 별자리 + 혜성/운석 담당 | `src/draw_constellation_comet.cpp` |

> `main.cpp`, `context.cpp`, `context.h`, `CMakeLists.txt` 등 공통 파일은 팀원 간 합의 없이 수정하지 않습니다.
