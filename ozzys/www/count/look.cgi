#!/usr/local/bin/perl
#
#################################################
#	アクセス解析（　）			#
#	Ver.1_0 (00/07/2001)			#
#	（株）アルファテック			#
#		製作：大河原　康史		#
#	email:ookawara@alphatec.co.jp		#
#	HP:http://www.alphatec.co.jp/		#
#################################################


$l = 1;
require './jcode.pl';
require './kanri.pl';
require './key.pl';
require './robo.pl';



#メイン--------------------------------
&decode;

&get_time;
	if ($name eq "") {
	$logfile_a = "./log/log_/count.dat";					#カウンター総数
	$logfile_t = "./log/log_/$year/$mon/$mday.dat"; 			#今日のカウント
	$logfile_y = "./log/log_/$year_y/$mon_y/$mday_y.dat";		#昨日のカウント
	$logfile_mn_ = "./log/log_/$y_n/$m_n/$y_n$m_n.dat"; 			#次月
		} else {
	$logfile_a = "./log/log_$name/count.dat";				#カウンター総数
	$logfile_t = "./log/log_$name/$year/$mon/$mday.dat"; 		#今日のカウント
	$logfile_y = "./log/log_$name/$year_y/$mon_y/$mday_y.dat";		#昨日のカウント
	$logfile_mn_ = "./log/log_$name/\$y_n/\$m_n/\$y_n\$m_n.dat"; 		#次月
		}

if ($FORM{'mode'} eq "top") { &top; }
if ($FORM{'mode'} eq "caren") { &caren; }
if ($FORM{'mode'} eq "day") { &day; }


&main;

exit;

#初期画面-------------------------------
sub main {

if ($pass != 0) { $nopass = 1; &top; }

print "Content-type: text/html\n\n";

print <<"ALPHA";
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=Shift_JIS">
<META http-equiv="Content-Style-Type" content="text/css">
<TITLE>アクセス解析閲覧パースワード</TITLE>
</HEAD>
<BODY bgcolor="#ccffff">
<CENTER>
<BR>
<BR>
$title<BR>
アクセス解析閲覧パースワード<BR>
<FORM action="$script" method="$method">
<INPUT type=hidden name="mode" value="top">
<INPUT size="20" type="text" name="pass_key"><BR>
<INPUT type="submit" value="送信">&nbsp;&nbsp;&nbsp;&nbsp;<INPUT type="reset"></FORM>
</CENTER>
</BODY>
</HTML>
ALPHA

exit;

}

#初期画面-------------------------------
sub top {

if ($pass == 0) {

	if ($FORM{'pass_key'} eq "") {&error("パスワードが不適切です。");}

	opendir (DIR,".");
	@list = readdir (DIR);
		$flag = 0;
		foreach (@list) {
			if ($_ eq "tec.pl") { $flag = 1; last;}
			}
		if ($flag == 1) {
			open (IN,"tec.pl") || &error("Can't read File");
			$line = <IN>;
			close(IN);
			$pass_key = $line;
			$pass_keys = crypt ($FORM{'pass_key'},$salt);
				} else { $pass_keys = $FORM{'pass_key'}; }
		if ($pass_key ne $pass_keys) { &error("パスワードが間違っています。");}
	}

&caren;

}

#外部リンク防止-------------------------------
sub link {
	$referer = $ENV{'HTTP_REFERER'};
	if ($nopass != 1) {
		if (index($referer,$script) < 0) { &home; }
		}
}

sub home {
print "Content-type: text/html\n\n";

print <<"ALPHA";
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=Shift_JIS">
<META http-equiv="Content-Style-Type" content="text/css">
<META http-equiv="Refresh" content="1; url=$homepage";>
<TITLE>禁止</TITLE>
</HEAD>
<BODY bgcolor="$bgcolor">
<CENTER>
<BR>
<BR>
<FONT color="#ff0000">直接リンクは禁止です。</FONT>
</CENTER>
</BODY>
</HTML>
ALPHA

exit;

}

#月表示-------------------------------
sub caren {

&link;
&get_time;

if (($FORM{'y'} eq "") || ($FORM{'m'} eq "")) {
		$y = $year;
		$m = $mon;
		$d = $mday;
			} else {
		$y = $FORM{'y'};
		$m = $FORM{'m'};
			}

&week;
if (($y % 4 == 0) && ($y % 400 != 0)) { $MON[2] = 29;} else { $MON[2] = 28; }

&count;

&hizuke;

	$date_file = './log';
	$check = (eval { opendir(DIR,"$date_file"); }, $@ eq "");
	@files = readdir(DIR);
	foreach (@files) {
		if ( $_ =~ /log/i ) {
			$_ =~ s/log_//i;
			push (@file,$_);
				}
			}
	$namepl = './name.pl';
	if (-e $namepl) { require $namepl; }
	if ($name eq "") { $name_ = ""; } else { &name; }

print "Content-type: text/html\n\n";
print <<"ALPHA";
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=Shift_JIS">
<META http-equiv="Content-Style-Type" content="text/css">
<TITLE>$title $name_ $y年$m_t月のアクセス一覧</TITLE>
</HEAD>
<BODY>
<BR>
<CENTER>
<TABLE border="0" width="780" cellpadding="0" cellspacing="0">
  <TBODY>
    <TR>
      <TD>
ALPHA

	$files_k = @file;
	if (($files_k == 0) || ($files_k == 1)) {
print <<"ALPHA";
<B><FONT size="+2">$title</FONT></B>
ALPHA
	} else {
print <<"ALPHA";
		<FORM action="$script" method="$method">
		<B><FONT size="+2">$title</FONT></B> 
<SELECT name="name">
ALPHA
	$name_ = "";
	foreach (@file) {
		if (-e $namepl) { &name; } else { $name_ = $_; }
		if ($_ eq "") { $name_ = "メイン"; $_ = ""; }
		if (($name ne "") && ($name eq $_)) { $selected = "selected";}
		elsif (($name eq "") && ($name eq $_)) { $selected = "selected";}
		else { $selected = ""; }
		print "<OPTION value=\"$_\" $selected>$name_</OPTION>\n";
		}
print <<"ALPHA";
	</SELECT>
	 <INPUT type="submit" value="変更">
	</FORM>

ALPHA
	}

print <<"ALPHA";
      </TD>
    </TR>
  </TBODY>
</TABLE>
<BR>
<TABLE border="0" cellpadding="0" cellspacing="0" bgcolor="#0000FF">
  <TBODY>
    <TR>
      <TD>
      <TABLE border="0" width="600">
        <TBODY>
          <TR bgcolor="#ffffff">
            <TD rowspan="14" align="center" valign="top">
      <TABLE border="0" cellpadding="0" cellspacing="0" width="130">
        <TBODY>
          <TR>
            <TD colspan="2"><BR>
            アクセス解析</TD>
          </TR>
          <TR>
            <TD colspan="2"><BR>
            </TD>
          </TR>
          <TR>
            <TD width="60">合計</TD>
            <TD align="right"><FONT color="#ff0000">$cun_a</FONT></TD>
          </TR>
          <TR>
            <TD colspan="2"><BR>
            </TD>
          </TR>
          <TR>
            <TD>今日</TD>
            <TD align="right"><FONT color="#ff0000">$cun_t</FONT></TD>
          </TR>
          <TR>
            <TD>昨日</TD>
            <TD align="right">$cun_y</TD>
          </TR>
          <TR>
            <TD colspan="2"><BR>
            </TD>
          </TR>
ALPHA
	if ($mon == $m_l) { $ff = "<FONT color=\"#ff0000\">"; $fl = "</FONT>";}
			else { $ff = ""; $fl = ""; }

	if ($cun_ml ne "") {
		print <<"ALPHA";
	          <TR>
	            <TD>$m_l月合計</TD>
	            <TD align="right">$ff$cun_ml$fl</TD>
	          </TR>
ALPHA
		} else { print "<TR><TD colspan=\"2\">&nbsp;</TD></TR>\n"; }

	if ($mon == $m_t) { $ff = "<FONT color=\"#ff0000\">"; $fl = "</FONT>";}
			else { $ff = ""; $fl = ""; }

print <<"ALPHA";
          <TR>
            <TD>$m_t月合計</TD>
            <TD align="right">$ff$cun_mt$fl</TD>
          </TR>
ALPHA
	if ($mon == $m_n) { $ff = "<FONT color=\"#ff0000\">"; $fl = "</FONT>";}
			else { $ff = ""; $fl = ""; }

	if ($cun_mn ne "") {
		print <<"ALPHA";
 	         <TR>
	            <TD>$m_n月合計</TD>
	            <TD align="right">$ff$cun_mn$fl</TD>
	          </TR>
ALPHA
		} else {
			print "<TR><TD colspan=\"2\">&nbsp;</TD></TR>\n";
		}

$m_l = sprintf("%02d",$m_l); 
$m_n = sprintf("%02d",$m_n); 
print <<"ALPHA";
        </TBODY>
      </TABLE>
      </TD>
            <TD colspan="7" align="center">
ALPHA

	if (((-e $logfile_ml) && ($ne == 1)) || ($ne == 0)) {
			print "<A href = \"$script?mode=caren&y=$y_l&m=$m_l&name=$name\">&lt;&lt;</A>\n";
				} else {
			print "&nbsp;&nbsp;\n";
				}

			print "&nbsp;&nbsp;&nbsp;$y年&nbsp;$m_t月&nbsp;&nbsp;&nbsp;\n";

	if (((-e $logfile_mn) && ($ne == 1)) || ($ne == 0)) {
			print "<A href = \"$script?mode=caren&y=$y_n&m=$m_n&name=$name\">&gt;&gt;</A>\n";
				} else {
			print "&nbsp;&nbsp;\n";
				}
print <<"ALPHA";
	    </TD>
          </TR>
          <TR bgcolor="#ffffff">
ALPHA
	for ($i=0; $i<=6; ++$i){
		if ($i == 0) { $color = "#ff0000"; }
			elsif ($i == 6){ $color = "#0000ff"; } else { $color = "#000000";}
			print "<TD align=\"center\" width=\"60\"><FONT color=$color>$WEEK[$i]</FONT></TD>";
			}
     print "</TR>\n";
	$month = $MON[$m]+$w;
	for ($i=1; $i<=$month; ++$i) {
		$flag = 0;

		$amari = $i % 7;
		if ($amari == 1) { print "			<TR bgcolor=\"#ffffff\">\n"; $d2="";}
		if ($i <= $w){
				$d1 = "&nbsp;";  $d2 .= "<TD>&nbsp;</TD>";
					} else {
				$d1 = $i-$w;
				$cun_h = "";
				$co = "";
				foreach $lines_mt(@line_mt){
##					($days,$cun) = split(/\,/, $lines_mt);
					($days,$cun) = explode(/\,/, $lines_mt);
					if ($days == $d1) { $cun_h = $cun; last; }
							}
			if (($year == $y) && ($mon == $m) && ($mday == $d1)) { $co = " bgcolor=\"#ccffff\""; }
				if ($cun_h eq "") { $cun_h = "&nbsp;";}
				$d2 .= "<TD align=\"right\" $co>$cun_h</TD>";
						}
&holyday;

	if ($d1 >= 1) {
		$date_days = "$script?mode=day&y=$y&m=$m_t&d=$d1&name=$name";
		if (($amari == 1) || ($flags == 1) || ($flags2 == 1)) {
			print "	<TD align=\"center\"><A href = \"$date_days\"><FONT color=\"#ff0000\">$d1</FONT></A></TD>\n";
			} elsif ($amari == 0) {
			print "	<TD align=\"center\"><A href = \"$date_days\"><FONT color=\"#0000ff\">$d1</FONT></A></TD>\n";
			} else {
		print "	<TD align=\"center\"><A href = \"$date_days\"><FONT color=\"#000000\">$d1</FONT></A></TD>\n";
			}
		} else { print "<TD>$d1</TD>\n"; }
		if ($i == $month) {
			$kara = 7-$amari;
				if ($kara != 7) {
					for ($ii=1; $ii<=$kara; ++$ii) {
						print "<TD>&nbsp;</TD>"; $d2 .= "<TD>&nbsp;</TD>";
						}
					}
				}
		if ($amari == 0) { print "			</TR>\n"; $flag = 1;}
		if (($flag == 1) && ($i != $month)) { print "<TR align=\"center\" bgcolor=\"#ffffff\">$d2</TR>\n"; }

		if (($flags == 1) && (($amari ==1) || ($flags2 == 1)))
						{ $flags2 = 1; $flags = 0;} else { $flags = 0; $flags2 = 0;}
		}
		print "<TR align=\"center\" bgcolor=\"#ffffff\">$d2</TR>\n";
print <<"ALPHA";
        </TBODY>
      </TABLE>
      </TD>
    </TR>
  </TBODY>
</TABLE>
</CENTER>
<CENTER><BR>
</CENTER>
<CENTER>
<TABLE border="0">
  <TBODY>
    <TR>
      <TD height="100">
      <TABLE border="0" cellpadding="0" cellspacing="0">
        <TBODY>
          <TR>
            <TD><B>$y年$m_t月の日毎のカウンター数の流れ</B><BR>
            <HR color="#0000FF">
            <TABLE border="0" cellpadding="0" cellspacing="0" bgcolor="#0000FF">
              <TBODY>
                <TR>
                  <TD>
                  <TABLE border="0">
                    <TBODY>
                      <TR bgcolor="#ffffff">
ALPHA
		$cun_max = "";
		foreach $lines_mt(@line_mt){
##			($days,$cun) = split(/\,/, $lines_mt);
			($days,$cun) = explode(/\,/, $lines_mt);
			if ($cun_max < $cun) { $cun_max = $cun; }
				}
		if ($cun_max eq "") { $cun_max = 0; }
		for ($i=1; $i<=$month; ++$i) {
		$d1 = $i-$w;
		if ($d1 > 0) {

			$cun_h = "";
			foreach $lines_mt(@line_mt){
##				($days,$cun) = split(/\,/, $lines_mt);
				($days,$cun) = explode(/\,/, $lines_mt);
				if ($d1 == $days) {
					$cun_he = int($cun/$cun_max*100);
					if ($cun_h <= 1) { $cun_h = 1; }
					$cun_h = "<FONT size=\"-1\">$cun</FONT><BR><IMG src=\"./image/ao.gif\" width=\"10\" height=\"$cun_he\" border=\"0\">"; last; }
						}
			if ($cun_h eq "") { $cun_h = "<FONT size=\"-1\">0</FONT>";}

			print "<TD height=\"110\" width=\"20\" align=\"center\" valign=\"bottom\">$cun_h</TD>\n";

{ $cun_h = 0; }
				}
			}
		print "                      </TR>\n";
		print "<TR bgcolor=\"#ffffff\">\n";
		$month = $MON[$m]+$w;
		for ($i=1; $i<=$month; ++$i) {
		$amari = $i % 7;
		$d1 = $i-$w;
		if ($d1 > 0) {
&holyday;
			if (($amari == 1) || ($flags == 1) || ($flags2 == 1)) {
					print "	<TD align=\"center\"><FONT color=\"#ff0000\">$d1</FONT></TD>\n";
				} elsif ($amari == 0) {
					print "	<TD align=\"center\"><FONT color=\"#0000ff\">$d1</FONT></TD>\n";
				} else {
			print "	<TD align=\"center\">$d1</TD>\n";
				}
			}
		if (($flags == 1) && (($amari ==1) || ($flags2 == 1)))
						{ $flags2 = 1; $flags = 0;} else { $flags = 0; $flags2 = 0;}
		}

print <<"ALPHA";
                      </TR>
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
      </TD>
    </TR>
  </TBODY>
</TABLE>
</CENTER>
<BR>
ALPHA

&robo_h;

print <<"ALPHA";
</BODY>
</HTML>
ALPHA


exit;

}

#日前後表示-----------------------------
sub hizuke {

	$y_t = $y+0;
	$m_t = $m+0;
	$d_t = $d+0;

	if ($FORM{'mode'} eq "caren") {
			$m_l = $m_t-1;
			if ($m_l < 1) { $m_l = 12; $y_l = $y_t-1; } else { $y_l = $y_t; }
			$m_n = $m_t+1;
			if ($m_n > 12) { $m_n = 1; $y_n = $y_t+1; } else { $y_n = $y_t; }
			} else {
			$y_l = $y_t;
			$m_l = $m_t;
			$d_l = $d-1;
			if ($d_l < 1) { $m_l = $m_l-1; }
			if ($m_l < 1) { $y_l = $y_l-1; $m_l = 12;}
			if (($y_l % 4 == 0) && ($y_l % 400 != 0)) { $MON[2] = 29;} else { $MON[2] = 28; }
			if ($d_l < 1) { $d_l = $MON[$m_l];}

			$y_n = $y_t;
			$m_n = $m_t;
			$d_n = $d+1;
			if (($y_n % 4 == 0) && ($y_n % 400 != 0)) { $MON[2] = 29;} else { $MON[2] = 28; }
			if ($d_n > $MON[$m_n]) { $m_n = $m_n+1; $d_n = 1; }
			if ($m_n > 12) { $y_n = $y_n+1; $m_n = 1;}
			}
}

#日毎詳細-------------------------------
sub day {
&link;
	$y = "$FORM{'y'}";
	$m = sprintf("%02d",$FORM{'m'}); 
	$d = sprintf("%02d",$FORM{'d'}); 

	if ($name eq "") {
		$logfile_day = "./log/log_/$y/$m/$d.dat";
			} else {
		$logfile_day = "./log/log_$name/$y/$m/$d.dat";
		}

&hizuke;

	unless (-e $logfile_day) { &not_date; }
	open(IN,"$logfile_day");
	@line_d = <IN>;
	close(IN);

$a = $FORM{'a'};
if ($FORM{'a_1'} ne "") {$a_1 = $FORM{'a_1'};} elsif ($a == 1) {$a_1 = 0;}
if ($FORM{'a_2'} ne "") {$a_2 = $FORM{'a_2'};} elsif ($a == 1) {$a_2 = 0;}
if ($FORM{'a_3'} ne "") {$a_3 = $FORM{'a_3'};} elsif ($a == 1) {$a_3 = 0;}
if ($FORM{'a_4'} ne "") {$a_4 = $FORM{'a_4'};} elsif ($a == 1) {$a_4 = 0;}
if ($FORM{'a_5'} ne "") {$a_5 = $FORM{'a_5'};} elsif ($a == 1) {$a_5 = 0;}
if ($FORM{'a_6'} ne "") {$a_6 = $FORM{'a_6'};} elsif ($a == 1) {$a_6 = 0;}

if ($a_1 == 1) { $check_1 = "checked"; }
if ($a_2 == 1) { $check_2 = "checked"; }
if ($a_3 == 1) { $check_3 = "checked"; }
if ($a_4 == 1) { $check_4 = "checked"; }
if ($a_5 == 1) { $check_5 = "checked"; }
if ($a_6 == 1) { $check_6 = "checked"; }

	$r = 0; $o = 0; $b = 0; $b = 0; $h = 0; $v = 0; $wo = 0;
		foreach $lines_d (@line_d) {
##			($times,$referer,$os,$br,$host,$via) = split(/\,/, $lines_d);
			($times,$referer,$os,$br,$host,$via) = explode(/\,/, $lines_d);
				$time = substr($times, 0, 2);

				if ($time ne "") { $TIME{$time}++; }
				if (($a_1 == 1)	&& ($referer ne "")) { $REFERER{$referer}++; $r++; }
				if ($a_2 == 1) { &keyword; }
				if (($a_3 == 1)	&& ($os ne "")) { $OS{$os}++; $o++; }
				if (($a_4 == 1)	&& ($br ne "")) { $BR{$br}++; $b++; }
				if (($a_5 == 1)	&& ($host ne "")) { $HOST{$host}++; $h++; }
				if (($a_6 == 1)	&& ($via ne "")) { $VIA{$via}++; $v++; }
				if ($a_2 == 1) { &keyword; }
				}
$cun_t = @line_d;
&week;

print "Content-type: text/html\n\n";
print <<"ALPHA";
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=Shift_JIS">
<META http-equiv="Content-Style-Type" content="text/css">
<TITLE>$title $y年$FORM{'m'}月$FORM{'d'}日のアクセス結果</TITLE>
<SCRIPT LANGUAGE="JavaScript">
<!--
function autoclear() {
 if (self.document.send) {
    if (self.document.send.a_1.checked) {
      self.document.send.a_1.checked = false;
    }
    if (self.document.send.a_2.checked) {
      self.document.send.a_2.checked = false;
    }
    if (self.document.send.a_3.checked) {
      self.document.send.a_3.checked = false;
    }
    if (self.document.send.a_4.checked) {
      self.document.send.a_4.checked = false;
    }
    if (self.document.send.a_5.checked) {
      self.document.send.a_5.checked = false;
    }
    if (self.document.send.a_6.checked) {
      self.document.send.a_6.checked = false;
    }
 }
}
function autoon() {
 if (self.document.send) {
    if (self.document.send.a_1) {
      self.document.send.a_1.checked = true;
    }
    if (self.document.send.a_2) {
      self.document.send.a_2.checked = true;
    }
    if (self.document.send.a_3) {
      self.document.send.a_3.checked = true;
    }
    if (self.document.send.a_4) {
      self.document.send.a_4.checked = true;
    }
    if (self.document.send.a_5) {
      self.document.send.a_5.checked = true;
    }
    if (self.document.send.a_6) {
      self.document.send.a_6.checked = true;
    }
 }
}
// -->

</SCRIPT>
</HEAD>
<BODY>
<CENTER>
<TABLE border="0" width="600" cellpadding="0" cellspacing="0">
  <TBODY>
    <TR>
      <TD colspan="4"><B><FONT size="+1">$title</FONT></B></TD>
    </TR>
    <TR>
      <TD width="300" colspan="3"><B><FONT size="+1">$y年$FORM{'m'}月$FORM{'d'}日($WEEK[$w])のアクセス解析</FONT></B><BR>
      今日のカウンター数 <B>$cun_t</B><BR>
      </TD>
      <TD width="300" rowspan="2">
	<FORM action="$script" method="$method" name="send">
	<INPUT type=hidden name="mode" value="day">
	<INPUT type=hidden name="y" value="$FORM{'y'}">
	<INPUT type=hidden name="m" value="$FORM{'m'}">
	<INPUT type=hidden name="d" value="$FORM{'d'}">
	<INPUT type=hidden name="a" value="1">
	<INPUT type=hidden name="name" value="$name">
      <TABLE border="0" cellpadding="0" cellspacing="0" bgcolor="#0000ff">
        <TBODY>
          <TR>
            <TD>
            <TABLE border="0" width="300" cellpadding="2" cellspacing="2">
        <TBODY>
                <TR bgcolor="#ffffff">
            <TD><INPUT type="checkbox" name="a_1" value="1" $check_1><FONT size="-1">リンク元</FONT></TD>
            <TD><INPUT type="checkbox" name="a_2" value="1" $check_2><FONT size="-1">キーワード</FONT></TD>
            <TD><INPUT type="checkbox" name="a_3" value="1" $check_3><FONT size="-1">OS種類</FONT></TD>
          </TR>
                <TR bgcolor="#ffffff">
            <TD><INPUT type="checkbox" name="a_4" value="1" $check_4><FONT size="-1">ブラウザー種類</FONT></TD>
            <TD><INPUT type="checkbox" name="a_5" value="1" $check_5><FONT size="-1">プロバイダー</FONT></TD>
            <TD><INPUT type="checkbox" name="a_6" value="1" $check_6><FONT size="-1">プロキシ</FONT></TD>
          </TR>
                <TR bgcolor="#ffffff">
            <TD colspan="3" align="center">
		<INPUT type="submit" value="\表\示">　　
		<INPUT type="button" value="全チェック"  OnClick="autoon()">　　
		<INPUT type="button" value="リセット"  OnClick="autoclear()">
		</TD>
          </TR>
              </TBODY>
      </TABLE>
      </TD>
          </TR>
        </TBODY>
      </TABLE>
            </FORM>
	</TD>
    </TR>
    <TR>
      <TD>
      	<FORM action="$script" method="$method">
	<INPUT type=hidden name="mode" value="caren">
	<INPUT type=hidden name="y" value="$y">
	<INPUT type=hidden name="m" value="$m">
	<INPUT type=hidden name="name" value="$name">
	<INPUT type="submit" value="$FORM{'m'}月の一覧に戻る">
	</FORM>
      </TD>
      <TD>
	<FORM action="$script" method="$method">
	<INPUT type=hidden name="mode" value="day">
	<INPUT type=hidden name="y" value="$y_l">
	<INPUT type=hidden name="m" value="$m_l">
	<INPUT type=hidden name="d" value="$d_l">
	<INPUT type=hidden name="name" value="$name">
	<INPUT type="submit" value="前日">
	</FORM>
      </TD>
      <TD>
	<FORM action="$script" method="$method">
	<INPUT type=hidden name="mode" value="day">
	<INPUT type=hidden name="y" value="$y_n">
	<INPUT type=hidden name="m" value="$m_n">
	<INPUT type=hidden name="d" value="$d_n">
	<INPUT type=hidden name="name" value="$name">
	<INPUT type="submit" value="次日">
	</FORM>
      </TD>
    </TR>
  </TBODY>
</TABLE>



<BR>
<TABLE border="0" width="600" cellpadding="0" cellspacing="0">
  <TBODY>
    <TR>
      <TD><B>時間毎のカウンター数</B><BR>
      <BR>
      <TABLE border="0" cellpadding="0" cellspacing="0" bgcolor="#0000ff">
        <TBODY>
          <TR>
            <TD>
            <TABLE border="0">
              <TBODY>
                <TR bgcolor="#ffffff">
ALPHA
#時間毎

$time_max = "";
foreach $TIME (0 .. 23) {
	$TIME = sprintf("%02d",$TIME);
	if ($time_max <= $TIME{$TIME}) { $time_max = $TIME{$TIME}; }
		}

foreach $TIME (0 .. 23) {
	$TIME = sprintf("%02d",$TIME);
	$cun_ti = int($TIME{$TIME}/$time_max*100);
	if ($cun_ti <= 1) { $cun_ti = 1; }
	if ($TIME{$TIME} eq "") {
			$TIME{$TIME} = 0;
			$cun_time = "<FONT size=\"-1\">$TIME{$TIME}</FONT>";
				} else {
	$cun_time = "<FONT size=\"-1\">$TIME{$TIME}</FONT><BR><IMG src=\"./image/ao.gif\" width=\"10\" height=\"$cun_ti\" border=\"0\">";
				}
	print "		<TD width=\"20\" height=\"110\" align=\"center\" valign=\"bottom\">$cun_time</TD>\n";
			}

print <<"ALPHA";
              </TR>
                <TR bgcolor="#ffffff">
ALPHA

	for ($i=0; $i<=23; ++$i) {
		print "                  <TD width=\"20\" align=\"center\">$i</TD>\n";
			}

print <<"ALPHA";
                </TR>
              </TBODY>
            </TABLE>
          </TD>
         </TR>
        </TBODY>
       </TABLE>
      <BR>
      <HR>
      </TD>
    </TR>
  </TBODY>
</TABLE>
ALPHA



if ($a_1 == 1) {
print <<"ALPHA";
<BR><BR>
<TABLE border="0" width="600" cellpadding="0" cellspacing="0">
  <TBODY>
    <TR>
      <TD><B>リンク元</B><BR>
      <BR>
ALPHA

if ($r != 0) {
print <<"ALPHA";
<TABLE border="0" cellpadding="0" cellspacing="0" bgcolor="#0000ff">
  <TBODY>
    <TR>
      <TD>
      <TABLE border="0" width="600">
        <TBODY>
ALPHA

	$num = "";
	$cun_a = "";
	$cun_max = "";
	foreach $REFERER (sort { $REFERER{$b} <=> $REFERER{$a} } keys %REFERER) {
		++$num;
	if ($num <= $hk) {
		$amari = $num % 2;
		if ($amari ==1) { $bg = "#ffffff"; } else { $bg = "#cccccc"; }
		print "          <TR bgcolor=$bg>";
		if ($num == 1) { $cun_max = $REFERER{$REFERER} ;}
		if ($REFERER{$REFERER} != $cun_a) {
				print "<TD width=\"25\" align=\"right\">$num</TD>";
					} else {
				print "<TD>&nbsp;</TD>";
					}
		$cun_a = $REFERER{$REFERER};
		if (length($REFERER) > 60) { $url = substr($REFERER,0,57) . "..."; } else { $url = $REFERER; }
		print "<TD width=\"440\"><A href=\"$REFERER\" target=\"_blank\" title=\"$REFERER\">$url</A></TD>";
		$cun_re = int($REFERER{$REFERER}/$cun_max*100);
		if ($cun_re <= 1) { $cun_re = 1; }
		print "<TD width=\"130\" nowrap><IMG src=\"image/ao.gif\" width=\"$cun_re\" height=\"10\" border=\"0\"><FONT size=\"-1\"> $REFERER{$REFERER}</FONT></TD></TR>\n";
		}
	}

print <<"ALPHA";
        </TBODY>
      </TABLE>
      </TD>
    </TR>
  </TBODY>
</TABLE>
      Totle $num<BR>
ALPHA

} else { 

print <<"ALPHA"
	<BR>
	データーが有りません。<BR>
	<BR>
ALPHA
	}

print <<"ALPHA";
      <HR>
      </TD>
    </TR>
  </TBODY>
</TABLE>
ALPHA
}



if ($a_2 == 1) {
print <<"ALPHA";
<BR><BR>
<TABLE border="0" width="600" cellpadding="0" cellspacing="0">
  <TBODY>
    <TR>
      <TD><B>キーワード</B><BR>
      <BR>
ALPHA

if ($wo != 0) {
print <<"ALPHA";
    <TR>
      <TD>
<TABLE border="0" cellpadding="0" cellspacing="0" bgcolor="#0000ff">
  <TBODY>
    <TR>
      <TD>
      <TABLE border="0" width="600">
        <TBODY>
ALPHA

	$num = "";
	$cun_a = "";
	$cun_max = "";
	foreach $WORD (sort { $WORD{$b} <=> $WORD{$a} } keys %WORD) {
		++$num;
	$word_cun = $WORD{$WORD}/2;
	if ($num <= $hk) {
		$amari = $num % 2;
		if ($amari ==1) { $bg = "#ffffff"; } else { $bg = "#cccccc"; }
		print "          <TR bgcolor=$bg>";
		if ($num == 1) { $cun_max = $word_cun; }
		if ($word_cun != $cun_a) {
				print "<TD width=\"25\" align=\"right\">$num</TD>";
					} else {
				print "<TD>&nbsp;</TD>";
					}
		$cun_a = $word_cun;
		print "<TD width=\"440\">$WORD</TD>";
		$cun_ke = int($word_cun/$cun_max*100);
		if ($cun_ke <= 1) { $cun_ke = 1; }
		print "<TD width=\"130\" nowrap><IMG src=\"image/ao.gif\" width=\"$cun_ke\" height=\"10\" border=\"0\"><FONT size=\"-1\"> $word_cun</FONT></TD></TR>\n";
		}
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
    <TR>
      <TD>Totle $num<BR>
ALPHA

} else { 

print <<"ALPHA"
	<BR>
	データーが有りません。<BR>
	<BR>
ALPHA
	}

print <<"ALPHA";
      <HR>
      </TD>
    </TR>
  </TBODY>
</TABLE>
ALPHA
}



if ($a_3 == 1) {
print <<"ALPHA";
<BR><BR>
<TABLE border="0" width="600" cellpadding="0" cellspacing="0">
  <TBODY>
    <TR>
      <TD><B>OS種類</B><BR>
      <BR>
ALPHA

if ($o != 0) {
print <<"ALPHA";
    <TR>
      <TD>
<TABLE border="0" cellpadding="0" cellspacing="0" bgcolor="#0000ff">
  <TBODY>
    <TR>
      <TD>
      <TABLE border="0" width="600">
        <TBODY>
ALPHA

	$num = "";
	$cun_a = "";
	$cun_max = "";
	foreach $OS (sort { $OS{$b} <=> $OS{$a} } keys %OS) {
		++$num;
	if ($num <= $hk) {
		$amari = $num % 2;
		if ($amari ==1) { $bg = "#ffffff"; } else { $bg = "#cccccc"; }
		print "          <TR bgcolor=$bg>";
		if ($num == 1) { $cun_max = $OS{$OS} ;}
		if ($OS{$OS} != $cun_a) {
				print "<TD width=\"25\" align=\"right\">$num</TD>";
					} else {
				print "<TD>&nbsp;</TD>";
					}
		$cun_a = $OS{$OS};
		print "<TD width=\"440\">$OS</TD>";
		$cun_os = int($OS{$OS}/$cun_max*100);
		if ($cun_os <= 1) { $cun_os = 1; }
		print "<TD width=\"130\" nowrap><IMG src=\"image/ao.gif\" width=\"$cun_os\" height=\"10\" border=\"0\"><FONT size=\"-1\"> $OS{$OS}</FONT></TD></TR>\n";
		}
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
    <TR>
      <TD>Totle $num<BR>
ALPHA

} else { 

print <<"ALPHA"
	<BR>
	データーが有りません。<BR>
	<BR>
ALPHA
	}

print <<"ALPHA";
      <HR>
      </TD>
    </TR>
  </TBODY>
</TABLE>
ALPHA
}



if ($a_4 == 1) {
print <<"ALPHA";
<BR><BR>
<TABLE border="0" width="600" cellpadding="0" cellspacing="0">
  <TBODY>
    <TR>
      <TD><B>ブラウザー種類</B><BR>
      <BR>
ALPHA

if ($b != 0) {
print <<"ALPHA";
    <TR>
      <TD>
<TABLE border="0" cellpadding="0" cellspacing="0" bgcolor="#0000ff">
  <TBODY>
    <TR>
      <TD>
      <TABLE border="0" width="600">
        <TBODY>
ALPHA

	$num = "";
	$cun_a = "";
	$cun_max = "";
	foreach $BR (sort { $BR{$b} <=> $BR{$a} } keys %BR) {
		++$num;
	if ($num <= $hk) {
		$amari = $num % 2;
		if ($amari ==1) { $bg = "#ffffff"; } else { $bg = "#cccccc"; }
		print "          <TR bgcolor=$bg>";
		if ($num == 1) { $cun_max = $BR{$BR} ;}
		if ($BR{$BR} != $cun_a) {
				print "<TD width=\"25\" align=\"right\">$num</TD>";
					} else {
				print "<TD>&nbsp;</TD>";
					}
		$cun_a = $BR{$BR};
		print "<TD width=\"440\">$BR</TD>";
		$cun_br = int($BR{$BR}/$cun_max*100);
		if ($cun_br <= 1) { $cun_br = 1; }
		print "<TD width=\"130\" nowrap><IMG src=\"image/ao.gif\" width=\"$cun_br\" height=\"10\" border=\"0\"><FONT size=\"-1\"> $BR{$BR}</FONT></TD></TR>\n";
		}
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
    <TR>
      <TD>Totle $num<BR>
ALPHA

} else { 

print <<"ALPHA"
	<BR>
	データーが有りません。<BR>
	<BR>
ALPHA
	}

print <<"ALPHA";
      <HR>
      </TD>
    </TR>
  </TBODY>
</TABLE>
ALPHA
}



if ($a_5 == 1) {
print <<"ALPHA";
<BR><BR>
<TABLE border="0" width="600" cellpadding="0" cellspacing="0">
  <TBODY>
    <TR>
      <TD><B>プロバイダー</B><BR>
      <BR>
ALPHA

if ($h != 0) {
print <<"ALPHA";
    <TR>
      <TD>
<TABLE border="0" cellpadding="0" cellspacing="0" bgcolor="#0000ff">
  <TBODY>
    <TR>
      <TD>
      <TABLE border="0" width="600">
        <TBODY>
ALPHA

	$num = "";
	$cun_a = "";
	$cun_max = "";
	foreach $HOST (sort { $HOST{$b} <=> $HOST{$a} } keys %HOST) {
		++$num;
	if ($num <= $hk) {
		$amari = $num % 2;
		if ($amari ==1) { $bg = "#ffffff"; } else { $bg = "#cccccc"; }
		print "          <TR bgcolor=$bg>";
		if ($num == 1) { $cun_max = $HOST{$HOST} ;}
		if ($HOST{$HOST} != $cun_a) {
				print "<TD width=\"25\" align=\"right\">$num</TD>";
				$up = $num;
					} else {
				print "<TD>&nbsp;</TD>";
					}
		$cun_a = $HOST{$HOST};
		print "<TD width=\"440\">$HOST</TD>";
		$cun_ho = int($HOST{$HOST}/$cun_max*100);
		if ($cun_ho <= 1) { $cun_ho = 1; }
		print "<TD width=\"130\" nowrap><IMG src=\"image/ao.gif\" width=\"$cun_ho\" height=\"10\" border=\"0\"><FONT size=\"-1\"> $HOST{$HOST}</FONT></TD></TR>\n";
		}
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
    <TR>
      <TD>Totle $num<BR>
ALPHA

} else { 

print <<"ALPHA"
	<BR>
	データーが有りません。<BR>
	<BR>
ALPHA
	}

print <<"ALPHA";
      <HR>
      </TD>
    </TR>
  </TBODY>
</TABLE>
ALPHA
}



if ($a_6 == 1) {
print <<"ALPHA";
<BR><BR>
<TABLE border="0" width="600" cellpadding="0" cellspacing="0">
  <TBODY>
    <TR>
      <TD><B>プロキシ</B><BR>
      <BR>
ALPHA

if ($v != 0) {
print <<"ALPHA";
    <TR>
      <TD>
<TABLE border="0" cellpadding="0" cellspacing="0" bgcolor="#0000ff">
  <TBODY>
    <TR>
      <TD>
      <TABLE border="0" width="600">
        <TBODY>
ALPHA

	$num = "";
	$cun_a = "";
	$cun_max = "";
	foreach $VIA (sort { $VIA{$b} <=> $VIA{$a} } keys %VIA) {
		++$num;
	if ($num <= $hk) {
		$amari = $num % 2;
		if ($amari ==1) { $bg = "#ffffff"; } else { $bg = "#cccccc"; }
		print "          <TR bgcolor=$bg>";
		if ($num == 1) { $cun_max = $VIA{$VIA} ;}
		if ($VIA{$VIA} != $cun_a) {
				print "<TD width=\"25\" align=\"right\">$num</TD>";
					} else {
				print "<TD>&nbsp;</TD>";
					}
		$cun_a = $VIA{$VIA};
		print "<TD width=\"440\">$VIA</TD>";
		$cun_via = int($VIA{$VIA}/$cun_max*100);
		if ($cun_via <= 1) { $cun_via = 1; }
		print "<TD width=\"130\" nowrap><IMG src=\"image/ao.gif\" width=\"$cun_via\" height=\"10\" border=\"0\"><FONT size=\"-1\"> $VIA{$VIA}</FONT></TD></TR>\n";
		}
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
    <TR>
      <TD>Totle $num<BR>
ALPHA

} else { 

print <<"ALPHA"
	<BR>
	データーが有りません。<BR>
	<BR>
ALPHA
	}

print <<"ALPHA";
      <HR>
      </TD>
    </TR>
  </TBODY>
</TABLE>
ALPHA
}



print <<"ALPHA";
</CENTER>
</BODY>
</HTML>
ALPHA


exit;

}

sub not_date {
print "Content-type: text/html\n\n";
print <<"ALPHA";
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=Shift_JIS">
<META http-equiv="Content-Style-Type" content="text/css">
<TITLE>$y年$FORM{'m'}月$FORM{'d'}日</TITLE>
</HEAD>
<BODY>
<BR>
<BR>
<CENTER>
<TABLE border="0" width="400">
  <TBODY>
    <TR>
      <TD colspan="3"><FONT size="+1"><B>$y年$FORM{'m'}月$FORM{'d'}日の、データーは有りません。</B></FONT></TD>
    </TR>
    <TR>
      <TD width="200">
      	<FORM action="$script" method="$method">
	<INPUT type=hidden name="mode" value="caren">
	<INPUT type=hidden name="y" value="$y">
	<INPUT type=hidden name="m" value="$m">
	<INPUT type=hidden name="name" value="$name">
	<INPUT type="submit" value="$FORM{'m'}月の一覧に戻る">
	</FORM>
	</TD>
      <TD width="100">
	<FORM action="$script" method="$method">
	<INPUT type=hidden name="mode" value="day">
	<INPUT type=hidden name="y" value="$y_l">
	<INPUT type=hidden name="m" value="$m_l">
	<INPUT type=hidden name="d" value="$d_l">
	<INPUT type=hidden name="name" value="$name">
	<INPUT type="submit" value="前日">
	</FORM>
      </TD>
      <TD width="100">
	<FORM action="$script" method="$method">
	<INPUT type=hidden name="mode" value="day">
	<INPUT type=hidden name="y" value="$y_n">
	<INPUT type=hidden name="m" value="$m_n">
	<INPUT type=hidden name="d" value="$d_n">
	<INPUT type=hidden name="name" value="$name">
	<INPUT type="submit" value="次日">
	</FORM>
      </TD>
    </TR>
  </TBODY>
</TABLE>
</CENTER>
</BODY>
</HTML>
ALPHA

exit;

}

##################################################
##--------------- カウンター処理 ---------------##
##################################################
sub count {

#総数
	open(IN,"$logfile_a"); # || &error();
	$line_a = <IN>;
	close(IN);

##	($xx,$cun_a) = split(/\,/, $line_a);
	($xx,$cun_a) = explode(/\,/, $line_a);
	if ($name eq "") {
		$cun_a = $cun_a + $cunt_f;
			} else {
		$namepl = './name.pl';
		if (-e $namepl) { require $namepl; }
		&name_cun;
		$cun_a = $cun_a + $cunt_f2;
		}

#昨日、今日

	$cun_t = "";
	if (-e $logfile_t) {
		open(IN,"$logfile_t");
		@line_t = <IN>;
		close(IN);
	$cun_t = @line_t;
		} else {
	$cunt_t = 0;
		}

	if (-e $logfile_y) {
		open(IN,"$logfile_y");
		@line_y = <IN>;
		close(IN);
	$cun_y = @line_y;
		} else {
	$cun_y = 0;
		}

#月ごとのカウント数

#前月
	$m_l = $m-1;
	if ($m_l < 1) { $year_l = $y-1; $m_l = 12;} else { $year_l = $y; }
		$m_l = sprintf("%02d",$m_l); 

	if ($name eq "") {
		$logfile_ml = "./log/log_/$year_l/$m_l/$year_l$m_l.dat";
			} else {
		$logfile_ml = "./log/log_$name/$year_l/$m_l/$year_l$m_l.dat";
			}

		open(IN,"$logfile_ml");
		@line_ml = <IN>;
		close(IN);

	$cun_ml = "";
	foreach $lines_ml(@line_ml) {
##		($days,$cun) = split(/\,/, $lines_ml);
		($days,$cun) = explode(/\,/, $lines_ml);
		$cun_ml = $cun_ml + $cun;
			}

#当月
$m = sprintf("%02d",$m); 
$d = sprintf("%02d",$d); 
	if ($name eq "") {
		$logfile_mt = "./log/log_/$y/$m/$y$m.dat";
			} else {
		$logfile_mt = "./log/log_$name/$y/$m/$y$m.dat";
			}

		open(IN,"$logfile_mt");
		@line_mt = <IN>;
		close(IN);

	$cun_mt = "";
	foreach $lines_mt(@line_mt) {
##		($days,$cun) = split(/\,/, $lines_mt);
		($days,$cun) = explode(/\,/, $lines_mt);
		$cun_mt = $cun_mt + $cun;
			}
	if ($cun_mt eq "") { $cun_mt = "--"; }

#後月
	$m_n = $m+1;
	if ($m_n > 12) { $y_n = $y+1; $m_n = 1;} else { $y_n = $y; }
	$m_n = sprintf("%02d",$m_n); 

	if ($name eq "") {
		$logfile_mn = "./log/log_/$y_n/$m_n/$y_n$m_n.dat";
			} else {
		$logfile_mn = "./log/log_$name/$y_n/$m_n/$y_n$m_n.dat";
			}

		open(IN,"$logfile_mn");
		@line_mn = <IN>;
		close(IN);

	$cun_mn = "";
	foreach $lines_mn(@line_mn) {
##		($days,$cun) = split(/\,/, $lines_mn);
		($days,$cun) = explode(/\,/, $lines_mn);
		$cun_mn = $cun_mn + $cun;
			}

}

######################################################
##-------------------- 時間取得 --------------------##
######################################################
sub get_time {

$ENV{'TZ'} = "JST-9"; 
($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);
$year = 1900 + $year;
$mon++;
$mon = sprintf("%02d",$mon); 
$mday = sprintf("%02d",$mday); 
$hour = sprintf("%02d",$hour); 
$min = sprintf("%02d",$min); 
$sec = sprintf("%02d",$sec); 
$week = ('Sun','Mon','Tue','Wed','Thu','Fri','Sat') [$wday];

#前日の日付
$year_y = $year;
$mday_y = $mday-1;
if ($mday == 01) { $mon_y = $mon-1; } else { $mon_y = $mon+0; }
if ($mon_y < 1) { $year_y = $year_y-1; $mon_y = 12;}
if ($mday_y < 1) { $mday_y = $MON[$mon_y];}
$mon_y = sprintf("%02d",$mon_y); 
$mday_y = sprintf("%02d",$mday_y); 
}

sub week {

	if ($m == 1 || $m == 2){
		$y--;
		$m += 12;
	}
	if ($FORM{'d'} ne "") {$d = $FORM{'d'}; } else { $d = 1; }
	$w = ($y + int($y/4) - int($y/100) + int($y/400) + int(2.6*$m+1.6) + $d) % 7;
	if ($m == 13) { $y = $y+1; $m = 1;}
	if ($m == 14) { $y = $y+1; $m = 2;}

}

#デコート-------------------------------
sub decode {
	# プラウザからのデータ取込み
	if ($ENV{'REQUEST_METHOD'} eq "POST") { read(STDIN, $buffer, $ENV{'CONTENT_LENGTH'}); }
	else { $buffer = $ENV{'QUERY_STRING'}; }

	# プラウザからのデータ変換
##	@pairs = split(/&/,$buffer);
	@pairs = explode(/&/,$buffer);
	foreach $pair (@pairs) {
		#１行毎に$name,$valueを取り出す
##		($name, $value) = split(/=/, $pair);
        ($name, $value) = explode(/=/, $pair);
		# 変換演算子　tr　+　を　スペースに置き換え
		$value =~ tr/+/ /;
		# 変換演算子　s/// 単語の構成文字にマッチ
		$value =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
		#\n を "" に変換
		$value =~ s/\n//g;
		# 日本語に変換
		&jcode'convert(*value,'sjis');
		&jcode'convert(*name,'sjis');
		$FORM{$name} = $value;
	}
	$mode		= $FORM{'mode'};
	$name		= $FORM{'name'};

	if ($ENV{'HTTP_USER_AGENT'} =~ /UP\.Browser\//) {
			$method='GET';
		}elsif($ENV{'HTTP_USER_AGENT'} =~ /PDXGW\//){
			$method='GET';
		}elsif($ENV{'HTTP_USER_AGENT'} =~ /DoCoMo\//i){
			$method='POST';
		}else{
			$method='POST';
		}

}

#エラー処理-----------------------------
sub error {

print "Content-type: text/html\n\n";

print <<"ALPHA";
<HTML>
<HEAD>
<TITLE>エラー</TITLE>
</HEAD>
<BODY>
<CENTER>
エラーです！！<BR>
<FONT color="#ff0000">$_[0]</FONT><BR>
<BR>
入力画面に戻りもう一度良くご確認ください。
</CENTER>
</BODY>
</HTML>
ALPHA

exit;

}

