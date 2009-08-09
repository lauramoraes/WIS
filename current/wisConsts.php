<?php

	define("WIS_OUTPUT_DIR", "/work/tiletransfer/wis/");

	define( "JAVA_HOME", "/afs/cern.ch/sw/java/i386_redhat61/jdk/sun-1.4.0");
	define("XALAN_HOME", "/afs/cern.ch/sw/java/share/opt/apache/xalan-2.4.0");

	define( "JAVA_BIN", "/afs/cern.ch/sw/java/i386_redhat61/jdk/sun-1.4.0/bin");
	define("XALAN_BIN", "/afs/cern.ch/sw/java/share/opt/apache/xalan-2.4.0/bin");

	define( "JAVA", "/afs/cern.ch/sw/java/i386_redhat61/jdk/sun-1.4.0/bin/java");
	define( "JAVA1_5", "/afs/cern.ch/sw/java/i386_redhat61/jdk/sun-1.5.0/bin/java");
	define("JAVAC", "/afs/cern.ch/sw/java/i386_redhat61/jdk/sun-1.4.0/bin/javac");
	define("XALAN", "org.apache.xalan.xslt.Process");
	define("XSLTC", "org.apache.xalan.xsltc.cmdline");

 	define( "AMI_ROOT", "/local_disk/httpd/html/tile/info/lps/AMI/AMI3");
	define("AMI_CLASS", "/local_disk/httpd/html/tile/info/lps/AMI/AMI3/lib/AMI.jar");
	define("CONF_FILE", "AMIClient.cnf");
	define(      "AMI", "-Dconfigfile=".AMI_ROOT."/".CONF_FILE.
                 " net.hep.atlas.Database.Bookkeeping.AMI.Webservice.AMIWebServiceClient");

	define( "RFCP_256_ERROR", 11);
	define("RFCP_7168_ERROR", 12);

	define(    "AT_TEMPORARY_DIRECTORY", "/work/tiletransfer/atlastransfer");
	define("AT_WEB_TEMPORARY_DIRECTORY", "http://tileinfo.web.cern.ch/tileinfo/tiletransfer/atlastransfer");

	define(   "DC2_DIRECTORY", "/work/tiletransfer/dc2");
	define(     "DC2_SCRIPTS", "/work/tiletransfer/dc2/scripts");
	define(    "WGET_SCRIPTS", "/work/tiletransfer/dc2/wgetscripts");
	define("DC2_SCRIPTS_NODQ", "/work/tiletransfer/dc2/scriptsnodq");

?>

