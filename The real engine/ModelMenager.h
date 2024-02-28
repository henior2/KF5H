#pragma once
#include<map>
#include<string>
#include"GameObject.h"
#include <Windows.h>
#include <utility>

class ModelMenager {
public:

	static std::map<std::wstring, VertexData> ObjectsDatas;

	static std::pair<unsigned int*, int> ReadUnsignedIntFile(std::wifstream file, std::wstring Name) {
		std::vector<unsigned int> indicies;

		unsigned int index = 0;

		std::wstring line;
		while (std::getline(file, line)) {
			for (int i = 0; i < line.length(); i++) {
				if (line[i] == ' ' || line[i] == '\n' || line[i] == '\0') {
					indicies.push_back(index);
					index = 0;
				}
				else if (line[i] == '/') {
					i++;
					while (line[i] != '/')
						i++;
					i++;
				}
				else
				{
					index *= 10;
					index += line[i] - '0';
				}
			}
			indicies.push_back(index);
			index = 0;
		}

		indicies.push_back(index);

		unsigned int* indicies2 = new unsigned int[indicies.size()];

		int iNum = 0;

		for (int i = 0; i < indicies.size(); i++) {
			indicies2[i] = indicies[i];
			iNum++;
		}

		return {indicies2, iNum / 2};
	}

		static void ReadVertexFile(std::wifstream file, std::wstring Name) {
		std::vector<float> vertecies;

		float vertex = 0.0f;
		bool minus = false;
		int przecinek = 0;
		bool czyPrzecinek = false;

		std::wstring line;
		while (std::getline(file, line)) {
			for (int i = 0; i < line.length(); i++) {
				if (line[i] == ' ' || line[i] == '\n' || line[i] == '\0') {
					if (minus == true) {
						vertex = -vertex;
					}
					vertecies.push_back(vertex / (float)(pow(10, przecinek)));
					minus = false;
					vertex = 0.0f;
					czyPrzecinek = false;
					przecinek = 0;
				}
				else if (line[i] == '-') {
					minus = true;
				}
				else if (line[i] == '.') {
					czyPrzecinek = true;
				}
				else if (line[i] == '/')
				{
					i++;
					while (line[i] != '/')
						i++;
					i++;
				}
				else
				{
					vertex *= 10.0f;
					vertex += line[i] - '0';
					if (czyPrzecinek == true) {
						przecinek++;
					}
				}
			}
			if (minus == true) {
				vertex = -vertex;
			}
			vertecies.push_back(vertex / (float)(pow(10, przecinek)));
			minus = false;
			vertex = 0.0f;
			czyPrzecinek = false;
			przecinek = 0;
		}
		if (minus == true) {
			vertex = -vertex;
		}
		vertecies.push_back(vertex / (float)(pow(10, przecinek)));
		minus = false;

		float* verecies2 = new float[vertecies.size()];

		int vNum = 0;

		for (int i = 0; i < vertecies.size(); i++) {
			verecies2[i] = vertecies[i];
			vNum++;
		}

		ObjectsDatas[Name].vertecies = verecies2;
		ObjectsDatas[Name].vNum = vNum;
		ObjectsDatas[Name].CreateCollision();
		float ffffff = ObjectsDatas[Name].Colision.farthestVertex;
		ffffff = ffffff;
	}

	static void ReadIndexFile(std::wifstream file, std::wstring Name) {
		std::pair<unsigned int*, int> Ind = ReadUnsignedIntFile(std::move(file), Name);
		
		ObjectsDatas[Name].indecies = Ind.first;
		ObjectsDatas[Name].iNum = Ind.second;
	}

	static void ReadMeshFile(std::wifstream file, std::wstring Name) {
		std::pair<unsigned int*, int> Mesh = ReadUnsignedIntFile(std::move(file), Name);

		ObjectsDatas[Name].Colision.Sides = Mesh.first;
		ObjectsDatas[Name].Colision.edgeSidesNumber = Mesh.second;
	}

	static void LoadModels(std::wstring folder) {
		WIN32_FIND_DATA findFileData;
		HANDLE hFind = FindFirstFile((folder + L"\\*").c_str(), &findFileData);

		if (hFind == INVALID_HANDLE_VALUE) {
			std::wcerr << L"Error finding files in the folder." << std::endl;
			return;
		}

		do {
			const std::wstring fileName = findFileData.cFileName;

			if (fileName != L"." && fileName != L"..") {

				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					// It's a directory, so call the function recursively
					LoadModels(folder + L"\\" + fileName);
				}
				else {
					size_t dotPosition = fileName.find_last_of(L".");
					std::wstring fileNameWithoutExtension = fileName.substr(0, dotPosition);
					dotPosition = fileNameWithoutExtension.find_last_of(L".");
					std::wstring extension = fileNameWithoutExtension.substr(dotPosition, fileNameWithoutExtension.length());
					fileNameWithoutExtension = fileNameWithoutExtension.substr(0, dotPosition);

					if (ObjectsDatas.find(fileNameWithoutExtension) == ObjectsDatas.end())
					{
						VertexData NEW;
						NEW.iNum = 0;
						ObjectsDatas[fileNameWithoutExtension] = NEW;
					}
					// It's a file
					std::wifstream File(folder + L"\\" + fileName);

					if (File.is_open()) {

						if (extension == L".vx") {
							ReadVertexFile(std::move(File), fileNameWithoutExtension);
						}
						else if(extension == L".ind") {
							ReadIndexFile(std::move(File), fileNameWithoutExtension);
						}
						else if(extension == L".mesh") {
							ReadMeshFile(std::move(File), fileNameWithoutExtension);
						}

						File.close();
					}
				}
			}

		} while (FindNextFile(hFind, &findFileData) != 0);

		FindClose(hFind);
	}
};