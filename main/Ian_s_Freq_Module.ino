//uses ADAFRUIT microphone (MAX4466 with Adjustable Gain)
// VCC = 2.4-5.5v
// connect OUT pin of mic to any teensy analog pin and change AUDIO_INPUT_PIN to that pin


#define ARM_MATH_CM4
#include <arm_math.h>

//constants 
//const int AUDIO_INPUT_PIN = A9;        // Input ADC pin for audio data.  Connect OUT pin of mic to any teensy analog pin 
//                                       // and change AUDIO_INPUT_PIN to that pin
//const int ANALOG_READ_RESOLUTION = 10; // Bits of resolution for the ADC.
//const int ANALOG_READ_AVERAGING = 16;  // Number of samples to average with each ADC reading.
//int SAMPLE_RATE_HZ = 9000;  
//float TONE_THRESHOLD_DB = 45.0;        // Threshold (in decibels) each tone must be above other frequencies to count.
//const int FFT_SIZE = 256;              // Size of the FFT.  Realistically can only be at most 256
//                                       // without running out of memory for buffers and other state.


////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

IntervalTimer samplingTimer;
float samples[FFT_SIZE * 2];
float magnitudes[FFT_SIZE];
int sampleCounter = 0;
////Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);
////char commandBuffer[MAX_CHARS];
//int tonePosition = 0;
//unsigned long toneStart = 0;


//ADD these to setup in main code
/*
 * setupMicrophone();
 */


 int getFreq(){
    
    
    int freq = 0;
    
    // Run FFT on sample data.
    arm_cfft_radix4_instance_f32 fft_inst;
    arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);
    arm_cfft_radix4_f32(&fft_inst, samples);
    // Calculate magnitude of complex numbers output by the FFT.
    arm_cmplx_mag_f32(samples, magnitudes, FFT_SIZE);

    // Detect tone sequence.
    freq = topFreq(magnitudes);
    
    // Restart audio sampling.
    samplingBegin();
    return freq;
    
  
 
 }


 int frequencyToBin(float frequency) {
  float binFrequency = float(SAMPLE_RATE_HZ) / float(FFT_SIZE);
  return int(frequency / binFrequency);
}

int binToFrequency(int bin) {
  float binFrequency = float(SAMPLE_RATE_HZ) / float(FFT_SIZE);
  return int(bin * binFrequency);
}

// Get most promeinent frequency
float topFreq(float* mag) {
  float highestNum = -10000000000.0;
  int best_bin = 0;
  for (int i = 1; i < FFT_SIZE / 2; ++i) {
    if (intensityDb(magnitudes[i]) > highestNum && intensityDb(magnitudes[i]) > TONE_THRESHOLD_DB) {
      highestNum = intensityDb(magnitudes[i]);
      best_bin = i;
    }

  }
  if (best_bin != 0) {
    return binToFrequency(best_bin);
  } else {
    return 0;
  }
}

// Convert intensity to decibels
float intensityDb(float intensity) {
  return 20.0 * log10(intensity);
}


void samplingCallback() {
  // Read from the ADC and store the sample data
  samples[sampleCounter] = (float32_t)analogRead(AUDIO_INPUT_PIN);
  // Complex FFT functions require a coefficient for the imaginary part of the input.
  // Since we only have real data, set this coefficient to zero.
  samples[sampleCounter + 1] = 0.0;
  // Update sample buffer position and stop after the buffer is filled
  sampleCounter += 2;
  if (sampleCounter >= FFT_SIZE * 2) {
    samplingTimer.end();
  }
}

void samplingBegin() {
  // Reset sample buffer position and start callback at necessary rate.
  sampleCounter = 0;
  samplingTimer.begin(samplingCallback, 1000000 / SAMPLE_RATE_HZ);
}

boolean samplingIsDone() {
  return sampleCounter >= FFT_SIZE * 2;
}
void setupMicrophone(){
  //AUDIO_INPUT_PIN = pinNum;
  analogReadResolution(ANALOG_READ_RESOLUTION);
  analogReadAveraging(ANALOG_READ_AVERAGING);
  
  samplingBegin();
}



 
