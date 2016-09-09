// Std. Includes
#include <string>
#include <algorithm>
using namespace std;

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <common/Shader.h>
#include <common/Camera.h>
#include <common/Model.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <common/lodePNG/lodepng.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Properties
GLuint screenWidth = 900, screenHeight = 800;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
GLuint loadTexture(GLchar* path);
GLuint loadCubemap(vector<const GLchar*> faces);

// Camera
Camera camera(glm::vec3(0.0f, 3.0f, 5.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

bool normalMapping = true, inTangentSpace = true;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "VISEFDEMO", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Setup and compile our shaders
    Shader shader("../resource/shaders/advanced.vs", "../resource/shaders/advanced.frag");
    Shader modelShader("../resource/shaders/modelShader.vs", "../resource/shaders/modelShader.frag");
    Shader NMShader("../resource/shaders/NMShader.vs", "../resource/shaders/NMShader.frag");
    Shader skyboxShader("../resource/shaders/skybox.vs", "../resource/shaders/skybox.frag");

#pragma region "object_initialization"
    // Set the object data (buffers, vertex attributes)
     GLfloat cubeVertices[] = {
         // Positions          // Normals
         -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
          0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
          0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
          0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

         -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
          0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
          0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
          0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
         -0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
         -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

         -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
         -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
         -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
         -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

         -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
          0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
          0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
          0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
         -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

         -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
          0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
          0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
          0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
         -0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
         -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
    };
     GLfloat modelCube[] = {
         // Positions          // Normals           // Texture Coords
         -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
         0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
         0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
         -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
         -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

         -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
         -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

         -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

         0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

         -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

         -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
     };
    GLfloat skyboxVertices[] = {
        // Positions          
        -50.0f,  50.0f, -50.0f,
        -50.0f, -50.0f, -50.0f,
         50.0f, -50.0f, -50.0f,
         50.0f, -50.0f, -50.0f,
         50.0f,  50.0f, -50.0f,
        -50.0f,  50.0f, -50.0f,
  
        -50.0f, -50.0f,  50.0f,
        -50.0f, -50.0f, -50.0f,
        -50.0f,  50.0f, -50.0f,
        -50.0f,  50.0f, -50.0f,
        -50.0f,  50.0f,  50.0f,
        -50.0f, -50.0f,  50.0f,
  
         50.0f, -50.0f, -50.0f,
         50.0f, -50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f, -50.0f,
         50.0f, -50.0f, -50.0f,
   
        -50.0f, -50.0f,  50.0f,
        -50.0f,  50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f, -50.0f,  50.0f,
        -50.0f, -50.0f,  50.0f,
  
        -50.0f,  50.0f, -50.0f,
         50.0f,  50.0f, -50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
        -50.0f,  50.0f,  50.0f,
        -50.0f,  50.0f, -50.0f,
  
        -50.0f, -50.0f, -50.0f,
        -50.0f, -50.0f,  50.0f,
         50.0f, -50.0f, -50.0f,
         50.0f, -50.0f, -50.0f,
        -50.0f, -50.0f,  50.0f,
         50.0f, -50.0f,  50.0f
    };

    // Load textures
    //NMShader.Use();
    //GLuint cubeDiffuse = loadTexture("../resource/textures/176.png");
    //GLuint cubeNormal = loadTexture("../resource/textures/176_norm.png");
    //glUniform1i(glGetUniformLocation(NMShader.Program, "cubeDiffuse"), 1);
    //glUniform1i(glGetUniformLocation(NMShader.Program, "cubeNormal"), 2);

#pragma endregion

    // Cubemap (Skybox)
    vector<const GLchar*> faces;
    faces.push_back("../resource/skybox/cove/cove_rt.png");
    faces.push_back("../resource/skybox/cove/cove_lf.png");
    faces.push_back("../resource/skybox/cove/cove_up.png");
    faces.push_back("../resource/skybox/cove/cove_dn.png");
    faces.push_back("../resource/skybox/cove/cove_bk.png");
    faces.push_back("../resource/skybox/cove/cove_ft.png");
    GLuint skyboxTexture = loadCubemap(faces);
    //GLuint cubeDiffuse = loadTexture("../resource/textures/176.png");
    //GLuint cubeNormal = loadTexture("../resource/textures/176_norm.png");
    GLuint cubeDiffuse = loadTexture("../resource/textures/container_D.png");
    GLuint cubeSpecular = loadTexture("../resource/textures/container_S.png");
    
   // modelShader.Use();

    //GLint vertex = glGetAttribLocation(modelShader.Program, "vertex");
    //GLint normal = glGetAttribLocation(modelShader.Program, "normal");
    //GLint texcoord = glGetAttribLocation(modelShader.Program, "texcoord");
    //GLint tangent = glGetAttribLocation(modelShader.Program, "_tangent");
    //GLint bitangent = glGetAttribLocation(modelShader.Program, "_bitangent");
    //GLint _normal = glGetAttribLocation(modelShader.Program, "_normal");
    //GLint posIndex = 0;
    //GLint texCoordIndex = 1;
    //glBindAttribLocation(modelShader.Program, posIndex, "in_Position");
    //glBindAttribLocation(modelShader.Program, texCoordIndex, "in_TexCoord");

    //GLint flag = glGetUniformLocation(modelShader.Program, "flag");

    // Model cube
    GLuint VBO, containerVAO;
    glGenVertexArrays(1, &containerVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(modelCube), modelCube, GL_STATIC_DRAW);

    glBindVertexArray(containerVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    // Setup cube VAO
    GLuint cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glBindVertexArray(0);

    // Setup skybox VAO
    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);

    // Load robot using our model loader
    Model robot("../resource/EM/EM-208.obj");

    glm::vec3 lightPos(1.0f, 3.0f, 3.5f);

    // Draw as wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        Do_Movement();

        // Clear buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw scene as normal
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 model;

        /////////////////////////////////////////////////////////////////////////////
        // Render Cube
        /////////////////////////////
        // Normal mapped

        modelShader.Use();
        glm::mat4 normalCube;
        normalCube = glm::translate(normalCube, glm::vec3(0, 3, 0));
        normalCube = glm::rotate(normalCube, (GLfloat)glfwGetTime() * 0.5f, glm::normalize(glm::vec3(1.0, 1.0, 1.0)));
        view = camera.GetViewMatrix();
        projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(normalCube));
        glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(glGetUniformLocation(modelShader.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z); 
        glUniform3fv(glGetUniformLocation(modelShader.Program, "Light.position"), 1, &lightPos[0]);
        glUniform3f(glGetUniformLocation(modelShader.Program, "Light.ambient"), 0.2f, 0.2f, 0.2f);
        glUniform3f(glGetUniformLocation(modelShader.Program, "Light.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(modelShader.Program, "Light.specular"), 1.0f, 1.0f, 1.0f);
        // Set material properties
        glUniform1f(glGetUniformLocation(modelShader.Program, "Material.shininess"), 32.0f);
        //glUniform3f(glGetUniformLocation(modelShader.Program, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        //glUniform1i(flag, (int)normal);
        glBindVertexArray(containerVAO);
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(modelShader.Program, "Material.diffuse"), 0);
        glBindTexture(GL_TEXTURE_2D, cubeDiffuse);
        glActiveTexture(GL_TEXTURE2);
        glUniform1i(glGetUniformLocation(modelShader.Program, "Material.specular"), 1);
        glBindTexture(GL_TEXTURE_2D, cubeSpecular);
        //glUniform1i(glGetUniformLocation(modelShader.Program, "NormalMap"), 1);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        /////////////////////////////
        shader.Use();
        model = glm::translate(model, glm::vec3(2, 3, 0));
        model = glm::rotate(model, (GLfloat)glfwGetTime() * -0.5f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        view = camera.GetViewMatrix();
        projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(glGetUniformLocation(shader.Program, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        glBindVertexArray(cubeVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
	
        /////////////////////////////////////////////////////////////////////////////
        // Draw robot model
        NMShader.Use();
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-2, 0, 0));
        model = glm::rotate(model, (GLfloat)glfwGetTime() * -0.5f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(glGetUniformLocation(shader.Program, "lightPos"), 1, &lightPos[0]);
        glUniform3fv(glGetUniformLocation(shader.Program, "viewPos"), 1, &camera.Position[0]);
        robot.Draw(NMShader);

        /////////////////////////////////////////////////////////////////////////
        //glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
        //projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        //glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS); // Set depth function back to default
        /////////////////////////////////////////////////////////////////////////

        // Swap the buffers
        glfwSwapBuffers(window);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glfwTerminate();
    return 0;
}

// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
GLuint loadCubemap(vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);

    std::vector<unsigned char> image, png; //the raw pixels
    unsigned width, height;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for (GLuint i = 0; i < faces.size(); i++)
    {
        lodepng::load_file(png, faces[i]);
        //decode
        unsigned error = lodepng::decode(image, width, height, png);
        if (error) // lataus ei onnistu --> virheilmotus
        {
            std::cout << "PNG load failed " << error << ": " << lodepng_error_text(error) << std::endl
                << faces[i] << std::endl;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
        image.clear();
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    return textureID;
}


// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(GLchar* path)
{
    //Generate texture ID and load texture data 
    GLuint textureID;
    glGenTextures(1, &textureID);
    std::vector<unsigned char> image, png; //the raw pixels
    unsigned width, height;
    lodepng::load_file(png, path);
    unsigned error = lodepng::decode(image, width, height, png);
    if (error) // lataus ei onnistu --> virheilmotus
    {
        std::cout << "PNG load failed " << error << ": " << lodepng_error_text(error) << std::endl
            << path << std::endl;;
    }
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
    image.clear();

    return textureID;
}

#pragma region "User input"

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

#pragma endregion