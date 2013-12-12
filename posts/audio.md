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
The above Physical Model of an FDN reverb was originally written by Julius Smith and is included with the faust distribution as reverb_designer. Initially I compield a native qt / core-audio application to gather a number of possible tunings for the reverb.

<br>
![example tuning](/images/example-tuning.png)
<br>
that was compiled to c++ from faust.  It   I took the compiled code from faust and broke out all the class definitions into a header file.



Some more

and some more