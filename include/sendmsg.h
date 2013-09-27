/******************************************************************************/
/*  xymon librarie - send message - header                                    */
/******************************************************************************/
#ifndef  _SENDMMESSAGE_H_
#define  _SENDMMESSAGE_H_
/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/
// ---------------------------------------------------------
// system
// ---------------------------------------------------------
#include <stdio.h>

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------
#include <strfunc.h>

/******************************************************************************/
/*   D E F I N E S                                                            */
/******************************************************************************/

/******************************************************************************/
/*   T Y P E S                                                                */
/******************************************************************************/
typedef enum eSendresult tSendresult ;
typedef struct sSendreturn tSendreturn ;

/******************************************************************************/
/*   S T R U C T S                                                            */
/******************************************************************************/
enum eSendresult 
{
  XYMONSEND_OK                ,
  XYMONSEND_EBADIP            ,
  XYMONSEND_EIPUNKNOWN        ,
  XYMONSEND_ENOSOCKET         ,
  XYMONSEND_ECANNOTDONONBLOCK ,
  XYMONSEND_ECONNFAILED       ,
  XYMONSEND_ESELFAILED        ,
  XYMONSEND_ETIMEOUT          ,
  XYMONSEND_EWRITEERROR       ,
  XYMONSEND_EREADERROR        ,
  XYMONSEND_EBADURL
} ;

struct sSendreturn
{
  FILE *respfd;
  tStrbuffer *respstr;
//  int fullresponse;
//  int haveseenhttphdrs;
} ;

/******************************************************************************/
/*   G L O B A L E S                                                          */
/******************************************************************************/

/******************************************************************************/
/*   M A C R O S                                                              */
/******************************************************************************/

/******************************************************************************/
/*   P R O T O T Y P E S                                                      */
/******************************************************************************/
//tSendresult sendmessage( char *message, tSendreturn *response );
tSendresult sendmessage( char* host, 
                         char* test, 
                         char* level,
                         char *message, 
                         int tiemout,
                         tSendreturn *response );

tSendresult sendtomany( const char* xymsrv    ,
                        const char* xymservers,
                        char* msgHeader       ,
                        char* msg             ,
                        int timeout           ,
                        tSendreturn *response );

int sendtoxymond( char *recipient ,
                  char *msgHead   ,
                  char *message   ,
                  FILE *respfd    ,
                  char **respstr  ,
                  int fullresponse,
                  int timeout    );
#endif
