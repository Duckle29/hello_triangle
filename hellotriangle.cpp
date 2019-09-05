// gluttriangle.c
//
//    This is a simple example that draws a single triangle with
//    a minimal vertex/fragment shader.  The purpose of this 
//    example is to demonstrate the basic concepts of 
//    OpenGL ES 2.0 rendering but using OpenGL and GLUT.

#include <stdlib.h>
#ifdef __APPLE__
# include <GLUT/glut.h>
#else
#include "GL/glew.h" 
#include "GL/glut.h" 
#endif
#include <stdio.h>

typedef struct
{
   GLuint programObject;
} UserData;

// Global data (because the Draw function cannot receive an argument).
UserData userData;

///
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;
   
   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
   	return 0;

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );
   
   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled ) 
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
         char* infoLog = malloc (sizeof(char) * infoLen );

         glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
         fprintf(stderr, "error compiling shader:\n%s\n", infoLog );            
         
         free ( infoLog );
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;

}

///
// Initialize the shader and program object
//
int Init ( void )
{
   GLbyte vShaderStr[] =  
      "attribute vec4 vPosition;    \n"
      "void main()                  \n"
      "{                            \n"
      "   gl_Position = vPosition;  \n"
      "}                            \n";

   GLbyte fShaderStr[] =  
      "void main()                                      \n"
      "{                                                \n"
      "   gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );   \n" 
      "}                                                \n";


   GLuint vertexShader;
   GLuint fragmentShader;
   GLuint programObject;
   GLint linked;

   // Load the vertex/fragment shaders
   vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
   fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, fShaderStr );

   // Create the program object
   programObject = glCreateProgram ( );
   
   if ( programObject == 0 )
      return 0;

   glAttachShader ( programObject, vertexShader );
   glAttachShader ( programObject, fragmentShader );

   // Bind vPosition to attribute 0   
   glBindAttribLocation ( programObject, 0, "vPosition" );

   // Link the program
   glLinkProgram ( programObject );

   // Check the link status
   glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

   if ( !linked ) 
   {
      GLint infoLen = 0;

      glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
         char* infoLog = malloc (sizeof(char) * infoLen );

         glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
         fprintf(stderr, "error linking program:\n%s\n", infoLog );            
         
         free ( infoLog );
      }

      glDeleteProgram ( programObject );
      return GL_FALSE;
   }

   // Store the program object
   userData.programObject = programObject;

   glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
   return GL_TRUE;
}

///
// Draw two triangles using the shader pair created in Init()
// (Note that this display function must not have arguments because of glut.)
//
void Draw ( void )
{
   GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f, // vertices of first triangle
                           -0.5f, -0.5f, 0.0f,
                            0.5f, -0.5f, 0.0f};

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData.programObject );

   // Load the vertex data
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
   glEnableVertexAttribArray ( 0 );


   // Draw the triangles   
   glDrawArrays ( GL_TRIANGLES, 0, 3 ); // start at vertex 0 and use 3 vertices (i.e. 1 triangles)

   glutSwapBuffers ( );   // Note the "glut" prefix
}

///
// React to a resize event of the window. 
// (Windows on desktops are often resized.)
//
void Reshape(int width, int height)
{
   // Set the viewport
   glViewport(0, 0, width, height);    
}

int main(int argc, char **argv)
{
   GLenum errorCode;

   // Init glut and create window.
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(320, 240);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Hello Triangle");

   // Check OpenGL error, version, and extensions.
   errorCode = glGetError();
   if (GL_NO_ERROR != errorCode)
   {
      fprintf(stderr, "OpenGL error: %s\n", gluErrorString(errorCode));
      return 1;
   }
   fprintf(stdout, "available extensions: %s\n", glGetString(GL_EXTENSIONS));
   fprintf(stdout, "using OpenGL %s\n", glGetString(GL_VERSION));
   fprintf(stdout, "using shading language %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

#ifndef __APPLE__
   // Init GLEW and check error and version.
   errorCode = glewInit();
   if (GLEW_OK != errorCode)
   {
      fprintf(stderr, "GLEW error: %s\n", glewGetErrorString(errorCode));
      return 2;
   }
   fprintf(stdout, "using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

   // Init program.
   if ( !Init ( ) )
      return 3;

   // Register display and reshape functions.
   glutDisplayFunc(Draw);
   glutReshapeFunc(Reshape);

   glutMainLoop();
   return 0; 
}