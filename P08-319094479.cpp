/*
Práctica 8: Iluminación 2
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dadoOTexture;

Model Kitt_M;
Model Blackhawk_M;
Model Carro;
Model Carro_Cofre;
Model Carro_Rueda;
Model Lampara;
Model Cubo;


Skybox skybox;
float apertura;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[2];
PointLight pointLights2[2];
SpotLight spotLights[4];
SpotLight spotLights2[4];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	unsigned int indices_octaedro[] = {
			0,1,2,
			3,4,5,
			6,7,8,
			9,10,11,
			12,13,14,
			15,16,17,
			18,19,20,
			21,22,23
	};

	GLfloat vertices_octaedro[] = {
		//Arriba frente (2)
		0.0f, 0.0f, 0.0f,	0.58f, 0.34f,		-1.0f, -1.0f, -1.0f,
		1.0f, 0.0f, 0.0f,	0.84f, 0.34f,		-1.0f, -1.0f, -1.0f,
		0.5f, 1.0f, -0.5f,	0.715f, 0.66f,		-1.0f, -1.0f, -1.0f,

		//Arriba derecha (1)
		1.0f, 0.0f, 0.0f,	0.86f, 0.34f,		-1.0f, -1.0f, -1.0f,
		1.0f, 0.0f, -1.0f,	0.99f, 0.66f,		-1.0f, -1.0f, -1.0f,
		0.5f, 1.0f,-0.5f,	0.72f, 0.66f,		-1.0f, -1.0f, -1.0f,

		//Arriba atras (4)
		1.0f, 0.0f, -1.0f,	0.57f, 0.99f,		1.0f, -1.0f, 1.0f,
		0.0f, 0.0f, -1.0f,	0.45f, 0.67f,		1.0f, -1.0f, 1.0f,
		0.5f, 1.0f,-0.5f,	0.7f, 0.67f,		1.0f, -1.0f, 1.0f,

		//Arriba izquierda (3)
		0.0f, 0.0f, -1.0f,	0.44f, 0.66f,		1.0f, -1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,	0.57f, 0.34f,		1.0f, -1.0f, 1.0f,
		0.5f, 1.0f,-0.5f,	0.7f, 0.66f,		1.0f, -1.0f, 1.0f,

		//Abajo frente (5)
		0.0f, 0.0f, 0.0f,	0.56f, 0.32f,		-1.0f, 1.0f, -1.0f,
		1.0f, 0.0f, 0.0f,	0.43f, 0.01f,		-1.0f, 1.0f, -1.0f,
		0.5f, -1.0f, -0.5f,	0.3f, 0.32f,		-1.0f, 1.0f, -1.0f,

		//Abajo derecha (8)
		1.0f, 0.0f, 0.0f,	0.01f, 0.34f,		-1.0f, 1.0f, -1.0f,
		1.0f, 0.0f, -1.0f,	0.145f, 0.66f,		-1.0f, 1.0f, -1.0f,
		0.5f, -1.0f,-0.5f,	0.28f, 0.34f,		-1.0f, 1.0f, -1.0f,

		//Abajo atras (7)
		1.0f, 0.0f, -1.0f,	0.15f, 0.66f,		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, -1.0f,	0.42f, 0.66f,		1.0f, 1.0f, 1.0f,
		0.5f, -1.0f,-0.5f,	0.28f, 0.34f,		1.0f, 1.0f, 1.0f,

		//Abajo izquierda (6)
		0.0f, 0.0f, -1.0f,	0.43f, 0.66f,		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,	0.56f, 0.34f,		1.0f, 1.0f, 1.0f,
		0.5f, -1.0f,-0.5f,	0.3f, 0.34f,		1.0f, 1.0f, 1.0f,
	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* octaedro = new Mesh();
	octaedro->CreateMesh(vertices_octaedro, indices_octaedro, 192, 24);
	meshList.push_back(octaedro);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	calcAverageNormals(indices_octaedro, 24, vertices_octaedro, 192, 8, 5);
}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dadoOTexture = Texture("Textures/DadoOctaedro.png");
	dadoOTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Carro = Model();
	Carro.LoadModel("Models/CarroBase.obj");
	Carro_Cofre = Model();
	Carro_Cofre.LoadModel("Models/CarroCofre.obj");
	Carro_Rueda = Model();
	Carro_Rueda.LoadModel("Models/Rueda.obj");
	Lampara = Model();
	Lampara.LoadModel("Models/Lampara.obj");
	Cubo = Model();
	Cubo.LoadModel("Models/CuboMario.obj");
	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	//Luz lámpara
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		4.0f, 1.0f,
		-3.0f, 5.0f, -8.0f,
		2.0f, 1.0f, 1.0f);
	pointLightCount++;

	//Luz cubo Mario
	pointLights[1] = PointLight(1.0f, 1.0f, 0.5f,
		4.0f, 1.0f,
		5.0f, 7.0f, -10.0f,
		2.0f, 1.0f, 1.0f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, //Color
		0.0f, 2.0f,								//Iluminación/Difusión
		0.0f, 0.0f, 0.0f,						//Posición
		0.0f, -1.0f, 0.0f,						//Dirección de iluminación
		1.0f, 0.0f, 0.0f,						//Coeficientes ecuación
		5.0f);									//Apertura del cono
	spotLightCount++;

	//Luz carro delantera
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,	
		1.0f, 0.5f,			
		5.0f, 10.0f, 0.0f,	
		0.0f, -5.0f, 0.0f,	
		1.0f, 0.0f, 0.0f,	
		10.0f);				
	spotLightCount++;
	
	////Luz helicoptero
	//spotLights[2] = SpotLight(0.9f, 0.9f, 0.0f,
	//	1.0f, 2.0f,
	//	5.0f, 10.0f, 0.0f,
	//	0.0f, -5.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	10.0f);
	//spotLightCount++;

	//Luz cofre
	spotLights[2] = SpotLight(0.0f, 0.9f, 0.5f,
		1.0f, 0.5f,
		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

	//Luz carro trasera
	spotLights[3] = SpotLight(0.8f, 0.6f, 1.0f,
		1.0f, 0.5f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);


		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//Luz carro delantera
		glm::vec3 carPos = glm::vec3(0.0f+mainWindow.getarticulacion1()*5.0f, 0.5f, 2.0f); 
		glm::vec3 carDir = glm::vec3(-1.0f, 0.0f, 0.0f);
		spotLights[1].SetFlash(carPos, carDir);
		////Luz helicoptero
		//glm::vec3 helPos = glm::vec3(0.0f + mainWindow.getarticulacion4(), 5.0f, 6.0f);
		//glm::vec3 helDir = glm::vec3(0.0f, -1.0f, 0.0f);
		//spotLights[2].SetFlash(helPos, helDir);
		//Luz cofre
		glm::vec3 cofPos = glm::vec3(0.3f + mainWindow.getarticulacion1()*5.0f, 1.25f, 2.0f);
		apertura = glm::radians(mainWindow.getarticulacion3());
		glm::mat4 rotacion = glm::rotate(glm::mat4(1.0f), apertura, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 frente = glm::vec3(-1.0f, 0.0f, 0.0f);
		glm::vec3 cofDir = glm::vec3(rotacion * glm::vec4(frente, 0.0f));
		spotLights[2].SetFlash(cofPos, cofDir);
		//Luz carro trasera
		glm::vec3 carPos2 = glm::vec3(9.0f + mainWindow.getarticulacion1() * 5.0f, 0.5f, 2.0f);
		glm::vec3 carDir2 = glm::vec3(1.0f, 0.0f, 0.0f);
		spotLights[3].SetFlash(carPos2, carDir2);

		pointLights2[0] = pointLights[1];
		pointLights2[1] = pointLights[0];

		spotLights2[0] = spotLights[0];
		spotLights2[1] = spotLights[3];
		spotLights2[2] = spotLights[2];
		spotLights2[3] = spotLights[1];



		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);

		if (mainWindow.getsKeys()[GLFW_KEY_1]) {
			shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
		}
		else if (mainWindow.getsKeys()[GLFW_KEY_2]) {
			shaderList[0].SetPointLights(pointLights2, pointLightCount - 1);
		}
		else if (mainWindow.getsKeys()[GLFW_KEY_3]) {
			shaderList[0].SetPointLights(pointLights, pointLightCount - 2);
		}
		else {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}

		if (mainWindow.getsKeys()[GLFW_KEY_F]) {
			shaderList[0].SetSpotLights(spotLights2, spotLightCount-1);
		}
		else {
			shaderList[0].SetSpotLights(spotLights, spotLightCount-1);
		}



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//Pared (piso)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 40.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 0.5f, 2.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::translate(model, glm::vec3(mainWindow.getarticulacion1(), 0.0f, 0.0f)); //F y G
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro.RenderModel();
		modelaux = model;

		//rueda delantera izquierda
		model = glm::translate(model, glm::vec3(-0.72f, -0.1f, 0.35f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f)); //H y J
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Rueda.RenderModel();

		//rueda delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.72f, -0.1f, -0.34f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f)); //H y J
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Rueda.RenderModel();

		//rueda trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.54f, -0.1f, 0.35f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f)); //H y J
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Rueda.RenderModel();

		//rueda trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.54f, -0.1f, -0.34f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f)); //H y J
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Rueda.RenderModel();

		//cofre
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.45f, 0.24f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f)); //K y L
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro_Cofre.RenderModel();
	

		//Helicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::translate(model, glm::vec3(mainWindow.getarticulacion4(), 0.0f, 0.0f)); //Z y X
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		//Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, 1.67f, -8.0));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();


		//CuboMario
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 7.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cubo.RenderModel();

		//Dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 7.0f, -10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoOTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
