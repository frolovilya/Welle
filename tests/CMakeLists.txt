add_executable(WelleTests 
  TestModule.cpp 
  FFT.cpp
  ValidationTests.cpp
  AmplitudeTests.cpp
  FrequencyTests.cpp
  PhaseTests.cpp)

find_package(Boost 1.85.0 REQUIRED COMPONENTS unit_test_framework)

find_path(FFTW_HEADER_PATH fftw3.h)
find_library(FFTW_LIB_PATH fftw3)
message("-- FFTW3 Library: " ${FFTW_LIB_PATH})

target_include_directories(WelleTests PRIVATE ${FFTW_HEADER_PATH} ${Boost_INCLUDE_DIRS})
target_link_libraries(WelleTests ${FFTW_LIB_PATH} Boost::unit_test_framework)