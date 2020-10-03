#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

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
	~OpenGLTexture() override { glDeleteTextures(1, &texture_id); }
	OpenGLTexture(): EngineResourceBase() {}
	OpenGLTexture(const char * name, const char * filepath, int scope=0)
		: EngineResourceBase(
			  name
			, filepath
			, EResourceType::OPENGL_TEXTURE
			, scope
	)
	{
		reserve_id();
	}

	void load() override 
	{
		unload();

		glBindTexture(GL_TEXTURE_2D, texture_id);

		stbi_set_flip_vertically_on_load(true);
		data = stbi_load(filepath.c_str(), &width, &height, &color_channels, 0);
		if (data != nullptr)
		{
			if (color_channels == 4)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			glGenerateMipmap(GL_TEXTURE_2D);
			// free loaded texture from ram (is still on video card)
			stbi_image_free(data);
			PLOGV << "Texture Loaded: " << filepath << "\tsize: " << width * height * sizeof(float);
		}
		else
		{
			PLOGE << "'FAILURE! Texture Not Loaded: " << filepath << "\n\t" << stbi_failure_reason();
		}
	}
	   
	void unload() override 
	{
		if (data) { stbi_image_free(data); }
	}

	void reserve_id()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// get id w/ video card
		glGenTextures(1, &texture_id);
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		// wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}


	unsigned int bind(unsigned int slot = 0)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		return texture_id;
	}

	int get_width() const { return width; }
	int get_height() const { return height; }
	int get_channels() const { return color_channels; }
	unsigned int get_texture_id() const { return texture_id; }
};

#endif // !OPENGL_TEXTURE_H
