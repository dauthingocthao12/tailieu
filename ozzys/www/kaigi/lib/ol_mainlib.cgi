

#============================================================================#
#
# 各クラスをまとめたファイル
# (C) Apps Page & YOSUKE TOBITA.
#
#============================================================================#




#use strict 'vars','subs';




# ↓FHオブジェクト作成クラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::handle;




my $num = 0;
my $pkg  = 'appspage::treecrsdx::handle::';



# →コンストラクタ
# out(新しいFHオブジェクト)
sub new {
    my $name = 'FH'.$num++;
    my $this = \*{$pkg.$name};
    delete $$pkg{$name};
    return bless $this,$_[0];
}




# ↓オブジェクト指向関係
#----------------------------------------------------------------------------#
package appspage::treecrsdx::oo;




# →輸出
# in(輸出先パッケージ名の一部)
sub export {
	shift;
	my ($class) = @_;
	my $pkg1 = 'appspage::treecrsdx::'.$class.'::';
	my $pkg2 = (caller)[0].'::';
	my ($k,$v);
	if($ENV{'MOD_PERL'}) {
		while(($k,$v) = each %{$pkg2}) {
			if(!(exists $pkg1->{$k})) { $pkg1->{$k} = $v; }
		}
		return;
	}
	while(($k,$v) = each %{$pkg2}) { $pkg1->{$k} = $v; }
}




# ↓基本的な処理
#----------------------------------------------------------------------------#
package appspage::treecrsdx::funcs;




# →都合上のコンストラクタ
sub new { return bless {},$_[0]; }




# →POSTまたはGETメソッドからの入力値、POSTメソッド優先
# in(ハッシュ)
# out(リクエストメソッド)
sub parse_input {
	shift;
	my ($ref) = @_;
	my (@pairs,$buf,$k,$v,$x,$method);
	
	if ($ENV{'REQUEST_METHOD'} eq 'POST') {
		read(STDIN, $buf, $ENV{'CONTENT_LENGTH'});
		$method = 'POST';
	}
	else{
		($buf) = $ENV{'QUERY_STRING'};
		$method = 'GET';
	}
	
##	@pairs = split(/&/,$buf);
	@pairs = explode(/&/,$buf);
	undef $buf;
	
	for($x = 0; $x < @pairs; $x++) {
##		($k, $v) = split(/=/, $pairs[$x]);
		($k, $v) = explode(/=/, $pairs[$x]);
		$v =~ tr/+/ /;
		$v =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
		$v =~ s/[\0\f\e]//g;
		$v =~ s/\r\n/\0/g;
		$v =~ s/\r/\0/g;
		$v =~ s/\n/\0/g;
		$v =~ s/\t/ /g;
		$v =~ s/\"/&quot;/g;
		$v =~ s/</&lt;/g;
		$v =~ s/>/&gt;/g;
		$ref->{$k} = $v;
	}
	
	return $method;
}




# →クッキーを解析し、ハッシュに代入
# in(ハッシュ、クッキー名)
# out(なし)
sub parse_cookie {
	shift;
	my ($ref,$c_name) = @_;
	my (%DUM,$k,$v,$x,@pairs,$Cookies);
	$Cookies = $ENV{'HTTP_COOKIE'};
##	@pairs = split(/;/,$Cookies);
	@pairs = explode(/;/,$Cookies);
	for $x (@pairs) {
##		($k, $v) = split(/=/,$x);
		($k, $v) = explode(/=/,$x);
		$v =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
		$k =~ s/ //g;$DUM{$k} = $v;
	}
##	@pairs = split(/<>/,$DUM{$c_name});
	@pairs = explode(/<>/,$DUM{$c_name});
	for $x (@pairs) {
##		($k, $v) = split(/>>/,$x);
		($k, $v) = explode(/>>/,$x);
		$ref->{$k} = $v;
	}
}




# →参照元を検査
# in(URL)
# out(0|1)
sub check_httpref {
	shift;
	my ($refv) = @_;
	if(!$refv){ return 1; }
	if($ENV{'HTTP_REFERER'} =~ /^$refv/) { return 1; }
	return undef;
}




# → jcode.plによる日本語コード変換
# in(jcode.pl,文字コード,ハッシュ)
# out(なし)
sub jcode_convert {
	shift;
	my ($jcode,$ocode,$ref,$lib_dir) = @_;
	if(!$jcode){ return; }
	require($lib_dir.'jcode.pl');
	while (my ($k,$v) = each %{$ref}) {
		jcode::convert(\$v,$ocode);
		$ref->{$k} = $v;
	}
}




# → 時刻取得
# in(UTCからの時刻,フォーマットの種類)
# out(時刻)
sub format_date {
	shift;
	my ($time,$type) = @_;
	if(!$time){ $time = time; }
	$time = ($time + (60 * 60 * 9));
	my ($sec, $min, $hour, $mday, $mon, $year, $wday) = gmtime($time);
	my (@wday_nam) = ('(日)','(月)','(火)','(水)','(木)','(金)','(土)');
	$year += 1900;
	$mon  = sprintf('%02d',++$mon);
	$mday = sprintf('%02d',$mday);
	$hour = sprintf('%02d',$hour);
	$min  = sprintf('%02d',$min);
	if($type eq 'A')
	{ return "$year\年$mon\月$mday\日$hour\時$min\分$wday_nam[$wday]"; }
	elsif($type eq 'B')
	{ return "$mon\月$mday\日$hour\時$min\分$wday_nam[$wday]"; }
	elsif($type eq 'C')
	{ return "$year/$mon/$mday/$hour:$min $wday_nam[$wday]"; }
	else
	{ return "$mon/$mday/$hour:$min $wday_nam[$wday]"; }
}




# → ホスト情報を返す
# in(0|1)
# out(IP|ホスト名)
sub get_host {
	shift;
	if(!($_[0])){ return $ENV{'REMOTE_ADDR'}; }
	elsif(
		!($ENV{'REMOTE_HOST'})
		||
		$ENV{'REMOTE_HOST'} eq $ENV{'REMOTE_ADDR'}
	) {
		return
		gethostbyaddr(
##			pack("C4",split(/\./, $ENV{'REMOTE_ADDR'})),
			pack("C4",explode(/\./, $ENV{'REMOTE_ADDR'})),
			2
		)
		||
		$ENV{'REMOTE_ADDR'};
	}
	else{ return $ENV{'REMOTE_ADDR'}; }
}




# → メッセージ部分を加工
# in(
#    文字列のリファレンス、
#    [オートリンク]、
#    [リンクのターゲット]、
#    [タグ]、
#    [禁止キーワード...]
# )
# out(なし)
sub msg_conv {
	shift;
	my (@tags,$z,$y);
	my ($ref,$auto,$target,$tag,@ngw) = @_;
	
	if(!$target) { $target = '_blank'; }
	
	${$ref} =~ s/\0/<br>/g;
	${$ref} =~ s/&quot;/\"/g;
	if($auto) {
		${$ref} =~ 
		s/([^=^\"]|^)(http|https|ftp)(\:\/\/[\w\.\~\-\/\?\&\+\=\:\@\%\;\#\%]+)/$1<a href=\"$2$3\" target=\"$target\">$2$3<\/a>/g;
	}
	if($tag) {
		#タグ抽出
		@tags = (${$ref} =~ /&lt;(.*?)&gt;/gim);
		TAGCHECK:for $y (@tags) {
			for $z (@ngw) {
				if ($y =~ m/$z/i) {
					undef @tags;
					last TAGCHECK;
				}
			}
		}
		if(@tags) {
			${$ref} =~ s/&lt;(.*?)&gt;/<$1>/gim;
			undef @tags;
		}
	}
}




# →スペースのみの投稿検査
# in(0|1、文字列リファレンス)
# out(0|1)
sub check_space {
	shift;
	my ($check,$ref) = @_;
	if(!$check) { return 1; }
	if(${$ref} eq '') { return 1; }
	my $check_var = ${$ref};
	$check_var =~ s/ //g;
	$check_var =~ s/　//g;
	$check_var =~ s/\0//g;
	if($check_var eq '') { return 0; }

	return 1;
}




# →種で文字列を暗号化
# in(文字列、種)
# out(暗号化された文字列)
sub crypt_by_salt {
	shift;
	my ($char,$salt) = @_;
	my @crypted;
	$crypted[0] = substr(crypt($char,$salt),2,6);
	$crypted[1] = substr(crypt(reverse($char),$salt),2,6);
	for(@crypted) { $_ =~ s/\W/X/g; }
	return join('',@crypted);
} 




# →テンプレート用
# in(値,[1])
# out( checked|空欄)
sub switch_by_val {
	shift;
	my ($val,$on) = @_;
	if($on || $val ne '') { return ' checked'; }
	return '';
}




# →テンプレート用
# out(admin|空欄)
sub footer_admin_link {
	shift;
	if($_[0]) {
		return ('admin',"admin_key = $_[1]");
	}
	return '';
}




# →テンプレート用
# out(bound|空欄)
sub footer_bound {
	shift;
	if(
		$_[0]
		&&
		!appspage::treecrsdx::license::check(
			$_[1]
		)
	) { return 'bound'; }
	return '';
}




# →エラーメッセージ表示
# in(エラーメッセージを格納した配列リファレンス)
sub out_errmsgs {
	shift;
	my ($ref) = @_;
	print STDOUT (
		'<html><head><title>エラー</title></head>',"\n",
		'<body bgcolor="#EEEEEE" text="#666666">',"\n",
		'<p></p>',"\n",
		'<b>エラー</b>',"\n",
		'<p></p><div style="background-color:#FFFFFF;color:#FF0000;">',"\n"
	);
	for(@{$ref}) {
		if   ($_ eq 'B') { $_ = '只今混雑しています。'; }
		elsif($_ eq 'F') { $_ = 'ファイルアクセスエラー。'; }
		elsif($_ eq 'Q') { $_ = 'そのリクエストは認識できません。'; }
		print STDOUT "→ $_<br>\n";
	}
	print STDOUT (
		'</div><p></p>',"\n",
		'<form>',"\n",
		'下記のボタンを押すか、ブラウザのバックボタンで戻れます。<br>',"\n",
		'<input type="button" value="　前の画面へ　"',
		'onClick="javascript:history.go(-1);">',"\n",
		'</form>',"\n",
		'</body></html>',"\n"
	);
}





# ↓HTTPレスポンスヘッダクラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::httpheader;




# →コンストラクタ
# in(なし)
sub new {
	my $this = {};
	$this->{'headers'} = [];
	return bless $this;
}




# →レスポンスヘッダ出力
sub send_header {
	my ($this,@lines) = @_;
	my $NPH;
	if($ENV{'SCRIPT_NAME'} =~ /nph\-[^\/\\]+$/) { $NPH = 1; }
	if($NPH) { binmode(STDOUT); }
	if($ENV{'MOD_PERL'} && $ENV{'PERL_SEND_HEADER'} ne 'On' || $NPH)
	{ print STDOUT "HTTP/1.1 200 OK\r\n"; }
	for (@{$this->{'headers'}}) { print STDOUT "$_\r\n"; }
	print STDOUT "\r\n";
	delete $this->{'headers'};
}




# →レスポンスヘッダ準備
# in(HTTPレスポンスヘッダ)
sub set_header {
	my ($this,$line) = @_;
	if($line ne '') { push @{$this->{'headers'}},$line; }
}




# →クッキーを準備
# in(クッキー名、値、パス、[有効日数])
sub set_cookie {
	my ($this,$name,$val,$path,$exp) = @_;
	my ($cookie);
	$val =~ s/([^_a-zA-Z0-9\.\-\_\@])/'%'.sprintf("%lx",unpack("C*",$1))/eg;
	$cookie .= "Set-Cookie: $name=$val;path=$path;";
	if($exp > 0){
		$cookie .= 'expires=';
		$cookie .= scalar gmtime(time + (60 * 60 * 24 * $exp));
		$cookie .= ';';
	}
	push @{$this->{'headers'}},$cookie;
}




# →リセット
sub reset_header {
	my $this = shift;
	undef @{$this->{'headers'}};
}




# ↓ファイルロッククラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::filelock;




# → コンストラクタ
# in(
#   0 mkdir |1 flock |2 ロックしない
#   ディレクトリ、
#   ファイル名、
#   EX,SH,UN
# )
# out(オブジェクト)
sub new {
	my ($this) = {};
	$this->{'METHOD'} = $_[1];
	$this->{'DIR'}    = $_[2];
	$this->{'NAME'}   = $_[3];
	$this->{'EX'}     = $_[4];
	$this->{'SH'}     = $_[5];
	$this->{'UN'}     = $_[6];
	$this->{'FH'}     = new appspage::treecrsdx::handle::;
	#ロック状態
	$this->{'STAT'}   = 0;
	return bless $this,$_[0];
}




# → ロック
# in(SH|EX)
# out(0|1)
sub lock {
	my ($this,$mode) = @_;
	#ロック制限時間
	my ($time_out) = 600;
	my ($retry) = 100;
	my (@sta,$n_t);
	
	#ロックしない
	if($this->{'METHOD'} eq '2') { return 1; }
	
	#flock
	if($this->{'METHOD'} eq '1'){
		if(
			!open(
				$this->{'FH'},
				'+<'.$this->{'DIR'}.$this->{'NAME'}.'.cgi'
			)
		)
		{ return 0; }
		if(!flock($this->{'FH'},$this->{$mode})) { return 0; }
		$this->{'STAT'} = 1;
		return 1;
	}
	
	#mkdir
	while (-d "$this->{'DIR'}$this->{'NAME'}") {
		select(undef, undef, undef, 0.1);
		if (--$retry <= 0) {
			$n_t = time;
			@sta = stat("$this->{'DIR'}$this->{'NAME'}");
			if(($sta[9] + $time_out) < $n_t) {
				rmdir("$this->{'DIR'}$this->{'NAME'}");
				last;
			}
			return 0;
		}
	}
	if((mkdir "$this->{'DIR'}$this->{'NAME'}",0755)){
		$this->{'STAT'} = 1;
		return 1;
	}
	return 0;
}




# → ロック解除
sub unlock {
	my $this = shift;
	if(!$this->{'STAT'})         { return; }
	if($this->{'METHOD'} eq '2') { return; }
	if($this->{'METHOD'} eq '1') {
		my $check = flock($this->{'FH'},$this->{'UN'});
		close($this->{'FH'});
	}
	else{ rmdir("$this->{'DIR'}$this->{'NAME'}"); }
	$this->{'STAT'} = 0;
}




# ↓ライセンス（変更不可）
#----------------------------------------------------------------------------#
package appspage::treecrsdx::license;




sub new { return bless {},$_[0]; }
sub copyright {
	my $code = shift;
	if(check($code)) {
		return 'copyright = <!-- A REGISTERED LICENSE -->';
	}
	return
	'copyright = (C) '.
	'<a href="http://apps.cside.com/" target="_blank">Apps Page</a>.';
}
sub check {
	my $code = shift;
	if($code =~ /REGISTERED/i) { return 1; }
	return 0;
}
sub to_verify {
	shift;
	print '<html><body>';
	if(check($_[0])) { print 'A REGISTERED LICENSE.'; }
	else { print 'A FREE LICENSE.'; }
	print '</body></html>';
}




# ↓簡易カウンタクラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::simplecounter;




# → カウンタ
# in(ファイル名、桁数、[1]、[LOCK_EX]、[LOCK_UN])
# out(0 | カウント数)
sub Countup {
	shift;
	my($file,$fig,$up,$flock,$lock_ex,$lock_un) = @_;
	my($out,$count,$last_ip,$FH);
	my $ret = sub {
		$out = $_[0];
		while(length($out) < $fig) { $out = '0'.$out; }
		return $out;
	};
	if(!open(COUNTER,'+<'.$file)) { return $ret->('',$fig); }
	if(
		$flock eq '1'
		&&
		!(flock(COUNTER,$lock_ex))
	)
	{ return $ret->('',$fig); }
	
	$count = readline *COUNTER;
	$last_ip = readline *COUNTER;
	chomp($count);
	chomp($last_ip);
	if($up || $ENV{'REMOTE_ADDR'} ne $last_ip) { $count++; }
	if(!(seek(COUNTER,0,0))) { return $ret->('',$fig); }
	print COUNTER $count,"\n";
	print COUNTER $ENV{'REMOTE_ADDR'},"\n";
	if($flock eq '1') { flock(COUNTER,$lock_un); }
	close(COUNTER);
	
	return $ret->($count,$fig);
}




# ↓まとめて扱うデータファイルのクラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::datafile;




# →コンストラクタ
# in(
#    ディレクトリ,
#    作業ディレクトリ,
#    エクステンション,
#    su_mode
# )
sub new {
	my $this = {};
	$this->{'dir'}     = $_[1];
	$this->{'tmp'}     = $_[2];
	$this->{'ext'}     = $_[3];
	$this->{'su_mode'} = $_[4];
	$this->{'sync'}    = {};
	$this->{'file'}    = {};
	return bless $this,$_[0];
}




# →ファイル
# in(名前、クラス、因数...)
sub Compose {
	my $this = shift;
	my ($name,$class,@params) = @_;
	
	my $pkg = "appspage::treecrsdx::datafile::".$class;
	$this->{'file'}->{$name} = $pkg->new (
		$this,
		@params
	);
	$this->{'file'}->{$name}->{'dir'} = $this->{'dir'};
}




# →保存先ディレクトリ変更
# in(名前、ディレクトリ)
sub Setdir { $_[0]->{'file'}->{$_[1]}->{'dir'} = $_[2]; }




# →後始末
sub Settle {
	my $this = shift;
	if(keys %{$this->{'sync'}}) { $this->Suspend; }
}




# →開く
# in(アレ、ファイルハンドル名)
sub fileopen {
	my $this = shift;
	my ($that,$fhname) = @_;
	$that->{'IN'}   = new appspage::treecrsdx::handle::;
	return
		open(
			*{$that->{$fhname}},
			'<'.$that->{'dir'}.$that->{'name'}.$this->{'ext'}
		)
	;
}




# ↓簡易セッション管理クラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::datafile::session_s;




# → コンストラクタ
# in(親、名前、文字、暗号化の種)
# out(オブジェクト)
sub new {
	my $class = shift;
	my ($this) = {};
	my ($base,$name,$char,$salt) = @_;
	$this->{'base'} = $base;
	$this->{'name'} = $name;
	$this->{'CHAR'} = $char;
	$this->{'SALT'} = substr($salt,0,2);
	return bless $this,$class;
}




# → セッション変数発行
# in([時刻])
# out(セッション変数)
sub Createvar {
	my ($this,$time) = @_;
	my (@svar);
	if(!$time) { $time = time; }
	$svar[0] = substr(crypt(reverse($time),$this->{'SALT'}),2,4);
	$svar[1] = substr(crypt(reverse($ENV{'REMOTE_ADDR'}),$svar[0]),2,4);
	$svar[2] = substr(crypt($this->{'CHAR'},$svar[1]),2,4);
	for(@svar) { $_ =~ s/\W/X/g; }
	return join('',@svar).'.'.$time;
}




# ↓設定ファイルのクラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::datafile::config;




# → コンストラクタ
# in(拡張子を除くファイル名前)
# out(オブジェクト)
sub new {
	my $class = shift;
	my ($base,$name) = @_;
	my $this = {};
	$this->{'base'} = $base;
	$this->{'name'} = $name;
	return bless $this,$class;
}




#  →開く
sub Open { return $_[0]->{'base'}->fileopen($_[0],'IN'); }
sub Store { return $_[0]->{'base'}->tmpfileopen($_[0]); }



#  →閉じる
sub Close { return close *{$_[0]->{'IN'}}; undef %{$_[0]}; }




# → 読む
# in(読み込み先ハッシュのリファレンス、名前)
# out(0|1)
sub Load {
	my ($this) = shift;
	my ($ref,@names) = @_;
	my ($k,$v,$x,$input,$bound);
	
	if(!seek(*{$this->{'IN'}},0,0))  { return 0; }
	
	LOOP1:while($_ = readline *{$this->{'IN'}}) {
		chomp($_);
		if($_ =~ /^\/\*(.+)\*\//) {
			undef $input;
			if(!@names) { $input = 1; next(LOOP1); }
			LOOP2:for $x (@names) { if($x eq $1) { $input = 1; } }
		}
		elsif($_ =~ /^__WRITING__/) {
			LOOP3:for $x (@names) {
				if($x =~ /^writing_/) { $bound = 1; last LOOP3 }
			}
			if(!$bound) { last LOOP1; }
		}
		elsif($input && $_ !~ /^\#/) {
##			($k,$v) = split(/ \= /,$_,2);
			($k,$v) = explode(/ \= /,$_,2);
			if($k ne '') { $ref->{$k} = $v; }
		}
	}
	return 1;
}




# ↓レコード取扱いクラス２階層版
#----------------------------------------------------------------------------#
package appspage::treecrsdx::datafile::recs_tree;




# → コンストラクタ
# in(拡張子を除くファイル名前、区切り文字)
# out(オブジェクト)
sub new {
	my $class = shift;
	my ($base,$name,$bnd) = @_;
	my $this = {};
	$this->{'base'} = $base;
	$this->{'name'} = $name;
	$this->{'bnd'}  = $bnd;
	return bless $this,$class;
}




#  →開く
sub Open { return $_[0]->{'base'}->fileopen($_[0],'IN'); }
sub Store { return $_[0]->{'base'}->tmpfileopen($_[0]); }



#  →閉じる
sub Close { return close *{$_[0]->{'IN'}}; undef %{$_[0]}; }




# ↓datafile::recs_treeの読込み機能
#----------------------------------------------------------------------------#
package appspage::treecrsdx::datafile::recs_tree::read;




export appspage::treecrsdx::oo:: 'datafile::recs_tree';




# → 取る
# in(
#    所属番号、
#    レコード番号、
#    [関連記事を読込む配列リファレンス],
#    [関連記事を読込むフィールド数]、
# )
# out(
#    行|空,
#    ファイルポインタ,
# )
sub Fetch {
	my $this = shift;
	my ($bnum,$num,$ref,$max) = @_;
	my ($line,$check,$fpointer,$resln,$ret,$flag);
	my $res_cnt = 0;
	$ref->[0] = [];
	if(!seek(*{$this->{'IN'}},0,0)) { return 0; }
	
	readline *{$this->{'IN'}};
	
	while($line = readline *{$this->{'IN'}}) {
		if( $line =~ /^$bnum$this->{'bnd'}.+/ ) {
			$flag = 1;
			$fpointer = (tell *{$this->{'IN'}}) - length($line);
			last;
		}
	}
	
	LOOP1: while($flag) {
		if(
			!$check
			&&
			$line =~
			/^$bnum$this->{'bnd'}(\d+)$this->{'bnd'}$num$this->{'bnd'}.+/
		) {
			chomp $line;
			$ret = $line;
			$check = 1;
		}
		if($ref) {
			chomp $line;
			if($max) {
##				@_ = split(/$this->{'bnd'}/,$line,($max + 1));
				@_ = explode(/$this->{'bnd'}/,$line,($max + 1));
				pop @_;
				push(
					@{$ref->[0]},
					join($this->{'bnd'},@_)
				);
			}
			else { push @{$ref->[0]},$line; }
		}
		
		$line = readline *{$this->{'IN'}};
		if( $line !~ /^$bnum$this->{'bnd'}.+/ ) { $flag = 0; }
	}
	
	if($check && $fpointer) { return($ret,$fpointer); }
	return ('',0);
}




# → 読む
# in(
#    ページ番号、
#    最大記録件数
#    読み件数、
#    全件数計算、
#    読み込み先配列のリファレンス
#    [読込むフィールド数]、
# )
# out(
#    結果 0|1、
#    空|前のページ番号、
#    次のページ番号|空
#    [件数]
# )
sub Readlines {
	my $this = shift;
	my ($page,$max_recs,$page_recs,$count,$ref,$max) = @_;
	my ($ln,$last_line,$lines_cnt,$prev,$next,$page2);
	my $lines = $page_recs;
	
	if(!seek(*{$this->{'IN'}},0,0)) { return 0; }
	readline *{$this->{'IN'}};
	
	
	#位置調整
	my $x = 0;
	if($page > 1) {
		$prev = $page - 1;
		$page2 = ($page - 1) * $lines;
		
		
		while($ln = readline *{$this->{'IN'}}) {
			if(
				$ln =~
				/^(.+?)$this->{'bnd'}(.+?)$this->{'bnd'}(.+?)$this->{'bnd'}.+/
				&&
				$1 eq $3
			) {
				$page2--;
				$lines_cnt++;
			}
			if($page2 < 0) {
				$last_line = 1;
				last;
			}
		}
	}
	
	
	#代入
	$x = -1;
	my $push = sub {
		chomp($ln);
		if($max) {
##			@_ = split(/$this->{'bnd'}/,$ln,($max + 1));
			@_ = explode(/$this->{'bnd'}/,$ln,($max + 1));
			pop @_;
			push(
				@{$ref->[$x]},
				join($this->{'bnd'},@_)
			);
		}
		else { push @{$ref->[$x]},$ln; }
	};
	while($last_line || ($ln = readline *{$this->{'IN'}})) {
		if(
			$ln =~
			/^(.+?)$this->{'bnd'}(.+?)$this->{'bnd'}(.+?)$this->{'bnd'}.+/
			&&
			$1 eq $3
		) {
			if($last_line) { $last_line = 0; }
			else {$lines_cnt++;}
			$x++;
			if($lines--) {
				$ref->[$x] = [];
				$push->();
			}
		}
		else {
			$push->();
		}
		if($lines < 0) {
			$last_line = 1;
			last;
		}
	}
	
	
	#次のページとカウント
	while($last_line || ($ln = readline *{$this->{'IN'}})) {
		if(
			$ln =~
			/^(.+?)$this->{'bnd'}(.+?)$this->{'bnd'}(.+?)$this->{'bnd'}.+/
			&&
			$1 eq $3
		) {
			if($last_line) { $last_line = 0; }
			else {$lines_cnt++;}
			$next = $page + 1;
			if(!$count) { last; }
		}
	}
	if(!$count) { undef $lines_cnt; }
	
	
	if(@{$ref}) {
		if($page * $page_recs >= $max_recs) { undef $next; }
		if($lines_cnt > $max_recs) { $lines_cnt = $max_recs; }
		return 1,$prev,$next,$lines_cnt;
	}
	return 0,'','',0;
}




# → 読む
# in(
#    読み件数、
#    読み込み先配列のリファレンス
#    [読込むフィールド数]、
# )
# out(0|1)
sub readTopics {
	my $this = shift;
	my ($max_recs,$ref,$max) = @_;
	my ($ln);
	
	if(!seek(*{$this->{'IN'}},0,0)) { return 0; }
	readline *{$this->{'IN'}};
	
	#代入
	my $x = 0;
	while($ln = readline *{$this->{'IN'}}) {
		if(
			$ln =~
			/^(.+?)$this->{'bnd'}(.+?)$this->{'bnd'}(.+?)$this->{'bnd'}.+/
			&&
			$1 eq $3
		) {
			$ref->[$x] = [];
			chomp $ln;
			if($max) {
##				@_ = split(/$this->{'bnd'}/,$ln,($max + 1));
				@_ = explode(/$this->{'bnd'}/,$ln,($max + 1));
				pop @_;
				push(
					@{$ref->[$x]},
					join($this->{'bnd'},@_)
				);
			}
			else { push @{$ref->[$x]},$ln; }
			$x++;
			if($max_recs-- <= 1) { last; }
		}
	}
}




# ↓テンプレートファイル処理クラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::Blocktemplate;




# →コンストラクタ
sub new {
	my $type = shift;
	my ($in,$out,$file,$mc) = @_;
	my ($this) = {};
	$this->{'IN'} = $in;
	$this->{'OUT'} = $out;
	$this->{'FILE'} = $file;
	if($mc eq '') { $mc = "\0"; }
	$this->{'MC'} = $mc;
	########
	$this->{'pos'}  = {};
	$this->{'mem'}  = {};
	return bless $this,$type;
}




# →開く
sub Open {
	my $this = shift;
	return open *{$this->{'IN'}},'<'.$this->{'FILE'};
}




# →閉じる
sub Close {
	my $this = shift;
	close *{$this->{'IN'}};
	$this->ClearMem;
}




# →出力
sub PrintBlock {
	my $this = shift;
	my ($blk,$options,@scripts) = @_; undef @_;
	my ($k,$v,$x,$ln,$mcrep,%opts,%vars,%subblk);
	
	
	#オプション
   if((@_) = explode(/,/,$options)) {
		for $x (@_) {
			if   ($x eq 'mem')  { $opts{'mem'} = 1;}
			elsif($x eq 'pos')  { $opts{'pos'} = 1;}
			elsif($x eq 'top')  { $opts{'top'} = 1;}
			elsif($x eq 'nos')  { $opts{'nos'} = 1;}
			elsif($x eq 'unix') { $opts{'ret'} = "\n";}
			elsif($x eq 'win')  { $opts{'ret'} = "\r\n";}
			elsif($x eq 'mac')  { $opts{'ret'} = "\r";}
		}
		undef $options;
	}
	
	#置換文
	for $x (@scripts) {
##		@_ = split(/ = /,$x,2);
		@_ = explode(/ = /,$x,2);
		if($_[1] ne '') {
			if($_[1] =~ s/\/\?/\/$this->{'MC'}\?/g){ $mcrep = 1; }
			if($_[1] =~ s/\?\//\?$this->{'MC'}\//g){ $mcrep = 1; }
			if($_[1] =~ s/\/BLOCK\=/$this->{'MC'}\/BLOCK\=/g){ $mcrep = 1; }
			if($_[1] =~ s/\/SUB\=/$this->{'MC'}\/SUB\=/g){ $mcrep = 1; }
			$vars{$_[0]} = $_[1];
		}
		elsif($x !~ / = / && $x){
##			@_ = split(/ if /,$x,2);
			@_ = explode(/ if /,$x,2);
			if($_[1] ne '') { $subblk{$_[0]} = $_[1]; }
			else { $subblk{$x} = ''; }
		}
	}
	undef @scripts;
	
	#サブブロック
	while(($k,$v) = each %subblk) {
		if($v && $vars{$v} eq '') { delete $subblk{$k}; }
	}
	
	#top,pos,mem
	if ($opts{'top'}){ seek(*{$this->{'IN'}},0,0); }
	if ($this->{'pos'}->{$blk} && !@{$this->{'mem'}->{$blk}}) {
		seek(*{$this->{'IN'}},$this->{'pos'}->{$blk},0);
	}
	elsif (!@{$this->{'mem'}->{$blk}}) {
		while (
			($ln = readline *{$this->{'IN'}})
			&&
			$ln !~ /^<!--BLOCK=\"$blk\"-->/
		){ next; }
		if ($opts{'pos'} && !($this->{'pos'}->{$blk})) {
			$this->{'pos'}->{$blk} = tell($this->{'IN'});
		}
	}
	
	#出力
	my $rep_ret = sub {
		#unix,mac,winオプション
		$ln =~ s/\n/$opts{'ret'}/g;
		$ln =~ s/\r\n/$opts{'ret'}/g;
		$ln =~ s/\r/$opts{'ret'}/g;
	};
	my $output = sub {
		#サブブロック
		if (
			!$opts{'nos'}
			&&
			$ln =~ /<!--SUB=\"(.+?)\"-->/
		) {
			while (($k,$v) = each %subblk) {
				$ln =~ s/<!--SUB=\"$k\"-->//g;
				$ln =~s/<!--\/SUB=\"$k\"-->//g;
			}
			$ln =~ s/<!--SUB=\"(.+?)\"-->(.+?)<!--\/SUB=\"(.+?)\"-->//g;
		}
		if ($ln =~ /\/\?(.+?)\?\//) {
			while (($k,$v) = each %vars)
			{$ln =~ s/\/\?$k\?\//$v/g;}
		}
		$ln =~ s/\/\?(.+?)\?\///g;
		if($mcrep){
			$ln =~ s/\/$this->{'MC'}\?/\/\?/g;
			$ln =~ s/\?$this->{'MC'}\//\?\//g;
		}
		if($opts{'ret'}){ $rep_ret->(); }
		print {$this->{'OUT'}} $ln;
	};
	
	if ($opts{'mem'} && !(@{$this->{'mem'}->{$blk}})) {
		$this->{'mem'}->{$blk} = [];
		while (
			($ln = readline *{$this->{'IN'}})
			&&
			$ln !~ /^<!--\/BLOCK=\"$blk\"-->/
		)
		{
			push @{$this->{'mem'}->{$blk}},$ln;
			#print "$ln\n";
			$output->();
		}
	}
	elsif (!(@{$this->{'mem'}->{$blk}})) {
		while (
			($ln = readline *{$this->{'IN'}})
			&&
			$ln !~ /^<!--\/BLOCK=\"$blk\"-->/
		){ $output->(); }
	}
	else {
		for ($x = 0; $x < @{$this->{'mem'}->{$blk}}; $x++)
		{ $ln = $this->{'mem'}->{$blk}->[$x]; $output->(); }
	}
}




# →記録したブロックの変数を消去
sub ClearMem {
	my ($this,$blk) = @_;
	if($blk ne ''){
		delete $this->{'mem'}->{$blk};
		delete $this->{'pos'}->{$blk};
	}
	else { undef $this->{'mem'}; undef $this->{'pos'}; }
}




# →記録したブロックのFPに移動
sub SeekBlock {
	my $this = shift;
	my ($blk) = @_;
	if($blk ne ''){ seek(*{$this->{'IN'}},$this->{'pos'}->{$blk},0); }
	else{ seek(*{$this->{'IN'}},0,0); }
} 




# →ブロックFPを記録／ブロックを記録
sub MarkBlock {
	my $this = shift;
	my ($blk,$opt) = @_;
	my ($ln);
	if($this->{'pos'}->{$blk} ne '')
	{ seek(*{$this->{'IN'}},$this->{'pos'}->{$blk},0); }
	
	#option
	if (@{$this->{'mem'}->{$blk}}) { return; }
	
	while (
		($ln = readline *{$this->{'IN'}})
		&&
		$ln !~ /^<!--BLOCK=\"$blk\"-->/
	) { next; }
	
	if($opt eq 'mem') {
		$this->{'mem'}->{$blk} = [];
		while (
			($ln = readline *{$this->{'IN'}})
			&&
			$ln !~ /^<!--\/BLOCK=\"$blk\"-->/
		) {
			push @{$this->{'mem'}->{$blk}},$ln;
		}
	}
	else{ $this->{'pos'}->{$blk} = tell *{$this->{'IN'}}; }
}




# →次のブロックへFPを移動し、ブロック名を返す
sub NextBlock {
	my $this = shift;
	my ($ln);
	while ($ln = readline *{$this->{'IN'}}) {
		if($ln =~ /^<!--BLOCK=\"(.+)\"-->/) {
			$this->{'pos'}->{$1} = tell *{$this->{'IN'}};
			return $1;
		}
	}
}




# →出力ファイルハンドルを変更
sub SelectOUT {
	my ($this) = shift;
	$this->{'OUT'} = $_[0];
}




1;
