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
void initMessageCfg() ;

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

  initMessageCfg() ;
  printMessageStructCfg() ;

  tXymMsgGrpData *grp  ;
  tXymMsgLine    *line ;
  grp = addMessageGroup( "krpan", "mqDlq", "" );
  grp = addMessageGroup( "krpan", "mqQ", "QLOCAL" );
  line = addMessageLine( grp );
  setMessageItem( line, "CURDEPTH", ERR, (tXymMsgValue) 1  );
  setMessageItem( line, "MSGAGE",   OK , (tXymMsgValue) 2  );
  setMessageItem( line, "PUT",      OK , (tXymMsgValue) (char*) "ENABLE" );
  setMessageItem( line, "GET",      OK , (tXymMsgValue) (char*) "ENABLE" );
  line = addMessageLine( grp );
  setMessageItem( line, "MSGAGE",   OK, (tXymMsgValue) 21  );
  setMessageItem( line, "CURDEPTH", OK, (tXymMsgValue) 22  );
  setMessageItem( line, "PUT",      ERR, (tXymMsgValue) (char*) "DISABLE" );
  setMessageItem( line, "GET",      ERR, (tXymMsgValue) (char*) "DISABLE" );
  line = addMessageLine( grp );
  setMessageItem( line, "MSGAGE",   WAR, (tXymMsgValue) 31  );
  setMessageItem( line, "PUT",      WAR, (tXymMsgValue) (char*) "ENABLE" );
  setMessageItem( line, "GET",      ERR, (tXymMsgValue) (char*) "DISABLE" );
  line = addMessageLine( grp );
  setMessageItem( line, "CURDEPTH", OK, (tXymMsgValue) 42  );
  grp = addMessageGroup( "krpan", "mqDlq", "" );
  line = addMessageLine( grp );
  setMessageItem( line, "REASON", ERR, (tXymMsgValue) 2038  );
  char buff[64];
  char *txt = (char*) buff ;
  
  sprintf( txt, "28.04.1970" );
  setMessageItem( line, "DATE", NA, (tXymMsgValue) txt );
  setMessageItem( line, "TIME", NA, (tXymMsgValue) (char*)"19:30"  );
  

//addMessageLine( grp );
 
  printMessageStructData();


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
void initMessageCfg()
{
  addMessageBoxCfg(   "mqDlq" ) ;
  addMessageItemCfg(  "mqDlq", "", "REASON" );
  addMessageItemCfg(  "mqDlq", "", "DATE"   );
  addMessageItemCfg(  "mqDlq", "", "TIME"   );
  addMessageBoxCfg(   "mqQ" ) ;
  addMessageGroupCfg( "mqQ", "QLOCAL" );
  addMessageItemCfg(  "mqQ", "QLOCAL" , "CURDEPTH" );


  setMessageItemCfg( addMessageItemCfg(  "mqDlq", "", "REASON" ),
                     4, RIGHT, INT );
  setMessageItemCfg( addMessageItemCfg(  "mqDlq", "", "DATE"   ),
                     10, LEFT, STRING );
  setMessageItemCfg( addMessageItemCfg(  "mqDlq", "", "TIME"   ),
                     12, LEFT, STRING );

  setMessageItemCfg( addMessageItemCfg( "mqQ", "QLOCAL" , "MSGAGE" ), 
                     8, RIGHT, INT );
  setMessageItemCfg( addMessageItemCfg(  "mqQ", "QLOCAL" , "CURDEPTH" ),
                     8, RIGHT, INT );
  setMessageItemCfg( addMessageItemCfg( "mqQ", "QLOCAL", "PUT" ), 
                     7, LEFT, STRING );
  setMessageItemCfg( addMessageItemCfg( "mqQ", "QLOCAL", "GET" ), 
                     7, LEFT, STRING );

  setMessageItemCfg( addMessageItemCfg( "mqQ", "QREMOTE", "PUT" ), 
                     7, LEFT, STRING );
  setMessageItemCfg( addMessageItemCfg( "mqQ", "QREMOTE", "GET" ), 
                     7, LEFT, STRING );

}
