declare name "Reverb for some drone shit";
declare author "Myles Borins (mborins@ccrma.stanford.edu)";
declare source "Julius O. Smith (jos at ccrma.stanford.edu)";
declare copyright "Julius O. Smith III";
declare license "STK-4.3"; // Synthesis Tool Kit 4.3 (MIT style license)

el = library("effect.lib");

  N = 16; // Feedback Delay Network (FDN) order (power of 2, 2 to 16)
 NB =  5; // Number of T60-controlled frequency-bands (3 or more)
BSO =  3; // Order of each lowpass/highpass bandsplit (odd positive integer)

process = el.fdnrev0_demo(N,NB,BSO);
