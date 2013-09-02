/******************************************************************************/
/*                 X Y M O N   M E S S A G E   H A N D L E R                  */
/*                                                                            */
/*  functions:                                                                */
/*    - printMessageStruct                                                    */
/*    - printBox                                                              */
/*    - printGroup                                                            */
/*    - printItemCfg                                                          */
/*    - xymDataType2Str                                                       */
/*    - xymAlign2str                                                          */
/*    - addMessageBoxCfg                                                      */
/*    - lastMessageBoxCfg                                                     */
/*    - findMessageBoxCfg                                                     */
/*    - addMessageGroupCfg                                                    */
/*    - lastMessageGroupCfg                                                   */
/*    - findMessageGroupCfg                                                   */
/*    - addMessageItemCfg                                                     */
/*    - lastMessageItemCfg                                                    */
/*    - findMessageItemCfg                                                    */
/*    - setMessageItemCfg                                                     */
/*    - addReceiver                                                           */
/*    - findReceiver                                                        */
/*    - lastReceiver                                      */
/*    - addMessageGroup                                */
/*    - findMessageGroup                  */
/*    - lastMessageGroup              */
/*    - lastLine                                                              */
/*    - addMessageLine                                                        */
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
/* add message receiver                                                       */
/******************************************************************************/
tXymMsgReceiver* addReceiver( const char* receiver, const char* test )
{
  tXymMsgReceiver *new  = NULL ;
  tXymMsgReceiver *last = NULL ;
  tXymMsgBoxCfg   *box  = NULL ;

  box = findMessageBoxCfg( test ) ;
  if( box == NULL ) goto _door ;

  new = findReceiver( receiver, test );
  if( new ) goto _door ;

  new = (tXymMsgReceiver*) malloc( sizeof(tXymMsgReceiver) );
  snprintf( new->client, XYM_CLIENT_NAME_LNG-1, "%s", receiver ) ;
  new->box = box ;
  new->data = NULL ;
  new->next = NULL ;

  if( gXymSnd == NULL ) 
  {
    gXymSnd = new ;
    goto _door ;
  }
  
  last = lastReceiver( ) ;
  last->next = new ;

  _door :
  return new ;
}

/******************************************************************************/
/* find receiver                                                              */
/******************************************************************************/
tXymMsgReceiver* findReceiver( const char* receiver, const char* test )
{
  tXymMsgReceiver *p   = NULL ;
  tXymMsgBoxCfg   *box = findMessageBoxCfg( test ) ;
  
  if( !box ) goto _door ;

  p = gXymSnd;
  while( p )
  {
    if( p->box == box            &&
        strcmp(p->client,test) == 0 )
    {
      break ;
    }
    p=p->next ;
  }

  _door:
  return p ;
}

/******************************************************************************/
/* find last receiver                                                         */
/******************************************************************************/
tXymMsgReceiver* lastReceiver( )
{
  tXymMsgReceiver *p = gXymSnd;
  if( p == NULL ) goto _door ;

  while( p->next )
  {
    p = p->next; 
  }

  _door:
  return p;
}

/******************************************************************************/
/* add message group                                                        */
/******************************************************************************/
tXymMsgGrpData *addMessageGroup( const char* receiver, 
            const char* test, 
            const char* group)
{
  tXymMsgReceiver *client = addReceiver( receiver, test );
  tXymMsgGrpData  *grp = NULL;
  tXymMsgGrpData  *last = NULL;
  tXymMsgGrpCfg   *cfg = NULL;

  if( client == NULL )    // client NULL since test(grpCfg)  not configured
  {                       //
    goto _door ;          //
  }                       //
                          //
  cfg = findMessageGroupCfg( client->box->group,test );
  if( !cfg )              //
  {                       // test (grpCfg) not configured
    goto _door;           //
  }                       //
                          //
                      //
  grp = findMessageGroup( client->data, cfg );
  if( grp )               // check if group allready exists in receiver/test
  {                       //
    goto _door;           //
  }                  //
                          //
  grp = (tXymMsgGrpData*) malloc( sizeof(tXymMsgGrpData) ); 
  grp->cfg = cfg  ;
  grp->line = NULL;
  grp->next = NULL;

  if( client->data == NULL )
  {
    client->data = grp ;
    goto _door ;
  }

  last = lastMessageGroup( client->data );
  last->next = grp;

  _door:
  return grp;
}

/******************************************************************************/
/* find message group                                                         */
/******************************************************************************/
tXymMsgGrpData* findMessageGroup( tXymMsgGrpData *data, tXymMsgGrpCfg *cfg )
{
  tXymMsgGrpData *p = data ;

  while( p )
  {
    if( p->cfg == cfg ) break;
    p=p->next;
  } 

  return p;
}

/******************************************************************************/
/* find last message group                                            */
/******************************************************************************/
tXymMsgGrpData* lastMessageGroup( tXymMsgGrpData *data )
{
  tXymMsgGrpData *p = data ;

  if( !p ) goto _door ;

  while( p->next != NULL )
  {
    p=p->next;
  }

  _door:
  return p;
}

/******************************************************************************/
/* add message line                                                           */
/******************************************************************************/
tXymMsgLine addMessageLine( tXymMsgGrpData *data )
{
  tXymMsgLine *line = NULL;
  tXymMsgLine *last = lastMessageLine( data->line );

  line = (tXymMsgLine*) malloc( sizeof(tXymMsgLine) );

  if( last == NULL )
  {
    data->line = line;
    goto _door;
  }

  last->next = line ;

  _door:
  return line ;
}

