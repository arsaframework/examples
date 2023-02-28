precision highp float;
uniform sampler2D tex0;
varying vec2 vTextureCoord;
varying vec4 vVertexColor;

uniform float     iTime;                 // shader playback time (in seconds)

const float waveLength = 5.0;
const float waveSpeed = 0.02;
const vec2 waveRow = vec2(1.0,1.0); // 0.0 is disable wave in row
void main()
{
	vec2 uv = vTextureCoord;
	if( waveRow.x > 0.0 )
		uv.x = uv.x + (sin(uv.y*waveLength+iTime)*waveSpeed);
	if( waveRow.y > 0.0 )
		uv.y = uv.y + (sin(uv.x*waveLength+iTime)*waveSpeed);
	gl_FragColor = texture2D( tex0, uv );
}
