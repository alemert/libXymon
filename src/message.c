/******************************************************************************/
/*                 X Y M O N   M E S S A G E   H A N D L E R                  */
/*                                                                            */
/*  functions:                                                                */
/*    - printMessageStruct                                                    */
/*    - printBox                                                              */
/*    - printGroup                                                            */
/*    - printItemCfg                                                          */
/*    - xymDataType2Str                                              */
/*    - xymAlign2str                                                */
/*    - addMessageBoxCfg                                                      */
/*    - lastMessageBoxCfg                                                     */
/*    - findMessageBoxCfg                                                     */
/*    - addMessageGroupCfg                                                    */
/*    - lastMessageGroupCfg                                                   */
/*    - findMessageGroupCfg                                                   */
/*    - addMessageItemCfg                                                     */
/*    - lastMessageItemCfg                                                    */
/*    - findMessageItemCfg                                                    */
/*    - setMessageItemCfg                                            */
/*    - lastLine                                                */
/*    - addMessageLine                                      */
/*    - findGroup4send                                        */
/*                                                                            */
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
void printBox( tXymMsgBoxCfg *_box ) ;
void printGroup( tXymMsgGrpCfg* _grp );
void printItemCfg( tXymMsgItemCfg *_head );

const char* xymDataType2Str( tXymType _type );
const char* xymAlign2str( tXymAlign _align);

/******************************************************************************/
/*                                                                            */
/*   F U N C T I O N S                                                        */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/* print message struct                                                       */
/******************************************************************************/
void printMessageStruct()
{
  printf("------------------------------------------------------------\n");
  printBox( gXymMsgCfg ) ;
  printf("------------------------------------------------------------\n");
}

/******************************************************************************/
/* print box                                                                  */
/******************************************************************************/
void printBox( tXymMsgBoxCfg *_box )
{
  tXymMsgBoxCfg *box = _box ;

  while( box )
  {
    printf( " %s\n", box->boxName );
    printGroup( box->group ) ;
    box=box->next ;
  }
}

/******************************************************************************/
/* print group                                                                */
/******************************************************************************/
void printGroup( tXymMsgGrpCfg* _grp )
{
  tXymMsgGrpCfg *grp = _grp ;

  while( grp )
  {
    printf( " |-- %s\n", grp->grpName );
    printItemCfg( grp->head );
    grp = grp->next ;   
  } 
}

/******************************************************************************/
/* print header                                                               */
/******************************************************************************/
void printItemCfg( tXymMsgItemCfg *_head )
{
  tXymMsgItemCfg *item = _head;

  while( item )
  {
    printf(" |    |-- %-12.11s%-7.6s%3d %-6.5s\n", item->itemName             ,
                                                   xymDataType2Str(item->type),
                                                   item->length               ,
                                                   xymAlign2str(item->align) );

    item = item->next;
  }
  printf(" |     \n");
}

/******************************************************************************/
/* convert type to string                                                */
/******************************************************************************/
const char* xymDataType2Str( tXymType _type )
{
  switch( _type )
  {
    case EMPTY  : return "EMPTY"  ; 
    case INT    : return "INT"    ;
    case STRING : return "STRING" ;
  }
  return "ERROR" ;   
}

/******************************************************************************/
/* convert align type to string                        */
/******************************************************************************/
const char* xymAlign2str( tXymAlign _align)
{
  switch( _align )
  {
    case NONE: return "NONE" ;
    case LEFT: return "LEFT" ;
    case RIGHT: return "RIGHT";
  }

  return "ERROR" ;
}

/******************************************************************************/
/* add message box                                                            */
/******************************************************************************/
tXymMsgBoxCfg *addMessageBoxCfg( const char *box )
{
  tXymMsgBoxCfg *last = NULL;
  tXymMsgBoxCfg *new  = NULL;

  if( findMessageBoxCfg( box ) )
  {
    goto _door ;
  }

  last = lastMessageBoxCfg() ;
  new  = (tXymMsgBoxCfg*) malloc( sizeof(tXymMsgBoxCfg) );
  snprintf( new->boxName, XYM_BOX_NAME_LNG, box ) ;
  new->group = NULL ;
  new->next  = NULL ;

  if( last == NULL )
  {
    gXymMsgCfg = new ;
    goto _door ;
  }

  last->next = new ;

  _door :
  return new ;
}

/******************************************************************************/
/* find last message box                                                      */
/******************************************************************************/
tXymMsgBoxCfg *lastMessageBoxCfg( )
{
  tXymMsgBoxCfg *last = gXymMsgCfg ;
 
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
tXymMsgBoxCfg *findMessageBoxCfg( const char* box )
{
  tXymMsgBoxCfg *p = gXymMsgCfg ;

  while( p != NULL )
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
tXymMsgGrpCfg* addMessageGroupCfg( const char* boxName, const char *grpName )
{
  tXymMsgBoxCfg *box ;
  tXymMsgGrpCfg *last ;
  tXymMsgGrpCfg *new ;

  box = findMessageBoxCfg( boxName );              // search for a box to which 
                                                   //  a group should be added
  if( box == NULL )                                //
  {                                                //
    box = addMessageBoxCfg( boxName );             // if box does not exists,
  }                                                //  create one
                                                   //
  if( findMessageGroupCfg(box->group,grpName) )    // if this message group 
  {                                                //  exists in this box, 
    goto _door;                                    //  do not do anything
  }                                                //
                                                   //
  new=(tXymMsgGrpCfg*)malloc(sizeof(tXymMsgGrpCfg));// allocate memory for a 
                                                   //  new group
  snprintf( new->grpName    ,                      // init new group with
            XYM_GRP_NAME_LNG,                      //  - group name
            grpName        );                      //
  new->head = NULL ;                               //  -all pointers
//new->data = NULL ;                               //
  new->next = NULL ;                               //
                                                   //
  last = lastMessageGroupCfg( box->group );        // a new group has to be 
  if( last == NULL )                               //  added at the end of 
  {                                                //  a linked list
    box->group = new;                              //
    goto _door;                                    //
  }                                                //
                                                   //
  last->next =  new;                               //
                                                   //
  _door :                                          //
  return new;                                      //
}

/******************************************************************************/
/* find last message group                                                    */
/******************************************************************************/
tXymMsgGrpCfg* lastMessageGroupCfg( tXymMsgGrpCfg* first )
{
  tXymMsgGrpCfg *p = first ;

  if( p == NULL ) goto _door ;

  while( p->next != NULL )
  {
    p = p->next ; 
  }

  _door :
  return p ;
}

/******************************************************************************/
/* find message group                                                         */
/******************************************************************************/
tXymMsgGrpCfg* findMessageGroupCfg( tXymMsgGrpCfg* first, 
                                    const char* grpName ) 
{
  tXymMsgGrpCfg* p = first;

  if( p == NULL ) goto _door ;

  while( p )
  {
    if( strcmp( p->grpName, grpName ) == 0 ) break ;
    p = p->next;
  }

  _door :
  return p ;
}

/******************************************************************************/
/* add message header                                                         */
/******************************************************************************/
tXymMsgItemCfg* addMessageItemCfg( const char *_boxName  ,
                                   const char *_grpName  ,
                                   const char *_itemName )
{
  tXymMsgBoxCfg  *box ;
  tXymMsgGrpCfg  *grp ;
  tXymMsgItemCfg *new ;
  tXymMsgItemCfg *last;
 
  addMessageGroupCfg( _boxName, _grpName ) ;

  box = findMessageBoxCfg( _boxName ) ;
  grp = findMessageGroupCfg( box->group, _grpName );

  new = findMessageItemCfg( grp->head, _itemName );
  if( new )
  {
    goto _door ;
  }

  new = (tXymMsgItemCfg*) malloc( sizeof(tXymMsgItemCfg) );
  snprintf( new->itemName, XYM_ITEM_LNG, _itemName );
  new->length = 0;
  new->align = NONE;
  new->type  = NONE;
//new->id    = 0   ;
  new->next  = NULL;
    
  last = lastMessageItemCfg( grp->head ) ;
  if( last == NULL )
  {
    grp->head = new;
    goto _door ;
  }

  last->next = new;

  _door:
  return new ;
}

/******************************************************************************/
/* last message header item                                                   */
/******************************************************************************/
tXymMsgItemCfg* lastMessageItemCfg( tXymMsgItemCfg *_first )
{
  tXymMsgItemCfg *head = _first;

  if( head == NULL) goto _door ;  

  while( head->next )
  {
    head = head->next;
  }

  _door :
  return head;
}

/******************************************************************************/
/* find message header item                                                   */
/******************************************************************************/
tXymMsgItemCfg* findMessageItemCfg( tXymMsgItemCfg *_first, 
                                    const char *_itemName )
{
  tXymMsgItemCfg *item = _first;

  if( item == NULL) goto _door ;  

  while( item )
  {
    if( strcmp( item->itemName, _itemName) == 0 ) 
    {
      goto _door ;
    }
    item = item->next;
  }

  _door :
  return item;
}

/******************************************************************************/
/* set message header                                                         */
/******************************************************************************/
void setMessageItemCfg( tXymMsgItemCfg *_head  , 
                        int          _length, 
                        tXymAlign    _align , 
                        tXymType     _type  )
{
  // _head->itemName ; allready set by add
     _head->length   = _length ;
     _head->align    = _align  ;
     _head->type     = _type   ;
  // _head->id       ; for future purposes
  // _head->next     ; allready set by add
}

/******************************************************************************/
/* find last message line                       */
/******************************************************************************/
tXymMsgLine* lastLine( tXymMsgLine *_first )
{
  tXymMsgLine *line = _first ;
 
  if( line == NULL ) goto _door ;

  while( line->next )
  {
    line = line->next ;
  }

  _door:
  return line ;
}

/******************************************************************************/
/* add message line                                           */
/******************************************************************************/
#if(0)
int addMessageLine( const char *_groupName )
{
  tXymMsgGrpData *grp = findGroup4send( _groupName ) ;

  tXymMsgLine *line = lastLine( grp->data );

  return line ;
}
#endif

/******************************************************************************/
/* find group for sending                                                  */
/******************************************************************************/
tXymMsgGrpCfg* findMsgGroupData( const char* _groupName )
{
  tXymMsgBoxCfg *box = gXymMsgCfg ;
  tXymMsgGrpCfg *grp ;

  while( box )
  {
    grp = box->group ; 
    while( grp )
    {
      if( strcmp( grp->grpName, _groupName ) == 0 )
      {
        goto _door ;
      }
      grp = grp->next ;
    }
    box = box->next ;
  }

  _door :
  return grp ; 
}
