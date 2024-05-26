import matplotlib.pyplot as plt
import argparse
import sys
sys.path.append("../build/visualize")
import WelleVisualize

# Simple script to plot Welle generated waves with various input parameters

parser = argparse.ArgumentParser(description='Visualize Welle generated waves')
parser.add_argument('-w', '--wave', required=True, 
                    choices=['sine', 'square', 'saw', 'triangle'])
parser.add_argument('-t', '--type', default='double', 
                    choices=['double', 'uint16_t'])
parser.add_argument('-s', '--samplingRate', required=True, type=int)
parser.add_argument('-f', '--frequency', required=True, type=int)
parser.add_argument('-a', '--peakToPeakAmplitude', default=10, type=float)
parser.add_argument('-p', '--phaseShift', default=0, type=float)

args = parser.parse_args()

generator = getattr(WelleVisualize, args.wave + 'Wave_' + args.type)
wave = generator(args.samplingRate, args.frequency, args.peakToPeakAmplitude, args.phaseShift)

for w in wave:
  print(w)

plt.figure(figsize=(6, 4))
plt.plot(wave)
plt.title(f'{args.wave.capitalize()}Wave<{args.type}>')
plt.xlabel('Samples')
plt.ylabel('Amplitude')
plt.figtext(0.6, 0.80, f'Sampling Rate: {args.samplingRate}Hz', color = 'gray')
plt.figtext(0.6, 0.74, f'Frequency: {args.frequency}Hz', color = 'gray')
plt.figtext(0.6, 0.68, f'Peak-to-peak: {args.peakToPeakAmplitude}', color = 'gray')
plt.figtext(0.6, 0.62, f'Phase Shift: {args.phaseShift}', color = 'gray')
plt.show()