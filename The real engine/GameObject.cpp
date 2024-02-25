#include "GameObject.h"
#include <math.h>

GameObject::GameObject(vec pos, vec rot, vec sc, std::wstring file, int i)
	:index(i), DifferentColor(true)
{
	this->Transform.position = pos;
	this->Transform.orientation = rot;
	this->Transform.scale = sc;

	UpdateVectors();

	activeStage = AddStage(file);

	Object = Stage[activeStage];
}

GameObject::GameObject(vec pos, vec rot, vec sc, std::vector<float> vertecies, std::vector<unsigned int> indecies, int i)
	:index(i), DifferentColor(false)
{
	this->Transform.position = pos;
	this->Transform.orientation = rot;
	this->Transform.scale = sc;

	UpdateVectors();

	activeStage = AddStage(vertecies, indecies);

	Object = Stage[activeStage];
	Object.doVerex = true;
}

GameObject::GameObject(const GameObject* second)
	:index(second->index), DifferentColor(second->DifferentColor), Transform(second->Transform), color(second->color), Front(second->Front), Up(second->Up), Right(second->Right), activeStage(second->activeStage)
{
	for (int i = 0; i < second->Stage.size(); i++) {
		VertexData nowy;
		nowy.iNum = second->Stage[i].verticies.iNum;
		nowy.vNum = second->Stage[i].verticies.vNum;
		if (nowy.iNum > 0 && nowy.vNum > 0) {
			nowy.vertecies = new float[nowy.vNum];
			nowy.indecies = new unsigned int[nowy.iNum * 2];
		}

		for (int j = 0; j < nowy.vNum; j++) {
			nowy.vertecies[j] = second->Stage[i].verticies.vertecies[j];
		}

		for (int j = 0; j < nowy.iNum * 2; j++) {
			nowy.indecies[j] = second->Stage[i].verticies.indecies[j];
		}

		Rendering n = second->Stage[i];
		n.verticies = nowy;
		Stage.push_back(n);
	}
	Object = Stage[activeStage];
}

GameObject::~GameObject() {

}

void GameObject::Move(vec pos) {
	this->Transform.position += Right * pos.x;
	this->Transform.position += Up * pos.y;
	this->Transform.position += Front * pos.z;
}

void GameObject::MoveGlobal(vec pos) {
	this->Transform.position += pos;
}

void GameObject::MoveTo(vec pos) {
	this->Transform.position = pos;
}

void GameObject::Rotate(vec rot, float degries) {
	this->Transform.orientation += rot * degries;
	this->Transform.orientation = vec(std::fmod(this->Transform.orientation.x, 360.0f), std::fmod(this->Transform.orientation.y, 360.0f), std::fmod(this->Transform.orientation.z, 360.0f));
	UpdateVectors();
}

void GameObject::Rotate(vec degries) {
	this->Transform.orientation += degries;
	this->Transform.orientation = vec(std::fmod(this->Transform.orientation.x, 360.0f), std::fmod(this->Transform.orientation.y, 360.0f), std::fmod(this->Transform.orientation.z, 360.0f));
	UpdateVectors();
}

void GameObject::RotateTo(vec rot) {
	this->Transform.orientation = vec(std::fmod(rot.x, 360.0f), std::fmod(rot.y, 360.0f), std::fmod(rot.z, 360.0f));
	UpdateVectors();
}

void GameObject::Scale(vec scale) {
	this->Transform.scale = vec(this->Transform.scale.x * scale.x, this->Transform.scale.y * scale.y, this->Transform.scale.z * scale.z);
}

void GameObject::ScaleTo(vec scale) {
	this->Transform.scale = scale;
}


void GameObject::SetColor(vec color3) {
	DifferentColor = true;
	this->color = color3;
}

void GameObject::UnColor() {
	this->DifferentColor = false;
}

int GameObject::AddStage(std::wstring file) {

	Rendering NEW;
	NEW.name = file;

	Stage.push_back(NEW);

	return Stage.size() - 1;
}

int GameObject::AddStage(std::vector<float>verticies, std::vector<unsigned int> indecies) {
	int vNum = verticies.size();
	int iNum = indecies.size();

	float* vertexy = new float[vNum];
	unsigned int* indexy = new unsigned int[iNum];

	for (int i = 0; i < vNum; i++) {
		vertexy[i] = verticies[i];
	}
	for (int i = 0; i < iNum; i++) {
		indexy[i] = indecies[i];
	}

	VertexData d;
	d.vertecies = vertexy;
	d.indecies = indexy;
	d.iNum = iNum / 2;
	d.vNum = vNum;

	Rendering NEW;
	NEW.doVerex = true;
	NEW.verticies = d;

	Stage.push_back(NEW);

	return Stage.size() - 1;
}








// Saved for later. Not working now
/*void GameObject::UpdateVectors() {
	vec front(0, 3);
	front.x = -cos(mat::Radians(this->Transform.orientation.y - 90.0f)) * cos(mat::Radians(this->Transform.orientation.x));
	front.y = sin(mat::Radians(this->Transform.orientation.x));
	front.z = sin(mat::Radians(this->Transform.orientation.y - 90.0f)) * cos(mat::Radians(this->Transform.orientation.x));
	Front = front.Normalize();

	Right.x = cos(mat::Radians(this->Transform.orientation.z));
	Right.y = sin(mat::Radians(this->Transform.orientation.z));
	Right.z = 0.0f;
	Right = Right.Normalize();

	Up = -vec::Cross(Front, Right).Normalize();
}

void GameObject::UpdateVectors() {
	vec front(0, 3);
	front.x = -cos(Kmath::Radians(this->Transform.orientation.y - 90.0f)) * cos(Kmath::Radians(this->Transform.orientation.x));
	front.y = sin(Kmath::Radians(this->Transform.orientation.x));
	front.z = sin(Kmath::Radians(this->Transform.orientation.y - 90.0f)) * cos(Kmath::Radians(this->Transform.orientation.x));
	Front = front.Normalize();

	Up = vec(0, 1, 0);

	Right = vec::Cross(Front, Up).Normalize();

	Up.Normalize();
}*/

void GameObject::UpdateVectors() {
	vec front(0, 3);
	front.x = -cos(Kmath::Radians(this->Transform.orientation.y - 90.0f)) * cos(Kmath::Radians(this->Transform.orientation.x));
	front.y = sin(Kmath::Radians(this->Transform.orientation.x));
	front.z = sin(Kmath::Radians(this->Transform.orientation.y - 90.0f)) * cos(Kmath::Radians(this->Transform.orientation.x));
	Front = front.Normalize();

	Right.x = cos(Kmath::Radians(this->Transform.orientation.z));
	Right.y = sin(Kmath::Radians(this->Transform.orientation.z));
	Right.z = 0.0f;
	Right = Right.Normalize();

	Up = -vec::Cross(Front, Right).Normalize();
}