//precision mediump float;
precision highp float;

const float PI = 3.14159265359;

uniform sampler2D uTextureUnit;
varying vec2 vTextureCoord;
varying vec4 vVertexColor;

//uniform sampler2D uSampler;
uniform vec2 uResolution;
uniform float uWeakThreshold;
uniform float uStrongThreshold;

/**
 * Apply a double threshold to each edge to classify each edge
 * as a weak edge or a strong edge
 */
float applyDoubleThreshold(
  vec2 gradient,
  float weakThreshold,
  float strongThreshold
) {
  float gradientLength = length(gradient);
  if (gradientLength < weakThreshold) return 0.;
  if (gradientLength < strongThreshold) return .5;
  return 1.;
}

/**
 * Rotate a 2D vector an angle (in degrees)
 */
vec2 rotate2D(vec2 v, float rad) {
  float s = sin(rad);
  float c = cos(rad);
  return mat2(c, s, -s, c) * v;
}

/**
 * Return a vector with the same length as v
 * but its direction is rounded to the nearest
 * of 8 cardinal directions
 */
vec2 round2DVectorAngle(vec2 v) {
  float len = length(v);
  vec2 n = normalize(v);
  float maximum = -1.;
  float bestAngle;
  for (int i = 0; i < 8; i++) {
    float theta = (float(i) * 2. * PI) / 8.;
    vec2 u = rotate2D(vec2(1., 0.), theta);
    float scalarProduct = dot(u, n);
    if (scalarProduct > maximum) {
      bestAngle = theta;
      maximum = scalarProduct;
    }
  }
  return len * rotate2D(vec2(1., 0.), bestAngle);
}

vec4 blur5(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.3333333333333333) * direction;
  color += texture2D(image, uv) * 0.29411764705882354;
  color += texture2D(image, uv + (off1 / resolution)) * 0.35294117647058826;
  color += texture2D(image, uv - (off1 / resolution)) * 0.35294117647058826;
  return color; 
}

const mat3 X_COMPONENT_MATRIX = mat3(
  1., 0., -1.,
  2., 0., -2.,
  1., 0., -1.
);
const mat3 Y_COMPONENT_MATRIX = mat3(
  1., 2., 1.,
  0., 0., 0.,
  -1., -2., -1.
);

/**
 * 3x3 Matrix convolution
 */
float convoluteMatrices(mat3 A, mat3 B) {
  return dot(A[0], B[0]) + dot(A[1], B[1]) + dot(A[2], B[2]);
}

/**
 * Get the color of a texture after
 * a Guassian blur with a radius of 5 pixels
 */
vec3 getBlurredTextureColor(
  sampler2D textureSampler,
  vec2 textureCoord,
  vec2 resolution
) {
  return blur5(
    textureSampler,
    textureCoord,
    resolution,
    normalize(textureCoord - vec2(0.5))).xyz;
}

/**
 * Get the intensity of the color on a
 * texture after a guassian blur is applied
 */
float getTextureIntensity(
  sampler2D textureSampler,
  vec2 textureCoord,
  vec2 resolution
) {
  vec3 color = getBlurredTextureColor(textureSampler, textureCoord, resolution);
  return pow(length(clamp(color, vec3(0.), vec3(1.))), 2.) / 3.;
}

/**
 * Get the gradient of the textures intensity
 * as a function of the texture coordinate
 */
vec2 getTextureIntensityGradient(
  sampler2D textureSampler,
  vec2 textureCoord,
  vec2 resolution
) {
  vec2 gradientStep = vec2(1.) / resolution;

  mat3 imgMat = mat3(0.);

  for (int i = 0; i < 3; i++) {
    //for (int j = 0; j < 3; j++) {

	int j = 0;
	vec2 ds = vec2( -gradientStep.x + (float(i) * gradientStep.x), -gradientStep.y + (float(j) * gradientStep.y));
	imgMat[i][0] = getTextureIntensity( textureSampler, clamp(textureCoord + ds, vec2(0.), vec2(1.)), resolution);

	j = 1;
	ds = vec2( -gradientStep.x + (float(i) * gradientStep.x), -gradientStep.y + (float(j) * gradientStep.y));
	imgMat[i][1] = getTextureIntensity( textureSampler, clamp(textureCoord + ds, vec2(0.), vec2(1.)), resolution);
    
	j = 2;
	ds = vec2( -gradientStep.x + (float(i) * gradientStep.x), -gradientStep.y + (float(j) * gradientStep.y));
	imgMat[i][2] = getTextureIntensity( textureSampler, clamp(textureCoord + ds, vec2(0.), vec2(1.)), resolution);
    //}
  }

  float gradX = convoluteMatrices(X_COMPONENT_MATRIX, imgMat);
  float gradY = convoluteMatrices(Y_COMPONENT_MATRIX, imgMat);

  return vec2(gradX, gradY);
}

/**
 * Get the texture intensity gradient of an image
 * where the angle of the direction is rounded to
 * one of the 8 cardinal directions and gradients
 * that are not local extrema are zeroed out
 */
vec2 getSuppressedTextureIntensityGradient(
  sampler2D textureSampler,
  vec2 textureCoord,
  vec2 resolution
) {
  vec2 gradient = getTextureIntensityGradient(textureSampler, textureCoord, resolution);
  gradient = round2DVectorAngle(gradient);
  vec2 gradientStep = normalize(gradient) / resolution;
  float gradientLength = length(gradient);
  vec2 gradientPlusStep = getTextureIntensityGradient(
    textureSampler, textureCoord + gradientStep, resolution);
  if (length(gradientPlusStep) >= gradientLength) return vec2(0.);
  vec2 gradientMinusStep = getTextureIntensityGradient(
    textureSampler, textureCoord - gradientStep, resolution);
  if (length(gradientMinusStep) >= gradientLength) return vec2(0.);
  return gradient;
}


float applyHysteresis(
  sampler2D textureSampler,
  vec2 textureCoord,
  vec2 resolution,
  float weakThreshold,
  float strongThreshold
) {
  float dx = 1. / resolution.x;
  float dy = 1. / resolution.y;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      vec2 ds = vec2(
        -dx + (float(i) * dx),
        -dy + (float(j) * dy));
      vec2 gradient = getSuppressedTextureIntensityGradient(
        textureSampler, clamp(textureCoord + ds, vec2(0.), vec2(1.)), resolution);
      float edge = applyDoubleThreshold(gradient, weakThreshold, strongThreshold);
      if (edge == 1.) return 1.;
    }
  }
  return 0.;
}

float cannyEdgeDetection(
  sampler2D textureSampler,
  vec2 textureCoord,
  vec2 resolution,
  float weakThreshold,
  float strongThreshold
) {
  vec2 gradient = getSuppressedTextureIntensityGradient(textureSampler, textureCoord, resolution);
  float edge = applyDoubleThreshold(gradient, weakThreshold, strongThreshold);
  if (edge == .5) {
    edge = applyHysteresis(
      textureSampler, textureCoord, resolution, weakThreshold, strongThreshold);
  }
  return edge;
}

void main() 
{
  // returns 1. if it is an edge, 0. otherwise
  float edge = cannyEdgeDetection(
    uTextureUnit, vTextureCoord, uResolution, uWeakThreshold, uStrongThreshold);
  gl_FragColor = vec4(vec3(edge), 1.);
//*/

  // returns 1. if it is an edge, 0. otherwise
/*  vec2 grad = getTextureIntensityGradient(
    uTextureUnit, vTextureCoord, uResolution);
  gl_FragColor = vec4(vec3(length(grad)), 1.);//*/
}

