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
>The dsp for this project was assembled by taking parts from a number of different libraries.  An fdn reverb from Faust (the model of which you can see below), a bi-quad filter from stk, butterworth lopass coefficients from Flocking, audio processing from rt-audio, and the MCD api as the glue to bind it all together. 

---

<iframe src="/images/Reverb-svg/process.svg" ></iframe>

---

---
The above Physical Model of an FDN reverb written by Julius Smith and is included with the [faust](http://faust.grame.fr/) distribution as reverb_designer. Below is the original faust code.

```
el = library("effect.lib");

  N = 16; // Feedback Delay Network (FDN) order (power of 2, 2 to 16)
 NB =  5; // Number of T60-controlled frequency-bands (3 or more)
BSO =  3; // Order of each lowpass/highpass bandsplit (odd positive integer)

process = el.fdnrev0_demo(N,NB,BSO);
```

Initially I compield a native qt / core-audio application to gather a number of possible tunings for the reverb.

---

<br>
![example tuning](/images/example-tuning.png)
<br>

---

Once I found a sound that was worth exploring I used faust to compile the reverb to a C++ function using the faust minimal.cpp architecture file.  After breaking out the headers from the compiled cpp file I was able to simple create a Reverb object and call its compute function to get an audio buffer.  This part was a little bit tricky as the way faust is expecting audio buffer to be an array of buffer which is a different interface than rt-audio's method of interlacing all channels on a single buffer.

After getting the faust reverb to work I was immediately faced with the dilemma that the reverb did not sound the same in my application as it did in the designer.  Specifically there were far too many high frequencies compared to the original.

This problem was solved through the use of a bi-quad filter that I borrowed from stk that was tuned using butterworth low-pass coefficients that I calculated using a function from Colin Clark's [Flocking Library](http://flockingjs.org/)

```js
lowPass: function (model, freq) {
    var co = model.coeffs;
    var lambda = 1 / Math.tan(Math.PI * freq / model.sampleRate);
    var lambdaSquared = lambda * lambda;
    var rootTwoLambda = flock.ROOT2 * lambda;
    var b0 = 1 / (1 + rootTwoLambda + lambdaSquared)
    co.b[0] = b0;
    co.b[1] = 2 * b0;
    co.b[2] = b0;
    co.a[0] = 2 * (1 - lambdaSquared) * b0;
    co.a[1] = (1 - rootTwoLambda + lambdaSquared) * b0;
}
```

This filter was applied to the signal during the process of unwrapping the faust arrays, ticking a sample through the filter while being assigned to rt-audios interlaced output buffer.

```c++
// Setup Biquad
Globals::biquad = new stk::BiQuad;
Globals::biquad->setCoefficients(
    0.0009405043311967682,
    0.0018810086623935365,
    0.0009405043311967682,
    -1.9113981953542545,
    0.9151602126790416
);

Globals::reverb->compute(numFrames, Globals::finputs, Globals::foutputs);

for (int j = 0; j < THEREMAX_FRAMESIZE; j++)
{
    for (int i = 0; i < THEREMAX_NUMCHANNELS; i++)
    {
        buffer[(j * THEREMAX_NUMCHANNELS) + i] =
         Globals::biquad->tick(Globals::foutputs[i][j]);
    }
}
```

The physical model was controlled using the cvIntensity that was constantly being calculated in the computer vision thread.  Every time the thread would update the value of cvIntensity it would also update a number of global values on the reverb model.  This method allowed the audio thread to be non-blocking, and to get sample based changes even with audio being process by frame.

```c++
if (Globals::cvIntensity > 0.50)
{
    Globals::reverb->fcheckbox0 = false;
}
else 
{
    Globals::reverb->fcheckbox0 = true;
}

// Update the reverb ... first the room dimentions
double exponent = (Globals::cvIntensity * -1) + 1;
exponent = (exponent * -8) + 1;
if (exponent > 1)
{
    exponent = 1;
}
// Room Dimensions / min acoustic ray length
Globals::reverb->fhslider1 = pow(10, exponent);
// Room Dimensions / max acoustic ray length
Globals::reverb->fhslider0 = pow(10, exponent);

double tuning = 1500 + 700 * Globals::cvIntensity;
// Now some of the upper edges
Globals::reverb->fhslider6 = tuning;
Globals::reverb->fhslider5 = tuning - 200;
```