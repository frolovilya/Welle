#include "../include/Welle.hpp"
#include <boost/test/unit_test.hpp>
#include <numbers>
#include <type_traits>

using namespace std;
using namespace welle;

BOOST_AUTO_TEST_SUITE(Amplitude_test)

constexpr double amplitudeTolerance = 0.02; // 2%

template <typename T>
void testWaveGeneration(const Wave<T> &generator, int waveFrequency,
                        T peakToPeak, double phaseShift = 0) {
  auto wave = generator.generatePeriod(waveFrequency, peakToPeak, phaseShift);

  BOOST_TEST(wave.size() ==
             ceil(generator.getSamplingRate() / (double)waveFrequency));

  const T targetMaxAmplitude =
      std::is_unsigned<T>() ? peakToPeak : peakToPeak / 2;
  const T targetMinAmplitude = std::is_unsigned<T>() ? 0 : -peakToPeak / 2;

  T max = *max_element(wave.begin(), wave.end());
  BOOST_TEST_REQUIRE(abs(targetMaxAmplitude - max) <
                     targetMaxAmplitude * amplitudeTolerance);

  T min = *min_element(wave.begin(), wave.end());
  BOOST_TEST_REQUIRE(abs(targetMinAmplitude - min) <
                     targetMaxAmplitude * amplitudeTolerance);
}

BOOST_AUTO_TEST_CASE(sine_amplitude_test) {
  testWaveGeneration(SineWave<uint16_t>(1000), 10, (uint16_t)1);
  testWaveGeneration(SineWave<uint16_t>(1000), 10, (uint16_t)1024);

  testWaveGeneration(SineWave<int8_t>(1000), 10, (int8_t)2);
  testWaveGeneration(SineWave<int8_t>(1000), 10, (int8_t)127);

  testWaveGeneration(SineWave<int>(1000), 15, 200);
  testWaveGeneration(SineWave<int>(1000), 500, 2, numbers::pi / 2);

  testWaveGeneration(SineWave<double>(1000), 20, 1.0);
  testWaveGeneration(SineWave<float>(1000), 32, (float)1);
}

BOOST_AUTO_TEST_SUITE_END()