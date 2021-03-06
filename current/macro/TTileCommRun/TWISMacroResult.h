#ifndef TWISMacroResult_h
#define TWISMacroResult_h

#include <TROOT.h>
#include <TSystem.h>
#include <iostream>

using namespace std;

void MyErrorHandler(int level, Bool_t abt, const char* location, const char* msg);


class TWISResult
{
private:
	TString mTitle;
	TString mTag;

public:
	TWISResult(TString title);
	void setTag(TString tag);
	TString getTag();
	TString getTitle();
};

class TWISPlot : public TWISResult
{
private:
	TString mFileName;
public:
	TWISPlot(TString title, TString fileName);
	virtual TString getFileName();
};

class TWISTable : public TWISResult
{
private:
	TString mData;
public:
	TWISTable(TString title, TString data);
	virtual TString getTable();
};

class TWISValue : public TWISResult
{
private:
	TString mData;
public:
	TWISValue(TString title, TString data);
	TWISValue(TString title, Int_t data);
	TWISValue(TString title, Float_t data);
	TWISValue(TString title, Double_t data);
	TWISValue(TString title, size_t data);
	TWISValue(TString title, Long64_t data);
	virtual TString getValue();
};


class TWISLink : public TWISResult
{
private:
	TString mHref;
public:
	TWISLink(TString title, TString href);
	virtual TString getHref();
};


class TWISMacroResult
{
private:
	TString mMacroName, mMacroVersion;

	vector<TString> mParamsTitles, mParamsValues;
	Int_t mNumOfParams;

	vector<TString> mErrorMsgs;
	Int_t mNumOfErrors;

	vector<TString> mWarnings;
	Int_t mNumOfWarnings;

	vector<TWISResult*> mResults;
	Int_t mNumOfResults;

	vector<TString> mURLs;
	Int_t mNumOfURLs;

public:
	TWISMacroResult();

	void setMacroName(TString macroName);
	void setMacroVersion(TString macroVersion);
	void setMacroNameVersion(TString macroName, TString macroVersion);
	
	void setParameter(TString title, TString value);
	void setParameter(TString title, Int_t value);
	void setParameter(TString title, Float_t value);
	void setParameter(TString title, Double_t value);
	void setParameter(TString title, size_t value);
	void setParameter(TString title, Long64_t value);

	void addPlot(TString title, TString fileName);

	void addTable(TString title, TString table);

	void addValue(TString title, TString value);
	void addValue(TString title, Int_t value);
	void addValue(TString title, Float_t value);
	void addValue(TString title, Double_t value);
	void addValue(TString title, size_t value);
	void addValue(TString title, Long64_t value);
	
	void addLink(TString title, TString href);

	void addURLToPage(TString URL);

	void addError(TString errorMsg);

	void addWarning(TString warningMsg);

	void out();

	~TWISMacroResult();
};

#endif

