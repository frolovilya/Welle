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
void testWaveGeneration(const Wave<T> &generator, int waveFrequency, T peakToPeak) {
  auto wave = generator.generatePeriod(waveFrequency, peakToPeak);

  BOOST_TEST(wave.size() == ceil(generator.getSamplingRate() / (double)waveFrequency));

  vector<T> oneSecond;
  for (unsigned int i = 0; i < generator.getSamplingRate() / wave.size(); i++) {
    oneSecond.insert(oneSecond.end(), wave.begin(), wave.end());
  }

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

BOOST_AUTO_TEST_CASE(sine_generation_test) {
  testWaveGeneration(SineWave<uint16_t>(1000), 10, (uint16_t)1);
  testWaveGeneration(SineWave<uint16_t>(1000), 10, (uint16_t)1024);

  testWaveGeneration(SineWave<double>(24000), 440, 256.0);
  testWaveGeneration(SineWave<double>(24000), 440, 2048.0);
  testWaveGeneration(SineWave<double>(48000), 440, 1.0);

  testWaveGeneration(SineWave<uint16_t>(24000), 440, (uint16_t)256);
  testWaveGeneration(SineWave<uint16_t>(24000), 440, (uint16_t)2048);
  testWaveGeneration(SineWave<uint16_t>(48000), 440, (uint16_t)4095);

  testWaveGeneration(SineWave<int8_t>(1000), 10, (int8_t)2);
  testWaveGeneration(SineWave<int8_t>(1000), 10, (int8_t)100);

  testWaveGeneration(SineWave<int>(1000), 10, 200); 
  testWaveGeneration(SineWave<double>(1000), 10, (double)1);
  testWaveGeneration(SineWave<float>(1000), 10, (float)1);
}

BOOST_AUTO_TEST_CASE(saw_generation_test) {
  testWaveGeneration(SawWave<uint16_t>(1000), 10, (uint16_t)10);
  testWaveGeneration(SawWave<uint16_t>(1000), 10, (uint16_t)1024);

  testWaveGeneration(SawWave<double>(24000), 440, 256.0);
  testWaveGeneration(SawWave<double>(24000), 440, 2048.0);
  testWaveGeneration(SawWave<double>(48000), 440, 1.0);

  testWaveGeneration(SawWave<uint16_t>(24000), 440, (uint16_t)256);
  testWaveGeneration(SawWave<uint16_t>(24000), 440, (uint16_t)2048);
  testWaveGeneration(SawWave<uint16_t>(48000), 440, (uint16_t)4095);

  testWaveGeneration(SawWave<int8_t>(1000), 10, (int8_t)2);
  testWaveGeneration(SawWave<int8_t>(1000), 10, (int8_t)100);

  testWaveGeneration(SawWave<int>(1000), 10, 200); 
  testWaveGeneration(SawWave<double>(1000), 10, (double)1);
  testWaveGeneration(SawWave<float>(1000), 10, (float)1);
}

BOOST_AUTO_TEST_CASE(square_generation_test) {
  testWaveGeneration(SquareWave<uint16_t>(1000), 10, (uint16_t)10);
  testWaveGeneration(SquareWave<uint16_t>(1000), 10, (uint16_t)1024);

  testWaveGeneration(SquareWave<double>(24000), 440, 256.0);
  testWaveGeneration(SquareWave<double>(24000), 440, 2048.0);
  testWaveGeneration(SquareWave<double>(48000), 440, 1.0);

  testWaveGeneration(SquareWave<uint16_t>(24000), 440, (uint16_t)256);
  testWaveGeneration(SquareWave<uint16_t>(24000), 440, (uint16_t)2048);
  testWaveGeneration(SquareWave<uint16_t>(48000), 440, (uint16_t)4095);

  testWaveGeneration(SquareWave<int8_t>(1000), 10, (int8_t)2);
  testWaveGeneration(SquareWave<int8_t>(1000), 10, (int8_t)100);

  testWaveGeneration(SquareWave<int>(1000), 10, 200); 
  testWaveGeneration(SquareWave<double>(1000), 10, (double)1);
  testWaveGeneration(SquareWave<float>(1000), 10, (float)1);
}

BOOST_AUTO_TEST_CASE(triangle_generation_test) {
  testWaveGeneration(TriangleWave<uint16_t>(1000), 10, (uint16_t)10);
  testWaveGeneration(TriangleWave<uint16_t>(1000), 10, (uint16_t)1024);

  testWaveGeneration(TriangleWave<double>(24000), 440, 256.0);
  testWaveGeneration(TriangleWave<double>(24000), 440, 2048.0);
  testWaveGeneration(TriangleWave<double>(48000), 440, 1.0);

  testWaveGeneration(TriangleWave<uint16_t>(24000), 440, (uint16_t)256);
  testWaveGeneration(TriangleWave<uint16_t>(24000), 440, (uint16_t)2048);
  testWaveGeneration(TriangleWave<uint16_t>(48000), 440, (uint16_t)4095);

  testWaveGeneration(TriangleWave<int8_t>(1000), 10, (int8_t)2);
  testWaveGeneration(TriangleWave<int8_t>(1000), 10, (int8_t)100);

  testWaveGeneration(TriangleWave<int>(1000), 10, 200); 
  testWaveGeneration(TriangleWave<double>(1000), 10, (double)1);
  testWaveGeneration(TriangleWave<float>(1000), 10, (float)1);
}

BOOST_AUTO_TEST_SUITE_END()