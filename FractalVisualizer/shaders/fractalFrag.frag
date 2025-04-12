#version 330 core

uniform float zoom;
uniform float centerX;
uniform float centerY;
uniform vec2 iResolution;
uniform float iterations;
uniform float escapeRadius;
uniform float contrast;

uniform vec4 colorStops[4];
uniform float stopPositions[3];

out vec4 fragColor;

#define LOG2 0.69314718055994530941723212145818

// [CUSTOM_UNIFORMS]

// Custom Equation Operations and Functions
// The float functions also contain the "complex" prefix for naming consistency

vec2 complexMultiply(vec2 a, vec2 b) {
    return vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}
vec2 complexMultiply(vec2 a, float b) {
    return a * b;
}
vec2 complexMultiply(float a, vec2 b) {
    return b * a;
}
float complexMultiply(float a, float b) {
    return a * b;
}

vec2 complexDivide(vec2 a, vec2 b) {
    float d = dot(b, b);
    return vec2(dot(a, b), a.y * b.x - a.x * b.y) / d;
}
vec2 complexDivide(vec2 a, float b) {
    return a / b;
}
vec2 complexDivide(float a, vec2 b) {
    float d = dot(b, b);
    return vec2(a * b.x, -a * b.y) / d;
}
float complexDivide(float a, float b) {
    return a / b;
}

vec2 complexSqrt(vec2 a) {
    float r = length(a);
    return sqrt(r) * vec2(cos(atan(a.y, a.x)/2.0), sin(atan(a.y, a.x)/2.0));
}
float complexSqrt(float a) {
    return sqrt(a);
}
vec2 complexExp(vec2 a) {
    return exp(a.x) * vec2(cos(a.y), sin(a.y));
}
float complexExp(float a) {
    return exp(a);
}
vec2 complexLog(vec2 a) {
    return vec2(log(length(a)), atan(a.y, a.x));
}
float complexLog(float a) {
    return log(a);
}
vec2 complexLn(vec2 a) {
    return vec2(log(length(a)), atan(a.y, a.x));
}
float complexLn(float a) {
    return log(a);
}
vec2 complexAbs(vec2 a) {
    return vec2(abs(a.x), abs(a.y));
}
float complexAbs(float a) {
    return abs(a);
}
vec2 complexMod(vec2 a) {
    return vec2(length(a), 0.0);
}
vec2 complexConj(vec2 a) {
    return vec2(a.x, -a.y);
}

vec2 complexPower(vec2 a, int n) {
    vec2 result = vec2(1.0, 0.0);
    for(int i=0; i<n; i++) result = complexMultiply(result, a);
    return result;
}
vec2 complexPower(vec2 a, float n) {
    float r = length(a);
    float theta = atan(a.y, a.x);
    float powerR = pow(r, n);
    return powerR * vec2(cos(n*theta), sin(n*theta));
}
vec2 complexPower(vec2 a, vec2 b) {
    return complexExp(complexMultiply(b, complexLog(a)));
}
vec2 complexPower(float n, vec2 a) {
    float logN = log(n);
    float scale = exp(a.x * logN);
    float angle = a.y * logN;
    return scale * vec2(cos(angle), sin(angle));
}
float complexPower(float a, float b) {
    return pow(a, b);
}

vec2 complexSin(vec2 a) { 
    return vec2(sin(a.x) * cosh(a.y), cos(a.x) * sinh(a.y)); 
}
vec2 complexCos(vec2 a) {
    return vec2(cos(a.x) * cosh(a.y), -sin(a.x) * sinh(a.y));
}
vec2 complexTan(vec2 a) {
    return complexDivide(complexSin(a), complexCos(a));
}

float complexSin(float a) { 
    return sin(a); 
}
float complexCos(float a) {
    return cos(a);
}
float complexTan(float a) {
    return tan(a);
}

vec2 complexSinh(vec2 a) {
    return vec2(sinh(a.x) * cos(a.y), cosh(a.x) * sin(a.y));
}
vec2 complexCosh(vec2 a) {
    return vec2(cosh(a.x) * cos(a.y), sinh(a.x) * sin(a.y));
}
vec2 complexTanh(vec2 a) {
    return complexDivide(complexSinh(a), complexCosh(a));
}

float complexSinh(float a) {
    return sinh(a);
}
float complexCosh(float a) {
    return cosh(a);
}
float complexTanh(float a) {
    return tanh(a);
}

vec2 complexAsin(vec2 a) {
    vec2 i = vec2(0.0, 1.0);
    return -i * complexLog(i*a + complexSqrt(vec2(1.0,0.0) - a*a));
}
vec2 complexAcos(vec2 a) {
    vec2 i = vec2(0.0, 1.0);
    return -i * complexLog(a + i*complexSqrt(vec2(1.0,0.0) - a*a));
}
vec2 complexAtan(vec2 a) {
    vec2 i = vec2(0.0, 1.0);
    return 0.5*i * (complexLog(vec2(1.0,0.0)-i*a) - complexLog(vec2(1.0,0.0)+i*a));
}

float complexAsin(float a) {
    return asin(a);
}
float complexAcos(float a) {
    return acos(a);
}
float complexAtan(float a) {
    return atan(a);
}

vec2 complexAsinh(vec2 a) {
    return complexLog(a + sqrt(a*a + vec2(1.0, 0.0)));
}
vec2 complexAcosh(vec2 a) {
    return complexLog(a + sqrt(a + vec2(1.0, 0.0)) * sqrt(a - vec2(1.0, 0.0)));
}
vec2 complexAtanh(vec2 a) {
    return 0.5 * (complexLog(vec2(1.0, 0.0) + a) - complexLog(vec2(1.0, 0.0) - a));
}

float complexAsinh(float a) {
    return asinh(a);
}
float complexAcosh(float a) {
    return acosh(a);
}
float complexAtanh(float a) {
    return atanh(a);
}

vec2 complexReal(vec2 a) {
    return vec2(a.x, 0.0);
}
vec2 complexImag(vec2 a) {
    return vec2(0.0, a.y);
}

// [BEGIN_CUSTOM_EQUATION]
vec2 customEquation(vec2 z, vec2 c) { return z; }
// [END_CUSTOM_EQUATION]

float getSmoothIterations() {
    highp float real = ((gl_FragCoord.x / iResolution.x - 0.5) * zoom + centerX) * 2.0;
    highp float imag = ((gl_FragCoord.y / iResolution.y - 0.5) * zoom + centerY) * 2.0;
    
    highp float constReal = real;
    highp float constImag = imag;
    
    highp float realSq = 0.0;
    highp float imagSq = 0.0;
    int initialIterations = 0;
    
    while (initialIterations < iterations) {
        realSq = real * real;
        imagSq = imag * imag;
        
        if (realSq + imagSq > escapeRadius) {
            float logZn = log(realSq + imagSq) / 2.0;
            float nu = log(logZn / LOG2) / LOG2;
            return float(initialIterations) + 1.0 - nu;
        }
        
        vec2 z = vec2(real, imag);
        vec2 c = vec2(constReal, constImag);
        vec2 nextZ;
        
        nextZ = customEquation(z, c);

        real = nextZ.x;
        imag = nextZ.y;

        ++initialIterations;
    }
    
    return float(iterations);
}

vec4 getGradientColor(float t, vec4 colors[4], float stops[3]) {
    int i;
    if (t < stops[0]) {
        i = 0;
    } else if (t < stops[1]) {
        i = 1;
    } else {
        i = 2;
    }

    float segmentStart;
    if (i == 0) {
        segmentStart = 0.0;
    } else {
        segmentStart = stops[i - 1];
    }

    float segmentEnd = stops[i];
    float factor = (t - segmentStart) / (segmentEnd - segmentStart);
    return mix(colors[i], colors[i+1], factor);
}

vec4 returnColor() {
    float smoothIter = getSmoothIterations();
    
    if (smoothIter >= float(iterations)) {
        return vec4(0.0, 0.0, 0.0, 1.0);
    }
    
    float t = smoothIter / float(iterations);
    t = pow(t, contrast);
    t = smoothstep(0.0, 1.0, t);

    return getGradientColor(t, colorStops, stopPositions);
}

void main() {
    fragColor = returnColor();
}