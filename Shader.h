#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath)
	{
		//POZYSKANIE SHADEROW Z PLIKOW


		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		//pozwolenie na wyrzucanie blendow
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		
		//zmiana pliku txt na string(kod)
		try {
			//otworzenie plikow z kodem shaderow
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			//zczytanie plikow do zmiennych stringstream
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//zamkniecie plikow
			vShaderFile.close();
			fShaderFile.close();

			//zmiana stringstream na string z kodem shaderow
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		//w razie blendu z plikiem, wypisanie blendu
		catch (std::ifstream::failure& e) {
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();


		//COMPILACJA SHADEROW


		unsigned int vertex, fragment;
		//kompilacja vertexShadera
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		
		//kompilacja fragmentShadera
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		//tworzenie shader Programu
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		//usuniecie shaderow
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}


	//FUNKCJE


	//urzycie programu
	void use() {
		glUseProgram(ID);
	}

	//funkcja do zmiany uniformu
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void SetVec3(const std::string& name, const glm::vec3& vec) const
	{
		float f[]{ vec.x, vec.y, vec.z};
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, f);
	}

	void SetBool(const std::string& name, const bool& value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
	}
private:

	//funkcja spawdzajaca bledy
	void checkCompileErrors(unsigned int shader, std::string type) {
		int succes;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &succes);
			if (!succes)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else {
			glGetProgramiv(shader, GL_LINK_STATUS, &succes);
			if (!succes) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};
#endif // !SHADER_H
