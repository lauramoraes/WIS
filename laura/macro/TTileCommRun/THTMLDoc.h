#ifndef THTMLDoc_h
#define THTMLDoc_h

#include <TROOT.h>
#include <iostream>

using namespace std;

//==== some definitions ====
const Int_t NtileDMUs = 16;
const TString defaultStyleSheet = "/home/httpd/html/tile/info/lps/WIS/dev/css/standard.css";


class THTMLDoc
{
	// HTML page elements
	TString fTitle;
	TString fPageTitle;
	TString fStyleSheets;
	TString fScripts;
	TString fTopBar;
	TString fHeader;
	TString fMainContainer;
	TString fContents;
	TString fFooter;
	
public:

	THTMLDoc(TString title, TString pageTitle);
	THTMLDoc::~THTMLDoc();
	void AddTable(Int_t *matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows = NULL);
	void AddTable(vector< vector<Int_t> > *matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows = NULL);
	void THTMLDoc::AddTable(TString &table, const vector< vector<Int_t> > &matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows);
	void AddImage(TString imageTitle, TString imageURL);

};

#endif

