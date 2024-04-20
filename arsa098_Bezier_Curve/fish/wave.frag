precision mediump float;
uniform sampler2D uTextureUnit;
varying vec2 vTextureCoord;
varying vec4 vVertexColor;

uniform float time; // time in seconds
uniform float waveLength; // = 100.0;
uniform float waveSpeed; // = 0.01;
uniform vec2 waveRow; // 0.0 is disable eave in row
void main()
{
	vec2 uv = vTextureCoord;
	if( waveRow.x > 0.0 )
		uv.x = uv.x + (sin(uv.y*waveLength+time)*waveSpeed);
	if( waveRow.y > 0.0 )
		uv.y = uv.y + (sin(uv.x*waveLength+time)*waveSpeed);
	gl_FragColor = texture2D( uTextureUnit, uv );
}
