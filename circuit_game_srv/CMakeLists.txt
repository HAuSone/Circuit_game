
cmake_minimum_required(VERSION 3.30)
project(circuit_game_srv)

set(CMAKE_CXX_STANDARD 20)

add_executable(circuit_game_srv
        main.cpp
        moteur.cpp
        tank.cpp
        Tire.cpp
        Vehicule.cpp
        Circuit.cpp
        server_network.cpp
)

#------------------- SFML -------------------

# (Windows seulement) indique où se trouve SFML
#  modifie le chemin si nécessaire
set(SFML_DIR "D:/SFML-2.5.0-windows-gcc-7.3.0-mingw-64-bit/SFML-2.5.0/lib/cmake/SFML")

# cherche la bibliothèque SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

target_link_libraries(circuit_game_srv sfml-graphics sfml-window sfml-system sfml-network)

