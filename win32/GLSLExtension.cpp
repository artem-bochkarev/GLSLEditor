#include "GLSLExtension.h"
#include <regex>

GLSLExtension::GLSLExtension() 
:m_bWaitNextKey(false), m_pHost(nullptr){}

GLSLExtension::~GLSLExtension() {}

GLSLExtension &GLSLExtension::Instance() {
    static GLSLExtension singleton;
    return singleton;
}

bool GLSLExtension::Initialise( ExtensionAPI *host_ ) {
    m_pHost = host_;
    m_bWaitNextKey = false;
    return false;
}

bool GLSLExtension::Finalise() {
    m_pHost = nullptr;
    return false;
}

bool GLSLExtension::Clear() {
    return false;
}

bool GLSLExtension::Load( const char *filename ) {
    bool loaded = false;

    return loaded;
}

bool GLSLExtension::OnOpen( const char *filename )
{
    return false;
}

void GLSLExtension::FromCString()
{
    int size = m_pHost->Send( ExtensionAPI::paneEditor, SCI_GETSELTEXT, 0, 0 );
    if ( size <= 1 )
    {
        m_pHost->Send( ExtensionAPI::paneEditor, SCI_SELECTALL, 0, 0 );
        size = m_pHost->Send( ExtensionAPI::paneEditor, SCI_GETSELTEXT, 0, 0 );
    }
    char* buffer = new char[ size+10 ];
    m_pHost->Send( ExtensionAPI::paneEditor, SCI_GETSELTEXT, 0, reinterpret_cast<sptr_t>(buffer) );

    std::string newText( buffer );
    std::regex re1( R"(\\\n|\\\r\n)" );
    newText = std::regex_replace( newText, re1, "" );
    std::regex red( R"("|\n|\r)" );
    newText = std::regex_replace( newText, red, "" );
    std::regex ren( R"(\\n)" );
    newText = std::regex_replace( newText, ren, "\r\n" );
    m_pHost->Send( ExtensionAPI::paneEditor, SCI_REPLACESEL, 0, reinterpret_cast<sptr_t>(newText.c_str()) );
    delete buffer;
}

void GLSLExtension::CopyToCString()
{
    int size = m_pHost->Send( ExtensionAPI::paneEditor, SCI_GETLENGTH );
    char* buffer = new char[ size+10 ];
    m_pHost->Send( ExtensionAPI::paneEditor, SCI_GETTEXT, size+1, reinterpret_cast<sptr_t>(buffer) );
    std::string newText( buffer );
    delete buffer;
    std::regex r0( R"(\s+(\r?\n|$))" );
    newText = std::regex_replace( newText, r0, "$1" );
    std::regex r1( R"((^|\r?\n)([^\n\r]*)(\r?\n|$))" );
    newText = std::regex_replace( newText, r1, "\"$2\\n\"$3" );
    m_pHost->Send( ExtensionAPI::paneEditor, SCI_COPYTEXT, newText.length() + 1, reinterpret_cast<sptr_t>(newText.c_str()) );
}

bool GLSLExtension::OnKey( int keyval, int modifiers ) {
    bool handled = false;
    if ( m_bWaitNextKey )
    {
        if ( keyval == 'F' )
        {
            FromCString();
            handled = true;
        }

        if ( keyval == 'C' )
        {
            CopyToCString();
            handled = true;
        }
        m_bWaitNextKey = false;
    }
    else if ( (SCMOD_CTRL & modifiers) != 0 )
    {
        if ( keyval == 'K' )
        {
            m_bWaitNextKey = true;
            handled = true;
        }
    }
    return handled;
}