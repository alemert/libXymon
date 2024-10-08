/******************************************************************************/
/* change title on for new project                                            */
/******************************************************************************/

#ifndef __LIBXYMON_H__
#define __LIBXYMON_H__

/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/
// ---------------------------------------------------------
// system
// ---------------------------------------------------------

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------

/******************************************************************************/
/*   D E F I N E S                                                            */
/******************************************************************************/
#define IP_ADDR_STRLEN 16

#define STRBUF(buf)     (buf->s)
#define STRBUFLEN(buf)  (buf->used)

/******************************************************************************/
/*   T Y P E S                                                                */
/******************************************************************************/
typedef struct strbuffer_t 
{
  char *s;
  int used, sz;
} strbuffer_t;

typedef struct htnames_t 
{
  char *name;
  struct htnames_t *next;
} htnames_t;

/******************************************************************************/
/*   S T R U C T S                                                            */
/******************************************************************************/

/******************************************************************************/
/*   G L O B A L E S                                                          */
/******************************************************************************/

/******************************************************************************/
/*   M A C R O S                                                              */
/******************************************************************************/

/******************************************************************************/
/*   P R O T O T Y P E S                                                      */
/******************************************************************************/

#endif
