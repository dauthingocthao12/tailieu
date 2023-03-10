@SEAR  = ('search.yahoo.co.jp',
	'infoseek.co.jp',
	'goo.ne.jp',
	'excite.co.jp',#s= ?
	'lycos.co.jp',
	'search.biglobe.ne.jp',
	'fresheye.com',
	'infoweb.ne.jp',
	'csj.co.jp',
	'altavista.com',
	'netjoy.ne.jp',
	'wave.co.jp',
	'isize.com',
	'msn.co.jp',
	'odn.ne.jp',
	'202.222.107.225',#i-con search
	'yappo.ne.jp',
	'google.com',
	'ohnew.co.jp',
	'scissors.nu',
	'rental.chat.co.jp',
	'google.yahoo.co.jp',
	'a1s.co.jp/asp/gunma',
	'nifty.com',
	'search-intl.netscape.com',
	'kensaku.org',
	'sougou.niigata-inet.or.jp',
	'hse.43n.net',
	'google.co.jp',
	'okinawa.to',
	'odin.ingrid.org');

@KEY = ('p=','qt=','MT=','search=','query=','q=','kw=','Querystring=','key=','q=',
	'key=','keyword=','SearchText=','q=','QueryString=','k3x=','k=','q=','k=','key=',
	'word=','p=','SearchStrings=','Text=','search=','key=','keyword=','KEY=','q=','word=','KEY=');

sub keyword {

		if (index($referer,"\?") > 0) { 
			($search,$key_w) = split(/\?/, $referer);
				$i = 0;
				$key = "";
				foreach $sear(@SEAR) {
					if (index($search,$sear) > 0) { $key = $KEY[$i]; last;}
					++$i;
						}
			@KEY_W = split(/&/, $key_w);
				foreach $word(@KEY_W) {
				if ($key eq "") { push (@errer,$referer); last;}
					if (index($word,$key) >= 0) {
						$word =~ s/$key//g ;
						$word =~ s/amp;//g ; 
						$word =~ s/\+/ /g ; 
						$word =~ s/\|/ /g ; 
						$word =~ s/\Åb/ /g ; 
						$word =~ s/Å@/ /g ; 
						$word =~ s/  / /g ; 
						$word =~ s/\s+$// ; 
							if ($word ne "") { $WORD{$word}++; $wo++; }
						last;
								}
							}
						}

}

1;
