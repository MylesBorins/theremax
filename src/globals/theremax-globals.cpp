//-----------------------------------------------------------------------------
// name: theremax-globals.h
// desc: global stuff for visquin visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "theremax-globals.h"


// defaults
#define DEFAULT_FULLSCREEN    TRUE
#define DEFAULT_WINDOW_WIDTH  1280
#define DEFAULT_WINDOW_HEIGHT 720
#define DEFAULT_BLENDSCREEN   FALSE
#define DEFAULT_FOG           FALSE
#define DEFAULT_VERSION       "1.0.0"

THEREMAXSim * Globals::sim = NULL;

GLsizei Globals::windowWidth = DEFAULT_WINDOW_WIDTH;
GLsizei Globals::windowHeight = DEFAULT_WINDOW_HEIGHT;
GLsizei Globals::lastWindowWidth = Globals::windowWidth;
GLsizei Globals::lastWindowHeight = Globals::windowHeight;

bool Globals::dead = false;

SAMPLE * Globals::lastAudioBuffer = NULL;
SAMPLE * Globals::lastAudioBufferMono = NULL;
SAMPLE * Globals::audioBufferWindow = NULL;
unsigned int Globals::lastAudioBufferFrames = 0;
unsigned int Globals::lastAudioBufferChannels = 0;

SAMPLE Globals::cvIntensity = 0;
SAMPLE Globals::freq = 220;

Reverb * Globals::reverb = NULL;

FAUSTFLOAT ** Globals::finputs = new FAUSTFLOAT*[THEREMAX_NUMCHANNELS];
FAUSTFLOAT ** Globals::foutputs = new FAUSTFLOAT*[THEREMAX_NUMCHANNELS];

GLboolean Globals::fullscreen = DEFAULT_FULLSCREEN;
GLboolean Globals::blendScreen = DEFAULT_BLENDSCREEN;

Vector3D Globals::blendAlpha( 1, 1, .5f );
GLfloat Globals::blendRed = 0.0f;
GLenum Globals::fillmode = GL_FILL;
iSlew3D Globals::bgColor( .5 );
Vector3D Globals::viewRadius( 5, 2, 1 );
Vector3D Globals::viewEyeY( 2, 0, 1.5f );
Vector3D Globals::fov( 80, 100, .2f );

GLuint Globals::textures[THEREMAX_MAX_TEXTURES];

GLfloat Globals::light0_pos[4] = { 2.0f, 1.2f, 4.0f, 1.0f };
GLfloat Globals::light1_ambient[4] = { .2f, .2f, .2f, 1.0f };
GLfloat Globals::light1_diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat Globals::light1_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat Globals::light1_pos[4] = { -2.0f, 0.0f, -4.0f, 1.0f };
GLfloat Globals::linewidth = 1.0f;
GLboolean Globals::fog = DEFAULT_FOG;
GLuint Globals::fog_mode[4];
GLuint Globals::fog_filter;
GLfloat Globals::fog_density;

// colors
Vector3D Globals::ourWhite( 1, 1, 1 );
Vector3D Globals::ourRed( 1, .5, .5 );
Vector3D Globals::ourBlue( 102.0f/255, 204.0f/255, 1.0f );
Vector3D Globals::ourOrange( 1, .75, .25 );
Vector3D Globals::ourGreen( .7, 1, .45 );
Vector3D Globals::ourGray( .4, .4, .4 );
Vector3D Globals::ourYellow( 1, 1, .25 );
Vector3D Globals::ourSoftYellow( .7, .7, .1 );
Vector3D Globals::ourPurple( .6, .25, .6 );

std::string Globals::path = "";
std::string Globals::relpath = "data/texture/";
std::string Globals::datapath = "";
std::string Globals::version = DEFAULT_VERSION;
