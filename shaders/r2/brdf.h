// Black ops BRDF
half3 fresnelSchlickRoughness(half cosTheta, half3 F0, half roughness)
{
    return F0 + (max((1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   

half a1vf( half g )
{
	return ( 0.25 * g + 0.75 );
}

half a004( half g, half vdotN )
{
	half t = min( 0.475 * g, exp2( -9.28 * vdotN ) );
	return ( t + 0.0275 ) * g + 0.015;
}

half a0r(half g, half VdotN)
{
    g = 1.0 - g;
    return ((a004(g, VdotN) - a1vf(g) * 0.04) / 0.96);
}

half2 EnvironmentBRDF( half g, half vdotN )
{
    g = 1.0 - g;
	half4 t = half4( 1.0 / 0.96, 0.475, ( 0.0275 - 0.25 * 0.04 ) / 0.96, 0.25 );
	t *= half4( g, g, g, g );
	t += half4( 0.0, 0.0, ( 0.015 - 0.75 * 0.04 ) / 0.96, 0.75 );
	half a0 = t.x * min( t.y, exp2( -9.28 * vdotN ) ) + t.z;
	half a1 = t.w;
	
	return half2(a0, a1);
}

half3 EnvironmentBRDF( half g, half vdotN, half3 color )
{
    g = 1.0 - g;
	half4 t = half4( 1.0 / 0.96, 0.475, ( 0.0275 - 0.25 * 0.04 ) / 0.96, 0.25 );
	t *= half4( g, g, g, g );
	t += half4( 0.0, 0.0, ( 0.015 - 0.75 * 0.04 ) / 0.96, 0.75 );
	half a0 = t.x * min( t.y, exp2( -9.28 * vdotN ) ) + t.z;
	half a1 = t.w;
	
	return saturate( a0 + color * ( a1 - a0 ) );
}