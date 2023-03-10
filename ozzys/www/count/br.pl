sub br {
  if ($agent =~ /WebTV/i){ $br = "WebTV";}
  elsif ($agent =~ /DreamPassport\/([0-9\.]+)/i){ $br = "DreamPassport $1"; }
  elsif ($agent =~ /Justview/i){ $br = "Justview";}
  elsif ($agent =~ /AIR Mosaic/i){ $br = "AIR Mosaic"; }
  elsif ($agent =~ /ANT Fresco\/([0-9\.]+)/i){ $br = "ANT Fresco $1"; }
  elsif ($agent =~ /Aplix (.*)\/([0-9\.]+)/i){ $br = "Aplix $1 $2"; }
  elsif ($agent =~ /Arachne\/([0-9\.]+)/i){ $br = "Arachne $1"; }
  elsif ($agent =~ /AvantGo ([0-9\.]+)/i){ $br = "AvantGo $1"; }
  elsif ($agent =~ /AVE Front\/([0-9\.]+)/i){
    $ave = $1;
    if($agent =~ /(sharp wd browser|sharp tv browser|So-netStation|Fujitsu\/Debut|NEC\/Vx601)/i){ $br = "AVE Front $ave ($1)"; }
    else{ $br = "AVE Front $ave"; }
  }
  elsif ($agent =~ /BBB\/([0-9\.]+)/i){ $br = "BBB $1"; }
  elsif ($agent =~ /(.?)Cab\/([0-9\.]+)/i){ $br = "$1Cab $2"; }
  elsif ($agent =~ /Charbtte/i){ $br = "Charbtte"; }
  elsif (($agent =~ /Cyberdog\/([0-9\.]+)/i) or ($agent =~ /Cyberdog ([0-9\.]+)/i)){ $br = "Cyberdog $1"; }
  elsif (($agent =~ /DM_\/([0-9\.]+)/i) or ($agent =~ /DM ([0-9\.]+)/i)){ $br = "DM $1 (Device Mosaic)"; }
  elsif ($agent =~ /EasyRider(.*)\/([a-z]?)([0-9\.]+)/i){ $br = "BBB $3"; }
  elsif ($agent =~ /Flashnavi\/([0-9\.]+)/i){ $br = "Flashnavi $1"; }
  elsif ($agent =~ /iBOX/i){ $br = "iBOX"; }
  elsif ($agent =~ /Lite ([0-9\.]+)/i){ $br = "Lite $1"; }
  elsif ($agent =~ /MSIA ([0-9\.]+)/i){ $br = "MSIA $1"; }
  elsif ($agent =~ /Navigate_with_an_Accent\/([0-9\.]+)/i){ $br = "Navigate_with_an_Accent $1"; }
  elsif ($agent =~ /NetCaptor ([0-9\.]+)/i){ $br = "NetCaptor $1"; }
  elsif ($agent =~ /WebSurfer\/([0-9\.]+)/i){ $br = "NetManage Chameleon WebSurfer $1"; }
  elsif ($agent =~ /NETSGO Browser ([0-9\.]+)/i){ $br = "NETSGO Browser $1"; }
  elsif ($agent =~ /OmniWeb\/([0-9\.]+)/i){ $br = "OmniWeb $1"; }
  elsif (($agent =~ /Opera\/([0-9\.]+)/i) or ($agent =~ /Opera ([0-9\.]+)/i)){ $br = "Opera $1"; }
  elsif ($agent =~ /PlanetWeb\/([0-9\.]+)/i){ $br = "PlanetWeb $1"; }
  elsif ($agent =~ /Pockey\/([0-9\.]+)/i){ $br = "GetHTMLW $1"; }
  elsif ($agent =~ /PRODIGY-WB/i){ $br = "PRODIGY-WB"; }
  elsif ($agent =~ /MSPIE\/([0-9\.]+)/i){ $br = "Pocket Internet Explorer $1"; }
  elsif ($agent =~ /The PointCast Network ([0-9\.]+)/i){ $br = "The PointCast Network $1"; }
  elsif ($agent =~ /pwWebSpeak ([0-9\.]+)/i){ $br = "pwWebSpeak $1"; }
  elsif ($agent =~ /Sax Webster/i){ $br = "Sax Webster"; }
  elsif ($agent =~ /Sextant v([0-9\.]+)/i){ $br = "Sextant $1"; }
  elsif ($agent =~ /URL_Captor\/([0-9\.]+)/i){ $br = "URLキャプター $1"; }
  elsif ($agent =~ /QNX Voyager ([0-9\.]+)/i){ $br = "QNX Voyager $1"; }
  elsif ($agent =~ /WebBoy Version ([0-9\.]+)/i){ $br = "WebBoy $1"; }
  elsif ($agent =~ /WebExplorer( | DLL |-DLL)\/v([0-9\.]+)/i){ $br = "WebExplorer $2"; }
  elsif ($agent =~ /World(TALK V |TALK Ver|TALK\/)([0-9\.]+)/i){ $br = "WorldTALK $2"; } 
  elsif ($agent =~ /Internet Ninja ([0-9\.]+)/i){ $br = "Internet Ninja $1"; } 
  elsif ($agent =~ /Offline Explorer\/([0-9\.]+)/i){ $br = "Offline Explorer $1"; } 
  elsif ($agent =~ /libwww-perl\/([0-9\.]+)/i){ $br = "libwww-perl $1"; } 
  elsif ($agent =~ /DiaGem\/([0-9\.]+)/i){ $br = "DiaGem $1"; } 
  elsif ($agent =~ /Pockey\/([0-9\.]+)/i){ $br = "Pockey $1"; } 
  elsif ($agent =~ /Microsoft URL Control \- ([0-9\.]+)/i){ $br = "Microsoft URL Control $1"; } 
  elsif ($agent =~ /MSProxy\/([0-9\.]+)/i){ $br = "Websense $1"; } 
  elsif ($agent =~ /EirGrabber/i){ $br = "EirGrabber"; } 
  elsif ($agent =~ /BMChecker/i){ $br = "BookMark Checker"; } 

#ロボット関係
  elsif ($agent =~ /ia_archiver/i){ $br = "ia_archiver (robo)"; }
  elsif ($agent =~ /Googlebot/i){ $br = "Google (robo)"; }
  elsif ($agent =~ /Lycos_Spider/i){ $br = "Lycos (robo)"; }
  elsif ($agent =~ /moget/i){ $br = "goo (robo)"; }
  elsif ($agent =~ /Sidewinder/i){ $br = "InfoSeek (robo)"; }
  elsif ($agent =~ /suke/i){ $br = "kensaku (robo)"; }
  elsif ($agent =~ /ArchitextSpider/i){ $br = "Excite (robo)"; }
  elsif ($agent =~ /ASPSeek/i){ $br = "ASPSeek (robo)"; }
  elsif ($agent =~ /Openbot/i){ $br = "Openfind (robo)"; }
  elsif ($agent =~ /Slurp/i){ $br = "Inktomi (robo)"; }
  elsif ($agent =~ /BaiDuSpider/i){ $br = "BaiDu (robo)"; }
  elsif ($agent =~ /FAST-WebCrawler/i){ $br = "FAST (robo)"; }
  elsif ($agent =~ /InfoNaviRobot/i){ $br = "InfoNavi (robo)"; }
  elsif ($agent =~ /iYappo/i){ $br = "iYappo (robo)"; }
  elsif ($agent =~ /NABOT/i) { $br = "NAVER (robo)"; }
  elsif ($agent =~ /ZyBorg/i) { $br = "WiseNut (robo)"; }
  elsif ($agent =~ /All About Japan Link Patrol/i) { $br = "All About Japan (robo)"; }
  elsif ($agent =~ /HyperRobot/i) { $br = "HyperRobot (robo)"; }
  elsif ($agent =~ /indexpert/i) { $br = "フレッシュアイ (robo)"; }
  elsif ($agent =~ /p-goo/i) { $br = "i-get (robo)"; }

#以下はきちんと取れるか不明
  elsif ($agent =~ /SpaceBison\/([0-9\.]+)/i){ $br = "The Proxomitron $1"; }
  elsif ($agent =~ /Space Bison\/([0-9\.]+)/i){ $br = "The Proxomitron $1"; }
  elsif ($agent =~ /1X Net Browser/i){ $br = "1X Net Browser"; }
  elsif ($agent =~ /Amaya/i){ $br = "Amaya"; }
  elsif ($agent =~ /AtomNet/i){ $br = "AtomNet"; }
  elsif ($agent =~ /AMSD Ariadna/i){ $br = "AMSD Ariadna"; }
  elsif ($agent =~ /Arena/i){ $br = "Arena"; }
  elsif ($agent =~ /Candle Web/i){ $br = "Candle Web"; }
  elsif ($agent =~ /Cello/i){ $br = "Cello"; }
  elsif ($agent =~ /Chimera/i){ $br = "Chimera"; }
  elsif ($agent =~ /Custom Browser/i){ $br = "Express"; }
  elsif ($agent =~ /Gzilla/i){ $br = "Gzilla"; }
  elsif ($agent =~ /Microviet First Explorer/i){ $br = "Microviet First Explorer"; }
  elsif ($agent =~ /MMM/i){ $br = "MMM"; }
  elsif ($agent =~ /Mnemonic/i){ $br = "Mnemonic"; }
  elsif ($agent =~ /MSKBrowser/i){ $br = "MSKBrowser"; }
  elsif ($agent =~ /Netomat/i){ $br = "Netomat"; }
  elsif ($agent =~ /NetPositive/i){ $br = "NetPositive"; }
  elsif ($agent =~ /NetQuest/i){ $br = "NetQuest"; }
  elsif ($agent =~ /PalmBrowser/i){ $br = "PalmBrowser"; }
  elsif ($agent =~ /PolyWeb/i){ $br = "PolyWeb"; }
  elsif ($agent =~ /Power Browser/i){ $br = "Power Browser"; }
  elsif ($agent =~ /SimulBrowse/i){ $br = "SimulBrowse"; }
  elsif ($agent =~ /Tango/i){ $br = "Tango"; }
  elsif ($agent =~ /WebExplorer/i){ $br = "WebExplorer"; }
  elsif ($agent =~ /Amadeus/i){ $br = "Amadeus"; }
  elsif ($agent =~ /EwtWeb/i){ $br = "EwtWeb"; }
  elsif ($agent =~ /Final Frontier/i){ $br = "Final Frontier"; }
  elsif ($agent =~ /Entry Plug/i){ $br = "Entry Plug"; }
  elsif ($agent =~ /Syber Web/i){ $br = "Syber Web"; }
  elsif ($agent =~ /HyBrick/i){ $br = "HyBrick"; }
  elsif ($agent =~ /MDI Internet Browser/i){ $br = "MDI Internet Browser"; }
  elsif ($agent =~ /ONCBrowser/i){ $br = "ONCBrowser"; }
  elsif ($agent =~ /WBrowser/i){ $br = "WBrowser"; }
  elsif ($agent =~ /GrassHopper/i){ $br = "GrassHopper"; }
  elsif ($agent =~ /HotBar/i){ $br = "HotBar"; }
  elsif ($agent =~ /Katiesoft/i){ $br = "Katiesoft"; }
  elsif ($agent =~ /NeoPlanet/i){ $br = "NeoPlanet"; }
  elsif ($agent =~ /Talking Browser/i){ $br = "Talking Browser"; }
  elsif ($agent =~ /Web SurfACE/i){ $br = "Web SurfACE"; }
  elsif ($agent =~ /Dona Web Browser/i){ $br = "Dona Web Browser"; }
  elsif ($agent =~ /MDIWeb/i){ $br = "MDIWeb"; }
  elsif ($agent =~ /MonjaKids/i){ $br = "MonjaKids"; }
  elsif ($agent =~ /PointCast/i){ $br = "PointCast"; }
  elsif ($agent =~ /TabSurf Browser/i){ $br = "TabSurf Browser"; }
  elsif ($agent =~ /ICE Browser/i){ $br = "ICE Browser"; }
  elsif ($agent =~ /JoZilla/i){ $br = "JoZilla"; }
  elsif ($agent =~ /Emacs\/W3/i){ $br = "Emacs\/W3"; }
  elsif ($agent =~ /Links/i){ $br = "Links"; }
  elsif ($agent =~ /Lynx/i){ $br = "Lynx"; }
  elsif ($agent =~ /Internet Text Browser/i){ $br = "Internet Text Browser"; }
  elsif ($agent =~ /w3m/i){ $br = "w3m"; }
  elsif ($agent =~ /Webspyder/i){ $br = "Webspyder"; }
  elsif ($agent =~ /X2WEB/i){ $br = "X2WEB"; }
  elsif ($agent =~ /My Browser/i){ $br = "My Browser"; }

#以下携帯電話関連
  elsif ($agent =~ /docomo\/([0-9\.]+)\/(.*)/i){ $br = "DoCoMo $1 $2"; }
  elsif ($agent =~ /^J-PHONE/i){ $br = "J-PHONE $ENV{'HTTP_X_JPHONE_MSNAME'}"; }
  elsif ($agent =~ /^UP\.(.*)/i){ $br = "EZweb $1"; }

#以下ＩＥ関連
  elsif ($agent =~ /MSIE ([0-9\.]+)/i){ $br = "MSIE $1"; }

#以下ネスケ関連
  elsif ($agent =~ /Netscape(\d)\/([0-9\.]+)/i){
	$br = "Netscape $2";
	}
  elsif ($agent =~ /Mozilla\/(\d)([0-9\.]+)/i){
    $n1 = $1; $n2 = $2;
    if($agent =~ /(I|U|N)/){
      $br = "Netscape $n1$n2";
      if ($n1 == 5){ $br = "Netscape 6$n2"; }
    }
  elsif ($agent =~ /Mozilla\/([0-9\.]+)/i){ $br = "Mozilla $1"; }
#    else { $br = "etc"; }
    else { $br = "$agent (etc)"; }
  }
  elsif ($agent =~ /Mozilla\/([0-9\.]+)/i){ $br = "$agent"; }
  elsif ($agent eq ""){ $br = "Etc";}
#  else{ $br = "etc"; }
  else{ $br = "$agent (etc)"; }

}

1;
