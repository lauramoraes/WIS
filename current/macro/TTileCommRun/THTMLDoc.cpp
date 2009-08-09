#define THTMLDoc_cxx

#include "THTMLDoc.h"

#ifdef THTMLDoc_cxx

THTMLDoc::THTMLDoc(TString title, TString pageTitle)
{
	// Initialize HTML page elements
	fTitle  = "<title>";
	fTitle += title;
	fTitle += "</title>\n";

	fPageTitle  = "<h2>";
	fPageTitle += pageTitle;
	fPageTitle += "</h2>\n";
	
	fStyleSheets = "<link href=\"";
	fStyleSheets += defaultStyleSheet;
	fStyleSheets += "\" type=\"text/css\" rel=\"stylesheet\" />\n";

	fScripts = "";

	fTopBar  = "<!-- Top bar -->\n";
	fTopBar += "<div id=\"top-bar\">\n";
	fTopBar += "	[ <a href=\"http://tileinfo.web.cern.ch/tileinfo/lps/WIS/hp/\">Project Home Page</a> ]&nbsp;.:.&nbsp;\n";
	fTopBar += "	[ <a href=\"http://www.cern.ch/\">CERN</a> ]&nbsp;.:.&nbsp;\n";
	fTopBar += "	[ <a href=\"http://atlas.web.cern.ch/\">ATLAS</a> ]&nbsp;.:.&nbsp;\n";
	fTopBar += "	[ <a href=\"http://atlas.web.cern.ch/Atlas/SUB_DETECTORS/TILE/tilecal.html\">Tile Calorimeter</a> ]&nbsp;.:.&nbsp;\n";
	fTopBar += "	[ <a href=\"http://www.ufrj.br\">Universidade Federal do Rio de Janeiro</a> ]&nbsp;\n";
	fTopBar += "</div>\n";
	fTopBar += "<!-- /Top bar -->\n";

	fHeader  = "<!-- Header -->\n";
	fHeader += "<div id=\"header\">\n";
	fHeader += "	<h1>TileCal Commissioning Offline Shifts</h1>\n";
	fHeader += "</div>\n";
	fHeader += "<!-- /Header -->\n";

	fMainContainer  = "<!-- Main Container -->\n";
	fMainContainer += "<div id=\"main-container\">\n";

	fContents  = "	<!-- Contents -->\n";
	fContents  = "	<div id=\"contents\">\n";

	fFooter  = "<!-- Footer -->\n";
	fFooter += "<div id=\"footer\">\n";
	fFooter += "&nbsp;Please <a href=\"mailto:Kaio.Galvao@cern.ch,Carmen.Maidantchik@cern.ch\">send us</a> your comments and suggestions.\n";
	fFooter += "</div>\n";
	fFooter += "<!-- /Footer -->\n";
	
}


void THTMLDoc::AddTable(Int_t *matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows)
{
	Int_t i, j;
	TString table;
	
	table  = "<table class=\"main\">\n<tr><th></th>";
	table += "<th colspan=\"";
	table += ncols;
	table += "\">";
	table += colsTitle;
	table += "</th></tr>\n";
	table += "<tr><th>";
	table += rowsTitle;
	table += "</th>";
/*
	cout << (matrix + (48*ncols) + 0) << endl;
	cout << &NSlinkErrors[48][0] << endl;
*/
	for (j = 0; j < ncols; j++)
	{
		table += "<th>";
		table += colNames[j];
		table += "</th>";
	}
	table += "</tr>\n";

	for (i = 0; i < nrows; i++)
	{
		if (selectedRows && !selectedRows[i])
			continue;
		
		table += "<tr><th>";
		table += rowNames[i];
		table += "</th>";
		for (j = 0; j < ncols; j++)
		{
			table += "<td>";
			table += *(matrix + (i*ncols) + j);
			table += "</td>";
		}
		table += "</tr>\n";

	}
	table += "</table>\n";

	fContents += table;

}


void THTMLDoc::AddTable(TString &table, const vector< vector<Int_t> > &matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows)
{
	Int_t i, j;
	TString table;
	
	table  = "<table class=\"main\">\n<tr><th></th>";
	table += "<th colspan=\"";
	table += ncols;
	table += "\">";
	table += colsTitle;
	table += "</th></tr>\n";
	table += "<tr><th>";
	table += rowsTitle;
	table += "</th>";
/*
	cout << (matrix + (48*ncols) + 0) << endl;
	cout << &NSlinkErrors[48][0] << endl;
*/
	for (j = 0; j < ncols; j++)
	{
		table += "<th>";
		table += colNames[j];
		table += "</th>";
	}
	table += "</tr>\n";

	for (i = 0; i < nrows; i++)
	{
		if (selectedRows && !selectedRows[i])
			continue;
		
		table += "<tr><th>";
		table += rowNames[i];
		table += "</th>";
		for (j = 0; j < ncols; j++)
		{
			table += "<td>";
			table += matrix.at(i).at(j);
			table += "</td>";
		}
		table += "</tr>\n";

	}
	table += "</table>\n";

	fContents += table;

}


THTMLDoc::~THTMLDoc()
{
}


THTMLDoc::Output()
{
	fContents += "	</div>\n";
	fContents += "	<!-- /Contents -->\n";

	fMainContents += fContents;
	fMainContents += "</div>\n";
	fMainContents += "<!-- /Main -->\n";
	
	cout << "<html>" << endl;
	cout << "<head>" << fTitle << "</head>" << endl;
	cout << "<body>" << fTopBar << endl << fHeader << endl;
	cout << fMainContents << endl << fFooter << endl << "</body>" << endl;
	cout << "</html>" << endl;
	
}


#endif // #ifdef THTMLDoc_cxx
