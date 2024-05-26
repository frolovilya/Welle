#include "../include/Welle.hpp"
#include <boost/test/unit_test.hpp>
#include <numbers>

using namespace std;
using namespace welle;

BOOST_AUTO_TEST_SUITE(Validation_test)

BOOST_AUTO_TEST_CASE(samplingRate_more_than_1_test) {
  BOOST_REQUIRE_THROW(SineWave<uint16_t>(0), invalid_argument);
  BOOST_REQUIRE_THROW(SineWave<int16_t>(-10), invalid_argument);
  BOOST_REQUIRE_NO_THROW(SineWave<uint16_t> SineWave(1));
  BOOST_REQUIRE_NO_THROW(SineWave<uint16_t> SineWave(10));
}

BOOST_AUTO_TEST_CASE(frequency_less_than_nyquist_test) {
  const int samplingRate = 1000;
  auto generator = SineWave<uint32_t>(samplingRate);

  int frequency = 501;
  BOOST_REQUIRE_THROW(generator.generatePeriod(frequency, 1), invalid_argument);
  frequency = 500;
  BOOST_REQUIRE_NO_THROW(generator.generatePeriod(frequency, 1));
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

BOOST_AUTO_TEST_SUITE_END()