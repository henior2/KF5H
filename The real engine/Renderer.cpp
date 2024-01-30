#include "Renderer.h"

void Renderer::DrawGame(std::vector<GameObject*>& GameObjects, HDC& hdc, const mat& ProjectionMatrix, const mat& ViewMatrix, const float& width, const float& height) {
	for (GameObject* i : GameObjects) {
		if (i->Stage[i->activeStage].doVerex) {
			DrawObject(i->Stage[i->activeStage].verticies, hdc, i->Transform, ProjectionMatrix, ViewMatrix, width / 2.0f, height / 2.0f);
		}
		else {
			DrawObject(ModelMenager::ObjectsDatas[i->Stage[i->activeStage].name], hdc, i->Transform, ProjectionMatrix, ViewMatrix, width / 2.0f, height / 2.0f);
		}
	}
}

void Renderer::DrawObject(VertexData Data, HDC& hdc, Transformations Model, const mat& ProjectionMatrix, const mat& ViewMatrix, const float& width, const float& height) {
	mat ModelMatrix = mat(1, 4);
	ModelMatrix = ModelMatrix * ModelMatrix.Translate(Model.position);
	ModelMatrix = ModelMatrix * ModelMatrix.Rotate(vec(1, 0, 0) * Kmath::Radians(Model.orientation.x));
	ModelMatrix = ModelMatrix * ModelMatrix.Rotate(vec(0, 1, 0) * Kmath::Radians(Model.orientation.y));
	ModelMatrix = ModelMatrix * ModelMatrix.Rotate(vec(0, 0, 1) * Kmath::Radians(Model.orientation.z));
	ModelMatrix = ModelMatrix * ModelMatrix.Scale(Model.scale);
	//fix creating vec
	for (int i = 0; i < Data.iNum; i ++) {
		unsigned int ind = Data.indecies[i * 2];
		unsigned int ind2 = Data.indecies[i * 2 + 1];
		float vx = Data.vertecies[ind * 6];
		float vy = Data.vertecies[ind * 6 + 1];
		float vz = Data.vertecies[ind * 6 + 2];
		float vx1 = Data.vertecies[ind2 * 6];
		float vy2 = Data.vertecies[ind2 * 6 + 1];
		float vz2 = Data.vertecies[ind2 * 6 + 2];

		vec pos1 = vec(vx, vy, vz);
		vec pos2 = vec(vx1, vy2, vz2);

		vec posS1 = vec(100, 4);
		vec posS2 = vec(100, 4);

		posS1 = ProjectionMatrix * ViewMatrix * ModelMatrix * pos1;
		posS2 = ProjectionMatrix * ViewMatrix * ModelMatrix * pos2;

		posS1 = pos1 * ModelMatrix;
		posS2 = pos2 * ModelMatrix;

		posS1 = posS1 * ViewMatrix;
		posS2 = posS2 * ViewMatrix;

		posS1 = posS1 * ProjectionMatrix;
		posS2 = posS2 * ProjectionMatrix;

		posS1.x = (posS1.x * width) + width;
		posS1.y = (-posS1.y * height) + height;

		posS2.x = (posS2.x * width) + width;
		posS2.y = (-posS2.y * height) + height;


		MoveToEx(hdc, posS1.x,  posS1.y, NULL);
		LineTo(hdc, posS2.x, posS2.y);
	}
}