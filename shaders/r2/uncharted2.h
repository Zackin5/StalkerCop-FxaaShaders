half3 Uncharted2Tonemap(half3 x)
{
	return ((x*(U2_A*x+U2_C*U2_B)+U2_D*U2_E)/(x*(U2_A*x+U2_B)+U2_D*U2_F))-U2_E/U2_F;
}

half3 Uncharted_Tonemap_Main(half3 color )
{	
	// Do inital de-gamma of the game image to ensure we're operating in the correct colour range.
	if( U2_Gamma > 1.00 )
		color = pow(color,U2_Gamma);
		
	color *= U2_Exp;  // Exposure Adjustment

	float ExposureBias = 2.0f;
	half3 curr;
	
	// Do tonemapping on RGB or Luminance
	if(!U2_Lum)
		curr = Uncharted2Tonemap(ExposureBias*color);
	else
	{
		float lum = 0.2126f * color[0] + 0.7152 * color[1] + 0.0722 * color[2];
		half3 newLum = Uncharted2Tonemap(ExposureBias*lum);
		float lumScale = newLum / lum;
		curr = color*lumScale;
	}

	half3 whiteScale = 1.0f/Uncharted2Tonemap(U2_W);
	color = curr*whiteScale;
    
	// Do the post-tonemapping gamma correction
	if( U2_Gamma > 1.00 )
		color = pow(color,1/U2_Gamma);
	
	return color;
}