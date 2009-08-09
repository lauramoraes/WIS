
var run = new Array();
var row = new Array();

var action = new Array();
var actionId, runNumber, runType;

var resultFile = new Array();
var cell = new Array();

var xmlDoc;

var maxNumberOfProcesses = 3;
var processesRunning = 0;

/************************* Load macros.xml **********************************/
	if (document.implementation && document.implementation.createDocument)
	{
		xmlDoc = document.implementation.createDocument("", "", null);
		//xmlDoc.onload = createActions;
	}
	else if (window.ActiveXObject)
	{
		xmlDoc = new ActiveXObject("Microsoft.XMLDOM");
		/*xmlDoc.onreadystatechange = function () {
			if (xmlDoc.readyState == 4) createActions()
		};*/
 	}
	else
	{
		alert('Your browser can\'t handle this script');
		//return;
	}
	
	xmlDoc.async = false;
	xmlDoc.load("macros.xml");
/****************************************************************************/


function writeMacroTitles()
{
	var i;
	var th;
	
	var tr = document.getElementById('MacroTitles');

	var macroTitle = xmlDoc.getElementsByTagName('title');
	var theTitle;

	for (i = 0; i < macroTitle.length; i++)
	{
		th = document.createElement('TH');
		theTitle = document.createTextNode(macroTitle[i].firstChild.nodeValue);
		th.setAttribute('noWrap', 'true');
		th.appendChild(theTitle);
		tr.appendChild(th);
	}
	
}


function writeRow(rowNumber)
{
//alert('Aqui!');
	var option, title, i, runTypes, runType, td;
	var select = document.getElementById('Row' + rowNumber + 'Select');
	//select.setAttribute('width', '15em');
	select.style.width = '9em';

	var macro = xmlDoc.getElementsByTagName('macro');
	var macroTitle = xmlDoc.getElementsByTagName('title');
	var macroName = xmlDoc.getElementsByTagName('name');
	
	var tr = document.getElementById('Row' + rowNumber);

	for (i = 0; i < macro.length; i++)
	{
		var tdId = 'Row' + rowNumber + macroName[i].firstChild.nodeValue;
		td = document.createElement('TD');
		td.setAttribute('id', tdId);
		td.setAttribute('name', tdId);
		td.setAttribute('noWrap', 'true');
		tr.appendChild(td);

		cell[tdId] = new Array;
		cell[tdId]['macroTitle'] = macroTitle[i].firstChild.nodeValue;
		cell[tdId]['macroName'] = macroName[i].firstChild.nodeValue;
		
		runTypes = macro[i].getElementsByTagName('runType');
		runType = row[rowNumber]['runType'];
		if (!found(runType, runTypes))
			continue;
		
		option = document.createElement('option');
		option.appendChild(document.createTextNode(macroTitle[i].firstChild.nodeValue));
		option.setAttribute('value', macroName[i].firstChild.nodeValue);
		select.appendChild(option);
		
		//td.appendChild(document.createTextNode(macroTitle[i].firstChild.nodeValue));
		
		var param = macro[i].getElementsByTagName('param');
		var parameters = '';
		for (j = 0; j < param.length; j++)
		{
			if (param[j].getAttribute('provided') != 'system') continue;
			var paramName = param[j].firstChild.nodeValue; //alert(actionId + paramName);
			var paramValue = row[rowNumber][paramName];
			if (parameters != '')
				parameters += ';';
			parameters += paramName + ':' + paramValue;
		}
		
		row[rowNumber]['parameters'] = parameters;
		
		if (row[rowNumber][macroName[i].firstChild.nodeValue])
		{
			resultFile[tdId] = 'wisSeeMacroResult.php?runNumber=' + row[rowNumber]['runNumber'] + '&macroName=' + macroName[i].firstChild.nodeValue;
			link = document.createElement('A');
			link.setAttribute('href', '#');
			link.onclick = parent.showMacroResult;
			link.onmouseover = parent.showStatusBarResultMsg;
			link.onmouseout = parent.eraseStatusBarMsg;
		
			if (link.captureEvents) link.captureEvents(Event.CLICK); // for Netscape 4(?)
			var text = parent.document.createTextNode(macroTitle[i].firstChild.nodeValue + ' Results');
			link.appendChild(text);
			//link.style.color = '#CC0000';
			td.appendChild(link);
			td.appendChild(document.createTextNode(' '));

			if (row[rowNumber][macroName[i].firstChild.nodeValue] == 'all')
			{
				resultFile[tdId] = resultFile[tdId] + '&moduleName=all';
			}
			else
			{
				var moduleSelect = document.createElement('SELECT');
				moduleSelect.setAttribute('id', 'Row' + rowNumber + macroName[i].firstChild.nodeValue + 'Select');
				td.appendChild(moduleSelect);
				var aux = row[rowNumber][macroName[i].firstChild.nodeValue];
				var moduleNames = new Array();
				moduleNames = aux.split(' ');
				//alert(moduleNames.length);
				
				for (var j = 0; j < moduleNames.length; j++)
				{
					var moduleOption = document.createElement('option');
					moduleOption.appendChild(document.createTextNode(moduleNames[j]));
					moduleOption.setAttribute('value', moduleNames[j]);
					moduleSelect.appendChild(moduleOption);
				}
				
			}
		}

	}

	if (row[rowNumber]['existNtuple'] == 0)
	{
		select.disabled = true;
		var runButton = document.getElementById('Row' + rowNumber + 'Submit');
		runButton.disabled = true;
	}
	
	//URL = 'wisHasResult.php?runNumber=' + row[rowNumber]['runNumber'] + '&rowNumber=' + rowNumber;// + '&tdId=' + tdId;
	//callToServer(URL);

}


function found(runType, runTypes)
{
	var i, achou;
	achou = false;
	for (i = 0; i < runTypes.length; i++)
	{
		if (runTypes[i].firstChild.nodeValue == runType)
			achou = true;
	}
	return achou;
}


function runMacro(rowNumber)
{
	if (processesRunning >= maxNumberOfProcesses)
	{
		alert('You cannot run more than ' + maxNumberOfProcesses + ' processes at the same time.');
		return(false);
	}
	var URL;

	var select = document.getElementById('Row' + rowNumber + 'Select');
	var macroName = select.options[select.selectedIndex].value;
	var macroTitle = select.options[select.selectedIndex].text;
	//alert(macroTitle);
	var parameters = row[rowNumber]['parameters'];
	
	var name = xmlDoc.getElementsByTagName('name')
	for (var i = 0; i < name.length; i++)
	{
		if (name[i].firstChild.nodeValue == macroName)
			break;
	}

	var params = xmlDoc.getElementsByTagName('params')[i];
	for (j = 0; j < params.childNodes.length; j++)
	{
		if (params.childNodes[j].nodeType != 1) continue;
		if (params.childNodes[j].getAttribute('provided') != 'user') continue;
		var paramName = params.childNodes[j].firstChild.nodeValue; //alert(actionId + paramName);
		var message = params.childNodes[j].getAttribute('prompt');
		var defaultValue = '';
		var paramValue;
		if (paramName == 'module')
		{
			var aux = new Array();
			aux = row[rowNumber]['modules'].split(' ');
			if (aux.length > 1)
				message = message + '\n Type one of: ' + row[rowNumber]['modules'];
			else
				defaultValue = aux[0];
		}
		else
		if (params.childNodes[j].getAttribute('defaultValue'))
		{
			defaultValue = params.childNodes[j].getAttribute('defaultValue');
		}

		paramValue = window.prompt(message, defaultValue);		
		if (paramValue == '')
		{
			alert('You must type a value for this parameter.');
			return false;
		}
		if (!paramValue)
			return false;

		if (parameters != '')
			parameters += ';';
			
		if (paramName == 'module')
		{
			paramValue = paramValue.toUpperCase();
			//alert(paramValue);
		}
		
		parameters += paramName + ':' + paramValue;
	}

	tdId = 'Row' + rowNumber + macroName;
	URL = 'wisExecuteMacro.php?name=' + macroName + '&parameters=' + parameters;
	URL += '&tdId=' + tdId;
	//alert(URL);

	var td = document.getElementById(tdId);
	td.setAttribute('align', 'left');
	var b = document.createElement('B');
	var image = document.createElement('IMG');
	image.src = "images/hourglass_icon.gif";
	image.width = 25;
	image.height= 25;
	b.appendChild(document.createTextNode('Running ' + cell[tdId]['macroTitle'] + ' '));
	b.appendChild(image);
	var link = td.getElementsByTagName('A')[0];
	if (link)
	{
		td.insertBefore(b, link);
		var br = document.createElement('BR');
		td.insertBefore(br, link);
	}
	else
	{
		td.appendChild(b);
	}
//	window.location=URL;
	callToServer(URL);
	processesRunning = processesRunning + 1;
//
	return false;
}


function insertComment(runNumber, moduleName, moduleOldComment)
{
	var URL;
	var comment;
	if (moduleOldComment == "")
		comment = window.prompt('Type a summary comment about the performance \nof module ' + moduleName + ' in run ' + runNumber + ':');
	else
		comment = window.prompt('Type a summary comment about the performance \nof module ' + moduleName + ' in run ' + runNumber + ':', moduleOldComment);
	
	if (comment == '')
	{
		alert('You should type a comment.');
		return false;
	}
	if (!comment)
		return false;
	
	URL = 'wisAddSummaryComment.php?module=' + moduleName + '&run=' + runNumber + '&comment=' + comment;
	//alert(URL);
	callToServer(URL);
	return false;
}


function showStatusBarMsg()
{
	window.status = "";
	var id = this.parentNode.getAttribute('id');

	window.status = cell[id]['macroTitle'];
	return true;
}


function showStatusBarResultMsg()
{
	window.status = "";
	var id = this.parentNode.getAttribute('id');
	//alert(cell[id]['macroTitle']);
	window.status = "See results for last execution of macro " + cell[id]['macroTitle'];
	return true;
}


function eraseStatusBarMsg()
{
	window.status = "";
}


function showMacroResult()
{
	var id = this.parentNode.getAttribute('id');

	var winName = id + 'result';
	var winURL = resultFile[id];
	var select = document.getElementById(id + 'Select');
	if (select)
	{
		winURL = winURL + '&moduleName=' + select.options[select.selectedIndex].value;
	}
	window.open(winURL, winName);

	return false;
}


function callToServer(URL)
{
	var IFrameObj; // our IFrame object

	if (!document.createElement)
	{
		return true
	};
	var IFrameDoc;
	//var URL = 'macros.xml';
	//if (theFormName)
		//URL += buildQueryString(theFormName);

	if (!IFrameObj && document.createElement)
	{
		// create the IFrame and assign a reference to the
		// object to our global variable IFrameObj.
		// this will only happen the first time 
		// callToServer() is called
		try
		{
			var tempIFrame=document.createElement('iframe');
			tempIFrame.setAttribute('id','RSIFrame');
			tempIFrame.style.border='0px';
			tempIFrame.style.width='0px';
			tempIFrame.style.height='0px';
			IFrameObj = document.body.appendChild(tempIFrame);

			if (document.frames)
			{
				// this is for IE5 Mac, because it will only
				// allow access to the document object
				// of the IFrame if we access it through
				// the document.frames array
				IFrameObj = document.frames['RSIFrame'];
			}
		}
		catch(exception)
		{
			// This is for IE5 PC, which does not allow dynamic creation
			// and manipulation of an iframe object. Instead, we'll fake
			// it up by creating our own objects.
			iframeHTML='\<iframe id="RSIFrame" style="';
			iframeHTML+='border:0px;';
			iframeHTML+='width:0px;';
			iframeHTML+='height:0px;';
			iframeHTML+='"><\/iframe>';
			document.body.innerHTML+=iframeHTML;
			IFrameObj = new Object();
			IFrameObj.document = new Object();
			IFrameObj.document.location = new Object();
			IFrameObj.document.location.iframe = document.getElementById('RSIFrame');
			IFrameObj.document.location.replace = function(location)
			{
				this.iframe.src = location;
			}
		}
	}
  
	if (navigator.userAgent.indexOf('Gecko') !=-1 && !IFrameObj.contentDocument)
	{
		// we have to give NS6 a fraction of a second
		// to recognize the new IFrame
		setTimeout('callToServer()',10);
		return false;
	}
  
	if (IFrameObj.contentDocument)
	{
		// For NS6
		IFrameDoc = IFrameObj.contentDocument;
	}
	else if (IFrameObj.contentWindow)
	{
		// For IE5.5 and IE6
		IFrameDoc = IFrameObj.contentWindow.document;
	}
	else if (IFrameObj.document)
	{
		// For IE5
		IFrameDoc = IFrameObj.document;
	}
	else
	{
		return true;
	}

	IFrameDoc.location.replace(URL);
	return false;
}


function buildQueryString(theFormName)
{
	theForm = document.forms[theFormName];
	var qs = '';
	for (e = 0; e < theForm.elements.length; e++)
	{
		if (theForm.elements[e].name != '')
		{
			qs+=(qs=='')?'?':'&';
			qs+=theForm.elements[e].name+'='+escape(theForm.elements[e].value);
		}
	}
	return qs;
}


// Aditional 'CSS Enhancing' functions.

function matchWidth(elementId, referenceId)
{
    element = document.getElementById(elementId);
    reference = document.getElementById(referenceId);
  /* if (element.offsetWidth != reference.offsetWidth)
    {
		element.style.width = reference.offsetWidth + 'px';
    }
*/
	if(element.getAttribute("width") != reference.getAttribute("width"))
	{
		var size = reference.getAttribute("width");
		element.setAttribute("width", size);
	}/**/
}

