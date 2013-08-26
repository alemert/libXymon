/******************************************************************************/
/*                 X Y M O N   M E S S A G E   H A N D L E R                  */
/*                                                                            */
/*  functions:                                                  */
/*    - printMessageStruct                          */
/*    - printBox                              */
/*    - printGroup                          */
/*    - addMessageBox                                        */
/*    - lastMessageBox                                      */
/*    - findMessageBox                                        */
/*    - addMessageGroup                                    */
/*    - lastMessageGroup                                  */
/*    - findMessageGroup                    */
/*                                                                  */
/******************************************************************************/

#define _C_XYMON_MESSAGE_MODULE_

/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/

// ---------------------------------------------------------
// system
// ---------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------
#include <message.h>

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
void printBox( tXymMsgBox *_box ) ;
void printGroup( tXymMsgGrp* _grp );

/******************************************************************************/
/*                                                                            */
/*   F U N C T I O N S                                                        */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/* print message struct                  */
/******************************************************************************/
void printMessageStruct()
{
  printf("------------------------------------------------------------\n");
  printBox( gXymMsgBox ) ;
}

/******************************************************************************/
/* print box                      */
/******************************************************************************/
void printBox( tXymMsgBox *_box )
{
  tXymMsgBox *box = _box ;

  while( box )
  {
    printf( "box\t%s\n", box->boxName );
    printGroup( box->group ) ;
    printf("------------------------------------------------------------\n");
    box=box->next ;
  }
}

/******************************************************************************/
/* print group              */
/******************************************************************************/
void printGroup( tXymMsgGrp* _grp )
{
  tXymMsgGrp *grp = _grp ;

  while( grp )
  {
    
  } 

}

/******************************************************************************/
/* add message box                                                            */
/******************************************************************************/
tXymMsgBox *addMessageBox( const char *box )
{
  tXymMsgBox *last = lastMessageBox() ;
  tXymMsgBox *new  = (tXymMsgBox*) malloc( sizeof(tXymMsgBox ) );
  snprintf( new->boxName, XYM_BOX_NAME_LNG, box ) ;
  new->group = NULL ;
  new->next  = NULL ;

  if( last == NULL )
  {
    gXymMsgBox = new ;
  }
  else
  {
    last->next = new ;
  }

  return new ;
}

/******************************************************************************/
/* find last message box                                                      */
/******************************************************************************/
tXymMsgBox *lastMessageBox( )
{
  tXymMsgBox *last = gXymMsgBox ;
 
  if( last == NULL )
  {
    goto _door ;
  }

  while( last->next != NULL )
  {
    last = last->next ;
  }

  _door :
  return last ;
}

/******************************************************************************/
/* find message box                                                           */
/******************************************************************************/
tXymMsgBox *findMessageBox( const char* box )
{
  tXymMsgBox *p = gXymMsgBox ;

  while( p == NULL )
  {
    if( strcmp(p->boxName,box) == 0 ) goto _door ;
    p = p->next ;
  }

  _door :
  return p ;
}

/******************************************************************************/
/* add a message group                                                        */
/******************************************************************************/
void addMessageGroup( const char* boxName, const char *grpName )
{
  tXymMsgBox *box ;
  tXymMsgGrp *last ;
  tXymMsgGrp *new ;

  box = findMessageBox( boxName );                 // search for a box to which 
                                                   //  a group should be added
  if( box == NULL )                                //
  {                                                //
    box = addMessageBox( boxName );                // if box does not exists,
  }                                                //  create one
                                                   //
  if( findMessageGroup(box->group,grpName) )       // if this message group 
  {                                                //  exists in this box, 
    goto _door;                                    //  do not do anything
  }                                                //
                                                   //
  new = (tXymMsgGrp*) malloc( sizeof(tXymMsgGrp) );// allocate memory for a 
                                                   //  new group
  snprintf( new->grpName    ,                      // init new group with
            XYM_GRP_NAME_LNG,                      //  - group name
            grpName        );                      //
  new->dscr = NULL ;                               //  -all pointers
  new->data = NULL ;                               //
  new->next = NULL ;                               //
                                                   //
  last = lastMessageGroup( box->group );           // a new group has to be 
  if( last == NULL )                               //  added at the end of 
  {                                                //  a linked list
    box->group = new;                              //
    goto _door;                                    //
  }                                                //
                                                   //
  last->next =  new;                               //
                                                   //
  _door :                                          //
  return;                                          //
}

/******************************************************************************/
/* find last message group                                                    */
/******************************************************************************/
tXymMsgGrp* lastMessageGroup( tXymMsgGrp* first )
{
  tXymMsgGrp *p = first ;

  if( p == NULL ) goto _door ;

  while( p->next != NULL )
  {
    p = p->next ; 
  }

  _door :
  return p ;
}

/******************************************************************************/
/* find message group                        */
/******************************************************************************/
tXymMsgGrp*  findMessageGroup( tXymMsgGrp* first, const char* grpName ) 
{
  tXymMsgGrp* p = first;

  if( p == NULL ) goto _door ;

  while( p )
  {
    if( strcmp( p->grpName, grpName ) == 0 ) break ;
  }

  _door :
  return p ;
}
