/******************************************************************************/
/*            X Y M O N   C - L A N G U A G E   I N T E R F A C E             */
/*                                                                  */
/*                        S E N D   M E S S A G E . C                         */
/*                                                  */
/*  functions:                                */
/*    - sendmessage                          */
/*    - sendtomany                      */
/*                                              */
/******************************************************************************/

/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/

// ---------------------------------------------------------
// system
// ---------------------------------------------------------
#include <string.h>

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------
#include <sendmsg.h>
#include <environ.h>

#include <ctl.h>
#include <msgcat/lgstd.h>

/******************************************************************************/
/*   G L O B A L S                                                            */
/******************************************************************************/

/******************************************************************************/
/*   D E F I N E S                                                            */
/******************************************************************************/

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
/*  send message                            */
/******************************************************************************/
tSendresult sendmessage( char *message, tSendreturn *response )
{
  logFuncCall() ;
  tSendresult result = XYMONSEND_OK ;

  result = sendtomany( getXymsrv()     , 
                       getXymservers() , 
                       message         , 
                       getXymTimeout() , 
                       response       );



  logFuncExit() ;
  return result ;
}

/******************************************************************************/
/*  send message to many deamons                        */
/******************************************************************************/
tSendresult sendtomany( const char* onercpt   , 
                        const char* morercpt  ,
                        char* message         , 
                        const int timeout     ,
                        tSendreturn *response )
{
  logFuncCall() ;
  tSendresult result = XYMONSEND_OK ;

  // -------------------------------------------------------
  // ?? send to one ore many server ??
  // -------------------------------------------------------
  int allservers = 1;              // Even though this is the "sendtomany" 
                                   // routine, we need to decide if the request
  if(strcmp(onercpt,"0.0.0.0")!=0) // should go to all servers,or just a single
  {                                // server.The default is to send to all
    allservers = 0;                // servers-but commands that trigger a 
  }                                // response can only go to a single server.
                                   //
  // -------------------------------------------------------
  // build a list of all server 
  // -------------------------------------------------------
   char *xymondlist ;

  if( strcmp(onercpt,"0.0.0.0") == 0 )  // send to many is necessary
  {                              //
    xymondlist = strdup(morercpt);      // send to many
  }                        //
  else                        //
  {                        //
    xymondlist = strdup(onercpt);       // send to one only
  }                            //
       //
  // -------------------------------------------------------
  // split xymondlist into single servers sepperated by blank or tab
  // -------------------------------------------------------
  rcpt = strtok(xymondlist, " \t");

  logFuncExit() ;
  return result ; 
}

