#ifndef FILEHEADER_H
#define FILEHEADER_H

#define PACKSTRUCT	__attribute__ ((packed))

//#ifdef TIMET64
//typedef int32_t time4_t;
//#else
//typedef time_t time4_t;
//#endif

typedef int32_t time4_t;


#define IPV4LEN	    (15)    /* a.b.c.d form */

#if 1
#define IDLEN	     12     /* Length of bid/uid */
#define TTLEN      64             /* Length of title */
#define FNLEN      28             /* Length of filename */

// Prefix of str_reply and str_forward usually needs longer.
// In artitlcle list, the normal length is (80-34)=46.
#define DISP_TTLEN  46

typedef struct fileheader_t { /* 128 bytes */
    char    filename[FNLEN];         /* M.1120582370.A.1EA [19+1], create time */
    time4_t modified;		     /* last modified time */
    char    pad;		     /* padding, not used */
    char    recommend;               /* important level */
    char    owner[IDLEN + 2];        /* uid[.] */
    char    date[6];                 /* [02/02] or space(5) */
    char    title[TTLEN + 1];
    /* TODO this multi is a mess now. */
    char    pad2;
    union {
    /* TODO: MOVE money to outside multi!!!!!! */
    int money;
    int anon_uid;
    /* different order to match alignment */
    struct {
        unsigned char posts;
        unsigned char logins;
        unsigned char regtime;
        unsigned char badpost;
    } vote_limits;
    struct {
        /* is this ordering correct? */
        unsigned int ref:31;
        unsigned int flag:1;
    } refer;
    }	    multi;		    /* rocker: if bit32 on ==> reference */
    /* XXX dirty, split into flag and money if money of each file is less than 16bit? */
    unsigned char   filemode;        /* must be last field @ boards.c */
    char    pad3[3];
} fileheader_t;
#else
#define TTLEN      64             /* Length of title */
#define FNLEN      33             /* Length of filename  */
#define IDLEN	     12     /* Length of bid/uid */

typedef struct fileheader_t {
    char    filename[FNLEN];         /* M.9876543210.A */
    char    recommend;               /* important level */
    char    owner[IDLEN + 2];        /* uid[.] */
    char    date[6];                 /* [02/02] or space(5) */
    char    title[TTLEN + 1];
    union {
    int money;
    int anon_uid;
    /* different order to match alignment */
#ifdef _BIG_ENDIAN
    struct {
        unsigned char pad[2];   /* money & 0xffff0000 */
        unsigned char logins;   /* money & 0xff00 */
        unsigned char posts;    /* money & 0xff */
    } vote_limits;
    struct {
        unsigned int flag:1;
        unsigned int ref:31;
    } refer;
#else
    struct {
        unsigned char posts;    /* money & 0xff */
        unsigned char logins;   /* money & 0xff00 */
        unsigned char pad[2];   /* money & 0xffff0000 */
    } vote_limits;
    struct {
        unsigned int ref:31;
        unsigned int flag:1;
    } refer;
#endif
    }       multi;          /* rocker: if bit32 on ==> reference */
    /* XXX dirty, split into flag and money if money of each file is less than 16bit? */
    unsigned char   filemode;        /* must be last field @ boards.c */
} fileheader_t;
#endif
#endif // FILEHEADER_H
