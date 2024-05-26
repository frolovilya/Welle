#include "../include/Welle.hpp"
#include <boost/test/unit_test.hpp>
#include <numbers>
#include <type_traits>

using namespace std;
using namespace welle;

BOOST_AUTO_TEST_SUITE(Amplitude_test)

template <typename T>
void testWaveGeneration(int waveFrequency, int samplingRate, T peakToPeak) {
  auto generator = SineWave<T>(samplingRate);
  auto wave = generator.generatePeriod(waveFrequency, peakToPeak);

  BOOST_TEST(wave.size() == ceil(samplingRate / (double)waveFrequency));

  const double amplitudeTolerance = 1e-1;
  const T targetMaxAmplitude = std::is_unsigned<T>() ? peakToPeak : peakToPeak / 2;
  const T targetMinAmplitude = std::is_unsigned<T>() ? 0 : -peakToPeak / 2;

  T max = *max_element(wave.begin(), wave.end());
  BOOST_TEST_REQUIRE(abs(targetMaxAmplitude - max) < amplitudeTolerance);

  T min = *min_element(wave.begin(), wave.end());
  BOOST_TEST_REQUIRE(abs(targetMinAmplitude - min) < amplitudeTolerance);
}

BOOST_AUTO_TEST_CASE(generation_test) {
  testWaveGeneration<uint16_t>(10, 1000, 1);
  testWaveGeneration<uint16_t>(10, 1000, 1024);

  testWaveGeneration<int8_t>(10, 1000, 2);
  testWaveGeneration<int8_t>(10, 1000, 100);

  testWaveGeneration<int>(10, 1000, 200);
  testWaveGeneration<double>(10, 1000, 1);
  testWaveGeneration<float>(10, 1000, 1);
}

BOOST_AUTO_TEST_SUITE_END()