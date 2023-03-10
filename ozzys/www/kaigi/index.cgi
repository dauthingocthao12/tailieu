#!/usr/local/bin/perl
#↑Perlのパスをサーバに合わせて記述。


# ↓定義
#----------------------------------------------------------------------------#
package appspage::treecrsdx::define;
my $define = {};


#++++++++++++++++++++++++++++#
# 定義の目次             
#++++++++++++++++++++++++++++#
# １：プログラム情報
# ２：ファイル構成図
# ３：基本設定
# ４：設置環境設定
# ５：広告挿入設定
#++++++++++++++++++++++++++++#




#【１：プログラム情報】
#-----------------------------------------------------------------------------#
#
#
# 著作権表示を残していただける範囲内であれば、
# 無料でお使いいただけ、改造も自由です。
# 詳しい情報は、ウェブサイトでご覧下さい。
#
# (C) Apps Page & YOSUKE TOBITA.
# http://apps.cside.com/
#
#
# ※次の３項目は、著作権表示に関係するので、変更しないで下さい。
# ※著作権表示を消してお使いいただく場合、キャッシュライセンス登録が必要です。
#


$define->{'THIS_NAME'} = 'ツリー会議室DX';
$define->{'VERSION'}   = '3.3';
$define->{'LICENSE'}   = 'FREE';




#【２：ファイル構成図】
#-----------------------------------------------------------------------------#
# 
#  
# treecrsdx/   ………………………… (755) プログラムディレクトリ
#    │
#    ├ index.cgi  …………………… (755) 実行ファイル/このファイル
#    │
#    ├ data/  ………………………… (777) データ記録ディレクトリ
#    │  └ 各データファイル ……… (666) 全て同じパーミッション
#    │
#    ├ lib/   ………………………… (755) ライブラリディレクトリ
#    │  └ 各ライブラリファイル … (644) 全て同じパーミッション
#    │
#    └ lock/  ………………………… (777) ロックディレクトリ
#         └ flock.cgi   …………… (666) ロックファイル
#
#




#【３：基本設定】
#-----------------------------------------------------------------------------#


# ■管理用パスワード（記号を除く半角英数）
$define->{'ADMIN_PASSW'} = '1234';


# ■ファイルロック方法
# '0' mkdir関数  / flock関数が使えないサーバ用
# '1' flock関数  / *標準
# '2' ロックしない
$define->{'LOCK_METHOD'} = '1';


# ■カラーチャート表示方法
# '0' 直接表示       / *標準
# '1' CGI経由で表示  / cgi-binなどで制限されるサーバ用
$define->{'COLOR_CHART'} = '0';


# ■設置するサーバOSの種類（パソコンのOSとは異なります。/ 通常変更なし）
# '0' Windowsなど非NFSサーバ  / パーミッションのないサーバ
# '1' UNIX,Linux系サーバ      / *標準
$define->{'NFS_MODE'} = '1';


# ※基本的な設定はここまで。




#【４：設置環境設定】
#-----------------------------------------------------------------------------#
#
# 以下のの設定項目で、セキュリティ強化や、設置環境特有の状態に変更できますが、
# 分からない場合、変更しない方が無難です。
#


# ▼ファイル/ディレクトリ構成
	# ■ロックディレクトリ（このファイルからのパス）
	$define->{'LOCK_DIR'}    = 'lock/';
	
	# ■データ記録ディレクトリ（このファイルからのパス）
	$define->{'DATA_DIR'}    = 'data/';
	
	# ■ライブラリディレクトリ（このファイルからのパス）
	$define->{'LIB_DIR'}     = 'lib/';
	
	# ■作業用ディレクトリ（このファイルからのパス）
	$define->{'TMP_DIR'}     = 'data/';
	
	# ■jcode.plライブラリ（'0'使わない / '1'使う#標準）
	$define->{'JCODE'}       = '';


# ▼セキュリティ強化
	# ■暗号化に使う文字列（半角英数２文字）
	$define->{'SALT'}        = 'PW';
	
	# ■セッション変数発行に使う文字列（半角英数８文字まで）
	$define->{'SESS_CHAR'}   = 'SESSION';
	
	
	# ■参照元アドレスによる一部機能のアクセス制限
	# 例１：設置アドレスが、http://www.yourserver.com/script/ の場合
	# '' → 'http://www.yourserver.com/'
	# 例２：設置アドレスが、http://www.hoge.com/~name/script/ の場合
	# '' → 'http://www.hoge.com/~name/'
	$define->{'HTTP_REF'}    = '';
	
	
	# ■管理画面へのキー（記号を除く半角英数）
	# 例： 'admin' → 'seclet' とした場合、
	# 管理画面へのアドレスは
	# http://設置アドレス/index.cgi?m=admin から、
	# http://設置アドレス/index.cgi?m=seclet へ変更されます。
	$define->{'ADMIN_KEY'}   = 'admin';
	
	
	# ■管理画面へのリンク（'0'非表示 / '1'表示#標準）
	# ※非表示にした場合、上記の"管理画面へのキー"を参考にアクセスして下さい。
	$define->{'ADMIN_LINK'}  = '1';
	
	
	# ■suEXEC環境オプション（'0'#標準 / '1' suEXEC環境限定）
	# '0'#標準の場合、データファイルパーミッションは常に666です。
	# '1'を設定した場合、更新時、元のパーミッションを引継ぐか、644です。
	# ※分からない方、suEXEC環境でない方は、『絶対に』変更しないで下さい。
	$define->{'SU_MODE'}     = '0';


# ▼クッキー
	# ■クッキー発行元パス（ウェブサーバルートパスからの記述）
	# 設定するとセキュリティが向上します。
	# 例１：設置アドレスが、http://www.hoge.com/script/ の場合
	# '/' → '/script/'
	# 例２：設置アドレスが、http://www.hoge.com/~name/script/ の場合
	# '/' → '/~name/script/'
	$define->{'CK_PATH'} = '/';
	
	# ■クッキーネーム（記号を除く半角英数）
	# 複数設置する場合に、適当な名前を付けて区別できます。
	# 例： 'treecrsdx' → 'treecrsdx2'
	$define->{'CK_NAME'} = 'treecrsdx';
	
	# ■クッキー有効日数（半角数字）
	$define->{'CK_DAYS'} = '30';


# ▼flock関数の因数（通常変更不要）
	# ■共有ロック
	$define->{'LOCK_SH'} = '1';
	
	# ■排他ロック
	$define->{'LOCK_EX'} = '2';
	
	# ■アンロック
	$define->{'LOCK_UN'} = '8';




#【５：広告挿入設定】
#-----------------------------------------------------------------------------#
#
# 画面上下にバナー広告など貼付ける場合のみ。
#


# ■画面上の広告HTMLタグ
# 例１： '' → '<a href="～"><img src="～"></a>'
# 例２： '' → '<iframe href="～">～</iframe>' など
$define->{'ADVERT_TOP'}     =  '';


# ■画面下の広告HTMLタグ
$define->{'ADVERT_BOTTOM'}  =  '';




#=============================================================================#
#
# これ以下は、変更の必要ありません。
#
#=============================================================================#




if( !$define->{'NFS_MODE'} ) { $define->{'SU_MODE'} = '2'; }


sub new { return bless $define; }




# ↓主なクラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::main;




#use strict;
#if($ENV{'MOD_PERL'}) { require Apache; }
my $this = new();
$this->start;
$this->stop;




# →コンストラクタ
sub new {
	my $this = new appspage::treecrsdx::define::;
	
	$this->{'input'}    = {};
	$this->{'cookie'}   = {};
	$this->{'config'}   = {};
	$this->{'recs'}     = [];
	$this->{'fields'}   = {};
	$this->{'tmpl'}     = {};
	
	
	#ライブラリ
	require($this->{'LIB_DIR'}.'ol_mainlib.cgi');
	
	
	#ロックオブジェクト
	$this->{'lock'} = new
	appspage::treecrsdx::filelock::(
		$this->{'LOCK_METHOD'},
		$this->{'LOCK_DIR'},
		'flock',
		$this->{'LOCK_EX'},
		$this->{'LOCK_SH'},
		$this->{'LOCK_UN'}
	);
	
	
	#データファイルオブジェクト
	$this->{'data'} = new
	appspage::treecrsdx::datafile::(
		$this->{'DATA_DIR'},
		$this->{'TMP_DIR'},
		'.cgi',
		$this->{'SU_MODE'}
	);
	
	#設定ファイル
	$this->{'data'}->Compose(
		'config',
		'config',
		'config'
	);
	
	#掲示板記事ファイル
	$this->{'data'}->Compose(
		'recs',
		'recs_tree',
		'recs',
		'<>'
	);
	
	
	#その他
	$this->{'header'}  = new appspage::treecrsdx::httpheader::;
	$this->{'funcs'}   = new appspage::treecrsdx::funcs::;
	$this->{'license'} = new appspage::treecrsdx::license::;
	$this->{'header'}->set_header(
		'Content-Type: text/html;accept-charset=UTF-8'
	);
	
	
	return bless $this;
}




# →主な処理開始
sub start {
	my $this = shift;
	
	
	#入力値解析
	$this->{'req_method'} =
	$this->{'funcs'}->parse_input($this->{'input'});
	
	
	#記事リスト
	if($this->{'input'}->{'m'} eq 'list') {
		$this->process_list;
	}
	
	#記事閲覧
	elsif($this->{'input'}->{'m'} eq 'read') {
		$this->{'subtitle'} = '記事閲覧';
		$this->process_read;
	}
	
	#記事閲覧（一覧表示）
	elsif($this->{'input'}->{'m'} eq 'look') {
		$this->{'subtitle'} = '記事閲覧（一覧表示）';
		$this->process_look;
	}
	
	#話題一覧
	elsif($this->{'input'}->{'m'} eq 'topics') {
		$this->{'subtitle'} = '話題一覧';
		$this->process_topics;
	}
	
	#検索
	elsif($this->{'input'}->{'m'} eq 'search') {
		$this->{'subtitle'} = '検索';
		require($this->{'LIB_DIR'}.'ol_search.cgi');
		appspage::treecrsdx::main::search::process_search($this);
	}
	
	#新規投稿
	elsif($this->{'input'}->{'m'} eq 'contrib') {
		$this->{'subtitle'} = '新規投稿';
		require($this->{'LIB_DIR'}.'ol_form.cgi');
		appspage::treecrsdx::main::form::process_contrib($this);
	}
	
	#書込み
	elsif($this->{'input'}->{'m'} eq 'insert') {
		if($this->{'input'}->{'confirm'}) {
			$this->{'subtitle'} = '新規投稿（内容確認）';
			require($this->{'LIB_DIR'}.'ol_form.cgi');
			appspage::treecrsdx::main::form::process_contrib($this);
		}
		else {
			$this->{'subtitle'} = '新規投稿';
			require($this->{'LIB_DIR'}.'ol_write.cgi');
			appspage::treecrsdx::main::write::process_insert($this);
		}
	}
	
	#返事を投稿(1)
	elsif($this->{'input'}->{'m'} eq 'res') {
		$this->{'subtitle'} = '返事を投稿';
		require($this->{'LIB_DIR'}.'ol_form.cgi');
		appspage::treecrsdx::main::form::process_res($this);
	}
	
	#返事を投稿(2)
	elsif($this->{'input'}->{'m'} eq 'insert_res') {
		if($this->{'input'}->{'confirm'}) {
			$this->{'subtitle'} = '返事を投稿（内容確認）';
			require($this->{'LIB_DIR'}.'ol_form.cgi');
			appspage::treecrsdx::main::form::process_res($this);
		}
		else {
			$this->{'subtitle'} = '返事を投稿';
			require($this->{'LIB_DIR'}.'ol_write.cgi');
			appspage::treecrsdx::main::write::process_insert_res($this);
		}
	}
	
	#留意事項
	elsif($this->{'input'}->{'m'} eq 'readme') {
		require($this->{'LIB_DIR'}.'ol_info.cgi');
		$this->{'subtitle'} = '留意事項';
		appspage::treecrsdx::main::info::process_readme($this);
	}
	
	#使い方
	elsif($this->{'input'}->{'m'} eq 'usage') {
		require($this->{'LIB_DIR'}.'ol_info.cgi');
		$this->{'subtitle'} = '使い方';
		appspage::treecrsdx::main::info::process_usage($this);
	}
	
	#編集
	elsif($this->{'input'}->{'m'} eq 'edit') {
		require($this->{'LIB_DIR'}.'ol_form.cgi');
		$this->{'subtitle'} = '再編集/削除';
		appspage::treecrsdx::main::form::process_edit($this);
	}
	
	#編集(削除)
	elsif($this->{'input'}->{'m'} eq 'rec_delete') {
		$this->{'subtitle'} = '記事削除';
		require($this->{'LIB_DIR'}.'ol_write.cgi');
		appspage::treecrsdx::main::write::process_rec_delete($this);
	}
	
	#編集(再編集1)
	elsif($this->{'input'}->{'m'} eq 'rec_edit') {
		$this->{'subtitle'} = '再編集';
		require($this->{'LIB_DIR'}.'ol_write.cgi');
		appspage::treecrsdx::main::write::process_rec_edit($this);
	}
	
	#編集(再編集2)
	elsif($this->{'input'}->{'m'} eq 'rec_update') {
		$this->{'subtitle'} = '再編集';
		require($this->{'LIB_DIR'}.'ol_write.cgi');
		appspage::treecrsdx::main::write::process_rec_update($this);
	}
	
	#管理用(1)
	elsif($this->{'input'}->{'m'} eq $this->{'ADMIN_KEY'}) {
		require($this->{'LIB_DIR'}.'ol_admin.cgi');
		appspage::treecrsdx::main::admin::process_admin($this);
	}
	
	
	#管理用(2)
	elsif($this->{'input'}->{'m'} eq $this->{'ADMIN_KEY'}.'.2') {
		require($this->{'LIB_DIR'}.'ol_admin.cgi');
		appspage::treecrsdx::main::admin::process_admin2($this);
	}
	
	
	#その他
	elsif($this->{'input'}->{'m'} eq 'chart') {
		require($this->{'LIB_DIR'}.'ol_chart.cgi');
		$this->out_chart;
	}
	elsif($this->{'input'}->{'m'} eq 'license') {
		$this->{'header'}->send_header;
		$this->{'license'}->to_verify($this->{'LICENSE'});
	}
	elsif($this->{'input'}->{'m'} eq '') {
		$this->{'input'}->{'m'} = 'list';
		$this->process_list;
	}
	else { $this->error('Q'); }
}




# →記事リスト
sub process_list {
	my $this = shift;
	my($x,$y);
	my $page =
	sub { return $_[0].' = '.$this->{'page'}->{$_[0]}; };
	
	if(
		$this->{'input'}->{'page'} < 1
		||
		$this->{'input'}->{'page'} =~ /\D/
	)
	{ $this->{'input'}->{'page'} = 1; }
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('SH')) { $this->error('B'); }
	
	
	#設定値
	if(
		!$this->{'data'}->{'file'}->{'config'}->Open
		||
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'recs',
			'session',
			'tag',
			'counter',
			'bgm'
		)
	) { $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	#有効なリクエストのみ実行
	my $check;
	if(
		($this->{'input'}->{'page'} * $this->{'config'}->{'page_recs'})
		<=
		$this->{'config'}->{'max_recs'}
	)
	{
		#開く
		if(!$this->{'data'}->{'file'}->{'recs'}->Open) { $this->error('F'); }
		
		
		#読む
		(
			$check,
			$this->{'page'}->{'prev'},
			$this->{'page'}->{'next'},
			$this->{'page'}->{'recs_count'}
		)
		=
		$this->{'data'}->{'file'}->{'recs'}->Readlines(
			$this->{'input'}->{'page'},
			$this->{'config'}->{'max_recs'},
			$this->{'config'}->{'page_recs'},
			$this->{'config'}->{'recs_count'},
			$this->{'recs'},
			9
		);
		
		
		#閉じる
		$this->{'data'}->{'file'}->{'recs'}->Close;
	}
	
	#カウンタ
	if($this->{'config'}->{'counter'}) {
		$this->{'counter'} = Countup
		appspage::treecrsdx::simplecounter::(
			$this->{'DATA_DIR'}.'counter.cgi',
			$this->{'config'}->{'counter_fig'},
			$this->{'config'}->{'counter_up'},
			$this->{'LOCK_METHOD'},
			$this->{'LOCK_EX'},
			$this->{'LOCK_UN'},
		);
	}
	
	$this->{'lock'}->unlock;
	
	
	
	########## 区切 ##########
	
	
	#ページ情報
	my $value = 1;
	if( !@{$this->{'recs'}} ) { $value = 0; }
	$this->{'page'}->{'begin'} =
		$this->{'config'}->{'page_recs'} *
		($this->{'input'}->{'page'} - 1) +
		$value
	;
	$value = @{$this->{'recs'}} - 1;
	if($value < 0) { $value = 0; }
	$this->{'page'}->{'end'} = $this->{'page'}->{'begin'} + $value;
	
	
	#無効なページ
	if(!$check && $this->{'input'}->{'page'} > 1) {
		$this->{'page'}->{'begin'}++;
		$this->error(
			'現在、'.
			$this->{'page'}->{'begin'}.
			'件目以降の記事は存在しないか、閲覧できません。'
		);
	}
	
	
	########## 区切 ##########
	
	
	#ヘッダ
	$this->out_header_block;
	
	
	#メニュー(1)
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'menu','nos',
		"home = $this->{'config'}->{'home'}"
	);
	
	
	#広告挿入
	if($this->{'ADVERT_TOP'} ne '') {
		print '<p></p>',$this->{'ADVERT_TOP'},"<p></p><hr>\n";
	}
	
	
	#マーク/キャッシュ
	$this->{'tmpl'}->{'main'}->MarkBlock( 'list_header', 'mem');
	$this->{'tmpl'}->{'main'}->MarkBlock( 'list_brec', 'mem');
	$this->{'tmpl'}->{'main'}->MarkBlock( 'list_rec', 'mem');
	$this->{'tmpl'}->{'main'}->MarkBlock( 'list_footer', 'mem');
	
	
	#記事リスト出力
	$this->out_list_block;
	undef $this->{'fields'};
	
	
	#キャッシュ破棄
	$this->{'tmpl'}->{'main'}->ClearMem(
		'list_header',
		'list_brec',
		'list_rec',
		'list_footer'
	);
	
	
	#ページ情報ヘッダ
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'page_header','',
		$page->('begin'),
		$page->('end'),
		$page->('recs_count'),
		'recs_count if recs_count',
	);
	
	
	#先頭
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'page_begin','nos'
	) if($this->{'page'}->{'prev'} >= 2);
	
	
	#前
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'page_prev','nos',
		$page->('prev')
	) if $this->{'page'}->{'prev'};
	
	
	#次
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'page_next','nos',
		$page->('next')
	) if $this->{'page'}->{'next'};
	
	
	#ページ情報フッタ
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'page_footer','nos'
	);
	
	
	#フッタ
	$this->out_footer_block;
}




# →記事閲覧
sub process_read {
	my $this = shift;
	my($x,$y);
	
	
	#入力検査
	if(
		!$this->{'input'}->{'bnum'}
		||
		!$this->{'input'}->{'num'}
	)
	{ $this->error('Q'); }
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('SH')) { $this->error('B'); }
	
	
	#設定値
	if(
		!$this->{'data'}->{'file'}->{'config'}->Open
		||
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'recs',
			'tag',
			'bgm'
		)
	) { $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	#有効なリクエストのみ実行
	my $line;
	if(
		($this->{'input'}->{'page'} * $this->{'config'}->{'page_recs'})
		<=
		$this->{'config'}->{'max_recs'}
	)
	{
		#開く
		if(!$this->{'data'}->{'file'}->{'recs'}->Open) { $this->error('F'); }
		
		
		#読む
		($line)
		=
		$this->{'data'}->{'file'}->{'recs'}->Fetch(
			$this->{'input'}->{'bnum'},
			$this->{'input'}->{'num'},
			$this->{'recs'},
			#関連記事を読込むフィールド数
			8
		);
		
		
		#閉じる
		$this->{'data'}->{'file'}->{'recs'}->Close;
	}
	if(!$line) {
		$this->error(
			'現在その記事は存在しません。'
		);
	}
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#ヘッダ
	$this->out_header_block(
		'list_header',
		'list_brec',
		'list_rec',
		'list_footer'
	);
	
	
	#返事数検査
	my $resmode = 'resform';
	if(
		$this->{'config'}->{'max_res'}
		&&
		(
			$this->{'config'}->{'max_res'}
			<
			@{$this->{'recs'}->[0]}
		)
	) {
		$resmode = 'resfull';
	}
	
	
	#記事
	$this->parse_fields(\$line);
	$this->out_rec_block('rec',$resmode);
	
	
	#記事リスト出力
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'subtitle','nos',
		"subtitle = 関連記事"
	);
	$this->out_list_block($this->{'input'}->{'num'});
	undef $this->{'fields'};
	
	
	#キャッシュ破棄
	$this->{'tmpl'}->{'main'}->ClearMem(
		'list_header',
		'list_brec',
		'list_rec',
		'list_footer'
	);
	
	
	#フッタ
	$this->out_footer_block(1);
}




# →記事閲覧（一覧表示）
sub process_look {
	my $this = shift;
	my($x,$y);
	
	
	#入力検査
	if(
		!$this->{'input'}->{'bnum'}
	)
	{ $this->error('Q'); }
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('SH')) { $this->error('B'); }
	
	
	#設定値
	if(
		!$this->{'data'}->{'file'}->{'config'}->Open
		||
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'recs',
			'tag',
			'bgm'
		)
	) { $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	#有効なリクエストのみ実行
	my ($fpointer);
	if(
		($this->{'input'}->{'page'} * $this->{'config'}->{'page_recs'})
		<=
		$this->{'config'}->{'max_recs'}
	)
	{
		#開く
		if(!$this->{'data'}->{'file'}->{'recs'}->Open) { $this->error('F'); }
		
		
		#読む
		(undef,$fpointer)
		=
		$this->{'data'}->{'file'}->{'recs'}->Fetch(
			$this->{'input'}->{'bnum'},
			$this->{'input'}->{'bnum'},
			$this->{'recs'},
			#関連記事を読込むフィールド数
			undef
		);
		
		
		#閉じる
		$this->{'data'}->{'file'}->{'recs'}->Close;
	}
	if(!$fpointer) {
		$this->error(
			'現在その話題は存在しません。'
		);
	}
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#ヘッダ
	$this->out_header_block(
		'list_header',
		'list_brec',
		'list_rec',
		'list_footer'
	);
	
	
	#返事数検査
	my $resmode = 'resform';
	if(
		$this->{'config'}->{'max_res'}
		&&
		(
			$this->{'config'}->{'max_res'}
			<
			@{$this->{'recs'}->[0]}
		)
	) {
		$resmode = 'resfull';
	}
	
	
	#記事
	my $x;
	for($x = 0; $x < @{$this->{'recs'}->[0]}; $x++) {
		$this->parse_fields(\$this->{'recs'}->[0]->[$x]);
		$this->out_rec_block('rec',$resmode);
	}
	
	#記事リスト出力
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'subtitle','nos',
		"subtitle = 関連記事"
	);
	$this->out_list_block($this->{'input'}->{'num'});
	
	
	#キャッシュ破棄
	$this->{'tmpl'}->{'main'}->ClearMem(
		'list_header',
		'list_brec',
		'list_rec',
		'list_footer'
	);
	
	
	#フッタ
	$this->out_footer_block(1);
}




# →話題一覧
sub process_topics {
	my $this = shift;
	my($x,$y);
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('SH')) { $this->error('B'); }
	
	
	#設定値
	if(
		!$this->{'data'}->{'file'}->{'config'}->Open
		||
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'recs',
			'bgm'
		)
	) { $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	#開く
	if(!$this->{'data'}->{'file'}->{'recs'}->Open) { $this->error('F'); }
	
	
	#読む
	my $check;
	(
		$check,
		undef,
		undef,
		$this->{'page'}->{'recs_count'}
	)
	=
	$this->{'data'}->{'file'}->{'recs'}->readTopics(
		$this->{'config'}->{'max_recs'},
		$this->{'recs'},
		9
	);
	
	
	#閉じる
	$this->{'data'}->{'file'}->{'recs'}->Close;
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#ヘッダ
	$this->out_header_block(
		'list_header',
		'list_brec',
		'list_rec',
		'list_footer'
	);
	
	
	#記事リスト出力
	$this->out_list_block;
	
	#キャッシュ破棄
	$this->{'tmpl'}->{'main'}->ClearMem(
		'list_header',
		'list_brec',
		'list_rec',
		'list_footer'
	);
	
	
	#フッタ
	$this->out_footer_block(1);
}




# →ヘッダブロック出力
sub out_header_block {
	my $this = shift;
	my @markblocks = (@_,'footer');
	my $subtitle;
	if($this->{'subtitle'}) { $subtitle = '::'.$this->{'subtitle'}; }
	
	#タイトル
	my $title2 = $this->{'config'}->{'title'};
	if($this->{'config'}->{'title_img'}) {
		$title2 =
		'<img src="'.
		$this->{'config'}->{'title_img'}.
		'">';
	}
	
	
	#区切りのマージン
	my $margin_top =
		int($this->{'config'}->{'font_size'} / 2).
		$this->{'config'}->{'font_unit'}
	;
	
	
	#テンプレートオブジェクト
	$this->{'tmpl'}->{'main'} = new
	appspage::treecrsdx::Blocktemplate::(
		new appspage::treecrsdx::handle::,
		\*STDOUT,
		$this->{'LIB_DIR'}.'tmpl_main.cgi',
		"\0"
	);
	
	
	#開く
	if(!$this->{'tmpl'}->{'main'}->Open) { $this->error('F'); }
	
	
	#HTTPレスポンスヘッダ
	$this->{'header'}->send_header;
	
	
	#ヘッダ
	my $rep = sub { return $_[0].' = '.$this->{'config'}->{$_[0]}; };
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'header','',
		"subtitle = $subtitle",
		"title2 = $title2",
		"bg_img if bg_img",
		"margin_top = $margin_top",
		$rep->('bg_img'),
		$rep->('title'),
		$rep->('home'),
		$rep->('bg_color'),
		$rep->('font_color'),
		$rep->('font_size'),
		$rep->('font_unit'),
		$rep->('strong_color'),
		$rep->('strong_color2'),
		$rep->('new_color'),
		$rep->('faint_color'),
		$rep->('link_color'),
		$rep->('vlink_color'),
		$rep->('link_line'),
		$rep->('title')
	);
	
	if($this->{'input'}->{'m'} ne 'list') {
		#マーク
		for (@markblocks) {
			$this->{'tmpl'}->{'main'}->MarkBlock($_);
		}
		
		#メニュー(2)
		$this->{'tmpl'}->{'main'}->PrintBlock(
			'menu2','nos',
			"home = $this->{'config'}->{'home'}"
		);
		
		
		#広告挿入
		if($this->{'ADVERT_TOP'} ne '') {
			print '<p></p>',$this->{'ADVERT_TOP'},"<p></p>\n";
		}
		
		
		#サブタイトルブロック
		$this->{'tmpl'}->{'main'}->PrintBlock(
			'subtitle','nos,mem',
			"subtitle = $this->{'subtitle'}"
		);
	}
}




# →フッタブロック出力
sub out_footer_block {
	my $this = shift;
	my ($footer_bound) = @_;
	
	#広告挿入
	if($this->{'ADVERT_BOTTOM'} ne '') {
		print '<p></p>',$this->{'ADVERT_BOTTOM'},"<p></p>\n";
	}
	
	#BGM
	if(!$this->{'config'}->{'bgm'}) {
		undef $this->{'config'}->{'bgm_src'};
	}
	
	#フッタ
	if($footer_bound) { $footer_bound = 'footer_bound'; }
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'footer','',
		
		#カウンタ
		"counter = ".$this->{'counter'},
		'counter if counter',
		
		#BGM
		"bgm_src = ".$this->{'config'}->{'bgm_src'},
		"bgm_loop = ".$this->{'config'}->{'bgm_loop'},
		'bgm if bgm_src',
		
		$footer_bound,
		$this->{'funcs'}->footer_admin_link(
			$this->{'ADMIN_LINK'},
			$this->{'ADMIN_KEY'}
		),
		$this->{'funcs'}->footer_bound(
			$this->{'ADMIN_LINK'},
			$this->{'LICENSE'}
		),
		appspage::treecrsdx::license::copyright(
			$this->{'LICENSE'}
		)
	);
	
	
	$this->{'tmpl'}->{'main'}->Close;
}




# →リストブロック出力
sub out_list_block {
	$this = shift;
	my ($x,$y);
	my($cursor) = @_;
	my $pnum;
	my $depth;
	my $n_depth;
	
	#時間
	$this->{'new_time'} =
	time - $this->{'config'}->{'new_time'} * 60 * 60;
	#フォルダマーク
	if($this->{'config'}->{'folder_img'}) {
		$this->{'folder_mark'} =
		'<img src="'.
		$this->{'config'}->{'folder_img'}.
		'" border="0">'
	}
	else {
		$this->{'folder_mark'} = '▼';
	}
	#文書マーク
	if($this->{'config'}->{'doc_img'}) {
		$this->{'doc_mark'} = '<img src="'.
		$this->{'config'}->{'doc_img'}.
		'"> '
	}
	else {
		$this->{'doc_mark'} = '';
	}
	
	LOOP1: for ( $x = 0; $x < @{$this->{'recs'}}; $x++ ) {
		
		$this->{'tmpl'}->{'main'}->PrintBlock( 'list_header', 'nos' );
		
		#階層１
		$this->parse_fields( \$this->{'recs'}->[$x]->[0] );
		$this->out_list_block_rec( 'list_brec', $cursor );
		$n_depth = 1;
		
		#階層２以降
		LOOP2: for ( $y = 1; $y < @{$this->{'recs'}->[$x]}; $y++ ) {
			$this->parse_fields( \$this->{'recs'}->[$x]->[$y] );
			
			while($this->{'fields'}->{'depth'} > $n_depth) {
				print "<ul>\n";
				$n_depth++;
			}
			while($this->{'fields'}->{'depth'} < $n_depth) {
				print "</ul>\n";
				$n_depth--;
			}
			
			$this->out_list_block_rec( 'list_rec', $cursor );
			$pnum = $this->{'fields'}->{'pnum'};
		}
		while($n_depth--) { print "</ul>\n"; }
		
		$this->{'tmpl'}->{'main'}->PrintBlock( 'list_footer', 'nos' );
		undef $this->{'recs'}->[$x];
	}
}
sub out_list_block_rec {
	my $this = shift;
	my($block,$cursor) = @_;
	my $rep  = sub { return $_[0].' = '.$this->{'fields'}->{$_[0]}; };
	my ($stat0,$subj);
	
	#選択記事
	if($cursor ne $this->{'fields'}->{'num'}) {
		$this->{'fields'}->{'subj'} =
			"<a href=\"?m=read&bnum=".
			$this->{'fields'}->{'bnum'}.
			"&num=".
			$this->{'fields'}->{'num'}.
			"\">".
			$this->{'fields'}->{'subj'}.
			'</a>'
		;
	}
	else {
		$this->{'fields'}->{'subj'} =
			"<b>&gt;&gt;".
			$this->{'fields'}->{'subj'}.
			'&lt;&lt;</b>'
		;
	}
	#メッセージ
	$this->{'funcs'}->msg_conv(
		\$this->{'fields'}->{'msg'},
		$this->{'config'}->{'auto_link'},
		$this->{'config'}->{'target_window'},
		$this->{'config'}->{'tag'},
##		split(/\,/,$this->{'config'}->{'ng_words'})
		explode(/\,/,$this->{'config'}->{'ng_words'})
	);
	#ホスト
	if($this->{'config'}->{'exhibit_host'} eq '1') {
		$this->{'fields'}->{'host'} =
		'<!--'.$this->{'fields'}->{'host'}.'-->';
	}
	elsif($this->{'config'}->{'exhibit_host'} eq '2') {
		$this->{'fields'}->{'host'} = '';
	}
	else {
		$this->{'fields'}->{'host'} =
		'('.$this->{'fields'}->{'host'}.')';
	}
	my $new_mark;
	#状態
	if(!$this->{'fields'}->{'stat'}) {
		$this->{'fields'}->{'subj'} =
		'<s>'.$this->{'fields'}->{'subj'}.'</s>';
		$stat0 = 'stat0';
	}
	#新着
	elsif(
		$this->{'new_time'}
		<
		$this->{'fields'}->{'date'}
	) {
		$new_mark = 'new_mark = '.$this->{'config'}->{'new_mark'}
	}
	
	$this->{'tmpl'}->{'main'}->PrintBlock(
		$block,'mem',
		$rep->('subj'),
		$rep->('bnum'),
		$rep->('num'),
		$rep->('name'),
		'date = '.$this->{'funcs'}->format_date(
			$this->{'fields'}->{'date'},
			$this->{'config'}->{'date_format'}
		),
		$stat0,
		$new_mark,
		'new_mark if new_mark',
		"folder_mark = ".$this->{'folder_mark'},
		"doc_mark = ".$this->{'doc_mark'},
	);
}




# →レコードブロック出力
sub out_rec_block {
	my $this = shift;
	my($block,$resmode,$noedit) = @_;
	my $rep  = sub { return $_[0].' = '.$this->{'fields'}->{$_[0]}; };
	my $edit = 'edit';
	
	#メッセージ
	$this->{'funcs'}->msg_conv(
		\$this->{'fields'}->{'msg'},
		$this->{'config'}->{'auto_link'},
		$this->{'config'}->{'target_window'},
		$this->{'config'}->{'tag'},
##		split(/\,/,$this->{'config'}->{'ng_words'})
		explode(/\,/,$this->{'config'}->{'ng_words'})
	);
	#ホスト
	if($this->{'config'}->{'exhibit_host'} eq '1') {
		$this->{'fields'}->{'host'} =
		'<!--'.$this->{'fields'}->{'host'}.'-->';
	}
	elsif($this->{'config'}->{'exhibit_host'} eq '2') {
		$this->{'fields'}->{'host'} = '';
	}
	else {
		$this->{'fields'}->{'host'} =
		'('.$this->{'fields'}->{'host'}.')';
	}
	#削除
	if(!$this->{'fields'}->{'stat'}) {
		$resmode = 'deleted';
		undef $edit;
	}
	#編集
	if($noedit) {
		undef $edit;
	}
	
	
	$this->{'tmpl'}->{'main'}->PrintBlock(
		$block,'mem',
		$rep->('bnum'),
		$rep->('pnum'),
		$rep->('num'),
		$rep->('subj'),
		$rep->('name'),
		$rep->('mail'),
		$rep->('url'),
		$rep->('msg'),
		$rep->('host'),
		'date = '.$this->{'funcs'}->format_date(
			$this->{'fields'}->{'date'},
			$this->{'config'}->{'date_format'}
		),
		'mail if mail',
		'url if url',
		$resmode,
		$edit,
		"target_window = ".$this->{'config'}->{'target_window'}
	);
}




# →フィールド解析
sub parse_fields {
	my $this = shift;
	my($ref) = @_;
	(
		$this->{'fields'}->{'bnum'},
		$this->{'fields'}->{'pnum'},
		$this->{'fields'}->{'num'},
		$this->{'fields'}->{'depth'},
		$this->{'fields'}->{'stat'},
		#5
		$this->{'fields'}->{'subj'},
		$this->{'fields'}->{'name'},
		$this->{'fields'}->{'date'},
		$this->{'fields'}->{'host'},
		#9
		$this->{'fields'}->{'mail'},
		$this->{'fields'}->{'url'},
		$this->{'fields'}->{'msg'},
		$this->{'fields'}->{'passw'}
		
	)
##	= split(/<>/,${$ref});
	= explode(/<>/,${$ref});
}




# →カラーチャート
sub out_chart {
	my $this = shift;
	$this->{'header'}->reset_header;
	$this->{'header'}->set_header('Content-Type: image/png');
	$this->{'header'}->send_header;
	binmode(STDOUT);
	if(!open(CHART,'<'.$this->{'LIB_DIR'}.'chart.png'))
	{ $this->stop; }
	binmode(CHART);
	my $data;
	while(read(CHART,$data,64)) { print STDOUT $data; }
	close(CHART);
	$this->stop;
}




# →エラー
sub error {
	my $this = shift;
	$this->{'data'}->Settle;
	$this->{'lock'}->unlock;
	$this->{'header'}->send_header;
	my(@err) = @_;
	$this->{'funcs'}->out_errmsgs(\@err);
	$this->stop;
}




# →終了
sub stop {
	my $this = shift;
	$this->{'lock'}->unlock;
	if($ENV{'MOD_PERL'}) { Apache::exit(0); }
	exit(0);
}




0;
