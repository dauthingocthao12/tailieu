#!/usr/local/bin/perl
#
#################################################
#	アクセス解析付きカウンター		#
#	Ver.1_0 (00/07/2001)			#
#	（株）アルファテック			#
#		製作：大河原　康史		#
#	email:ookawara@alphatec.co.jp		#
#	HP:http://www.alphatec.co.jp/		#
#################################################


$l = 1;
require './jcode.pl';
require './kanri.pl';
require './os.pl';
require './br.pl';
require './robo.pl';

#-------初期設定-------
&decode;

&get_time;

	$dir_a = "./log";
	unless (-e $dir_a) {
		mkdir ("$dir_a",0777);
		chmod (0777,"$dir_a");
			}

if ($name eq "") {
	$dir = "./log/log_";
	unless (-e $dir) {
		mkdir ("$dir",0777);
		chmod (0777,"$dir");
			}
	$logfile_a = "./log/log_/count.dat";			#カウンター総数
	$logfile_m = "./log/log_/$year/$mon/$year$mon.dat";
	$logfile_t = "./log/log_/$year/$mon/$mday.dat";
	$logfile_y = "./log/log_/$year_y/$mon_y/$mday_y.dat";
	$logchesk = "./log/log_/check.dat";
	$dir_year = "./log/log_/$year";
	$dir_mon = "./log/log_/$year/$mon";
		} else {
	$dir = "./log/log_$name";
	unless (-e $dir) {
		mkdir ("$dir",0777);
		chmod (0777,"$dir");
			}
	$logfile_a = "./log/log_$name/count.dat";			#カウンター総数
	$logfile_m = "./log/log_$name/$year/$mon/$year$mon.dat";
	$logfile_t = "./log/log_$name/$year/$mon/$mday.dat";
	$logfile_y = "./log/log_$name/$year_y/$mon_y/$mday_y.dat";
	$logchesk = "./log/log_$name/check.dat";
	$dir_year = "./log/log_$name/$year";
	$dir_mon = "./log/log_$name/$year/$mon";
		}

if ($mode eq "counter") { &counter; }
if ($mode eq "today") { &today; }
if ($mode eq "yesterday") { &yesterday; }



#メイン--------------------------------
&get_host;
&get_etc;

&robo;

#IPチェック
	foreach (@IP) {
		if ($addr eq $_ ) { &counter; exit; }
		}

&count_up;
&count;
&counter;

exit;

######################################################
##----------------- カウンター制御 -----------------##
######################################################

sub count_up {
if ($coun_up1 == 0) { &count; &counter;}
	elsif ($coun_up1 == 1) {

		if (-e $logfile_t) {
			open(IN,"$logfile_t");
			@line2 = <IN>;
			close(IN);
				}

		foreach $lines2 (@line2) {
##			($n,$n,$os_d,$br_d,$ip_d) = split(/\,/, $lines2);
			($n,$n,$os_d,$br_d,$ip_d) = explode(/\,/, $lines2);
			$ip = "$host \[ $addr \]";
			if (($ip eq $ip_d) && ($os eq $os_d) && ($br eq $br_d)) { &counter; }
				}
		}

	elsif ($coun_up1 == 2) {
	 	# ロックファイル名を定義
		$lockdir = "lock_check";

		# ロック開始
		if ($lockkey == 1) { &lock1; }
		elsif ($lockkey == 2) { &lock2; }

		if (-e $logchesk) {
		open(IN,"$logchesk");
		@line_log = <IN>;
		close(IN);
			}

		$flag = 0;
		foreach $lines_log (@line_log) {
##			($time_d,$os_d,$br_d,$ip_d) = split(/\,/, $lines_log);
			($time_d,$os_d,$br_d,$ip_d) = explode(/\,/, $lines_log);
##			($hour_d,$min_d) = split(/\:/, $time_d);
			($hour_d,$min_d) = explode(/\:/, $time_d);
			$hour_d = $hour_d + $count_up2;
			if (($hour <= $hour_d) && ($min >= $min_d)) {
				$ip = "$host \[ $addr \]";
				if (($ip eq $ip_d) && ($os eq $os_d) && ($br eq $br_d)) { $flag = 1; next;
									} else {
									 push(@list,$lines_log); }
							}
					}

		$log = "$hour:$min:$sec,$os,$br,$host \[ $addr \],\n";
		push (@list,$log);

		# 記録ファイルを更新する
		open(OUT,">$logchesk") || &error;
		eval 'flock(OUT,2);';
		print OUT @list;
		eval 'flock(OUT,8);';
		close(OUT);
		chmod (0666,"$logchesk");

		# ロック解除
		if (-e $lockdir) { rmdir($lockdir); }

		if ($flag == 1) { &counter; }
		}

	elsif ($coun_up1 == 3) {
	 	# ロックファイル名を定義
		$lockdir = "lock_check2";

		# ロック開始
		if ($lockkey == 1) { &lock1; }
		elsif ($lockkey == 2) { &lock2; }

		if (-e $logchesk) {
		open(IN,"$logchesk");
		$line_log = <IN>;
		close(IN);
			}
		$flag = 0;
##		($time_d,$os_d,$br_d,$ip_d) = split(/\,/, $line_log);
        ($time_d,$os_d,$br_d,$ip_d) = explode(/\,/, $line_log);
		$ip = "$host \[ $addr \]";
		if (($ip eq $ip_d) && ($os eq $os_d) && ($br eq $br_d)) { $flag = 1; }

		$log = "$hour:$min:$sec,$os,$br,$host \[ $addr \],\n";

		# 記録ファイルを更新する
		open(OUT,">$logchesk") || &error;
		eval 'flock(OUT,2);';
		print OUT $log;
		eval 'flock(OUT,8);';
		close(OUT);
		chmod (0666,"$logchesk");
	
		# ロック解除
		if (-e $lockdir) { rmdir($lockdir); }

		if ($flag == 1) { &counter; }
	}

}

#カウンター-------------------------------
sub count {
#総数カウンター

 	# ロックファイル名を定義
	$lockdir = "lock_all";

	# ロック開始
	if ($lockkey == 1) { &lock1; }
	elsif ($lockkey == 2) { &lock2; }

	# 記録ファイルから読み込み

	if (-e $logfile_a) {
	open(IN,"$logfile_a");
	$line_a = <IN>;
	close(IN);
		}

	# 記録ファイルを分解
	## ($daykey,$count) = split(/\,/, $line_a);
	($daykey,$count) = explode(/\,/, $line_a);

	# カウントアップ
	$count++;

	# ファイルをフォーマット
	$lines_a = "$mday,$count,\n";

	# 記録ファイルを更新する
	open(OUT,">$logfile_a") || &error;
	eval 'flock(OUT,2);';
	print OUT $lines_a;
	eval 'flock(OUT,8);';
	close(OUT);
	chmod (0666,"$logfile_a");

	# ロック解除
	if (-e $lockdir) { rmdir($lockdir); }

#日毎カウンター,月のファイルに日毎の数字を入れる

 	# ロックファイル名を定義
	$lockdir = "lock_day";

	# ロック開始
	if ($lockkey == 1) { &lock1; }
	elsif ($lockkey == 2) { &lock2; }

	# 記録ファイルから読み込み

	if (-e $logfile_m) {
	open(IN,"$logfile_m");
	@line2 = <IN>;
	close(IN);
		}

	unless (-e $dir_year) {
	mkdir ($dir_year,0777);
	chmod (0777,$dir_year);
		}
	unless (-e $dir_mon) {
	mkdir ($dir_mon,0777);
	chmod (0777,$dir_mon);
		}

	@LINE2 = ('');
	$flag = 0;
	foreach (@line2) {

		# 記録ファイルを分解
##		($daykey,$count2) = split(/\,/, $_);
		($daykey,$count2) = explode(/\,/, $_);

		if ($mday == $daykey) {
			# カウントアップ
			$count2++;
			$counts = "$mday,$count2,\n";
			push (@LINE2,$counts); $flag = 1;
				} else {
			push (@LINE2,$_);
			}
		}

		if ($flag != 1) {
			$counts = "";
			$daykey = $mday; $count2 =1;
			$counts = "$mday,$count2,\n";
			push (@LINE2,$counts)
			}

	# 記録ファイルを更新する
	open(OUT,">$logfile_m") || &error;
	eval 'flock(OUT,2);';
	print OUT @LINE2;
	eval 'flock(OUT,8);';
	close(OUT);
	chmod (0666,"$logfile_m");

	# ロック解除
	if (-e $lockdir) { rmdir($lockdir); }

#日毎に、細かな詳細データー記録

 	# ロックファイル名を定義
	$lockdir = "lock_day_s";

	# ロック開始
	if ($lockkey == 1) { &lock1; }
	elsif ($lockkey == 2) { &lock2; }

	$LINE3 = "$hour:$min:$sec,$referer,$os,$br,$host \[ $addr \],$via,$agent,$buffer_,$name,\n";

	# 記録ファイルを更新する
	open(OUT,">>$logfile_t") || &error;
	eval 'flock(OUT,2);';
	print OUT $LINE3;
	eval 'flock(OUT,8);';
	close(OUT);
	chmod (0666,"$logfile_t");

	# ロック解除
	if (-e $lockdir) { rmdir($lockdir); }

}

## ロックファイル（symlink関数）処理サブルーチン
sub lock1 {
	local($retry) = 5;
	while (!symlink(".", $lockfile)) {
		if (--$retry <= 0) { &error; }
		sleep(1);
	}
}

## ロックファイル（open関数）処理サブルーチン
sub lock2 {
	local($flag) = 0;
	foreach (1 .. 5) {
		unless (-e $lockdir) {
			mkdir ("$lockdir",0777);
			chmod (0777,"$lockdir");
			$flag = 1;
			last;
		} else {
			sleep(1);
		}
	}
	if ($flag == 0) { &error; }
}

######################################################
##----------------- カウンター表示 -----------------##
######################################################
sub counter {

if (($FORM{'h'} eq "") || ($FORM{'h'} == "0")) {

		if (-e $logfile_a) {
		open(IN,"$logfile_a");
		$line_a = <IN>;
		close(IN);
			}

		# 記録ファイルを分解
##		($daykey,$count) = split(/\,/, $line_a);
($daykey,$count) = explode(/\,/, $line_a);
		if ($name eq "") {
			$count = $count + $cunt_f;
				} else {
			$namepl = './name.pl';
			if (-e $namepl) {
				require $namepl;
				&name_cun;
				}
			$count = $count + $cunt_f2;
			}

		$count = sprintf(sprintf("%%0%dld", $countk), $count);

&count_look;

	} else {
		printf("Content-type: text/html\n"); printf("\n");
	}

exit;

}

sub today {

	$cun_t = "";
	if (-e $logfile_t) {
		open(IN,"$logfile_t");
		@line_t = <IN>;
		close(IN);
	$count = @line_t;
		} else {
	$count = 0;
		}

&count_look;

exit;

}

sub yesterday {

	if (-e $logfile_y) {
		open(IN,"$logfile_y");
		@line_y = <IN>;
		close(IN);
	$count = @line_y;
		} else {
	$count = 0;
		}

&count_look;

exit;

}

sub count_look {

		if (($counth == 1) && ($ssi == 0)) {
			print "Content-type: text/html\n\n";
			print $count;
				}
		elsif (($counth == 0) && ($ssi == 0)) {
			print "Content-type: text/html\n\n";
			foreach (0..length($count)-1) {
				$img = substr($count,$_,1);
				print "<img src=\"$cun_image/$img.gif\" alt=\"$img\" border=\"0\">"; 
						}
				}
		else {
			require './gifcat.pl';
			printf("Content-type: image/gif\n");
			printf("\n");
			@files = ();
			foreach (0..length($count)-1) {
				$img = substr($count,$_,1);
				push(@files, "$cun_image2/$img.gif");
						}
			binmode(STDOUT);
			print &gifcat'gifcat(@files);
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

$date = "$year$mon$mday$hour$min$sec";

#前日の日付
$year_y = $year;
$mday_y = $mday-1;
if ($mday == 01) { $mon_y = $mon-1; } else { $mon_y = $mon+0; }
if ($mon_y < 1) { $year_y = $year_y-1; $mon_y = 12;}
if ($mday_y < 1) { $mday_y = $MON[$mon_y];}
$mon_y = sprintf("%02d",$mon_y); 
$mday_y = sprintf("%02d",$mday_y); 
}

######################################################
##------------------- IPチェック -------------------##
######################################################
sub get_host {

$addr  = $ENV{'REMOTE_ADDR'};
## $host = gethostbyaddr(pack("C4",split(/\./,$addr)),2);
$host = gethostbyaddr(pack("C4",explode(/\./,$addr)),2);
if (!$host){$host = $addr;}

$via = $ENV{'HTTP_VIA'};
#$xfor = $ENV{'HTTP_X_FORWARDED_FOR'};
#$for = $ENV{'HTTP_FORWARDED'};
#$xfor_name = gethostbyaddr(pack("C4",split(/\./,$xfor)),2);
#if (!$xfor_name){$xfor_name = $for;}

}

######################################################
##--------------------- その他 ---------------------##
######################################################
sub get_etc {

if ($ssi == 0) { $buffer = $ENV{'HTTP_REFERER'}; 
        	$buffer =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
			}
elsif ($ssi == 1) {
		$buffer =~ s/&name=top//g;
		$buffer =~ s/ref=//g;
        	$buffer =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
        	$buffer =~ tr/+/ /;
        	$buffer =~ s/&/&amp;/g;
        	$buffer =~ s/"/&quot;/g;
        	$buffer =~ s/</&lt;/g;
        	$buffer =~ s/>/&gt;/g;
        	$buffer =~ tr/+/ /;

        } #リンク元

		$a1t = '[0-9]';
		## &jcode::convert(*a1t,'euc');
		&jcode::convert(*a1t,'utf8');
		&jcode::tr(\$buffer,'[０１２３４５６７８９]',$a1t); 
		$a2t = '[A-Z]';
		## &jcode::convert(*a2t,'euc');
		&jcode::convert(*a2t,'utf8');
		&jcode::tr(\$buffer,'[ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ]',$a2t); 
		$a3t = '[a-z]';
		## &jcode::convert(*a3t,'euc');
		&jcode::convert(*a3t,'utf8');
		&jcode::tr(\$buffer,'[ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ]',$a3t); 
		$a4t = '-';
		## &jcode'convert(*a4t,'euc');
		&jcode'convert(*a4t,'utf8');
		&jcode::tr(\$buffer,'－',$a4t); 

	&jcode::convert(*buffer,'sjis');

		$buffer =~ s/\,/，/g; 
		$buffer =~ s/　/ /g; 
		$buffer =~ s/  / /g; 

		$referer = "$buffer";


$agent = $ENV{'HTTP_USER_AGENT'};	#ブラウザーチェック
$agent =~ s/\,/，/g; 
&os;
&br;

}

######################################################
##-------------------- デコード --------------------##
######################################################
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
		# " を &quot; に変換
		$value =~ s/"/&quot;/g;
		#\n を "" に変換
		$value =~ s/\n//g;
		$FORM{$name} = $value;
	}
	$mode	= $FORM{'mode'};
	$name	= $FORM{'name'};

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
$_[0]<BR>
<BR>
入力画面に戻りもう一度良くご確認ください。
</CENTER>
</BODY>
</HTML>
ALPHA

exit;

}

