/*
 * Copyright (c) 2010 VMware, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * on the rights to use, copy, modify, merge, publish, distribute, sub
 * license, and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL VMWARE AND/OR THEIR SUPPLIERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file api-teximage.c
 *
 * Tests GL_EXT_texture_integer's error behavior with glTexImage2D().
 */

#include "piglit-util.h"

#define ELEMENTS(ARRAY)  (sizeof(ARRAY) / sizeof(ARRAY[0]))

int piglit_width = 10, piglit_height = 10;
int piglit_window_mode = GLUT_RGB | GLUT_ALPHA | GLUT_DOUBLE;

enum piglit_result
piglit_display(void)
{
	static const float black[4] = {0, 0, 0, 0};
	GLuint tex;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	/* Check that GL_FLOAT type is not accepted with integer formats */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16UI_EXT, 1, 1, 0,
		     GL_RGBA_INTEGER, GL_FLOAT, black);
	piglit_check_gl_error(GL_INVALID_ENUM, PIGLIT_FAIL);

	/* Check that GL_INVALID_OPERATION is generated by trying to mix
	 * integer/float formats/types.
	 */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 4, 0,
		     GL_RGBA_INTEGER, GL_SHORT, NULL);
	piglit_check_gl_error(GL_INVALID_OPERATION, PIGLIT_FAIL);

	glTexSubImage2D(GL_TEXTURE_2D, 0,
			0, 0, 4, 4,
			GL_RGBA_INTEGER, GL_FLOAT, NULL);
	piglit_check_gl_error(GL_INVALID_OPERATION, PIGLIT_FAIL);

	/* Check for GL_INVALID_OPERATION when trying to copy framebuffer pixels
	 * to an integer texture when the framebuffer is not an integer format.
	 */
	/* make valid texture image here */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16UI_EXT, 4, 4, 0,
		     GL_RGBA_INTEGER, GL_UNSIGNED_SHORT, NULL);

	glCopyTexSubImage2D(GL_TEXTURE_2D, 0,
			    0, 0, 0, 0, 4, 4);
	piglit_check_gl_error(GL_INVALID_OPERATION, PIGLIT_FAIL);

	glDeleteTextures(1, &tex);

	return PIGLIT_PASS;
}

void
piglit_init(int argc, char **argv)
{
	piglit_require_extension("GL_EXT_texture_integer");
}