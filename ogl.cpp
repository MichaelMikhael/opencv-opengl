#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GL/freeglut.h"
#include "GL/gl.h"
#include "opencv2/opencv.hpp"
#include "/root/cvgl/shader.h"

using namespace cv;
using namespace std;

void drw()
{
	glClearColor(1.0f,1.0f,1.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f(1.0f,1.0f,1.0f);
	glOrtho(-1.0,1.0,-1.0,1.0,-1.0,1.0);
	
		glBegin(GL_TRIANGLES);
		   glVertex3f(-0.7,0.7,0);
		   glVertex3f(0.7,0.7,0);
		   glVertex3f(0,-1,0);
		glEnd();
	glFlush;	
}
static void init_opengl() {
    glViewport(0, 0, 500, 500); // use a screen size of WIDTH x HEIGHT

    glMatrixMode(GL_PROJECTION);     // Make a simple 2D projection on the entire window
    glLoadIdentity();
    glOrtho(0.0, 500, 500, 0.0, 0.0, 100.0);

    glMatrixMode(GL_MODELVIEW);    // Set the matrix mode to object modeling

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the window
}

int main(int argc, char **argv)
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	
	GLFWwindow* win = glfwCreateWindow(500,500,"Mike",NULL,NULL);
	
	glfwMakeContextCurrent(win);
	
	glewInit();
	
	Mat im0 = imread("/root/Desktop/IVIL.jpg");
	
	Mat im;
	
	
	VideoCapture v(0);
	
	GLuint texture1;
	
	init_opengl();
	
	Shader shader("/root/cvgl/vert_shader.vs","/root/cvgl/frag_shader.fs");
	
	float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    
    unsigned int VBO,VAO,EBO;
    
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    
    glGenBuffers(1,&EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);
	
	shader.use();
	glUniform1i(glGetUniformLocation(shader.ID,"texture1"),0);
	
	while(1)
		{
		
		v>>im;
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		
		glEnable(GL_TEXTURE_2D);
		
		glGenTextures(1,&texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
	
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_NEAREST);
	
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,im.cols,im.rows,0,GL_BGR,GL_UNSIGNED_BYTE,im.ptr());
		//glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,im0.cols,im0.rows,0,GL_BGR,GL_UNSIGNED_BYTE,im0.ptr());
	
		glGenerateMipmap(GL_TEXTURE_2D);
		
		/*
		glBegin(GL_QUADS);
		
			glTexCoord2i(0, 0); glVertex2i(0,   0);
			glTexCoord2i(0, 1); glVertex2i(0, 500);
			glTexCoord2i(1, 1); glVertex2i(500, 500);
			glTexCoord2i(1, 0); glVertex2i(500, 0);
		
		glEnd();
		*/
		shader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		
		glDeleteTextures(1, &texture1);
		glDisable(GL_TEXTURE_2D);
		
		glfwSwapBuffers(win);
		glfwPollEvents();
		
		}
	
}	
