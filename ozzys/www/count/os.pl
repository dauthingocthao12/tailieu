sub os{

$agent =~ s/</&lt;/g;
$agent =~ s/>/&gt;/g;

#ロボット関係
  if ($agent =~ /ia_archiver/i){ $os = "Etc (robo)"; }
  elsif ($agent =~ /Googlebot/i){ $os = "Etc (robo)"; }
  elsif ($agent =~ /Lycos_Spider/i){ $os = "Etc (robo)"; }
  elsif ($agent =~ /moget/i){ $os = "Etc (robo)"; }
  elsif ($agent =~ /Sidewinder/i){ $os = "Etc (robo)"; }
  elsif ($agent =~ /suke/i){ $os = "Etc (robo)"; }
  elsif ($agent =~ /ArchitextSpider/i){ $os = "Etc (robo)"; }
  elsif ($agent =~ /ASPSeek/i){ $os = "Etc (robo)"; }
  elsif ($agent =~ /Openbot/i){ $os = "Etc (robo)"; }
  elsif ($agent =~ /Slurp/i){ $os = "Etc (robo)"; }
  elsif ($agent =~ /BaiDuSpider/i){ $os = "Etc (robo)"; }
  elsif ($agent =~ /FAST-WebCrawler/i){ $os = "Etc (robo)"; }
  elsif ($agent =~ /InfoNaviRobot/i){ $os = "Etc (robo)"; }
  elsif ($agent =~ /iYappo/i){ $os = "Etc (robo)"; }
  elsif ($agent =~ /NABOT/i) { $os = "Etc (robo)"; }
  elsif ($agent =~ /ZyBorg/i) { $os = "Etc (robo)"; }
  elsif ($agent =~ /All About Japan Link Patrol/i) { $os = "Etc (robo)"; }
  elsif ($agent =~ /HyperRobot/i) { $os = "Etc (robo)"; }
  elsif ($agent =~ /indexpert/i) { $os = "Etc (robo)"; }
  elsif ($agent =~ /p-goo/i) { $os = "Etc (robo)"; }

  elsif ($agent =~ /Win(.*)(95|98|NT|CE|3\.1|ME|9x)/i) {
	if ($agent =~ /NT/i){
		if ($agent =~ /NT 5\.0/i){ $os = "2000"; }
		elsif ($agent =~ /NT 5\.1/i){ $os = "XP"; }
		else { $os = "NT $1"; }
			}
	if ($agent =~ /98/i){
		if ($agent =~ /Win 9x 4\.90/i){ $os = "ME"; } else { $os = "98"; }
			}
	if ($os eq "") { $os = $2; }
	$os =~ y/a-z/A-Z/; $os = "Windows $os"; 
  }

elsif ($agent =~ /Macintosh/i) { $os = "Macintosh"; }
elsif (($agent =~ /Mac_PowerPC/i) && ($agent =~ /MSIE 5\.1b1/i)) { $os = "Mac OS X"; }#代用
elsif ($agent =~ /Mac_PowerPC/i) { $os = "Mac_PowerPC"; }
elsif ($agent =~ /X11/i) { $os = "X11"; }
elsif ($agent =~ /Linux/i) { $os = "Linux"; }
elsif ($agent =~ /SunOS/i) { $os = "SunOS"; }
elsif ($agent =~ /FreeBSD/i) { $os = "FreeBSD"; }
elsif ($agent =~ /WebTV/i) { $os = "WebTV"; }
elsif ($agent =~ /AIX/i) { $os = "AIX"; }
elsif ($agent =~ /OSF1/i) { $os = "OSF1"; }
elsif ($agent =~ /NEWS-OS/i) { $os = "NEWS-OS"; }
elsif ($agent =~ /IRIX/i) { $os = "IRIX"; }
elsif ($agent =~ /HP-UX/i) { $os = "HP-UX"; }
elsif ($agent =~ /BSD\/OS/i) { $os = "BSD/OS"; }
elsif ($agent =~ /AIX/i){ $os = "AIX"; }
elsif ($agent =~ /OSF1/i){ $os = "OSF1"; }
elsif ($agent =~ /NEWS-OS/i){ $os = "NEWS-OS"; }
elsif ($agent =~ /IRIX/i){ $os = "IRIX"; }
elsif ($agent =~ /HP-UX/i){ $os = "HP-UX"; }
elsif (($agent =~ /DreamPassport/i) or ($agent =~ /Dreamcast ([us]*)/i)){ $os = "DreamCast"; }
elsif ($agent =~ /AmigaOS\/([0-9\.]+)/i){ $os = "AmigaOS $1"; }
elsif ($agent =~ /NetBSD/i){ $os = "NetBSD"; }
elsif ($agent =~ /OS\/400/i){ $os = "OS/400"; }
elsif ($agent =~ /VMS/i){ $os = "VMS"; }
elsif ($agent =~ /Lisa2/i){ $os = "Lisa2"; }
elsif ($agent =~ /docomo\/([0-9\.]+)\/(.*)/i){ $os = "DoCoMo/$2"; }
elsif ($agent =~ /^J-PHONE/i){ $os = "J-PHONE $ENV{'HTTP_X_JPHONE_MSNAME'}"; }
elsif ($agent =~ /^UP\./i){ $os = "au or TU-KA"; }

#不明
  elsif ($agent =~ /Internet Ninja/i){ $os = "Windows"; }
  elsif ($agent =~ /SpaceBison/i){ $os = "Windows"; }
  elsif ($agent =~ /Space Bison/i){ $os = "Windows"; }
  elsif ($agent =~ /Offline Explorer/i){ $os = "Windows"; } 
  elsif ($agent =~ /libwww-perl\/([0-9\.]+)/i){ $os = "Windows"; } 
  elsif ($agent =~ /DiaGem\/([0-9\.]+)/i){ $os = "Windows"; } 
  elsif ($agent =~ /Pockey\/([0-9\.]+)/i){ $os = "Windows"; } 
  elsif ($agent =~ /EirGrabber/i){ $os = "Windows"; } 
  elsif ($agent =~ /Microsoft URL Control/i){ $os = "Windows"; } 
  elsif ($agent =~ /MSProxy/i){ $os = "Windows"; } 
  elsif ($agent =~ /BMChecker/i){ $os = "Windows"; } 

elsif ($agent =~ /Mozilla\/3\.01 (compatible\;)/i){ $os = "Etc"; } 
elsif ($agent eq ""){ $os = "Etc";}
else {$os = "$agent (etc)"; }
#else {$os = "Etc."; }

}

1;
