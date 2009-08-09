<?php

if (!$_POST)
	$_POST = &$_GET;
reset($_POST);

if (isset($_POST["filename"]))
{
	$URL = "wisGetXMLContent.php?filename=" . $_POST["filename"];
}
else
{
	if (isset($_POST["runNumber"]))
		$runNumber = $_POST["runNumber"];
	
	if (isset($_POST["macroName"]))
		$macroName = $_POST["macroName"];
	
	if (isset($_POST["moduleName"]))
		$moduleName = $_POST["moduleName"];

	$URL = "wisGetXMLContent.php?runNumber=$runNumber&macroName=$macroName&moduleName=$moduleName";
}


?>
<html>

<head>

<title>title</title>
<link href="./css/standard.css" type="text/css" rel="stylesheet" />
<!--<script language="JavaScript" src="../../wisRunSelectionScript.js"></script>-->
	
</head>

<body bgcolor="#FFFFFF" text="#000000" link="#FF9966" vlink="#FF9966" alink="#FFCC99">
</body>

<script language="JavaScript" type="text/javascript">
var xmlDoc;


function createIFrame(URL, parentNode)
{
	var IFrameObj; // our IFrame object

	if (!document.createElement)
	{
		return true
	};
	var IFrameDoc;

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
			tempIFrame.style.width='100%';
			//tempIFrame.style.height='0px';
			//IFrameObj = document.body.appendChild(tempIFrame);
			IFrameObj = parentNode.appendChild(tempIFrame);

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
			iframeHTML+='width:100%;';
			//iframeHTML+='height:0px;';
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
		setTimeout('createIFrame(\'' + URL + '\', parentNode)', 10);
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


/****************** Load macro results in XML format *****************/
var xmlDoc;

if (document.implementation && document.implementation.createDocument)
{
	xmlDoc = document.implementation.createDocument("", "", null);
	xmlDoc.onload = showContentsInHTML;
}
else if (window.ActiveXObject)
{
	xmlDoc = new ActiveXObject("Microsoft.XMLDOM");
	xmlDoc.async = false;
	xmlDoc.onreadystatechange = function () {
		if (xmlDoc.readyState == 4) showContentsInHTML()
	};
}
else
{
	alert('Your browser can\'t handle this script');
	//return;
}

xmlDoc.load('<? echo $URL; ?>');
if (xmlDoc.parseError)
{
	if (xmlDoc.parseError.errorCode != 0)
	{
		var message;
		message  = "errorCode: " + xmlDoc.parseError.errorCode + "\n";
		message += "filepos: " + xmlDoc.parseError.filepos + "\n";
		message += "line: " + xmlDoc.parseError.line + "\n";
		message += "linepos: " + xmlDoc.parseError.linepos + "\n";
		message += "reason: " + xmlDoc.parseError.reason + "\n";
		message += "srcText: " + xmlDoc.parseError.srcText + "\n";
		message += "url: " + xmlDoc.parseError.url;
		alert(message);
	}
}
else if (xmlDoc.DOMException)
{
	//alert('AQUI!');
}
/**********************************************************************/

//setTimeout("showContentsInHTML()", 2*1000);

/********** Put XML contents in HTML format ********/
function showContentsInHTML()
{
	var i, b, text;
	var body = document.getElementsByTagName('BODY')[0];
	
	body.style.margin = '1em';

	//alert(xmlDoc);
	var aux = xmlDoc.getElementsByTagName('macroName');
	//alert(aux.length);
	var macroName = aux[0].firstChild.nodeValue;
	text = document.createTextNode('Macro Name: ' + macroName);
	body.appendChild(text);
	body.appendChild(document.createElement('BR'));

	var macroVersion = xmlDoc.getElementsByTagName('macroVersion');
	if (macroVersion.length > 0)
	{
		if (macroVersion[0].firstChild)
		{
			text = document.createTextNode('Macro Version: ' + macroVersion[0].firstChild.nodeValue);
			body.appendChild(text);
			body.appendChild(document.createElement('BR'));
		}
	}

	var errors = xmlDoc.getElementsByTagName('error');
	if (errors.length > 0)
		body.appendChild(document.createElement('BR'));

	for (i = 0; i < errors.length; i++)
	{
		var errorMsg = errors[i].firstChild.nodeValue;
		b = document.createElement('B');
		text = document.createTextNode('Error: ');
		b.appendChild(text);
		body.appendChild(b);
		text = document.createTextNode(errorMsg);
		if (errorMsg.match('Generating stack trace'))
		{
			var pre = document.createElement('PRE');
			pre.appendChild(text);
			body.appendChild(pre);
		}
		else
		{
			body.appendChild(text);
		}
		body.appendChild(document.createElement('BR'));
	}

	var params = xmlDoc.getElementsByTagName('parameter');
	for (i = 0; i < params.length; i++)
	{
		var paramTitle = params[i].getElementsByTagName('title')[0].firstChild.nodeValue;
		var paramValue = params[i].getElementsByTagName('value')[0].firstChild.nodeValue;
		b = document.createElement('B');
		text = document.createTextNode(paramTitle + ': ' + paramValue);
		b.appendChild(text);
		body.appendChild(b);
		body.appendChild(document.createElement('BR'));
	}

	var URLs = xmlDoc.getElementsByTagName('URL');
	if (URLs.length > 0)
		body.appendChild(document.createElement('BR'));

	for (i = 0; i < URLs.length; i++)
	{
		var URL = URLs[i].firstChild.nodeValue;
		createIFrame(URL, body);
		//body.appendChild(document.createElement('BR'));
	}

	var warnings = xmlDoc.getElementsByTagName('warning');
	if (warnings.length > 0)
		body.appendChild(document.createElement('BR'));

	for (i = 0; i < warnings.length; i++)
	{
		var warningMsg = warnings[i].firstChild.nodeValue;
		b = document.createElement('B');

		font = document.createElement('FONT');
		font.setAttribute('color', 'red');
		text = document.createTextNode('Warning: ');
		font.appendChild(text);

		b.appendChild(font);
		body.appendChild(b);
		text = document.createTextNode(warningMsg);
		body.appendChild(text);
		body.appendChild(document.createElement('BR'));
	}
	body.appendChild(document.createElement('BR'));

	var result = xmlDoc.getElementsByTagName('result');
	var ul = document.createElement('UL');
	body.appendChild(ul);
	for (i = 0; i < result.length; i++)
	{
		var type = result[i].getAttribute('type');
		var title = result[i].getElementsByTagName('title')[0].firstChild;

		var li = document.createElement('LI');

		if (type == 'link')
		{
			var href = result[i].getElementsByTagName('href')[0].firstChild.nodeValue;
			var a = document.createElement('A');
			a.setAttribute('href', href);
			a.setAttribute('target', '_blank');
			a.style.color = 'blue';
			a.appendChild(document.createTextNode(title.nodeValue));
			li.appendChild(a);
			//ul.insertBefore(document.createElement('BR'), ul.firstChild);
			//ul.insertBefore(li, ul.firstChild);
			ul.appendChild(li);
			ul.appendChild(document.createElement('BR'));
			continue;
		}
		
		ul.appendChild(li);
		ul.appendChild(document.createElement('BR'));

		b = document.createElement('B');

		if (type == 'table')
		{
			var data = result[i].getElementsByTagName('data')[0];

			if (title)
			{
				b.appendChild(document.createTextNode(title.nodeValue));
				li.appendChild(b);
			}
			li.appendChild(document.createElement('BR'));

			var htmlCode = data.firstChild.nodeValue;
			//alert(htmlCode);
			var table = document.createElement('TABLE');
			li.appendChild(table);
			
			var thead = document.createElement('THEAD');
			var tbody = document.createElement('TBODY');
			table.appendChild(thead); table.appendChild(tbody);

			if (window.ActiveXObject) // The browser is Internet Explorer
			{
				var xmlTable;
				/********** Load Table to an XML document ***********/
				if (document.implementation && document.implementation.createDocument)
				{
					xmlTable = document.implementation.createDocument("", "", null);
					//xmlTable.onload = showContentsInHTML;
				}
				else if (window.ActiveXObject)
				{
					xmlTable = new ActiveXObject("Microsoft.XMLDOM");
					/*xmlTable.onreadystatechange = function () {
						if (xmlTable.readyState == 4) showContentsInHTML()
					};*/
				}
				else
				{
					alert('Your browser can\'t handle this script');
					//return;
				}
				
				xmlTable.loadXML(htmlCode);
				/**************************************************/
				
				table.className = xmlTable.getElementsByTagName('table')[0].getAttribute('class');
				var tr = xmlTable.getElementsByTagName('tr');
				
				var j;
				for (j = 0; j < tr.length; j++)
				{
					var tableTR = document.createElement('TR');
					//table.appendChild(tableTR);
					var th = tr[j].getElementsByTagName('th');
					//alert(th.length);
					var k;
					for (k = 0; k < th.length; k++)
					{
						var tableTH = document.createElement('TH');
						var text;
						if (th[k].firstChild)
							text = document.createTextNode(th[k].firstChild.nodeValue);
						else
							text = document.createTextNode('');
						//alert(text);
						tableTH.appendChild(text);
						if (th[k].getAttribute('colspan'))
							tableTH.colSpan = th[k].getAttribute('colspan');
						tableTR.appendChild(tableTH);
					}

					var td = tr[j].getElementsByTagName('td');
					//alert(td.length);
					var k;
					for (k = 0; k < td.length; k++)
					{
						var tableTD = document.createElement('TD');
						var text;
						if (td[k].firstChild)
							text = document.createTextNode(td[k].firstChild.nodeValue);
						else
							text = document.createTextNode('');
						//alert(text);
						tableTD.appendChild(text);
						if (td[k].getAttribute('colspan'))
							tableTd.colSpan = td[k].getAttribute('colspan');
						tableTR.appendChild(tableTD);
					}
					
					if (th.length > 0)
						thead.appendChild(tableTR);
					else
						tbody.appendChild(tableTR);

				}
								
			}
			else
			{
				table.innerHTML = htmlCode;
			}
	
		}
		else
		if (type == 'value')
		{
			var data;
			if (result[i].getElementsByTagName('data')[0].firstChild)
				data = result[i].getElementsByTagName('data')[0].firstChild.nodeValue;
			else
				data = '';
			var str = '';
			if (title)
				str += title.nodeValue + ': ';
			str += data;
			b.appendChild(document.createTextNode(str));
			li.appendChild(b);
		}
		else
		if (type == 'plot')
		{
			var filename = result[i].getElementsByTagName('fileName')[0].firstChild.nodeValue;
			var img = document.createElement('IMG'); img.setAttribute('src', filename);
			if (title)
			{
				b.appendChild(document.createTextNode(title.nodeValue));
				li.appendChild(b);
			}
			li.appendChild(document.createElement('BR'));
			li.appendChild(img);
		}
	}
	
	return;
}

//showContentsInHTML();

</script>

</html>
