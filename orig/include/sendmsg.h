/******************************************************************************/
/* xymon library      */
/******************************************************************************/

#ifndef __SENDMSG_H_
#define __SENDMSG_H_

/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/
// ---------------------------------------------------------
// system
// ---------------------------------------------------------

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------
#include <libxymon.h>

/******************************************************************************/
/*   D E F I N E S                                                            */
/******************************************************************************/
#define XYMON_TIMEOUT 15 // Default timeout for a request going to Xymon server

/******************************************************************************/
/*   T Y P E S                                                                */
/******************************************************************************/
typedef enum {
        XYMONSEND_OK,
        XYMONSEND_EBADIP,
        XYMONSEND_EIPUNKNOWN,
        XYMONSEND_ENOSOCKET,
        XYMONSEND_ECANNOTDONONBLOCK,
        XYMONSEND_ECONNFAILED,
        XYMONSEND_ESELFAILED,
        XYMONSEND_ETIMEOUT,
        XYMONSEND_EWRITEERROR,
        XYMONSEND_EREADERROR,
        XYMONSEND_EBADURL
} sendresult_t;

typedef struct sendreturn_t 
{
  FILE *respfd;
  strbuffer_t *respstr;
  int fullresponse;
  int haveseenhttphdrs;
} sendreturn_t;

/******************************************************************************/
/*   S T R U C T S                                                            */
/******************************************************************************/

/******************************************************************************/
/*   G L O B A L E S                                                          */
/******************************************************************************/

/******************************************************************************/
/*   M A C R O S                                                              */
/******************************************************************************/

/******************************************************************************/
/*   P R O T O T Y P E S                                                      */
/******************************************************************************/
sendresult_t sendmessage( char *msg      ,
                          char *recipient,
                          int timeout  ,
                          sendreturn_t *response) ;

int sendtomany( char *onercpt,
                       char *morercpts  ,
                       char *msg    ,
                       int timeout    ,
                       sendreturn_t *response) ;

int sendtoxymond( char *recipient   ,
                         char *message     ,
                         FILE *respfd      ,
                         char **respstr    ,
                         int fullresponse  ,
                         int timeout      );

void setup_transport(char *recipient) ;
const char *xymonSendState2str( int id ) ;
void setproxy( char *proxy ) ;
sendreturn_t *newsendreturnbuf( int fullresponse, FILE *respfd ) ;

#endif
