#タイトル
$title = 'Ozzy\'s';

#あなたのホームページに戻るためのＵＲＬ
$homepage = 'http://www.ozzys.jp/';

#管理者
$pass = '1';			#アクセスログを見るのにパスワードが必要か？(Yes=0,No=1)
$pass_key = 'alphatec';		#パスワードを設定した時のパスワード

#IPチェック
@IP = ('','');	#記入したIPは記録しない。

#カウンター制御　同じ人がカウントアップさせない。
$coun_up1 = 0;		#日付が変わる毎にOK！(no = 0 , yes = 1 , 時間でOK = 2 連続でなければOK = 3)
$count_up2 = 1;		#$coun_up1で2を選択した場合。時間を記入

#元のカウンター数
$cunt_f = '';

#カウンター表示
$counth = 0;		#カウンター表示　(gif = 0 , text = 1)
$countk = 5;		#カウンター表示数

#GIFカウンター画像の置いてある場所（フルパスで）
$cun_image = 'http://www.ozzys.jp/count/image/count'; #フルパスで(SSI用)
$cun_image2 = './image/count'; #このcgiのファイルの場所から。
#クッキーを格納する名前を設定する
$CookieName = 'alpha';

#SSIを利用するか？
$ssi = 0;	#yes = 0, no = 1

#ロック
$lockkey  = 2;			# ファイルロック形式 (0=no 1=symlink関数 2=open関数)

#cgiファイル
$script  = 'look.cgi';		#閲覧用cgi
$script2 = 'access.cgi';	#データー記録cgi
$script3 = 'pass.cgi';		#パスワード設定cgi

$salt = 'al';

#データーが無くても他の月日が見れるか？
$ne = 0;	#yes:0,no:1

#個々の詳細データーの最初の表示(非表示 0,表示 1)
$a_1 = 1;	#リンク元
$a_2 = 1;	#キーワード
$a_3 = 0;	#OS種類
$a_4 = 0;	#ブラウザー種類
$a_5 = 0;	#プロバイダー
$a_6 = 0;	#プロキシ

#データーファイル
$logfile_a = "./log/count.dat";			#カウンター総数

$bgcolor="#ffffff";

$hk = 50;	#情報表示数

@MON = ('0','31','28','31','30','31','30','31','31','30','31','30','31');
@WEEK = ('日','月','火','水','木','金','土');

#祭日設定
sub holyday {
		if (($m == 1) && ($d1 == 1)){ $flags = 1; }
		if (($m == 2) && ($d1 == 11)){ $flags = 1; }
		if (($m == 4) && ($d1 == 29)){ $flags = 1; }
		if (($m == 5) && ($d1 == 3)){ $flags = 1; }
		if (($m == 5) && ($d1 == 4)){ $flags = 1; }
		if (($m == 5) && ($d1 == 5)){ $flags = 1; }
		if (($m == 7) && ($d1 == 20)){ $flags = 1; }
		if (($m == 9) && ($d1 == 15)){ $flags = 1; }
		if (($m == 11) && ($d1 == 3)){ $flags = 1; }
		if (($m == 11) && ($d1 == 23)){ $flags = 1; }
		if (($m == 12) && ($d1 == 23)){ $flags = 1; }


		$idou3 = (($y - 2000) * 0.242194);
		$uru3 = int(($y - 2000) / 4);
		$hol3 = int(20.69115 + $idou3 - $uru3);
		if (($m == 3) && ($d1 == $hol3)){ $flags = 1; }

		$idou9 = (($y - 2000) * 0.242194);
		$uru9 = int(($y - 2000) / 4);
		$hol9 = int(23.09 + $idou9 - $uru9);
		if (($m == 9) && ($d1 == $hol9)){ $flags = 1; }

		if (($m == 1) && ($d1 >= 8) && ($d1 <= 14) && ($amari == 2)){ $flags = 1; }
		if (($m == 10) && ($d1 >= 8) && ($d1 <= 14) && ($amari == 2)){ $flags = 1; }

}

1;
