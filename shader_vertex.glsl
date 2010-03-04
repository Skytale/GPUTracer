varying vec3 eye, pos_on_plane;
varying vec3 light1;

void main(void)
{
	gl_Position = gl_Vertex;

	/*
	 * The eye is located at (0, 0, 1).
	 * Viewing direction is (0, 0, -1).
	 */
	eye = vec3(0.0, 0.0, 1.0);
	pos_on_plane = gl_Vertex.xyz;

	/* Headlight two units above the eye. */
	light1 = eye + vec3(0.0, 2.0, 0.0);

	/* TODO: Multiplication with gl_ModelViewMatrix to allow moving. */
}
