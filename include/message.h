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

/******************************************************************************/
/*   T Y P E S                                                                */
/******************************************************************************/
typedef struct sXymMsgBox      tXymMsgBox     ;
typedef struct sXymMsgGrp      tXymMsgGrp     ;
typedef struct sXymMsgItemDscr tXymMsgItemDscr;
typedef struct sXymMsgItemData tXymMsgItemData;

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
  LEFT ,
  RIGHT
} ;

enum eXymType
{
  INT   ,
  STRING
};

enum eXymLev
{
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
struct sXymMsgBox
{
  char boxName[XYM_BOX_NAME_LNG] ;
  tXymMsgGrp *group;
  tXymMsgBox *next;
};

struct sXymMsgGrp
{
  char grpName[XYM_GRP_NAME_LNG];
  tXymMsgItemDscr  *dscr;
  tXymMsgItemData  *data;
  tXymMsgGrp       *next;
};

struct sXymMsgItemDscr 
{
  char      itemName[XYM_ITEM_LNG] ;
  unsigned  short length ;
  tXymAlign align ;
  tXymType  type ;
  int       id ;
  tXymMsgItemDscr *next ;
};

struct sXymMsgItemData 
{
  tXymLev lev;
  int     id ;
  tXymMsgValue    value ;
  tXymMsgItemData *next ;
};


/******************************************************************************/
/*   G L O B A L E S                                                          */
/******************************************************************************/
#ifdef _C_XYMON_MESSAGE_MODULE_
  tXymMsgBox *gXymMsgBox = NULL ;
#else
  extern tXymMsgBox *gXymMsgBox ;
#endif

/******************************************************************************/
/*   M A C R O S                                                              */
/******************************************************************************/

/******************************************************************************/
/*   P R O T O T Y P E S                                                      */
/******************************************************************************/
void printMessageStruct() ;

tXymMsgBox *addMessageBox( const char *box ) ;
tXymMsgBox *lastMessageBox( ) ;
tXymMsgBox *findMessageBox( const char* box );

void addMessageGroup( const char* box, const char *grp ) ;
tXymMsgGrp* lastMessageGroup( tXymMsgGrp* first ) ;
tXymMsgGrp* findMessageGroup( tXymMsgGrp* first, const char* grpName );
