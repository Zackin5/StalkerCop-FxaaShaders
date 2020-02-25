// Global settings
#define BUFFER_SCREEN_SIZE half2(1680,1050) // TODO: get proper screen dimensions

// FXAA settings
#define FXAA_QUALITY__PRESET 12

// Choose the amount of sub-pixel aliasing removal.
// This can effect sharpness.
//   1.00 - upper limit (softer)
//   0.75 - default amount of filtering
//   0.50 - lower limit (sharper, less sub-pixel aliasing removal)
//   0.25 - almost off
//   0.00 - completely off
#define FXAA_Subpix 0.75

// The minimum amount of local contrast required to apply algorithm.
//   0.333 - too little (faster)
//   0.250 - low quality
//   0.166 - default
//   0.125 - high quality 
//   0.063 - overkill (slower)
#define FXAA_EdgeThreshold 0.75

// Trims the algorithm from processing darks.
//   0.0833 - upper limit (default, the start of visible unfiltered edges)
//   0.0625 - high quality (faster)
//   0.0312 - visible limit (slower)
// Special notes when using FXAA_GREEN_AS_LUMA,
//   Likely want to set this to zero.
//   As colors that are mostly not-green
//   will appear very dark in the green channel!
//   Tune by looking at mostly non-green content,
//   then start at zero and increase until aliasing is a problem.
#define FXAA_EdgeThresholdMin 0.0

// DOF settings
#define DOF_TYPE 2	// DOF shader to use. 0 is vanilla, 1 is LightDoF from ReShade (breaks FXAA) and 2 is a combination of vanilla and LightDoF
#define DOF_BLURWIDTH 5.0

// Bloom settings

// Tonemapping settings
#define U2_Tonemapping false
#define U2_Lum true
#define U2_A 0.44h
#define U2_B 0.4h
#define U2_C 0.1h
#define U2_D 0.25h
#define U2_E 0.01h
#define U2_F 0.22h
#define U2_W 9.0h
#define U2_Exp 1.0h
#define U2_Gamma 2.2h

// Noise settings
#define FG2_Noise true
#define FG2_GrainAmount 0.07h	//0.05h
#define FG2_ColorAmount 0.4h 	//0.6h
#define FG2_LumAmount 1.0h		//1.0h
#define FG2_GrainSize 2.0h		//1.6h

// Required defines (Don't touch these)
#define FXAA_PC 1
#define FXAA_HLSL_3 1
#define FXAA_GREEN_AS_LUMA 1

#define BUFFER_PIXEL_SIZE half2(1.0/BUFFER_SCREEN_SIZE.x, 1.0/BUFFER_SCREEN_SIZE.y) // TODO: get proper screen dimensions
#define BUFFER_ASPECT_RATIO (BUFFER_SCREEN_SIZE.x/BUFFER_SCREEN_SIZE.y)
