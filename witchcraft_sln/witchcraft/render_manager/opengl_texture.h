#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <tuple>

#include <SDL.h>
#include "GL/glew.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "../stb_image.h"

#include "../resource_manager/resource.h"

class OpenGLTexture : public qResource
{
private:
protected:					// uninit values
	int width				= -1;
	int height				= -1;
	int color_channels		= -1;
	unsigned int texture_id = 0;

	unsigned char * data = nullptr;
public:
	OpenGLTexture(){}
	~OpenGLTexture(){}

	OpenGLTexture(const char & filepath)
	{
		// get id w/ video card
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		
		// wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// file load
		data = stbi_load(&filepath, &width, &height, &color_channels, 0);
		if (data != nullptr)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			PLOGV << "Texture Loaded: " << filepath << "\n\twidth: " << width
				<< "\theight: " << height << " \tdata size: " << sizeof(data);
		}
		else
		{
			PLOGE << "'FAILURE! Texture Not Loaded: " << filepath << "\n\t" << stbi_failure_reason();
		}

		// free loaded texture (is still on video card)
		stbi_image_free(data);
	}

	unsigned int bind()
	{
		glBindTexture(GL_TEXTURE_2D, texture_id);
		return texture_id;
	}

	// width, height, color channels
	std::tuple<int, int, int> get_info()
	{
		return std::make_tuple(width, height, color_channels);
	}

};

#endif // !OPENGL_TEXTURE_H
