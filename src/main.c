/******************************************************************************/
/* change title on for new project                                            */
/******************************************************************************/

/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/

// ---------------------------------------------------------
// system
// ---------------------------------------------------------
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------
#include "main.h"

#include <strfunc.h>
#include <sendmsg.h>

/******************************************************************************/
/*   D E F I N E S                                                            */
/******************************************************************************/
#define VERSION "TEST"

/******************************************************************************/
/*   M A C R O S                                                              */
/******************************************************************************/

/******************************************************************************/
/*   P R O T O T Y P E S                                                      */
/******************************************************************************/
void initMessageGroups() ;

/******************************************************************************/
/*                                                                            */
/*                                  M A I N                                   */
/*                                                                            */
/******************************************************************************/

int main(int argc, const char* argv[] )
{
  int sysRc ;

  tSendresult result  ;
  tSendreturn response;

  char message[] = "status+15 krpan.mqDlq green Mon Aug 26 CEST 2013 green hello world" ;

  initMessageGroups() ;
  printMessageStruct() ;
#if(1)
  result = sendmessage( message, &response );
#endif
}


/******************************************************************************/
/*                                                                            */
/*   F U N C T I O N S                                                        */
/*                                                                            */
/******************************************************************************/
void initMessageGroups()
{
  addMessageBox( "mqDlq" ) ;
  addMessageBox( "mqQ" ) ;
  addMessageGroup( "mqQ", "QLOCAL" );

#if(0)
  addMessageGroup( createMessageGroup( "QLOCAL" ) );
  addMessageGroup( createMessageGroup( "QREMOTE" ) );
#endif
}
