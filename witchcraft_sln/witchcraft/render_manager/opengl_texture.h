#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <tuple>

#include <SDL.h>
#include "GL/glew.h"

#include "../stb_image.h"

#include "../resource_manager/engine_resource_base.h"

class OpenGLTexture : public EngineResourceBase
{
private:
protected:					// uninit values
	int width				= -1;
	int height				= -1;
	int color_channels		= -1;
	unsigned int texture_id =  0;

	unsigned char * data = nullptr;
public:
	OpenGLTexture(){}
	~OpenGLTexture() { glDeleteTextures(1, &texture_id); }

	OpenGLTexture(const char * filepath)
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
		data = stbi_load(filepath, &width, &height, &color_channels, 0);
		if (data != nullptr)
		{
			glTexImage2D(
				GL_TEXTURE_2D		//  can also make 1d or 3d textures
				, 0					//	mipmap level (default=0)
				, GL_RGB			//	storage format
				, width				//	image width
				, height			//	image height
				, 0					//	legacy-unused
				, GL_RGB			//	source format
				, GL_UNSIGNED_BYTE	//	source format
				, data				//
			);
			glGenerateMipmap(GL_TEXTURE_2D);
			PLOGV << "Texture Loaded: " << *filepath << "\n\twidth: " << width
				<< "\theight: " << height << " \tdata size: " << sizeof(data);
		}
		else
		{
			PLOGE << "'FAILURE! Texture Not Loaded: " << *filepath << "\n\t" << stbi_failure_reason();
		}

		// free loaded texture from ram (is still on video card)
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
