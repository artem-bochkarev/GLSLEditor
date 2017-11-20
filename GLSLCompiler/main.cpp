#include <glload/gl_3_3.h>
#include <glload/gl_load.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>

/*static const struct
{
    float x, y;
    float r, g, b;
} vertices[ 3 ] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    { 0.6f, -0.4f, 0.f, 1.f, 0.f },
    { 0.f,  0.6f, 0.f, 0.f, 1.f }
};
static const char* vertex_shader_text =
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
static const char* fragment_shader_text =
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";*/
static void error_callback( int error, const char* description )
{
    fprintf( stderr, "Error: %s\n", description );
}
static void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
        glfwSetWindowShouldClose( window, GLFW_TRUE );
}
int main( int argc, char *argv[] )
{
    GLFWwindow* window;
    glfwSetErrorCallback( error_callback );
    if ( !glfwInit() )
        exit( EXIT_FAILURE );
    std::string filename( "" );
    for ( int i = 1; i<argc; ++i )
    {
        std::string tmp = argv[ i ];
        if ( tmp.compare( "-c" ) == 0 )
        {
            filename = argv[ i+1 ];
            break;
        }
        else
        {
            filename = argv[ i ];
            break;
        }
    }

    std::ifstream ifs;
    ifs.open ( filename, std::ifstream::in );
    std::stringstream ss;

    std::regex rver( "(#version\\s+)(\\d)(\\d)(\\d)" );
    while ( ifs.good() ) {
        std::string line;
        std::getline( ifs, line );
        ss << line << "\r\n";
    }
    ifs.close();

    std::string text = ss.str();
    std::cmatch m;
    int verMaj, verMin;
    if ( std::regex_search( text.c_str(), m, rver ) )
    {
        std::stringstream s1;
        s1 << m[ 2 ] << " " << m[3];
        s1 >> verMaj >> verMin;
    }

    bool bVertexShader = false, bGeometryShader = false;
    std::regex rvertex( "gl_Position" );
    if ( std::regex_search( text.c_str(), m, rvertex ) )
    {
        bVertexShader = true;
    }

    std::regex rgeometry( "EmitVertex\\s*(\\s*)" );
    if ( std::regex_search( text.c_str(), m, rgeometry ) )
    {
        bGeometryShader = true;
    }

    if ( verMaj < 3 )
    {
        verMaj = 2;
        verMin = 1;
    }
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, verMaj );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, verMin );
    glfwWindowHint( GLFW_VISIBLE, GLFW_FALSE );
    window = glfwCreateWindow( 640, 480, "Simple example", NULL, NULL );
    if ( !window )
    {
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    glfwSetKeyCallback( window, key_callback );
    glfwMakeContextCurrent( window );
    int loadTest = ogl_LoadFunctions();
    if ( loadTest == ogl_LOAD_FAILED )
    {
        glfwTerminate();
        exit( EXIT_FAILURE );
    }

    GLenum shaderType = GL_VERTEX_SHADER;
    if ( bVertexShader )
    {
        if ( bGeometryShader )
            shaderType = GL_GEOMETRY_SHADER;
    }
    else
    {
        shaderType = GL_FRAGMENT_SHADER;
    }
    GLuint shader = glCreateShader( shaderType );
    const char *c[ 1 ] = {text.c_str()};
    glShaderSource( shader, 1, c, NULL );
    glCompileShader( shader );
    GLint bStatus;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &bStatus );
    //if ( bStatus == false )
    {
        GLint length;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &length );
        if ( length > 0 )
        {
            char* txt = new char[ length + 1 ];
            GLint rlength;
            glGetShaderInfoLog( shader, length, &rlength, txt );
            std::cout << txt;
        }
    }
    if ( bStatus == GL_TRUE )
    {
        std::cout << "Succeeded\n";
    }

    //glfwSwapInterval( 1 );
    // NOTE: OpenGL error checks have been omitted for brevity
    /*glGenBuffers( 1, &vertex_buffer );
    glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    vertex_shader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex_shader, 1, &vertex_shader_text, NULL );
    glCompileShader( vertex_shader );
    GLint bStatus;
    glGetShaderiv( vertex_shader, GL_COMPILE_STATUS, &bStatus );
    if ( bStatus == false )
    {
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    fragment_shader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment_shader, 1, &fragment_shader_text, NULL );
    glCompileShader( fragment_shader );
    glGetShaderiv( fragment_shader, GL_COMPILE_STATUS, &bStatus );
    if ( bStatus == false )
    {
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    program = glCreateProgram();
    glAttachShader( program, vertex_shader );
    glAttachShader( program, fragment_shader );
    glLinkProgram( program );
    glGetProgramiv( program, GL_LINK_STATUS, &bStatus );
    glGetShaderiv( vertex_shader, GL_COMPILE_STATUS, &bStatus );
    if ( bStatus == false )
    {
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    mvp_location = glGetUniformLocation( program, "MVP" );
    vpos_location = glGetAttribLocation( program, "vPos" );
    vcol_location = glGetAttribLocation( program, "vCol" );
    glEnableVertexAttribArray( vpos_location );
    glVertexAttribPointer( vpos_location, 2, GL_FLOAT, GL_FALSE,
        sizeof( float ) * 5, (void*)0 );
    glEnableVertexAttribArray( vcol_location );
    glVertexAttribPointer( vcol_location, 3, GL_FLOAT, GL_FALSE,
        sizeof( float ) * 5, (void*)(sizeof( float ) * 2) );
    while ( !glfwWindowShouldClose( window ) )
    {
        float ratio;
        int width, height;
        glm::mat4 m, p, mvp;
        glfwGetFramebufferSize( window, &width, &height );
        ratio = width / (float)height;
        glViewport( 0, 0, width, height );
        glClear( GL_COLOR_BUFFER_BIT );

        m = glm::mat4( 1.0 );
        m = glm::rotate( m, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        //mat4x4_rotate_Z( m, m, (float)glfwGetTime() );
        p = glm::ortho( -ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f );
        //mat4x4_ortho( p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f );
        mvp = p * m;
        //mat4x4_mul( mvp, p, m );
        glUseProgram( program );
        glUniformMatrix4fv( mvp_location, 1, GL_FALSE, glm::value_ptr( mvp ) );
        glDrawArrays( GL_TRIANGLES, 0, 3 );
        glfwSwapBuffers( window );
        glfwPollEvents();
    }*/
    glfwDestroyWindow( window );
    glfwTerminate();
    exit( EXIT_SUCCESS );
}