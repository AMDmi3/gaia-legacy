/*
 * gaia - opensource 3D interface to the planet
 * Copyright (C) 2006 gaia project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __GAIA__TEXTURE_H__
#define __GAIA__TEXTURE_H__

//#include "Messages.h"
#include "Exceptions.h"

#include <SDL.h>
#include <GL/gl.h>
#include <string>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <png.h>
extern "C" {
#include <jpeglib.h>
}

/**
 * Data sources for texture loading
 */
enum {
	TEXTURE_SOURCE_FILE,	///< File. Pathname is given.
	TEXTURE_SOURCE_MEM,	///< Memory. Pointer and buffer size are given.
};

struct my_jpeg_error_ptr {
	jpeg_error_mgr	mgr;
	jmp_buf		jmpbuf;
	std::exception	err;
};

struct my_png_error_ptr {
	jmp_buf		jmpbuf;
	std::exception	err;
};

/**
 * Texture Container. Handles loading textures from files (or memory buffer) and OpenGL stuff
 */
class Texture {
public:
	/**
	 * Ctor. Constructs empty texture object.
	 */
	Texture();

	/**
	 * Dtor. Destroys texture unloading it from OpenGL if necessary.
	 */
	virtual ~Texture();

	/**
	 * Selects current texture in OpenGL.
	 */
	void SetTexture();

	/**
	 * Forcibly unloads texture from OpenGL
	 */
	void Unload();

	/**
	 * Loads texture from JPEG file
	 *
	 * Usage:
	 *
	 * LoadJPEG(TEXTURE_SOURCE_FILE, char *filename);
	 * LoadJPEG(TEXTURE_SOURCE_MEM, void *buffer, size_t size);
	 */
	void LoadJPEG(int source, ...);

	/**
	 * Loads texture from PNG file
	 *
	 * See LoadJPEG
	 */
	void LoadPNG(int source, ...);

private:
	static void my_jpeg_noop(j_decompress_ptr cinfo);
	static boolean my_jpeg_fill_input_buffer(j_decompress_ptr cinfo);
	static void my_jpeg_skip_input_data(j_decompress_ptr cinfo, long num_bytes);
	static void my_jpeg_mem_src(j_decompress_ptr cinfo, void *mem, int len);
	static void my_jpeg_error_exit(j_common_ptr cinfo);

	static void my_png_read_fn(png_structp png_ptr, png_bytep data, png_size_t length);
	static void my_png_error_fn(png_structp png_ptr, png_const_charp error_msg);

protected:
	GLint	m_glInternalFormat;	///< Internal OpenGL format of texture. Determined on loading, used in glTexture2d
	GLint	m_glFormat;	///< Format of texture. Determined on loading, used in glTexture2d
	GLint	m_Width;	///< Texture width
	GLint	m_Height;	///< Texture height

	GLuint	m_ID;		///< OpenGL ID of texture
	unsigned char	*m_Pixels;	///< Store for raw rexture data (when not uploaded to OpenGL)
};

#endif
