#!/usr/local/bin/perl
#
#################################################
#	アクセス解析（パスワード製作）		#
#	Ver.1_0 (00/07/2001)			#
#	（株）アルファテック			#
#		製作：大河原　康史		#
#	email:ookawara@alphatec.co.jp		#
#	HP:http://www.alphatec.co.jp/		#
#################################################


$l = 1;
require './kanri.pl';	#管理用データーファイル

#メイン--------------------------------
&decode;
if ($mode eq "top") { &top; }
if ($mode eq "end") { &end; }

&main;

exit;

#初期画面-------------------------------
sub main {

	opendir (DIR,".");
	@list = readdir (DIR);
		$flag = 0;
		foreach (@list) {
			if ($_ eq "tec.pl") { $flag = 1; last;}
			}

	if ($flag != 0) { &home;}

print "Content-type: text/html\n\n";

print <<"ALPHA";
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=Shift_JIS">
<META http-equiv="Content-Style-Type" content="text/css">
<TITLE>アクセス解析閲覧パースワード製作</TITLE>
</HEAD>
<BODY bgcolor="$bgcolor">
<CENTER>
<BR>
<BR>
<TABLE border="0" cellpadding="0" cellspacing="0">
  <TBODY>
    <TR>
      <TD>アクセス解析閲覧するためのパースワードを決め入力して下さい。</TD>
    </TR>
    <TR>
      <TD align="right"><FONT size="-1">注：半角英数字6文字以上</FONT></TD>
    </TR>
    <TR>
      <TD><BR>
      </TD>
    </TR>
    <TR>
      <TD>
      <CENTER>
	<FORM action="$script3" method="$method">
	<INPUT type=hidden name="mode" value="top">
	<INPUT size="20" type="text" name="pass_key">&nbsp;&nbsp;
	<INPUT type="submit" value="送信">&nbsp;&nbsp;&nbsp;&nbsp;
	<INPUT type="reset">
	</FORM>
</CENTER>
</TD>
    </TR>
  </TBODY>
</TABLE>
</BODY>
</HTML>
ALPHA

exit;

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
<TITLE>変更済み！</TITLE>
</HEAD>
<BODY bgcolor="$bgcolor">
<CENTER>
<BR>
<BR>
<FONT color="#ff0000">パスワード変更済み！</FONT>
</CENTER>
</BODY>
</HTML>
ALPHA

exit;

}

#設定登録処理-------------------------------
sub top {


if ($FORM{'pass_key'} =~ /\W/ || $FORM{'pass_key'} eq "") {&error("パスワードが不適切です。");}
if (length($FORM{'pass_key'}) <= 6) {&error("パスワードが短すぎます。");}

	$pass = crypt ($FORM{'pass_key'},$salt);

	open(OUT,">tec.pl") || &error("Can't write File");
	print OUT $pass;
	close(OUT);

print "Content-type: text/html\n\n";

print <<"ALPHA";
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=Shift_JIS">
<META http-equiv="Content-Style-Type" content="text/css">
<META http-equiv="Refresh" content="1; url=./$script3?mode=end";>
<TITLE>変更終了</TITLE>
</HEAD>
<BODY bgcolor="$bgcolor">
<CENTER>
<BR>
<BR>
無事にパスワードは変更されました。
</CENTER>
</BODY>
</HTML>
ALPHA

exit;

}

#終了画面-------------------------------
sub end {

print "Content-type: text/html\n\n";

print <<"ALPHA";
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=Shift_JIS">
<META http-equiv="Content-Style-Type" content="text/css">
<TITLE>変更終了</TITLE>
</HEAD>
<BODY bgcolor="$bgcolor">
<CENTER>
<BR>
<BR>
無事にパスワードは変更されました。
</CENTER>
</BODY>
</HTML>
ALPHA

exit;

}

#デコート-------------------------------
sub decode {
	# プラウザからのデータ取込み
	if ($ENV{'REQUEST_METHOD'} eq "POST") { read(STDIN, $buffer, $ENV{'CONTENT_LENGTH'}); }
	else { $buffer = $ENV{'QUERY_STRING'}; }

	# プラウザからのデータ変換
##	@pairs = split(/&/,$buffer);
	@pairs =explode(/&/,$buffer);
	foreach $pair (@pairs) {
		#１行毎に$name,$valueを取り出す
##		($name, $value) = split(/=/, $pair);
		($name, $value) = explode(/=/, $pair);
		# 変換演算子　tr　+　を　スペースに置き換え
		$value =~ tr/+/ /;
		# 変換演算子　s/// 単語の構成文字にマッチ
		$value =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
		$FORM{$name} = $value;
	}

	$mode	= $FORM{'mode'};

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
<BODY bgcolor="$bgcolor">
<CENTER>
<BR>
<BR>
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

