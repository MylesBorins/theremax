{
  title: "audio synthesis",
  description: "faust | stk | rt-audio",
  date:  "2013-6-3"
}

<br>
![Hand Tracking](/images/stew.jpg)
<br>

---

>###You got yourself a stew
>The dsp for this project was assembled by taking parts from a number of different libraries.  An fdn reverb from Faust (the model of which you can see below), a bi-quad filter from stk, butterworth lopass coefficients from Flocking, audio processing from rt-audio, and the MCD api as the glue to bind it all together

---

<iframe src="/images/Reverb-svg/process.svg" ></iframe>

---
The above Physical Model of an FDN reverb was originally written by Julius Smith and is included with the [faust](http://faust.grame.fr/) distribution as reverb_designer. Below is the original faust code.

```
el = library("effect.lib");

  N = 16; // Feedback Delay Network (FDN) order (power of 2, 2 to 16)
 NB =  5; // Number of T60-controlled frequency-bands (3 or more)
BSO =  3; // Order of each lowpass/highpass bandsplit (odd positive integer)

process = el.fdnrev0_demo(N,NB,BSO);
```

Initially I compield a native qt / core-audio application to gather a number of possible tunings for the reverb.

<br>
![example tuning](/images/example-tuning.png)
<br>

Once I found a sound that was worth exploring I used faust to compile the reverb to a C++ function using the faust minimal.cpp architecture file.  After breaking out the headers from the compiled cpp file I was able to simple create a Reverb object and call its compute function to get an audio buffer.  This part was a little bit tricky as the way faust is expecting audio buffer to be an array of buffer which is a different interface than rt-audio's method of interlacing all channels on a single buffer.

