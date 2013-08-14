#ifndef BBS2HTML_H
#define BBS2HTML_H

#include <QString>

class BBS2Html
{
public:
    BBS2Html();

    static int ann2htmlFile( const char *bbsfile, const char *htmlfile, char *tit );
    static int ann2html( QString& out, const char *htmlfile, char *tit );

};

#endif // BBS2HTML_H
