#ifndef WELLE_HPP
#define WELLE_HPP

#include <cmath>
#include <numbers>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace welle {
/**
 * Base class for wave generation
 */
template <typename T> class Wave {
public:
  explicit Wave(const int samplingRate)
      : samplingRate{samplingRate}, dcOffset([] {
          if constexpr (std::is_unsigned<T>()) {
            return 1;
          } else {
            return 0;
          }
        }()) {
    checkSamplingRate(samplingRate);
  }

  virtual ~Wave() {}

  /**
   * Get sampling rate (Hz) configured for this wave generator
   */
  int getSamplingRate() const { return samplingRate; }

  /**
   * Generate one wave period
   *
   * @param frequency target wave frequency (must be >= 1)
   * @param peakToPeak max wave peak-to-peak amplitude (must be >= 1 for
   * unsigned and floating point types and >=2 for signed integer types)
   * @param phaseShift shift wave start in radians
   * @return sine wave one period samples
   */
  virtual std::vector<T> generatePeriod(const int frequency, const T peakToPeak,
                                        const double phaseShift = 0) const {
    checkFrequency(frequency);
    checkAmplitude(peakToPeak);

    const int period = calculatePeriodSamplesCount(samplingRate, frequency);
    std::vector<T> samples;
    samples.reserve(period);

    for (int i = 0; i < period; i++) {
      samples.push_back(
          calculateSampleAtIndex(i, period, peakToPeak, phaseShift));
    }

    return samples;
  }

protected:
  const int samplingRate;
  const int dcOffset;

  virtual inline T calculateSampleAtIndex(const int i, const int period,
                                          const T peakToPeak,
                                          const double phaseShift) const = 0;

  static inline int modulo(const int x, const int p) {
    return ((x % p) + p) % p;
  }

  static inline int phaseShiftToSamplesOffset(const double phaseShift,
                                              const int period) {
    return std::fmod(phaseShift, 2 * std::numbers::pi) /
           (2 * std::numbers::pi) * period;
  }

private:
  /**
   * Nyquist frequency defines max possible frequency
   * to be digitally captured with a given sampling rate
   *
   * @param samplingRate sampling rate (Hz) (must be >=1)
   * @return max possible frequency (Hz)
   */
  static inline int nyquistFrequency(const int samplingRate) {
    checkSamplingRate(samplingRate);

    return samplingRate / 2;
  }

  /**
   * Calculate how many samples will take a single-period wave with a given
   * frequency and sampling rate
   *
   * @param samplingRate sampling rate (Hz) (must be >=1)
   * @param frequency target wave frequency (must be >= 1 and <= Nyquist
   * frequency)
   * @return number of samples to represent one period
   */
  static int calculatePeriodSamplesCount(const int samplingRate,
                                         const int frequency) {
    checkSamplingRate(samplingRate);
    checkFrequency(frequency);
    checkFrequencyVsSamplingRate(frequency, samplingRate);

    return std::ceil(samplingRate / (double)frequency);
  }

  static inline void checkSamplingRate(const int samplingRate) {
    if (samplingRate < 1) {
      throw std::invalid_argument("samplingRate must be >= 1");
    }
  }
  static inline void checkFrequency(const int frequency) {
    if (frequency < 1) {
      throw std::invalid_argument("frequency must be >= 1");
    }
  }
  static inline void checkAmplitude(const T amplitude) {
    constexpr T minAmplitude =
        std::is_unsigned<T>() || std::is_floating_point<T>() ? 1 : 2;
    if (amplitude < minAmplitude) {
      std::string errorMsg = "peak-to-peak amplitude must be >= ";
      errorMsg += std::to_string(minAmplitude);
      throw std::invalid_argument(errorMsg);
    }
  }
  static inline void checkFrequencyVsSamplingRate(const int frequency,
                                           const int samplingRate) {
    if (frequency > nyquistFrequency(samplingRate)) {
      throw std::invalid_argument(
          "frequency must be < samplingRate / 2 (Nyquist frequency)");
    }
  }
};

/**
 * Sine wave generator
 */
template <typename T> class SineWave : public Wave<T> {
public:
  using Wave<T>::Wave;

protected:
  inline T calculateSampleAtIndex(const int i, const int period,
                                  const T peakToPeak,
                                  const double phaseShift) const override {
    // uint16_t, peakToPeak = 4
    // sin + dcOffset = [0, 2]
    // *peakToPeak = [0, 8]
    // / 2 = [0, 4]

    // uint16_t, peakToPeak = 1
    // sin + dcOffset = [0, 2]
    // *peakToPeak = [0, 2]
    // /2 = [0, 1]

    // int, peakToPeak = 4
    // sin + dcOffset = [-1, 1]
    // *peakToPeak = [-4, 4]
    // /2 = [-2, 2]

    // int, peakToPeak = 2
    // sin + dcOffset = [-1, 1]
    // *peakToPeak = [-2, 2]
    // /2 = [-1, 1]

    return (sin(2 * std::numbers::pi * i / period + phaseShift) +
            this->dcOffset) *
               peakToPeak / 2;
  }
};

/**
 * Saw wave generator
 */
template <typename T> class SawWave : public Wave<T> {
public:
  using Wave<T>::Wave;

protected:
  inline T calculateSampleAtIndex(const int i, const int period,
                                  const T peakToPeak,
                                  const double phaseShift) const override {
    const int shift = this->phaseShiftToSamplesOffset(phaseShift, period);

    return (peakToPeak / (double)period) *
               std::abs(this->modulo(i - period / 2 + shift, period)) -
           (1 - this->dcOffset) * peakToPeak / 2;
  }
};

/**
 * Square wave generator
 */
template <typename T> class SquareWave : public Wave<T> {
public:
  using Wave<T>::Wave;

protected:
  inline T calculateSampleAtIndex(const int i, const int period,
                                  const T peakToPeak,
                                  const double phaseShift) const override {
    const int shift = this->phaseShiftToSamplesOffset(phaseShift, period);

    return ((2 * this->modulo(i + shift, period) < period ? 1 : -1) +
            this->dcOffset) *
           peakToPeak / 2;
  }
};

/**
 * Triangle wave generator
 */
template <typename T> class TriangleWave : public Wave<T> {
public:
  using Wave<T>::Wave;

protected:
  inline T calculateSampleAtIndex(const int i, const int period,
                                  const T peakToPeak,
                                  const double phaseShift) const override {
    const int shift = this->phaseShiftToSamplesOffset(phaseShift, period);

    return 2 * peakToPeak / (double)period *
               std::abs(this->modulo(i - period / 4 + shift, period) - period / 2) -
           (1 - this->dcOffset) * peakToPeak / 2;
  }
};

} // namespace welle

#endif