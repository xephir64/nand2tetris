# CMakeList.txt : projet CMake pour VMTranslator, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.
add_executable (VMTranslator "VMTranslator.cpp" "VMTranslator.h" "Parser.cpp" "Parser.h" "CodeWriter.cpp" "CodeWriter.h" "Instruction.h" "Instruction.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VMTranslator PROPERTY CXX_STANDARD 17)
endif()

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
