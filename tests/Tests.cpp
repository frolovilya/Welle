#define BOOST_TEST_MODULE filter_designer_tests
#include "../Welle.hpp"
#include <boost/test/included/unit_test.hpp>
#include <numbers>

using namespace std;
using namespace welle;

constexpr double diffTolerance = 1e-5;

BOOST_AUTO_TEST_SUITE(SineWave_test)

BOOST_AUTO_TEST_CASE(samplingRate_more_than_1_test) {
  BOOST_REQUIRE_THROW(SineWave<uint16_t>(0), invalid_argument);
  BOOST_REQUIRE_NO_THROW(SineWave<uint16_t> SineWave(1));
  BOOST_REQUIRE_NO_THROW(SineWave<uint16_t> SineWave(10));
}

BOOST_AUTO_TEST_CASE(frequency_less_than_nyquist_test) {
  const int samplingRate = 1000;
  const int frequency = 501;
  auto generator = SineWave<uint32_t>(samplingRate);
  BOOST_REQUIRE_THROW(generator.generatePeriod(frequency, 1), invalid_argument);
}

BOOST_AUTO_TEST_CASE(frequency_more_than_1_test) {
  const int samplingRate = 1000;
  const int frequency = 0;
  auto generator = SineWave<float>(samplingRate);
  BOOST_REQUIRE_THROW(generator.generatePeriod(frequency, 1), invalid_argument);
}

BOOST_AUTO_TEST_CASE(amplitude_is_positive_test) {
  const int samplingRate = 1000;
  const int frequency = 10;
  const int8_t amplitude = -1;
  auto generator = SineWave<int8_t>(samplingRate);
  BOOST_REQUIRE_THROW(generator.generatePeriod(frequency, amplitude),
                      invalid_argument);
}

template <typename T> int numberOfPeaks(const vector<T> &samples, T peakValue) {
  int count = 0;
  for (unsigned int i = 0; i < samples.size(); i++) {
    if (abs(samples[i] - peakValue) < diffTolerance) {
      count++;
    }
  }

  return count;
}

void testWaveGeneration(int waveFrequency, int samplingRate, double amplitude) {
  auto generator = SineWave<double>(samplingRate);
  auto wave = generator.generatePeriod(waveFrequency, amplitude);

  BOOST_TEST(wave.size() == ceil(samplingRate / (double)waveFrequency));

  const double amplitudeTolerance = 1e-1;
  double max = *max_element(wave.begin(), wave.end());
  BOOST_TEST(abs(amplitude - abs(max)) < amplitudeTolerance);

  double min = *min_element(wave.begin(), wave.end());
  BOOST_TEST(abs(amplitude - abs(min)) < amplitudeTolerance);

  // 1 peak - 1 period
  const int peaksCount = numberOfPeaks(wave, max);

  // wave buffer contains one period samples
  BOOST_TEST(peaksCount == 1);
}

BOOST_AUTO_TEST_CASE(generation_test) {
  testWaveGeneration(10, 1000, 1);
  testWaveGeneration(440, 10000, 1);
  testWaveGeneration(800, 20000, 1);
  testWaveGeneration(10000, 48000, 1);
  testWaveGeneration(21000, 100000, 1);
}

void testSamplingWithPhaseShift(int waveFrequency, int samplingRate,
                                double phaseShift,
                                double expectedFirstSampleValue) {
  auto generator = SineWave<double>(samplingRate);
  auto wave = generator.generatePeriod(waveFrequency, 1, phaseShift);

  BOOST_TEST(wave.size() > 0);

  BOOST_TEST(abs(abs(wave[0]) - abs(expectedFirstSampleValue)) < diffTolerance);
}

BOOST_AUTO_TEST_CASE(phase_shift_sampling_test) {
  // whole period is 2 * Pi
  testSamplingWithPhaseShift(10, 100, 0, 0);
  testSamplingWithPhaseShift(10, 100, numbers::pi / 2, 1);
  testSamplingWithPhaseShift(10, 100, numbers::pi, 0);
  testSamplingWithPhaseShift(10, 100, 3 * numbers::pi / 2, -1);
  testSamplingWithPhaseShift(10, 100, 2 * numbers::pi, 0);
}

void testPhaseShiftDiff(int waveFrequency, int samplingRate, double phaseShift1,
                        double phaseShift2, double expectedDiff) {
  auto generator = SineWave<double>(samplingRate);
  auto wave1 = generator.generatePeriod(waveFrequency, 1, phaseShift1);
  auto wave2 = generator.generatePeriod(waveFrequency, 1, phaseShift2);

  double actualDiff = phaseShift(wave1, wave2);

  BOOST_TEST(abs(actualDiff - expectedDiff) < diffTolerance);
}

BOOST_AUTO_TEST_CASE(phase_shift_diff_test) {
  testPhaseShiftDiff(440, 48000, 0, 0, 0);
  testPhaseShiftDiff(440, 48000, 0, numbers::pi, 0);
  testPhaseShiftDiff(440, 48000, 0, 2 * numbers::pi, 0);

  testPhaseShiftDiff(440, 48000, 0, numbers::pi / 2, numbers::pi / 2);
  testPhaseShiftDiff(440, 48000, 0, numbers::pi / 4, numbers::pi / 4);
  testPhaseShiftDiff(440, 48000, 0, 2 * numbers::pi / 3, 2 * numbers::pi / 3);
}

BOOST_AUTO_TEST_SUITE_END()