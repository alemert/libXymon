/******************************************************************************/
/* change title on for new project                                            */
/******************************************************************************/

/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/

// ---------------------------------------------------------
// system
// ---------------------------------------------------------
#include <stdio.h>
#include <string.h>

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

void loadenv(char *envfile, char *area)
{
  FILE *fd;
  strbuffer_t *inbuf;
  char *p, *oneenv;

//MEMDEFINE(l);
  inbuf = newstrbuffer(0);

  fd = stackfopen(envfile, "r", NULL);
  if (fd) 
  {
    while (stackfgets(inbuf, NULL)) 
    {
      char *equalpos;
      int appendto = 0;

      sanitize_input(inbuf, 1, 1);

      if ((STRBUFLEN(inbuf) == 0) || ((equalpos = strchr(STRBUF(inbuf), '=')) == NULL)) continue;

      appendto = ((equalpos > STRBUF(inbuf)) && (*(equalpos-1) == '+'));

      //
      // Do the environment "area" stuff: If the input
      // is of the form AREA/NAME=VALUE, then setup the variable
      // only if we're called with the correct AREA setting.
      //
      oneenv = NULL;

      p = STRBUF(inbuf) + strcspn(STRBUF(inbuf), "=/");
      if (*p == '/') 
      {
        if (area) 
        {
          *p = '\0';
          if (strcasecmp(STRBUF(inbuf), area) == 0) 
          {
            oneenv = strdup(expand_env(p+1));
          }
        }
      }
      else 
      {
        oneenv = strdup(expand_env(STRBUF(inbuf)));
      }

      if (oneenv) 
      {
        p = strchr(oneenv, '=');
        if (*(p+1) == '"') 
        {
          // Move string over the first '"' 
          memmove(p+1, p+2, strlen(p+2)+1);
          // Kill a trailing '"' 
          if (*(oneenv + strlen(oneenv) - 1) == '"') 
          {
            *(oneenv + strlen(oneenv) - 1) = '\0';
          }
        }

        if (appendto) 
        {
          char *oldval, *addstring, *p;

          addstring = strchr(oneenv, '='); 
          if( addstring ) 
          {
            *addstring = '\0'; addstring++; 
          }
          p = strchr(oneenv, '+'); 
          if (p) *p = '\0';

          oldval = getenv(oneenv);
          if (oldval) 
          {
            char *combinedenv = (char*) malloc( strlen(oneenv) + 
                                                strlen(oldval) + 
                                                strlen(addstring) + 2);
            sprintf(combinedenv, "%s=%s%s", oneenv, oldval, (addstring));
            xfree(oneenv);
            oneenv = combinedenv;
          }
          else 
          {
            // oneenv is now VARxxVALUE, so fix it to be a normal env. variable format 
            strcat(oneenv, "=");
            memmove(oneenv+strlen(oneenv), addstring, strlen(addstring) + 1);
          }
        }

        putenv(oneenv);
      }
    }
    stackfclose(fd);
  }
  else 
  {
    errprintf("Cannot open env file %s - %s\n", envfile, strerror(errno));
  }

  freestrbuffer(inbuf);
//MEMUNDEFINE(l);
}


