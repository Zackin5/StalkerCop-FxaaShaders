# STALKER Call of Pripyat FXAA Shaders #

Shader files for an in-engine implementation of FXAA, among other effects.

FXAA can be enabled using the R2_AA console command in-game, additional effects can be controlled by editing the shaders_config.h file as per follows:

* `BUFFER_SCREEN_SIZE`: Your screen resolution, required for accurate calculation of effects.

* `DOF_TYPE`: DOF shader to use. 0 is vanilla, 1 is LightDoF from ReShade (breaks FXAA) and 2 is a combination of vanilla and LightDoF
* `DOF_BLURWIDTH`: Blur amount for DOF_TYPE 1 shader

* `U2_Tonemapping`: Enable Uncharted 2 filmic tonemapping curve
* `U2_Lum`: Tonemap using pixel luminance
* `U2_A` through `U2_F`: Tonemap curve parameters
* `U2_W`: Tonemap white value
* `U2_Exp`: Tonemap exposure value
* `U2_Gamma`: Tonemap gamma correction value

* `FG2_Noise`: Enable FilmGrain2 noise shader from ReShade for in-game noise shader (eg radiation, nightvision)
* `FG2_GrainAmount`
* `FG2_ColorAmount`
* `FG2_LumAmount`
* `FG2_GrainSize`
