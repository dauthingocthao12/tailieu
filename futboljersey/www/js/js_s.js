function browserCheck(){ 
	this.ver=navigator.appVersion
	this.agent=navigator.userAgent
	this.dom=document.getElementById?1:0
	this.opera6=((this.agent.indexOf("Opera 6")>-1) && this.dom)?1:0;
	this.ie5=((this.ver.indexOf("MSIE 5")>-1) && this.dom && !this.opera6)?1:0; 
	this.ie6r=((this.ver.indexOf("MSIE 6")>-1) && this.dom && (document.compatMode == "BackCompat"))?1:0;
	this.ie6s=((this.ver.indexOf("MSIE 6")>-1) && this.dom && (document.compatMode == "CSS1Compat"))?1:0;
	this.ie7r=((this.ver.indexOf("MSIE 7")>-1) && this.dom && (document.compatMode == "BackCompat"))?1:0;
	this.ie7s=((this.ver.indexOf("MSIE 7")>-1) && this.dom && (document.compatMode == "CSS1Compat"))?1:0;
	this.ie4=(document.all && !this.dom)?1:0;
	this.ie=(this.ie4||this.ie5||this.ie6r||this.ie6s||this.ie7r||this.ie7s)?1:0;
	this.mac=(this.agent.indexOf("Mac")>-1)?1:0;
	this.ns6=(this.dom && (parseInt(this.ver) >= 5)) ?1:0; 
	this.ns4=(document.layers && !this.dom)?1:0;
	this.ns=(this.ns4||this.ns6)?1:0;
	this.bw5=(this.ie5||this.ie6r||this.ie7r)?1:0;
	this.bw6=(this.ie7r||this.ie7s||this.ie6s||this.ns6||this.opera6)?1:0;
	this.bw=(this.ie7r||this.ie7s||this.ie6r||this.ie6s||this.ie5||this.ns6||this.opera6)?1:0;
	return this;
}

var ENMarginTop, ENMarginBottom, ENTop;
var ENDivName, ENObject, ENCurrentY;

function ENInit(id, mt, mb, tp)
{
	bw=new browserCheck;
	if (bw.bw) {
		ENDivName = bw.bw5 ? document.all(id) : bw.bw6 ? document.getElementById(id) : 0;
		ENObject = ENDivName.style;
		ENObject.position = 'absolute';
		ENMarginTop = mt ? mt : 0; 
		ENMarginBottom = mb ? mb : 0; 
		ENCurrentY = ENTop = tp ? tp : ENDivName.offsetTop; 
		ENSmoothMove();
	} 
}

function ENSmoothMove()
{
	var winh = (bw.ie7s || bw.ie6s) ? document.documentElement.clientHeight : (bw.ns6||bw.opera6) ? innerHeight : bw.bw5 ? document.body.clientHeight : 0 ;
	var yt = (bw.ie7s || bw.ie6s) ? document.documentElement.scrollTop : bw.bw5 ? document.body.scrollTop : (bw.ns6||bw.opera6) ? window.pageYOffset : 0;
	var divh = ENDivName.offsetHeight;

	if (winh >= ENMarginTop + divh + ENMarginBottom) {
		yt = Math.max(yt + ENMarginTop, ENTop);
	} else {
		var yt1 = Math.max(yt + ENMarginTop, ENTop);
		var f1 = (yt1 > ENCurrentY) ? 1 : 0;
		var yt2 = yt - (divh + ENMarginBottom - winh);
		yt2 = Math.max(yt2, ENTop);
		var f2 = (yt2 < ENCurrentY) ? 1 : 0;
		if (f1 && f2) yt = ENCurrentY;
		else yt = f2 ? Math.max(yt1, yt2) : Math.min(yt1, yt2);
	}

	if (yt != ENCurrentY) {
		var vy = (yt - ENCurrentY) * 0.25;
		if (Math.abs(vy) < 1) vy = (vy > 0) ? 1 : (vy < 0) ? -1 : 0;
		ENCurrentY += Math.round(vy);
		ENObject.top = ENCurrentY + 'px';
	}
	setTimeout('ENSmoothMove()', 20);
}

bsVer=navigator.appVersion;
bsDom=document.getElementById?1:0;
IE=(document.all)?1:0;
IE5=(document.all && bsDom) ?1:0;
NN5=(bsDom && parseInt(bsVer) >= 5) ?1:0;
NN4=(document.layers && !bsDom)?1:0;

var startchgstyle = 0;

function DSC_SetUP(){startchgstyle=1}


function chg_bg(obj,clr) {
	if (startchgstyle && (IE || NN5)) {
		var div=NN5?document.getElementById(obj).style:IE?document.all(obj).style:0;
		div.backgroundColor = clr; 
	}
}


function PosMove_DIV(obj,leftnum,topnum) {
	if (startchgstyle && (IE5 || NN5)) {
		var div=NN5?document.getElementById(obj).style:IE?document.all(obj).style:0;
		div.position='relative';
		div.paddingLeft=leftnum; 
		div.paddingTop=topnum;
	}
}

function LAYSETTER(obj,topdivnum) {
	if(startchgstyle&&(IE5||NN5)){
		var div=NN5?document.getElementById(obj).style:IE?document.all(obj).style:0;
		div.position='absolute';
		div.top=topdivnum; 
	}
}

zindexnum=0;

function LAYCHANGER(obj,ZInum) {
	if(startchgstyle&&(IE5||NN5)){
		var div=NN5?document.getElementById(obj).style:IE?document.all(obj).style:0;
		if(div.position!='absolute')div.position='relative';
		zindexnum=ZInum;
		div.zIndex=zindexnum;
	}
}

bsVer=navigator.appVersion;
bsDom=document.getElementById?1:0;
IE=(document.all)?1:0;
IE5=(document.all && bsDom)?1:0;
NN5=(bsDom && parseInt(bsVer) >= 5)?1:0;
NN4=(document.layers && !bsDom)?1:0;

function DhtmlStarter(){
	ENInit('navbox');
	DSC_SetUP();
}

