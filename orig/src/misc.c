/******************************************************************************/
/*  xymon misc                                                                */
/*                                                                            */
/*  functions:                                                          */
/*    - argnmatch                                                             */
/*    - sanitize_input                                                      */
/******************************************************************************/

/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/

// ---------------------------------------------------------
// system
// ---------------------------------------------------------
#include <string.h>
#include <ctype.h>

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------
#include <libxymon.h>
#include <strfunc.h>

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

/******************************************************************************/
/*                                                                            */
/*   F U N C T I O N S                                                        */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*  argument match                                                          */
/******************************************************************************/
int argnmatch(char *arg, char *match)
{
  return (strncmp(arg, match, strlen(match)) == 0);
}

/******************************************************************************/
/*  sanitaze input                                                            */
/******************************************************************************/
void sanitize_input(strbuffer_t *l, int stripcomment, int unescape)
{
  int i;

  //
  // This routine sanitizes an input line, stripping off 
  // leading/trailing whitespace.
  // If requested, it also strips comments.
  // If requested, it also un-escapes \-escaped charactes.
  //

  // Kill comments 
  if (stripcomment || unescape) 
  {
    char *p, *commentstart = NULL;
    char *noquotemarkers = (unescape ? "\"'#\\" : "\"'#");
    char *inquotemarkers = (unescape ? "\"'\\" : "\"'");
    int inquote = 0;

    p = STRBUF(l) + strcspn(STRBUF(l), noquotemarkers);
    while (*p && (commentstart == NULL)) 
    {
      switch (*p) 
      {
        case '\\':
        {
          if (inquote)
          {
            p += 2+strcspn(p+2, inquotemarkers);
          }
          else
          {
            p += 2+strcspn(p+2, noquotemarkers);
          }
          break;
        }

        case '"':
        case '\'':
        {
          inquote = (1 - inquote);
          if (inquote)
          {
            p += 1+strcspn(p+1, inquotemarkers);
          }
          else
          {
            p += 1+strcspn(p+1, noquotemarkers);
          }
          break;
        }

        case '#':
        {
          if (!inquote) commentstart = p;
          break;
        }
      }
    }

    if( commentstart )
    {
      strbufferchop(l, STRBUFLEN(l) - (commentstart - STRBUF(l)));
    }
  }

  // Kill a trailing CR/NL 
  i = strcspn(STRBUF(l), "\r\n");
  if (i != STRBUFLEN(l)) 
  {
    strbufferchop(l, STRBUFLEN(l)-i);
  }

  // Kill trailing whitespace 
  i = STRBUFLEN(l);
  while( (i > 0) && isspace((int)(*(STRBUF(l)+i-1))) )
  {
     i--;
  }
  if (i != STRBUFLEN(l)) 
  {
    strbufferchop(l, STRBUFLEN(l)-i);
  }

  // Kill leading whitespace 
  i = strspn(STRBUF(l), " \t");
  if (i > 0) 
  {
    memmove(STRBUF(l), STRBUF(l)+i, STRBUFLEN(l)-i);
    strbufferchop(l, i);
  }

  if (unescape) 
  {
    char *p;

    p = STRBUF(l) + strcspn(STRBUF(l), "\\");
    while (*p) 
    {
      memmove(p, p+1, STRBUFLEN(l)-(p-STRBUF(l)));
      strbufferchop(l, 1);
      p = p + 1 + strcspn(p+1, "\\");
    }
  }
}

