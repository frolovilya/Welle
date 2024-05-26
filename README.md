# Welle

Multi-purpose header-only periodic wave generation library

## Usage

The library provides C++ class templates to generate single $2 \pi$ wave periods  of various forms and store them in a vector of user-provided type `T`. So that it's possible to optimize memory usage and conform to hardware limits.

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

### Sine

Let's start with a basic Sine wave example at low sampling rate:

```C++
// Sine wave with real sample values
welle::SineWave<double>(1000) // sampled at rate 1000Hz
    .generatePeriod(100, // with frequency 100Hz
                    10,  // with amplitude [-5, 5]
                    0);  // without phase shift
```

![sine1](https://github.com/frolovilya/Welle/assets/271293/835140f1-a595-4869-a1f5-331f6f02698d)

If we take some unsigned type like `uint16_t`, then DC offset is added to each sample, so that peak to peak amplitude is now [0, 10]:

```C++
// Sine wave with unsigned integer sample values
welle::SineWave<uint16_t>(1000) // sampled at rate 1000Hz
    .generatePeriod(100, // with frequency 100Hz
                    10,  // with amplitude [0, 10]
                    0);  // without phase shift
```

![sine2](https://github.com/frolovilya/Welle/assets/271293/1e44c33a-2cb9-4fb1-89f3-8c884aeb6f1d)

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

### Saw

### Triangle

## Build

### Install

The library is header only, you may just copy [Welle.hpp](/include/Welle.hpp) into your project. Alternatively it's also possible to install it into _/usr/local/include_ folder:

```sh
mkdir build; cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release
make install
```

### Run Tests
Tests have dependency on [FFTW3](https://www.fftw.org/download.html) and [Boost](https://www.boost.org/) libraries.

```sh
# brew install boost

mkdir build; cd build
cmake ../ -DCMAKE_BUILD_TYPE=Debug
make
./test/WelleTests
```

### Visualize

Images in this README are generated with [Visualize.py](/visualize/Visualize.py) Python wrapper around Welle API:

```sh
# brew install boost-python3

# build the project as for running tests

cd visualize
/usr/local/bin/pip3.11 install -r requirements.txt
/usr/local/bin/python3.11 Visualize.py -h
```
