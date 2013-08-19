/******************************************************************************/
/*            X Y M O N   C - L A N G U A G E   I N T E R F A C E             */
/*                                                                            */
/*                        S E N D   M E S S A G E . C                         */
/*                                                                            */
/*  functions:                                                        */
/*    - sendmessage                                              */
/*    - sendtomany                                          */
/*    - sendtoxymond                                      */
/*                                                                      */
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
#include <sendmsg.h>
#include <environ.h>
#include <strfunc.h>

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
/*  send message                                                      */
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
/*  send message to many deamons                                  */
/******************************************************************************/
tSendresult sendtomany( const char* onercpt   , 
                        const char* morercpt  ,
                        char* message         , 
                        const int timeout     ,
                        tSendreturn *response )
{
  logFuncCall() ;
  tSendresult rc = XYMONSEND_OK ;

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
  int first = 1;                   // flag identifing sending to first server
                                   //
  // -------------------------------------------------------
  // build a list of all server 
  // -------------------------------------------------------
   char *xymondlist ;

  if( strcmp(onercpt,"0.0.0.0") == 0 )  // send to many is necessary
  {                                     //
    xymondlist = strdup(morercpt);      // send to many
  }                                     //
  else                                  //
  {                                     //
    xymondlist = strdup(onercpt);       // send to one only
  }                                     //
                                        //
  // -------------------------------------------------------
  // split xymondlist into single servers sepperated by blank or tab
  // -------------------------------------------------------
  char *rcpt ;
  rcpt = strtok(xymondlist, " \t");

  while( rcpt )
  {
    if( first )                  // grab the result from the first server
    {                            //
      char *respstr = NULL;  //
                                 //
      if( response )         //
      {                          //
        rc = sendtoxymond( rcpt, 
                           message,  
                           response->respfd ,
                           (response->respstr ? &respstr : NULL),
                           (response->respfd || response->respstr),
                           timeout      );

      }
      else
      {
        rc = sendtoxymond( rcpt     ,
                           message  , 
                           NULL     , 
                           NULL     ,
                           0        , 
                           timeout );
      }

      if( rc == XYMONSEND_OK )
      {
        if( respstr && response && response->respstr ) // respstr != NULL (true)
        {                                              //
          add2buffer( response->respstr, respstr );    // free respstr possible
          free( respstr );                             // since resptr != NULL
        }                                              //
        first = 0;              // send msg to first server ok
      }                          //
    }                      //
    else                            // secondary server do not yield a response 
    {                        // 
      rc = sendtoxymond( rcpt     ,
                         message  , 
                         NULL     , 
                         NULL     ,
                         0        , 
                         timeout );
    }
                             //
    if( allservers || first )       // Handle more servers IF we're doing 
    {                                   // all servers, OR we are still at the 
      rcpt = strtok( NULL, " \t" ); // first one (because the previous ones 
    }                                   // failed).
    else                                //
    {                                   //
      rcpt = NULL;                      //
    }                                   //
  }                                     //
                        //
  if( xymondlist ) free( xymondlist );  //
                        //
  logFuncExit() ;
  return rc ; 
}


/******************************************************************************/
/*  send to xymon deamon                                */
/******************************************************************************/
#if(0)
int sendtoxymond( char *recipient ,
                  char *message   ,
                  FILE *respfd    ,
                  char **respstr  ,
                  int fullresponse,
                  int timeout     )
{
  return 0 ;
}
#endif
