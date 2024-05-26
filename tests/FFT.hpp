#ifndef FFT_HPP
#define FFT_HPP

#include "fftw3.h"
#include <complex>
#include <vector>

namespace fft {

/**
 * Convert T values vector to complex values vector
 *
 * @param samples real sample values
 * @return complex(real, 0) samples
 */
template <typename T>
std::vector<std::complex<double>>
toComplexDoubleVector(const std::vector<T> &samples) {
  std::vector<std::complex<double>> result;
  result.reserve(samples.size());

  for (const double &sample : samples) {
    result.push_back(std::complex<double>(static_cast<double>(sample)));
  }

  return result;
}

std::vector<std::complex<double>>
transform(const std::vector<std::complex<double>> &samples, bool direct = true);

} // namespace fft

#endif