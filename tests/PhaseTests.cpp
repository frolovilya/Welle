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
  auto wave = generator.generatePeriod(waveFrequency, 1, phaseShift);

  BOOST_TEST(wave.size() > 0);

  BOOST_TEST(abs(wave[0] - expectedFirstSampleValue) <= diffTolerance);
}

BOOST_AUTO_TEST_CASE(phase_shift_sampling_test) {
  const int samplingRate = 100;

  // whole period is 2 * Pi
  testSamplingWithPhaseShift(SineWave<double>(samplingRate), 10, 0, 0);
  testSamplingWithPhaseShift(SineWave<double>(samplingRate), 10, numbers::pi / 2, 0.5);
  testSamplingWithPhaseShift(SineWave<double>(samplingRate), 10, numbers::pi, 0);
  testSamplingWithPhaseShift(SineWave<double>(samplingRate), 10, 3 * numbers::pi / 2, -0.5);
  testSamplingWithPhaseShift(SineWave<double>(samplingRate), 10, 2 * numbers::pi, 0);

  testSamplingWithPhaseShift(SineWave<uint16_t>(samplingRate), 10, 0, 0);
  testSamplingWithPhaseShift(SineWave<uint16_t>(samplingRate), 10, numbers::pi / 2, 1);
  testSamplingWithPhaseShift(SineWave<uint16_t>(samplingRate), 10, numbers::pi, 0);
  testSamplingWithPhaseShift(SineWave<uint16_t>(samplingRate), 10, 3 * numbers::pi / 2, 0);
  testSamplingWithPhaseShift(SineWave<uint16_t>(samplingRate), 10, 2 * numbers::pi, 0);
}

BOOST_AUTO_TEST_SUITE_END()