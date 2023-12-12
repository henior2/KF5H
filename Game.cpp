#include "Game.h"

Game::Game(GLFWwindow* win, unsigned int width, unsigned int height, irrklang::ISoundEngine* SoundEngine)
    : State(Game_Init), Keys(), window(win), SCR_WIDTH(width), SCR_HEIGHT(height), engine(SoundEngine)
{

    program = new Shader("VertexShader.txt", "FragmentShader.txt");
    camera = new Camera();
    this->ChangeState(State);
}

Game::~Game()
{
    engine->drop();
}

void Game::ChangeState(GameState state) {
    this->State = state;
    camera->perspective = true;
    engine->stopAllSounds();
    for (int i = 0; i < Objects.size(); i++) {
        delete Objects[i];
    }
    for (int i = 0; i < Teksts.size(); i++) {
		delete Teksts[i];
	}
    Teksts.clear();
    Objects.clear();
    camera->Position = glm::vec3(0.0f, 0.0f, 0.0f);
    camera->Yaw = -90.0f;
    camera->Pitch = 0.0f;
    camera->MoveCamera(FORWARD, 0.0f);
    camera->RotateCamera(0.0f, 0.0f);
    if (state == Game_Menu) {
        this->MenuInit();
    }
    else if (state == Game_Battlezone) {
        this->BattlezoneInit();
    }
    else if (state == Game_Asteroids) {
        this->AsteroidsInit();
    }
    else if (state == Game_Tempest) {
        this->TempestInit();
    }
    else if (state == Game_TEST) {
		this->TESTInit();
    }
    else if (state == Game_Init) {
        this->GameInit();
    }
}

void Game::Update(float dt)
{
    if (this->State == Game_Menu){
        this->Menu(dt);
    }else if (this->State == Game_Battlezone) {
        this->Battlezone(dt);
    }
    else if (this->State == Game_Asteroids) {
        this->Asteroids(dt);
    }
    else if (this->State == Game_Tempest) {
        this->Tempest(dt);
    }
    else if (this->State == Game_TEST) {
        this->TEST(dt);
    }
    else if (this->State == Game_Init) {
        this->Init(dt);
    }

    this->Render(dt);
}

void Game::Render(float dt){
    glClearColor(Buffer.Red, Buffer.Green, Buffer.Blue, Buffer.Alpha);  // tworzenie bufferru
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->use();

    //tworzenie transformow
    glm::mat4 viev = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    projection = camera->GetPerspectiveMatrix();
    viev = camera->GetViewMatrix();

    //przekazanie transformow do shaderow
    program->setMat4("projection", projection);
    program->setMat4("viev", viev);

    for (int i = 0; i < Objects.size(); i++) {
        
        program->SetBool("onTop", Objects[i]->Stage[Objects[i]->activeStage].onTop);

        glBindVertexArray(this->Objects[i]->Stage[Objects[i]->activeStage].VAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->Objects[i]->Transform.position);
        model = glm::rotate(model, glm::radians(this->Objects[i]->Transform.orientation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(this->Objects[i]->Transform.orientation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(this->Objects[i]->Transform.orientation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, this->Objects[i]->Transform.scale);

        if (Objects[i]->DifferentColor == false)
            program->SetBool("DifferColor", false);
        else {
            program->SetBool("DifferColor", true);
            program->SetVec3("color", Objects[i]->color);
        }

        program->SetFloat("alpha", Objects[i]->Stage[Objects[i]->activeStage].opacity);

        program->setMat4("model", model);

        glLineWidth(this->Objects[i]->Stage[Objects[i]->activeStage].lineWidth);
        glPointSize(this->Objects[i]->Stage[Objects[i]->activeStage].lineWidth);

        glDrawElements(GL_LINES, this->Objects[i]->Stage[Objects[i]->activeStage].lines, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_POINTS, 0, this->Objects[i]->Stage[Objects[i]->activeStage].pointsNum);
    }
    
    for (int i = 0; i < Teksts.size(); i++) {
        program->SetBool("onTop", true);

        glBindVertexArray(this->Teksts[i]->Letters.VAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(this->Teksts[i]->Transform.position, 0.0f));
        model = glm::rotate(model, glm::radians(this->Teksts[i]->Transform.orientation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(this->Teksts[i]->Transform.scale, 0.0f));

        program->SetBool("DifferColor", true);
        program->SetVec3("color", Teksts[i]->color);

        program->SetFloat("alpha", this->Teksts[i]->properties.opacity);

        program->setMat4("model", model);

        glLineWidth(this->Teksts[i]->properties.lineWidth);
        glPointSize(this->Teksts[i]->properties.lineWidth);

        glDrawElements(GL_LINES, this->Teksts[i]->Letters.lines, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_POINTS, 0, this->Teksts[i]->Letters.pointsNum);
    }
}

GameObject* Game::Create(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, std::string Object) {
    int i;
    i = Objects.size();
    GameObject* obj = new GameObject(pos, rot, scale, Object, i);
    Objects.push_back(obj);
    return obj;
}

GameObject* Game::Create(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, std::vector<float> vertecies, std::vector<unsigned int> indecies) {
    int i;
    i = Objects.size();
    GameObject* obj = new GameObject(pos, rot, scale, vertecies, indecies, i);
    Objects.push_back(obj);
    return obj;
}

void Game::Destroy(GameObject* obj) {
    for (int i = obj->index + 1; i < Objects.size(); i++)
        Objects[i]->index--;
    Objects.erase(Objects.begin() + obj->index);
    delete obj;
}



Tekst2d* Game::CreateTekst(glm::vec2 pos, float rot, glm::vec2 scale, float height, float spacing, std::string tekst) {
    Tekst2d* txt = new Tekst2d(pos, rot, scale, tekst, height, spacing, Teksts.size());
    Teksts.push_back(txt);
    return txt;
}

void Game::DestroyTekst(Tekst2d* tekst) {
    for (int i = tekst->index + 1; i < Teksts.size(); i++)
        Teksts[i]->index--;
    Teksts.erase(Teksts.begin() + tekst->index);
    delete tekst;
}




void Game::ProcessInput(float dt)
{

}

void Game::PlaySound2d(const char file[], bool loop) {
   engine->play2D(file, loop);
}

void Game::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    MousePosition.x = xpos;
    MousePosition.y = ypos;
}