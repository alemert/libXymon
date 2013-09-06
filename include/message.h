/******************************************************************************/
/*          X Y M O N   M E S S A G E   H A N D L E R   H E A D E R           */
/******************************************************************************/

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

/******************************************************************************/
/*   D E F I N E S                                                            */
/******************************************************************************/
#define XYM_BOX_NAME_LNG  8
#define XYM_GRP_NAME_LNG 32
#define XYM_ITEM_LNG     64

#define XYM_CLIENT_NAME_LNG 32

/******************************************************************************/
/*   T Y P E S                                                                */
/******************************************************************************/
typedef struct sXymMsgBoxCfg   tXymMsgBoxCfg ;
typedef struct sXymMsgGrpCfg   tXymMsgGrpCfg ;
typedef struct sXymMsgItemCfg  tXymMsgItemCfg;

typedef struct sXymMsgReceiver tXymMsgReceiver;
typedef struct sXymMsgGrpData  tXymMsgGrpData ;
typedef struct sXymMsgLine     tXymMsgLine;
typedef struct sXymMsgItem     tXymMsgItem;

typedef union uXymMsgValue tXymMsgValue ;
 
typedef enum eXymAlign tXymAlign ;
typedef enum eXymType  tXymType ;
typedef enum eXymLev   tXymLev  ;

/******************************************************************************/
/*   S T R U C T S                                                            */
/******************************************************************************/

// ---------------------------------------------------------
// enum
// ---------------------------------------------------------
enum eXymAlign
{
  NONE = 0,
  LEFT    ,
  RIGHT
} ;

enum eXymType
{
  EMPTY =0,
  INT     ,
  STRING
};

enum eXymLev
{
  UNKNOWN = -3,
  CLEAR   = -2,
  DISABLE = -2,
  BLUE    = -1,
  IGNORE  = -1,
  GREEN   =  0,
  OK      =  0,
  YELLOW  =  1,
  WAR     =  1,
  RED     =  2 ,
  ERR     =  2
};

// ---------------------------------------------------------
// union
// ---------------------------------------------------------
union uXymMsgValue
{
  char txt[XYM_ITEM_LNG] ;
  int  digit ;
};

// ---------------------------------------------------------
// struct
// ---------------------------------------------------------
struct sXymMsgBoxCfg
{
  char boxName[XYM_BOX_NAME_LNG] ;
  tXymMsgGrpCfg *group;
  tXymMsgBoxCfg *next;
};

struct sXymMsgGrpCfg
{
  char grpName[XYM_GRP_NAME_LNG];
  tXymMsgItemCfg  *head;
  tXymMsgGrpCfg   *next;
};

struct sXymMsgItemCfg
{
  char          itemName[XYM_ITEM_LNG] ;
  unsigned      short length ;
  tXymAlign     align ;
  tXymType      type ;
  tXymMsgItemCfg *next ;
};


struct sXymMsgReceiver
{
  char client[XYM_CLIENT_NAME_LNG] ;
  tXymMsgBoxCfg  *box ;
  tXymMsgGrpData *data;
  tXymMsgReceiver *next ;
};

struct sXymMsgGrpData 
{
  tXymMsgGrpCfg *cfg;
  tXymMsgLine    *line;
  tXymMsgGrpData *next;
};

struct sXymMsgLine
{
  tXymLev  lev ;
  tXymMsgItem    *item;
  tXymMsgLine    *next;
};

struct sXymMsgItem
{
  tXymLev lev;
  char          itemName[XYM_ITEM_LNG] ;
  tXymMsgItemCfg *cfg;
  tXymMsgValue  value ;
  tXymMsgItem   *next ;
};


/******************************************************************************/
/*   G L O B A L E S                                                          */
/******************************************************************************/
#ifdef _C_XYMON_MESSAGE_MODULE_
  tXymMsgBoxCfg   *gXymMsgCfg = NULL ;
  tXymMsgReceiver *gXymSnd    = NULL ;
#else
  extern tXymMsgBoxCfg   *gXymMsgCfg ;
  extern tXymMsgReceiver *gXymSnd    ;
#endif

/******************************************************************************/
/*   M A C R O S                                                              */
/******************************************************************************/

/******************************************************************************/
/*   P R O T O T Y P E S                                                      */
/******************************************************************************/
void printMessageStructCfg() ;
void printMessageStructData();


tXymMsgBoxCfg *addMessageBoxCfg( const char *box ) ;
tXymMsgBoxCfg *lastMessageBoxCfg( ) ;
tXymMsgBoxCfg *findMessageBoxCfg( const char* box );

tXymMsgGrpCfg* addMessageGroupCfg( const char* box, const char *grp ) ;
tXymMsgGrpCfg* lastMessageGroupCfg( tXymMsgGrpCfg* first ) ;
tXymMsgGrpCfg* findMessageGroupCfg( tXymMsgGrpCfg* first, const char* grpName );

tXymMsgItemCfg* addMessageItemCfg( const char *_boxName  ,
                                const char *_grpName  ,
                                const char *_itemName );
tXymMsgItemCfg* lastMessageItemCfg( tXymMsgItemCfg *_first );
tXymMsgItemCfg* findMessageItemCfg( tXymMsgItemCfg *_first, 
                                 const char *_itemName );
void setMessageItemCfg( tXymMsgItemCfg *_head  ,
                        int             _length,
                        tXymAlign       _align ,
                        tXymType        _type );

tXymMsgReceiver* addReceiver( const char* receiver, const char* test );
tXymMsgReceiver* findReceiver( const char* receiver, const char* test );
tXymMsgReceiver* lastReceiver( ) ;

tXymMsgGrpData* addMessageGroup( const char* receiver, 
                     const char* test, 
                     const char* group);
tXymMsgGrpData* findMessageGroup( tXymMsgGrpData *data, tXymMsgGrpCfg *cfg );
tXymMsgGrpData* lastMessageGroup( tXymMsgGrpData *data );

tXymMsgLine* addMessageLine( tXymMsgGrpData *data );
tXymMsgLine* lastMessageLine( tXymMsgLine *data );

tXymMsgItem* addMessageItem( tXymMsgLine *line );
tXymMsgItem* lastMessageItem( tXymMsgItem *first );
void setMessageItem( tXymMsgLine* line, 
                     const char* itemName,
                     tXymType type ,
                     tXymMsgValue value );
tXymMsgItem* findMessageItem( const char* itemName, tXymMsgItem* first );
