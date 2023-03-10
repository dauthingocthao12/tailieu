function BrowserCheck(){

	this.NavName = navigator.appName;
	this.NavAnt = navigator.userAgent;
	this.NavVer = navigator.appVersion;
	this.NavPlug = navigator.plugins;
	this.NavVsub = navigator.vendorSub;
	this.NavVerI = parseInt(this.NavVer);
	this.NavVerF = parseFloat(this.NavVer);
	
	this.NN = (this.NavName == "Netscape")
	this.NN4 = (this.NavAnt.indexOf("Mozilla/4") != -1);
	this.NN6 = (this.NavAnt.indexOf("Netscape6/") != -1);
	this.NN7 = (this.NavAnt.indexOf("Netscape/7") != -1);
		
	this.IE = (this.NavName == "Microsoft Internet Explorer");
	this.IE3 = (this.NavAnt.indexOf('MSIE 3')>0);
	this.IE45 = (this.NavVer.indexOf('MSIE 4.5')>0);
	this.IE401 = (this.NavVer.indexOf('MSIE 4.01')>0);
	this.IE4 = (this.NavVer.indexOf('MSIE 4')>0);
	this.IE51 = (this.NavAnt.indexOf('MSIE 5.1')>0);
	this.IE52 = (this.NavAnt.indexOf('MSIE 5.2')>0);
	this.IE5 = (this.NavVer.indexOf('MSIE 5')>0);		
	this.IE6 = (this.NavVer.indexOf('MSIE 6')>0);

	this.GEK = (this.NavAnt.indexOf("Gecko") != -1);
	this.SAF = (this.NavAnt.indexOf("Safari",0) != -1);
	this.CAB = (this.NavAnt.indexOf("iCab",0) != -1);
	this.OPE = (this.NavAnt.indexOf("Opera",0) != -1);

	this.Win = (this.NavAnt.indexOf("Win",0) != -1);
	this.XP = (this.NavAnt.match(/NT 5\.1|XP/));
	this.ME = (this.NavAnt.match(/4\.90|ME/));
	this.TK = (this.NavAnt.match(/NT 5\.0|2000/));
	this.NT = (this.NavAnt.match(/NT 5\.0|WinNT/));

	this.Mac = (this.NavAnt.indexOf("Mac",0) != -1);
	this.M68k = (this.NavAnt.indexOf("68k",0)!=-1);

	this.Uix = (this.NavAnt.indexOf("X11",0) != -1);
	this.EGB = (this.NavAnt.indexOf("Planetweb",0) != -1);

}

var checkB = new BrowserCheck();

var flash_version = 5;
var FlashInstalled = false;

if(checkB.OPE && navigator.plugins["Shockwave Flash"]){
	var sp = navigator.plugins["Shockwave Flash"].description.indexOf("Flash");
	var ep = navigator.plugins["Shockwave Flash"].description.lastIndexOf(" ");
	var aver = parseFloat(navigator.plugins["Shockwave Flash"].description.substring(sp+6,ep));
	if(aver >= flash_version){
		var FlashInstalled = true;
	}
}
else if(checkB.Win && checkB.IE){
	document.write('<SCR' + 'IPT LANGUAGE=VBScript\> \n');
	document.write('on error resume next \n');
	document.write('contentVersion = 5 \n');
	document.write('FlashInstalled = ( IsObject(CreateObject("ShockwaveFlash.ShockwaveFlash." & contentVersion))) \n');
	document.write('</SCR' + 'IPT\> \n');
	if(checkB.IE4){
		var FlashInstalled = false;
	}
}
else if(checkB.Mac && checkB.IE){
	if(!checkB.IE4 && navigator.plugins["Shockwave Flash"]){
		var sp = navigator.plugins["Shockwave Flash"].description.indexOf("Flash");
		var ep = navigator.plugins["Shockwave Flash"].description.lastIndexOf(" ");
		var aver = parseFloat(navigator.plugins["Shockwave Flash"].description.substring(sp+6,ep));
		if(aver >= flash_version){
			var FlashInstalled = true;
		}
	}
}
else if(checkB.NN && navigator.plugins["Shockwave Flash"]){
	var sp = navigator.plugins["Shockwave Flash"].description.indexOf("Flash");
	var ep = navigator.plugins["Shockwave Flash"].description.lastIndexOf(" ");
	var aver = parseFloat(navigator.plugins["Shockwave Flash"].description.substring(sp+6,ep));
	if(aver >= flash_version){
		var FlashInstalled = true;
	}
}

function top_swf(){
	if(FlashInstalled){
		document.write('<OBJECT classid=\"clsid:D27CDB6E-AE6D-11cf-96B8-444553540000\" codebase=\"http:\/\/active.macromedia.com\/flash2\/cabs\/swflash.cab#version=4,0,0,0\" id=\"top\" width=\"190\" height=\"130\"><PARAM name=\"movie\" value=\"\/images\/top.swf\"><PARAM name=\"quality\" value=\"high\"><PARAM name=\"bgcolor\" value=\"#000000\"><EMBED src=\"\/images\/top.swf\" quality=\"high\" bgcolor=\"#000000\" width=\"190\" height=\"130\" type=\"application\/x-shockwave-flash\" PLUGINSPAGE=\"http:\/\/www.macromedia.com\/shockwave\/download\/index.cgi?P1_Prod_Version=ShockwaveFlash\"></OBJECT>');
	}
	else{
		document.write('<FONT color=\"#ffffff\">このサイトの内容を完全な状態でご覧いただくには下記の案内よりFlashプレーヤーの最新版（無料）をダウンロードしてください。</FONT><BR><BR>');
	}
}

function flashplayer(){
	if(FlashInstalled){
	}
	else{
		document.write('<a href=\"http:\/\/www.macromedia.com\/shockwave\/download\/download.cgi?P1_Prod_Version=ShockwaveFlash&Lang=Japanese&P5_Language=Japanese\" target=\"blank\"><FONT color=\"#ffffff\">この案内をクリックするとmacromedia FLASH PLAYER最新版（無料）のダウンロードページへ移動します。</FONT><\/a><BR>');
	}
}
