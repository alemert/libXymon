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
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_A"  );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );
#if(1)
  line = addMessageLine( grp );
  setMessageItem( line, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_1"  );
  setMessageItem( line, "CURDEPTH", ERR, (tXymMsgValue) 1  );
  setMessageItem( line, "MSGAGE",   OK , (tXymMsgValue) 2  );
  setMessageItem( line, "PUT",      OK , (tXymMsgValue) (char*) "ENABLE" );
  setMessageItem( line, "GET",      OK , (tXymMsgValue) (char*) "ENABLE" );
  line = addMessageLine( grp );
  setMessageItem( line, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_2"  );
  setMessageItem( line, "MSGAGE",   OK, (tXymMsgValue) 21  );
  setMessageItem( line, "CURDEPTH", OK, (tXymMsgValue) 22  );
  setMessageItem( line, "PUT",      ERR, (tXymMsgValue) (char*) "DISABLE" );
  setMessageItem( line, "GET",      ERR, (tXymMsgValue) (char*) "DISABLE" );
  line = addMessageLine( grp );
  setMessageItem( line, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_3"  );
  setMessageItem( line, "MSGAGE",   WAR, (tXymMsgValue) 31  );
  setMessageItem( line, "PUT",      WAR, (tXymMsgValue) (char*) "ENABLE" );
  setMessageItem( line, "GET",      ERR, (tXymMsgValue) (char*) "DISABLE" );
  addMessageLine( grp );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_4"  );
  setMessageItem( NULL, "MSGAGE",   IGNORE, (tXymMsgValue) 31  );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_5"  );
  addMessageLine( grp );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_6"  );
  setMessageItem( NULL, "MSGAGE",   IGNORE, (tXymMsgValue) 32  );
  addMessageLine( grp );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_7"  );
  setMessageItem( NULL, "MSGAGE",   DISABLE, (tXymMsgValue) 33  );
  addMessageLine( grp );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_8"  );
  setMessageItem( NULL, "MSGAGE",   DISABLE, (tXymMsgValue) 34  );
  addMessageLine( grp );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_9"  );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );
  addMessageLine( grp );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_10"  );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );

  line = addMessageLine( grp );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_11"  );
  setMessageItem( line, "CURDEPTH", OK, (tXymMsgValue) 42  );
  addMessageLine( grp );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_12"  );
  addMessageLine( grp );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_13"  );
  addMessageLine( grp );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_14"  );
  addMessageLine( grp );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_15"  );
  addMessageLine( grp );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_16"  );
  addMessageLine( grp );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_17"  );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );
  addMessageLine( grp );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_18"  );
  addMessageLine( grp );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_19"  );
  addMessageLine( grp );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_20"  );
  addMessageLine( grp );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_21"  );
  addMessageLine( grp );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_22"  );
  addMessageLine( grp );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_23"  );
  addMessageLine( grp );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_24"  );
  addMessageLine( grp );
  setMessageItem( NULL, "GET",   DISABLE, (tXymMsgValue) (char*) "DISABLE"  );
  setMessageItem( NULL, "PUT",   ERR, (tXymMsgValue) (char*) "ENABLE"  );
  setMessageItem( NULL, "MSGAGE", WAR, (tXymMsgValue) 12345  );
  setMessageItem( NULL, "CURDEPTH", IGNORE, (tXymMsgValue) 76543210  );
  setMessageItem( NULL, "QUEUE", SHOW, (tXymMsgValue) (char*) "QL_456789_123456789_123456789"  );
#endif
  grp = addMessageGroup( "krpan", "mqDlq", "" );
  line = addMessageLine( grp );
  setMessageItem( line, "REASON", ERR, (tXymMsgValue) 2038  );
  char buff[64];
  char *txt = (char*) buff ;
  
  sprintf( txt, "28.04.1970" );
  setMessageItem( line, "DATE", SHOW, (tXymMsgValue) txt );
  setMessageItem( line, "TIME", SHOW, (tXymMsgValue) (char*)"19:30"  );

#if(1)  
  grp = addMessageGroup( "krpan", "mqQ", "QREMOTE" );
  addMessageLine( grp ) ;
  setMessageItem( NULL, "PUT", OK, (tXymMsgValue) (char*) "ENABLE" );
  setMessageItem( NULL, "GET", OK, (tXymMsgValue) (char*) "DISABLE" );
#endif

  printMessageStructData();

  xymSendAllReceiver( ) ;

#if(0)
  result = sendmessage( message, &response );
#endif
  return sysRc ;
}


/******************************************************************************/
/*                                                                            */
/*   F U N C T I O N S                                                        */
/*                                                                            */
/******************************************************************************/
void initMessageCfg()
{
#if(1)
  addMessageBoxCfg(   "mqDlq" ) ;
  addMessageItemCfg(  "mqDlq", "", "REASON" );
  addMessageItemCfg(  "mqDlq", "", "DATE"   );
  addMessageItemCfg(  "mqDlq", "", "TIME"   );
#endif
  addMessageBoxCfg(   "mqQ" ) ;
  addMessageGroupCfg( "mqQ", "QLOCAL" );
  setMessageItemCfg( addMessageItemCfg( "mqQ", "QLOCAL" , "QUEUE" ), 
                     20, LEFT, STRING );
  addMessageItemCfg(  "mqQ", "QLOCAL"  , "CURDEPTH" );
  addMessageItemCfg(  "mqQ", "QREMOTE" , "PUT" );
  addMessageItemCfg(  "mqQ", "QREMOTE" , "GET" );


  setMessageItemCfg( addMessageItemCfg(  "mqDlq", "", "REASON" ),
                     4, RIGHT, INT );
  setMessageItemCfg( addMessageItemCfg(  "mqDlq", "", "DATE"   ),
                     10, LEFT, STRING );
  setMessageItemCfg( addMessageItemCfg(  "mqDlq", "", "TIME"   ),
                     5, LEFT, STRING );

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
