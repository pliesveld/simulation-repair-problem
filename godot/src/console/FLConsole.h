/**
    FLTK console interface to CVars.

    This Code is covered under the LGPL.  See COPYING file for the license.

    $Id: FLConsole.h 199 2012-04-09 01:00:25Z gsibley $
 */

#ifndef __FLCONSOLE_H__
#define __FLCONSOLE_H__

#include "CVar.h"

#include <limits.h>
#include <stdio.h>
#include <stdarg.h>

#include <algorithm>
#include <cstring>

#include <deque>
#include <string>

#include <core/Godot.hpp>

// how to make GDConsole a singleton and have FLConsole a private member?
//struct FLConsole;
//#include "GDConsole.h"


#define CVAR_DEL_KEY 127
#define FLCONSOLE_KEY 96

#define FLCONSOLE_HELP_FILE 			"helpfile.txt"
#define FLCONSOLE_HISTORY_FILE 			".cvar_history"
#define FLCONSOLE_SETTINGS_FILE 		".glconsole_settings"
#define FLCONSOLE_SCRIPT_FILE 			"default.script.txt"
#define FLCONSOLE_INITIAL_SCRIPT_FILE 	"initial.script.txt"

#include <core/Godot.hpp>

////////////////////////////////////////////////////////////////////////////////
/// The type of line entered. Used to determine how each line is treated.
enum LineProperty 
{
    LINEPROP_LOG,         // text coming from a text being logged to the console
    LINEPROP_COMMAND,     // command entered at the console
    LINEPROP_FUNCTION,    // a function
    LINEPROP_ERROR,       // an error
    LINEPROP_HELP         //help text
};

////////////////////////////////////////////////////////////////////////////////
/// A line of text contained in the console can be either inputted commands or
//  log text from the application.
class ConsoleLine
{
    public:
        ConsoleLine( std::string t, LineProperty p = LINEPROP_LOG, bool display = true ){
            m_sText = t;
            m_nOptions = p;
            m_bDisplay = display;
        }
        std::string m_sText;        //< Actual text.
        LineProperty m_nOptions;    //< See LineProperty enum.
        bool m_bDisplay;            //< Display on the console screen?
};

////////////////////////////////////////////////////////////////////////////////
///  The FLConsole class, of which there will only ever be one instance.
class FLConsoleInstance
{
    friend void FLConsoleCheckInit( FLConsoleInstance* pConsole ); 

    public:
         FLConsoleInstance();
        ~FLConsoleInstance();

        // call this after OpenGL is up 
        void Init();
 
        //Prints to console using familiar printf style
        void Printf(const char *msg, ...);
        void Printf_All(const char *msg, ...);

        //error and help versions of printf.
        void PrintError(const char *msg, ...);
        void PrintHelp(const char *msg, ...);

        void PrintAllCVars();

        // Help.
        bool Help(std::vector<std::string> *vArgs);

        bool HistorySave( std::string sFileName = "" );
        bool HistoryLoad( std::string sFileName = "" );

        bool SettingsSave(std::string sFileName = "");
        bool SettingsLoad(std::string sFileName = "");

        bool ScriptRun( std::string sFileName = "" );
        bool ScriptSave( std::string sFileName = "" );
        bool ScriptLoad( std::string sFileName = "" );

        // Script interface.
        void ScriptRecordStart();
        void ScriptRecordStop();
        void ScriptRecordPause();
        void ScriptShow();

        /// clears all of the console's history.
        void HistoryClear();

        int handleCommandEntered( const char * );

        void HistoryBack();
        void HistoryForward();

		std::string _TabComplete();
		void SetCurrentCommand(const char *);
		std::string GetCurrentCommand();
private:

        // printf style function take position to print to as well
        // NB: coordinates start from bottom left
        void glPrintf(int x, int y, const std::string& sMsg, ... );
        void glPrintf(int x, int y, const char* sMsg, ... );


        //scrolling text up and down in the console
        void ScrollUp(int pixels);
        void ScrollDown(int pixels);
        void ScrollUpLine();
        void ScrollDownLine();
        void CursorLeft();
        void CursorRight();
        void CursorToBeginningOfLine();
        void CursorToEndOfLine();
        void ScrollUpPage();
        void ScrollDownPage(); 


        /// Clear the current command.
        void ClearCurrentCommand();

        /// enter a full line of text to the log text.
        void EnterLogLine( const char *line, LineProperty prop = LINEPROP_LOG, bool display = true );

        /// display previous command in history on command line.
//        void HistoryBack();
   
        /// go forward in the history.
//        void HistoryForward();

        void _CheckInit();

        /// Height of the console in pixels (even if it is currently animating).
        int           _GetConsoleHeight();
        void          _RenderText();
//        void          _TabComplete();
        bool          _ProcessCurrentCommand( bool bExecute = true );
        bool          _ExecuteFunction( CVarUtils::CVar<ConsoleFunc> * cvar, bool bExecute );
        bool          _IsCursorOn();
        bool          _IsConsoleFunc( TrieNode *node );
        int           _FindRecursionLevel( std::string sCommand );
        bool          _LoadExecuteHistory( std::string sFileName = "", bool bExecute=false );
        std::string   _GetHistory();

    private:
        /// Member cvars accessible from console.
        int&            m_nConsoleMaxHistory;
        std::string&    m_sHistoryFileName;
        std::string&    m_sScriptFileName;
        std::string&    m_sSettingsFileName;
        std::string&    m_sInitialScriptFileName;

        CVarUtils::Color&       m_LogColor;
        CVarUtils::Color&       m_CommandColor;
        CVarUtils::Color&       m_FunctionColor;
        CVarUtils::Color&       m_ErrorColor;
        CVarUtils::Color&       m_HelpColor;
        CVarUtils::Color&       m_ConsoleColor;

        bool          m_bExecutingHistory; //Are we executing a script or not.
        bool          m_bSavingScript; // Are we saving a script.
        int           m_nCommandNum;
 
        //history variables
        std::string   m_sOldCommand;
        char          m_cLastChar;
        unsigned int  m_nSuggestionNum;

        std::string             m_sCurrentCommandBeg; //< command being typed, up to cursor
        std::string             m_sCurrentCommandEnd; //< command being typed, after cursor
        std::deque<ConsoleLine> m_sConsoleText;       //< all lines of console text
        std::deque<ConsoleLine> m_ScriptText;         //< lines of console text marked as script
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INLINE FUNCTION DEFINITIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/// This function returns a pointer to the very first FLConsoleInstance ever
//  created.  As there should only be one, this is ok.  
//  This is a workaround for header only programming.
inline FLConsoleInstance* GetConsole( FLConsoleInstance* pFirstConsole = NULL )
{
    static FLConsoleInstance* pSavedConsole = NULL; 
    if( pSavedConsole ){
        return pSavedConsole;
    }
    if( pFirstConsole == NULL ){
        // if pFirstConsole is NULL (e.g. user is asking for this first console), then 
        // pSavedConsole BETTER not also be NULL; 
        return NULL;
    }
    else{
        pSavedConsole = pFirstConsole;        
    }
    return pSavedConsole;
}

////////////////////////////////////////////////////////////////////////////////
/// This function calls "GetConsole" to set the static variable pSavedConsole so
//  we can get access to the console globally.
//  This is a workaround for header only programming.
inline void SetConsole( FLConsoleInstance* pFirstConsole )
{
    GetConsole( pFirstConsole );
}

////////////////////////////////////////////////////////////////////////////////
/// Return whether first element is greater than the second.
inline bool StringIndexPairGreater( std::pair<std::string,int> e1, std::pair<std::string,int> e2 )
{
    return e1.first < e2.first;
}

////////////////////////////////////////////////////////////////////////////////
/// Utility function.
inline std::string FindLevel( std::string sString, int iMinRecurLevel )
{   
    int level = 0;
    int index = sString.length();
    for( unsigned int ii = 0; ii < sString.length(); ii++ ) {
        if( sString.c_str()[ii]=='.' ) {
            level ++;
        }
        if( level == iMinRecurLevel ) {
            index = ii+1;
        }
    }
    return sString.substr( 0, index );
}

////////////////////////////////////////////////////////////////////////////////
/// remove all spaces from the front and back...
inline std::string& RemoveSpaces( std::string &str )
{
    // remove them off the front
    int idx = str.find_first_not_of( ' ' );
    if( idx > 0 && idx != 0 ) {
        str = str.substr( idx, str.length() );
    }

    // remove them off the back
    idx = str.find_last_not_of(' ');
    if( idx != -1 ) {
        str = str.substr( 0, idx+1 );
    }
    return str;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// The FLConsole for users.  
//  For now, this is just a wrapper around the singleton instance.
//  There can only be a single instance because we have to avoid CVar name duplication.
//  Know a better approach?
//  E.g., we could force users to instantiate their own "CVarSets"....
class FLConsole
{
    public:
        FLConsole();

        int handleCommandEntered( const char * );
        std::string TabComplete();
        void SetCurrentCommand( const char * );
        std::string GetCurrentCommand();

		void HistoryBack()
		{
			m_pFLConsoleInstance->HistoryBack();
		}

		void HistoryForward()
		{
			m_pFLConsoleInstance->HistoryForward();
		}

private:
        FLConsoleInstance*  m_pFLConsoleInstance; 
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor.
inline FLConsole::FLConsole()
{
    if( GetConsole() == NULL ){
        m_pFLConsoleInstance = new FLConsoleInstance();
        SetConsole( m_pFLConsoleInstance );
    }
    m_pFLConsoleInstance = GetConsole();
}

inline int FLConsole::handleCommandEntered( const char * cmd)
{
	return m_pFLConsoleInstance->handleCommandEntered(cmd);
}

inline std::string FLConsole::TabComplete()
{
	return m_pFLConsoleInstance->_TabComplete();
}

inline void FLConsole::SetCurrentCommand( const char * cmd) {
	return m_pFLConsoleInstance->SetCurrentCommand(cmd);
}

inline std::string FLConsole::GetCurrentCommand() {
	return m_pFLConsoleInstance->GetCurrentCommand();
}


#include "CVar.h"
#include "TrieNode.h"
#include "FLConsoleFunction.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor.
inline FLConsoleInstance::FLConsoleInstance() :
    // Init our member cvars  (can't init the names in the class decleration)
    m_nConsoleMaxHistory( CVarUtils::CreateCVar<int>(     "console.history.MaxHistory", 100 ) ), // max lines ofconsole history
    m_sHistoryFileName( CVarUtils::CreateCVar<> (         "console.history.HistoryFileName", std::string( FLCONSOLE_HISTORY_FILE ) ) ),
    m_sScriptFileName( CVarUtils::CreateCVar<> (          "script.ScriptFileName", std::string( FLCONSOLE_SCRIPT_FILE ) ) ),
    m_sSettingsFileName( CVarUtils::CreateCVar<> (        "console.settings.SettingsFileName", std::string( FLCONSOLE_SETTINGS_FILE ) ) ),
    m_sInitialScriptFileName( CVarUtils::CreateCVar<> (   "console.InitialScriptFileName", std::string( FLCONSOLE_INITIAL_SCRIPT_FILE ) ) ),


    // TODO: convert to Godot
    m_LogColor( CVarUtils::CreateCVar<CVarUtils::Color>(           "console.colors.LogColor", CVarUtils::Color( 255, 255, 64 ) ) ),
    m_CommandColor( CVarUtils::CreateCVar<CVarUtils::Color>(       "console.colors.CommandColor", CVarUtils::Color( 255, 255, 255 ) ) ),
    m_FunctionColor( CVarUtils::CreateCVar<CVarUtils::Color>(      "console.colors.FunctionColor", CVarUtils::Color( 64, 255, 64 ) ) ),
    m_ErrorColor( CVarUtils::CreateCVar<CVarUtils::Color>(         "console.colors.ErrorColor", CVarUtils::Color( 255, 128, 64 ) ) ),
    m_HelpColor( CVarUtils::CreateCVar<CVarUtils::Color>(          "console.colors.HelpColor", CVarUtils::Color( 110, 130, 200 ) ) ),
    m_ConsoleColor( CVarUtils::CreateCVar<CVarUtils::Color>(       "console.colors.ConsoleColor", CVarUtils::Color( 25, 60, 130, 150 ) ) )
{
    SetConsole( this );
    m_bSavingScript = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Destructor.
inline FLConsoleInstance::~FLConsoleInstance()
{
    HistorySave();
    SettingsSave();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Initialise the console. Sets up all the default values.
inline void FLConsoleInstance::Init()
{
    static bool bInitialized = false;
    if( bInitialized ){
        return;
    } 
    bInitialized = true;

    m_bExecutingHistory = false;
    m_bSavingScript = false;
    m_nCommandNum = 0;

    // add basic functions to the console
    CVarUtils::CreateCVar( "console.version", ConsoleVersion, "The current version of FLConsole" );
    CVarUtils::CreateCVar( "help", ConsoleHelp, "Gives help information about the console or more specifically about a CVar." );
    CVarUtils::CreateCVar( "find", ConsoleFind, "find 'name' will return the list of CVars containing 'name' as a substring." );
    CVarUtils::CreateCVar( "exit", ConsoleExit, "Close the application" );
    CVarUtils::CreateCVar( "quit", ConsoleExit, "Close the application" );
    CVarUtils::CreateCVar( "save", ConsoleSave, "Save the CVars to a file" );
    CVarUtils::CreateCVar( "load", ConsoleLoad, "Load CVars from a file" );

    CVarUtils::CreateCVar( "console.history.load", ConsoleHistoryLoad, "Load console history from a file" );
    CVarUtils::CreateCVar( "console.history.save", ConsoleHistorySave, "Save the console history to a file" );
    CVarUtils::CreateCVar( "console.history.clear", ConsoleHistoryClear, "Clear the current console history" );

    CVarUtils::CreateCVar( "console.settings.load", ConsoleSettingsLoad, "Load console settings from a file" );
    CVarUtils::CreateCVar( "console.settings.save", ConsoleSettingsSave, "Save the console settings to a file" );

    CVarUtils::CreateCVar( "script.record.start", ConsoleScriptRecordStart );
    CVarUtils::CreateCVar( "script.record.stop", ConsoleScriptRecordStop );
    CVarUtils::CreateCVar( "script.record.pause", ConsoleScriptRecordPause );
    CVarUtils::CreateCVar( "script.show", ConsoleScriptShow );
    CVarUtils::CreateCVar( "script.run", ConsoleScriptRun );
    CVarUtils::CreateCVar( "script.save", ConsoleScriptSave );
    CVarUtils::CreateCVar( "script.load", ConsoleScriptLoad );

    //load the default settings file
    SettingsLoad();

    //load the history file
    HistoryLoad();

    //load the initial execute script
    std::ifstream ifs( m_sInitialScriptFileName.c_str() );

    if( ifs.is_open() ) {
        ifs.close();
        ScriptRun(m_sInitialScriptFileName);
    }
    else
    {
//        std::cout << "Info: Initial script file, " << m_sInitialScriptFileName << ", not found." << std::endl;
        ifs.clear(std::ios::failbit);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Check that Init() has run. If not run.
inline void FLConsoleInstance::_CheckInit()
{
	// TODO: replace with explicit initialization from godot?
//    if( m_Viewport.width == 0 ) {
        Init();
//    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// printf style function take position to print to as well
// NB: coordinates start from bottom left
inline void FLConsoleInstance::glPrintf(int x, int y, const std::string& sMsg, ... )
{
	godot::Godot::print(sMsg.c_str()); // TODO: print to window
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// printf style function take position to print to as well
// NB: coordinates start from bottom left
inline void FLConsoleInstance::glPrintf(int x, int y, const char* sMsg, ... )
{
    if( sMsg == NULL ){
        return;
    }
    char *pBuffer = NULL;
    va_list ap;
    memset( &ap, 0, sizeof( ap ) );
    int nLength = 2048;
    int nResult = INT_MAX;
    va_start( ap, sMsg );
    {
        while( nResult >= nLength ) {
            if( pBuffer != NULL ) {
                delete[] pBuffer;
            }
            nLength *= 2;
            pBuffer = new char[nLength + 1];
            memset( pBuffer, 0, nLength + 1 );
            nResult = vsnprintf( pBuffer, nLength, sMsg, ap );
        }
    }
    va_end( ap );

    godot::Godot::print(pBuffer);  // TODO: print to window
    delete[] pBuffer;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// This will output text to the GL console.
inline void FLConsoleInstance::Printf(const char *msg, ... )
{
    _CheckInit();
    char msgBuf[1024];
    va_list va_alist;

    if (!msg) return;

    va_start( va_alist, msg );
    vsnprintf( msgBuf, 1024, msg, va_alist );
    va_end( va_alist );
    msgBuf[1024 - 1] = '\0';

    EnterLogLine( msgBuf );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// same as Printf() except it also prints to the terminal.
inline void FLConsoleInstance::Printf_All(const char *msg, ... )
{
    _CheckInit();
    char msgBuf[1024];
    va_list va_alist;

    if (!msg) return;

    va_start( va_alist, msg );
    vsnprintf( msgBuf, 1024, msg, va_alist );
    va_end( va_alist );
    msgBuf[1024 - 1] = '\0';

    EnterLogLine( msgBuf );
    printf( "%s", msgBuf );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// This will output help formatted text to the GL console.
inline void FLConsoleInstance::PrintHelp(const char *msg, ... )
{
    _CheckInit();
    char msgBuf[1024];
    va_list va_alist;

    if (!msg) return;

    va_start( va_alist, msg );
    vsnprintf( msgBuf, 1024, msg, va_alist );
    va_end( va_alist );
    msgBuf[1024 - 1] = '\0';

    EnterLogLine( msgBuf, LINEPROP_HELP );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// This will output error formatted text to the GL console.
inline void FLConsoleInstance::PrintError(const char *msg, ... )
{
    _CheckInit();
    char msgBuf[1024];
    va_list va_alist;

    if (!msg) return;

    va_start( va_alist, msg );
    vsnprintf( msgBuf, 1024, msg, va_alist );
    va_end( va_alist );
    msgBuf[1024 - 1] = '\0';

    EnterLogLine( msgBuf, LINEPROP_ERROR );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Clears all of the console's history.
inline void FLConsoleInstance::HistoryClear()
{	
    m_sConsoleText.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Save the console history to the named file.
//  If no file is specified the name held in the history.HistoryFileName CVar will be used.
//  @param sFilename save history to this file.
//  @return successor failure
inline bool FLConsoleInstance::HistorySave( std::string sFileName )
{
    ///@TODO check filenames for validity - no spaces or illegal characters.
    if( !m_bExecutingHistory ) {
        if( sFileName == ""){
            if(m_sHistoryFileName != "") {
                sFileName = m_sHistoryFileName;
            }
            else {
                PrintError( "Warning: No default name. Resetting history filename to: \"%s\".", FLCONSOLE_HISTORY_FILE );
                sFileName = m_sHistoryFileName = FLCONSOLE_HISTORY_FILE;
            }
        }

        std::ofstream ofs( sFileName.c_str() );

        if( !ofs.is_open() ) {
            PrintError( "Error: could not open \"%s\" for saving.", sFileName.c_str() );
            m_bExecutingHistory = false;
            return false;
        }

        unsigned int nTextSize = m_sConsoleText.size();
        for( int ii = nTextSize-1; ii >= 0 ; --ii ) {
            if( m_sConsoleText[ii].m_nOptions == LINEPROP_COMMAND ) {
                ofs << m_sConsoleText[ii].m_sText << "\n";
            }
        }
        ofs.close();
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Start script recording.
inline void FLConsoleInstance::ScriptRecordStart()
{	
    m_ScriptText.clear();
    m_bSavingScript = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Stop script recording.
inline void FLConsoleInstance::ScriptRecordStop()
{	
    if( !m_bSavingScript ) {
        return;
    }
    m_ScriptText.pop_front();
    m_ScriptText.pop_front();
    m_bSavingScript = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pause script recording.
inline void FLConsoleInstance::ScriptRecordPause()
{
    // unpause
    if( !m_bSavingScript && !m_ScriptText.empty() )
    {
        m_ScriptText.pop_front();
        m_ScriptText.pop_front();
        m_bSavingScript = true;
        return;
    }
    else  // pause
    {
        m_ScriptText.pop_front();
        m_ScriptText.pop_front();
        m_bSavingScript = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Print the script to the console.
inline void FLConsoleInstance::ScriptShow()
{	
    if( m_bSavingScript ) {
        m_ScriptText.pop_front();
        m_ScriptText.pop_front();
    }
    bool bWasSavingScript = m_bSavingScript;
    m_bSavingScript = false;
    for( int ii = m_ScriptText.size()-1; ii >= 0 ; --ii ) {
        if( m_ScriptText[ii].m_nOptions == LINEPROP_COMMAND ) {
            EnterLogLine( m_ScriptText[ii].m_sText.c_str(), LINEPROP_COMMAND );
        }
    }
    m_bSavingScript = bWasSavingScript;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  Run the current script or the one specified on disk.
//   @TODO currently overwrites the script in memory. need to allow multiple scripts 
//   to be held and ran for recursion.
inline bool FLConsoleInstance::ScriptRun( std::string sFileName )
{
    if(!sFileName.empty()){
        bool bsuccess = ScriptLoad(sFileName);
        if(!bsuccess){
            PrintError("Aborting script run");
            return false;
        }
    }

    if( m_bSavingScript ) {
        m_ScriptText.pop_front();
        m_ScriptText.pop_front();
    }
    bool bWasSavingScript = m_bSavingScript;
    m_bSavingScript = false;
    for( int ii = m_ScriptText.size()-1; ii >= 0 ; --ii ) {
        if( m_ScriptText[ii].m_nOptions == LINEPROP_COMMAND ) {
            m_sCurrentCommandBeg = m_ScriptText[ii].m_sText;
            m_sCurrentCommandEnd = "";
            _ProcessCurrentCommand( true );
            m_sCurrentCommandBeg = "";
        }
    }
    m_bSavingScript = bWasSavingScript;

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Save the current script to a file.
inline bool FLConsoleInstance::ScriptSave( std::string sFileName )
{	
    if( !m_bExecutingHistory ) {
        m_bExecutingHistory = true;

        if( m_bSavingScript ) {
            m_ScriptText.pop_front();
            m_ScriptText.pop_front();
        }

        if( sFileName == "" ) {
            sFileName = CVarUtils::GetCVar<std::string>( "script.ScriptFileName" );
        }

        std::ofstream ofs( sFileName.c_str() );

        if( !ofs.is_open() ) {
            PrintError( "Error: could not open \"%s\" for saving.", sFileName.c_str() );
            m_bExecutingHistory = false;
            return false;
        }

        unsigned int nTextSize = m_ScriptText.size();

        for( int ii = nTextSize-1; ii >= 0 ; --ii ) {
            if( m_ScriptText[ii].m_nOptions == LINEPROP_COMMAND ) {
                ofs << m_ScriptText[ii].m_sText << "\n";
            }
        }

        ofs.close();
        m_bExecutingHistory = false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Load history from a file.
//  @param sFileName Load history from this file, else use default.
//  @return sucess or failure.
inline bool FLConsoleInstance::HistoryLoad( std::string sFileName )
{
    if( sFileName == "" ) {
        if(m_sHistoryFileName != "")
            sFileName = m_sHistoryFileName;
        else {
            PrintError("Warning: No default name. Resetting history filename to: \"%s\".", FLCONSOLE_HISTORY_FILE);
            sFileName = m_sHistoryFileName = FLCONSOLE_HISTORY_FILE;
        }
    }

    //test if file exists
    std::ifstream ifs( sFileName.c_str() );

    if( ifs.is_open() ) {
        ifs.close();
        return _LoadExecuteHistory( sFileName, false );
    }
    else {
//        std::cout << "Info: History file, " << sFileName << ", not found." << std::endl;
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Load a script from a file.
inline bool FLConsoleInstance::ScriptLoad( std::string sFileName )
{
    if( sFileName == "") {
        if(m_sScriptFileName != "") {
            sFileName = m_sScriptFileName;
        }
        else {
            PrintError("Warning: No default name. Resetting script filename to: \"%s\".", FLCONSOLE_SCRIPT_FILE);
            sFileName = m_sScriptFileName = FLCONSOLE_SCRIPT_FILE;
        }
    }

    //test if file exists
    std::ifstream ifs( sFileName.c_str() );

    if( ifs.is_open() ) {
        ifs.close();
        return _LoadExecuteHistory( sFileName, true );
    }
    else {
//        std::cout << "Info: Script file, " << sFileName << ", not found." << std::endl;
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Load history from a file and execute it if desired.
inline bool FLConsoleInstance::_LoadExecuteHistory( std::string sFileName, bool bExecute )
{
    if( sFileName == "" ) {
        std::cerr << "_LoadExecuteHistory: No file specified. There is a bug in FLConsole. Please report it." << std::endl;
        return false;
    }

    if( !m_bExecutingHistory ) {
        m_bExecutingHistory = true;

        std::ifstream ifs( sFileName.c_str() );

        if( !ifs.is_open() ) {
            PrintError("Error: could not open \"%s\" for loading.", sFileName.c_str());
            m_bExecutingHistory = false;
            return false;
        }

        while( ifs.good() ) {
            char linebuf[1024];
            ifs.getline(linebuf, 1024);
            if( ifs.good() ) {
                m_sCurrentCommandBeg = linebuf;
                m_sCurrentCommandEnd = "";
                _ProcessCurrentCommand( bExecute );
                m_sCurrentCommandBeg = "";
            }
        }
        m_bExecutingHistory = false;

        ifs.close();
    }
    return true;
}

inline int FLConsoleInstance::handleCommandEntered( const char * sInput ) {

	_CheckInit();
	m_sCurrentCommandBeg = "";
	m_sCurrentCommandBeg = std::string(sInput);
	_ProcessCurrentCommand();
	m_sCurrentCommandBeg = "";
	m_sCurrentCommandEnd = "";
	m_nCommandNum = 0; //reset history
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void FLConsoleInstance::ClearCurrentCommand() 
{
    m_sCurrentCommandBeg = ""; 
    m_sCurrentCommandEnd = ""; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void FLConsoleInstance::SetCurrentCommand(const char *cmd)
{
	m_sCurrentCommandBeg = std::string(cmd);
	m_sCurrentCommandEnd = "";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline std::string FLConsoleInstance::GetCurrentCommand()
{
	return std::string(m_sCurrentCommandBeg);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Add a line to the history log.
// TODO: moved to GDConsole.cpp
// TODO: test if impacted by header-only
//inline void FLConsoleInstance::EnterLogLine(const char *line, const LineProperty prop, bool display)
//{
//    _CheckInit();
//    if( (int)m_sConsoleText.size() >= m_nConsoleMaxHistory ) {
//        m_sConsoleText.pop_back();
//    }
//
//    if( line != NULL ) {
//        m_sConsoleText.push_front( ConsoleLine(std::string(line), prop, display) );
//
//        if( m_bSavingScript && prop != LINEPROP_ERROR ) {
//            m_ScriptText.push_front( ConsoleLine(std::string(line), prop, display) );
//        }
//    }
//}

////////////////////////////////////////////////////////////////////////////////
/// Retreives command in history, if m_nCommandNum is invalid then
//  m_sCurrentCommand is returned.
inline std::string FLConsoleInstance::_GetHistory()
{
    _CheckInit();
    std::deque<ConsoleLine>::iterator it;
    int commandCount = 1;

    if( m_nCommandNum <= 0 ) {
        return m_sCurrentCommandBeg+m_sCurrentCommandEnd;
    }

    for( it = m_sConsoleText.begin() ; it != m_sConsoleText.end() ; it++ ) {
        if( it->m_nOptions == LINEPROP_COMMAND ) {
            if( commandCount == m_nCommandNum ) {
                return it->m_sText;
            }
            else {//not the right command
                commandCount++;
            }
        }
    }
    if(  m_nCommandNum > commandCount ) {
        m_nCommandNum = commandCount-1;
    }

    return m_sCurrentCommandBeg+m_sCurrentCommandEnd;
}

////////////////////////////////////////////////////////////////////////////////
inline void FLConsoleInstance::HistoryBack()
{
    _CheckInit();
    if(m_nCommandNum <= 0 ) {
        m_sOldCommand = m_sCurrentCommandBeg+m_sCurrentCommandEnd;
    }
    m_nCommandNum++;
    std::string temp(m_sCurrentCommandBeg+m_sCurrentCommandEnd);
    m_sCurrentCommandBeg = _GetHistory();
    m_sCurrentCommandEnd = "";
}

////////////////////////////////////////////////////////////////////////////////
inline void FLConsoleInstance::HistoryForward()
{
    _CheckInit();
    if( m_nCommandNum > 0 ) {
        m_nCommandNum--;
        if( m_nCommandNum == 0 ) { //restore old command line
            m_sCurrentCommandBeg = m_sOldCommand;
            m_sCurrentCommandEnd = "";
        }
        else{
            m_sCurrentCommandBeg = _GetHistory();
            m_sCurrentCommandEnd = "";
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// TODO: Document what this is used for, or remove it
inline bool FLConsoleInstance::_IsConsoleFunc( TrieNode *node )
{
    if( typeid( ConsoleFunc ).name() 
            == ((CVarUtils::CVar<int>*)node->m_pNodeData)->type() ) {
        return true;
    }

    return false; 
}

////////////////////////////////////////////////////////////////////////////////
inline int FLConsoleInstance::_FindRecursionLevel( std::string sCommand )
{
    int level = 0;
    for( unsigned int ii = 0; ii < sCommand.length(); ii++ ) {
        if( sCommand.c_str()[ii]=='.' ) {
            level ++;
        }
    }
    return level;
}



/*
////////////////////////////////////////////////////////////////////////////////
void FLConsoleInstance::PrintAllCVars()
{
    Trie& trie = CVarUtils::TrieInstance();
    TrieNode* node = trie.FindSubStr(  RemoveSpaces( m_sCurrentCommandBeg ) );
    if( !node ) {
        return;
    }

    std::cout << "CVars:" << std::endl;

    // Retrieve suggestions (retrieve all leaves by traversing from current node)
    std::vector<TrieNode*> suggest = trie.CollectAllNodes( node );
    std::sort( suggest.begin(), suggest.end() );
    //output suggestions
    unsigned int nLongestName = 0;
    unsigned int nLongestVal = 0;
    for( unsigned int ii = 0; ii < suggest.size(); ii++ ){
        std::string sName = ( (CVarUtils::CVar<int>*) suggest[ii]->m_pNodeData )->m_sVarName;
        std::string sVal = CVarUtils::GetValueAsString( suggest[ii]->m_pNodeData );
        if( sName.length() > nLongestName ){
            nLongestName = sName.length();
        }
        if( sVal.length() > nLongestVal ){
            nLongestVal = sVal.length();
        }
     }

    if( suggest.size() > 1) {
        for( unsigned int ii = 0; ii < suggest.size(); ii++ ){
            std::string sName = ( (CVarUtils::CVar<int>*) suggest[ii]->m_pNodeData )->m_sVarName;
            std::string sVal = CVarUtils::GetValueAsString( suggest[ii]->m_pNodeData );
            std::string sHelp = CVarUtils::GetHelp( sName ); 
            sName.resize( nLongestName, ' ' );
            sVal.resize( nLongestVal, ' ' );
            printf( "%-s: Default value = %-30s   %-50s\n", sName.c_str(), sVal.c_str(), sHelp.empty() ? "" : sHelp.c_str() );
        }
    }
}
*/

////////////////////////////////////////////////////////////////////////////////
inline std::string FLConsoleInstance::_TabComplete()
{
	_CheckInit();
	Trie& trie = CVarUtils::TrieInstance();
    TrieNode* node = trie.FindSubStr(  RemoveSpaces( m_sCurrentCommandBeg ) );
    if( !node ) {
        return "";
    }
    else if( node->m_nNodeType == TRIE_LEAF || (node->m_children.size() == 0) ) {
        node = trie.Find(m_sCurrentCommandBeg);
        if( !_IsConsoleFunc( node ) ) {
            m_sCurrentCommandBeg += " = " + CVarUtils::GetValueAsString(node->m_pNodeData);
        }
    } else {
        // Retrieve suggestions (retrieve all leaves by traversing from current node)
        std::vector<TrieNode*> suggest = trie.CollectAllNodes(node);
        //output suggestions
        if( suggest.size() > 1) {
            std::vector<std::pair<std::string,int> > suggest_name_index_full;
            std::vector<std::pair<std::string,int> > suggest_name_index_set;
            // Build list of names with index from suggest
            // Find lowest recursion level
            int iMinRecurLevel = 100000;
            for( unsigned int ii = 0; ii < suggest.size(); ii++ ) {
                std::string sName = ( (CVarUtils::CVar<int>*) suggest[ii]->m_pNodeData )->m_sVarName;
                suggest_name_index_full.push_back( std::pair<std::string,int>( sName, ii ) );
                if( _FindRecursionLevel( sName ) < iMinRecurLevel ) {
                    iMinRecurLevel = _FindRecursionLevel( sName );
                }
            }
            // Sort alphabetically (this is useful for later removing
            // duplicate name at a given level and looks nice too...)
            std::sort( suggest_name_index_full.begin(), suggest_name_index_full.end(), StringIndexPairGreater );

            // Remove suggestions at a higher level of recursion
            std::string sCurLevel = "";
            int iCurLevel;
            for( unsigned int ii = 0; ii < suggest_name_index_full.size() ; ii++ ) {
                std::string sCurString = suggest_name_index_full[ii].first;
                iCurLevel = _FindRecursionLevel( sCurString );
                if( sCurLevel.length()==0 ) {
                    if( iCurLevel == iMinRecurLevel ) {
                        sCurLevel = "";    
                        suggest_name_index_set.
                            push_back( std::pair<std::string,int>( sCurString,suggest_name_index_full[ii].second ) );
                    } else {
                        // Add new substring at given level
                        sCurLevel = FindLevel( sCurString, iMinRecurLevel );
                        suggest_name_index_set.push_back( std::pair<std::string,int>( sCurLevel,suggest_name_index_full[ii].second ) );
                    }
                } else {
                    if( sCurString.find( sCurLevel ) == std::string::npos ) {
                        // Add new substring at given level
                        sCurLevel = FindLevel( sCurString, iMinRecurLevel );
                        suggest_name_index_set.push_back( std::pair<std::string,int>( sCurLevel,suggest_name_index_full[ii].second ) );
                    } 
                }
            }

            // Get all commands and function separately
            // Print out all suggestions to the console
            std::string commands, functions; //collect each type separately

            unsigned int longest = 0;
            for( unsigned int ii = 0; ii < suggest_name_index_set.size(); ii++ ) {
                if( suggest_name_index_set[ii].first.length() > longest ) {
                    longest = suggest_name_index_set[ii].first.length();
                }
            }
            longest +=3;

            std::vector<std::string> cmdlines;
            std::vector<std::string> funclines;

            // add command lines
            for( unsigned int ii = 0; ii < suggest_name_index_set.size() ; ii++ ) {
                std::string tmp = suggest_name_index_set[ii].first;
                tmp.resize( longest, ' ' );

// TODO: update for Godot, or remove
//                if( (commands+tmp).length() > m_Viewport.width/m_nCharWidth ) {
//                    cmdlines.push_back( commands );
//                    commands.clear();
//                }

                if( !_IsConsoleFunc( suggest[suggest_name_index_set[ii].second] ) ) {
                    commands += tmp;
                }
            }
            if( commands.length() ) cmdlines.push_back( commands );

            // add function lines
            for( unsigned int ii = 0; ii < suggest_name_index_set.size() ; ii++ ) {
                std::string tmp = suggest_name_index_set[ii].first;
                tmp.resize( longest, ' ' );
// TODO: update for Godot, or remove
//                if( (functions+tmp).length() > m_Viewport.width/m_nCharWidth ) {
//                    funclines.push_back( functions );
//                    functions.clear();
//                }
                if( _IsConsoleFunc( suggest[suggest_name_index_set[ii].second] ) ) {
                    functions += tmp;
                }
            }
            if( functions.length() ) funclines.push_back( functions );

            // enter the results
            if( cmdlines.size() + funclines.size() > 0 ) {
                EnterLogLine( " ", LINEPROP_LOG );
            }
            for( unsigned int ii = 0; ii < cmdlines.size(); ii++ ) {
                EnterLogLine( cmdlines[ii].c_str(), LINEPROP_LOG );
            }
            for( unsigned int ii = 0; ii < funclines.size(); ii++ ) {
                EnterLogLine( funclines[ii].c_str(), LINEPROP_FUNCTION );
            }


            //do partial completion - look for paths with one child down the trie
            int c = m_sCurrentCommandBeg.length();
            while(node->m_children.size() == 1) {
                node = node->m_children.front();
                c++;
            }
            m_sCurrentCommandBeg = suggest_name_index_set[0].first.substr(0, c);
        } else if( suggest.size() == 1 ) {
            // Is this what the use wants? Clear the left bit...
            m_sCurrentCommandEnd = "";
            m_sCurrentCommandBeg = ((CVarUtils::CVar<int>*) suggest[0]->m_pNodeData)->m_sVarName;
        }
    }

	return m_sCurrentCommandBeg;
}

////////////////////////////////////////////////////////////////////////////////
/// commands are in the following form:
// [Command] = value //sets a value
// or
// [Command] //prints out the command's value
inline bool FLConsoleInstance::_ProcessCurrentCommand( bool bExecute )
{
    //trie version
    //int error = 0;
    TrieNode*node;
    std::string sRes;
    bool bSuccess = true;

    Trie& trie = CVarUtils::TrieInstance();
    std::string m_sCurrentCommand = m_sCurrentCommandBeg+m_sCurrentCommandEnd;

    // remove leading and trailing spaces
    int pos = m_sCurrentCommand.find_first_not_of( " ", 0 );
    if( pos >= 0 ) {
        m_sCurrentCommand = m_sCurrentCommand.substr( pos );
    }
    pos = m_sCurrentCommand.find_last_not_of( " " );
    if( pos >= 0 ) {
        m_sCurrentCommand = m_sCurrentCommand.substr( 0, pos+1 );
    }

    // Make sure the command gets added to the history
    if( !m_sCurrentCommand.empty() ) {
        EnterLogLine( m_sCurrentCommand.c_str(), LINEPROP_COMMAND, false );
    }

    // Simply print value if the command is just a variable
    if( ( node = trie.Find( m_sCurrentCommand ) ) ) {
        //execute function if this is a function cvar
        if( _IsConsoleFunc( node ) ) {
            bSuccess &= CVarUtils::ExecuteFunction( m_sCurrentCommand, (CVarUtils::CVar<ConsoleFunc>*) node->m_pNodeData, sRes, bExecute );
            EnterLogLine( m_sCurrentCommand.c_str(), LINEPROP_FUNCTION );
        }
        else { //print value associated with this cvar
            EnterLogLine( ( m_sCurrentCommand + " = " + 
                        CVarUtils::GetValueAsString(node->m_pNodeData)).c_str(), LINEPROP_LOG );
        }
    }
    //see if it is an assignment or a function execution (with arguments)
    else {
        int eq_pos; //get the position of the equal sign
        //see if this an assignment
        if( ( eq_pos = m_sCurrentCommand.find( "=" ) ) != -1 ) {
            std::string command, value;
            std::string tmp = m_sCurrentCommand.substr(0, eq_pos ) ;
            command = RemoveSpaces( tmp );
            value = m_sCurrentCommand.substr( eq_pos+1, m_sCurrentCommand.length() );
            if( !value.empty() ) { 
                value = RemoveSpaces( value );
                if( ( node = trie.Find(command) ) ) {
                    if( bExecute ) {
                        CVarUtils::SetValueFromString( node->m_pNodeData, value );
                    }
                    EnterLogLine( ( command + " = " + value ).c_str(), LINEPROP_LOG );
                }
            }
            else {  
                if( bExecute ) {
                    std::string out = "-glconsole: " + command + ": command not found"; 
                    EnterLogLine( out.c_str(), LINEPROP_ERROR );
                }
                bSuccess = false;
            }
        }
        //check if this is a function
        else if( ( eq_pos = m_sCurrentCommand.find(" ") ) != -1 ) {
            std::string function;
            function = m_sCurrentCommand.substr( 0, eq_pos );
            //check if this is a valid function name
            if( ( node = trie.Find( function ) ) && _IsConsoleFunc( node ) ) {
                bSuccess &= CVarUtils::ExecuteFunction( m_sCurrentCommand, (CVarUtils::CVar<ConsoleFunc>*)node->m_pNodeData, sRes, bExecute );
                EnterLogLine( m_sCurrentCommand.c_str(), LINEPROP_FUNCTION );
            }
            else {
                if( bExecute ) {
                    std::string out = "-glconsole: " + function + ": function not found"; 
                    EnterLogLine( out.c_str(), LINEPROP_ERROR );
                }
                bSuccess = false;
            }
        }
        else if( !m_sCurrentCommand.empty() ) {
            if( bExecute ) {
                std::string out = "-glconsole: " + m_sCurrentCommand + ": command not found"; 
                EnterLogLine( out.c_str(), LINEPROP_ERROR );
            }
            bSuccess = false;
        }
        else { // just pressed enter
            EnterLogLine( " ", LINEPROP_LOG );
        }
    }

    return bSuccess;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Help interpreter function.
//  With no arguments prints out help from FLCONSOLE_HELPFILE
inline bool FLConsoleInstance::Help(std::vector<std::string> *vArgs)
{
    if( vArgs && vArgs->size() != 0 )
    {
        for( size_t i = 0; i < vArgs->size(); i++ ) {
            try {
                PrintHelp( "help for %s", vArgs->at(i).c_str() );
                if(CVarUtils::GetHelp( vArgs->at(i) ).empty())
                    PrintHelp( "No help available." );
                else
                    PrintHelp( "%s", CVarUtils::GetHelp( vArgs->at(i) ).c_str() );
            }
            catch( CVarUtils::CVarException e ) {
                PrintHelp( "Unknown variable %s.", vArgs->at(i).c_str() );
                return false;
            }
        }
    }
    else {
        //try opening a default helpfile
        //else use the built in help
        std::ifstream sIn(FLCONSOLE_HELP_FILE  );
        if( sIn.is_open() ) {
            char s[1024];
            while(sIn.good()) {
                sIn.getline(s, 1024);
                PrintHelp(s);
            }
            sIn.close();
        }
        else{
            std::cerr << "WARNING: No custom " << FLCONSOLE_HELP_FILE << ". Using default FLConsole help." << std::endl;

            PrintHelp("");
            PrintHelp("[color=grey]----------------- HELP -----------------[/color]");
            PrintHelp("[color=grey]Press ~ key to open and close console[/color]");
            PrintHelp("[color=grey]Press TAB to see the available commands and functions[/color]");
            PrintHelp("[color=grey]Functions are shown in green and variables in yellow[/color]");
            PrintHelp("[color=grey]Setting a value: [command] = value[/color]");
            PrintHelp("[color=grey]Getting a value: [command][/color]");
            PrintHelp("[color=grey]Functions: [function] [arg1] [arg2] ...[/color]");
            PrintHelp("[color=grey]Entering just the function name will give a description.[/color]");
            PrintHelp("[color=grey]History: Up and Down arrow keys move through history.[/color]");
            PrintHelp("[color=grey]Tab Completion: TAB does tab completion and makes suggestions.[/color]");
            PrintHelp("[color=grey]----------------- HELP -----------------[/color]");
            PrintHelp("");
        }
    }
    return true;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Save settings current console settings to a file
//  @param sFileName the file to save to. if none given the one specified in
//  console.SettingsFileName is use 
//  @return sucess or failure
inline bool FLConsoleInstance::SettingsSave(std::string sFileName)
{
    if( !m_bExecutingHistory ) {
        if( sFileName == ""){
            if(m_sSettingsFileName != "") {
                sFileName = m_sSettingsFileName;
            }
            else {
                PrintError( "Warning: No default name. Resetting settings filename to: \"%s\".", FLCONSOLE_SETTINGS_FILE );
                sFileName = m_sHistoryFileName = FLCONSOLE_SETTINGS_FILE;
            }
        }

        std::ofstream ofs( sFileName.c_str() );

        if( !ofs.is_open() ) {
            PrintError("Error: could not open \"%s\" for saving.", sFileName.c_str());
            return false;
        }

        std::vector<std::string> vSave;
        vSave.push_back(sFileName);
        vSave.push_back("console");
        vSave.push_back("script");
        ConsoleSave( &vSave );

        ofs.close();
    }
    return true;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Load console settings from a file.
//  @param sFileName the file to load from. if none given the one specified in
//  console.SettingsFileName is used
//  @return sucess or failure
inline bool FLConsoleInstance::SettingsLoad( std::string sFileName )
{
    if( sFileName == "" ) {
        if( m_sSettingsFileName != "" ) {
            sFileName = m_sSettingsFileName;
        }
        else {
            PrintError( "Warning: No default name. Resetting settigns filename to: \"%s\".", FLCONSOLE_SETTINGS_FILE );
            sFileName = m_sSettingsFileName = FLCONSOLE_SETTINGS_FILE;
        }
    }

    //test if file exists
    std::ifstream ifs( sFileName.c_str() );

    if( ifs.is_open() ) {
        ifs.close();
        std::vector<std::string> v;
        v.push_back(sFileName);
        ConsoleLoad(&v);
    }
    else {
//        std::cout << "Info: Settings file, " << sFileName << ", not found." << std::endl;
        return false;
    }

    return true;
}

#endif
