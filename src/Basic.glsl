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

float sphereSDF(vec3 samplePoint) {
    return length(samplePoint) - 1.0;
}

float shortestDistanceToSurface(vec3 eye, vec3 marchingDirection, float start, float end) {
    float depth = start;
    for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
        float dist = sphereSDF(eye + depth * marchingDirection);
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

void main()
{
	vec3 dir = rayDirection(45.0, iResolution, gl_FragCoord);
    vec3 eye = vec3(0.0, 0.0, 5.0);
    float dist = shortestDistanceToSurface(eye, dir, MIN_DIST, MAX_DIST);
	vec3 color = 0.5 + 0.5 * cos(iTime + dist + vec3(0.1,2,4));
    
    if (dist > MAX_DIST - EPSILON) {
        // Didn't hit anything
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
	}
	else
	{
        gl_FragColor = vec4(color, 1.0);
	}
}
