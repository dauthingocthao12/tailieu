

#============================================================================#
#
# 書込み処理機能提供ファイル
# (C) Apps Page & YOSUKE TOBITA.
#
#============================================================================#




#use strict;




# ↓書込み処理クラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::main::write;




# →書込み処理
sub process_insert {
	my $this = shift;
	
	#ライブラリ
	require ($this->{'LIB_DIR'}.'ol_writelib.cgi');
	
	
	#リクエストメソッド検査など
	check_beforelock($this);
	
	
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
			'session',
			'bgm',
			'mail'
		)
	)
	{ $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	########## 区切 ##########
	
	
	#入力値検査
	check_input($this);
	
	
	########## 区切 ##########
	
	
	#セッション変数ファイルを開く/更新準備
	if(
		!$this->{'data'}->{'file'}->{'session'}->Open
		||
		!$this->{'data'}->{'file'}->{'session'}->Store
	)
	{ $this->error('F'); }
	
	
	#セッション管理
	my $check = 1;
	my ($min,$sec) ;
	if($this->{'config'}->{'session_timeout'}) {
		($check,$min,$sec) =
		$this->{'data'}->{'file'}->{'session'}->CheckandUpdate(
			$this->{'input'}->{'session'},
			$this->{'config'}->{'session_timeout'},
			$this->{'config'}->{'session_interval'}
		);
		
		if($check eq '0') {
			$this->error('F');
		}
		elsif($check eq '3' || $check eq '4' || $check eq '5') {
			session_error($this,$check,$min,$sec);
		}
		
	}
	
	
	#パスワード
	my $crypted_passw = '';
	if($this->{'input'}->{'passw'}) {
		$crypted_passw =
		$this->{'funcs'}->crypt_by_salt(
			$this->{'input'}->{'passw'},
			$this->{'SALT'}
		);
	}
	
	
	#追加
	my $time = time;
	if($check eq '1') {
		if(
			!$this->{'data'}->{'file'}->{'recs'}->Open
			||
			!$this->{'data'}->{'file'}->{'recs'}->Store
			||
			!$this->{'data'}->{'file'}->{'recs'}->InsertNewrec(
				#最大記録件数
				$this->{'config'}->{'max_recs'},
				#番号など除く行追加
				'1',
				$this->{'input'}->{'subj'},
				$this->{'input'}->{'name'},
				$time,
				$this->{'funcs'}->get_host($this->{'config'}->{'hostname'}),
				$this->{'input'}->{'mail'},
				$this->{'input'}->{'url'},
				$this->{'input'}->{'msg'},
				$crypted_passw
			)
			||
			!$this->{'data'}->Sync
		)
		{
			$this->error('投稿に失敗しました。');
		}
		
		#閉じる
		$this->{'data'}->{'file'}->{'recs'}->Close;
	}
	else {
		$this->{'data'}->Settle;
	}
	
	#閉じる
	$this->{'data'}->{'file'}->{'session'}->Close;
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#メール通知
	if(
		$check eq '1' &&
		$this->{'config'}->{'mailto_admin'} &&
		$this->{'config'}->{'sendmail'} ne '' &&
		$this->{'config'}->{'mail_addr'} ne ''
	) { sendmail($this,$time); }
	
	
	#クッキー発行
	set_formcookie($this);
	
	
	#ヘッダ
	$this->out_header_block;
	
	
	#応答
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'response','nos',
		"response = ".
		'投稿完了しました。'.set_session_msg($this)
	);
	
	
	#フッタ
	$this->out_footer_block;
}




# →返事の書込み処理
sub process_insert_res {
	my $this = shift;
	
	#ライブラリ
	require ($this->{'LIB_DIR'}.'ol_writelib.cgi');
	
	
	#リクエストメソッド検査など
	check_beforelock($this);
	
	
	#記事番号の検査
	if(
		!$this->{'input'}->{'bnum'}
		||
		$this->{'input'}->{'num'} =~ /\D/
	)
	{ $this->error('Q'); }
	
	
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
			'session',
			'bgm',
			'mail'
		)
	)
	{ $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	########## 区切 ##########
	
	
	#入力値検査
	check_input($this);
	
	
	########## 区切 ##########
	
	
	#セッション変数ファイルを開く/更新準備
	if(
		!$this->{'data'}->{'file'}->{'session'}->Open
		||
		!$this->{'data'}->{'file'}->{'session'}->Store
	)
	{ $this->error('F'); }
	
	
	#セッション管理
	my $check = 1;
	my ($min,$sec) ;
	if($this->{'config'}->{'session_timeout'}) {
		($check,$min,$sec) =
		$this->{'data'}->{'file'}->{'session'}->CheckandUpdate(
			$this->{'input'}->{'session'},
			$this->{'config'}->{'session_timeout'},
			$this->{'config'}->{'session_interval'}
		);
		
		if($check eq '0') {
			$this->error('F');
		}
		elsif($check eq '3' || $check eq '4' || $check eq '5') {
			session_error($this,$check,$min,$sec);
		}
		
	}
	
	
	#パスワード
	my $crypted_passw = '';
	if($this->{'input'}->{'passw'}) {
		$crypted_passw =
		$this->{'funcs'}->crypt_by_salt(
			$this->{'input'}->{'passw'},
			$this->{'SALT'}
		);
	}
	
	
	########## 区切 ##########
	
	
	#開く
	if(!$this->{'data'}->{'file'}->{'recs'}->Open) { $this->error('F'); }
	
	
	#取る
	my($line,$fpointer);
	if($check eq '1') {
		(
			$line,
			$fpointer
		) =
		$this->{'data'}->{'file'}->{'recs'}->Fetch(
			$this->{'input'}->{'bnum'},
			$this->{'input'}->{'num'},
			$this->{'recs'},
			1
		);
		
		
		#無効
		if(!$fpointer) {
			$this->error('元の記事が見つかりませんでした。');
		}
		
		
		#無効
		if(
			$this->{'config'}->{'max_res'}
			&&
			(
				$this->{'config'}->{'max_res'}
				<
				@{$this->{'recs'}->[0]}
			)
		) {
			$this->error('返事数が満タンです。');
		}
	}
	
	
	########## 区切 ##########
	
	
	#追加
	my $time = time;
	if($check eq '1') {
		if(
			!$this->{'data'}->{'file'}->{'recs'}->Store
			||
			!$this->{'data'}->{'file'}->{'recs'}->InsertChildrec(
				#追加後の位置
				$this->{'config'}->{'moveto_top'},
				#所属番号の位置 Fetchで取得したファイルポインタ
				$fpointer,
				#所属番号、
				$this->{'input'}->{'bnum'},
				#親番号、
				$this->{'input'}->{'num'},
				#番号を除く新しいレコード
				'1',
				$this->{'input'}->{'subj'},
				$this->{'input'}->{'name'},
				$time,
				$this->{'funcs'}->get_host($this->{'config'}->{'hostname'}),
				$this->{'input'}->{'mail'},
				$this->{'input'}->{'url'},
				$this->{'input'}->{'msg'},
				$crypted_passw,
			)
			||
			!$this->{'data'}->Sync
		)
		{
			$this->error('投稿に失敗しました。');
		}
		
		#閉じる
		$this->{'data'}->{'file'}->{'recs'}->Close;
	}
	else {
		$this->{'data'}->Settle;
	}
	
	#閉じる
	$this->{'data'}->{'file'}->{'session'}->Close;
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#メール通知
	if(
		$check eq '1'
		&&
		$this->{'config'}->{'mailto_admin'}
	)
	{ sendmail($this,$time); }
	
	
	#クッキー発行
	set_formcookie($this);
	
	
	#ヘッダ
	$this->out_header_block;
	
	
	#応答
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'response','nos',
		"response = ".
		'投稿完了しました。'.set_session_msg($this)
	);
	
	
	#フッタ
	$this->out_footer_block;
}




# →削除
sub process_edit_delete {
	my $this = shift;
	
	
	#ライブラリ
	require ($this->{'LIB_DIR'}.'ol_writelib.cgi');
	
	
	#リクエストメソッド検査など
	check_beforelock($this);
	if($this->{'input'}->{'passw'} eq '')
	{ $this->error('編集キーを入力して下さい。'); }
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('EX')) { $this->error('B'); }
	
	
	#設定値
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'write',
			'bgm'
		)
	)
	{ $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	#開く
	if(!$this->{'data'}->{'file'}->{'recs'}->Open) { $this->error('F'); }
	
	
	#編集キー検査
	if(!check_passw($this)) { $this->error('編集キーが間違っています。'); }
	
	
	#削除（更新）
	if(
		!$this->{'data'}->{'file'}->{'recs'}->Store
		||
		!$this->{'data'}->{'file'}->{'recs'}->Update(
			$this->{'input'}->{'bnum'},
			$this->{'fields'}->{'pnum'},
			$this->{'input'}->{'num'},
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
	
	
	#ヘッダ
	$this->out_header_block;
	
	
	#応答
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'response','nos',
		"response = ".
		'記事を削除しました。'
	);
	
	
	#フッタ
	$this->out_footer_block;
}




# →再編集(1)
sub process_rec_edit {
	my $this = shift;
	
	
	my $rep =
	sub { return $_[0].' = '.$this->{'fields'}->{$_[0]}; };
	
	
	#リクエストメソッド検査など
	check_beforelock($this);
	if($this->{'input'}->{'passw'} eq '')
	{ $this->error('編集キーを入力して下さい。'); }
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('SH')) { $this->error('B'); }
	
	
	#設定値
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'write',
			'bgm'
		)
	)
	{ $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	#開く
	if(!$this->{'data'}->{'file'}->{'recs'}->Open) { $this->error('F'); }
	
	
	#編集キー検査
	if(!check_passw($this)) { $this->error('編集キーが間違っています。'); }
	
	
	#閉じる
	$this->{'data'}->{'file'}->{'recs'}->Close;
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#データ加工
	if($this->{'fields'}->{'url'} eq '') {
		$this->{'fields'}->{'url'} = 'http://';
	}
	$this->{'fields'}->{'msg'} =~ s/\0/\n/g;
	
	
	#ヘッダ
	$this->out_header_block;
	
	
	#再編集フォーム
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'edit_form','nos',
		$rep->('bnum'),
		$rep->('pnum'),
		$rep->('num'),
		$rep->('depth'),
		$rep->('name'),
		$rep->('subj'),
		$rep->('mail'),
		$rep->('url'),
		$rep->('msg'),
		$rep->('date'),
		$rep->('host'),
		$rep->('passw'),
		"this_passw = $this->{'input'}->{'passw'}"
	);
	
	
	#フッタ
	$this->out_footer_block;
}




# →再編集(2)
sub process_rec_update {
	my $this = shift;
	
	
	#ライブラリ
	require ($this->{'LIB_DIR'}.'ol_writelib.cgi');
	
	
	#リクエストメソッド検査など
	check_beforelock($this);
	if($this->{'input'}->{'passw'} eq '')
	{ $this->error('編集キーを入力して下さい。'); }
	
	
	#新しい編集キー検査
	if($this->{'input'}->{'new_passw'} ne '') {
		if($this->{'input'}->{'new_passw'} =~ /\W/) {
			$this->error('新しい編集キーに使えない文字が含まれています。');
		}
	}
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('EX')) { $this->error('B'); }
	
	
	#設定値
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'write',
			'bgm'
		)
	)
	{ $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	########## 区切 ##########
	
	
	#入力値検査
	check_input($this);
	
	
	########## 区切 ##########
	
	
	#開く
	if(!$this->{'data'}->{'file'}->{'recs'}->Open) { $this->error('F'); }
	
	
	#編集キー検査
	if(!check_passw($this)) { $this->error('編集キーが間違っています。'); }
	
	
	#新しい編集キー設定
	if($this->{'input'}->{'new_passw'} ne '') {
		$this->{'input'}->{'new_passw'} =
		$this->{'funcs'}->crypt_by_salt(
			$this->{'input'}->{'new_passw'},
			$this->{'SALT'}
		);
	}
	else
	{ $this->{'input'}->{'new_passw'} = $this->{'input'}->{'old_passw'}; }
	
	
	#更新
	if(
		!$this->{'data'}->{'file'}->{'recs'}->Store
		||
		!$this->{'data'}->{'file'}->{'recs'}->Update(
			#番号など
			$this->{'input'}->{'bnum'},
			$this->{'input'}->{'pnum'},
			$this->{'input'}->{'num'},
			$this->{'input'}->{'depth'},
			'2',
			
			#その他フィールド
			$this->{'input'}->{'subj'},
			$this->{'input'}->{'name'},
			$this->{'input'}->{'date'},
			$this->{'input'}->{'host'},
			
			$this->{'input'}->{'mail'},
			$this->{'input'}->{'url'},
			$this->{'input'}->{'msg'},
			$this->{'input'}->{'new_passw'}
		)
		||
		!$this->{'data'}->Sync
	)
	{ $this->error('F'); }
	
	
	#閉じる
	$this->{'data'}->{'file'}->{'recs'}->Close;
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#ヘッダ
	$this->out_header_block;
	
	
	#応答
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'response','nos',
		"response = ".
		'記事の再編集完了しました。'
	);
	
	
	#フッタ
	$this->out_footer_block;
}



# →削除
sub process_rec_delete {
	my $this = shift;
	
	
	#ライブラリ
	require ($this->{'LIB_DIR'}.'ol_writelib.cgi');
	
	
	#リクエストメソッド検査など
	check_beforelock($this);
	if($this->{'input'}->{'passw'} eq '')
	{ $this->error('編集キーを入力して下さい。'); }
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('EX')) { $this->error('B'); }
	
	
	#設定値
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'write',
		)
	)
	{ $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	#開く
	if(!$this->{'data'}->{'file'}->{'recs'}->Open) { $this->error('F'); }
	
	
	#編集キー検査
	if(!check_passw($this)) { $this->error('編集キーが間違っています。'); }
	
	
	#削除（更新）
	if(
		!$this->{'data'}->{'file'}->{'recs'}->Store
		||
		!$this->{'data'}->{'file'}->{'recs'}->Update(
			$this->{'input'}->{'bnum'},
			$this->{'fields'}->{'pnum'},
			$this->{'input'}->{'num'},
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
	
	
	#ヘッダ
	$this->out_header_block;
	
	
	#応答
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'response','nos',
		"response = ".
		'記事を削除しました。'
	);
	
	
	#フッタ
	$this->out_footer_block;
}



# →メール通知
sub sendmail {
	my $this = shift;
	my($time) = @_;
	my $MAIL = new appspage::treecrsdx::handle::;
	
	my $rep  = sub { return $_[0].' = '.$this->{'config'}->{$_[0]}; };
	my $var; 
	my $rep2 = sub {
		$var = $this->{'input'}->{$_[0]};
		$var =~ s/\0/\n/g;
		$var =~ s/&lt;/</g;
		$var =~ s/&gt;/>/g;
		$var =~ s/&quot;/\"/g;
		if($var eq '') { return $_[0].' = '.'----'; }
		return $_[0].' = '.$var;
	};
	
	#メールテンプレート
	$this->{'tmpl'}->{'mail'} = new
	appspage::treecrsdx::Blocktemplate::(
		new appspage::treecrsdx::handle::,
		$MAIL,
		$this->{'LIB_DIR'}.'tmpl_mail.cgi',
		"\0"
	);
	
	
	#Sendmailとテンプレートを開く
	if(
		open(
			*{$MAIL},
			"| $this->{'config'}->{'sendmail'} ".
			$this->{'config'}->{'mail_addr'}
		)
		&&
		$this->{'tmpl'}->{'mail'}->Open
	)
	{
		#メールヘッダブロック
		$this->{'tmpl'}->{'mail'}->PrintBlock(
			'mail_header','nos',
			$rep->('mail_subj'),
			$rep->('mail_addr'),
			$rep->('mail_priority'),
		);
		
		
		#メールボディブロック
		$this->{'tmpl'}->{'mail'}->PrintBlock(
			'mail_body','nos',
			'title = '.$this->{'config'}->{'title'},
			$rep2->('subj'),
			$rep2->('name'),
			$rep2->('mail'),
			$rep2->('url'),
			$rep2->('msg'),
			'host = '.$this->{'funcs'}->get_host(
				$this->{'config'}->{'hostname'}
			),
			'date = '.$this->{'funcs'}->format_date(
				$time,
				$this->{'config'}->{'date_format'}
			)
		);
		
		
		#閉じる
		$this->{'tmpl'}->{'mail'}->Close;
		close(*{$MAIL});
		return 1;
	}
	return 0;
}




# →リクエストメソッド検査など
sub check_beforelock {
	my $this = shift;
	
	
	#リクエスト制限
	if(
		#参照元
		!$this->{'funcs'}->check_httpref($this->{'HTTP_REF'})
		||
		#リクエストメソッド
		$this->{'req_method'} ne 'POST'
	)
	{ $this->error('Q'); }
	
	
	#セッションオブジェクト
	$this->{'data'}->Compose(
		'session',
		'session_s',
		'session',
		$this->{'SESS_CHAR'},
		$this->{'SALT'}
	);
	
	
	#日本語変換
	$this->{'funcs'}->jcode_convert(
		$this->{'JCODE'},
		'sjis',
		$this->{'input'},
		$this->{'LIB_DIR'}
	);
}




# →入力値検査
sub check_input {
	my $this = shift;
	
	my @err_msg;
	
	
	#題名
	if(
		$this->{'config'}->{'check_subj'}
		&&
		$this->{'input'}->{'subj'} eq ''
	){ push @err_msg,('題名を記入して下さい。'); }
	elsif(
		!$this->{'funcs'}->check_space(
			$this->{'config'}->{'check_space'},
			\$this->{'input'}->{'subj'}
		)
	){ push @err_msg,('題名が無効です。'); }
	
	
	#名前
	if(
		$this->{'config'}->{'check_name'}
		&&
		$this->{'input'}->{'name'} eq ''
	){ push @err_msg,('名前を記入して下さい。'); }
	elsif(
		!$this->{'funcs'}->check_space(
			$this->{'config'}->{'check_space'},
			\$this->{'input'}->{'name'}
		)
	){ push @err_msg,('名前が無効です。'); }
	
	
	#メール
	if(
		$this->{'config'}->{'check_mail'}
		&&
		$this->{'input'}->{'mail'} eq ''
	){ push @err_msg,('メールを記入して下さい。'); }
	elsif(
		$this->{'config'}->{'check_mail'} eq '2'
		&&
		$this->{'input'}->{'mail'} !~ /[\w\.\-]+\@[\w\.\-]+\.[\w\.\-]{2,5}$/
	){ push @err_msg,('メールアドレス書式が間違っています。'); }
	elsif(
		!$this->{'funcs'}->check_space(
			$this->{'config'}->{'check_space'},
			\$this->{'input'}->{'mail'}
		)
	){ push @err_msg,('メールが無効です。'); }
	
	
	#ＵＲＬ
	$this->{'input'}->{'url'} =~ s/http\:\/\/http\:\/\//http\:\/\//g;
	if($this->{'input'}->{'url'} eq 'http://') {
		$this->{'input'}->{'url'} = '';
	}
	if(
		$this->{'config'}->{'check_url'}
		&&
		$this->{'input'}->{'url'} eq ''
	){ push @err_msg,('ＵＲＬを記入して下さい。'); }
	elsif(
		!$this->{'funcs'}->check_space(
			$this->{'config'}->{'check_space'},
			\$this->{'input'}->{'url'}
		)
	){ push @err_msg,('ＵＲＬが無効です。'); }
	
	
	#メッセージ
	if($this->{'input'}->{'msg'}  eq '')
	{ push @err_msg,('メッセージを記入して下さい。'); }
	elsif(
		!$this->{'funcs'}->check_space(
			$this->{'config'}->{'check_space'},
			\$this->{'input'}->{'msg'}
		)
	){ push @err_msg,('メッセージが無効です。'); }
	
	
	#編集キー
	if($this->{'input'}->{'passw'}  =~ /\W/)
	{ push @err_msg,('編集キーに使えない文字が含まれています。'); }
	
	
	#入力値制限
	
	
	#題名、名前
	if($this->{'config'}->{'max_char'}) {
		if(
			length($this->{'input'}->{'subj'})
			>
			$this->{'config'}->{'max_char'}
		)
		{ push @err_msg,('題名の文字数オーバーです。'); }
		
		if(
			length($this->{'input'}->{'name'})
			>
			$this->{'config'}->{'max_char'}
		)
		{ push @err_msg,('名前の文字数オーバーです。'); }
		
		if(
			length($this->{'input'}->{'mail'})
			>
			$this->{'config'}->{'max_char'}
		)
		{ push @err_msg,('メールの文字数オーバーです。'); }
		
		if(
			length($this->{'input'}->{'url'})
			>
			$this->{'config'}->{'max_char'}
		)
		{ push @err_msg,('ＵＲＬの文字数オーバーです。'); }
	}
	
	
	#メッセージ
	if(
		$this->{'config'}->{'max_msg'}
		&&
		length($this->{'input'}->{'msg'}) > $this->{'config'}->{'max_msg'}
	)
	{ push @err_msg,('メッセージの文字数オーバーです。'); }
	
	
	if(@err_msg) { $this->error(@err_msg); }
	
	
	########## 区切 ##########
	
	
	#標準値
	if($this->{'input'}->{'subj'} eq '')
	{ $this->{'input'}->{'subj'} = $this->{'config'}->{'std_subj'}; }
	
	if($this->{'input'}->{'name'} eq '')
	{ $this->{'input'}->{'name'} = $this->{'config'}->{'std_name'}; }
}




# →パスワード検査
sub check_passw {
	my $this = shift;
	
	my($line,$fp) =
	$this->{'data'}->{'file'}->{'recs'}->Fetch(
		$this->{'input'}->{'bnum'},
		$this->{'input'}->{'num'},
		undef,
		undef
	);
	
	$this->parse_fields(\$line);
	if(
		$line eq ''
		||
		!$this->{'fields'}->{'stat'}
	)
	{ $this->error('その記事は削除済みです。'); }
	undef $line;
	
	if($this->{'fields'}->{'passw'} eq '') { return 0; }
	if(
		$this->{'fields'}->{'passw'}
		eq
		$this->{'funcs'}->crypt_by_salt(
			$this->{'input'}->{'passw'},
			$this->{'SALT'}
		)
	)
	{ return 1; }
	return 0;
}




# →セッションに関する応答
sub set_session_msg {
	my $this = shift;
	my $sess_msg = '<br>次の投稿は、リロード後可能です。';
	if(
		$this->{'config'}->{'session_timeout'}
		&&
		$this->{'config'}->{'session_interval'}
	) {
		$sess_msg .=
			'<br>連続投稿は<b>'.
			$this->{'config'}->{'session_interval'}.
			'</b>分間の間隔が必要です。'
		;
	}
	return ($sess_msg);
}




# →セッションのエラー
sub session_error {
	my $this = shift;
	my ($error,$min,$sec) = @_;
	if($error eq '3') {
		$this->error(
			'連続投稿は'.
			$this->{'config'}->{'session_interval'}.
			'分の間隔が必要です。',
			$min.'分'.$sec.'秒後、投稿できます。'
		);
	}
	else {
		$this->error(
			'そのリクエストは認識できません。',
			'投稿画面をリロード後、再度投稿してください。'
		);
	}
}




# →投稿処理後のクッキー
sub set_formcookie {
	my $this = shift;
	my $cookie =
	sub { return $_[0].'>>'.$this->{'input'}->{$_[0]}.'<>'; };
	
	
	if($this->{'input'}->{'cookie'}) {
		$this->{'header'}->set_cookie(
			$this->{'CK_NAME'},
			'on>>1<>'.
			$cookie->('name').
			$cookie->('mail').
			$cookie->('url').
			$cookie->('passw'),
			$this->{'CK_PATH'},
			$this->{'CK_DAYS'}
		);
	}
	else {
		$this->{'header'}->set_cookie(
			$this->{'CK_NAME'},
			'',
			$this->{'CK_PATH'},
			undef
		);
	}
}




#============================================================================#
#
# 区切
#
#============================================================================#




# ↓session_sのセッション変数検査クラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::session_s::check;




export appspage::treecrsdx::oo:: 'datafile::session_s';




#  →開く
sub Open { return $_[0]->{'base'}->fileopen($_[0],'IN'); }
sub Store { return $_[0]->{'base'}->tmpfileopen($_[0]); }



#  →閉じる
sub Close { return close *{$_[0]->{'IN'}}; undef %{$_[0]}; }




# → 書込セッションを管理
# in(
#    セッション変数,
#    制限時間,
#    連続防止時間
# )
# out(
#    0 セッション変数ファイルアクセスエラー
#    1 問題無し | 
#    2 投稿済み |
#    3,残り秒数 連続投稿禁止時間帯 |
#    4 古いセッション |
#    5 不正なセッション変数
# )
sub CheckandUpdate {
	my $this = shift;
	$this->{'vars'} = [];
	my($svar,$timeout,$interval) = @_;
##	my($schar,$stime) = split(/\./,$svar);
	my($schar,$stime) = explode(/\./,$svar);
	my $time = time;
	$timeout = $time - $timeout  * 60;
	$interval= $time - $interval * 60;
	
	
	if($svar ne $this->Createvar($stime)) { return 5; }
	
	
	my($o_addr,$o_svar,$o_time,$o_schar,$o_stime,$check);
	my($registered,$continuous,$older);
	my($min,$sec);
	LOOP1: while(readline *{$this->{'IN'}}) {
##		($o_addr,$o_svar,$o_time) = split(/<>/,$_);
		($o_addr,$o_svar,$o_time) = explode(/<>/,$_);
		
		if($o_addr eq $ENV{'REMOTE_ADDR'}) {
##			($o_schar,$o_stime) = split(/\./,$o_svar);
			($o_schar,$o_stime) = explode(/\./,$o_svar);
			
			#タイムアウト
			if($o_time <= $timeout) {
				next LOOP1;
			}
			#二重
			elsif($o_svar eq $svar) {
				$check = 1;
				$registered = 1;
			}
			#古い
			elsif($o_stime > $stime) {
				$check = 1;
				$older = 1;
			}
			#連続
			elsif($interval < $o_time) {
				$check = 1;
				$continuous = 1;
				$min = int(($o_time - $interval) / 60);
				$sec = ($o_time - $interval) - $min * 60;
			}
			else {
				next LOOP1;
			}
			print {$this->{'OUT'}} $_;
		}
		elsif($o_time > $timeout) {
			print {$this->{'OUT'}} $_;
		}
	}
	
	
	if(!$check) {
		print {$this->{'OUT'}}
			$ENV{'REMOTE_ADDR'}."<>".$svar."<>".$time."<>\n"
		;
	}
	
	
	if($older)      { return 4; }
	if($continuous) { return (3,$min,$sec); }
	if($registered) { return 2; }
	return 1;
}




1;
