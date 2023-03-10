

#============================================================================#
#
# 検索機能提供ファイル
# (C) Apps Page & YOSUKE TOBITA.
#
#============================================================================#




#use strict;




# ↓検索のクラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::main::search;




# →記事検索
sub process_search {
	my $this = shift;
	my $select =
	sub { return $_[0].'.'.$this->{'input'}->{$_[0]}.' =  selected'; };
	
	
	#日本語変換
	$this->{'funcs'}->jcode_convert(
		$this->{'JCODE'},
		'sjis',
		$this->{'input'},
		$this->{'LIB_DIR'}
	);
	
	
	#入力検査
	$this->{'input'}->{'keywords'} =~ s/　/ /g;
	while($this->{'input'}->{'keywords'} =~ s/  / /g) {}
	while($this->{'input'}->{'keywords'} =~ s/^ //g) {}
	while(substr($this->{'input'}->{'keywords'},-1) eq ' ')
	{ chop($this->{'input'}->{'keywords'}); }
	
	
	#項目番号
	my $field_num = 0;
	if   ($this->{'input'}->{'field'} eq 'all')  { $field_num = 0; }
	elsif($this->{'input'}->{'field'} eq 'msg')  { $field_num = 11; }
	elsif($this->{'input'}->{'field'} eq 'subj') { $field_num = 5; }
	elsif($this->{'input'}->{'field'} eq 'name') { $field_num = 6; }
	elsif($this->{'input'}->{'field'} eq 'mail') { $field_num = 9; }
	elsif($this->{'input'}->{'field'} eq 'url')  { $field_num = 10; }
	
	
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
	
	
	#検索
	my $rescheck = {};
	if($this->{'input'}->{'keywords'}) {
		#開く
		if(!$this->{'data'}->{'file'}->{'recs'}->Open) { $this->error('F'); }
		
		
		#検索
		$this->{'data'}->{'file'}->{'recs'}->Select(
			$this->{'input'}->{'keywords'},
			$field_num,
			$this->{'input'}->{'logic'},
			$this->{'input'}->{'max'},
			$this->{'recs'},
			$rescheck
		);
		
		
		#閉じる
		$this->{'data'}->{'file'}->{'recs'}->Close;
		
	}
	
	
	$this->{'lock'}->unlock;
	
	
	########## 区切 ##########
	
	
	#ヘッダ
	$this->out_header_block();
	
	
	#マーク
	$this->{'tmpl'}->{'main'}->MarkBlock('rec','mem');
	
	#検索フォームブロック
	$this->{'tmpl'}->{'main'}->PrintBlock(
		'search_form','nos',
		'keywords = '.$this->{'input'}->{'keywords'},
		$select->('field'),
		$select->('logic'),
		$select->('max')
	);
	
	
	#結果
	my ($block,$resmode,$x);
	if($this->{'input'}->{'keywords'} ne '') {
		if(@{$this->{'recs'}}) {
			LOOP1: for($x = 0; $x < @{$this->{'recs'}}; $x++) {
				$this->parse_fields(\$this->{'recs'}->[$x]);
				
				#返事数
				if(
					!$this->{'config'}->{'max_res'}
					||
					$rescheck->{$this->{'fields'}->{'bnum'}}
					<
					$this->{'config'}->{'max_res'}
				)
				{ $resmode = 'resform'; }
				else { $resmode = 'resfull'; }
				
				
				$this->out_rec_block('rec',$resmode);
				
			}
		}
		else {
			$this->{'tmpl'}->{'main'}->PrintBlock(
				'search_notfound','nos',
			);
		}
	}
	else {
		$this->{'tmpl'}->{'main'}->PrintBlock(
			'search_usage','nos',
		);
	}
	
	
	#フッタ
	$this->out_footer_block;
}




#============================================================================#
#
# 区切
#
#============================================================================#




# ↓datafile::bbs_rの検索機能提供クラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::datafile::recs_tree::search;




export appspage::treecrsdx::oo:: 'datafile::recs_tree';




# → 検索
# in(
#    スペース区切りキーワード、
#    項目番号、
#    OR|AND、
#    件数、
#    配列リファレンス
#    ハッシュリファレンス返事数カウント用
# )
# out(なし)
sub Select { 
	my $this = shift;
	my($keys,$filed_num,$logic,$max,$ref,$resref) = @_;
	my($line,$field,$match,@keywords,$x,$bnum,$resflag);
	
	my $res_count = sub {
		if(!$max) { return; }
		if($resflag) {
			if($line =~ /^$bnum$this->{'bnd'}.+/) {
				$resref->{$bnum}++;
			}
			elsif(
				$line =~ /^(.+?)$this->{'bnd'}(.+?)$this->{'bnd'}(.+?)$this->{'bnd'}.+/
				&&
				$1 eq $3
			) {
				$bnum = $1;
				$resref->{$bnum} = 0;
			}
			else {
				$resflag = 0;
			}
			return;
		}
		elsif(
			$line =~ /^(.+?)$this->{'bnd'}(.+?)$this->{'bnd'}(.+?)$this->{'bnd'}.+/
			&&
			$1 eq $3
		) {
			$resflag = 1;
			$bnum = $1;
			$resref->{$bnum} = 0;
		}
		return;
	};
	
	
##	@_ = split(/ /,$keys);
	@_ = explode(/ /,$keys);
	for ($x = 0; $x < @_; $x++) {
		if($_[$x] ne '') { push @keywords,(quotemeta $_[$x]); }
	}
	
	
	if(!seek(*{$this->{'IN'}},0,0)) { return 0; }
	
	
	$line =  readline *{$this->{'IN'}};
	
	
	LOOP1: while($line = readline *{$this->{'IN'}}) {
		if($filed_num) {
##			$field = (split(/$this->{'bnd'}/,$line))[$filed_num];
			$field = (explode(/$this->{'bnd'}/,$line))[$filed_num];
			$match = \$field;
		}
		else {
			$match = \$line;
		}
		
		$res_count->();
		
		#AND
		if($logic eq 'and' && @{$ref} < $max) {
			LOOP2: for ($x = 0; $x < @keywords; $x++) {
				if(${$match} !~ /$keywords[$x]/) {
					next LOOP1;
				}
			}
			push @{$ref},$line;
		}
		#OR
		elsif(@{$ref} < $max) {
			LOOP3: for ($x = 0; $x < @keywords; $x++) {
				if(${$match} =~ /$keywords[$x]/) {
					push @{$ref},$line;
					last LOOP3;
				}
			}
		}
	}
	return 1;
}




1;
