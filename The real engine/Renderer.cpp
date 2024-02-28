#include "Renderer.h"

void Renderer::DrawGame(std::vector<std::pair<Rendering, Transformations>>& GameObjects, HDC& hdc, const mat& ProjectionMatrix, const mat& ViewMatrix, const float& width, const float& height) {
	//HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0)); // Green color pen
	//HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	mat Pro = ProjectionMatrix * ViewMatrix;
	for (std::pair<Rendering, Transformations> i : GameObjects) {

		if (i.first.doVerex) {
			DrawObject(i.first.verticies, hdc, i.second, /*ProjectionMatrix, ViewMatrix*/ Pro, width, height, i.first.color, i.first.lineWidth, !i.first.DifferentColor, i.first.onTop);
		}
		else {
			DrawObject(ModelMenager::ObjectsDatas[i.first.name], hdc, i.second, /*ProjectionMatrix, ViewMatrix*/Pro, width, height, i.first.color, i.first.lineWidth, !i.first.DifferentColor, i.first.onTop);
		}
	}
	//SelectObject(hdc, hOldPen);
	//DeleteObject(hPen);
}

void Renderer::DrawObject(const VertexData& Data, HDC& hdc, Transformations Model, /*const mat& ProjectionMatrix, const mat& ViewMatrix*/const mat& Pro, const float& width, const float& height, const vec& color, const int& LineWidth, const bool& UseVertexColor, const bool& onTop) {

	HPEN hPen = CreatePen(PS_SOLID, LineWidth, RGB(color.x * 255, color.y * 255, color.z * 255));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

	mat ModelMatrix = mat(1, 4);
	mat Translate = ModelMatrix.Translate(Model.position);
	ModelMatrix = ModelMatrix * Translate;
	//ModelMatrix = ModelMatrix * ModelMatrix.Rotate(vec(1, 0, 0) * Kmath::Radians(Model.orientation.x));
	//ModelMatrix = ModelMatrix * ModelMatrix.Rotate(vec(0, 1, 0) * Kmath::Radians(Model.orientation.y));																								AD ROTATE USING NORMAL MATRIX, NOT QUATERNIONS
	//ModelMatrix = ModelMatrix * ModelMatrix.Rotate(vec(0, 0, 1) * Kmath::Radians(Model.orientation.z));
	//ModelMatrix = ModelMatrix * ModelMatrix.Rotate(vec(Kmath::Radians(Model.orientation.x), Kmath::Radians(Model.orientation.y), Kmath::Radians(Model.orientation.z)));

	ModelMatrix.Rotate(Kmath::Radians(Model.orientation));
	mat Scale = ModelMatrix.Scale(Model.scale);
	ModelMatrix = ModelMatrix * Scale;
	mat Mat(4);
	if (!onTop)
		Mat = Pro * ModelMatrix;
	else
		Mat = ModelMatrix;
	for (int i = 0; i < Data.iNum; i ++) {
		unsigned int ind = Data.indecies[i * 2] * 6;
		unsigned int ind2 = Data.indecies[i * 2 + 1] * 6;
		float vx = Data.vertecies[ind];
		float vy = Data.vertecies[ind + 1];
		float vz = Data.vertecies[ind + 2];
		float vx1 = Data.vertecies[ind2];
		float vy2 = Data.vertecies[ind2 + 1];
		float vz2 = Data.vertecies[ind2 + 2];

		vec pos1 = vec(vx, vy, vz);
		vec pos2 = vec(vx1, vy2, vz2);

		vec posS1 = vec(4);
		vec posS2 = vec(4);

		posS1 = Mat * pos1;//Pro/* ProjectionMatrix * ViewMatrix*/ * ModelMatrix * pos1;
		posS2 = Mat * pos2;//Pro/*ProjectionMatrix * ViewMatrix*/ * ModelMatrix * pos2;

		for (int j = 0; j < 3; j++) {
			posS1.array[j] /= posS1.array[3];
			posS2.array[j] /= posS2.array[3];
		}

		if (UseVertexColor) {
			SelectObject(hdc, hOldPen);
			DeleteObject(hPen);
			hPen = CreatePen(PS_SOLID, LineWidth, RGB(Data.vertecies[ind + 3] * 255, Data.vertecies[ind + 4] * 255, Data.vertecies[ind + 5] * 255));
			hOldPen = (HPEN)SelectObject(hdc, hPen);

		}

		if ((posS1.z < 0 && posS2.z < 0) || (posS1.z >= 1 || posS2.z >= 1) || (posS1.x < -1 && posS2.x < -1) || (posS1.x > 1 && posS2.x > 1) || (posS1.y < -1 && posS2.y < -1) || (posS1.y > 1 && posS2.y > 1)) {
			continue;
		}

		posS1.array[0] = (posS1.array[0] * width) + width;
		posS1.array[1] = (-posS1.array[1] * height) + height;

		posS2.array[0] = (posS2.array[0] * width) + width;
		posS2.array[1] = (-posS2.array[1] * height) + height;

		MoveToEx(hdc, posS1.x, posS1.y, NULL);
		LineTo(hdc, posS2.x, posS2.y);
	}

	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}