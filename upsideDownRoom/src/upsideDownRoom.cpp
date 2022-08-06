#include <iostream>
#include <list>
//#include "audio/irrKlang.h"
#include "upsideDownRoom.h"

using namespace glm;
using namespace std;
//using namespace irrklang;

GLFWwindow* upsideDownRoomWindow = nullptr;

namespace upsideDownRoom {
    void createWindow() {
        upsideDownRoomWindow = glfwCreateWindow(1024, 768, "Upside down room", NULL, NULL);
        glfwMakeContextCurrent(upsideDownRoomWindow);
    }
}


    const char* getVertexShaderSource()
    {
        return
                "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;"
                "layout (location = 1) in vec3 aColor;"
                "layout (location = 2) in vec2 aUV;"
                ""
                "uniform mat4 worldMatrix;"
                "uniform mat4 viewMatrix = mat4(1.0);"
                "uniform mat4 projectionMatrix = mat4(1.0);"
                ""
                "out vec3 vertexColor;"
                "out vec2 vertexUV;"
                ""
                "void main()"
                "{"
                "   vertexColor = aColor;"
                "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
                "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
                "   vertexUV = aUV;"
                "}";
    }

    const char* getFragmentShaderSource()
    {
        return
                "#version 330 core\n"
                "in vec3 vertexColor;"
                "in vec2 vertexUV;"
                ""
                "uniform sampler2D textureSampler;"
                ""
                "out vec4 FragColor;"
                "void main()"
                "{"
                "   vec4 textureColor = texture( textureSampler, vertexUV );"
                "   FragColor = textureColor * (vertexColor,1.0);"
                "}";
    }

    const char* getTexturedVertexShaderSource()
    {
        return
                "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;"
                "layout (location = 1) in vec4 aColor;"
                "layout (location = 2) in vec2 aUV;"
                "layout (location = 3) in vec3 aNormal;"
                ""
                "uniform mat4 worldMatrix;"
                "uniform mat4 viewMatrix = mat4(1.0);"
                "uniform mat4 projectionMatrix = mat4(1.0);"
                ""
                "out vec4 vertexColor;"
                "out vec2 vertexUV;"
                "out vec3 Normal;"
                "out vec3 FragPos;"
                ""
                "void main()"
                "{"
                "   vertexColor = aColor;"
                "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
                "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
                "   FragPos = vec3(worldMatrix * vec4(aPos, 1.0));"
                "   vertexUV = aUV;"
                "   Normal = mat3(worldMatrix) * aNormal;"
                "}";
    }

    const char* getTexturedFragmentShaderSource()
    {
        return
                "#version 330 core\n"
                "in vec4 vertexColor;"
                "in vec2 vertexUV;"
                "in vec3 Normal;"
                "in vec3 FragPos;"
                ""
                "uniform sampler2D textureSampler;"
                ""
                "uniform vec3 lightPos;"
                "uniform vec3 lightColor;"
                ""
                "out vec4 FragColor;"
                "void main()"
                "{"
                ""
                "   vec3 ambient= 0.5 * lightColor;"
                ""
                "   vec3 norm = normalize(Normal);"
                "   vec3 lightDir = normalize(lightPos - FragPos);"
                "   float diff = max(dot(norm, lightDir), 0.0);"
                "   vec3 diffuse = diff * lightColor;"
                ""
                "   float specularStrength = 0.5;"
                "   vec3 viewDir = normalize(lightPos - FragPos);"
                "   vec3 reflectDir = reflect(-lightDir, norm);"
                "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);"
                "   vec3 specular = specularStrength * spec * lightColor;"
                ""
                "   vec3 objectColor = vec3(vertexColor.r,vertexColor.g,vertexColor.b);"
                "   vec4 colorResult = vec4(((ambient + diffuse + specular) * objectColor),vertexColor.a);"
                "   if(lightColor.x ==0 && lightColor.y ==0 && lightColor.z ==0){colorResult = vec4((0.5 * objectColor), vertexColor.a);}"
                "   vec4 textureColor = texture( textureSampler, vertexUV );"
                ""
                "   if (textureColor.x == 0 && textureColor.y == 0 && textureColor.z == 0 || textureColor.a < 0.1){FragColor = colorResult;}"
                "   else"
                "   FragColor = textureColor * colorResult;"
                "}";
    }

    const char* getTexturedVertexMetalSource()
    {
        return
                "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;"
                "layout (location = 1) in vec4 aColor;"
                "layout (location = 2) in vec2 aUV;"
                "layout (location = 3) in vec3 aNormal;"
                ""
                "uniform mat4 worldMatrix;"
                "uniform mat4 viewMatrix = mat4(1.0);"
                "uniform mat4 projectionMatrix = mat4(1.0);"
                ""
                "out vec4 vertexColor;"
                "out vec2 vertexUV;"
                "out vec3 Normal;"
                "out vec3 FragPos;"
                ""
                "void main()"
                "{"
                "   vertexColor = aColor;"
                "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
                "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
                "   FragPos = vec3(worldMatrix * vec4(aPos, 1.0));"
                "   vertexUV = aUV;"
                "   Normal = mat3(worldMatrix) * aNormal;"
                "}";
    }

    const char* getTexturedFragmentMetalShader()
    {
        return
                "#version 330 core\n"
                "in vec4 vertexColor;"
                "in vec2 vertexUV;"
                "in vec3 Normal;"
                "in vec3 FragPos;"
                ""
                "uniform sampler2D textureSampler;"
                ""
                "uniform vec3 lightPos;"
                "uniform vec3 lightColor;"
                ""
                "out vec4 FragColor;"
                "void main()"
                "{"
                ""
                "   vec3 ambient= vec3(0.19225f, 0.19225f, 0.19225f) * lightColor;"
                ""
                "   vec3 norm = normalize(Normal);"
                "   vec3 lightDir = normalize(lightPos - FragPos);"
                "   float diff = max(dot(norm, lightDir), 0.0);"
                "   vec3 diffuse = diff * lightColor * vec3(0.50754f, 0.50754f, 0.50754f);"
                ""
                "   vec3 viewDir = normalize(lightPos - FragPos);"
                "   vec3 reflectDir = reflect(-lightDir, norm);"
                "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 51.2);"
                "   vec3 specular = spec * lightColor * vec3(0.508273f, 0.508273f, 0.508273f);"
                ""
                "   vec3 objectColor = vec3(vertexColor.r,vertexColor.g,vertexColor.b);"
                "   vec4 colorResult = vec4(((ambient + diffuse + specular) * objectColor),vertexColor.a);"
                "   if(lightColor.x ==0 && lightColor.y ==0 && lightColor.z ==0){colorResult = vec4((vec3(0.19225f, 0.19225f, 0.19225f) * objectColor), vertexColor.a);}"
                "   vec4 textureColor = texture( textureSampler, vertexUV );"
                ""
                "   if (textureColor.x == 0 && textureColor.y == 0 && textureColor.z == 0 || textureColor.a < 0.1){FragColor = colorResult;}"
                "   else"
                "   FragColor = textureColor * colorResult;"
                "}";
    }

    int compileAndLinkShaders(const char *vertexShaderSource, const char *fragmentShaderSource) {
        // vertex shader
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // link shaders
        int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
    }

    struct TexturedColoredVertex {
        TexturedColoredVertex(vec3 _position, vec4 _color, vec2 _uv, vec3 _normal)
                : position(_position), color(_color), uv(_uv), normal(_normal) {}

        vec3 position;
        vec4 color;
        vec2 uv;
        vec3 normal;
    };

    struct texturedVertex {
        texturedVertex(vec3 _position, vec3 _color, vec2 _uv)
                : position(_position), color(_color), uv(_uv) {}

        vec3 position;
        vec3 color;
        vec2 uv;
    };

    texturedVertex ground[] = {
            texturedVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
            texturedVertex(vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),
            texturedVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

            texturedVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
            texturedVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
            texturedVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f))
    };

    texturedVertex texturedCube[] = {
            texturedVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
            texturedVertex(vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
            texturedVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),

            texturedVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
            texturedVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
            texturedVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

            texturedVertex(vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
            texturedVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
            texturedVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),

            texturedVertex(vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
            texturedVertex(vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),
            texturedVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

            texturedVertex(vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
            texturedVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
            texturedVertex(vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

            texturedVertex(vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
            texturedVertex(vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
            texturedVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

            texturedVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
            texturedVertex(vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
            texturedVertex(vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

            texturedVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
            texturedVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
            texturedVertex(vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

            texturedVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
            texturedVertex(vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
            texturedVertex(vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

            texturedVertex(vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
            texturedVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
            texturedVertex(vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),

            texturedVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
            texturedVertex(vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),
            texturedVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

            texturedVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
            texturedVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
            texturedVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f))
    };


    int createVertexBufferObject(TexturedColoredVertex *vertexArray, int arraySize) {
        // Create a vertex array
        GLuint vertexArrayObject;
        glGenVertexArrays(1, &vertexArrayObject);
        glBindVertexArray(vertexArrayObject);

        // Upload Vertex Buffer to the GPU
        GLuint vertexBufferObject;
        glGenBuffers(1, &vertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, arraySize, vertexArray, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedColoredVertex), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(TexturedColoredVertex), (void *) sizeof(vec3));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedColoredVertex), (void *) (2 * sizeof(vec3)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedColoredVertex), (void *) (3 * sizeof(vec3)));
        glEnableVertexAttribArray(3);

        return vertexArrayObject;
    }

    int createTexturedBufferObject(texturedVertex *vertexArray, int arraySize) {
        // Create a vertex array
        GLuint vertexArrayObject;
        glGenVertexArrays(1, &vertexArrayObject);
        glBindVertexArray(vertexArrayObject);

        // Upload Vertex Buffer to the GPU
        GLuint vertexBufferObject;
        glGenBuffers(1, &vertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, arraySize, vertexArray, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(texturedVertex), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(texturedVertex), (void *) sizeof(vec3));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(texturedVertex), (void *) (2 * sizeof(vec3)));
        glEnableVertexAttribArray(2);

        return vertexArrayObject;
    }

    GLuint loadTexture(const char *filename) {
        // Step1 Create and bind textures
        GLuint textureId = 0;
        glGenTextures(1, &textureId);
        assert(textureId != 0);


        glBindTexture(GL_TEXTURE_2D, textureId);

        // Step2 Set filter parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Step3 Load Textures with dimension data
        int width, height, nrChannels;
        unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
        if (!data) {
            std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
            return 0;
        }

        // Step4 Upload the texture to the GPU
        GLenum format = 0;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // Step5 Free resources
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
        return textureId;
    }

    namespace upsideDownRoom{
    void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix) {
        glUseProgram(shaderProgram);
        GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
    }
    }
    namespace upsideDownRoom {
        void setViewMatrix(int shaderProgram, mat4 viewMatrix) {
            glUseProgram(shaderProgram);
            GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
            glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        }
    }
    void setUniformMatrix(int shaderProgram, mat4 worldMatrix) {
        glUseProgram(shaderProgram);
        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    }
    namespace upsideDownRoom {
    void SetUniformVec3(GLuint shader_id, const char *uniform_name, vec3 uniform_value) {
        glUseProgram(shader_id);
        glUniform3fv(glGetUniformLocation(shader_id, uniform_name), 1, value_ptr(uniform_value));
    }
    }
    template<class T>
    void SetUniform1Value(GLuint shader_id, const char *uniform_name, T uniform_value) {
        glUseProgram(shader_id);
        glUniform1i(glGetUniformLocation(shader_id, uniform_name), uniform_value);
        glUseProgram(0);
    }

    namespace upsideDownRoom {
        void upsideDownRoomMain() {
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//

            //Disable mouse cursor
            glfwSetInputMode(upsideDownRoomWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            //Textures
            GLuint cubeTexture = loadTexture("../upsideDownRoom/assets/textures/cube.jpg");
            GLuint greenTexture = loadTexture("../upsideDownRoom/assets/textures/green.jpg");
            GLuint blueTexture = loadTexture("../upsideDownRoom/assets/textures/blue.jpg");
            GLuint redTexture = loadTexture("../upsideDownRoom/assets/textures/red.jpg");
            GLuint yellowTexture = loadTexture("../upsideDownRoom/assets/textures/yellow.jpg");
            GLuint magentaTexture = loadTexture("../upsideDownRoom/assets/textures/magenta.jpg");
            GLuint orangeTexture = loadTexture("../upsideDownRoom/assets/textures/orange.jpg");
            GLuint playTexture = loadTexture("../upsideDownRoom/assets/textures/play.png");
            GLuint winTexture = loadTexture("../upsideDownRoom/assets/textures/win.jpg");
            GLuint instructionsTexture = loadTexture("../upsideDownRoom/assets/textures/instructions.png");

            //backgroun color
            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

            //use shaders
            int texturedProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
            int texturedShaderProgram = compileAndLinkShaders(getTexturedVertexShaderSource(),
                                                              getTexturedFragmentShaderSource());

            //Camera
            vec3 cameraPosition(0.0f, 0.0f, -10.0f);
            vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
            float cameraHorizontalAngle = 0.0f;
            float cameraVerticalAngle = 0.0f;
            float fov = 45.0f;
            float cameraSpeed = 6.0f;

            //projection matrix
            mat4 projectionMatrix = perspective(radians(fov), 800.0f / 600.0f, 0.1f, 200.0f);

            //view matrix
            mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

            setViewMatrix(texturedProgram, viewMatrix);
            setViewMatrix(texturedShaderProgram, viewMatrix);

            setProjectionMatrix(texturedProgram, projectionMatrix);
            setProjectionMatrix(texturedShaderProgram, projectionMatrix);

            // Define objects
            GLuint texturedcubevao = createTexturedBufferObject(texturedCube, sizeof(texturedCube));

            //Depth Test
            glEnable(GL_DEPTH_TEST);

            //frame time
            float lastFrameTime = glfwGetTime();
            double lastMousePosX, lastMousePosY;
            glfwGetCursorPos(upsideDownRoomWindow, &lastMousePosX, &lastMousePosY);

            float dt = 0.0f, dx, dy, theta, phi, angle = 0.0f, speed = 0.0f;
            bool game = false, g = false, b = false, r = false, y = false, m = false, o = false, win = false;
            mat4 green, blue, red, yellow, magenta, orange;
            int score = 0;

            do {
                dt = glfwGetTime() - lastFrameTime;
                lastFrameTime += dt;
                angle += 180.0f * dt;
                speed += 10.0f * dt;

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                if (score > 5 && score < 10) {
                    speed += 0.01f;
                } else if (score >= 10 && score < 18) {
                    speed += 0.015f;
                } else if (score == 18) {
                    win = true;
                    game = false;
                    score = 0;
                }

                glUseProgram(texturedShaderProgram);
                glActiveTexture(GL_TEXTURE0);
                GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");

                //room =====================================================================================================================================
                glBindTexture(GL_TEXTURE_2D, cubeTexture);
                glUniform1i(textureLocation, 0);

                mat4 cubeRoom =
                        translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(15.0f, 7.0f, 30.0f));
                setUniformMatrix(texturedProgram, cubeRoom);
                glBindVertexArray(texturedcubevao);
                glDrawArrays(GL_TRIANGLES, 0, sizeof(texturedCube));

                //play screen ==============================================================================================================================
                if (game) {
                    glBindTexture(GL_TEXTURE_2D, 0);
                } else if (win) {
                    glBindTexture(GL_TEXTURE_2D, winTexture);
                } else {
                    glBindTexture(GL_TEXTURE_2D, playTexture);
                }
                glUniform1i(textureLocation, 0);

                mat4 playScreen = translate(mat4(1.0f), vec3(0.0f, 0.0f, 14.7f)) *
                                  rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 0.0f, 1.0f)) *
                                  scale(mat4(1.0f), vec3(10.0f, 5.0f, 0.5f));
                setUniformMatrix(texturedProgram, playScreen);
                glBindVertexArray(texturedcubevao);
                glDrawArrays(GL_TRIANGLES, 0, sizeof(texturedCube));

                //instructions screen ===============================================================================================================================
                glBindTexture(GL_TEXTURE_2D, instructionsTexture);
                mat4 instructionsScreen = translate(mat4(1.0f), vec3(0.0f, 0.0f, -14.7f)) *
                                          rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 0.0f, 1.0f)) *
                                          rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f)) *
                                          scale(mat4(1.0f), vec3(10.0f, 5.0f, 0.5f));
                setUniformMatrix(texturedProgram, instructionsScreen);
                glBindVertexArray(texturedcubevao);
                glDrawArrays(GL_TRIANGLES, 0, sizeof(texturedCube));

                //red line ==========================================================================================================================================
                glBindTexture(GL_TEXTURE_2D, redTexture);
                glUniform1i(textureLocation, 0);

                mat4 line =
                        translate(mat4(1.0f), vec3(0.0f, -3.4f, -5.0f)) * scale(mat4(1.0f), vec3(15.0f, 0.0f, 0.4f));
                setUniformMatrix(texturedProgram, line);
                glBindVertexArray(texturedcubevao);
                glDrawArrays(GL_TRIANGLES, 0, sizeof(texturedCube));

                //green cube =====================================================================================================================================
                glBindTexture(GL_TEXTURE_2D, greenTexture);
                glUniform1i(textureLocation, 0);

                mat4 greenCube = translate(mat4(1.0f), vec3(4.0f, -2.0f, 0.0f)) *
                                 rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) *
                                 rotate(mat4(1.0f), radians(angle), vec3(0.0f, 1.0f, 0.0f)) *
                                 scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
                setUniformMatrix(texturedProgram, greenCube);
                glBindVertexArray(texturedcubevao);
                glDrawArrays(GL_TRIANGLES, 0, sizeof(texturedCube));

                //blue cube =====================================================================================================================================
                glBindTexture(GL_TEXTURE_2D, blueTexture);
                glUniform1i(textureLocation, 0);

                mat4 blueCube = translate(mat4(1.0f), vec3(4.0f, -2.0f, -3.0f)) *
                                rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) *
                                rotate(mat4(1.0f), radians(angle), vec3(0.0f, 1.0f, 0.0f)) *
                                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
                setUniformMatrix(texturedProgram, blueCube);
                glBindVertexArray(texturedcubevao);
                glDrawArrays(GL_TRIANGLES, 0, sizeof(texturedCube));

                //red cube =====================================================================================================================================
                glBindTexture(GL_TEXTURE_2D, redTexture);
                glUniform1i(textureLocation, 0);

                mat4 redCube = translate(mat4(1.0f), vec3(4.0f, -2.0f, 3.0f)) *
                               rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) *
                               rotate(mat4(1.0f), radians(angle), vec3(0.0f, 1.0f, 0.0f)) *
                               scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
                setUniformMatrix(texturedProgram, redCube);
                glBindVertexArray(texturedcubevao);
                glDrawArrays(GL_TRIANGLES, 0, sizeof(texturedCube));

                //magenta cube =====================================================================================================================================
                glBindTexture(GL_TEXTURE_2D, magentaTexture);
                glUniform1i(textureLocation, 0);

                mat4 magentaCube = translate(mat4(1.0f), vec3(-4.0f, -2.0f, 0.0f)) *
                                   rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) *
                                   rotate(mat4(1.0f), radians(angle), vec3(0.0f, 1.0f, 0.0f)) *
                                   scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
                setUniformMatrix(texturedProgram, magentaCube);
                glBindVertexArray(texturedcubevao);
                glDrawArrays(GL_TRIANGLES, 0, sizeof(texturedCube));

                //yellow cube =====================================================================================================================================
                glBindTexture(GL_TEXTURE_2D, yellowTexture);
                glUniform1i(textureLocation, 0);

                mat4 yellowCube = translate(mat4(1.0f), vec3(-4.0f, -2.0f, 3.0f)) *
                                  rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) *
                                  rotate(mat4(1.0f), radians(angle), vec3(0.0f, 1.0f, 0.0f)) *
                                  scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
                setUniformMatrix(texturedProgram, yellowCube);
                glBindVertexArray(texturedcubevao);
                glDrawArrays(GL_TRIANGLES, 0, sizeof(texturedCube));

                //orange cube =====================================================================================================================================
                glBindTexture(GL_TEXTURE_2D, orangeTexture);
                glUniform1i(textureLocation, 0);

                mat4 orangeCube = translate(mat4(1.0f), vec3(-4.0f, -2.0f, -3.0f)) *
                                  rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) *
                                  rotate(mat4(1.0f), radians(angle), vec3(0.0f, 1.0f, 0.0f)) *
                                  scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
                setUniformMatrix(texturedProgram, orangeCube);
                glBindVertexArray(texturedcubevao);
                glDrawArrays(GL_TRIANGLES, 0, sizeof(texturedCube));

                //game ==========================================================================================================================================================================================================================================================================
                if (game) {
                    if (g) {
                        glBindTexture(GL_TEXTURE_2D, greenTexture);
                        glUniform1i(textureLocation, 0);

                        green = translate(mat4(1.0f), vec3(0.0f, 0.5f, 13.0f - speed)) *
                                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
                        setUniformMatrix(texturedProgram, green);
                        glBindVertexArray(texturedcubevao);
                        glDrawArrays(GL_TRIANGLES, 0, sizeof(texturedCube));
                    }
                    if (b) {
                        glBindTexture(GL_TEXTURE_2D, blueTexture);
                        glUniform1i(textureLocation, 0);

                        blue = translate(mat4(1.0f), vec3(2.0f, 0.1f, 13.0f - speed)) *
                               scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
                        setUniformMatrix(texturedProgram, blue);
                        glBindVertexArray(texturedcubevao);
                        glDrawArrays(GL_TRIANGLES, 0, sizeof(texturedCube));
                    }
                    if (r) {
                        glBindTexture(GL_TEXTURE_2D, redTexture);
                        glUniform1i(textureLocation, 0);

                        red = translate(mat4(1.0f), vec3(-2.0f, -0.1f, 13.0f - speed)) *
                              scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
                        setUniformMatrix(texturedProgram, red);
                        glBindVertexArray(texturedcubevao);
                        glDrawArrays(GL_TRIANGLES, 0, sizeof(texturedCube));
                    }
                    if (y) {
                        glBindTexture(GL_TEXTURE_2D, yellowTexture);
                        glUniform1i(textureLocation, 0);

                        yellow = translate(mat4(1.0f), vec3(-1.0f, -0.5f, 13.0f - speed)) *
                                 scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
                        setUniformMatrix(texturedProgram, yellow);
                        glBindVertexArray(texturedcubevao);
                        glDrawArrays(GL_TRIANGLES, 0, sizeof(texturedCube));
                    }
                    if (m) {
                        glBindTexture(GL_TEXTURE_2D, magentaTexture);
                        glUniform1i(textureLocation, 0);

                        magenta = translate(mat4(1.0f), vec3(2.0f, 1.5f, 13.0f - speed)) *
                                  scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
                        setUniformMatrix(texturedProgram, magenta);
                        glBindVertexArray(texturedcubevao);
                        glDrawArrays(GL_TRIANGLES, 0, sizeof(texturedCube));
                    }
                    if (o) {
                        glBindTexture(GL_TEXTURE_2D, orangeTexture);
                        glUniform1i(textureLocation, 0);

                        orange = translate(mat4(1.0f), vec3(-2.0f, 1.0f, 13.0f - speed)) *
                                 scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
                        setUniformMatrix(texturedProgram, orange);
                        glBindVertexArray(texturedcubevao);
                        glDrawArrays(GL_TRIANGLES, 0, sizeof(texturedCube));
                    }
                    if (speed - 13.0f >= 5) {
                        game = false;
                        win = false;
                        score = 0;
                        g = false;
                        b = false;
                        r = false;
                        y = false;
                        m = false;
                        o = false;
                    }
                }

                //green cube disappear
                if ((glfwGetKey(upsideDownRoomWindow, GLFW_KEY_G) == GLFW_PRESS) && g == true) {
                    g = false;
                    b = true;
                    score++;
                    speed = 0.0f;
                }

                //blue cube disappear
                if (glfwGetKey(upsideDownRoomWindow, GLFW_KEY_B) == GLFW_PRESS && b == true) {
                    b = false;
                    r = true;
                    score++;
                    speed = 0.0f;
                }

                //red cube disappear
                if (glfwGetKey(upsideDownRoomWindow, GLFW_KEY_R) == GLFW_PRESS && r == true) {
                    r = false;
                    y = true;
                    score++;
                    speed = 0.0f;
                }

                //yellow cube disappear
                if (glfwGetKey(upsideDownRoomWindow, GLFW_KEY_Y) == GLFW_PRESS && y == true) {
                    y = false;
                    m = true;
                    score++;
                    speed = 0.0f;
                }

                //magenta cube disappear
                if (glfwGetKey(upsideDownRoomWindow, GLFW_KEY_M) == GLFW_PRESS && m == true) {
                    m = false;
                    o = true;
                    score++;
                    speed = 0.0f;
                }

                //orange cube disappear
                if (glfwGetKey(upsideDownRoomWindow, GLFW_KEY_O) == GLFW_PRESS && o == true) {
                    o = false;
                    g = true;
                    score++;
                    speed = 0.0f;
                }

                //start game
                if (glfwGetKey(upsideDownRoomWindow, GLFW_KEY_P) == GLFW_PRESS && game != true) {
                    game = true;
                    win = false;
                    score = 0;
                    g = true;
                    speed = 0.0f;
                }

                //close upsideDownRoomWindow
                if (glfwGetKey(upsideDownRoomWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                    glfwSetWindowShouldClose(upsideDownRoomWindow, true);

                //mouse motion dx and dy
                double mousePosX, mousePosY;
                glfwGetCursorPos(upsideDownRoomWindow, &mousePosX, &mousePosY);


                dx = mousePosX - lastMousePosX;
                dy = mousePosY - lastMousePosY;

                lastMousePosX = mousePosX;
                lastMousePosY = mousePosY;

                const float cameraAngularSpeed = 60.0f;
                cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
                cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

                cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));

                theta = radians(cameraHorizontalAngle);
                phi = radians(cameraVerticalAngle);

                //new look at view
                cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
                viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, vec3(0.0f, 1.0f, 0.0f));
                vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));
                normalize(cameraSideVector);

                //move left
                if (glfwGetKey(upsideDownRoomWindow, GLFW_KEY_A) == GLFW_PRESS) {
                    cameraPosition -= cameraSideVector * dt * cameraSpeed;
                }

                //move right
                if (glfwGetKey(upsideDownRoomWindow, GLFW_KEY_D) == GLFW_PRESS) {
                    cameraPosition += cameraSideVector * dt * cameraSpeed;
                }

                //move forward
                if (glfwGetKey(upsideDownRoomWindow, GLFW_KEY_W) == GLFW_PRESS) {
                    cameraPosition += cameraLookAt * dt * cameraSpeed;
                }

                //move backward
                if (glfwGetKey(upsideDownRoomWindow, GLFW_KEY_S) == GLFW_PRESS) {
                    cameraPosition -= cameraLookAt * dt * cameraSpeed;
                }

                setViewMatrix(texturedProgram, viewMatrix);
                setViewMatrix(texturedShaderProgram, viewMatrix);

                setProjectionMatrix(texturedProgram, projectionMatrix);
                setProjectionMatrix(texturedShaderProgram, projectionMatrix);

                // End Frame
                glfwSwapBuffers(upsideDownRoomWindow);
                glfwPollEvents();
            } while (glfwGetKey(upsideDownRoomWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS);

            //    SoundEngine->stopAllSounds();
            glfwDestroyWindow(upsideDownRoomWindow);
            // Shutdown GLFW
            //glfwTerminate();
        }
    }
