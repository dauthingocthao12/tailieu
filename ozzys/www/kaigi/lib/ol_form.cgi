

#==============================================================================#
#
# 投稿フォーム出力機能提供ファイル
# (C) Apps Page & YOSUKE TOBITA.
#
#==============================================================================#




#use strict;




# ↓フォームのクラス
#------------------------------------------------------------------------------#
package appspage::treecrsdx::main::form;




# →新規投稿
sub process_contrib {
	my $this = shift;
	
	
	########## 区切 ##########
	
	
	#セッション
	$this->{'data'}->Compose(
		'session',
		'session_s',
		'session',
		$this->{'SESS_CHAR'},
		$this->{'SALT'}
	);
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('SH')) { $this->error('B'); }
	
	
	#設定値
	if(
		!$this->{'data'}->{'file'}->{'config'}->Open
		||
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'write',
			'tag',
			'writing_readme_form',
			'bgm'
		)
	) { $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#確認用入力検査
	if($this->{'input'}->{'confirm'}) {
		check_input($this);
		delete $this->{'config'}->{'readme_form'};
	}
	else {
		$this->{'config'}->{'readme_form'} =~ s/\0/<br>/g;
		$this->{'config'}->{'readme_form'} =~ s/&lt;/</g;
		$this->{'config'}->{'readme_form'} =~ s/&gt;/>/g;
		$this->{'config'}->{'readme_form'} =~ s/&quot;/\"/g;
	}
	
	
	#フォームの内容
	my $key = 'cookie';
	my $switch;
	if($this->{'input'}->{'confirm'}) {
		$key = 'input';
		$switch = $this->{'input'}->{'cookie'};
		$this->{'input'}->{'msg'} =~ s/\0/\n/g;
	}
	else {
		#クッキーの解析
		$this->{'funcs'}->parse_cookie(
			$this->{'cookie'},
			$this->{'CK_NAME'}
		);
		if($this->{'cookie'}->{'url'} eq '') {
			$this->{'cookie'}->{'url'} = 'http://';
		}
		$switch = $this->{'cookie'}->{'on'};
	}
	
	
	
	#ヘッダ
	$this->out_header_block();
	
	
	#フォーム
	my $form =
	sub { return $_[0].' = '.$this->{$key}->{$_[0]}; };
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'form','nos',
		'm = insert',
		"session = ".$this->{'data'}->{'file'}->{'session'}->Createvar,
		$form->('subj'),
		$form->('name'),
		$form->('mail'),
		$form->('url'),
		$form->('passw'),
		$form->('msg'),
		"switch = ".$this->{'funcs'}->switch_by_val(
			$switch
		),
		"readme = ".$this->{'config'}->{'readme_form'}
	);
	
	
	#内容確認
	if($this->{'input'}->{'confirm'}) {
		$this->{'funcs'}->msg_conv(
			\$this->{'input'}->{'msg'},
			$this->{'config'}->{'auto_link'},
			$this->{'config'}->{'target_window'},
			$this->{'config'}->{'tag'},
##			split(/\,/,$this->{'config'}->{'ng_words'})
			explode(/\,/,$this->{'config'}->{'ng_words'})
		);
		$this->{'input'}->{'msg'} =~ s/\n/<br>/g;
		$this->{'tmpl'}->{'main'}->PrintBlock(
			'subtitle','nos',
			'subtitle = 確認表示'
		);
		$this->{'tmpl'}->{'main'}->PrintBlock(
			'confirm','nos',
			$form->('subj'),
			$form->('name'),
			$form->('mail'),
			$form->('msg'),
			$form->('url'),
		);
	}
	
	
	#フッタ
	$this->out_footer_block;
}




# →返事を投稿
sub process_res {
	my $this = shift;
	
	
	#入力検査
	if(
		!$this->{'input'}->{'bnum'}
		||
		!$this->{'input'}->{'num'}
	)
	{ $this->error('Q'); }
	
	
	########## 区切 ##########
	
	
	#セッション
	$this->{'data'}->Compose(
		'session',
		'session_s',
		'session',
		$this->{'SESS_CHAR'},
		$this->{'SALT'}
	);
	
	
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
			'write',
			'session',
			'tag',
			'writing_readme_resform',
			'bgm'
		)
	) { $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	#開く
	if(!$this->{'data'}->{'file'}->{'recs'}->Open) { $this->error('F'); }
	
	
	#取る
	my($line) =
	$this->{'data'}->{'file'}->{'recs'}->Fetch(
		$this->{'input'}->{'bnum'},
		$this->{'input'}->{'num'},
		$this->{'recs'},
		#関連記事を読込むフィールド数
		8
	);
	
	
	#閉じる
	$this->{'data'}->{'file'}->{'recs'}->Close;
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#確認用入力検査
	if($this->{'input'}->{'confirm'}) {
		check_input($this);
		delete $this->{'config'}->{'readme_resform'};
	}
	else {
		$this->{'config'}->{'readme_resform'} =~ s/\0/<br>/g;
		$this->{'config'}->{'readme_resform'} =~ s/&lt;/</g;
		$this->{'config'}->{'readme_resform'} =~ s/&gt;/>/g;
		$this->{'config'}->{'readme_resform'} =~ s/&quot;/\"/g;
	}
	
	
	#無効なレコード
	if($line eq '') {
		$this->error('元の記事が見つかりませんでした。');
	}
	
	
	#無効なレコード
	if(
		$this->{'config'}->{'max_res'}
		&&
		$this->{'config'}->{'max_res'}
		<
		@{$this->{'recs'}->[0]}
	) {
		$this->error('返事数が満タンです。');
	}
	
	
	########## 区切 ##########
	
	
	#記事
	$this->parse_fields(\$line);
	if(!$this->{'fields'}->{'stat'}) { $this->error('その記事は削除済みです。'); }
	
	#フォームの内容
	my $key = 'cookie';
	my $switch;
	my $msg2;
	my $subj;
	if($this->{'input'}->{'confirm'}) {
		$key = 'input';
		$switch = $this->{'input'}->{'cookie'};
		$this->{'input'}->{'msg'} =~ s/\0/\n/g ;
		$msg2 = $this->{'input'}->{'msg'};
		$subj = $this->{'input'}->{'subj'};
	}
	else {
		#クッキーの解析
		$this->{'funcs'}->parse_cookie(
			$this->{'cookie'},
			$this->{'CK_NAME'}
		);
		if($this->{'cookie'}->{'url'} eq '') {
			$this->{'cookie'}->{'url'} = 'http://';
		}
		$switch = $this->{'cookie'}->{'on'};
		#引用文
		$msg2 = '&gt; '.$this->{'fields'}->{'msg'};
		$msg2 =~ s/\0/\n&gt; /g;
		$msg2 .= "\n";
		$subj = 'Re:'.$this->{'fields'}->{'subj'};
	}
	
	
	########## 区切 ##########
	
	
	#ヘッダ/ブロックのキャッシュ
	$this->out_header_block(
		'list_header',
		'list_brec',
		'list_rec',
		'list_footer'
	);
	$this->{'tmpl'}->{'main'}->MarkBlock('subtitle');
	
	
	#フォーム
	my $form =
	sub { return $_[0].' = '.$this->{$key}->{$_[0]}; };
	my $rep  =
	sub { return $_[0].' = '.$this->{'fields'}->{$_[0]}; };
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'form','nos',
		'm = insert_res',
		$rep->('bnum'),
		$rep->('num'),
		'session = '.$this->{'data'}->{'file'}->{'session'}->Createvar,
		"subj = $subj",
		$form->('name'),
		$form->('mail'),
		$form->('url'),
		$form->('passw'),
		"switch = ".$this->{'funcs'}->switch_by_val($switch),
		'msg = '.$msg2,
		"readme = ".$this->{'config'}->{'readme_resform'}
	);
	
	
	#内容確認
	if($this->{'input'}->{'confirm'}) {
		$this->{'funcs'}->msg_conv(
			\$this->{'input'}->{'msg'},
			$this->{'config'}->{'auto_link'},
			$this->{'config'}->{'target_window'},
			$this->{'config'}->{'tag'},
##			split(/\,/,$this->{'config'}->{'ng_words'})
			explode(/\,/,$this->{'config'}->{'ng_words'})
		);
		$this->{'input'}->{'msg'} =~ s/\n/<br>/g;
		$this->{'tmpl'}->{'main'}->PrintBlock(
			'subtitle','nos',
			'subtitle = 確認表示'
		);
		$this->{'tmpl'}->{'main'}->PrintBlock(
			'confirm','nos',
			$form->('subj'),
			$form->('name'),
			$form->('mail'),
			$form->('msg'),
			$form->('url'),
		);
	}
	
	#元の記事
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'subtitle','nos',
		"subtitle = 元の記事"
	);
	$this->out_rec_block('rec',undef,1);
	
	
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




# →編集その１
sub process_edit {
	my $this = shift;
	
	
	if(
		!$this->{'input'}->{'bnum'}
		||
		!$this->{'input'}->{'num'}
	)
	{ $this->error('Q'); }
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('SH')) { $this->error('B'); }
	
	
	#設定値
	if(!$this->{'data'}->{'file'}->{'config'}->Open) { $this->error('F'); }
	if(
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'bgm'
		)
	)
	{ $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#ヘッダ
	$this->out_header_block;
	
	
	#削除フォーム
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'edit','nos',
		'bnum = '.$this->{'input'}->{'bnum'},
		'num = '.$this->{'input'}->{'num'}
	);
	
	
	#フッタ
	$this->out_footer_block;
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
		if(length($this->{'input'}->{'subj'}) > $this->{'config'}->{'max_char'})
		{ push @err_msg,('題名の文字数オーバーです。'); }
		
		if(length($this->{'input'}->{'name'}) > $this->{'config'}->{'max_char'})
		{ push @err_msg,('名前の文字数オーバーです。'); }
		
		if(length($this->{'input'}->{'mail'}) > $this->{'config'}->{'max_char'})
		{ push @err_msg,('メールの文字数オーバーです。'); }
		
		if(length($this->{'input'}->{'url'}) > $this->{'config'}->{'max_char'})
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




1;
