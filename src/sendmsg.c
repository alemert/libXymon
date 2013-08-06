/******************************************************************************/
/*                                                                            */
/*                          X Y M O N   L I B R A R Y                         */
/*                                                                            */
/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
/*  description:                                                              */
/*    c interface for sending monitoring messages to xymon.                   */
/*    source code was taken from xymon and adjustet for internal needs.       */
/*                                                                            */
/*  functions:                                                                */
/*    - sendmessage                                                */
/*    - sendtomany                                      */
/*    - sendtoxymond                  */
/******************************************************************************/

#define _DEV_CPY_

/******************************************************************************/
/*   I N C L U D E S                                                          */
/******************************************************************************/

// ---------------------------------------------------------
// system
// ---------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/in.h>

// ---------------------------------------------------------
// own 
// ---------------------------------------------------------
#include <sendmsg.h>
#include <environ.h>
#include <memory.h>
#include <misc.h>
#include <libxymon.h>
#include <strfunc.h>

/******************************************************************************/
/*   G L O B A L S                                                            */
/******************************************************************************/
#ifdef _DEV_CPY_
  static char errordetails[1024];
#endif

static int  sleepbetweenmsgs = 0;

// ---------------------------------------------------------
// Stuff for combo message handling
// ---------------------------------------------------------
int xymonmsgcount = 0;      // Number of messages transmitted

// ---------------------------------------------------------
//  These commands go to all Xymon servers 
// ---------------------------------------------------------
static char *multircptcmds[] = { "status", "combo" , "meta"   , "data", 
                                 "notify", "enable", "disable", "drop", 
                                 "rename", "client", NULL };


/******************************************************************************/
/*   D E F I N E S                                                            */
/******************************************************************************/
#define SENDRETRIES 2

/******************************************************************************/
/*   M A C R O S                                                              */
/******************************************************************************/

/******************************************************************************/
/*   P R O T O T Y P E S                                                      */
/******************************************************************************/
#ifdef _DEV_CPY_
  extern void errprintf(const char *fmt, ...);
#endif

/******************************************************************************/
/*                                                                            */
/*   F U N C T I O N S                                                        */
/*                                                                            */
/******************************************************************************/


/******************************************************************************/
/* send message to xymon                            */
/******************************************************************************/
sendresult_t sendmessage( char *msg, 
                          char *recipient, 
                      int timeout, 
                        sendreturn_t *response)
{
  static char *xymsrv = NULL;
  int res = 0;

  *errordetails = '\0';

  if( (xymsrv == NULL) && xgetenv("XYMSRV") ) 
  {
    xymsrv = strdup(xgetenv("XYMSRV"));
  }

  if (recipient == NULL) recipient = xymsrv;

  if( (recipient == NULL) && xgetenv("XYMSERVERS") )
  {
    recipient = "0.0.0.0";
  } 
  else if (recipient == NULL) 
  {
    errprintf("No recipient for message\n");
    return XYMONSEND_EBADIP;
  }

  res = sendtomany( recipient, xgetenv("XYMSERVERS"), msg, timeout, response);

  if (res != XYMONSEND_OK) 
  {
    char *statustext = "";
    char *eoln;

    switch (res) 
    {
      case XYMONSEND_OK            : statustext = "OK"; break;
      case XYMONSEND_EBADIP        : statustext = "Bad IP address"; break;
      case XYMONSEND_EIPUNKNOWN    : statustext = "Cannot resolve hostname"; break;
      case XYMONSEND_ENOSOCKET     : statustext = "Cannot get a socket"; break;
      case XYMONSEND_ECANNOTDONONBLOCK   : statustext = "Non-blocking I/O failed"; break;
      case XYMONSEND_ECONNFAILED   : statustext = "Connection failed"; break;
      case XYMONSEND_ESELFAILED    : statustext = "select(2) failed"; break;
      case XYMONSEND_ETIMEOUT      : statustext = "timeout"; break;
      case XYMONSEND_EWRITEERROR   : statustext = "write error"; break;
      case XYMONSEND_EREADERROR    : statustext = "read error"; break;
      case XYMONSEND_EBADURL       : statustext = "Bad URL"; break;
      default:                statustext = "Unknown error"; break;
    };

    eoln = strchr(msg, '\n'); if (eoln) *eoln = '\0';
    if( strcmp(recipient, "0.0.0.0") == 0 ) 
    {
      recipient = xgetenv("XYMSERVERS");
    }

    errprintf("Whoops ! Failed to send message (%s)\n", statustext);
    errprintf("->  %s\n", errordetails);
    errprintf("->  Recipient '%s', timeout %d\n", recipient, timeout);
    errprintf("->  1st line: '%s'\n", msg);
    if (eoln) *eoln = '\n';
  }

  /* Give it a break */
  if (sleepbetweenmsgs) usleep(sleepbetweenmsgs);

  xymonmsgcount++;

  return res;
}

/******************************************************************************/
/*  send message to many xymon server                                         */
/*                                    */
/******************************************************************************/
static int sendtomany( char *onercpt, 
            char *morercpts, 
                char *msg, 
          int timeout, 
            sendreturn_t *response)
{
  int allservers = 1 ;
  int first  = 1 ;
  int result = XYMONSEND_OK;

  char *xymondlist, *rcpt;

  // -------------------------------------------------------
  // 
  // Even though this is the "sendtomany" routine, we need to decide if the  
  // request should go to all servers, or just a single server. The default 
  // is to send to all servers - but commands that trigger a response can  
  // only go to a single server. 
  // 
  // "schedule" is special - when scheduling an action there is no response, 
  // but when it is the blank "schedule" command there will be a response.  
  // So a schedule action goes to all Xymon servers, the blank "schedule"  
  // goes to a single server.           
  // 
  if (strcmp(onercpt, "0.0.0.0") != 0)
  {
    allservers = 0;
  }
  
  // -------------------------------------------------------
  // See if it's just a blank "schedule" command 
  // -------------------------------------------------------
  else if (strncmp(msg, "schedule", 8) == 0)      //
  {      //
    allservers = (strcmp(msg, "schedule") != 0);      //
  }      //

  else 
  {
    char *msgcmd;
    int i;

    // See if this is a multi-recipient command 
    i = strspn(msg, "abcdefghijklmnopqrstuvwxyz");
    msgcmd = (char *)malloc(i+1);
    strncpy(msgcmd, msg, i); *(msgcmd+i) = '\0';
    for (i = 0; (multircptcmds[i] && strcmp(multircptcmds[i], msgcmd)); i++) ;
    xfree(msgcmd);

    allservers = (multircptcmds[i] != NULL);
  }

  if (allservers && !morercpts) 
  {
    sprintf(errordetails+strlen(errordetails), "No recipients listed! XYMSRV was %s, XYMSERVERS %s",
    onercpt, textornull(morercpts));
    return XYMONSEND_EBADIP;
  }

  if (strcmp(onercpt, "0.0.0.0") != 0)
  {
    xymondlist = strdup(onercpt);
  }
  else
  {
    xymondlist = strdup(morercpts);
  }

  rcpt = strtok(xymondlist, " \t");
  while (rcpt) 
  {
    int oneres;

    if (first) 
    {
      /* We grab the result from the first server */
      char *respstr = NULL;

      if (response) 
      {
        oneres =  sendtoxymond( rcpt, msg,
                                response->respfd,
                                (response->respstr ? &respstr : NULL),
                                (response->respfd || response->respstr),
                                timeout      );
      }
      else 
      {
        oneres =  sendtoxymond(rcpt, msg, NULL, NULL, 0, timeout);
      }

      if (oneres == XYMONSEND_OK) 
      {
        if (respstr && response && response->respstr) 
        {
          addtobuffer(response->respstr, respstr);
          xfree(respstr);
        }
        first = 0;
      }
    }
    else 
    {
      /* Secondary servers do not yield a response */
      oneres =  sendtoxymond(rcpt, msg, NULL, NULL, 0, timeout);
    }

    /* Save any error results */
    if (result == XYMONSEND_OK) result = oneres;

    /*
     * Handle more servers IF we're doing all servers, OR
     * we are still at the first one (because the previous
     * ones failed).
    */
    if (allservers || first)
    {
      rcpt = strtok(NULL, " \t");
    }
    else
    {
      rcpt = NULL;
    }
  }

  xfree(xymondlist);

  return result;
}

/******************************************************************************/
/*  send to xymon deamon            */
/******************************************************************************/
static int sendtoxymond( char *recipient, 
                         char *message, 
                         FILE *respfd, 
                         char **respstr, 
                         int fullresponse, 
                         int timeout)
{
  struct in_addr addr;
  struct sockaddr_in saddr;
  int     sockfd = -1;
  fd_set  readfds;
  fd_set  writefds;
  int     res, isconnected, wdone, rdone;
  struct timeval tmo;
  char *msgptr = message;
  char *p;
  char *rcptip = NULL;
  int rcptport = 0;
  int connretries = SENDRETRIES;
  char *httpmessage = NULL;
  char recvbuf[32768];
  int haveseenhttphdrs = 1;
  int respstrsz = 0;
  int respstrlen = 0;
  int result = XYMONSEND_OK;

  if (dontsendmessages && !respfd && !respstr) 
  {
    fprintf(stdout, "%s\n", message);
    fflush(stdout);
    return XYMONSEND_OK;
  }

  setup_transport(recipient);

  dbgprintf("Recipient listed as '%s'\n", recipient);

  if (strncmp(recipient, "http://", strlen("http://")) != 0) 
  {
    /* Standard communications, directly to Xymon daemon */
    rcptip = strdup(recipient);
    rcptport = xymondportnumber;
    p = strchr(rcptip, ':');
    if( p ) 
    {
      *p = '\0'; p++; rcptport = atoi(p);
    }
    dbgprintf("Standard protocol on port %d\n", rcptport);
  }
  else 
  {
    char *bufp;
    char *posturl = NULL;
    char *posthost = NULL;

    if (xymonproxyhost == NULL) 
    {
      char *p;

      /*
       * No proxy. "recipient" is "http://host[:port]/url/for/post"
       * Strip off "http://", and point "posturl" to the part after the hostname.
       * If a portnumber is present, strip it off and update rcptport.
       */
      rcptip = strdup(recipient+strlen("http://"));
      rcptport = xymondportnumber;

      p = strchr(rcptip, '/');
      if (p) 
      {
        posturl = strdup(p);
        *p = '\0';
      }

      p = strchr(rcptip, ':');
      if (p) 
      {
        *p = '\0';
        p++;
        rcptport = atoi(p);
      }

      posthost = strdup(rcptip);

      dbgprintf("HTTP protocol directly to host %s\n", posthost);
    }
    else 
    {
      char *p;

      /*
       * With proxy. The full "recipient" must be in the POST request.
      */
      rcptip = strdup(xymonproxyhost);
      rcptport = xymonproxyport;

      posturl = strdup(recipient);

      p = strchr(recipient + strlen("http://"), '/');
      if (p) 
      {
        *p = '\0';
        posthost = strdup(recipient + strlen("http://"));
        *p = '/';

        p = strchr(posthost, ':');
        if (p) *p = '\0';
      }

      dbgprintf("HTTP protocol via proxy to host %s\n", posthost);
    }

    if ((posturl == NULL) || (posthost == NULL)) 
    {
      sprintf(errordetails + strlen(errordetails), "Unable to parse HTTP recipient");
      if (posturl) xfree(posturl);
      if (posthost) xfree(posthost);
      if (rcptip) xfree(rcptip);
      return XYMONSEND_EBADURL;
    }

    bufp = msgptr = httpmessage = malloc(strlen(message)+1024);
    bufp += sprintf(httpmessage, "POST %s HTTP/1.0\n", posturl);
    bufp += sprintf(bufp, "MIME-version: 1.0\n");
    bufp += sprintf(bufp, "Content-Type: application/octet-stream\n");
    bufp += sprintf(bufp, "Content-Length: %d\n", (int)strlen(message));
    bufp += sprintf(bufp, "Host: %s\n", posthost);
    bufp += sprintf(bufp, "\n%s", message);

    if (posturl) xfree(posturl);
    if (posthost) xfree(posthost);
    haveseenhttphdrs = 0;

    dbgprintf("HTTP message is:\n%s\n", httpmessage);
  }

  if (inet_aton(rcptip, &addr) == 0) 
  {
    /* recipient is not an IP - do DNS lookup */

    struct hostent *hent;
    char hostip[IP_ADDR_STRLEN];

    hent = gethostbyname(rcptip);
    if (hent) 
    {
      memcpy(&addr, *(hent->h_addr_list), sizeof(struct in_addr));
      strcpy(hostip, inet_ntoa(addr));

      if (inet_aton(hostip, &addr) == 0) 
      {
        result = XYMONSEND_EBADIP;
        goto done;
      }
    }
    else 
    {
      sprintf(errordetails+strlen(errordetails), "Cannot determine IP address of message recipient %s", rcptip);
      result = XYMONSEND_EIPUNKNOWN;
      goto done;
    }
  }

retry_connect:
  dbgprintf("Will connect to address %s port %d\n", rcptip, rcptport);

  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = addr.s_addr;
  saddr.sin_port = htons(rcptport);

  /* Get a non-blocking socket */
  sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) 
  { 
    result = XYMONSEND_ENOSOCKET; goto done; 
  }
  res = fcntl(sockfd, F_SETFL, O_NONBLOCK);
  if (res != 0) 
  { 
    result = XYMONSEND_ECANNOTDONONBLOCK; goto done; 
  }

  res = connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
  if( (res == -1) && (errno != EINPROGRESS) ) 
  {
    sprintf(errordetails+strlen(errordetails), "connect to Xymon daemon@%s:%d failed (%s)", rcptip, rcptport, strerror(errno));
    result = XYMONSEND_ECONNFAILED;
    goto done;
  }

  rdone = ((respfd == NULL) && (respstr == NULL));
  isconnected = wdone = 0;
  while( !wdone || !rdone ) 
  {
    FD_ZERO(&writefds);
    FD_ZERO(&readfds);
    if (!rdone) FD_SET(sockfd, &readfds);
    if (!wdone) FD_SET(sockfd, &writefds);
    tmo.tv_sec = timeout;  tmo.tv_usec = 0;
    res = select(sockfd+1, &readfds, &writefds, NULL, (timeout ? &tmo : NULL));
    if (res == -1) 
    {
      sprintf(errordetails+strlen(errordetails), "Select failure while sending to Xymon daemon@%s:%d", rcptip, rcptport);
      result = XYMONSEND_ESELFAILED;
      goto done;
    }
    else if (res == 0) 
    {
      /* Timeout! */
      shutdown(sockfd, SHUT_RDWR);
      close(sockfd);

      if (!isconnected && (connretries > 0)) 
      {
        dbgprintf("Timeout while talking to Xymon daemon@%s:%d - retrying\n", rcptip, rcptport);
        connretries--;
        sleep(1);
        goto retry_connect;     /* Yuck! */
      }

      result = XYMONSEND_ETIMEOUT;
      goto done;
    }
    else 
    {
      if (!isconnected) 
      {
        /* Havent seen our connect() status yet - must be now */
        int connres;
        socklen_t connressize = sizeof(connres);

        res = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &connres, &connressize);
        dbgprintf("Connect status is %d\n", connres);
        isconnected = (connres == 0);
        if (!isconnected) 
        {
          sprintf(errordetails+strlen(errordetails), "Could not connect to Xymon daemon@%s:%d (%s)",
          rcptip, rcptport, strerror(connres));
          result = XYMONSEND_ECONNFAILED;
          goto done;
        }
      }

      if (!rdone && FD_ISSET(sockfd, &readfds)) 
      {
        char *outp;
        int n;

        n = recv(sockfd, recvbuf, sizeof(recvbuf)-1, 0);
        if (n > 0) 
        {
          dbgprintf("Read %d bytes\n", n);
          recvbuf[n] = '\0';

          /*
           * When running over a HTTP transport, we must strip
           * off the HTTP headers we get back, so the response
           * is consistent with what we get from the normal Xymon daemon
           * transport.
           * (Non-http transport sets "haveseenhttphdrs" to 1)
          */
          if (!haveseenhttphdrs) 
          {
            outp = strstr(recvbuf, "\r\n\r\n");
            if( outp ) 
            {
              outp += 4;
              n -= (outp - recvbuf);
              haveseenhttphdrs = 1;
            }
            else
            {
              n = 0;
            }
          }
          else 
          {
            outp = recvbuf;
          }

          if (n > 0) 
          {
            if (respfd) 
            {
              fwrite(outp, n, 1, respfd);
            }
            else if (respstr) 
            {
              char *respend;

              if (respstrsz == 0) 
              {
                respstrsz = (n+sizeof(recvbuf));
                *respstr = (char *)malloc(respstrsz);
              }
              else if ((n+respstrlen) >= respstrsz) 
              {
                respstrsz += (n+sizeof(recvbuf));
                *respstr = (char *)realloc(*respstr, respstrsz);
              }
              respend = (*respstr) + respstrlen;
              memcpy(respend, outp, n);
              *(respend + n) = '\0';
              respstrlen += n;
            }
            if (!fullresponse) 
            {
              rdone = (strchr(outp, '\n') == NULL);
            }
          }
        }
        else 
        {
          rdone = 1;
        }
        if (rdone) shutdown(sockfd, SHUT_RD);
      }

      if (!wdone && FD_ISSET(sockfd, &writefds)) 
      {
        /* Send some data */
        res = write(sockfd, msgptr, strlen(msgptr));
        if (res == -1) 
        {
          sprintf(errordetails+strlen(errordetails), "Write error while sending message to Xymon daemon@%s:%d", rcptip, rcptport);
          result = XYMONSEND_EWRITEERROR;
          goto done;
        }
        else 
        {
          dbgprintf("Sent %d bytes\n", res);
          msgptr += res;
          wdone = (strlen(msgptr) == 0);
          if (wdone) shutdown(sockfd, SHUT_WR);
        }
      }
    }
  }

done:
  dbgprintf("Closing connection\n");
  shutdown(sockfd, SHUT_RDWR);
  if (sockfd > 0) close(sockfd);
  xfree(rcptip);
  if (httpmessage) xfree(httpmessage);
  return result;
}

