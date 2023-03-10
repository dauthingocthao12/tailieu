

#============================================================================#
#
# 書込み処理、管理用処理共通のクラスをまとめたファイル
# (C) Apps Page & YOSUKE TOBITA.
#
#============================================================================#




#use strict;




# ↓datafile::recs_treeの書込み機能提供クラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::datafile::recs_tree::insert;




export appspage::treecrsdx::oo:: 'datafile::recs_tree';




# →新規追加
# in(最大件数、番号を除く新しいレコード)
# out(0|1)
sub InsertNewrec {
	my $this = shift;
	my($max,@newrec) = @_;
	my($ln,$cnt);
	
	
	$max--;
	
	
	if(!seek(*{$this->{'IN'}},0,0))  { return 0; }
	if(!seek(*{$this->{'OUT'}},0,0)) { return 0; }
	
	
	chomp($cnt = readline *{$this->{'IN'}});
	$cnt++;
	
	print {$this->{'OUT'}} ($cnt,"\n");
	print {$this->{'OUT'}} (
		$cnt,$this->{'bnd'},
		$cnt,$this->{'bnd'},
		$cnt,$this->{'bnd'},
		'1',$this->{'bnd'},
		join($this->{'bnd'},@newrec),"\n"
	);
	
	if($ln = readline *{$this->{'IN'}}) {
		LOOP1: while($max--) {
			print {$this->{'OUT'}} ($ln);
			
			#子レコード
			LOOP2: while($ln = readline *{$this->{'IN'}}) {
				if(
					$ln =~/^(.+?)$this->{'bnd'}(.+?)$this->{'bnd'}(.+?)$this->{'bnd'}.+/
					&&
					$1 ne $3
				) {
					print {$this->{'OUT'}} ($ln);
				}
				else { last LOOP2; }
			}
			if($ln eq '') { last LOOP1; }
		}
	}
	
	return 1;
}




# →既存レコードへ追加
# メモ：Fetchしてから、実行する
# in(
#    追加後の位置 '0'そのまま | '1'先頭に移動
#    親番号の位置 Fetchで取得したファイルポインタ
#    所属番号、
#    親番号、
#    親レコードの階層
#    レコード番号を除く新しいレコード
# )
# out(0 失敗 |1 成功)
sub InsertChildrec {
	my $this = shift;
	my($moveto_top,$fpointer,$bnum,$pnum,@newrec) = @_;
	my($ln,$prec,$cnt,$check,@recs,$lastpos,$x);
	
	#番号など
	if(!$fpointer || !$bnum || !$pnum){ return 0; }
	
	if(!seek(*{$this->{'IN'}},0,0))  { return 0; }
	if(!seek(*{$this->{'OUT'}},0,0)) { return 0; }
	
	chomp($cnt = readline *{$this->{'IN'}});
	$cnt++;
	print {$this->{'OUT'}} ($cnt,"\n");
	
	my $newline;
	my $depth;
	my $flag;
	my ($lpnum,$lnum,$ldepth);
	
	#位置調整
	if(!seek(*{$this->{'IN'}},$fpointer,0)) { return 0; }
	
	#挿入
	while($ln = readline *{$this->{'IN'}}) {
		if(
			$ln =~
			/^$bnum$this->{'bnd'}(.+?)$this->{'bnd'}(.+?)$this->{'bnd'}(.+?)$this->{'bnd'}.+/
		) {
			$lpnum = $1;
			$lnum = $2;
			$ldepth = $3;
		}
		else {
			$lastpos = (tell *{$this->{'IN'}}) - length($ln);
			last;
		}
		
		
		#親記事と階層
		if(
			!$flag
			&&
			$pnum eq $lnum
		)
		{
			$check = 1;
			$depth = $ldepth + 1;
			$flag = 1;
			#追加記事作成
			$newline =
				$bnum.$this->{'bnd'}.
				$pnum.$this->{'bnd'}.
				$cnt.$this->{'bnd'}.
				$depth.$this->{'bnd'}.
				join($this->{'bnd'},@newrec)."\n"
			;
			undef @newrec;
			push @recs,$ln;
			next;
		}
		if(
			$flag eq '1'
			&&
			$ldepth < $depth
		)
		{
			push @recs,$newline;
			undef $newline;
			$flag = 2;
		}
		push @recs,$ln;
	}
	if($flag eq '1') { push @recs,$newline; }
	
	
	#先頭へ移動
	if($check && $moveto_top) {
		#関連記事
		print {$this->{'OUT'}} @recs;
		
		#残り
		if(!seek(*{$this->{'IN'}},0,0)) { return 0; }
		$ln = readline *{$this->{'IN'}};
		while($ln = readline *{$this->{'IN'}}) {
			if($ln !~ /^$bnum$this->{'bnd'}.+/) {
				print {$this->{'OUT'}} $ln;
			}
		}
	}
	#そのまま
	elsif($check) {
		if(!seek(*{$this->{'IN'}},0,0)) { return 0; }
		$ln = readline *{$this->{'IN'}};
		
		#前の記事
		while($ln = readline *{$this->{'IN'}}) {
			if($ln !~ /^$bnum$this->{'bnd'}.+/) {
				print {$this->{'OUT'}} $ln;
			}
			else { last; }
		}
		
		#関連記事
		print {$this->{'OUT'}} @recs;
		
		#残り
		if(!seek(*{$this->{'IN'}},$lastpos,0)) { return 0; }
		while($ln = readline *{$this->{'IN'}}) {
			print {$this->{'OUT'}} $ln;
		}
	}
	return $check;
}




# ↓datafile::recs_treeの編集機能提供クラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::datafile::recs_tree::edit;




export appspage::treecrsdx::oo:: 'datafile::recs_tree';




# →消す
# in("親記事番号.記事番号"...)
# out(エラー0|通常1|削除済2)
sub Delete {
	my $this = shift,
	my(@bnums) = @_;
	my ($x,$ln);
	my $check = 2;
	
	
	if(!seek(*{$this->{'IN'}},0,0))  { return 0; }
	if(!seek(*{$this->{'OUT'}},0,0)) { return 0; }
	
	
	$ln = readline(*{$this->{'IN'}});
	print {$this->{'OUT'}} $ln;
	
	
	LOOP1: while($ln = readline(*{$this->{'IN'}})) {
		LOOP2: for ($x = 0; $x < @bnums; $x++) {
			if($ln =~ /^$bnums[$x]$this->{'bnd'}.+/) {
				$check = 1;
				next LOOP1;
			}
		}
		print {$this->{'OUT'}} $ln;
	}
	
	return $check;
}




# →更新
# in(
#   所属、
#   親番号、
#   番号、
#   項目...)
# out(エラー0|通常1|削除済2)
sub Update {
	my $this = shift;
	my($bnum,$pnum,$num,@fields) = @_; undef @_;
	my $check = 2;
	
	
	if(!seek(*{$this->{'IN'}},0,0))  { return 0; }
	if(!seek(*{$this->{'OUT'}},0,0)) { return 0; }
	
	
	my $ln = readline *{$this->{'IN'}};
	print {$this->{'OUT'}} $ln;
	while($ln = readline *{$this->{'IN'}}) {
		if(
			$ln =~
			/^$bnum$this->{'bnd'}$pnum$this->{'bnd'}$num$this->{'bnd'}.+/
		) {
			chomp($ln);
			print {$this->{'OUT'}}
				join(
					$this->{'bnd'},$bnum,$pnum,$num,@fields
				)."\n"
			;
			$check = 1;last;
		}
		print {$this->{'OUT'}} $ln;
	}
	
	#残り
	while($ln = readline *{$this->{'IN'}}) { print {$this->{'OUT'}} $ln; }
	
	return $check;
}




# ↓datafile::*のファイル更新機能提供クラス
#----------------------------------------------------------------------------#
package appspage::treecrsdx::datafile::update;




export appspage::treecrsdx::oo:: 'datafile';




# →作業ファイ作成
# in(アレ,方法)
sub tmpfileopen {
	my $this = shift;
	my ($that,$newfile) = @_;
	$that->{'OUT'}  = new appspage::treecrsdx::handle::;
	
	my $newf = $this->{'tmp'}.'tmp_'.$that->{'name'}.$this->{'ext'};
	my $oldf = $that->{'dir'}.$that->{'name'}.$this->{'ext'};
	
	if(!open(*{$that->{'OUT'}},'+>'.$newf))
	{ return 0; }
	
	my $mode;
	
	#非NFS環境では、パーミッション操作なし
	if( $this->{'su_mode'} ne '2' ) {
		if(!$this->{'su_mode'}) {
			if(!chmod(0666,$newf)) { return 0; }
		}
		elsif($newfile) {
			if(!chmod(0644,$newf)) { return 0; }
		}
		else {
			$mode = (stat $oldf)[2];
			if(!$mode || !chmod($mode,$newf)) { return 0; }
		}
	}
	
	#更新モード
	if(!$newfile) { $that->{'ready'} = 1; }
	#作成モード
	else { $that->{'ready'} = 2; }
	#削除モード
	#$that->{'ready'} = 3;
	
	$this->{'sync'}->{$that->{'name'}} = $that;
	return 1;
}




# →更新処理
# in([回数])
# out(0|1)
sub Sync {
	my $this = shift;
	my($retry) = @_;
	if(!$retry) { $retry = 5; }
	my($k,$v,$x,$y);
	my($check1,@check,$failed,$that);
	my @sync = keys %{$this->{'sync'}};
	my($IN,$OUT);
	my $back = sub {
		if($that->{'ready'} eq '1') {
			return $that->{'dir'}.'bk_'.$sync[$_[0]].$this->{'ext'};
		}
		else {
			return $that->{'dir'}.'bk_'.$that->{'filename'};
		}
	};
	my $oldf = sub {
		if($that->{'ready'} eq '1') {
			$that->{'dir'}.$sync[$_[0]].$this->{'ext'};
		}
		else {
			return $that->{'dir'}.$that->{'filename'};
		}
	};
	my $newf = sub {
		$this->{'tmp'}.'tmp_'.$sync[$_[0]].$this->{'ext'};
	};
	my $sleep= sub { select(undef,undef,undef,0.05); };
	my $switch = sub {
		$OUT = $_[0]->{'IN'};
		$IN  = $_[0]->{'OUT'};
		$_[0]->{'OUT'} = $OUT;
		$_[0]->{'IN'}  = $IN;
		close *{$_[0]->{'OUT'}};
	};
	
	
	LOOP1: for($x = 0; $x < @sync; $x++) {
		$check1 = 0;
		$that = $this->{'sync'}->{$sync[$x]};
		LOOP2: for($y = 0; $y < $retry; $y++) {
			if(!$check1) {
				if(
					$that->{'ready'} eq '2'
					||
					rename($oldf->($x),$back->($x))
				) { $check1 = 1; }
			}
			if($check1 eq '1') {
				if(
					$that->{'ready'} eq '3'
					||
					rename($newf->($x),$oldf->($x))
				) {
					$check1 = 2;
					last LOOP2;
				}
			}
			$sleep->();
		}
		$check[$x] = $check1;
		if($check1 ne '2') {
			$failed = 1;
			last LOOP1;
		}
	}
	if(!$failed) {
		LOOP1: for($x = 0; $x < @check; $x++) {
			$that = $this->{'sync'}->{$sync[$x]};
			LOOP3: for($y = 0; $y < $retry; $y++) {
				if(
					$that->{'ready'} eq '2'
					||
					unlink $back->($x)
				) {
					last LOOP3;
				}
				$sleep->();
			}
			if($that->{'ready'} eq '1') {
				$switch->($this->{'sync'}->{$sync[$x]});
			}
			if($that->{'ready'} eq '2') {
				close *{$that->{'OUT'}};
			}
		}
		$this->{'sync'} = {};
	}
	else {
		LOOP1: for($x = 0; $x < @check; $x++) {
			$that = $this->{'sync'}->{$sync[$x]};
			LOOP2: for($y = 0; $y < $retry; $y++) {
				if(
					$that->{'ready'} eq '2'
					||
					rename($back->($x),$oldf->($x))
				) { last LOOP2; }
			}
			LOOP3: for($y = 0; $y < $retry; $y++) {
				if(
					$that->{'ready'} eq '3'
					||
					unlink($newf->($x))
				) { last LOOP3; }
			}
		}
		return 0;
	}
	return 1;
}




# →更新中止
sub Suspend {
	my $this = shift;
	my ($k,$v);
	while(($k,$v) = each %{$this->{'sync'}}) {
		if($v->{'ready'} eq '3') { next; }
		unlink $this->{'tmp'}.'tmp_'.$k.$this->{'ext'};
		close *{$v->{'OUT'}};
	}
	$this->{'sync'} = {};
}




1;
