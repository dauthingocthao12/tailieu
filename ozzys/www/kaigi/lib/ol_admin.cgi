

#============================================================================#
#
# 管理機能提供ファイル
# (C) Apps Page & YOSUKE TOBITA.
#
#============================================================================#




#use strict;




# ↓管理機能提供クラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::main::admin;




# →管理用(1)
sub process_admin {
	my $this = shift;
	
	
	#クッキーの解析
	$this->{'funcs'}->parse_cookie(
		$this->{'cookie'},
		$this->{'CK_NAME'}.'.admin'
	);
	
	
	if(!$this->{'lock'}->lock('SH')) { $this->error('B'); }
	
	
	#設定値
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'admin'
		)
	)
	{ $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#ヘッダ
	out_admin_header($this,undef);
	
	
	$this->{'tmpl'}->{'admin'}->PrintBlock('header1','nos');
	
	
	$this->{'tmpl'}->{'admin'}->PrintBlock(
		'menu','nos',
		"this_name = $this->{'THIS_NAME'}",
		"admin_key = $this->{'ADMIN_KEY'}",
		"switch = ".$this->{'funcs'}->switch_by_val(
			$this->{'cookie'}->{'admin_passw'}
		),
		"admin_passw = $this->{'cookie'}->{'admin_passw'}"
	);
	
	
	#フッタ
	out_admin_footer($this);
}




# →管理用(2)
sub process_admin2 {
	my $this = shift;
	
	
	#入力検査
	if(
		#POSTメソッド以外からのアクセスを排除
		$this->{'req_method'} ne 'POST'
		||
		#参照元
		!$this->{'funcs'}->check_httpref($this->{'HTTP_REF'})
	)
	{ $this->error('Q'); }
	if(
		$this->{'input'}->{'admin_passw'} ne $this->{'ADMIN_PASSW'}
	)
	{ $this->error('管理用パスワードが間違っています。'); }
	
	#クッキー
	if($this->{'input'}->{'passw_ck'} && $this->{'input'}->{'admin_top'}){
		$this->{'header'}->set_cookie(
			$this->{'CK_NAME'}.'.admin',
			"admin_passw>>$this->{'input'}->{'admin_passw'}<>",
			$this->{'CK_PATH'},
			$this->{'CK_DAYS'}
		);
	}
	elsif($this->{'input'}->{'admin_top'}) {
		$this->{'header'}->set_cookie(
			$this->{'CK_NAME'}.'.admin',
			'',
			$this->{'CK_PATH'},
			undef
		);
	}
	
	
	########## 区切 ##########
	
	
	#入力値別に処理
	
	if($this->{'input'}->{'m2'} eq 'display') {
		$this->{'sub_title'} = '::表示設定';
		admin_config($this);
	}
	
	elsif($this->{'input'}->{'m2'} eq 'display2') {
		require ($this->{'LIB_DIR'}.'ol_writelib.cgi');
		$this->{'sub_title'} = '::表示設定';
		admin_display2($this);
	}
	
	elsif($this->{'input'}->{'m2'} eq 'detail') {
		$this->{'sub_title'} = '::詳細設定';
		admin_config($this);
	}
	
	elsif($this->{'input'}->{'m2'} eq 'detail2') {
		require ($this->{'LIB_DIR'}.'ol_writelib.cgi');
		$this->{'sub_title'} = '::詳細設定';
		admin_detail2($this);
	}
	
	elsif($this->{'input'}->{'m2'} eq 'options') {
		$this->{'sub_title'} = '::オプション設定';
		admin_config($this);
	}
	
	elsif($this->{'input'}->{'m2'} eq 'options2') {
		require ($this->{'LIB_DIR'}.'ol_writelib.cgi');
		$this->{'sub_title'} = '::オプション設定';
		admin_options2($this);
	}
	
	elsif($this->{'input'}->{'m2'} eq 'readme') {
		$this->{'sub_title'} = '::留意事項設定';
		admin_readme($this);
	}
	
	elsif($this->{'input'}->{'m2'} eq 'readme2') {
		require ($this->{'LIB_DIR'}.'ol_writelib.cgi');
		$this->{'sub_title'} = '::留意事項設定';
		admin_readme2($this);
	}
	
	elsif($this->{'input'}->{'m2'} eq 'delete') {
		$this->{'sub_title'} = '::レコード削除';
		admin_recslist($this);
	}
	
	elsif($this->{'input'}->{'m2'} eq 'delete2') {
		require ($this->{'LIB_DIR'}.'ol_writelib.cgi');
		$this->{'sub_title'} = '::レコード削除';
		admin_delete($this);
	}
	
	elsif($this->{'input'}->{'m2'} eq 'edit') {
		$this->{'sub_title'} = '::レコード再編集';
		admin_recslist($this);
	}
	
	elsif($this->{'input'}->{'m2'} eq 'edit2') {
		$this->{'sub_title'} = '::レコード再編集';
		admin_edit($this);
	}
	
	elsif($this->{'input'}->{'m2'} eq 'edit3') {
		require ($this->{'LIB_DIR'}.'ol_writelib.cgi');
		$this->{'sub_title'} = '::レコード再編集';
		admin_update($this);
	}
	
	elsif($this->{'input'}->{'m2'} eq 'restore') {
		$this->{'sub_title'} = '::データ初期化';
		admin_restore($this);
	}
	
	elsif($this->{'input'}->{'m2'} eq 'restore2'){
		require ($this->{'LIB_DIR'}.'ol_writelib.cgi');
		$this->{'sub_title'} = '::データ初期化';
		admin_restore2($this);
	}
	
	else { $this->error('Q'); }
}




# →表示設定/詳細設定(1)
sub admin_config {
	my $this = shift;
	
	my $select  =
	sub { return $_[0].'.'.$this->{'config'}->{$_[0]}.' =  selected'; };
	
	my $rep     =
	sub { return $_[0].' = '.$this->{'config'}->{$_[0]}; };
	
	my $prefurl =
	sub {
		$this->{'config'}->{$_[0]} = 'http://'
		if $this->{'config'}->{$_[0]} eq '';
	};
	
	my $chart = $this->{'LIB_DIR'}.'chart.png';
	if( $this->{'COLOR_CHART'} ) {
		$chart = '?m=chart';
	}
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('SH')) { $this->error('B'); }
	
	
	#設定値
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'}
		)
	)
	{ $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#データを加工
	$prefurl->('home');
	$prefurl->('title_img');
	$prefurl->('bg_img');
	$prefurl->('folder_img');
	$prefurl->('doc_img');
	$prefurl->('bgm_src');
	
	#テーマカラー
	my @theme_color = admin_theme_color($this);
	
	
	########## 区切 ##########
	
	
	#テンプレートオブジェクト
	$this->{'tmpl'}->{'config'} = new
	appspage::treecrsdx::Blocktemplate::(
		new appspage::treecrsdx::handle::,
		\*STDOUT,
		$this->{'LIB_DIR'}.
		'tmpl_'.$this->{'input'}->{'m2'}.'.cgi',
		"\0"
	);
	
	
	#テンプレートを開く
	if(!$this->{'tmpl'}->{'config'}->Open) { $this->error('F'); }
	
	
	#HTTPレスポンスヘッダ
	$this->{'header'}->send_header;
	
	
	#HTMLヘッダ
	$this->{'tmpl'}->{'config'}->PrintBlock(
		'header','nos',
		"title = $this->{'config'}->{'title'}",
		"sub_title = $this->{'sub_title'}",
		"this_name = $this->{'THIS_NAME'}",
		"admin_font = $this->{'config'}->{'admin_font'}",
		"admin_key = $this->{'ADMIN_KEY'}",
		"theme_color0 = $theme_color[0]",
		"theme_color1 = $theme_color[1]",
		"theme_color2 = $theme_color[2]",
		"theme_color3 = $theme_color[3]"
	);
	
	
	#フォーム部分
	$this->{'tmpl'}->{'config'}->PrintBlock(
		$this->{'input'}->{'m2'},'nos',
		"this_name = $this->{'THIS_NAME'}",
		"admin_key = $this->{'ADMIN_KEY'}",
		"admin_passw = $this->{'input'}->{'admin_passw'}",
		"chart = $chart",
		
		#基本設定
		$rep->('title'),
		$rep->('home'),
		
		#新着
		$select->('new_time'),
		$select->('new_mark'),
		
		#一般表示設定
		$rep->('title_img'),
		$rep->('bg_img'),
		$rep->('folder_img'),
		$rep->('doc_img'),
		$select->('font_size'),
		$select->('font_unit'),
		$select->('margin_w'),
		$select->('link_line'),
		$select->('date_format'),
		$select->('exhibit_host'),
		$select->('auto_link'),
		$select->('target_window'),
		$select->('recs_count'),
		
		#表示設定（色）
		$rep->('bg_color'),
		$rep->('font_color'),
		$rep->('link_color'),
		$rep->('vlink_color'),
		$rep->('strong_color'),
		$rep->('strong_color2'),
		$rep->('new_color'),
		$rep->('faint_color'),
		
		#レコード設定
		$select->('max_recs'),
		$select->('page_recs'),
		$select->('max_res'),
		$select->('res_position'),
		$select->('moveto_top'),
		
		#新規投稿設定（基本）
		$rep->('std_subj'),
		$rep->('std_name'),
		$select->('max_char'),
		$select->('max_msg'),
		$select->('hostname'),
		
		#新規投稿設定（入力チェック）
		$select->('check_subj'),
		$select->('check_name'),
		$select->('check_mail'),
		$select->('check_url'),
		$select->('check_msg'),
		$select->('check_space'),
		
		#新規投稿設定（セッション）
		$select->('session_timeout'),
		$select->('session_interval'),
		
		#HTMLタグ設定
		$select->('tag'),
		$rep->('ng_words'),
		
		#カウンタ設定
		$select->('counter'),
		$select->('counter_fig'),
		$select->('counter_up'),
		
		#BGM設定
		$select->('bgm'),
		$rep->('bgm_src'),
		$select->('bgm_loop'),
		
		#メール通知設定
		$select->('mailto_admin'),
		$rep->('sendmail'),
		$rep->('mail_addr'),
		$rep->('mail_subj'),
		$select->('mail_priority'),
		
		#管理用設定
		$select->('admin_font'),
		$select->('admin_theme')
	);
	
	
	#HTMLフッタ
	$this->{'tmpl'}->{'config'}->PrintBlock(
		'footer','nos',
		"version = v.$this->{'VERSION'}",
		appspage::treecrsdx::license::copyright(
			$this->{'LICENSE'}
		)
	);
	
	
	$this->{'tmpl'}->{'config'}->Close;
}




# →表示設定(2)
sub admin_display2 {
	my $this = shift;
	my $ch_url = sub {
		$this->{'input'}->{$_[0]} = ''
		if
		$this->{'input'}->{$_[0]} eq 'http://';
	};
	
	
	#日本語変換
	$this->{'funcs'}->jcode_convert(
		$this->{'JCODE'},
		'sjis',
		$this->{'input'},
		$this->{'LIB_DIR'}
	);
	
	
	#変換
	$ch_url->('home');
	$ch_url->('title_img');
	$ch_url->('bg_img');
	$ch_url->('folder_img');
	$ch_url->('doc_img');
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('EX')) { $this->error('B'); }
	
	
	#開く
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	
	
	#更新
	if(
		!$this->{'data'}->{'file'}->{'config'}->Store
		||
		!$this->{'data'}->{'file'}->{'config'}->Update(
			undef,
			$this->{'input'},
			'display'
		)
		||
		!$this->{'data'}->Sync
	)
	{ $this->error('F'); }
	
	
	#読む
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display','admin'
		)
	)
	{ $this->error('F'); }
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#出力
	out_admin_header($this,1);
	out_admin_response($this,'表示設定を変更しました。');
	out_admin_footer($this);
}




# →詳細設定(2)
sub admin_detail2 {
	my $this = shift;
	
	
	#日本語変換
	$this->{'funcs'}->jcode_convert(
		$this->{'JCODE'},
		'sjis',
		$this->{'input'},
		$this->{'LIB_DIR'}
	);
	
	
	#入力検査
	
	
	#題名
	if($this->{'input'}->{'std_subj'} eq '')
	{ $this->{'input'}->{'std_subj'} = '無題'; }
	
	
	#名前
	if($this->{'input'}->{'std_name'} eq '')
	{ $this->{'input'}->{'std_name'} = '名無し'; }
	
	
	#NGワード
##	my @ng_words = split(/,/,$this->{'input'}->{'ng_words'});
	my @ng_words = explode(/,/,$this->{'input'}->{'ng_words'});
	my @ng_words_2;
	for (@ng_words) {
		if($_ ne '') {
			push @ng_words_2,$_;
		}
	}
	$this->{'input'}->{'ng_words'} = join(',',@ng_words_2);
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('EX')) { $this->error('B'); }
	
	
	#開く
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	
	
	#更新
	if(
		!$this->{'data'}->{'file'}->{'config'}->Store
		||
		!$this->{'data'}->{'file'}->{'config'}->Update(
			undef,
			$this->{'input'},
			'recs',
			'write',
			'session',
			'tag',
			'admin'
		)
		||
		!$this->{'data'}->Sync
	)
	{ $this->error('F'); }
	
	
	#読む
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display','admin'
		)
	)
	{ $this->error('F'); }
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#出力
	out_admin_header($this,1);
	out_admin_response($this,'詳細設定を変更しました。');
	out_admin_footer($this);
}




# →オプション設定(2)
sub admin_options2 {
	my $this = shift;
	
	
	#日本語変換
	$this->{'funcs'}->jcode_convert(
		$this->{'JCODE'},
		'sjis',
		$this->{'input'},
		$this->{'LIB_DIR'}
	);
	
	
	#入力検査
	
	
	#BGM設定
	if($this->{'input'}->{'bgm_src'} eq 'http://')
	{ $this->{'input'}->{'bgm_src'} = ''; }
	if(
		$this->{'input'}->{'bgm_src'} eq ''
		||
		$this->{'input'}->{'bgm'} eq ''
	)
	{ $this->{'input'}->{'bgm'} = ''; }
	
	
	#メール通知設定
	if(
		$this->{'input'}->{'sendmail'} eq ''
		||
		$this->{'input'}->{'mail_addr'} eq ''
	)
	{ $this->{'input'}->{'mailto_admin'} = 0; }
	if($this->{'input'}->{'mail_subj'} eq '') {
		$this->{'input'}->{'mail_subj'} = 'a new message';
	}
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('EX')) { $this->error('B'); }
	
	
	#開く
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	
	
	#更新
	if(
		!$this->{'data'}->{'file'}->{'config'}->Store
		||
		!$this->{'data'}->{'file'}->{'config'}->Update(
			undef,
			$this->{'input'},
			'counter',
			'bgm',
			'mail'
		)
		||
		!$this->{'data'}->Sync
	)
	{ $this->error('F'); }
	
	
	#読む
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display','admin'
		)
	)
	{ $this->error('F'); }
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#出力
	out_admin_header($this,1);
	out_admin_response($this,'オプション設定を変更しました。');
	out_admin_footer($this);
}




# →留意事項設定(1)
sub admin_readme {
	my $this = shift;
	
	
	if(!$this->{'lock'}->lock('SH')) { $this->error('B'); }
	
	
	#設定値
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'admin',
			'writing_readme',
			'writing_readme_form',
			'writing_readme_resform'
		)
	)
	{ $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	$this->{'config'}->{'readme'} =~ s/\0/\n/g;
	$this->{'config'}->{'readme_form'} =~ s/\0/\n/g;
	$this->{'config'}->{'readme_resform'} =~ s/\0/\n/g;
	
	
	#出力
	out_admin_header($this,1);
	
	
	$this->{'tmpl'}->{'admin'}->PrintBlock(
		'readme','nos',
		"admin_passw = $this->{'input'}->{'admin_passw'}",
		"admin_key = $this->{'ADMIN_KEY'}",
		"readme = ".$this->{'config'}->{'readme'},
		'readme_form = '.$this->{'config'}->{'readme_form'},
		'readme_resform = '.$this->{'config'}->{'readme_resform'}
	);
	
	
	out_admin_footer($this);
}




# →留意事項設定(2)
sub admin_readme2 {
	my $this = shift;
	
	
	#日本語変換
	$this->{'funcs'}->jcode_convert(
		$this->{'JCODE'},
		'sjis',
		$this->{'input'},
		$this->{'LIB_DIR'}
	);
	
	
	if(!$this->{'lock'}->lock('EX')) { $this->error('B'); }
	
	
		#開く
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	
	
	#更新
	if(
		!$this->{'data'}->{'file'}->{'config'}->Store
		||
		!$this->{'data'}->{'file'}->{'config'}->Update(
			undef,
			$this->{'input'},
			'writing_readme',
			'writing_readme_form',
			'writing_readme_resform'
		)
		||
		!$this->{'data'}->Sync
	)
	{ $this->error('F'); }
	
	
	#読む
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display','admin'
		)
	)
	{ $this->error('F'); }
	
	
	#閉じる
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#出力
	out_admin_header($this,1);
	out_admin_response($this,'留意事項設定を変更しました。');
	out_admin_footer($this);
}




# →レコード削除／編集(1)
sub admin_recslist {
	my $this = shift;
	my $rep = sub { return $_[0].' = '.$this->{'fields'}->{$_[0]}; };
	my $max_len = 50;
	
	
	if(
		!($this->{'input'}->{'page'} > 1)
		|| $this->{'input'}->{'page'} =~ /\D/
	) { $this->{'input'}->{'page'} = 1; } 
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('SH')) { $this->error('B'); }
	
	
	#設定値
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'recs',
			'admin'
		)
	)
	{ $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	#開く
	if(!$this->{'data'}->{'file'}->{'recs'}->Open) { $this->error('F'); }
	
	
	#読む
	my (
		$check,
		$prev,
		$next
	) =
	$this->{'data'}->{'file'}->{'recs'}->Readlines(
		$this->{'input'}->{'page'},
		$this->{'config'}->{'max_recs'},
		$this->{'config'}->{'page_recs'},
		0,
		$this->{'recs'},
		undef
	);
	
	
	#閉じる
	$this->{'data'}->{'file'}->{'recs'}->Close;
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#出力
	out_admin_header($this,1);
	
	
	#実行モードごとに別のブロックを使う
	$this->{'tmpl'}->{'admin'}->PrintBlock(
		$this->{'input'}->{'m2'}.'_header','nos',
		"admin_passw = $this->{'input'}->{'admin_passw'}",
		"admin_key = $this->{'ADMIN_KEY'}"
	);
	
	
	#レコードブロック
	$this->{'tmpl'}->{'admin'}->MarkBlock(
		$this->{'input'}->{'m2'}.'_rec','mem'
	);
	my($x,$y,$bnum2,$num2,$topic);
	LOOP1: for ($x = 0; $x < @{$this->{'recs'}}; $x++) {
		LOOP2: for ($y = 0; $y < @{$this->{'recs'}->[$x]}; $y++) {
			$this->parse_fields(\$this->{'recs'}->[$x]->[$y]);
			
			if(length($this->{'fields'}->{'msg'}) > $max_len) {
				$this->{'fields'}->{'msg'} =
				substr($this->{'fields'}->{'msg'},0,$max_len).'...';
			}
			
			if(length($this->{'fields'}->{'subj'}) > $max_len) {
				$this->{'fields'}->{'subj'} =
				substr($this->{'fields'}->{'subj'},0,$max_len).'...';
			}
			
			if(length($this->{'fields'}->{'name'}) > $max_len) {
				$this->{'fields'}->{'name'} =
				substr($this->{'fields'}->{'name'},0,$max_len).'...';
			}
			
			
			#親記事
			$bnum2 = $this->{'fields'}->{'bnum'};
			$num2  = $this->{'fields'}->{'num'};
			$topic = 'res';
			if($this->{'fields'}->{'bnum'} eq $this->{'fields'}->{'num'}) {
				$bnum2 =
				'<b class="stc">'.$this->{'fields'}->{'bnum'}.'</b>';
				$num2 =
				'<b class="stc">'.$this->{'fields'}->{'num'}.'</b>';
				$topic = 'topic';
			}
			my $stat = 'stat1';
			if(!$this->{'fields'}->{'stat'}) {
				$stat = 'stat0';
			}
			
			
			$this->{'tmpl'}->{'admin'}->PrintBlock(
				$this->{'input'}->{'m2'}.'_rec','',
				"bnum2 = $bnum2",
				"num2 = $num2",
				$rep->('bnum'),
				$rep->('num'),
				$rep->('subj'),
				$rep->('name'),
				$rep->('msg'),
				"date = ".$this->{'funcs'}->format_date(
					$this->{'fields'}->{'date'},
					'B'
				),
				$topic,
				$stat
			);
		}
	}
	
	
	#キャッシュ破棄
	$this->{'tmpl'}->{'admin'}->ClearMem(
		$this->{'input'}->{'m2'}.'_rec'
	);
	
	
	#実行モードごとに別のブロックを使う
	$this->{'tmpl'}->{'admin'}->PrintBlock(
		$this->{'input'}->{'m2'}.'_footer','nos'
	);
	
	
	#ページ移動
	if($next || $prev) {
		$this->{'tmpl'}->{'admin'}->PrintBlock('move_header','nos');
		
		
		#前
		if($prev) {
			$this->{'tmpl'}->{'admin'}->PrintBlock(
				'prev','nos',
				"admin_passw = $this->{'input'}->{'admin_passw'}",
				"admin_key = $this->{'ADMIN_KEY'}",
				"prev = $prev",
				"m2 = $this->{'input'}->{'m2'}",
			);
		}
		
		
		#次
		if($next) {
			$this->{'tmpl'}->{'admin'}->PrintBlock(
				'next','nos',
				"admin_passw = $this->{'input'}->{'admin_passw'}",
				"admin_key = $this->{'ADMIN_KEY'}",
				"next = $next",
				"m2 = $this->{'input'}->{'m2'}",
			);
		}
		
		$this->{'tmpl'}->{'admin'}->PrintBlock('move_footer','nos');
	}
	
	
	out_admin_footer($this);
}




# →レコード削除(2)
sub admin_delete {
	my $this = shift;
	my($bnum,$num,$topic);
	my($k,$v);
	if($this->{'input'}->{'delete'} =~ /^rec\..+/) {
##		(undef,$bnum,$num) = split(/\./,$this->{'input'}->{'delete'});
		(undef,$bnum,$num) = explode(/\./,$this->{'input'}->{'delete'});
	}
	elsif($this->{'input'}->{'delete'} =~ /^topic\..+/) {
##		(undef,$bnum,$num) = split(/\./,$this->{'input'}->{'delete'});
		(undef,$bnum,$num) = explode(/\./,$this->{'input'}->{'delete'});
		$topic = 1;
	}
	if(!$bnum || !$num)
	{ this->error('話題または記事が選択されていません。'); }
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('EX')) { $this->error('B'); }
	
	
	#設定値
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'admin'
		)
	)
	{ $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	#開く
	if(!$this->{'data'}->{'file'}->{'recs'}->Open) { $this->error('F'); }
	
	
	#取る
	my($line,$fp) =
	$this->{'data'}->{'file'}->{'recs'}->Fetch(
		$bnum,
		$num,
		undef,
		undef
	);
	$this->parse_fields(\$line);
	if(!$this->{'fields'}->{'stat'}) {
		$this->error('その記事は、既に削除されています。');
	}
	
	
	#消す
	my $check;
	if($this->{'input'}->{'delete'} =~ /^topic\..+/) {
		if(
			!$this->{'data'}->{'file'}->{'recs'}->Store
			||
			!($check = $this->{'data'}->{'file'}->{'recs'}->Delete($bnum))
			||
			!$this->{'data'}->Sync
		)
		{ $this->error('F');}
		if(!$check) { $this->error('既に削除されています。'); }
	}
	
	#更新
	elsif(
		!$this->{'data'}->{'file'}->{'recs'}->Store
		||
		!$this->{'data'}->{'file'}->{'recs'}->Update(
			$bnum,
			$this->{'fields'}->{'pnum'},
			$num,
			$this->{'fields'}->{'depth'},
			'0',
			$this->{'fields'}->{'subj'},
			$this->{'fields'}->{'name'},
			$this->{'fields'}->{'date'},
			$this->{'fields'}->{'host'},
			'',
			'',
			'？？？',
			''
		)
		||
		!$this->{'data'}->Sync
	)
	{ $this->error('F'); }
	
	
	#閉じる
	$this->{'data'}->{'file'}->{'recs'}->Close;
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	out_admin_header($this,1);
	out_admin_response($this,'選択記事を削除しました。');
	out_admin_footer($this);
}




# →レコード編集(2)
sub admin_edit {
	my $this = shift;
	my $rep = sub { return $_[0].' = '.$this->{'fields'}->{$_[0]}; };
	
	
	#番号
	($this->{'input'}->{'bnum'},$this->{'input'}->{'num'}) =
##	split(/\./,$this->{'input'}->{'num'});
	explode(/\./,$this->{'input'}->{'num'});
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('SH')) { $this->error('B'); }
	
	
	#設定値
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'recs',
			'admin'
		)
	)
	{ $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	#開く
	if(!$this->{'data'}->{'file'}->{'recs'}->Open) { $this->error('F'); }
	
	
	#取る
	my($line,$fpointer);
	(
		$line,
		$fpointer,
		undef
	) = 
	$this->{'data'}->{'file'}->{'recs'}->Fetch(
		$this->{'input'}->{'bnum'},
		$this->{'input'}->{'num'}
	);
	
	
	#閉じる
	$this->{'data'}->{'file'}->{'recs'}->Close;
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	if($line eq '')
	{ $this->error('指定のレコードが見つかりませんでした。'); }
	
	
	#フィールドを取出す
	$this->parse_fields(\$line);
	undef $line;
	if(!$this->{'fields'}->{'stat'}) { $this->error('既に削除済みです。'); }
	
	#加工
	if($this->{'fields'}->{'url'} eq '') {
		$this->{'fields'}->{'url'} = 'http://';
	}
	$this->{'fields'}->{'msg'} =~ s/\0/\n/g;
	
	
	########## 区切 ##########
	
	
	out_admin_header($this,1);
	
	
	$this->{'tmpl'}->{'admin'}->MarkBlock('rec_edit');
	$this->{'tmpl'}->{'admin'}->PrintBlock(
		'rec_edit','nos',
		"admin_passw = $this->{'input'}->{'admin_passw'}",
		"admin_key = $this->{'ADMIN_KEY'}",
		$rep->('bnum'),
		$rep->('pnum'),
		$rep->('num'),
		$rep->('depth'),
		$rep->('stat'),
		$rep->('subj'),
		$rep->('name'),
		$rep->('mail'),
		$rep->('url'),
		$rep->('msg'),
		$rep->('passw'),
		"date2 = ".$this->{'funcs'}->format_date(
			$this->{'fields'}->{'date'},
			'B'
		),
		$rep->('date'),
		$rep->('host')
	);
	
	out_admin_footer($this);
}




# →レコード編集(3)
sub admin_update {
	my $this = shift;
	
	
	#日本語変換
	$this->{'funcs'}->jcode_convert(
		$this->{'JCODE'},
		'sjis',
		$this->{'input'},
		$this->{'LIB_DIR'}
	);
	
	
	#加工
	
	#URL
	$this->{'input'}->{'url'} =~ s/http\:\/\/http\:\/\//http\:\/\//g;
	if($this->{'input'}->{'url'} eq 'http://') {
		$this->{'input'}->{'url'} = '';
	}
	
	#編集キー
	if($this->{'input'}->{'new_passw'} ne '') {
		if($this->{'input'}->{'new_passw'} =~ /\W/) {
			$this->error('新しい編集キーに使えない文字が含まれています。');
		}
		$this->{'input'}->{'passw'} =
		$this->{'funcs'}->crypt_by_salt(
			$this->{'input'}->{'new_passw'},
			$this->{'SALT'}
		);
	}
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('EX')) { $this->error('B'); }
	
	
	#設定値
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'recs',
			'write',
			'admin'
		)
	)
	{ $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	#入力検査その３
	#標準値
	if($this->{'input'}->{'subj'} eq '')
	{ $this->{'input'}->{'subj'} = $this->{'config'}->{'std_subj'}; }
	if($this->{'input'}->{'name'} eq '')
	{ $this->{'input'}->{'name'} = $this->{'config'}->{'std_name'}; }
	
	
	#開く
	if(!$this->{'data'}->{'file'}->{'recs'}->Open) { $this->error('F'); }
	
	
	#取る
	my($line) = 
	$this->{'data'}->{'file'}->{'recs'}->Fetch(
		$this->{'input'}->{'bnum'},
		$this->{'input'}->{'num'}
	);
	if($line eq '') {
		$this->{'data'}->{'file'}->{'recs'}->Close;
		$this->error('指定のレコードが見つかりませんでした。');
	}
	$this->parse_fields(\$line);
	undef $line;
	if(!$this->{'fields'}->{'stat'}) { $this->error('既に削除済みです。'); }
	
	
	
	#更新
	my $check;
	if(
		!$this->{'data'}->{'file'}->{'recs'}->Store
		||
		!($check = $this->{'data'}->{'file'}->{'recs'}->Update(
			$this->{'input'}->{'bnum'},
			$this->{'input'}->{'pnum'},
			$this->{'input'}->{'num'},
			$this->{'input'}->{'depth'},
			'2',
			$this->{'input'}->{'subj'},
			$this->{'input'}->{'name'},
			$this->{'input'}->{'date'},
			$this->{'input'}->{'host'},
			$this->{'input'}->{'mail'},
			$this->{'input'}->{'url'},
			$this->{'input'}->{'msg'},
			$this->{'input'}->{'passw'}
		))
		||
		!$this->{'data'}->Sync
	)
	{ $this->error('F');}
	
	
	#閉じる
	$this->{'data'}->{'file'}->{'recs'}->Close;
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	if(!$check) { $this->error('既に削除されています。'); }
	
	
	########## 区切 ##########
	
	
	out_admin_header($this,1);
	out_admin_response($this,'選択記事の編集完了しました。');
	out_admin_footer($this);
}




# →データファイル初期化(1)
sub admin_restore {
	my $this = shift;
	
	
	if(!$this->{'lock'}->lock('SH')) { $this->error('B'); }
	
	
	#設定値
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'admin'
		)
	)
	{ $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	out_admin_header($this,1);
	
	
	$this->{'tmpl'}->{'admin'}->PrintBlock(
		'restore','nos',
		"admin_key = $this->{'ADMIN_KEY'}",
		"admin_passw = $this->{'input'}->{'admin_passw'}"
	);
	
	
	out_admin_footer($this);
}




# →データファイル初期化(2)
sub admin_restore2 {
	my $this = shift;
	
	#入力データ検査
	if(!(
		$this->{'input'}->{'display'}
		||
		$this->{'input'}->{'detail'}
		||
		$this->{'input'}->{'options'}
		||
		$this->{'input'}->{'counter'}
		||
		$this->{'input'}->{'readme'}
		||
		$this->{'input'}->{'recs'}
	)) { $this->error('初期化するデータを選択して下さい。'); }
	
	
	if(!$this->{'input'}->{'check'}) { $this->error('初期化確認して下さい。'); }
	
	
	########## 区切 ##########
	
	
	#標準設定データ
	if(
		$this->{'input'}->{'display'}
		||
		$this->{'input'}->{'detail'}
		||
		$this->{'input'}->{'options'} 
	)
	{
		#標準設定ファイル
		$this->{'data'}->Compose(
			'def_config',
			'config',
			'def_config'
		);
		$this->{'data'}->Setdir(
			'def_config',
			$this->{'LIB_DIR'}
		);
		
		#読む
		if(
			!$this->{'data'}->{'file'}->{'def_config'}->Open
			||
			!$this->{'data'}->{'file'}->{'def_config'}->Load(
				$this->{'config'},
			)
		)
		{ $this->error('F'); }
		$this->{'data'}->{'file'}->{'def_config'}->Close;
	}
	
	
	########## 区切 ##########
	
	
	my @spaces;
	
	
	if(!$this->{'lock'}->lock('EX')) { $this->error('B'); }
	
	
	#開く
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	
	
	#設定データ更新
	if(
		$this->{'input'}->{'display'}
		||
		$this->{'input'}->{'detail'}
		||
		$this->{'input'}->{'options'}
		|| 
		$this->{'input'}->{'readme'}
	)
	{
		#※表示設定の空間名
		if($this->{'input'}->{'display'}) {
			push @spaces,('display');
		}
		
		#※詳細設定の空間名
		if($this->{'input'}->{'detail'}) {
			push @spaces,(
				'recs',
				'write',
				'session',
				'tag',
				'admin'
			);
		}
		
		#※留意事項設定の空間名
		if($this->{'input'}->{'readme'}) {
			push @spaces,(
				'writing_readme',
				'writing_readme_form',
				'writing_readme_resform'
			);
		}
		
		
		#更新
		if(
			!$this->{'data'}->{'file'}->{'config'}->Store
			||
			!$this->{'data'}->{'file'}->{'config'}->Update(
				undef,
				$this->{'config'},
				@spaces
			)
		)
		{ $this->error('F'); }
	}
	
	
	#カウント数データ
	if($this->{'input'}->{'counter'}) {
		if(
			$this->{'input'}->{'new_count'} =~ /\D/
			||
			$this->{'input'}->{'new_count'} eq ''
		)
		{ $this->{'input'}->{'new_count'} = 0; }
		if(
			!Restore appspage::treecrsdx::simplecounter::(
				$this->{'DATA_DIR'}.'counter.cgi',
				$this->{'input'}->{'new_count'}
			)
		)
		{ $this->error('F'); }
	}
	
	
	#記事データ
	if($this->{'input'}->{'recs'}) {
		if(
			!$this->{'data'}->{'file'}->{'recs'}->Open
			||
			!$this->{'data'}->{'file'}->{'recs'}->Store
			||
			!$this->{'data'}->{'file'}->{'recs'}->Restore
		)
		{ $this->error('F'); }
	}
	
	
	if(!$this->{'data'}->Sync) { $this->error('F');}
	
	
	#読む
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display','admin'
		)
	)
	{ $this->error('F'); }
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	out_admin_header($this,1);
	out_admin_response($this,'選択データを初期化しました。');
	out_admin_footer($this);
}




#============================================================================#
#
# 区切
#
#============================================================================#




# →管理用画面ヘッダ
sub out_admin_header {
	my $this = shift;
	my($admin2) = @_;
	
	
	#テーマカラー
	my @theme_color = admin_theme_color($this);
	
	
	$this->{'tmpl'}->{'admin'} = new
	appspage::treecrsdx::Blocktemplate::(
		new appspage::treecrsdx::handle::,
		\*STDOUT,
		$this->{'LIB_DIR'}.'tmpl_admin.cgi',"\0"
	);
	
	
	if(!$this->{'tmpl'}->{'admin'}->Open) { $this->error('F'); }
	
	
	$this->{'tmpl'}->{'admin'}->MarkBlock('footer');
	
	
	#HTTPレスポンスヘッダ
	$this->{'header'}->send_header;
	
	
	$this->{'tmpl'}->{'admin'}->PrintBlock(
		'header','nos',
		"title = $this->{'config'}->{'title'}",
		"sub_title = $this->{'sub_title'}",
		"this_name = $this->{'THIS_NAME'}",
		"admin_font = $this->{'config'}->{'admin_font'}",
		"theme_color0 = $theme_color[0]",
		"theme_color1 = $theme_color[1]",
		"theme_color2 = $theme_color[2]",
		"theme_color3 = $theme_color[3]"
	);
	
	
	if($admin2) {
		$this->{'sub_title'} =~ s/:://g;
		$this->{'tmpl'}->{'admin'}->PrintBlock(
			'header2','nos',
			"sub_title = $this->{'sub_title'}",
			"admin_key = $this->{'ADMIN_KEY'}"
		);
	}
	delete $this->{'sub_title'};
}




# →管理用画面フッタ
sub out_admin_footer {
	my $this = shift;
	$this->{'tmpl'}->{'admin'}->PrintBlock(
		'footer','nos',
		"version = v.$this->{'VERSION'}",
		appspage::treecrsdx::license::copyright(
			$this->{'LICENSE'}
		)
	);
	$this->{'tmpl'}->{'admin'}->Close;
}




# →管理用応答メッセージ
sub out_admin_response {
	my $this = shift;
	my ($response) = @_;
	$this->{'tmpl'}->{'admin'}->PrintBlock(
		'response','nos',
		"admin_key = $this->{'ADMIN_KEY'}",
		"response = $response"
	);
}




# →管理用画面テーマ
sub admin_theme_color {
	my $this = shift;
	$this->{'admin_theme'} = [];
	
	
	#背景色,box1背景色,box1文字色,強調色
	
	
	#オレンジ
	if   ($this->{'config'}->{'admin_theme'} eq 1)
	{ return ('#FFFF99','#FFCC00','#000000','#FF9900'); }
	
	#インディゴ
	elsif($this->{'config'}->{'admin_theme'} eq 2)
	{ return ('#6666CC','#333399','#EEEEEE','#000000'); }
	
	#ピーチ
	elsif($this->{'config'}->{'admin_theme'} eq 3)
	{ return ('#FFCCCC','#FF9999','#FFFFFF','#FF6666'); }
	
	#バイオレット
	elsif($this->{'config'}->{'admin_theme'} eq 4)
	{ return ('#CC99CC','#CC66CC','#EEEEEE','#993366'); }
	
	#マロン
	elsif($this->{'config'}->{'admin_theme'} eq 5)
	{ return ('#CCCC66','#CC9900','#FFFFCC','#FF9900'); }
	
	#フォレスト
	elsif($this->{'config'}->{'admin_theme'} eq 6)
	{ return ('#99CC66','#669933','#CCCC66','#CC6600'); }
	
	#グレー
	elsif($this->{'config'}->{'admin_theme'} eq 7)
	{ return ('#999999','#666666','#EEEEEE','#000000'); }
	
	#ストロベリー
	elsif($this->{'config'}->{'admin_theme'} eq 8)
	{ return ('#FF9999','#FF0066','#FFFF99','#CC3333'); }
	
	#ライム
	elsif($this->{'config'}->{'admin_theme'} eq 9)
	{ return ('#99FF00','#339900','#99EE00','#FF6600'); }
	
	#チョコレート
	elsif($this->{'config'}->{'admin_theme'} eq 10)
	{ return ('#CC9933','#990000','#CCCC33','#660000'); }
	
	#グレープ
	elsif($this->{'config'}->{'admin_theme'} eq 11)
	{ return ('#CC33FF','#660066','#CC66FF','#990066'); }
	
	#スタンダード
	else
	{ return ('#AAAACC','#9999CC','#333366','#FF6600'); }
}




#============================================================================#
#
# 区切
#
#============================================================================#




# ↓datafile::configの更新機能提供クラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::datafile::config::update;




export appspage::treecrsdx::oo:: 'datafile::config';




# → 更新
# in(
# 読み込み先ハッシュのリファレンス、
# 更新元ハッシュのリファレンス、
# [更新する空間の名前...]
# )
# out(0|1)
# → 更新
# in(
# 読み込み先ハッシュのリファレンス、
# 更新元ハッシュのリファレンス、
# [更新する空間の名前...]
# )
# out(0|1)
sub Update {
	my $this = shift;
	my($to,$from,@names) = @_;
	my($x,$k,$v,$upd);
	
	
	if(!seek(*{$this->{'IN'}},0,0))  { return 0; }
	if(!seek(*{$this->{'OUT'}},0,0)) { return 0; }
	
	
	LOOP1:while($_ = readline *{$this->{'IN'}}) {
		if($_ =~ /^\/\*(.+)\*\//) {
			print {$this->{'OUT'}} $_;
			undef $upd;
			if(!@names) { $upd = 1; next(LOOP1); }
			LOOP2:for $x (@names) { 
				if($x eq $1) { $upd = 1; }
			}
		}
		elsif($upd && $_ !~ /^\#/ && $_ !~ /^__WRITING__/) {
##			($k,$v) = split(/ \= /,$_,2);
			($k,$v) = explode(/ \= /,$_,2);
			$to->{$k} = $from->{$k} if $to;
			print {$this->{'OUT'}} "$k = ",$from->{$k},"\n";
		}
		else { print {$this->{'OUT'}} $_; }
	}
	
	return 1;
}




# ↓datafile::recs_treeのリストア機能提供クラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::datafile::recs_tree::restore;




export appspage::treecrsdx::oo:: 'datafile::recs_tree';




# →リストア
sub Restore {
	my($this) = shift;
	
	if(!seek(*{$this->{'IN'}},0,0))  { return 0; }
	if(!seek(*{$this->{'OUT'}},0,0)) { return 0; }
	
	print {$this->{'OUT'}} "0\n";
	
	return 1;
}




# ↓simplecounterのリストア機能提供クラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::simplecounter::restore;




export appspage::treecrsdx::oo:: 'simplecounter';




# →リストア
# in(ファイル名、新しいカウント数)
sub Restore {
	shift;
	my($file,$new_count) = @_;
	if(!open(COUNTER,'>'.$file)) { return 0; }
	print COUNTER $new_count,"\n\n";
	close(COUNTER);
	return 1;
}




1;
