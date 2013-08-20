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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
//#include <unistd.h>
#include <errno.h>

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------
#include <sendmsg.h>
#include <environ.h>
#include <strfunc.h>

#include <ctl.h>
#include <msgcat/lgstd.h>
#include <msgcat/lgxym.h>

/******************************************************************************/
/*   G L O B A L S                                                            */
/******************************************************************************/

/******************************************************************************/
/*   D E F I N E S                                                            */
/******************************************************************************/
#define IP_ADDR_STRLEN 16

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
    if( first )                                  // grab the result from 
    {                                            // the first server
      char *respstr = NULL;                      //
                                                 //
      if( response )                             //
      {                                          //
        rc = sendtoxymond( rcpt   ,              //
                           message,              //
                           response->respfd,     //
                           (response->respstr ? &respstr : NULL),
                           (response->respfd || response->respstr),
                           timeout      );       //
      }                                          //
      else                                       //
      {                                          //
        rc = sendtoxymond( rcpt     ,            //
                           message  ,            //
                           NULL     ,            //
                           NULL     ,            //
                           0        ,            //
                           timeout );            //
      }                                          //
                                                 //
      if( rc == XYMONSEND_OK )                   //
      {                                          //
        if( respstr          &&                  // respstr != NULL (true)
            response         &&                  //
            response->respstr )                  //
        {                                        //
          add2buffer(response->respstr,respstr); // free respstr possible
          free( respstr );                       // since resptr != NULL
        }                                        //
        first = 0;                               // send msg to 1st server done
      }                                          //
    }                                            //
    else                                         // secondary server do not 
    {                                            // yield a response 
      rc = sendtoxymond( rcpt     ,              //
                         message  ,              //
                         NULL     ,              //
                         NULL     ,              //
                         0        ,              //
                         timeout );              //
    }                                            //
                                                 //
    if( allservers || first )                    // Handle more servers IF we're
    {                                            // doing still all servers, OR 
      rcpt = strtok( NULL, " \t" );              // we are at the first one 
    }                                            // (because the previous ones 
    else                                         // failed).
    {                                            //
      rcpt = NULL;                               //
    }                                            //
  }                                              //
                                                 //
  if( xymondlist ) free( xymondlist );           //
                                                 //
  logFuncExit() ;
  return rc ; 
}


/******************************************************************************/
/*  send to xymon deamon                                            */
/******************************************************************************/
int sendtoxymond( char *recipient ,    // deamon ip adress
                  char *message   ,    // message
                  FILE *respfd    ,    // response file descriptor
                  char **respstr  ,    // response string
                  int fullresponse,    // full response
                  int timeout     )    // timeout
{
  int sysRc = XYMONSEND_OK ;
#if(0)
  if( dontsendmessages && !respfd && !respstr )
  {
    goto _door;
  }
#endif

  // -------------------------------------------------------
  // http not wanted 
  //
  // http not supported 
  // http code is missing 
  // -------------------------------------------------------

  // -------------------------------------------------------
  // Non-HTTP transport 
  // lookup portnumber in both XYMONDPORT env.
  // and the "xymond" entry from /etc/services.
  // -------------------------------------------------------

  int port = getXymPort() ;
  struct hostent *hent;         // get host by name buffer
  struct in_addr addr;          // ip adress in binary form
  char hostip[IP_ADDR_STRLEN];  // ip adress string buffer
                                //
  struct sockaddr_in sockAddr;  // ip socket addres
  int    sockfd = -1;      // socket file descriptor

  // -------------------------------------------------------
  // check if reipient is an IP or DNS
  // -------------------------------------------------------
  if( inet_aton( recipient, &addr ) == 0 ) // recipient is DNS (not IP)
  {                                        //
    hent = gethostbyname( recipient );     // dns to ip
    if( hent )                             //
    {                                      //
      memcpy( &addr                   ,    // 
              *(hent->h_addr_list)    ,    //
              sizeof( struct in_addr) );   //
                                           //
      strcpy( hostip, inet_ntoa(addr) );   //
                                           //
      if( inet_aton( hostip, &addr) == 0 ) // DNS to IP to binary IP
      {                                    //
        sysRc = XYMONSEND_EBADIP;          //
        goto _door ;                       //
      }                                    //
    }                                      //
    else                                   //
    {                                      //
      logger( LSTD_GETHOST_BY_NAME_ERROR, recipient );
      sysRc = XYMONSEND_EIPUNKNOWN;        //
      goto _door;                          //
    }                                      //
  }                                        //
                                           //
  // -------------------------------------------------------
  // set internet adress
  // -------------------------------------------------------
  memset( &sockAddr,0,sizeof(sockAddr) );  // flush memory
  sockAddr.sin_family = AF_INET;           // type = Internet Protocol Adress
  sockAddr.sin_addr.s_addr = addr.s_addr;  // copy internet adress
  sockAddr.sin_port = htons(port);         // set port
                                           //
  // -------------------------------------------------------
  // setup socket
  // -------------------------------------------------------
  sockfd = socket(PF_INET,SOCK_STREAM,0);  // open socket
  if( sockfd == -1 )            //
  {                                        //
    sysRc = XYMONSEND_ENOSOCKET ;      //
    goto _door;                          //
  }                                        //
                                           //
  if(fcntl(sockfd,F_SETFL,O_NONBLOCK)!=0)  //  set socket non-blocking
  {                                    //
    sysRc = XYMONSEND_ECANNOTDONONBLOCK;   //
    goto _door;                          //
  }                                    //
                                      //
  // -------------------------------------------------------
  // connect to xymond
  // -------------------------------------------------------
  if( ( connect( sockfd                     ,
                 (struct sockaddr*)&sockAddr,
                 sizeof(sockAddr)           ) == -1 )
      &&  
      ( errno != EINPROGRESS )  )
  {
    logger( LXYM_CONNECT_ERROR, recipient, port, strerror(errno) );
    sysRc = XYMONSEND_ECONNFAILED;
    goto _door;
  }
                                           //
  _door :

  return sysRc ;
}
