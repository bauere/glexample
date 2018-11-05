#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <unistd.h>

#include "hitbox.h"
#include "h7_gl.h"
#include "h7_sdl.h"

PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLBINDBUFFERARBPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLISPROGRAMPROC glIsProgram;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLISSHADERPROC glIsShader;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation;

int
initgl_w32(SDL_GLContext *glctxp, SDL_Window *winp)
{
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE))
		return 1;
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3))
		return 1;
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3))
		return 1;
	if (SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8))
		return 1;
	*glctxp = SDL_GL_CreateContext(winp);
	if (!*glctxp)
		return 1;
	if (!(glGenBuffers = (PFNGLGENBUFFERSPROC)SDL_GL_GetProcAddress("glGenBuffers")))
		return 1;
	if (!(glCreateProgram = (PFNGLCREATEPROGRAMPROC)SDL_GL_GetProcAddress("glCreateProgram")))
		return 1;
	if (!(glCreateShader = (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader")))
		return 1;
	if (!(glShaderSource = (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource")))
		return 1;
	if (!(glCompileShader = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader")))
		return 1;
	if (!(glGetShaderiv = (PFNGLGETSHADERIVPROC)SDL_GL_GetProcAddress("glGetShaderiv")))
		return 1;
	if (!(glAttachShader = (PFNGLATTACHSHADERPROC)SDL_GL_GetProcAddress("glAttachShader")))
		return 1;
	if (!(glLinkProgram = (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram")))
		return 1;
	if (!(glGetProgramiv = (PFNGLGETPROGRAMIVPROC)SDL_GL_GetProcAddress("glGetProgramiv")))
		return 1;
	if (!(glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)SDL_GL_GetProcAddress("glGetAttribLocation")))
		return 1;
	if (!(glBindBuffer = (PFNGLBINDBUFFERARBPROC)SDL_GL_GetProcAddress("glBindBuffer")))
		return 1;
	if (!(glBufferData = (PFNGLBUFFERDATAPROC)SDL_GL_GetProcAddress("glBufferData")))
		return 1;
	if (!(glIsProgram = (PFNGLISPROGRAMPROC)SDL_GL_GetProcAddress("glIsProgram")))
		return 1;
	if (!(glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)SDL_GL_GetProcAddress("glGetProgramInfoLog")))
		return 1;
	if (!(glIsShader = (PFNGLISSHADERPROC)SDL_GL_GetProcAddress("glIsShader")))
		return 1;
	if (!(glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress("glGetShaderInfoLog")))
		return 1;
	if (!(glUseProgram = (PFNGLUSEPROGRAMPROC)SDL_GL_GetProcAddress("glUseProgram")))
		return 1;
	if (!(glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)SDL_GL_GetProcAddress("glEnableVertexAttribArray")))
		return 1;
	if (!(glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)SDL_GL_GetProcAddress("glVertexAttribPointer")))
		return 1;
	if (!(glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glDisableVertexAttribArray")))
		return 1;
	if (!(glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)SDL_GL_GetProcAddress("glGenVertexArrays")))
		return 1;
	if (!(glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)SDL_GL_GetProcAddress("glBindVertexArray")))
		return 1;
	if (!(glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)SDL_GL_GetProcAddress("glBindFragDataLocation")))
		return 1;
	return 0;
}

int
setvsync(int interval)
{
	if (interval > 1 || interval < -1)
		return 1;
	if (SDL_GL_SetSwapInterval(interval))
		return 1;
	return 0;
}

int
mkshader()
{
	GLuint progid;
	GLuint vertexshader;
	GLuint fragmentshader;
	GLint compstatus;
	GLint progstatus;
	GLuint vbo;
	GLuint ibo;
	GLuint vao;
	GLint vertexpos;
	progid = glCreateProgram();
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *vertexshadersrc[] = {
		"#version 150\nin vec2 LVertexPos2D; in vec3 color; out vec3 Color; void main() { Color = color; gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
	};
	glShaderSource(vertexshader, 1, vertexshadersrc, NULL);
	glCompileShader(vertexshader);
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &compstatus);
	if (compstatus != GL_TRUE)
		return 1;
	glAttachShader(progid, vertexshader);
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar *fragmentshadersrc[] = {
		"#version 150\nin vec3 Color; out vec4 LFragment; void main() { LFragment = vec4( Color, 1.0 ); }"
	};
	glShaderSource(fragmentshader, 1, fragmentshadersrc, NULL);
	glCompileShader(fragmentshader);
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &compstatus);
	if (compstatus != GL_TRUE)
		return 1;
	glAttachShader(progid, fragmentshader);
	glLinkProgram(progid);
	glGetProgramiv(progid, GL_LINK_STATUS, &progstatus);
	if (progstatus != GL_TRUE)
		return 1;
	glClearColor(0, 0, 0, 0);
	GLfloat vertexdata[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f
	};
	GLuint indexdata[] = {0, 1, 2, 3};
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexdata, GL_STATIC_DRAW);
	glGenBuffers(1, &ibo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexdata, GL_STATIC_DRAW);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);



	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(progid);
	vertexpos = glGetAttribLocation(progid, "LVertexPos2D");
	if (vertexpos == -1)
		return 1;
	glEnableVertexAttribArray(vertexpos);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(vertexpos, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), NULL);
	GLint colattrib = glGetAttribLocation(progid, "color");
	glEnableVertexAttribArray(colattrib);
	glVertexAttribPointer(colattrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void *)(2*sizeof(float)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);
	glDrawArrays(GL_POINTS, 0 , 4);
	return 0;
}

int
mkprog(GLuint *progidp)
{
	GLuint vshaderid;
	GLuint fshaderid;
	const GLchar *vshadersrc[1];
	const GLchar *fshadersrc[1];
	GLint status;

	*progidp = glCreateProgram();
	vshaderid = glCreateShader(GL_VERTEX_SHADER);
	fshaderid = glCreateShader(GL_FRAGMENT_SHADER);
	vshadersrc[0] = "#version 150\nin vec2 LVertexPos2D; void main() {gl_Position = vec4( LVertexPos2D.x/2, LVertexPos2D.y/2, 0, 1 );}";
	fshadersrc[0] = "#version 150\nout vec4 LFragment; void main() { LFragment = vec4(1.0, 1.0, 1.0, 1.0 );}";
	glShaderSource(vshaderid, 1, vshadersrc, NULL);
	glCompileShader(vshaderid);
	glGetShaderiv(vshaderid, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
		return 1;
	glShaderSource(fshaderid, 1, fshadersrc, NULL);
	glCompileShader(fshaderid);
	glGetShaderiv(fshaderid, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
		return 1;
	glAttachShader(*progidp, vshaderid);
	glAttachShader(*progidp, fshaderid);
	glLinkProgram(*progidp);
	glGetProgramiv(*progidp, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
		return 1;
	glUseProgram(*progidp);
	return 0;
}

int
renderhitbox(GLuint progid, hitbox_t *hbp)
{
	GLuint vboid;
	GLuint vaoid;
	GLint vertexattr;
	glGenVertexArrays(1, &vaoid);
	glBindVertexArray(vaoid);
	glGenBuffers(1, &vboid);
	glBindBuffer(GL_ARRAY_BUFFER, vboid);

	vertexattr = glGetAttribLocation(progid, "LVertexPos2D");
	if (vertexattr == -1)
		return 1;
	//glClear(GL_COLOR_BUFFER_BIT);
	glEnableVertexAttribArray(vertexattr);
	glBindBuffer(GL_ARRAY_BUFFER, vboid);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), hbp->posarr, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexattr, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2, NULL);
	glDrawArrays(GL_TRIANGLE_FAN, 0 , 4);
	return 0;
}

int
renderframe(SDL_Window *winp)
{
	SDL_GL_SwapWindow(winp);
	glClear(GL_COLOR_BUFFER_BIT);
}