//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
static const char* vshaderRojo = "shaders/shaderRojo.vert";
static const char* vshaderAzul = "shaders/shaderAzul.vert";
static const char* vshaderVerde1 = "shaders/shaderVerde1.vert";
static const char* vshaderVerde2 = "shaders/shaderVerde2.vert";
static const char* vshaderCafe = "shaders/shaderCafe.vert";
//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
			//J
			-0.3f,	0.25f,		0.5f,			1.0f,	0.2f,	0.1f,
			-0.3f,	0.125f,		0.5f,			1.0f,	0.2f,	0.1f,
			-0.5f,	0.25f,		0.5f,			1.0f,	0.2f,	0.1f,

			-0.3f,	0.125f,		0.5f,			1.0f,	0.2f,	0.1f,
			-0.5f,	0.25f,		0.5f,			1.0f,	0.2f,	0.1f,
			-0.5f,	0.125f,		0.5f,			1.0f,	0.2f,	0.1f,

			-0.4f,	0.125f,		0.5f,			1.0f,	0.2f,	0.1f,
			-0.35f, 0.125f,		0.5f,			1.0f,	0.2f,	0.1f,
			-0.4f,	-0.25f,		0.5f,			1.0f,	0.2f,	0.1f,
	
			-0.35f,	0.125f,		0.5f,			1.0f,	0.2f,	0.1f,
			-0.4f,	-0.25f,		0.5f,			1.0f,	0.2f,	0.1f,
			-0.35f,	-0.25f,		0.5f,			1.0f,	0.2f,	0.1f,
	
			-0.4f,	-0.25f,		0.5f,			1.0f,	0.2f,	0.1f,
			-0.4f,	-0.2f,		0.5f,			1.0f,	0.2f,	0.1f,
			-0.55f, -0.25f,		0.5f,			1.0f,	0.2f,	0.1f,

			-0.4f,	-0.2f,		0.5f,			1.0f,	0.2f,	0.1f,
			-0.55f, -0.25f,		0.5f,			1.0f,	0.2f,	0.1f,
			-0.55f, -0.2f,		0.5f,			1.0f,	0.2f,	0.1f,

			-0.55f, -0.2f,		0.5f,			1.0f,	0.2f,	0.1f,
			-0.5f,	-0.2f,		0.5f,			1.0f,	0.2f,	0.1f,
			-0.55f,	-0.15f,		0.5f,			1.0f,	0.2f,	0.1f,

			-0.5f,	-0.2f,		0.5f,			1.0f,	0.2f,	0.1f,
			-0.55f,	-0.15f,		0.5f,			1.0f,	0.2f,	0.1f,
			-0.5f,	-0.15f,		0.5f,			1.0f,	0.2f,	0.1f,

			//A
			-0.25f,	-0.25f,		0.5f,			0.5f,	0.5f,	0.5f,
			-0.125f,-0.25f,		0.5f,			0.5f,	0.5f,	0.5f,
			-0.125f,0.25f,		0.5f,			0.5f,	0.5f,	0.5f,

			0.25f,	-0.25f,		0.5f,			0.5f,	0.5f,	0.5f,
			0.125f, -0.25f,		0.5f,			0.5f,	0.5f,	0.5f,
			0.125f,	0.25f,		0.5f,			0.5f,	0.5f,	0.5f,

			-0.125f,0.25f,		0.5f,			0.5f,	0.5f,	0.5f,
			0.125f, 0.25f,		0.5f,			0.5f,	0.5f,	0.5f,
			0.125f, 0.125f,		0.5f,			0.5f,	0.5f,	0.5f,

			-0.125f,0.25f,		0.5f,			0.5f,	0.5f,	0.5f,
			-0.125f,0.125f,		0.5f,			0.5f,	0.5f,	0.5f,
			0.125f, 0.125f,		0.5f,			0.5f,	0.5f,	0.5f,

			0.125f, 0.125f,		0.5f,			0.5f,	0.5f,	0.5f,
			0.0f,	0.125f,		0.5f,			0.5f,	0.5f,	0.5f,
			0.125f, 0.0f,		0.5f,			0.5f,	0.5f,	0.5f,

			0.0f,	0.125f,		0.5f,			0.5f,	0.5f,	0.5f,
			-0.125f,0.125f,		0.5f,			0.5f,	0.5f,	0.5f,
			-0.125f,0.0f,		0.5f,			0.5f,	0.5f,	0.5f,

			-0.125f,0.0f,		0.5f,			0.5f,	0.5f,	0.5f,
			0.125f, 0.0f,		0.5f,			0.5f,	0.5f,	0.5f,
			-0.125f,-0.125f,	0.5f,			0.5f,	0.5f,	0.5f,

			0.125f, -0.125f,	0.5f,			0.5f,	0.5f,	0.5f,
			0.125f, 0.0f,		0.5f,			0.5f,	0.5f,	0.5f,
			-0.125f,-0.125f,	0.5f,			0.5f,	0.5f,	0.5f,

			//P
			0.3f,	-0.25f,		0.5f,			0.8f,	0.5f,	1.0f,
			0.425f, -0.25f,		0.5f,			0.8f,	0.5f,	1.0f,
			0.3f,	0.25f,		0.5f,			0.8f,	0.5f,	1.0f,

			0.425f, -0.25f,		0.5f,			0.8f,	0.5f,	1.0f,
			0.3f,	0.25f,		0.5f,			0.8f,	0.5f,	1.0f,
			0.425f, 0.25f,		0.5f,			0.8f,	0.5f,	1.0f,

			0.425f, 0.25f,		0.5f,			0.8f,	0.5f,	1.0f,
			0.425f, 0.2f,		0.5f,			0.8f,	0.5f,	1.0f,
			0.625f, 0.25f,		0.5f,			0.8f,	0.5f,	1.0f,

			0.425f, 0.2f,		0.5f,			0.8f,	0.5f,	1.0f,
			0.625f, 0.25f,		0.5f,			0.8f,	0.5f,	1.0f,
			0.625f, 0.2f,		0.5f,			0.8f,	0.5f,	1.0f,

			0.75f,	0.25f,		0.5f,			0.8f,	0.5f,	1.0f,
			0.625f, 0.25f,		0.5f,			0.8f,	0.5f,	1.0f,
			0.75f,	0.0f,		0.5f,			0.8f,	0.5f,	1.0f,

			0.625f, 0.25f,		0.5f,			0.8f,	0.5f,	1.0f,
			0.75f,	0.0f,		0.5f,			0.8f,	0.5f,	1.0f,
			0.625f, 0.0f,		0.5f,			0.8f,	0.5f,	1.0f,

			0.625f, 0.0f,		0.5f,			0.8f,	0.5f,	1.0f,
			0.625f, 0.05f,		0.5f,			0.8f,	0.5f,	1.0f,
			0.425f, 0.0f,		0.5f,			0.8f,	0.5f,	1.0f,

			0.625f, 0.05f,		0.5f,			0.8f,	0.5f,	1.0f,
			0.425f, 0.0f,		0.5f,			0.8f,	0.5f,	1.0f,
			0.425f, 0.05f,		0.5f,			0.8f,	0.5f,	1.0f
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,432);
	meshColorList.push_back(letras);

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		
	};
	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	GLfloat vertices_trianguloazul[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,

	};
	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);

	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.5f,	0.0f,

	};
	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};
	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);

	GLfloat vertices_cuadradorojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};
	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo);

	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,

	};
	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);
}


void CreateShaders()
{
	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	Shader *shader3 = new Shader();
	shader3->CreateFromFiles(vshaderRojo, fShader);
	shaderList.push_back(*shader3);

	Shader* shader4 = new Shader();
	shader4->CreateFromFiles(vshaderAzul, fShader);
	shaderList.push_back(*shader4);

	Shader* shader5 = new Shader();
	shader5->CreateFromFiles(vshaderVerde1, fShader);
	shaderList.push_back(*shader5);

	Shader* shader6 = new Shader();
	shader6->CreateFromFiles(vshaderVerde2, fShader);
	shaderList.push_back(*shader6);

	Shader* shader7 = new Shader();
	shader7->CreateFromFiles(vshaderCafe, fShader);
	shaderList.push_back(*shader7);
}


int main(){
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		/*LETRAS EJERCICIO1
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		//JAP
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();*/
		
		//Cubo rojo
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Piramide azul
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.35f, -2.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Cubos verdes
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.775f, -2.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.2f, -0.15f, -2.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.2f, -0.15f, -2.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Cubos cafes
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.75f, -0.83f, -4.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.75f, -0.83f, -4.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Piramides verdes
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.75f, -0.48f, -4.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.55f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.75f, -0.48f, -4.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.55f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/