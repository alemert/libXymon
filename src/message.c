/******************************************************************************/
/*                 X Y M O N   M E S S A G E   H A N D L E R                  */
/*                                                                            */
/*  functions:                                                                */
/*    - printMessageStructCfg                                                 */
/*    - printMessageStructData                                            */
/*    - printBox                                                              */
/*    - printGroup                                                            */
/*    - printItemCfg                                                          */
/*    - printReceiver                                                      */
/*    - printGroupData                                              */
/*    - printMessageLine                                  */
/*    - printTopLine                                          */
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
/*    - findReceiver                                                          */
/*    - lastReceiver                                                          */
/*    - addMessageGroup                                                       */
/*    - findMessageGroup                                                      */
/*    - lastMessageGroup                                                      */
/*    - addMessageLine                                                        */
/*    - lastMessageLine                                                       */
/*    - addMessageItem                                                        */
/*    - lastMessageItem                                                       */
/*    - setMessageItem                                    */
/*    - findMessageItem                                */
/*    - openLevelChar                            */
/*    - closeLevelChar                        */
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

void printReceiver(tXymMsgReceiver* first);
void printGroupData( tXymMsgGrpData *first );
void printMessageLine( const char* offset, tXymMsgGrpData *grp);
void printTopLine(const char *offset, tXymMsgGrpData *grp );
//void printTopLine(const char *offset, tXymMsgItemCfg *first );

const char* xymDataType2Str( tXymType _type );
const char* xymAlign2str( tXymAlign _align);

const char openLevelChar( tXymMsgItem  *item );
const char closeLevelChar( tXymMsgItem *item );
/******************************************************************************/
/*                                                                            */
/*   F U N C T I O N S                                                        */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/* print message struct cfg                                                   */
/******************************************************************************/
void printMessageStructCfg()
{
  printf("------------------------ C O N F I G -----------------------\n");
  printBox( gXymMsgCfg ) ;
  printf("------------------------------------------------------------\n");
}

/******************************************************************************/
/* print message struct data                              */
/******************************************************************************/
void printMessageStructData()
{
  printf("\n");
  printf("------------------------   D A T A   -----------------------\n");
  printReceiver( gXymSnd );
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
/* print receiver                                                             */
/******************************************************************************/
void printReceiver(tXymMsgReceiver* first)
{
  tXymMsgReceiver *p = first;

  while( p )
  {
    printf(" %s - %s\n", p->client, p->box->boxName );
    printGroupData( p->data );
    printf(" |\n");
    p=p->next;
  }
}

/******************************************************************************/
/* print group data                                                           */
/******************************************************************************/
void printGroupData( tXymMsgGrpData *first )
{
  tXymMsgGrpData *p=first;
  
  while( p )
  {
    printf(       " |-- %s\n", p->cfg->grpName );
    printTopLine( " |   "    , p );
    printMessageLine( " |   ",p );
    p = p->next;
  }
}

/******************************************************************************/
/* print message line                                      */
/******************************************************************************/
void printMessageLine( const char* offset, tXymMsgGrpData *grp)
{
  tXymMsgItemCfg *cfg  ;
  tXymMsgLine    *line = grp->line;
  tXymMsgItem    *item ;
  char format[16];    // %-xx.xxs
  char lineBuff[64];

  while( line )                                     //
  {                                                 //
    printf( "%s", offset );                         //
    cfg  = grp->cfg->head;                          //
                                                    //
    // -----------------------------------------------------
    // setup format for output
    // -----------------------------------------------------
    while( cfg )                                    //
    {                                               //
      item = findMessageItem( cfg->itemName,        // find a data item to 
                              line->item  );        //  the config item
      if( item == NULL )                            //
      {                                             //
         snprintf( format, 15, "%%%2.2d.%2.2ds",   // setup a format for 
                                cfg->length-1  ,      //  printing out empty
                                cfg->length-1 );      //  line
      }                                             //
      else                                          //
      {                                             //
        switch( cfg->type )                         //
        {                                           //
          case INT :                                // intiger format
          {                                         //  %-d
            snprintf( format, 15, "%%%-dd",        //
                                   cfg->length-2 ); //
            break;                                  //
          }                                         //
          case STRING :                             //
          {                                         //
            snprintf( format,15,"%%-%2.2d.%2.2ds ", // string format
                                 cfg->length-1 ,      //   %05.05s 
                                 cfg->length-1 );     //
            break;                                  //
          }                                         //
          case EMPTY :                              // empty format 
          {                                         // %s (fill with blanks)
            snprintf( format, 15, "%%s" );          //
          }                                         //
        }                                           //
      }                                             //
                                                    //
      // ---------------------------------------------------
      // print out the item
      // ---------------------------------------------------
      if( item == NULL )                            // if item not found
      {                                             //  print "---"
         snprintf( lineBuff, 64, format, "---- " ); //
      }                                             //
      else                                          // item found
      {                                             //  print out the union
        switch( cfg->type )                         //
        {                                           // 
          case INT :                                // print the union as 
          {                                         //  an initiger
            snprintf( lineBuff, 64, format, item->value.digit); //
            break;                                  //
          }                                         //
          case STRING :                             // print the union as
          {                                         //  a string
            snprintf( lineBuff, 64, format, item->value.txt );  //
            break;                                  //
          }                                         //
          case EMPTY :                              // print the empty value
          {                                         // (not a part of the union)
            snprintf( lineBuff, 64, format, " " );  //   
            break;                                  //
          }                                         //
        }                                           //
      }                                             //
      printf( "%c%s%c", openLevelChar(item),     //
                        lineBuff              ,     //
                        closeLevelChar(item) );  //
      cfg = cfg->next;                              //
    }                                               //
    printf( "\n" );                                 //
    line = line->next;                              //
  }                                                 //

}

/******************************************************************************/
/* print top (header) line                                                    */
/******************************************************************************/
void printTopLine(const char *offset, tXymMsgGrpData *grp )
{
  tXymMsgItemCfg *p = grp->cfg->head ;
  char format[16];    // %-xx.xxs

  printf("%s", offset );
  while( p )
  {
    snprintf( format, 15, "%%-%2.2d.%2.2ds", p->length, p->length );
    printf( format, p->itemName );
//  printf( "%s ", format );
    p=p->next ;
  }
  printf("\n");

}

/******************************************************************************/
/* convert type to string                                                     */
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
/* convert align type to string                                               */
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
     _head->length   = _length+2 ;
     _head->align    = _align  ;
     _head->type     = _type   ;
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
        strcmp(p->box->boxName,test) == 0 )
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
/* add message group                                                          */
/******************************************************************************/
tXymMsgGrpData *addMessageGroup( const char* receiver, 
                                 const char* test    , 
                                 const char* group   )
{
  tXymMsgReceiver *client = addReceiver( receiver, test );
  tXymMsgGrpData  *grp = NULL;
  tXymMsgGrpData  *last = NULL;
  tXymMsgBoxCfg   *cfg = NULL;

  if( client == NULL )    // client NULL since test(grpCfg)  not configured
  {                       //
    goto _door ;          //
  }                       //
                          //
  cfg = findMessageBoxCfg( test );
  if( !cfg )              //
  {                       // test (grpCfg) not configured
    goto _door;           //
  }                       //
                          //
                          //
  grp = findMessageGroup( client->data, cfg->group );
  if( grp )               // check if group allready exists in receiver/test
  {                       //
    goto _door;           //
  }                       //
                          //
  grp = (tXymMsgGrpData*) malloc( sizeof(tXymMsgGrpData) ); 
  grp->cfg = cfg->group  ;
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
/* find last message group                                                    */
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
tXymMsgLine* addMessageLine( tXymMsgGrpData *data )
{
  tXymMsgLine *line = NULL;
  tXymMsgLine *last = lastMessageLine( data->line );

  line = (tXymMsgLine*) malloc( sizeof(tXymMsgLine) );
  line->lev  = UNKNOWN;
  line->item = NULL;
  line->next = NULL;

  if( last == NULL )
  {
    data->line = line;
    goto _door;
  }

  last->next = line ;

  _door:
  return line ;
}

/******************************************************************************/
/* find last message line                                                     */
/******************************************************************************/
tXymMsgLine* lastMessageLine( tXymMsgLine *first )
{
  tXymMsgLine *p = first ;

  if( !p ) goto _door;

  while( p->next )
  {
    p=p->next;
  }
 
  _door:
  return p;
}

/******************************************************************************/
/* add message item                                                           */
/******************************************************************************/
tXymMsgItem* addMessageItem( tXymMsgLine *line )
{
  tXymMsgItem *item = NULL;
  tXymMsgItem *last = lastMessageItem( line->item );

  item = (tXymMsgItem*) malloc( sizeof(tXymMsgItem) );
  item->lev = UNKNOWN;
  item->itemName[0] = '\0' ;
//item->value.txt[0] =  '\0' ;
  item->value.txt = NULL ;
  item->cfg = NULL;
  item->next  = NULL;

  if( !last )
  {
    line->item=item;
    goto _door;
  }

  last->next=item;

  _door:
  return item;
}

/******************************************************************************/
/* find last message item                                                     */
/******************************************************************************/
tXymMsgItem* lastMessageItem( tXymMsgItem *first )
{
  tXymMsgItem *p=first;

  if( !p ) goto _door;

  while( p->next )
  {
    p=p->next;
  }

  _door:
  return p;
}

/******************************************************************************/
/* set message item                                                           */
/******************************************************************************/
void setMessageItem( tXymMsgLine* line   , 
                     const char *itemName, 
                     tXymLev level       ,
                     tXymMsgValue value  )
{
//tXymMsgItem *last ;
  tXymMsgItem *item = addMessageItem( line );

  snprintf( item->itemName, XYM_ITEM_LNG, "%s", itemName );
#if(0)
  switch( type )
  {
    case INT    : item->value.digit = value.digit ;
                  break ;
    case STRING : snprintf( item->value.txt, XYM_ITEM_LNG, "%s", value.txt );
                  break ;
    default : break ;
  }
#else
  item->value = value ;
#endif
  item->lev = level ;

  return ;
}

/******************************************************************************/
/* find message item                                                          */
/******************************************************************************/
tXymMsgItem* findMessageItem( const char* itemName,  tXymMsgItem* first )
{
  tXymMsgItem *p = first ;

  while( p )
  {
    if( strcmp(p->itemName, itemName) == 0 ) goto _door;
    p = p->next ;
  }

  _door:
  return p;
}

/******************************************************************************/
/* open level char                                                  */
/******************************************************************************/
const char openLevelChar( tXymMsgItem *item )
{
  if( item == NULL ) return ' ';

  switch( item->lev )
  {
    case NA      : return ' ';
    case DISABLE : 
    case IGNORE  : return '<';
    case OK      : return ' ';
    case WAR     : return '(';
    case ERR     : return '[';
  }
  return ' ';
}

/******************************************************************************/
/* close level char                                        */
/******************************************************************************/
const char closeLevelChar( tXymMsgItem *item )
{
  if( item == NULL ) return ' ';

  switch( item->lev )
  {
    case NA      : return ' ';
    case DISABLE : 
    case IGNORE  : return '>';
    case OK      : return ' ';
    case WAR     : return ')';
    case ERR     : return ']';
  }
  return ' ';
}

