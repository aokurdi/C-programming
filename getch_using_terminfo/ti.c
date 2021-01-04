/*
 * =====================================================================================
 *
 *       Filename:  ti.c
 *
 *    Description: Reads terminfo db and find the key pressed from the db
 *
 *        Version:  1.0
 *        Created:  12/21/2020 04:56:50 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ayman Kurdi (AK), a.o.kurdi2@gmail.com
 *   Organization:  Arees Group
 *
 * =====================================================================================
 */

#include    <stdint.h>
#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdbool.h>
#include	<stdlib.h>

#include    "ti.h"

/* Key names if user which to get the name of key pressed
 * the order and abriviations are from ncuses source code
 * order is important, DON'T CHANGE */
static const char *KeyName[] = {
    "cbt"  , "bel"  , "cr"   , "csr"  , "tbc"  , "clear", "el"   , "ed"   , "hpa"  ,
    "cmdch", "cup"  , "cud1" , "home" , "civis", "cub1" , "mrcup", "cnorm", "cuf1" ,
    "ll"   , "cuu1" , "cvvis", "dch1" , "dl1"  , "dsl"  , "hd"   , "smacs", "blink",
    "bold" , "smcup", "smdc" , "dim"  , "smir" , "invis", "prot" , "rev"  , "smso" ,
    "smul" , "ech"  , "rmacs", "sgr0" , "rmcup", "rmdc" , "rmir" , "rmso" , "rmul" ,
    "flash", "ff"   , "fsl"  , "is1"  , "is2"  , "is3"  , "if"   , "ich1" , "il1"  ,
    "ip"   , "kbs"  , "ktbc" , "kclr" , "kctab", "kdch1", "kdl1" , "kcud1", "krmir",
    "kel"  , "ked"  , "kf0"  , "kf1"  , "kf10" , "kf2"  , "kf3"  , "kf4"  , "kf5"  ,
    "kf6"  , "kf7"  , "kf8"  , "kf9"  , "khome", "kich1", "kil1" , "kcub1", "kll"  ,
    "knp"  , "kpp"  , "kcuf1", "kind" , "kri"  , "khts" , "kcuu1", "rmkx" , "smkx" ,
    "lf0"  , "lf1"  , "lf10" , "lf2"  , "lf3"  , "lf4"  , "lf5"  , "lf6"  , "lf7"  ,
    "lf8"  , "lf9"  , "rmm"  , "smm"  , "nel"  , "pad"  , "dch"  , "dl"   , "cud"  ,
    "ich"  , "indn" , "il"   , "cub"  , "cuf"  , "rin"  , "cuu"  , "pfkey", "pfloc",
    "pfx"  , "mc0"  , "mc4"  , "mc5"  , "rep"  , "rs1"  , "rs2"  , "rs3"  , "rf"   ,
    "rc"   , "vpa"  , "sc"   , "ind"  , "ri"   , "sgr"  , "hts"  , "wind" , "ht"   ,
    "tsl"  , "uc"   , "hu"   , "iprog", "ka1"  , "ka3"  , "kb2"  , "kc1"  , "kc3"  ,
    "mc5p" , "rmp"  , "acsc" , "pln"  , "kcbt" , "smxon", "rmxon", "smam" , "rmam" ,
    "xonc" , "xoffc", "enacs", "smln" , "rmln" , "kbeg" , "kcan" , "kclo" , "kcmd" ,
    "kcpy" , "kcrt" , "kend" , "kent" , "kext" , "kfnd" , "khlp" , "kmrk" , "kmsg" ,
    "kmov" , "knxt" , "kopn" , "kopt" , "kprv" , "kprt" , "krdo" , "kref" , "krfr" ,
    "krpl" , "krst" , "kres" , "ksav" , "kspd" , "kund" , "kBEG" , "kCAN" , "kCMD" ,
    "kCPY" , "kCRT" , "kDC"  , "kDL"  , "kslt" , "kEND" , "kEOL" , "kEXT" , "kFND" ,
    "kHLP" , "kHOM" , "kIC"  , "kLfFT", "kMSG" , "kMOV" , "kNXT" , "kOPT" , "kPRV" ,
    "kPRT" , "kRDO" , "kRPL" , "kRIT" , "kRES" , "kSAV" , "kSPD" , "kUND" , "rfi"  ,
    "kf11" , "kf12" , "kf13" , "kf14" , "kf15" , "kf16" , "kf17" , "kf18" , "kf19" ,
    "kf20" , "kf21" , "kf22" , "kf23" , "kf24" , "kf25" , "kf26" , "kf27" , "kf28" ,
    "kf29" , "kf30" , "kf31" , "kf32" , "kf33" , "kf34" , "kf35" , "kf36" , "kf37" ,
    "kf38" , "kf39" , "kf40" , "kf41" , "kf42" , "kf43" , "kf44" , "kf45" , "kf46" ,
    "kf47" , "kf48" , "kf49" , "kf50" , "kf51" , "kf52" , "kf53" , "kf54" , "kf55" ,
    "kf56" , "kf57" , "kf58" , "kf59" , "kf60" , "kf61" , "kf62" , "kf63" , "el1"  ,
    "mgc"  , "smgl" , "smgr" , "fln"  , "sclk" , "dclk" , "rmclk", "cwin" , "wingo",
    "hup"  , "dial" , "qdial", "tone" , "pulse", "hook" , "pause", "wait" , "u0"   ,
    "u1"   , "u2"   , "u3"   , "u4"   , "u5"   , "u6"   , "u7"   , "u8"   , "u9"   ,
    "op"   , "oc"   , "initc", "initp", "scp"  , "setf" , "setb" , "cpi"  , "lpi"  ,
    "chr"  , "cvr"  , "defc" , "swidm", "sdrfq", "sitm" , "slm"  , "smicm", "snlq" ,
    "snrmq", "sshm" , "ssubm", "ssupm", "sum"  , "rwidm", "ritm" , "rlm"  , "rmicm",
    "rshm" , "rsubm", "rsupm", "rum"  , "mhpa" , "mcud1", "mcub1", "mcuf1", "mvpa" ,
    "mcuu1", "porder", "mcud" , "mcub" , "mcuf" , "mcuu" , "scs"  , "smgb" , "smgbp",
    "smglp", "smgrp", "smgt" , "smgtp", "sbim" , "scsd" , "rbim" , "rcsd" , "subcs",
    "supcs", "docr" , "zerom", "csnm" , "kmous", "minfo", "reqmp", "getm" , "setaf",
    "setab", "pfxl" , "devt" , "csin" , "s0ds" , "s1ds" , "s2ds" , "s3ds" , "smglr",
    "smgtb", "birep", "binel", "bicr" , "colornm" , "defbi"   , "endbi"   , "setcolor",
    "slines"  , "dispc"   , "smpch"   , "rmpch"   , "smsc"    , "rmsc"    , "pctrm"   ,
    "scesc"   , "scesa"   , "ehhlm"   , "elhlm"   , "elohlm"  , "erhlm"   , "ethlm"   ,
    "evhlm"   , "sgr1"    , "slength" , "OTi2"    , "OTrs"    , "OTnl"    , "OTbc"    ,
    "OTko"    , "OTma"    , "OTG2"    , "OTG3"    , "OTG1"    , "OTG4"    , "OTGR"    ,
    "OTGL"    , "OTGU"    , "OTGD"    , "OTGH"    , "OTGV"    , "OTGC"    , "meml"    ,
    "memu"    , "box1"
};

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_code_from_ti
 *  Description: This routine will open a ti db and read the strings cap in a buffer
 *              then will try to compare the string keyStr, if found match to keyStr
 *              it will gnenrate a code > 255 depending on string passed and return it
 *  @Param
 *  keyStr: escape sequence from a keypress.
 *  @Return
 *  int: code > 255 or -1 if key is not in terminfo db
 *
 *  NOTE: documintatin about terminf is scatered on internet, so i try to simpliefy it
 *      for begainers "like me", i will heavly document what i am doing in code, but
 *      i will try to explain all in the README file; if you need help drop me a line.
 *
 *      I did not try to protect against errors so much cause my aim is to try to make
 *      this as simple to follow as possible :')
 * =====================================================================================
 */
    int
get_code_from_ti( const char *keyStr )
{
    FILE *tinfoDB;
    char *buffer, *pos, *startOfTbl;
    int dbSize, intSize, offset;
    uint16_t header[6], magic, nameLen, boolLen;
    uint16_t numLen, strsLen;

    tinfoDB = open_terminfo_file( );

    dbSize = get_db_size( tinfoDB );

    buffer = malloc( dbSize + 1 );
    if(dbSize != fread( buffer, sizeof (char), dbSize, tinfoDB) ) {
        free( buffer );
        fprintf( stderr, "Error while reading terminfo db\n" );
        return EXIT_FAILURE;
    }

    fclose( tinfoDB );

    read_terminfo_header( buffer, header );
    pos    = buffer;
    magic  = header[0];
    nameLen = header[1];
    boolLen = header[2];
    numLen  = header[3];
    strsLen = header[4];

    if( magic != MAGIC16 && magic != MAGIC32 ) {
        fprintf( stderr, "Unknow terminfo format! %o\n", magic );
        exit( -1 );
    }

    intSize = (magic == 01036 ? 4 : 2 );

    offset = 12 + nameLen + boolLen + numLen * intSize;

    /* Apply padding if needed */
    if( (nameLen + boolLen) % 2 )
        offset++;

    pos += offset;  /* At start of strs */
    startOfTbl = pos + (strsLen * 2);

    for( int i = 0; i < strsLen ; ++i ) {
        offset = get_str_cap(pos);
        pos += 2;

        if( offset != -1 ) {
            if( strcmp( keyStr, &startOfTbl[offset] ) == 0) {
                free( buffer );
                return code_begain + i;
            }
        }
    }

    /* Memory cleanup */
    free( buffer );

    return -1;
}				/* ----------  end of function main  ---------- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  read_terminfo_header
 *  Description: Terminfo header is 12 bytes, each 2 bytes represnt a field as follows:
 *          1st 2bytes: Magic number which is 01036 for 32bit db or 0432 for 16bits
 *          2nd 2bytes: Terminal name length
 *          3rd 2bytes: Boolean section length
 *          4th 2bytes: Numaric section length
 *          5th 2bytes: Strings cap length
 *          6th 2bytes: Strings "escape sequence" table size; after that you ll have
 *          the second header for extended caps which is also 12 bytes but out of
 *          scope of this example
 * =====================================================================================
 */
    void
read_terminfo_header ( const char *buffer, uint16_t *header )
{
    size_t i, j = 0;
    const unsigned char *temp = (const unsigned char *) buffer;

    for( i = 0; i < 5; i++, j += 2 )
        header[i] = temp[j] + temp[j + 1] * 256u;

}		/* -----  end of function read_terminfo_header  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_str_cap
 *  Description:
 * =====================================================================================
 */
    int
get_str_cap( const char *pos )
{
    unsigned int ret;
    const unsigned char *temp = (const unsigned char *)pos;

    /* The byte order is in little Endian order, meaning first byte = least seg bit
     * second byte = most seg bit; so to convert those 2 chars into a unsigned 16 bit
     * int we multiply the second byte by 256 and add the first byte */
    ret = temp[0] + temp[1] * 256u;
    return ret <= MAX_S16 ? ret : -1;
}		/* -----  end of function get_str_cap  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  open_terminfo_file
 *  Description: Tries to open terminfo db, returns Null on fail... user must deal with
 *          that; not a rebust way, BUT just an example
 * =====================================================================================
 */
    FILE *
open_terminfo_file( )
{
    FILE *fp;
    char *term, *home, dbPath[1024];

    term = NULL;
    term = getenv( "TERM" );
    if( !term )
        term = "linux";

    char *path[] = {
                    "/lib/terminfo/",
                    "/etc/terminfo/",
                    "/usr/share/terminfo/",
    };

    for ( int i = 0; i < 3; ++i ) {
        snprintf( dbPath, 1024, "%s%c/%s", path[i], term[0], term);
        if( (fp = fopen( dbPath, "rb" )) ){
            return fp;
        }
    }

    home = getenv( "HOME" );
    snprintf( dbPath, 1024, "%s/.terminfo/%c/%s", home, term[0], term);
    if( (fp = fopen( dbPath, "rb" )) )
        return fp;

    return NULL;
}		/* -----  end of function open_terminfo_file  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_db_size
 *  Description:
 * =====================================================================================
 */
    size_t
get_db_size( FILE *db )
{
    size_t dbSize;

    fseek( db, 0, SEEK_END );
    dbSize = ftell( db );
    rewind( db );

    return dbSize;
}		/* -----  end of function get_db_size  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_key_name
 *  Description: rturns a string represinting the key name as in ncurses names
 * =====================================================================================
 */
    const char *
get_key_name( int kcode )
{
    return KeyName[kcode-256];
}		/* -----  end of function get_key_name  ----- */
