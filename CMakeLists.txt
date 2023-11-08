cmake_minimum_required(VERSION 3.16)
project(particles VERSION 0.1.0)

# abilita il supporto per i test, tra cui l'opzione BUILD_TESTING usata sotto
include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# richiedi l'uso di C++17, senza estensioni non-standard offerte dal compilatore usato 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# abilita warning
string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")

# abilita l'address sanitizer e l'undefined-behaviour sanitizer in debug mode
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")

set(LIBRARY_PATH /home/nico/root/include)

# Link against your library
target_link_libraries(YourExecutable PRIVATE ${LIBRARY_PATH}/libmylibrary.so)


add_executable(particles ParticleType.cpp ResonanceType.cpp Particle.cpp generation.cpp)

target_link_libraries(particles ${ROOT_LIBRARIES})

# se il testing e' abilitato...
 # per disabilitare il testing, passare -DBUILD_TESTING=OFF a cmake durante la fase di configurazione
# if (BUILD_TESTING)

#   # aggiungi l'eseguibile particles.t
#   add_executable(particles.t ParticleType.cpp resonancetype.cpp particle.cpp test.cpp)
#   # aggiungi l'eseguibile particles.t alla lista dei test
#   add_test(NAME particles.t COMMAND particles.t)

#endif()