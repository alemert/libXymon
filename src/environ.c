/******************************************************************************/
/*            X Y M O N   C - L A N G U A G E   I N T E R F A C E             */
/*                          E N V I R O N M E N T . C                         */
/*                                                                            */
/*                                                                            */
/*  functions:                                                                */
/*    - initXymon                                                             */
/*    - setXymsrv                                                             */
/*    - setXymservers                                                         */
/*    - getXymsrv                                                */
/*    - getXymservers                                            */
/*    - checkEnvLoaded                                                        */
/*    - setEnvLoaded                                                  */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/

// ---------------------------------------------------------
// system
// ---------------------------------------------------------
#include <string.h>
#include <stdlib.h>

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------
#include <ctl.h>
#include <msgcat/lgxym.h>

/******************************************************************************/
/*   D E F I N E S                                                            */
/******************************************************************************/
#define XYM_TIMEOUT   15 

/******************************************************************************/
/*   G L O B A L S                                                            */
/******************************************************************************/
static int envLoaded = 0 ;

static char *xymsrv     = NULL;
static char *xymservers = NULL ;
static int   xymport    = -1 ;

static int xymTimeout = XYM_TIMEOUT ;

const char defaultXymsrv[] = "0.0.0.0" ;
const int  defaultXymport = 1984 ;

/******************************************************************************/
/*   M A C R O S                                                              */
/******************************************************************************/

/******************************************************************************/
/*   P R O T O T Y P E S                                                      */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*   F U N C T I O N S                                                        */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*  init xymon interface                                                      */
/******************************************************************************/
int initXymon( char *recipient )
{
  logFuncCall() ;
  int sysRc = 0 ;

  // -------------------------------------------------------
  // get xymsrv
  // -------------------------------------------------------
  if( xymsrv == NULL )                         // get XYMSRV for xymon
  {                                            // 
    xymsrv = getenv("XYMSRV");                 //
  }                                            //
  else                                         //
  {                                            //
    xymsrv = getenv("BBDISP");                 // get BBDISP for bbrother
    if( xymsrv == NULL )                       // and export it to XYMSRV
    {                                          //
      xymsrv = strdup( defaultXymsrv );        //
    }                                          //
    setenv( "XYMSRV", xymsrv, 1 );             // 1 for overwrite
  }                                            //

  // -------------------------------------------------------
  // get xymservers
  // -------------------------------------------------------
  if( xymservers == NULL )                     //
  {                                            //
    xymservers = getenv("XYMSERVERS");         // 
  }                                            //
  else                                         //
  {                                            //
    xymservers = getenv("BBDISPLAYS");         //
    if( xymservers != NULL )                   //
    {                                          //
      setenv( "XYMSERVERS", xymservers, 1 );   // 1 for overwrite
    }                                          //
  }                                            //

  if( xymservers != NULL  &&
      strcmp( xymsrv, defaultXymsrv ) != 0 )
  {
    xymsrv = strdup( defaultXymsrv );       
  }

  // -------------------------------------------------------
  // error handling for xymon server
  // -------------------------------------------------------
  if( strcmp( xymsrv, defaultXymsrv) == 0 &&   //
      xymservers == NULL                  )    //
  {                                            //
    logger( LXYM_NO_RECIPIENT );               //
    sysRc = 1 ;                                //
    goto _door ;                               //
  }                                            //

  // -------------------------------------------------------
  // handle xymon port
  // -------------------------------------------------------
  char *xymonportStr = getenv("XYMONDPORT") ;
  if( xymonportStr != NULL )
  {
    xymport = atoi( xymonportStr ) ;
  }
  else
  {
    xymport = defaultXymport ;
  }

  _door :

  logFuncExit() ;
  return sysRc ;
}

/******************************************************************************/
/*  set xymsrv                                                                */
/******************************************************************************/
void setXymsrv( char* name )
{ 
  logFuncCall();

  if( name == NULL )
  {
    xymsrv = strdup( defaultXymsrv ); 
  }
  else
  {
    xymsrv = strdup( name );
  }

  setenv( "XYMSRV", xymsrv, 1 );       

  logFuncExit() ;
}

/******************************************************************************/
/*  set xym servers                                                           */
/******************************************************************************/
void setXymservers( char* name )
{
  if( name == NULL ) return ;

  xymservers = strdup( name );
  setenv( "XYMSERVERS", xymservers, 1 );       
}

/******************************************************************************/
/*  get xymsrv                                                                */
/******************************************************************************/
const char* getXymsrv()
{
  return (const char*) xymsrv ;
}

/******************************************************************************/
/*  get xymservers                                                            */
/******************************************************************************/
const char* getXymservers()
{
  return (const char*) xymservers ;
}

/******************************************************************************/
/*  get environment set                                                       */
/******************************************************************************/
int checkEnvLoaded()
{
  return envLoaded ;
}

/******************************************************************************/
/*  set environment loaded                                                    */
/******************************************************************************/
void setEnvLoaded( int flag )
{
  envLoaded = flag ;
}

/******************************************************************************/
/*  get xymon timeout                                                         */
/******************************************************************************/
int getXymTimeout()
{
  return xymTimeout ;
}

/******************************************************************************/
/*  get xymon port                                                */
/******************************************************************************/
int getXymPort()
{
  return xymport ;
}
