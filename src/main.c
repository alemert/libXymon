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
#include <message.h>

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
  int sysRc = 0 ;

  tSendresult result  ;
  tSendreturn response;

  char message[] = "status+15 krpan.mqDlq green Mon Aug 26 CEST 2013 green hello world" ;

  initMessageGroups() ;
  printMessageStruct() ;

  return sysRc ;
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
  addMessageBoxCfg(   "mqDlq" ) ;
  addMessageItemCfg(  "mqDlq", "", "REASON" );
  addMessageItemCfg(  "mqDlq", "", "DATE"   );
  addMessageItemCfg(  "mqDlq", "", "TIME"   );
  addMessageBoxCfg(   "mqQ" ) ;
  addMessageGroupCfg( "mqQ", "QLOCAL" );
  addMessageItemCfg(  "mqQ", "QLOCAL" , "CURDEPTH" );
  setMessageItemCfg( addMessageItemCfg( "mqQ", "QLOCAL" , "MSGAGE" ), 
                     9, RIGHT, INT );

  setMessageItemCfg( addMessageItemCfg( "mqQ", "QREMOTE", "PUT" ), 
                     7, LEFT, STRING );
  setMessageItemCfg( addMessageItemCfg( "mqQ", "QREMOTE", "GET" ), 
                     7, LEFT, STRING );

}
