

#============================================================================#
#
# 情報表示機能提供ファイル
# (C) Apps Page & YOSUKE TOBITA.
#
#============================================================================#




#use strict;




# ↓書込み処理クラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::main::info;




# →留意事項
sub process_readme {
	my $this = shift;
	
	
	########## 区切 ##########
	
	
	if(!$this->{'lock'}->lock('SH')) { $this->error('B'); }
	
	
	#設定値
	if(
		!$this->{'data'}->{'file'}->{'config'}->Open
		||
		!$this->{'data'}->{'file'}->{'config'}->Load(
			$this->{'config'},
			'display',
			'writing_readme',
			'bgm'
		)
	) { $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	if($this->{'config'}->{'readme'} eq '') {
		$this->{'config'}->{'readme'} = '特にありません。';
	}
	else {
		$this->{'config'}->{'readme'} =~ s/\0/<br>/g;
		$this->{'config'}->{'readme'} =~ s/&lt;/</g;
		$this->{'config'}->{'readme'} =~ s/&gt;/>/g;
		$this->{'config'}->{'readme'} =~ s/&quot;/\"/g;
	}
	
	#ヘッダ
	$this->out_header_block;
	
	
	print "\n";
	#留意事項ブロック
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'readme','nos',
		'readme = '.$this->{'config'}->{'readme'}
	);
	
	
	#フッタ
	$this->out_footer_block;
}




# →使い方
sub process_usage {
	my $this = shift;
	my $rep1 = sub {
		if($this->{'config'}->{$_[0]}) { return $_[0].' = *必須'; }
		return $_[0].' = 任意';
	};
	my $rep2 = sub {
		return $_[0].' = '.$this->{'config'}->{$_[0]};
	};
	my $rep3 = sub {
		if($this->{'config'}->{$_[0]}) { return $_[0].' = 有効'; }
		return $_[0].' = 無効';
	};
	my $rep4 = sub {
		if($this->{'config'}->{$_[0]}) {
			return 
				$_[0].' = 、全角'.
				($this->{'config'}->{$_[0]} / 2).
				'文字位まで'
			;
		}
		return $_[0].' = ';
	};
	
	
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
			'bgm'
		)
	) { $this->error('F'); }
	$this->{'data'}->{'file'}->{'config'}->Close;
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#加工
	if(!$this->{'config'}->{'tag'}) {
		$this->{'config'}->{'ng_words'} = '無効の為、省略';
	}
	if(!$this->{'config'}->{'max_res'}) {
		$this->{'config'}->{'max_res'} = '--';
	}
	if(!$this->{'config'}->{'session_interval'}) {
		$this->{'config'}->{'session_interval'} = '--';
	}
	
	
	########## 区切 ##########
	
	
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
	
	
	#ヘッダ
	$this->out_header_block;
	
	
	#使い方ブロック
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'usage','nos',
		
		#入力項目の設定
		$rep1->('check_subj'),
		$rep1->('check_name'),
		$rep1->('check_mail'),
		$rep1->('check_url'),
		$rep1->('check_msg'),
		$rep1->('std_subj'),
		$rep4->('max_char'),
		$rep4->('max_msg'),
		
		#記事の設定
		$rep2->('max_recs'),
		$rep2->('page_recs'),
		$rep2->('max_res'),
		$rep2->('new_time'),
		
		#連続投稿の設定
		$rep2->('session_interval'),
		
		#HTMLタグの設定
		$rep3->('tag'),
		$rep2->('ng_words'),
		
		#オートリンクの設定 
		$rep3->('auto_link'),
		
		"folder_mark = ".$this->{'folder_mark'}
	);
	
	
	#フッタ
	$this->out_footer_block;
}




1;
