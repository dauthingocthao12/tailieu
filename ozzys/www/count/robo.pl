
@ROBO = ('','Google','Lycos','goo','InfoSeek',
	'kensaku','ASPSeek','Openfind','Inktomi',
	'Excite','BaiDu','FAST','InfoNavi','iYappo','NAVER','WiseNut','フレッシュアイ',
	'i-get');
@URL = ('','http://www.google.com','http://www.lycos.co.jp/','http://www.goo.ne.jp/','http://www.infoseek.co.jp/',
	'http://kensaku.org/','http://www.aspseek.org/','http://www.openfind.com.tw/','http://www.inktomi.com/',
	'http://www.excite.co.jp/','http://www.baidu.com/','http://fast.no/','http://infonavi.infoweb.ne.jp/',
	'http://i.yappo.ne.jp/','http://search.naver.com/','http://www.wisenut.com/','http://www.fresheye.com/',
	'http://www.i-get.ne.jp/');

#ロボットの可能性データー
#larbin_devel sebastien.ailleret@inria.fr
#RRC crawler_admin@bigfoot.com
#BunnySlippers



sub robo {
	$bot = "";
	if ($agent =~ /Googlebot/i) { $bot = "Google"; }
	elsif ($agent =~ /Lycos_Spider/i) { $bot = "Lycos"; }
	elsif ($agent =~ /moget/i) { $bot = "goo"; }
	elsif ($agent =~ /Sidewinder/i) { $bot = "InfoSeek"; }
	elsif ($agent =~ /suke/i) { $bot = "kensaku"; }
	elsif ($agent =~ /ArchitextSpider/i) { $bot = "Excite"; }
	elsif ($agent =~ /FAST-WebCrawler/i) { $bot = "FAST"; }
	elsif ($agent =~ /InfoNaviRobot/i) { $bot = "InfoNavi"; }
	elsif ($agent =~ /iYappo/i) { $bot = "iYappo"; }
	elsif ($agent =~ /ZyBorg/i) { $bot = "WiseNut"; }
	elsif ($agent =~ /indexpert/i) { $bot = "フレッシュアイ"; }
	elsif ($agent =~ /p-goo/i) { $bot = "i-get"; }

#下記は海外？
	elsif ($agent =~ /ASPSeek/i) { $bot = "ASPSeek"; }
	elsif ($agent =~ /Openbot/i) { $bot = "Openfind"; }
	elsif ($agent =~ /Slurp/i) { $bot = "Inktomi"; }
	elsif ($agent =~ /BaiDuSpider/i) { $bot = "BaiDu"; }
	elsif ($agent =~ /NABOT/i) { $bot = "NAVER"; }

	if ($name eq "") {
		$logfile_r = "./log/log_/robo.dat";

			} else {

	$logfile_r = "./log\/log_$name\/robo.dat";
		}

	if (-e $logfile_r) {
	open(IN,"$logfile_r");
	@line_r = <IN>;
	close(IN);
	chmod (0666,"$logfile_r");
		}

	&get_time;
	$date_r = "$year\/$mon\/$mday";

	$flag = 0;
	$flag2 = 0;
	@lines_r = ();
	foreach (@line_r) {
		## ($name_k1,$date_r1,$date_r2,$date_r3,$date_r4,$date_r5,$date_r6) = split(/\,/,$_);
		($name_k1,$date_r1,$date_r2,$date_r3,$date_r4,$date_r5,$date_r6) = explode(/\,/,$_);
		if (($bot eq $name_k1) && ($date_r ne $date_r1)) {
				$lines = "$name_k1,$date_r,$date_r1,$date_r2,$date_r3,$date_r4,$date_r5,\n";
				push(@lines_r,$lines);
				$flag = 1;
					}
		elsif (($bot eq $name_k1) && ($date_r eq $date_r1)){
				push(@lines_r,$_);
				$flag2 = 1;
					}
		else { push(@lines_r,$_); }
			}

	if (($bot ne "") && ($flag == 0) && ($flag2 == 0)) { 
			$line_r_n = "$bot,$date_r,,,,,,\n";
			push(@lines_r,$line_r_n);
			$flag = 1;
			}


	if ($flag == 1) {
 	# ロックファイル名を定義
	$lockdir = "lock_robo_$name";

	# ロック開始
	if ($lockkey == 1) { &lock1; }
	elsif ($lockkey == 2) { &lock2; }

	# 記録ファイルを更新する
	open(OUT,">$logfile_r");
	eval 'flock(OUT,2);';
	print OUT @lines_r;
	eval 'flock(OUT,8);';
	close(OUT);
	chmod (0666,"$logfile_r");

	# ロック解除
	if (-e $lockdir) { rmdir($lockdir); }
		}

}

sub robo_h {

	if ($name eq "") {
		$logfile_r = "./log/log_/robo.dat";
			} else {
	$logfile_r = "./log\/log_$name\/robo.dat";
		}

	$size = (stat ($logfile_r))[7];

	if ($size != 0) {

	open(IN,"$logfile_r");
	@line_r = <IN>;
	close(IN);


print <<"ALPHA";
<CENTER>
<TABLE border="0" width="700" cellpadding="0" cellspacing="0">
  <TBODY>
    <TR>
      <TD><B>検索ロボットチェック</B>
      <HR color="#0000FF">
      <TABLE border="0" cellpadding="0" cellspacing="0" bgcolor="#0000FF">
        <TBODY>
          <TR>
            <TD>
            <TABLE border="0" width="780">
              <TBODY>
		    <TR bgcolor="#ffffff">
		      <TD width="180">検索サイト名</TD>
		      <TD width="100" align="center">最　新</TD>
		      <TD width="100" align="center">前　回</TD>
		      <TD width="100" align="center">2回前</TD>
		      <TD width="100" align="center">3回前</TD>
		      <TD width="100" align="center">4回前</TD>
		      <TD width="100" align="center">5回前</TD>
		    </TR>
ALPHA

		foreach $_(@line_r) {
			## ($name_r,$date_r1,$date_r2,$date_r3,$date_r4,$date_r5,$date_r6) = split(/\,/,$_);
			($name_r,$date_r1,$date_r2,$date_r3,$date_r4,$date_r5,$date_r6) = explode(/\,/,$_);

			$i = "";
			foreach $name_r1(@ROBO) {
				if ($name_r eq $name_r1) { $url_r = "$URL[$i]"; }
				++$i;
					}

			if ($date_r1 eq "") { $date_r1 = "---"; }
			if ($date_r2 eq "") { $date_r2 = "---"; }
			if ($date_r3 eq "") { $date_r3 = "---"; }
			if ($date_r4 eq "") { $date_r4 = "---"; }
			if ($date_r5 eq "") { $date_r5 = "---"; }
			if ($date_r6 eq "") { $date_r6 = "---"; }

			print <<"ALPHA";
			    <TR bgcolor="#ffffff">
			      <TD align="center"><B><A href = "$url_r" target="_blanr">$name_r</A></B></TD>
			      <TD align="center">$date_r1</TD>
			      <TD align="center">$date_r2</TD>
			      <TD align="center">$date_r3</TD>
			      <TD align="center">$date_r4</TD>
			      <TD align="center">$date_r5</TD>
			      <TD align="center">$date_r6</TD>
			    </TR>
ALPHA

}

		print <<"ALPHA";
              </TBODY>
            </TABLE>
            </TD>
          </TR>
        </TBODY>
      </TABLE>
      </TD>
    </TR>
  </TBODY>
</TABLE>
</CENTER>
ALPHA

	}

}

1;
