#define TWISMacroResult_cxx

#include "TWISMacroResult.h"
#include <TError.h>

#ifdef TWISMacroResult_cxx

#define NUM_OF_ERRORS_THRESHOLD 100

vector<TString> handledErrors;
Int_t numOfHandledErrors;
TString _MacroName_, _MacroVersion_;

void MyErrorHandler(int level, Bool_t abort, const char* location, const char* msg)
{
	//cout << "IT WORKS!" << endl;
	TString type;
	if (level >= kInfo && level < kWarning)
		return; //type = "Info";
	if (level >= kWarning)
		type = "Warning";
	if (level >= kError)
		type = "Error";
	if (level >= kBreak)
		type = "*** Break ***";
	if (level >= kSysError)
		type = "SysError";
	if (level >= kFatal)
		type = "Fatal";

	TString errorMsg;
	if (!location || strlen(location) == 0)
		errorMsg = "ROOT " + type + ": " + msg;
	else
	if (level >= kBreak && level < kSysError)
		errorMsg = "ROOT " + type + " " + msg;
	else
		errorMsg = "ROOT " + type + " in " + location + ": " + msg;
		//errorMsg = "<![CDATA[ROOT " + type + " in <" + location + ">: " + msg + "]]>";

	//cout << errorMsg << endl;
	//cout << "ROOT " << type << " in " << location << ": " << msg << endl;
	handledErrors.push_back(errorMsg);//cout << "ops!" << endl;
	numOfHandledErrors++;

	if ((level >= kBreak /*&& level < kSysError*/) || abort || numOfHandledErrors > NUM_OF_ERRORS_THRESHOLD)
	{
		cout << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>" << endl;
		//cout << "<!DOCTYPE xml [<!ENTITY nbsp \"&#160;\">]>" << endl; // causes an error in IE
		cout << "<?xml-stylesheet type=\"text/css\" href=\"blank.css\" ?>" << endl;
		cout << "<analysis>" << endl;
		cout << "	<macroName>" << " " << "</macroName>" << endl;
		//cout << "	<macroVersion>" << "_MacroVersion_" << "</macroVersion>" << endl;
		Int_t i;
		for (i = 0; i < numOfHandledErrors; i++)
		{
			cout << "       <error>" << handledErrors.at(i) << "</error>" << endl;
		}
		if (gSystem && (abort || level < kSysError))
		{
			cout << "       <error><![CDATA[" << endl; fflush(stdout);
			gSystem->StackTrace(); fflush(stderr); fflush(stdout);
			cout << "]]></error>" << endl;
		}
		cout << "</analysis>" << endl << endl;

		if (gSystem) gSystem->Abort();
		else ::abort();
	}

	//cout << "Handled errors: " << numOfHandledErrors << endl;
	//cout << errorMsg << endl;
}


TWISResult::TWISResult(TString title)
{
	mTitle = title;
}

void TWISResult::setTag(TString tag)
{
	mTag = tag;
}

TString TWISResult::getTag()
{
	return(mTag);
}

TString TWISResult::getTitle()
{
	return(mTitle);
}


TWISPlot::TWISPlot(TString title, TString fileName) : TWISResult::TWISResult(title)
{
	mFileName = fileName;
	setTag("plot");
}

TString TWISPlot::getFileName()
{
	return(mFileName);
}


TWISTable::TWISTable(TString title, TString data) : TWISResult::TWISResult(title)
{
	mData = data;
	setTag("table");
}

TString TWISTable::getTable()
{
	return(mData);
}


TWISValue::TWISValue(TString title, TString data) : TWISResult::TWISResult(title)
{
	mData = data;
	setTag("value");
}
TWISValue::TWISValue(TString title, Int_t data) : TWISResult::TWISResult(title)
{
	mData = ""; mData += data;
	setTag("value");
}
TWISValue::TWISValue(TString title, Float_t data) : TWISResult::TWISResult(title)
{
	mData = ""; mData += data;
	setTag("value");
}
TWISValue::TWISValue(TString title, Double_t data) : TWISResult::TWISResult(title)
{
	mData = ""; mData += data;
	setTag("value");
}
TWISValue::TWISValue(TString title, size_t data) : TWISResult::TWISResult(title)
{
	mData = ""; mData += data;
	setTag("value");
}
TWISValue::TWISValue(TString title, Long64_t data) : TWISResult::TWISResult(title)
{
	mData = ""; mData += data;
	mData = data;
	setTag("value");
}

TString TWISValue::getValue()
{
	return(mData);
}


TWISLink::TWISLink(TString title, TString href) : TWISResult::TWISResult(title)
{
	mHref = href;
	setTag("link");
}
TString TWISLink::getHref()
{
	return(mHref);
}


TWISMacroResult::TWISMacroResult()
{
	numOfHandledErrors = 0;
	handledErrors.clear();
	SetErrorHandler(MyErrorHandler);
	mNumOfResults = 0;
	mNumOfParams = 0;
	mNumOfErrors = 0;
	mNumOfWarnings = 0;
	mNumOfURLs = 0;
	mMacroName = ""; //_MacroName_ = "";
	mMacroVersion = ""; //_MacroVersion_ = "";
}


void TWISMacroResult::setMacroName(TString macroName)
{
	mMacroName = macroName;
	//_MacroName_ = mMacroName;
}


void TWISMacroResult::setMacroVersion(TString macroVersion)
{
	mMacroVersion = macroVersion;
	//_MacroVersion_ = mMacroVersion;
}


void TWISMacroResult::setMacroNameVersion(TString macroName, TString macroVersion)
{
	mMacroName = macroName;
	mMacroVersion = macroVersion;

}


void TWISMacroResult::setParameter(TString title, TString value)
{
	mParamsTitles.push_back(title);
	mParamsValues.push_back(value);
	mNumOfParams++;

}
void TWISMacroResult::setParameter(TString title, Int_t value)
{
	mParamsTitles.push_back(title);
	TString aux = ""; aux += value;
	mParamsValues.push_back(aux);
	mNumOfParams++;
}
void TWISMacroResult::setParameter(TString title, Float_t value)
{
	mParamsTitles.push_back(title);
	TString aux = ""; aux += value;
	mParamsValues.push_back(aux);
	mNumOfParams++;
}
void TWISMacroResult::setParameter(TString title, Double_t value)
{
	mParamsTitles.push_back(title);
	TString aux = ""; aux += value;
	mParamsValues.push_back(aux);
	mNumOfParams++;
}
void TWISMacroResult::setParameter(TString title, size_t value)
{
	mParamsTitles.push_back(title);
	TString aux = ""; aux += value;
	mParamsValues.push_back(aux);
	mNumOfParams++;
}
void TWISMacroResult::setParameter(TString title, Long64_t value)
{
	mParamsTitles.push_back(title);
	TString aux = ""; aux += value;
	mParamsValues.push_back(aux);
	mNumOfParams++;
}


void TWISMacroResult::addPlot(TString title, TString fileName)
{
	TWISPlot *aux = new TWISPlot(title, fileName);
	mResults.push_back(aux);
	mNumOfResults++;
}


void TWISMacroResult::addTable(TString title, TString table)
{
	TWISTable *aux = new TWISTable(title, table);
	mResults.push_back(aux);
	mNumOfResults++;
}


void TWISMacroResult::addValue(TString title, TString value)
{
	TWISValue *aux = new TWISValue(title, value);
	mResults.push_back(aux);
	mNumOfResults++;
}
void TWISMacroResult::addValue(TString title, Int_t value)
{
	TWISValue *aux = new TWISValue(title, value);
	mResults.push_back(aux);
	mNumOfResults++;
}
void TWISMacroResult::addValue(TString title, Float_t value)
{
	TWISValue *aux = new TWISValue(title, value);
	mResults.push_back(aux);
	mNumOfResults++;
}
void TWISMacroResult::addValue(TString title, Double_t value)
{
	TWISValue *aux = new TWISValue(title, value);
	mResults.push_back(aux);
	mNumOfResults++;
}
void TWISMacroResult::addValue(TString title, size_t value)
{
	TWISValue *aux = new TWISValue(title, value);
	mResults.push_back(aux);
	mNumOfResults++;
}
void TWISMacroResult::addValue(TString title, Long64_t value)
{
	TWISValue *aux = new TWISValue(title, value);
	mResults.push_back(aux);
	mNumOfResults++;
}


void TWISMacroResult::addLink(TString title, TString href)
{
	TWISLink *aux = new TWISLink(title, href);
	mResults.push_back(aux);
	mNumOfResults++;
}


void TWISMacroResult::addURLToPage(TString URL)
{
	mURLs.push_back(URL);
	mNumOfURLs++;
}


void TWISMacroResult::addError(TString errorMsg)
{
	mErrorMsgs.push_back(errorMsg);
	mNumOfErrors++;
}


void TWISMacroResult::addWarning(TString warningMsg)
{
	mWarnings.push_back(warningMsg);
	mNumOfWarnings++;
}


void TWISMacroResult::out()
{
	Int_t i;

	//cout << "Handled errors: " << numOfHandledErrors << endl;
	//cout << "Handled errors: " << handledErrors.size() << endl;
	for (i = 0; i < numOfHandledErrors; i++)
	{
		addError(handledErrors.at(i));
		//cout << handledErrors.at(i) << endl;
	}

	cout << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>" << endl;
	//cout << "<!DOCTYPE analysis [<!ENTITY nbsp \"&#160;\">]>" << endl; // causes an error in IE
	cout << "<?xml-stylesheet type=\"text/css\" href=\"blank.css\" ?>" << endl;

	cout << "<analysis>" << endl;
	
	cout << "	<macroName>" << mMacroName << "</macroName>" << endl;
	if (mMacroVersion != "")
		cout << "	<macroVersion>" << mMacroVersion << "</macroVersion>" << endl;

	for (i = 0; i < mNumOfErrors; i++)
	{
		cout << "	<error>" << mErrorMsgs.at(i) << "</error>" << endl;
	}

	for (i = 0; i < mNumOfWarnings; i++)
	{
		cout << "	<warning>" << mWarnings.at(i) << "</warning>" << endl;
	}

	for (i = 0; i < mNumOfURLs; i++)
	{
		cout << "	<URL><![CDATA[" << mURLs.at(i) << "]]></URL>" << endl;
	}

	cout << "	<parameters>" << endl;
	for (i = 0; i < mNumOfParams; i++)
	{
		cout << "		<parameter>" << endl;
		cout << "			<title>" << mParamsTitles.at(i) << "</title>" << endl;
		cout << "			<value>" << mParamsValues.at(i) << "</value>" << endl;
		cout << "		</parameter>" << endl;
	}
	cout << "	</parameters>" << endl;

	cout << "	<results>" << endl;
	cout << "		<title></title>" << endl;
	for (i = 0; i < mNumOfResults; i++)
	{
		cout << "		<result type=\"" << mResults[i]->getTag() << "\">" << endl;
		cout << "			<title>" << mResults.at(i)->getTitle() << "</title>" << endl;
		if (mResults.at(i)->getTag() == "plot")
		{
			cout << "			<fileName>" << ((TWISPlot*)mResults.at(i))->getFileName() << "</fileName>" << endl;
		}
		else
		if (mResults.at(i)->getTag() == "table")
		{
			cout << "			<data><![CDATA[" << ((TWISTable*)mResults.at(i))->getTable() << "]]></data>" << endl;
			//cout << "			<data>" << ((TWISTable*)mResults.at(i))->getTable() << "</data>" << endl;
		}
		else
		if (mResults.at(i)->getTag() == "value")
		{
			cout << "			<data>" << ((TWISValue*)mResults.at(i))->getValue() << "</data>" << endl;
		}
		else
		if (mResults.at(i)->getTag() == "link")
		{
			cout << "			<href><![CDATA[" << ((TWISLink*)mResults.at(i))->getHref() << "]]></href>" << endl;
		}

		cout << "		</result>" << endl;
	}
	cout << "	</results>" << endl;
	cout << "</analysis>" << endl << endl;

}


TWISMacroResult::~TWISMacroResult()
{

}


#endif // #ifdef TWISMacroResult_cxx

