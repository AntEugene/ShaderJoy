#shader vertex
varying vec4 p;
void main()
{
	gl_Position = p = gl_Vertex;
	p.z = length(p.xy);
}

#shader fragment
varying vec4 p;
uniform float iTime;
void main()
{
	vec3 color = 0.5 + 0.5 * cos(iTime + p.yxy + vec3(1,2,4));
	gl_FragColor = vec4(color, 1.0);
}
