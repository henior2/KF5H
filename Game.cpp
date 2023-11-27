#include "Game.h"

Game::Game(GLFWwindow* win, unsigned int width, unsigned int height, irrklang::ISoundEngine* SoundEngine)
    : State(Game_Menu), Keys(), window(win), SCR_WIDTH(width), SCR_HEIGHT(height), engine(SoundEngine)
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
    Objects.clear();
    nulls.clear();
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

        if (Objects[i] == NULL) {
            continue;
        }
        glBindVertexArray(this->Objects[i]->VAO);
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

        program->setMat4("model", model);

        glLineWidth(this->Objects[i]->View.lineWidth);
        glPointSize(this->Objects[i]->View.lineWidth);

        glDrawElements(GL_LINES, this->Objects[i]->View.lines, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_POINTS, 0, this->Objects[i]->View.pointsNum);
    }
}

GameObject* Game::Create(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, std::string Object) {
    int i;
    if (nulls.size() > 0) {
        i = nulls[nulls.size() - 1];
        nulls.pop_back();
    }
    else {
        i = Objects.size();
    }
    GameObject* obj = new GameObject(pos, rot, scale, Object, i);
    Objects.push_back(obj);
    return obj;
}

GameObject* Game::Create(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, float vertecies[], unsigned int indecies[]) {
    int i;
    if (nulls.size() > 0) {
        i = nulls[nulls.size() - 1];
        nulls.pop_back();
    }
    else {
        i = Objects.size();
    }
    GameObject* obj = new GameObject(pos, rot, scale, vertecies, indecies, i);
    Objects.push_back(obj);
    return obj;
}

void Game::Destroy(GameObject* obj) {
    Objects[obj->index] == NULL;
    nulls.push_back(obj->index);
    delete obj;
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