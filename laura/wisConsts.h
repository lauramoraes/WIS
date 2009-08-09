/*
  Kaio Karam <kaiokaram@lps.ufrj.br>
 */

#ifndef __CONSTS__
#define __CONSTS__

#define SIZE                    1025
#define EXPIRATION_TIME         172800  /*48 x 60 x 60 */
//#define GETNTUPLE           "/afs/cern.ch/user/t/tilebeam/offline/tape-info/get_ntuple"
#define MAX_LENGTH_COMMAND     1024
#define MAX_LENGTH_FILENAME    1024
#define MAX_LENGTH_STRING      1024

#define TILEBEAM_UID           13977
#define TILEBEAM_GID           1307
#define TILEBEAM_HOME          "/afs/cern.ch/user/t/tilebeam"
#define TILECOM_HOME          "/afs/cern.ch/user/t/tilecom"

#define COMM_FILES             "/castor/cern.ch/user/t/tilebeam/commissioning"
#define CASTOR_COMM_FILES      "/castor/cern.ch/user/t/tilebeam/commissioning"
#define LOCAL_COMM_FILES       "/work/tiletransfer/wis"
#define LOCAL_COMM_POOL_FILES  "/afs/cern.ch/user/t/tilebeam/scratch0/Commissioning/tilec/TileCalorimeter/TileExample/TileRecEx/TileRecEx-00-00-04/run/data"

#define WIS_OUTPUT_DIR         "/work/tiletransfer/wis/"
//#define ROOT_SYS               "/usr/local/root/5.10.00e/slc3_ia32_gcc323/root"
#define ROOT_PATH              "/afs/cern.ch/sw/lcg/external/root/5.06.00/slc3_gcc323/root/bin/"
#define ROOT_SYS               "/afs/cern.ch/sw/lcg/external/root/5.06.00/slc3_gcc323/root"

//#define ROOT_SYS_INTEGRATOR    "/usr/local/root/5.10.00e/slc3_ia32_gcc323/root"
#define ROOT_SYS_INTEGRATOR    "/afs/cern.ch/sw/lcg/external/root/5.06.00/slc3_gcc323/root/bin/"
#define ROOT_PATH_INTEGRATOR   "/afs/cern.ch/sw/lcg/external/root/5.06.00/slc3_gcc323/root"

//#define ROOT_SYS_INTEGRATOR    "/usr/local/root/5.10.00e/slc3_ia32_gcc323/root"
//#define ROOT_PATH_INTEGRATOR   "/usr/local/root/5.10.00e/slc3_ia32_gcc323/root/bin"

#define WORK_DIRECTORY         "/work/tiletransfer/atlastransfer"
#define WORK_DIRECTORY_ONLINE  "/work/tiletransfer/atlastransfer/online"
#define WORK_DIRECTORY_OFFLINE "/work/tiletransfer/atlastransfer/offline"

#define TEMP_DIRECTORY         "/work/tiletransfer/atlastransfer"
#define TEMP_DIRECTORY_ONLINE  "/work/tiletransfer/atlastransfer/online"
#define TEMP_DIRECTORY_OFFLINE "/work/tiletransfer/atlastransfer/offline"

#define WGET                   "wget"
#define DQ_SCRIPTS             "dq"

#define DC2_DIRECTORY          "/work/tiletransfer/atlastransfer/DC2"
#define DC2_SCRIPTS            "/work/tiletransfer/atlastransfer/DC2/scripts/"
#define DC2_SCRIPTSNODQ        "/work/tiletransfer/atlastransfer/DC2/scriptsnodq/"
#define WGET_SCRIPTS           "/work/tiletransfer/atlastransfer/DC2/wgetscripts/"

#define FILES_DIRECTORY        "/work/combined"
#define NTUPLES_DIRECTORY      "/work/combined"
#define WEB_SERVER             "http://tileinfo.web.cern.ch"
#define SYSTEM_HOME            "tileinfo/lps/CTB"
#define FILES_WEBDIR           "tileinfo/tiletransfer/atlastransfer"
#define FILES_WEBDIR_ONLINE    "tileinfo/tiletransfer/atlastransfer/online"
#define FILES_WEBDIR_OFFLINE   "tileinfo/tiletransfer/atlastransfer/offline"
#define TEMP_WEBDIR            "tileinfo/tiletransfer/atlastransfer"
#define TEMP_WEBDIR_ONLINE     "tileinfo/tiletransfer/atlastransfer/online"
#define TEMP_WEBDIR_OFFLINE    "tileinfo/tiletransfer/atlastransfer/offline"
#define CASTOR_TILEBEAM        "/castor/cern.ch/user/t/tilebeam"
#define CASTOR_ONLINE          "/castor/cern.ch/user/t/tilebeam/online"
#define CASTOR_OFFLINE         "/castor/cern.ch/user/t/tilebeam/offline"
#define RFCP                   "/usr/local/bin/rfcp"
#define VDEV                   "/local_disk/httpd/html/tileinfo/lps/CTB/VDev"

#define JAVA_HOME  "/afs/cern.ch/sw/java/i386_redhat61/jdk/sun-1.4.0"
#define XALAN_HOME "/afs/cern.ch/sw/java/share/opt/apache/xalan-2.4.0"

#define JAVA_BIN  "/afs/cern.ch/sw/java/i386_redhat61/jdk/sun-1.4.0/bin"
#define XALAN_BIN "/afs/cern.ch/sw/java/share/opt/apache/xalan-2.4.0/bin"

#define JAVA     "/afs/cern.ch/sw/java/i386_redhat61/jdk/sun-1.4.0/bin/java"
#define JAVA1_5  "/afs/cern.ch/sw/java/i386_redhat61/jdk/sun-1.5.0/bin/java"
#define JAVAC    "/afs/cern.ch/sw/java/i386_redhat61/jdk/sun-1.4.0/bin/javac"
#define XALAN    "org.apache.xalan.xslt.Process"

#define AMI_ROOT  "/local_disk/httpd/html/tile/info/lps/AMI/AMI3"

#define AMI_CLASS "/home/httpd/html/tile/info/lps/AMI/AMI3/lib/AMI.jar:/home/httpd/html/tile/info/lps/AMI/AMI3/lib/BkkJDBC.jar:/home/httpd/html/tile/info/lps/AMI/AMI3/lib/axis.jar:/home/httpd/html/tile/info/lps/AMI/AMI3/lib/commons-logging-1.0.4.jar:/home/httpd/html/tile/info/lps/AMI/AMI3/lib/jaxrpc.jar:/home/httpd/html/tile/info/lps/AMI/AMI3/lib/commons-discovery-0.2.jar:/home/httpd/html/tile/info/lps/AMI/AMI3/lib/saaj.jar"

#define CONF_FILE "AMIClient.cnf"
#define AMI       "-Dconfigfile=/local_disk/httpd/html/tile/info/lps/AMI/AMI3/AMIClient.cnf net.hep.atlas.Database.Bookkeeping.AMI.Webservice.AMIWebServiceClient"
#define DQ        "/afs/cern.ch/atlas/offline/external/DQClient/dms3/dms3.py"

/*
#define NSLS            "/usr/local/bin/nsls"
#define BASE_PATH       "/castor/cern.ch/user/t/tilebeam/online/"
*/

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

