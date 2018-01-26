// ==========================================================================
// Barebones OpenGL Core Profile Boilerplate
//    using the GLFW windowing system (http://www.glfw.org)
//
// Loosely based on
//  - Chris Wellons' example (https://github.com/skeeto/opengl-demo) and
//  - Camilla Berglund's example (http://www.glfw.org/docs/latest/quick.html)
//
// Author:  Sonny Chan, University of Calgary
// Co-Authors:
//			Jeremy Hart, University of Calgary
//			John Hall, University of Calgary
// Date:    December 2015
//Code inspiration for Sierpenski's Triangle:
			//(http://pages.cpsc.ucalgary.ca/~brosz/TA/453f08/lab3_fractal.cpp)
//Colors Reference: (https://www.opengl.org/discussion_boards/showthread.php/132502-Color-tables)
// ==========================================================================

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <glm-0.9.8.2/glm/glm.hpp>
#include <glm-0.9.8.2/glm/gtc/matrix_transform.hpp>
#include <glm-0.9.8.2/glm/gtc/type_ptr.hpp>


#include <glad/include/glad/glad.h>
#include <glfw/include/GLFW/glfw3.h>
#include <vector>

using namespace std;
using namespace glm;
// --------------------------------------------------------------------------
// OpenGL utility and support function prototypes

void QueryGLVersion();
bool CheckGLErrors();

string LoadSource(const string &filename);
GLuint CompileShader(GLenum shaderType, const string &source);
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

// --------------------------------------------------------------------------
//Global variables
#define GL_PI 3.1415f
int layersForUser = 1; //default starting point for layers for user
int maxLayersAllowed = 6; //Max level allowed for each question is 6 for the users
char caseNumber = '1'; 
vector<vec2> vertices;
vector<vec3> colours;

// Functions to set up OpenGL shader programs for rendering

// load, compile, and link shaders, returning true if successful
GLuint InitializeShaders()
{
	// load shader source from files
	string vertexSource = LoadSource("shaders/vertex.glsl");
	string fragmentSource = LoadSource("shaders/fragment.glsl");
	if (vertexSource.empty() || fragmentSource.empty()) return false;

	// compile shader source into shader objects
	GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	// link shader program
	GLuint program = LinkProgram(vertex, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	// check for OpenGL errors and return false if error occurred
	return program;
}

// --------------------------------------------------------------------------
// Functions to set up OpenGL buffers for storing geometry data

struct Geometry
{
	// OpenGL names for array buffer objects, vertex array object
	GLuint  vertexBuffer;
	GLuint  textureBuffer;
	GLuint  colourBuffer;
	GLuint  vertexArray;
	GLsizei elementCount;

	// initialize object names to zero (OpenGL reserved value)
	Geometry() : vertexBuffer(0), colourBuffer(0), vertexArray(0), elementCount(0)
	{}
};
Geometry geometry;

//Structure made specifically for Sierpenski Triangle
struct Coordinate
{
	float x;
	float y;
};

/*
Creating functions for questions
*/
//Question 1: Squares & Diamonds
void squareAndDiamond(vector<vec2>*vertices, vector<vec3>*colours, int layers)
{
	vertices->clear();
	colours->clear();
	float baseNeg = -0.9f;
	float basePos = 0.9f;

	//Creating the squares
	for(int i = 0; i<layers; i++)
	{
		vertices->push_back(vec2((baseNeg/pow(2,i)), (baseNeg/pow(2,i)))); //(x1,y1)
		vertices->push_back(vec2((basePos/pow(2,i)), (baseNeg/pow(2,i)))); //(x2,y2)
		colours->push_back(vec3(0.0f,0.0f,0.0f));
		colours->push_back(vec3(0.0f,0.0f,0.0f));

		vertices->push_back(vec2((basePos/pow(2,i)), (baseNeg/pow(2,i)))); //(x2,y2)
		vertices->push_back(vec2((basePos/pow(2,i)), (basePos/pow(2,i)))); // (x3,y3)
		colours->push_back(vec3(0.0f,0.0f,0.0f));
		colours->push_back(vec3(0.0f,0.0f,0.0f));

		vertices->push_back(vec2((basePos/pow(2,i)), (basePos/pow(2,i))));// (x3,y3)
		vertices->push_back(vec2((baseNeg/pow(2,i)), (basePos/pow(2,i)))); // (x4,y4)
		colours->push_back(vec3(0.0f,0.0f,0.0f));
		colours->push_back(vec3(0.0f,0.0f,0.0f));

		vertices->push_back(vec2((baseNeg/pow(2,i)), (basePos/pow(2,i)))); // (x4,y4)
		vertices->push_back(vec2((baseNeg/pow(2,i)), (baseNeg/pow(2,i)))); //(x1,y1)
		colours->push_back(vec3(0.0f,0.0f,0.0f));
		colours->push_back(vec3(0.0f,0.0f,0.0f));
	}

	//Creating Diamonds
	for(int i=0; i< layers; i++)
	{
		vertices->push_back(vec2(0.0,baseNeg/pow(2,i))); //(x1,y1)
		vertices->push_back(vec2(basePos/pow(2,i),0.0)); //(x2,y2)
		colours->push_back(vec3(0.0f,0.0f,1.0f));
		colours->push_back(vec3(0.0f,0.0f,1.0f));

		vertices->push_back(vec2(basePos/pow(2,i),0.0)); //(x2,y2)
		vertices->push_back(vec2(0.0,basePos/pow(2,i))); //(x3,y3)
		colours->push_back(vec3(0.0f,0.0f,1.0f));
		colours->push_back(vec3(0.0f,0.0f,1.0f));

		vertices->push_back(vec2(0.0,basePos/pow(2,i))); //(x3,y3)
		vertices->push_back(vec2(baseNeg/pow(2,i),0.0)); //(x4,y4)
		colours->push_back(vec3(0.0f,0.0f,1.0f));
		colours->push_back(vec3(0.0f,0.0f,1.0f));

		vertices->push_back(vec2(baseNeg/pow(2,i),0.0)); //(x4,y4)
		vertices->push_back(vec2(0.0,baseNeg/pow(2,i))); //(x1,y1)
		colours->push_back(vec3(0.0f,0.0f,1.0f));
		colours->push_back(vec3(0.0f,0.0f,1.0f));
	}

}

//Question 2: Parametric Spiral
void paraMetricSpiral(vector<vec2>*vertices, vector<vec3>*colours, int layers)
{
	vertices->clear();
	colours->clear();
	float maximumRadius = layers*2*GL_PI;
	float i;
	float gradientFactor = 50.0f; //as spiral grows, color changes 
	for(i = 0.0f; i<maximumRadius; i+=0.01f)
	{	
		vertices->push_back(vec2((i/maximumRadius) * (float) cos(i),
			(i/maximumRadius) * (float) -sin(i) ));
		colours->push_back(vec3(0.0f,0.0f,0.0f+(i/gradientFactor)));	
	}
}

//Question 3: Sierpinski Triangle
void SierpinskiRecursive(Coordinate a, Coordinate b, Coordinate c, int layer)
{
	Coordinate a1, a2, a3;
	if(layer > 1)
	{
		a1.x = (a.x + b.x)/2.0;
		a1.y = (a.y + b.y)/2.0;

		a2.x = (a.x + c.x)/2.0;
		a2.y = (a.y + c.y)/2.0;

		a3.x = (b.x + c.x)/2.0;
		a3.y = (b.y + c.y)/2.0;

		vertices.push_back(vec2(a.x,a.y));
		colours.push_back(vec3(1.0f + (layer/50) ,.0f,0.0f));
		vertices.push_back(vec2(a1.x,a1.y));
		colours.push_back(vec3(0.0f ,1.0f,0.0f+ (layer/50)));
		vertices.push_back(vec2(a2.x,a2.y));
		colours.push_back(vec3(0.0f+ (layer/50) ,.0f,1.0f));

		vertices.push_back(vec2(b.x,b.y));
		colours.push_back(vec3(0.0f,0.0f+ (layer/50) ,1.0f));
		vertices.push_back(vec2(a3.x,a3.y));
		colours.push_back(vec3(0.0f,1.0f ,0.0f+ (layer/50)));
		vertices.push_back(vec2(a1.x,a1.y));
		colours.push_back(vec3(1.0f+ (layer/50),.0f ,.0f));
		

		vertices.push_back(vec2(c.x,c.y));
		colours.push_back(vec3(.0f,1.0f+ (layer/50),0.0f ));
		vertices.push_back(vec2(a2.x,a2.y));
		colours.push_back(vec3(1.0f,0.0f,0.0f+ (layer/50) ));
		vertices.push_back(vec2(a3.x,a3.y));
		colours.push_back(vec3(.0f+ (layer/50),0.0f,1.0f));
		

		//Inverted Triangle Coordinates and Color
		vertices.push_back(vec2(a1.x,a1.y));
		colours.push_back(vec3(0.8f,0.498039f,0.196078f ));
		vertices.push_back(vec2(a2.x,a2.y));
		colours.push_back(vec3(0.55f,0.47f,0.14f ));
		vertices.push_back(vec2(a3.x,a3.y));
		colours.push_back(vec3(0.9f,0.91f,0.98f ));



		SierpinskiRecursive(a,a1,a2,layer-1);
		SierpinskiRecursive(b,a3,a1,layer-1);
		SierpinskiRecursive(c,a2,a3,layer-1);
		

	}
		

}


void sierpinkskiTriangle(int layers)
{
	vertices.clear();
	colours.clear();
	Coordinate a,b,c;
	//Base Triangle Coordinates
	a.x = -0.6f;
	a.y = -0.4f;
	b.x = 0.6f;
	b.y = -0.4f;
	c.x = 0.0f;
	c.y = 0.6f; 

	vertices.push_back(vec2(a.x,a.y));
	colours.push_back(vec3(1.0f,0.0f,0.0f));
	vertices.push_back(vec2(b.x,b.y));
	colours.push_back(vec3(0.0f,1.0f,.0f));
	vertices.push_back(vec2(c.x,c.y));
	colours.push_back(vec3(0.0f,0.0f,1.0f));


	if(layers != 1)
	{
		SierpinskiRecursive(a,b,c, layers);
	}


}

bool InitializeVAO(Geometry *geometry){

	const GLuint VERTEX_INDEX = 0;
	const GLuint COLOUR_INDEX = 1;

	//Generate Vertex Buffer Objects
	// create an array buffer object for storing our vertices
	glGenBuffers(1, &geometry->vertexBuffer);

	// create another one for storing our colours
	glGenBuffers(1, &geometry->colourBuffer);

	//Set up Vertex Array Object
	// create a vertex array object encapsulating all our vertex attributes
	glGenVertexArrays(1, &geometry->vertexArray);
	glBindVertexArray(geometry->vertexArray);

	// associate the position array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glVertexAttribPointer(
		VERTEX_INDEX,		//Attribute index 
		2, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		sizeof(vec2),		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(VERTEX_INDEX);

	// associate the colour array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, geometry->colourBuffer);
	glVertexAttribPointer(
		COLOUR_INDEX,		//Attribute index 
		3, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		sizeof(vec3), 		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(COLOUR_INDEX);

	// unbind our buffers, resetting to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return !CheckGLErrors();
}

// create buffers and fill with geometry data, returning true if successful
bool LoadGeometry(Geometry *geometry, vec2 *vertices, vec3 *colours, int elementCount)
{
	geometry->elementCount = elementCount;

	// create an array buffer object for storing our vertices
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2)*geometry->elementCount, vertices, GL_STATIC_DRAW);

	// create another one for storing our colours
	glBindBuffer(GL_ARRAY_BUFFER, geometry->colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*geometry->elementCount, colours, GL_STATIC_DRAW);

	//Unbind buffer to reset to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// check for OpenGL errors and return false if error occurred
	return !CheckGLErrors();
}

// deallocate geometry-related objects
void DestroyGeometry(Geometry *geometry)
{
	// unbind and destroy our vertex array object and associated buffers
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &geometry->vertexArray);
	glDeleteBuffers(1, &geometry->vertexBuffer);
	glDeleteBuffers(1, &geometry->colourBuffer);
}

// --------------------------------------------------------------------------
// Rendering function that draws our scene to the frame buffer

void RenderSceneForSquareAndDiamond(Geometry *geometry, GLuint program)
{
	// clear screen to a dark grey colour
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(program);
	glBindVertexArray(geometry->vertexArray);
	glDrawArrays(GL_LINES, 0, geometry->elementCount);

	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}

void RenderSceneForParametricSpiral(Geometry *geometry, GLuint program)
{
	// clear screen to a dark grey colour
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(program);
	glBindVertexArray(geometry->vertexArray);
	glDrawArrays(GL_POINTS, 0, geometry->elementCount);

	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}

void RenderSceneForSierpinkski(Geometry *geometry, GLuint program)
{
	// clear screen to a dark grey colour
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(program);
	glBindVertexArray(geometry->vertexArray);
	glDrawArrays(GL_TRIANGLES, 0, geometry->elementCount);

	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}


// --------------------------------------------------------------------------
// GLFW callback functions

// reports GLFW errors
void ErrorCallback(int error, const char* description)
{
	cout << "GLFW ERROR " << error << ":" << endl;
	cout << description << endl;
}

// handles keyboard input events
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_UP && action == GLFW_PRESS && layersForUser!=maxLayersAllowed)
		layersForUser++;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS && layersForUser != 1)
		layersForUser--;
	if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS && caseNumber != '3'){
		layersForUser = 1;
		caseNumber++;
	}
	if(key == GLFW_KEY_LEFT && action == GLFW_PRESS && caseNumber != '1'){
		layersForUser = 1;
		caseNumber--;
	}
}

// ==========================================================================
// PROGRAM ENTRY POINT

int main(int argc, char *argv[])
{
	// initialize the GLFW windowing system
	if (!glfwInit()) {
		cout << "ERROR: GLFW failed to initialize, TERMINATING" << endl;
		return -1;
	}
	glfwSetErrorCallback(ErrorCallback);

	// attempt to create a window with an OpenGL 4.1 core profile context
	GLFWwindow *window = 0;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int width = 512, height = 512;
	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		cout << "Program failed to create GLFW window, TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}

	// set keyboard callback function and make our context current (active)
	glfwSetKeyCallback(window, KeyCallback);
	glfwMakeContextCurrent(window);

	//Intialize GLAD
	if (!gladLoadGL())
	{
		cout << "GLAD init failed" << endl;
		return -1;
	}

	// query and print out information about our OpenGL environment
	QueryGLVersion();

	// call function to load and compile shader programs
	GLuint program = InitializeShaders();
	if (program == 0) {
		cout << "Program could not initialize shaders, TERMINATING" << endl;
		return -1;
	}
	
	if (!InitializeVAO(&geometry))
		cout << "Program failed to intialize geometry!" << endl;

	if(!LoadGeometry(&geometry, vertices.data(), colours.data(), vertices.size()))
		cout << "Failed to load geometry" << endl;

	// run an event-triggered main loop
	
	while (!glfwWindowShouldClose(window))
	{
		// call function to draw our scene
		switch (caseNumber){
			case '1':
				squareAndDiamond(&vertices,&colours,layersForUser);
			//LoadGeometry(&geometry, vertices.data(), colours.data(), vertices.size());
				break;
			case '2':
				paraMetricSpiral(&vertices,&colours,layersForUser);
			//LoadGeometry(&geometry, vertices.data(), colours.data(), vertices.size());
				break;
			case '3':
				sierpinkskiTriangle(layersForUser);
				break;
		}
		
		LoadGeometry(&geometry, vertices.data(), colours.data(), vertices.size());
		if (caseNumber == '3')
		{
			RenderSceneForSierpinkski(&geometry, program);
		}

		else if( caseNumber == '2')
		{
			RenderSceneForParametricSpiral(&geometry, program);
		}
		else{
		RenderSceneForSquareAndDiamond(&geometry, program);
	}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// clean up allocated resources before exit
	DestroyGeometry(&geometry);
	glUseProgram(0);
	glDeleteProgram(program);
	glfwDestroyWindow(window);
	glfwTerminate();

	cout << "Goodbye!" << endl;
	return 0;
}

// ==========================================================================
// SUPPORT FUNCTION DEFINITIONS

// --------------------------------------------------------------------------
// OpenGL utility functions

void QueryGLVersion()
{
	// query opengl version and renderer information
	string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << endl;
}

bool CheckGLErrors()
{
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			cout << "GL_INVALID_ENUM" << endl; break;
		case GL_INVALID_VALUE:
			cout << "GL_INVALID_VALUE" << endl; break;
		case GL_INVALID_OPERATION:
			cout << "GL_INVALID_OPERATION" << endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl; break;
		case GL_OUT_OF_MEMORY:
			cout << "GL_OUT_OF_MEMORY" << endl; break;
		default:
			cout << "[unknown error code]" << endl;
		}
		error = true;
	}
	return error;
}

// --------------------------------------------------------------------------
// OpenGL shader support functions

// reads a text file with the given name into a string
string LoadSource(const string &filename)
{
	string source;

	ifstream input(filename.c_str());
	if (input) {
		copy(istreambuf_iterator<char>(input),
			istreambuf_iterator<char>(),
			back_inserter(source));
		input.close();
	}
	else {
		cout << "ERROR: Could not load shader source from file "
			<< filename << endl;
	}

	return source;
}

// creates and returns a shader object compiled from the given source
GLuint CompileShader(GLenum shaderType, const string &source)
{
	// allocate shader object name
	GLuint shaderObject = glCreateShader(shaderType);

	// try compiling the source as a shader of the given type
	const GLchar *source_ptr = source.c_str();
	glShaderSource(shaderObject, 1, &source_ptr, 0);
	glCompileShader(shaderObject);

	// retrieve compile status
	GLint status;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetShaderInfoLog(shaderObject, info.length(), &length, &info[0]);
		cout << "ERROR compiling shader:" << endl << endl;
		cout << source << endl;
		cout << info << endl;
	}

	return shaderObject;
}

// creates and returns a program object linked from vertex and fragment shaders
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	// allocate program object name
	GLuint programObject = glCreateProgram();

	// attach provided shader objects to this program
	if (vertexShader)   glAttachShader(programObject, vertexShader);
	if (fragmentShader) glAttachShader(programObject, fragmentShader);

	// try linking the program with given attachments
	glLinkProgram(programObject);

	// retrieve link status
	GLint status;
	glGetProgramiv(programObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetProgramInfoLog(programObject, info.length(), &length, &info[0]);
		cout << "ERROR linking shader program:" << endl;
		cout << info << endl;
	}

	return programObject;
}
