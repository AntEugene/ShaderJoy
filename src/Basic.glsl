#shader vertex
varying vec4 pos;
void main()
{
	gl_Position = pos = gl_Vertex;
    pos.z = length(pos.xy);
}

#shader fragment
const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.0001;

varying vec4 pos;
uniform float iTime;
uniform vec2 iResolution;

float softMin(float a, float b, float r)
{
    float e = max(r - abs(a - b), 0.0);
    return min(a, b) - e*e*0.25 / r;
}

float FirstSphere(vec3 samplePoint) {
    return length(samplePoint) - .5;
}
// return length(samplePoint + cos(iTime)) - .3;
float SecondSphere(vec3 samplePoint) {
	return length(samplePoint + cos(iTime) - 0.5) - .4;
}

float shortestDistanceToSurface(vec3 eye, vec3 marchingDirection, float start, float end) {
    float depth = start;
    for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
        float dist = softMin(FirstSphere(eye + depth * marchingDirection), SecondSphere(.7 + eye + depth * marchingDirection), 1.0);
        if (dist < EPSILON) {
			return depth;
        }
        depth += dist;
        if (depth >= end) {
            return end;
        }
    }
    return end;
}

vec3 rayDirection(float fieldOfView, vec2 size, vec4 fragCoord) {
    vec2 xy = fragCoord.xy - size / 2.0;
    float z = size.y / tan(radians(fieldOfView) / 2.0);
    return normalize(vec3(xy, -z));
}

vec3 estimateNormal(vec3 p) {
	return normalize(vec3(
		FirstSphere(vec3(p.x + EPSILON, p.y, p.z)) - FirstSphere(vec3(p.x - EPSILON, p.y, p.z)),
		FirstSphere(vec3(p.x, p.y + EPSILON, p.z)) - FirstSphere(vec3(p.x, p.y - EPSILON, p.z)),
		FirstSphere(vec3(p.x, p.y, p.z + EPSILON)) - FirstSphere(vec3(p.x, p.y, p.z - EPSILON))
	));
}

vec3 phongContribForLight(vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 eye,
						  vec3 lightPos, vec3 lightIntensity) {
	vec3 N = estimateNormal(p);
	vec3 L = normalize(lightPos - p);
	vec3 V = normalize(eye - p);
	vec3 R = normalize(reflect(-L, N));

	float dotLN = dot(L, N);
	float dotRV = dot(R, V);

	if (dotLN < 0.0) {
		return vec3(0.0, 0.0, 0.0);
	}

	if (dotRV < 0.0) {
		return lightIntensity * (k_d * dotLN);
	}

	return lightIntensity * (k_d * dotLN + k_s * pow(dotRV, alpha));
}

vec3 phongIllumination(vec3 k_a, vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 eye) {
    const vec3 ambientLight = 0.5 * vec3(1.0, 1.0, 1.0);
    vec3 color = ambientLight * k_a;
    
    vec3 light1Pos = vec3(4.0 * sin(iTime),
                          2.0,
                          4.0 * cos(iTime));
    vec3 light1Intensity = vec3(0.4, 0.4, 0.4);
    
    color += phongContribForLight(k_d, k_s, alpha, p, eye,
                                  light1Pos,
                                  light1Intensity);
    
    vec3 light2Pos = vec3(2.0 * sin(0.37 * iTime),
                          2.0 * cos(0.37 * iTime),
                          2.0);
    vec3 light2Intensity = vec3(0.4, 0.4, 0.4);
    
    color += phongContribForLight(k_d, k_s, alpha, p, eye,
                                  light2Pos,
                                  light2Intensity);    
    return color;
}


void main()
{
	vec3 dir = rayDirection(70.0, iResolution, gl_FragCoord);
    vec3 eye = vec3(0.0, 0.0, 5.0);
    float dist = shortestDistanceToSurface(eye, dir, MIN_DIST, MAX_DIST);
	//vec3 color = 0.5 + 0.5 * cos(iTime + dist + vec3(0.1,2,4));
    
    if (dist > MAX_DIST - EPSILON) {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
		return;
	}
	
	vec3 p = eye + dist * dir;
	vec3 K_a = 0.5 + 0.5 * cos(iTime + dist + vec3(0.1,2,4));
	vec3 K_d = vec3(0.7, 0.2, 0.2);
	vec3 K_s = vec3(1.0, 1.0, 1.0);
	float shininess = 10.0;
	vec3 color = phongIllumination(K_a, K_d, K_s, shininess, p, eye);
	gl_FragColor = vec4(color, 1.0);
}
