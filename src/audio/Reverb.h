#ifndef  __Reverb_H__
#define  __Reverb_H__
/************************************************************************
 ************************************************************************
    FAUST Architecture File
	Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------

	This is sample code. This file is provided as an example of minimal
	FAUST architecture file. Redistribution and use in source and binary
	forms, with or without modification, in part or in full are permitted.
	In particular you can create a derived work of this FAUST architecture
	and distribute that work under terms of your choice.

	This sample code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 ************************************************************************
 ************************************************************************/

#include <cmath>
#include <algorithm>

#ifndef FAUST_UI_H
#define FAUST_UI_H

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

using namespace std;
/*******************************************************************************
 * UI : Faust User Interface
 * This abstract class contains only the method that the faust compiler can
 * generate to describe a DSP interface.
 ******************************************************************************/

class UI
{

 public:

	UI() {}

	virtual ~UI() {}

    // -- widget's layouts

    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;

    // -- active widgets

    virtual void addButton(const char* label, FAUSTFLOAT* zone) = 0;
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) = 0;
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;

    // -- passive widgets

    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;

	// -- metadata declarations

    virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) {}
};

#endif
/************************************************************************
	IMPORTANT NOTE : this file contains two clearly delimited sections :
	the ARCHITECTURE section (in two parts) and the USER section. Each section
	is governed by its own copyright and license. Please check individually
	each section for license and copyright information.
*************************************************************************/

/*******************BEGIN ARCHITECTURE SECTION (part 1/2)****************/

/************************************************************************
    FAUST Architecture File
	Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3 of
	the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
	along with this program; If not, see <http://www.gnu.org/licenses/>.

	EXCEPTION : As a special exception, you may create a larger work
	that contains this FAUST architecture section and distribute
	that work under terms of your choice, so long as this FAUST
	architecture section is not modified.


 ************************************************************************
 ************************************************************************/
 
/******************************************************************************
*******************************************************************************

								FAUST DSP

*******************************************************************************
*******************************************************************************/

#ifndef __dsp__
#define __dsp__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

class UI;

//----------------------------------------------------------------
//  signal processor definition
//----------------------------------------------------------------

class dsp {

 public:
	dsp() {}
	virtual ~dsp() {}

	virtual int getNumInputs() 										= 0;
	virtual int getNumOutputs() 									= 0;
	virtual void buildUserInterface(UI* interface) 					= 0;
	virtual void init(int samplingRate) 							= 0;
 	virtual void compute(int len, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) 	= 0;
};

// On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
// flags to avoid costly denormals
#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

#endif
/************************************************************************
 ************************************************************************
    FAUST Architecture File
	Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3 of
	the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
	along with this program; If not, see <http://www.gnu.org/licenses/>.

 ************************************************************************
 ************************************************************************/
 
#ifndef __meta__
#define __meta__

struct Meta
{
    virtual void declare(const char* key, const char* value) = 0;
};

#endif


/******************************************************************************
*******************************************************************************

							       VECTOR INTRINSICS

*******************************************************************************
*******************************************************************************/


/******************************************************************************
*******************************************************************************

			ABSTRACT USER INTERFACE

*******************************************************************************
*******************************************************************************/

//----------------------------------------------------------------------------
//  FAUST generated signal processor
//----------------------------------------------------------------------------

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif  

#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS Reverb
#endif

class Reverb : public dsp {
	
  public:
	
	float fRec15[3];
	float fVec66[8192];
	float fRec14[3];
	float fVec65[8192];
	float fRec13[3];
	float fVec64[8192];
	float fRec12[3];
	float fVec63[8192];
	float fRec11[3];
	float fVec62[8192];
	float fRec10[3];
	float fVec61[8192];
	float fRec9[3];
	float fVec60[8192];
	float fRec8[3];
	float fVec59[8192];
	float fRec7[3];
	float fVec58[8192];
	float fRec6[3];
	float fVec57[8192];
	float fRec5[3];
	float fVec56[8192];
	float fRec4[3];
	float fVec55[8192];
	float fRec3[3];
	float fVec54[8192];
	float fRec2[3];
	float fVec53[8192];
	float fRec1[3];
	float fVec52[8192];
	float fVec51[2];
	float fRec0[3];
	float fVec50[8192];
	float fRec368[3];
	float fRec369[2];
	float fRec364[3];
	float fRec365[2];
	float fVec49[2];
	float fRec366[3];
	float fRec367[2];
	float fRec359[3];
	float fRec360[3];
	float fRec361[2];
	float fVec48[2];
	float fRec362[3];
	float fRec363[2];
	float fRec353[3];
	float fRec354[3];
	float fRec355[3];
	float fRec356[2];
	float fVec47[2];
	float fRec357[3];
	float fRec358[2];
	float fRec348[3];
	float fRec349[3];
	float fRec350[3];
	float fRec351[3];
	float fRec352[2];
	float fRec346[3];
	float fRec347[2];
	float fRec342[3];
	float fRec343[2];
	float fVec46[2];
	float fRec344[3];
	float fRec345[2];
	float fRec337[3];
	float fRec338[3];
	float fRec339[2];
	float fVec45[2];
	float fRec340[3];
	float fRec341[2];
	float fRec331[3];
	float fRec332[3];
	float fRec333[3];
	float fRec334[2];
	float fVec44[2];
	float fRec335[3];
	float fRec336[2];
	float fRec326[3];
	float fRec327[3];
	float fRec328[3];
	float fRec329[3];
	float fRec330[2];
	float fRec324[3];
	float fRec325[2];
	float fRec320[3];
	float fRec321[2];
	float fVec43[2];
	float fRec322[3];
	float fRec323[2];
	float fRec315[3];
	float fRec316[3];
	float fRec317[2];
	float fVec42[2];
	float fRec318[3];
	float fRec319[2];
	float fRec309[3];
	float fRec310[3];
	float fRec311[3];
	float fRec312[2];
	float fVec41[2];
	float fRec313[3];
	float fRec314[2];
	float fRec304[3];
	float fRec305[3];
	float fRec306[3];
	float fRec307[3];
	float fRec308[2];
	float fRec302[3];
	float fRec303[2];
	float fRec298[3];
	float fRec299[2];
	float fVec40[2];
	float fRec300[3];
	float fRec301[2];
	float fRec293[3];
	float fRec294[3];
	float fRec295[2];
	float fVec39[2];
	float fRec296[3];
	float fRec297[2];
	float fRec287[3];
	float fRec288[3];
	float fRec289[3];
	float fRec290[2];
	float fVec38[2];
	float fRec291[3];
	float fRec292[2];
	float fRec282[3];
	float fRec283[3];
	float fRec284[3];
	float fRec285[3];
	float fRec286[2];
	float fRec280[3];
	float fRec281[2];
	float fRec276[3];
	float fRec277[2];
	float fVec37[2];
	float fRec278[3];
	float fRec279[2];
	float fRec271[3];
	float fRec272[3];
	float fRec273[2];
	float fVec36[2];
	float fRec274[3];
	float fRec275[2];
	float fRec265[3];
	float fRec266[3];
	float fRec267[3];
	float fRec268[2];
	float fVec35[2];
	float fRec269[3];
	float fRec270[2];
	float fRec260[3];
	float fRec261[3];
	float fRec262[3];
	float fRec263[3];
	float fRec264[2];
	float fRec258[3];
	float fRec259[2];
	float fRec254[3];
	float fRec255[2];
	float fVec34[2];
	float fRec256[3];
	float fRec257[2];
	float fRec249[3];
	float fRec250[3];
	float fRec251[2];
	float fVec33[2];
	float fRec252[3];
	float fRec253[2];
	float fRec243[3];
	float fRec244[3];
	float fRec245[3];
	float fRec246[2];
	float fVec32[2];
	float fRec247[3];
	float fRec248[2];
	float fRec238[3];
	float fRec239[3];
	float fRec240[3];
	float fRec241[3];
	float fRec242[2];
	float fRec236[3];
	float fRec237[2];
	float fRec232[3];
	float fRec233[2];
	float fVec31[2];
	float fRec234[3];
	float fRec235[2];
	float fRec227[3];
	float fRec228[3];
	float fRec229[2];
	float fVec30[2];
	float fRec230[3];
	float fRec231[2];
	float fRec221[3];
	float fRec222[3];
	float fRec223[3];
	float fRec224[2];
	float fVec29[2];
	float fRec225[3];
	float fRec226[2];
	float fRec216[3];
	float fRec217[3];
	float fRec218[3];
	float fRec219[3];
	float fRec220[2];
	float fRec214[3];
	float fRec215[2];
	float fRec210[3];
	float fRec211[2];
	float fVec28[2];
	float fRec212[3];
	float fRec213[2];
	float fRec205[3];
	float fRec206[3];
	float fRec207[2];
	float fVec27[2];
	float fRec208[3];
	float fRec209[2];
	float fRec199[3];
	float fRec200[3];
	float fRec201[3];
	float fRec202[2];
	float fVec26[2];
	float fRec203[3];
	float fRec204[2];
	float fRec194[3];
	float fRec195[3];
	float fRec196[3];
	float fRec197[3];
	float fRec198[2];
	float fRec192[3];
	float fRec193[2];
	float fRec188[3];
	float fRec189[2];
	float fVec25[2];
	float fRec190[3];
	float fRec191[2];
	float fRec183[3];
	float fRec184[3];
	float fRec185[2];
	float fVec24[2];
	float fRec186[3];
	float fRec187[2];
	float fRec177[3];
	float fRec178[3];
	float fRec179[3];
	float fRec180[2];
	float fVec23[2];
	float fRec181[3];
	float fRec182[2];
	float fRec172[3];
	float fRec173[3];
	float fRec174[3];
	float fRec175[3];
	float fRec176[2];
	float fRec170[3];
	float fRec171[2];
	float fRec166[3];
	float fRec167[2];
	float fVec22[2];
	float fRec168[3];
	float fRec169[2];
	float fRec161[3];
	float fRec162[3];
	float fRec163[2];
	float fVec21[2];
	float fRec164[3];
	float fRec165[2];
	float fRec155[3];
	float fRec156[3];
	float fRec157[3];
	float fRec158[2];
	float fVec20[2];
	float fRec159[3];
	float fRec160[2];
	float fRec150[3];
	float fRec151[3];
	float fRec152[3];
	float fRec153[3];
	float fRec154[2];
	float fRec148[3];
	float fRec149[2];
	float fRec144[3];
	float fRec145[2];
	float fVec19[2];
	float fRec146[3];
	float fRec147[2];
	float fRec139[3];
	float fRec140[3];
	float fRec141[2];
	float fVec18[2];
	float fRec142[3];
	float fRec143[2];
	float fRec133[3];
	float fRec134[3];
	float fRec135[3];
	float fRec136[2];
	float fVec17[2];
	float fRec137[3];
	float fRec138[2];
	float fRec128[3];
	float fRec129[3];
	float fRec130[3];
	float fRec131[3];
	float fRec132[2];
	float fRec126[3];
	float fRec127[2];
	float fRec122[3];
	float fRec123[2];
	float fVec16[2];
	float fRec124[3];
	float fRec125[2];
	float fRec117[3];
	float fRec118[3];
	float fRec119[2];
	float fVec15[2];
	float fRec120[3];
	float fRec121[2];
	float fRec111[3];
	float fRec112[3];
	float fRec113[3];
	float fRec114[2];
	float fVec14[2];
	float fRec115[3];
	float fRec116[2];
	float fRec106[3];
	float fRec107[3];
	float fRec108[3];
	float fRec109[3];
	float fRec110[2];
	float fRec104[3];
	float fRec105[2];
	float fRec100[3];
	float fRec101[2];
	float fVec13[2];
	float fRec102[3];
	float fRec103[2];
	float fRec95[3];
	float fRec96[3];
	float fRec97[2];
	float fVec12[2];
	float fRec98[3];
	float fRec99[2];
	float fRec89[3];
	float fRec90[3];
	float fRec91[3];
	float fRec92[2];
	float fVec11[2];
	float fRec93[3];
	float fRec94[2];
	float fRec84[3];
	float fRec85[3];
	float fRec86[3];
	float fRec87[3];
	float fRec88[2];
	float fRec82[3];
	float fRec83[2];
	float fRec78[3];
	float fRec79[2];
	float fVec10[2];
	float fRec80[3];
	float fRec81[2];
	float fRec73[3];
	float fRec74[3];
	float fRec75[2];
	float fVec9[2];
	float fRec76[3];
	float fRec77[2];
	float fRec67[3];
	float fRec68[3];
	float fRec69[3];
	float fRec70[2];
	float fVec8[2];
	float fRec71[3];
	float fRec72[2];
	float fRec62[3];
	float fRec63[3];
	float fRec64[3];
	float fRec65[3];
	float fRec66[2];
	float fRec60[3];
	float fRec61[2];
	float fRec56[3];
	float fRec57[2];
	float fVec7[2];
	float fRec58[3];
	float fRec59[2];
	float fRec51[3];
	float fRec52[3];
	float fRec53[2];
	float fVec6[2];
	float fRec54[3];
	float fRec55[2];
	float fRec45[3];
	float fRec46[3];
	float fRec47[3];
	float fRec48[2];
	float fVec5[2];
	float fRec49[3];
	float fRec50[2];
	float fRec40[3];
	float fRec41[3];
	float fRec42[3];
	float fRec43[3];
	float fRec44[2];
	float fRec38[3];
	float fRec39[2];
	float fRec34[3];
	float fRec35[2];
	float fVec4[2];
	float fRec36[3];
	float fRec37[2];
	float fRec29[3];
	float fRec30[3];
	float fRec31[2];
	float fVec3[2];
	float fRec32[3];
	float fRec33[2];
	float fRec23[3];
	float fRec24[3];
	float fRec25[3];
	float fRec26[2];
	float fVec2[2];
	float fRec27[3];
	float fRec28[2];
	float fRec18[3];
	float fRec19[3];
	float fRec20[3];
	float fRec21[3];
	float fRec22[2];
	float fVec1[2];
	float fVec0[2];
	float fRec16[4];
	int iRec17[2];
	FAUSTFLOAT fhslider0;
	FAUSTFLOAT fcheckbox0;
	FAUSTFLOAT fbutton0;
	FAUSTFLOAT fbutton1;
	FAUSTFLOAT fcheckbox1;
	FAUSTFLOAT fbutton2;
	int fSamplingFreq;
	int iConst0;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fhslider1;
	FAUSTFLOAT fhslider2;
	FAUSTFLOAT fvslider0;
	float fConst3;
	FAUSTFLOAT fhslider3;
	FAUSTFLOAT fhslider4;
	FAUSTFLOAT fhslider5;
	FAUSTFLOAT fhslider6;
	FAUSTFLOAT fvslider1;
	FAUSTFLOAT fvslider2;
	FAUSTFLOAT fvslider3;
	FAUSTFLOAT fvslider4;
	int IOTA;
	FAUSTFLOAT fbutton3;
	
  public:
	
	void static metadata(Meta* m) { 
		m->declare("effect.lib/author", "Julius O. Smith (jos at ccrma.stanford.edu)");
		m->declare("effect.lib/copyright", "Julius O. Smith III");
		m->declare("effect.lib/exciter_author", "Priyanka Shekar (pshekar@ccrma.stanford.edu)");
		m->declare("effect.lib/exciter_copyright", "Copyright (c) 2013 Priyanka Shekar");
		m->declare("effect.lib/exciter_license", "MIT License (MIT)");
		m->declare("effect.lib/exciter_name", "Harmonic Exciter");
		m->declare("effect.lib/exciter_version", "1.0");
		m->declare("effect.lib/license", "STK-4.3");
		m->declare("effect.lib/name", "Faust Audio Effect Library");
		m->declare("effect.lib/version", "1.33");
		m->declare("filter.lib/author", "Julius O. Smith (jos at ccrma.stanford.edu)");
		m->declare("filter.lib/copyright", "Julius O. Smith III");
		m->declare("filter.lib/license", "STK-4.3");
		m->declare("filter.lib/name", "Faust Filter Library");
		m->declare("filter.lib/reference", "https://ccrma.stanford.edu/~jos/filters/");
		m->declare("filter.lib/version", "1.29");
		m->declare("math.lib/author", "GRAME");
		m->declare("math.lib/copyright", "GRAME");
		m->declare("math.lib/license", "LGPL with exception");
		m->declare("math.lib/name", "Math Library");
		m->declare("math.lib/version", "1.0");
		m->declare("music.lib/author", "GRAME");
		m->declare("music.lib/copyright", "GRAME");
		m->declare("music.lib/license", "LGPL with exception");
		m->declare("music.lib/name", "Music Library");
		m->declare("music.lib/version", "1.0");
		m->declare("oscillator.lib/author", "Julius O. Smith (jos at ccrma.stanford.edu)");
		m->declare("oscillator.lib/copyright", "Julius O. Smith III");
		m->declare("oscillator.lib/license", "STK-4.3");
		m->declare("oscillator.lib/name", "Faust Oscillator Library");
		m->declare("oscillator.lib/version", "1.11");
	}

	virtual int getNumInputs();
    
	virtual int getNumOutputs();
    
	virtual int getInputRate(int channel);
    
	virtual int getOutputRate(int channel);
	
	static void classInit(int samplingFreq);
	
	virtual void instanceInit(int samplingFreq);
	
	virtual void init(int samplingFreq);
	
	virtual void buildUserInterface(UI* interface);
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs);
	
};

#ifdef FAUST_UIMACROS
	#define FAUST_INPUTS 2
	#define FAUST_OUTPUTS 2
	#define FAUST_ACTIVES 0
	#define FAUST_PASSIVES 0
	FAUST_ADDHORIZONTALSLIDER("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[1] Band Crossover Frequencies/Band 0 upper edge in Hz", fhslider6, 5e+02f, 1e+02f, 1e+04f, 1.0f);
	FAUST_ADDHORIZONTALSLIDER("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[1] Band Crossover Frequencies/Band 1 upper edge in Hz", fhslider5, 1e+03f, 1e+02f, 1e+04f, 1.0f);
	FAUST_ADDHORIZONTALSLIDER("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[1] Band Crossover Frequencies/Band 2 upper edge in Hz", fhslider4, 2e+03f, 1e+02f, 1e+04f, 1.0f);
	FAUST_ADDHORIZONTALSLIDER("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[1] Band Crossover Frequencies/Band 3 upper edge in Hz", fhslider3, 4e+03f, 1e+02f, 1e+04f, 1.0f);
	FAUST_ADDVERTICALSLIDER("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[2] Band Decay Times (T60)/0", fvslider3, 8.4f, 0.1f, 1e+01f, 0.1f);
	FAUST_ADDVERTICALSLIDER("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[2] Band Decay Times (T60)/1", fvslider4, 6.5f, 0.1f, 1e+01f, 0.1f);
	FAUST_ADDVERTICALSLIDER("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[2] Band Decay Times (T60)/2", fvslider2, 5.0f, 0.1f, 1e+01f, 0.1f);
	FAUST_ADDVERTICALSLIDER("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[2] Band Decay Times (T60)/3", fvslider1, 3.8f, 0.1f, 1e+01f, 0.1f);
	FAUST_ADDVERTICALSLIDER("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[2] Band Decay Times (T60)/4", fvslider0, 2.7f, 0.1f, 1e+01f, 0.1f);
	FAUST_ADDHORIZONTALSLIDER("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[3] Room Dimensions/min acoustic ray length", fhslider1, 46.0f, 0.1f, 63.0f, 0.1f);
	FAUST_ADDHORIZONTALSLIDER("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[3] Room Dimensions/max acoustic ray length", fhslider2, 63.0f, 0.1f, 63.0f, 0.1f);
	FAUST_ADDCHECKBOX("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[4] Input Controls/[1] Input Config/Mute Ext Inputs", fcheckbox1);
	FAUST_ADDCHECKBOX("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[4] Input Controls/[1] Input Config/Pink Noise", fcheckbox0);
	FAUST_ADDBUTTON("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[4] Input Controls/[2] Impulse Selection/Left", fbutton1);
	FAUST_ADDBUTTON("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[4] Input Controls/[2] Impulse Selection/Center", fbutton0);
	FAUST_ADDBUTTON("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[4] Input Controls/[2] Impulse Selection/Right", fbutton3);
	FAUST_ADDBUTTON("reverb_designer/FEEDBACK DELAY NETWORK (FDN) REVERBERATOR, ORDER 16     [tooltip: See Faust's effect.lib for documentation and references]/[4] Input Controls/[3] Reverb State/Quench", fbutton2);
	FAUST_ADDHORIZONTALSLIDER("reverb_designer/Output Level (dB)", fhslider0, -4e+01f, -7e+01f, 2e+01f, 0.1f);
#endif
    
#endif