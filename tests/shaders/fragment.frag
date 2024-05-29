#version 330

uniform float u_time;
uniform vec2 u_resolution;
uniform vec2 u_mouse;

out vec4 fragColor;

float plot(vec2 st, float pct) {
  return smoothstep(pct, sin(pct / 2.0), st.y);
}

void main() {
  vec2 st = gl_FragCoord.xy / u_resolution;

  float y = smoothstep(0.1, 0.9, st.x);

  float pct = plot(st, y);

  vec3 color = vec3(y);
  color = (1.0 - pct) * color + pct * vec3(0.0, 1.0, 0.0);

  fragColor = vec4(color, 1.0);
}