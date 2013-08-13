/******************************************************************************/
/*  xymon string functions                                                    */
/*                                                                            */
/*  functions:                                                                */
/*    - newstrbuffer                                                          */
/*    - addtobuffer                                                           */
/*    - dupstrbuffer                                                          */
/*    - clearstrbuffer                                                        */
/*    - freestrbuffer                                                         */
/*    - strbuf_addtobuffer                                                    */
/*    - strbufferchop                                                         */
/*    - addtostrbuffer                                                        */
/******************************************************************************/

/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/

// ---------------------------------------------------------
// system
// ---------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include <memory.h>

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------
#include <libxymon.h>
#include <strfunc.h>

#include <ctl.h>

/******************************************************************************/
/*   G L O B A L S                                                            */
/******************************************************************************/

/******************************************************************************/
/*   D E F I N E S                                                            */
/******************************************************************************/
#define BUFSZINCREMENT 4096

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
/*  new string buffer                                                         */
/******************************************************************************/
strbuffer_t *newstrbuffer(int initialsize)
{
  strbuffer_t *newbuf;

  newbuf = calloc(1, sizeof(strbuffer_t));

  if (!initialsize) initialsize = 4096;

  newbuf->s = (char *)malloc(initialsize);
  *(newbuf->s) = '\0';
  newbuf->sz = initialsize;

  return newbuf;
}

/******************************************************************************/
/*  add to buffer                                                             */
/******************************************************************************/
void addtobuffer(strbuffer_t *buf, char *newtext)
{
  if (newtext) strbuf_addtobuffer(buf, newtext, strlen(newtext));
}

/******************************************************************************/
/*  duplicate string buffer                                                   */
/******************************************************************************/
strbuffer_t *dupstrbuffer(char *src)
{
  strbuffer_t *newbuf;
  int len = 0;

  newbuf = newstrbuffer(0);
  if (src) 
  {
    newbuf->s = strdup(src);
    len = strlen(src);
    newbuf->used = newbuf->sz = len;
  }

  return newbuf;
}

/******************************************************************************/
/*  clear string buffer                                                       */
/******************************************************************************/
void clearstrbuffer(strbuffer_t *buf)
{
  if (buf == NULL) return;

  if (buf->s) 
  {
    *(buf->s) = '\0';
    buf->used = 0;
  }
}

/******************************************************************************/
/*  free string buffer                                                        */
/******************************************************************************/
void freestrbuffer(strbuffer_t *buf)
{
  if (buf == NULL) return;

  if (buf->s) xfree(buf->s);
  xfree(buf);

  _door :

  return ;
}

/******************************************************************************/
/*  string buffer add to buffer                                               */
/******************************************************************************/
void strbuf_addtobuffer(strbuffer_t *buf, char *newtext, int newlen)
{
  if (buf->s == NULL) 
  {
    buf->used = 0;
    buf->sz = newlen + BUFSZINCREMENT;
    buf->s = (char *) malloc(buf->sz);
    *(buf->s) = '\0';
  }
  else if ((buf->used + newlen + 1) > buf->sz) 
  {
    buf->sz += (newlen + BUFSZINCREMENT);
    buf->s = (char *) realloc(buf->s, buf->sz);
  }

  if (newtext) 
  {
    memcpy(buf->s+buf->used, newtext, newlen);
    buf->used += newlen;
    // Make sure result buffer is NUL-terminated; newtext might not be. 
    *(buf->s + buf->used) = '\0';
  }
}

/******************************************************************************/
/*  string buffer chop                                                        */
/******************************************************************************/
void strbufferchop(strbuffer_t *buf, int count)
{
  // Remove COUNT characters from end of buffer 
  if ((buf == NULL) || (buf->s == NULL)) return;

  if (count >= buf->used) count = buf->used;

  buf->used -= count;
  *(buf->s+buf->used) = '\0';
}

/******************************************************************************/
/*  add to string buffer                                                      */
/******************************************************************************/
void addtostrbuffer(strbuffer_t *buf, strbuffer_t *newtext)
{
  strbuf_addtobuffer(buf, STRBUF(newtext), STRBUFLEN(newtext));
}

