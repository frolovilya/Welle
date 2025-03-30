# Welle

Multi-purpose header-only periodic wave generation library

## Usage

The library provides C++ class templates to generate single $2 \pi$ wave periods of various forms and store them in a vector of user-provided type `T`. So that it's possible to optimize memory usage and conform to hardware limits.

```C++
namespace welle {

// base abstract class
template <typename T> class Wave {
public:
  Wave(int samplingRateHz);

  virtual std::vector<T> generatePeriod(int frequencyHz, 
                                        double peakToPeakAmplitude, 
                                        double phaseShift) = 0;
};

// classes for each waveform
class SineWave : public Wave<T>
class SquareWave : public Wave<T>
class SawWave : public Wave<T>
class TriangleWave : public Wave<T>

} // namespace welle
```

Given that wave frequency is an amount of periods per second, it's then straightforward to generate samples for any time duration by repeating the result vector.

### Sine

Let's start with a basic Sine wave example at low sampling rate:

```C++
// Sine wave with real sample values
welle::SineWave<double>(1000) // sampled at rate 1000Hz
    .generatePeriod(50,  // with frequency 50Hz
                    32,  // with amplitude [-16, 16]
                    0);  // without phase shift
```

![sine1_1](https://github.com/frolovilya/Welle/assets/271293/5c67652e-33a0-4325-8854-777807af6551)

If we take some unsigned type like `uint16_t`, then DC offset is added to each sample, so that peak to peak amplitude is now [0, 32]:

```C++
// Sine wave with unsigned integer sample values
welle::SineWave<uint16_t>(1000) // sampled at rate 1000Hz
    .generatePeriod(50,  // with frequency 50Hz
                    32,  // with amplitude [0, 32]
                    0);  // without phase shift
```

![sine2_1](https://github.com/frolovilya/Welle/assets/271293/361a5e98-b58b-4d81-bdf6-f2369f9cce10)

High-precision waves are sampled at higher rates. You could see real usage example in [stm32-wave-generator](https://github.com/frolovilya/stm32-wave-generator) and [filter-designer](https://github.com/frolovilya/filter-designer) projects.

```C++
// Sine wave with unsigned integer values
welle::SineWave<uint16_t>(48000) // sampled at rate 48kHz
    .generatePeriod(440,  // with frequency 440Hz
                    4095, // with amplitude [0, 2^12-1]
                    0);   // without phase shift
```

![sine3](https://github.com/frolovilya/Welle/assets/271293/3b03e142-bf53-45fa-b1bd-8b8849af4f87)

It's also possible to apply a phase shift in radians. Wave from the previous example is shifted to $\pi / 2$:

```C++
// Sine wave with unsigned integer values
welle::SineWave<uint16_t>(48000) // sampled at rate 48kHz
    .generatePeriod(440,                   // with frequency 440Hz
                    4095,                  // with amplitude [0, 2^12-1]
                    std::numeric::pi / 2); // with pi/2 phase shift
```

![sine4](https://github.com/frolovilya/Welle/assets/271293/1c5982b4-8386-41b7-9fc6-4654f812fecb)

### Square

All other wave generators share the same API. Let's just visualize supported wave forms:

```C++
// Square wave with signed integer values
welle::SineWave<uint16_t>(10000) // sampled at rate 100Hz
    .generatePeriod(25,   // with frequency 25Hz
                    100,  // with amplitude [-50, 50]
                    0);   // without phase shift
```

![square1](https://github.com/frolovilya/Welle/assets/271293/0c6d8e8c-b7dd-4010-988a-33e811e13db0)

### Saw

```C++
// Saw wave with signed integer values
welle::SineWave<int>(24000) // sampled at rate 24kHz
    .generatePeriod(22,     // with frequency 22Hz
                    1000,   // with amplitude [-500, 500]
                    0);     // without phase shift
```

![saw2](https://github.com/frolovilya/Welle/assets/271293/a1521d0d-a379-4b7d-88c5-eb8a5dfc5370)

### Triangle

```C++
// Triangle wave with unsigned integer values
welle::SineWave<uint16_t>(1000) // sampled at rate 1kHz
    .generatePeriod(10,   // with frequency 10Hz
                    64,   // with amplitude [0, 64]
                    0);   // without phase shift
```

![triangle1](https://github.com/frolovilya/Welle/assets/271293/0d072fe9-22d4-4c98-859b-746ef1e339a8)

## Build

### Install

The library is header only, you may just copy [Welle.hpp](/include/Welle.hpp) into your project. Alternatively it's also possible to install it into _/usr/local/include_ folder:

```sh
mkdir build; cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release
make install
```
And include header directory in CMake:

```cmake
find_path(WELLE_HEADER_PATH Welle.hpp)
target_include_directories(%TARGET_NAME% PUBLIC ${WELLE_HEADER_PATH})
```

### Run Tests
Tests have dependency on [FFTW3](https://www.fftw.org/download.html) and [Boost](https://www.boost.org/) libraries.

```sh
# brew install boost
# brew install boost-python3
# brew install fftw

mkdir build; cd build
cmake ../ -DCMAKE_BUILD_TYPE=Debug
make
./tests/WelleTests
```

### Visualize

Images in this README are generated with [Visualize.py](/visualize/Visualize.py) Python wrapper around Welle API:

```sh
# build the project in Debug mode first

cd visualize
python3 -m venv welle
welle/bin/pip3 install -r requirements.txt
welle/bin/python3 Visualize.py -h
```
