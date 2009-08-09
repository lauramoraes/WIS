/*
  Kaio Karam Galvao <Kaio.Galvao@cern.ch>
  File: wisMacroConsts.h
  Description: Defines some constants to be used in all ROOT macros.
 */

#ifndef __MACRO_CONSTS__
#define __MACRO_CONSTS__

// tilebeam user ID and group ID
#define TILEBEAM_UID           17767
#define TILEBEAM_GID           1307
#define TILEBEAM_HOME          "/afs/cern.ch/user/t/tilebeam"

// CASTOR directory where are stored commissioning reconstructed files (ntuples)
#define COMM_NTUPLES           "/castor/cern.ch/user/t/tilebeam/commissioning"

// Path and home for the ROOT Framework
#define ROOT_SYS               "/usr/local/root/5.10.00e/slc3_ia32_gcc323/root"
#define ROOT_PATH              "/usr/local/root/5.10.00e/slc3_ia32_gcc323/root/bin"

// Local directory where macro results are stored
#define WIS_OUTPUT_DIR         "/work/tiletransfer/wis/"

// General purpose contants
#define WEB_SERVER             "http://tileinfo.web.cern.ch"
#define SYSTEM_HOME            "tileinfo/lps/WIS"
#define CASTOR_TILEBEAM        "/castor/cern.ch/user/t/tilebeam"
#define RFCP                   "/usr/local/bin/rfcp"

// Error return constants
#define OK                           0
#define INVALID_ARGS                 1
#define FILE_NOT_FOUND               2
#define ERROR_OPENING_DIRECTORY      3
#define GET_NTUPLE_ERROR             4
#define RFCP_ERROR                   5
#define FILE_ERROR                   6
#define PROCESS_ERROR                7
#define NO_RETURNING_CODE            8
#define LINK_COULD_NOT_BE_CREATED    9
#define NSLS_ERROR                  10
#define RFCP_256_ERROR              11
#define RFCP_7168_ERROR             12

#endif
