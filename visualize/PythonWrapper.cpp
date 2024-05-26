#include "../include/Welle.hpp"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <iterator>
#include <vector>

template <typename T>
std::vector<T> sineWave(int samplingRate, int frequency, double peakToPeak,
                        double phaseShift) {
  return welle::SineWave<T>(samplingRate)
      .generatePeriod(frequency, peakToPeak, phaseShift);
}

template <typename T>
std::vector<T> squareWave(int samplingRate, int frequency, double peakToPeak,
                          double phaseShift) {
  return welle::SquareWave<T>(samplingRate)
      .generatePeriod(frequency, peakToPeak, phaseShift);
}

template <typename T>
std::vector<T> sawWave(int samplingRate, int frequency, double peakToPeak,
                       double phaseShift) {
  return welle::SawWave<T>(samplingRate)
      .generatePeriod(frequency, peakToPeak, phaseShift);
}

template <typename T>
std::vector<T> triangleWave(int samplingRate, int frequency, double peakToPeak,
                            double phaseShift) {
  return welle::TriangleWave<T>(samplingRate)
      .generatePeriod(frequency, peakToPeak, phaseShift);
}

/**
  Expose Welle C++ code to Python
*/
BOOST_PYTHON_MODULE(WelleVisualize) {
  using namespace boost::python;

  class_<std::vector<double>>("std::vector<double>")
      .def(vector_indexing_suite<std::vector<double>>());

  class_<std::vector<uint16_t>>("std::vector<uint16_t>")
      .def(vector_indexing_suite<std::vector<uint16_t>>());

  class_<std::vector<int>>("std::vector<int>")
      .def(vector_indexing_suite<std::vector<int>>());

  def("sineWave_double", sineWave<double>);
  def("sineWave_uint16_t", sineWave<uint16_t>);
  def("sineWave_int", sineWave<int>);

  def("squareWave_double", squareWave<double>);
  def("squareWave_uint16_t", squareWave<uint16_t>);
  def("squareWave_int", squareWave<int>);

  def("sawWave_double", sawWave<double>);
  def("sawWave_uint16_t", sawWave<uint16_t>);
  def("sawWave_int", sawWave<int>);

  def("triangleWave_double", triangleWave<double>);
  def("triangleWave_uint16_t", triangleWave<uint16_t>);
  def("triangleWave_int", triangleWave<int>);
}