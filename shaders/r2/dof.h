#ifndef	DOF_H_INCLUDED
#define	DOF_H_INCLUDED

//#define USE_DOF

#ifndef	USE_DOF

half3	dof(half3 color, float2 center)
{
	return	color;
}

#else	//	USE_DOF

// x - near y - focus z - far w - sky distance
half4	dof_params;
half3	dof_kernel;	// x,y - resolution pre-scaled z - just kernel size

#define EPSDEPTH	0.0001h

half DOFFactor( half depth)
{
	half	dist_to_focus	= depth-dof_params.y;
	half 	blur_far	= saturate( dist_to_focus
					/ (dof_params.z-dof_params.y) );
	half 	blur_near	= saturate( dist_to_focus
					/ (dof_params.x-dof_params.y) );
	half 	blur 		= blur_near+blur_far;
	blur*=blur;
	return blur;
}

#if (DOF_TYPE == 1)
// Light DOF from ReShade LightDoF.Fx
float2 rot2D(float2 pos, float angle) {
	float2 source = float2(sin(angle), cos(angle));
	return float2(dot(pos, float2(source.y, -source.x)), dot(pos, source));
}

half3 dof(half3 color, float2 center)
{
	half depth = tex2D(s_position,center).z;
	if (depth <= EPSDEPTH)	depth = dof_params.w;
	half blurAmount = DOFFactor(depth);
	
	// poisson
	half2 	o  [12];
		o[0]	= half2(-0.326212f , -0.405810f);
		o[1] 	= half2(-0.840144f , -0.073580f);
		o[2] 	= half2(-0.695914f ,  0.457137f);
		o[3] 	= half2(-0.203345f ,  0.620716f);
		o[4] 	= half2( 0.962340f , -0.194983f);
		o[5] 	= half2( 0.473434f , -0.480026f);
		o[6] 	= half2( 0.519456f ,  0.767022f);
		o[7] 	= half2( 0.185461f , -0.893124f);
		o[8] 	= half2( 0.507431f ,  0.064425f);
		o[9] 	= half2( 0.896420f ,  0.412458f);
		o[10] 	= half2(-0.321940f , -0.932615f);
		o[11] 	= half2(-0.791559f , -0.597710f);
		
	float3 col = 0;
	float random = frac(sin(dot(center, float2(12.9898, 78.233))) * 43758.5453);
	float4 basis = float4(rot2D(float2(1, 0), random), rot2D(float2(0, 1), random));
	
	for (int i = 0; i < 12; ++i) {
		float2 offset = o[i];
		offset = float2(dot(offset, basis.xz), dot(offset, basis.yw));
		
		float2 coord = center + offset * BUFFER_PIXEL_SIZE * DOF_BLURWIDTH;
		coord = lerp(center, coord, blurAmount);
		col += tex2Dlod(s_image, float4(coord, 0, 0)).rgb;		
	}
	return col * 0.083;
}

#elif (DOF_TYPE == 2)
// Light DOF from ReShade LightDoF.Fx fused with vanilla
float2 rot2D(float2 pos, float angle) {
	float2 source = float2(sin(angle), cos(angle));
	return float2(dot(pos, float2(source.y, -source.x)), dot(pos, source));
}


half3 dof(half3 color, float2 center)
{
	// Scale tap offsets based on render target size
	half 	depth		= tex2D(s_position,center).z;
	if (depth <= EPSDEPTH)	depth = dof_params.w;
	half	blur 		= DOFFactor(depth);

	//	const amount of blur: engine controlled
	half2 	scale 	= half2	(.5f / 1024.h, .5f / 768.h) * (dof_kernel.z * blur);
	//	amount of blur varies according to resolution
	//	but kernel size in pixels is fixed.
	//	half2 	scale 	= dof_kernel.xy * blur;

	// poisson
	half2 	o  [12];
		o[0]	= half2(-0.326212f , -0.405810f)*scale;
		o[1] 	= half2(-0.840144f , -0.073580f)*scale;
		o[2] 	= half2(-0.695914f ,  0.457137f)*scale;
		o[3] 	= half2(-0.203345f ,  0.620716f)*scale;
		o[4] 	= half2( 0.962340f , -0.194983f)*scale;
		o[5] 	= half2( 0.473434f , -0.480026f)*scale;
		o[6] 	= half2( 0.519456f ,  0.767022f)*scale;
		o[7] 	= half2( 0.185461f , -0.893124f)*scale;
		o[8] 	= half2( 0.507431f ,  0.064425f)*scale;
		o[9] 	= half2( 0.896420f ,  0.412458f)*scale;
		o[10] 	= half2(-0.321940f , -0.932615f)*scale;
		o[11] 	= half2(-0.791559f , -0.597710f)*scale;

	// sample
	half3	sum 	= color;
	half 	contrib	= 1.h;
	
	float random = frac(sin(dot(center, float2(12.9898, 78.233))) * 43758.5453);
	float4 basis = float4(rot2D(float2(1, 0), random), rot2D(float2(0, 1), random));

   	for (int i=0; i<12; i++)
	{
		float2 	tap 		= center + o[i];
		half4	tap_color	= tex2D	(s_image,tap);
		half 	tap_depth 	= tex2D	(s_position,tap).z;
		if (tap_depth <= EPSDEPTH)	tap_depth = dof_params.w;
		half 	tap_contrib	= DOFFactor(tap_depth);
		sum 		+= tap_color	* tap_contrib;
		contrib		+= tap_contrib;
	}

	return 	half3	(sum/contrib);
}

#else
// Vanilla DOF Shader w/ AA-compat tweaks

//#define MAXCOF		5.h
#define MAXCOF		7.h
half3	dof(half3 color, float2 center)
{
	// Scale tap offsets based on render target size
	half 	depth		= tex2D(s_position,center).z;
	if (depth <= EPSDEPTH)	depth = dof_params.w;
	half	blur 		= DOFFactor(depth);

	//half blur = 1;
	//	const amount of blur: define controlled
	//half2 	scale 	= half2	(.5f / 1024.h, .5f / 768.h) * MAXCOF * blur;
	//	const amount of blur: engine controlled
	half2 	scale 	= half2	(.5f / 1024.h, .5f / 768.h) * (dof_kernel.z * blur);
	//	amount of blur varies according to resolution
	//	but kernel size in pixels is fixed.
	//	half2 	scale 	= dof_kernel.xy * blur;

	// poisson
	half2 	o  [12];
		o[0]	= half2(-0.326212f , -0.405810f)*scale;
		o[1] 	= half2(-0.840144f , -0.073580f)*scale;
		o[2] 	= half2(-0.695914f ,  0.457137f)*scale;
		o[3] 	= half2(-0.203345f ,  0.620716f)*scale;
		o[4] 	= half2( 0.962340f , -0.194983f)*scale;
		o[5] 	= half2( 0.473434f , -0.480026f)*scale;
		o[6] 	= half2( 0.519456f ,  0.767022f)*scale;
		o[7] 	= half2( 0.185461f , -0.893124f)*scale;
		o[8] 	= half2( 0.507431f ,  0.064425f)*scale;
		o[9] 	= half2( 0.896420f ,  0.412458f)*scale;
		o[10] 	= half2(-0.321940f , -0.932615f)*scale;
		o[11] 	= half2(-0.791559f , -0.597710f)*scale;

	// sample
	half3	sum 	= color;
	half 	contrib	= 1.h;

   	for (int i=0; i<12; i++)
	{
		float2 	tap 		= center + o[i];
		tap = float2(dot(tap, basis.xz), dot(tap, basis.yw));		
		half4	tap_color	= tex2D	(s_image,tap);
		half 	tap_depth 	= tex2D	(s_position,tap).z;
		if (tap_depth <= EPSDEPTH)	tap_depth = dof_params.w;
		half 	tap_contrib	= DOFFactor(tap_depth);
		sum 		+= tap_color	* tap_contrib;
		contrib		+= tap_contrib;
	}

	return 	half3	(sum/contrib);
}
#endif	//	DOF_VANILLA

#endif	//	USE_DOF

#endif	//	DOF_H_INCLUDED