file(GLOB_RECURSE SOURCES "src/*.cpp" "include/*.h")
include("../vcpkg.cmake")
add_executable (OPDA ${SOURCES})

target_link_libraries(OPDA PRIVATE sfml-system sfml-graphics sfml-window sfml-audio imgui::imgui ImGui-SFML::ImGui-SFML unofficial::sqlite3::sqlite3)

find_package(SFML COMPONENTS system window graphics network audio CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

target_include_directories(OPDA PRIVATE "include")


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OPDA PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
