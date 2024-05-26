#include "../include/Welle.hpp"
#include <boost/test/unit_test.hpp>
#include <numbers>

using namespace std;
using namespace welle;

BOOST_AUTO_TEST_SUITE(Phase_test)

constexpr double diffTolerance = 1e-4;

template <typename T>
void testSamplingWithPhaseShift(const Wave<T> &generator,
                                int waveFrequency,
                                double phaseShift,
                                double expectedFirstSampleValue) {
  auto wave = generator.generatePeriod(waveFrequency, 10, phaseShift);

  BOOST_TEST(wave.size() > 0);

  BOOST_TEST(abs(wave[0] - expectedFirstSampleValue) <= diffTolerance);
}

BOOST_AUTO_TEST_CASE(sine_phase_shift_test) {
  const int samplingRate = 100;

  // whole period is 2 * Pi
  testSamplingWithPhaseShift(SineWave<double>(samplingRate), 10, 0, 0);
  testSamplingWithPhaseShift(SineWave<double>(samplingRate), 10, numbers::pi / 2, 5);
  testSamplingWithPhaseShift(SineWave<double>(samplingRate), 10, numbers::pi, 0);
  testSamplingWithPhaseShift(SineWave<double>(samplingRate), 10, 3 * numbers::pi / 2, -5);
  testSamplingWithPhaseShift(SineWave<double>(samplingRate), 10, 2 * numbers::pi, 0);

  testSamplingWithPhaseShift(SineWave<uint16_t>(samplingRate), 10, 0, 5);
  testSamplingWithPhaseShift(SineWave<uint16_t>(samplingRate), 10, numbers::pi / 2, 10);
  testSamplingWithPhaseShift(SineWave<uint16_t>(samplingRate), 10, numbers::pi, 5);
  testSamplingWithPhaseShift(SineWave<uint16_t>(samplingRate), 10, 3 * numbers::pi / 2, 0);
  testSamplingWithPhaseShift(SineWave<uint16_t>(samplingRate), 10, 2 * numbers::pi, 4);
}

BOOST_AUTO_TEST_CASE(square_phase_shift_test) {
  const int samplingRate = 100;

  // whole period is 2 * Pi
  testSamplingWithPhaseShift(SquareWave<double>(samplingRate), 10, 0, 5);
  testSamplingWithPhaseShift(SquareWave<double>(samplingRate), 10, numbers::pi / 2, 5);
  testSamplingWithPhaseShift(SquareWave<double>(samplingRate), 10, numbers::pi, -5);
  testSamplingWithPhaseShift(SquareWave<double>(samplingRate), 10, 3 * numbers::pi / 2, -5);
  testSamplingWithPhaseShift(SquareWave<double>(samplingRate), 10, 2 * numbers::pi, 5);
  testSamplingWithPhaseShift(SquareWave<double>(samplingRate), 10, -numbers::pi / 2, -5);

  testSamplingWithPhaseShift(SquareWave<uint16_t>(samplingRate), 10, 0, 10);
  testSamplingWithPhaseShift(SquareWave<uint16_t>(samplingRate), 10, numbers::pi / 2, 10);
  testSamplingWithPhaseShift(SquareWave<uint16_t>(samplingRate), 10, numbers::pi, 0);
  testSamplingWithPhaseShift(SquareWave<uint16_t>(samplingRate), 10, 3 * numbers::pi / 2, 0);
  testSamplingWithPhaseShift(SquareWave<uint16_t>(samplingRate), 10, 2 * numbers::pi, 10);
  testSamplingWithPhaseShift(SquareWave<uint16_t>(samplingRate), 10, -numbers::pi / 2, 0);
}

BOOST_AUTO_TEST_CASE(saw_phase_shift_test) {
  const int samplingRate = 100;

  // whole period is 2 * Pi
  testSamplingWithPhaseShift(SawWave<double>(samplingRate), 10, 0, 0);
  testSamplingWithPhaseShift(SawWave<double>(samplingRate), 10, numbers::pi, -5);
  testSamplingWithPhaseShift(SawWave<double>(samplingRate), 10, 2 * numbers::pi, 0);

  testSamplingWithPhaseShift(SawWave<uint16_t>(samplingRate), 10, 0, 5);
  testSamplingWithPhaseShift(SawWave<uint16_t>(samplingRate), 10, numbers::pi, 0);
  testSamplingWithPhaseShift(SawWave<uint16_t>(samplingRate), 10, 2 * numbers::pi, 5);
}

BOOST_AUTO_TEST_CASE(triangle_phase_shift_test) {
  const int samplingRate = 100;

  // whole period is 2 * Pi
  testSamplingWithPhaseShift(TriangleWave<double>(samplingRate), 10, 0, 1);
  testSamplingWithPhaseShift(TriangleWave<double>(samplingRate), 10, numbers::pi, -1);
  testSamplingWithPhaseShift(TriangleWave<double>(samplingRate), 10, 2 * numbers::pi, 1);

  testSamplingWithPhaseShift(TriangleWave<uint16_t>(samplingRate), 10, 0, 6);
  testSamplingWithPhaseShift(TriangleWave<uint16_t>(samplingRate), 10, numbers::pi, 4);
  testSamplingWithPhaseShift(TriangleWave<uint16_t>(samplingRate), 10, 2 * numbers::pi, 6);
}

BOOST_AUTO_TEST_SUITE_END()