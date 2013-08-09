/******************************************************************************/
/*  xymon stack io            */
/*                      */
/*  functions:                */
/*    - unlimfgets              */
/*    - initfgets              */
/******************************************************************************/

/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/

// ---------------------------------------------------------
// system
// ---------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------
#include <stackio.h>
#include <strfunc.h>

#include <ctl.h>
#include <msgcat/lgstd.h>

/******************************************************************************/
/*   G L O B A L S                                                            */
/******************************************************************************/
static fgetsbuf_t *fgetshead = NULL;

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
/*                    */
/*  initfgets() and unlimfgets() implements a fgets() style input routine     */
/*  that can handle arbitrarily long input lines. Buffer space for the input  */
/*  is dynamically allocated and expanded, until the input hits a newline     */
/*  character. Simultaneously, lines ending with a '\' character are merged   */
/*  into one line, allowing for transparent handling of very long lines.      */
/*                */
/*  This interface is also used by the stackfgets() routine.      */
/*                  */
/*  If you open a file directly, after getting the FILE descriptor call       */
/*  initfgets(FILE). Then use unlimfgets() to read data one line at a time.   */
/*  You must read until unlimfgets() returns NULL (at which point you should  */
/*  not call unlimfgets() again with this fd).      */
/*            */
/******************************************************************************/
int initfgets(FILE *fd)
{
  fgetsbuf_t *newitem;

  newitem = (fgetsbuf_t *)malloc(sizeof(fgetsbuf_t));
  *(newitem->inbuf) = '\0';
  newitem->inbufp = newitem->inbuf;
  newitem->moretoread = 1;
  newitem->fd = fd;
  newitem->next = fgetshead;
  fgetshead = newitem;
  return 0;
}

char *unlimfgets(strbuffer_t *buffer, FILE *fd)
{
  fgetsbuf_t *fg;
  size_t n;
  char *eoln = NULL;

  for( fg = fgetshead; (fg && (fg->fd != fd)); fg = fg->next ) ;
  if( !fg ) 
  {
    logger( LSTD_ERR_BAD_FD, __FUNCTION__ ) ;
    return NULL;
  }

  // End of file ? 
  if (!(fg->moretoread) && (*(fg->inbufp) == '\0')) 
  {
    if (fg == fgetshead) 
    {
      fgetshead = fgetshead->next;
      free(fg);
    }
    else 
    {
      fgetsbuf_t *prev;
      for (prev = fgetshead; (prev->next != fg); prev = prev->next) ;
      prev->next = fg->next;
      free(fg);
    }
    return NULL;
  }

  // Make sure the output buffer is empty
  clearstrbuffer(buffer);

  while (!eoln && (fg->moretoread || *(fg->inbufp))) 
  {
    int continued = 0;

    if (*(fg->inbufp)) 
    {
      // Have some data in the buffer 
      eoln = strchr(fg->inbufp, '\n');
      if (eoln) 
      {
        // See if there's a continuation character just before the eoln 
        char *contchar = eoln-1;
        while( (contchar > fg->inbufp) && 
               isspace((int)*contchar) && 
               (*contchar != '\\')) 
        {
          contchar--;
        }
        continued = (*contchar == '\\');

        if (continued) 
        {
          *contchar = '\0';
          addtobuffer(buffer, fg->inbufp);
          fg->inbufp = eoln+1;
          eoln = NULL;
        }
        else 
        {
          char savech = *(eoln+1);
          *(eoln+1) = '\0';
          addtobuffer(buffer, fg->inbufp);
          *(eoln+1) = savech;
          fg->inbufp = eoln+1;
        }
      }
      else 
      {
        // No newline in buffer, so add all of it to the output buffer 
        addtobuffer(buffer, fg->inbufp);

        // Input buffer is now empty 
        *(fg->inbuf) = '\0';
        fg->inbufp = fg->inbuf;
      }
    }

    if (!eoln && !continued) 
    {
      // Get data for the input buffer 
      char *inpos = fg->inbuf;
      size_t insize = sizeof(fg->inbuf);
  
      // If the last byte we read was a continuation char, 
      // we must do special stuff.
      //
      // Mike Romaniw discovered that if we hit an input with a newline exactly 
      // at the point of a buffer refill, then strlen(*buffer) is 0, and 
      // contchar then points before the start of the buffer. Bad. But this 
      // can only happen when the previous char WAS a newline, and hence it is 
      // not a continuation line. So the simple fix is to only do the 
      // cont-char stuff if **buffer is not NUL. Hence the test for both 
      // *buffer and **buffer.
      if (STRBUF(buffer) && *STRBUF(buffer)) 
      {
        char *contchar = STRBUF(buffer) + STRBUFLEN(buffer) - 1;
        while( (contchar > STRBUF(buffer)) && 
               isspace((int)*contchar)     && 
               (*contchar != '\\')) 
        {
          contchar--;
        }

        if (*contchar == '\\') 
        {
          //
          // Remove the cont. char from the output buffer, and stuff it into the
          // input buffer again - so we can check if there's a new-line coming.
          //
          strbufferchop(buffer, 1);
          *(fg->inbuf) = '\\';
          inpos++;
          insize--;
        }
      }

      n = fread(inpos, 1, insize-1, fd);
      *(inpos + n) = '\0';
      fg->inbufp = fg->inbuf;
      if (n < insize-1) fg->moretoread = 0;
    }
  }

  return STRBUF(buffer);
}


