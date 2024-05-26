#include "../include/Welle.hpp"
#include "FFT.hpp"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <numbers>

using namespace std;
using namespace welle;

BOOST_AUTO_TEST_SUITE(Frequency_test)

constexpr double frequencyTolerance = 0.05; // 5%

template <typename T>
void testWaveGeneration(int waveFrequency, int samplingRate, T peakToPeak) {
  auto generator = SineWave<T>(samplingRate);
  auto wave = generator.generatePeriod(waveFrequency, peakToPeak);

  vector<T> oneSecond;
  for (unsigned int i = 0; i < samplingRate / wave.size(); i++) {
    oneSecond.insert(oneSecond.end(), wave.begin(), wave.end());
  }

  BOOST_TEST(wave.size() == ceil(samplingRate / (double)waveFrequency));

  auto fftResult = fft::transform(fft::toComplexDoubleVector(oneSecond));
  int dominantFrequency = 1;
  // skip DC offset bin 0
  for (unsigned int i = 1; i < fftResult.size() / 2; i++) {
    if (abs(fftResult[i]) > abs(fftResult[dominantFrequency])) {
      dominantFrequency = i;
    }
  }

  // wave buffer contains one period samples
  BOOST_TEST(abs(dominantFrequency - waveFrequency) <=
             waveFrequency * frequencyTolerance);
}

BOOST_AUTO_TEST_CASE(generation_test) {
  testWaveGeneration<uint16_t>(10, 1000, 1);
  testWaveGeneration<uint16_t>(10, 1000, 1024);

  testWaveGeneration<double>(440, 24000, 256);
  testWaveGeneration<double>(440, 24000, 2048);
  testWaveGeneration<double>(440, 48000, 1);

  testWaveGeneration<uint16_t>(440, 24000, 256);
  testWaveGeneration<uint16_t>(440, 24000, 2048);
  testWaveGeneration<uint16_t>(440, 48000, 4095);

  testWaveGeneration<int8_t>(10, 1000, 2);
  testWaveGeneration<int8_t>(10, 1000, 100);

  testWaveGeneration<int>(10, 1000, 200); 
  testWaveGeneration<double>(10, 1000, 1);
  testWaveGeneration<float>(10, 1000, 1);
}

BOOST_AUTO_TEST_SUITE_END()