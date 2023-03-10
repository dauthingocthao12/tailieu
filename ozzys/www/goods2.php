<?PHP
//	商品表示メインプログラム

	include("../cone.inc");
	include("./sub/setup.inc");
	include("./sub/array.inc");

//	session_cache_limiter('public');
	session_start();
//	session_register("idpass","customer");

	$PHP_SELF = $_SERVER['PHP_SELF'];

	//	ログアウトチェック
##	if (eregi("out$",$PHP_SELF)) {
	if (preg_match("/out$/i",$PHP_SELF)) {
##		$PHP_SELF = eregi_replace("/out$","",$PHP_SELF);
		$PHP_SELF = preg_replace("/\/out$/i","",$PHP_SELF);
##		$PHP_SELF = eregi_replace("out$","",$PHP_SELF);
		$PHP_SELF = preg_replace("/out$/i","",$PHP_SELF);
		unset($idpass);
		unset($_SESSION['idpass']);
		unset($_COOKIE['idpass']);
		setcookie("idpass");
	}

	//	スラッシュチェック
##	if (!eregi(".html$",$PHP_SELF) && !eregi(".htm$",$PHP_SELF) && !eregi("/$",$PHP_SELF)) {
	if (!preg_match("/\.html$/i",$PHP_SELF) && !preg_match("/\.htm$/i",$PHP_SELF) && !preg_match("/\/$/i",$PHP_SELF)) {
		$sent_url = $URL . $PHP_SELF . "/";
		header ("Location: $sent_url\n\n");
	}

	//	階層データー読込
	$LIST = explode("/",$PHP_SELF);
	$LIST[0] = $LIST[1] = "";
	$VALUE = array();
	$i=1;
	$CHECK = array();
	foreach ($LIST AS $VAL) {
		if ($VAL != "") {
##			if (eregi(".htm",$VAL)) {
			if (preg_match("/\.htm/i",$VAL)) {
				$VALUE[$i] = $VAL;
				break;
			}
##			elseif (eregi("^s",$VAL)) {
			elseif (preg_match("/^s/i",$VAL)) {
				if ($CHECK[s] || $CHECK[n] || $CHECK[g] || $i > 3) { continue; }
				else {
					$CHECK[s] = $VAL;
					$VALUE[$i] = $VAL;
				}
			}
##			elseif (eregi("^m",$VAL)) {
			elseif (preg_match("/^m/i",$VAL)) {
				if ($CHECK[m] || $CHECK[n] || $CHECK[g] || $i > 3) { continue; }
				else {
					$CHECK[m] = $VAL;
					$VALUE[$i] = $VAL;
				}
			}
##			elseif (eregi("^n",$VAL)) {
			elseif (preg_match("/^n/i",$VAL)) {
				if ($CHECK[n] || $CHECK[g] || $i > 4) { continue; }
				else {
					$CHECK[n] = $VAL;
					$VALUE[$i] = $VAL;
				}
			}
##			elseif (eregi("^g",$VAL)) {
			elseif (preg_match("/^g/i",$VAL)) {
				if ($CHECK[g]) { continue; }
				else {
					$CHECK[g] = $VAL;
					$VALUE[$i] = $VAL;
				}
			}
##			elseif (!eregi("[^0-9]",$VAL)) {
			elseif (!preg_match("/[^0-9]/i",$VAL)) {
				if ($CHECK[main] || $CHECK[s] || $i > 2) { continue; }
				else {
					$CHECK[main] = $VAL;
					$VALUE[$i] = $VAL;
				}
			}
			else { break; }
			$i++;
		}
	}

	$main = "";
	//	ページリンク表示
	if ($CHECK) {
		include("./sub/linktitle.inc");
		list($title,$main) = linktitle($CHECK);
	}

	if ($CHECK[g]) {	//	商品詳細
//		include("./sub/syousai.inc");	//test del okb 2016/07/11
include("./sub/syousai2.inc");	// test add okb 2016/07/11
		$syousai = syousai($VALUE,$CHECK);
		$main .= $syousai;
//echo("1<BR>\n");
	}
	elseif ($CHECK[n]) {	//	商品一覧
//		include("./sub/goodsname.inc");	//test del okb 2016/07/08
include("./sub/goodsname2.inc");	// test add okb 2016/07/08
		$goodsname = goodsname($VALUE,$CHECK);
		$main .= $goodsname;
//echo("2<BR>\n");
	}
	elseif ($CHECK[m]) {
		if ($CHECK[s]) {	//	商品名一覧
			include("./sub/goodslist.inc");
			$goodslist = goodslist($VALUE,$CHECK);	//test del okb 2016/07/08
//			include("./sub/goodsname.inc");	
include("./sub/goodsname2.inc");	// test add okb 2016/07/08
			$goodsname = goodsname($VALUE,$CHECK);
			$main .= $goodslist . $goodsname;
//echo("3<BR>\n");
		}
		elseif ($CHECK[main]) {	//	サブカテゴリー＆商品名一覧
			include("./sub/sublist.inc");
			$sublist = sublist($VALUE,$CHECK);
			include("./sub/goodslist.inc");
			$goodslist = goodslist($VALUE,$CHECK);
			$main .= $sublist . $goodslist;
//echo("4<BR>\n");
		}
		else {	//	メインカテゴリー＆商品名一覧
			include("./sub/mainlist.inc");
			$mainlist = mainlist($VALUE,$CHECK);
			include("./sub/goodslist.inc");
			$goodslist = goodslist($VALUE,$CHECK);
			$main .= $mainlist . $goodslist;
//echo("5<BR>\n");
		}
	}
	elseif ($CHECK[s]) {	//	メーカー名一覧＆商品名一覧
		include("./sub/makerlist.inc");
		$makerlist = makerlist($VALUE,$CHECK);
		include("./sub/goodslist.inc");
		$goodslist = goodslist($VALUE,$CHECK);
		$main .= $makerlist . $goodslist;
//echo("6<BR>\n");
	}
	elseif ($CHECK[main]) {	//	サブカテゴリー＆メーカー名一覧
		include("./sub/sublist.inc");
		$sublist = sublist($VALUE,$CHECK);
		include("./sub/makerlist.inc");
		$makerlist = makerlist($VALUE,$CHECK);
		$main .= $sublist . $makerlist;
//echo("7<BR>\n");
	}
	else {
		$value = $VALUE[1];
##		if (!$CHECK && eregi("^$m_cate_file",$value)) {	//	メインカテゴリー
		if (!$CHECK && preg_match("/^$m_cate_file/i",$value)) {	//	メインカテゴリー
			include("./sub/mainlist.inc");
			$mainlist = mainlist($VALUE,$CHECK);
			$main .= $mainlist;
			$title = "カテゴリー 一覧";
//echo("8<BR>\n");
		}
##		elseif (!$CHECK && eregi("^$m_maker_file",$value)) {	//	メーカー一覧
		elseif (!$CHECK && preg_match("/^$m_maker_file/i",$value)) {	//	メーカー一覧
			include("./sub/makerlist.inc");
			$makerlist = makerlist($VALUE,$CHECK);
			$main .= $makerlist;
			$title = "メーカー 一覧";
//echo("9<BR>\n");
		}
		else {	//	メインカテゴリー＆メーカー一覧
			include("./sub/mainlist.inc");
			$mainlist = mainlist($VALUE,$CHECK);
			include("./sub/makerlist.inc");
			$makerlist = makerlist($VALUE,$CHECK);
			$main .= $mainlist . $makerlist;
			$title = "カテゴリー・メーカー 一覧";
//echo("10<BR>\n");
		}
	}


	//	ひな形読込
	$html = "";
	if (file_exists($hina_file)) {
		$fp = fopen ($hina_file, "r");
		if ($fp) {
			while (!feof ($fp)) {
				$html .= fgets($fp, 4096);
			}
		}
	}

	//	ページタイトル
##	$html = eregi_replace("<!--TITLE-->",$title,$html);
	$html = preg_replace("/<!--TITLE-->/i",$title,$html);

	//	ヘッド背景画
	include("./sub/headimg.inc");
	$headimg = headimg($HEADIMG);
##	$html = eregi_replace("<!--HEADIMG-->",$headimg,$html);
	$html = preg_replace("/<!--HEADIMG-->/i",$headimg,$html);

	//	ヘッドメッセージ
	include("./sub/headmsg.inc");
	$headmsg = headmsg($LOGDATA_DIR,$headmsg_file);
##	$html = eregi_replace("<!--HEADMSG-->",$headmsg,$html);
	$html = preg_replace("/<!--HEADMSG-->/i",$headmsg,$html);
	//	ログインメッセージ
	include("./sub/loginmsg.inc");
	$burl = "";
	$loginmsg = loginmsg($burl);
##	$html = eregi_replace("<!--LOGIN-->",$loginmsg,$html);
	$html = preg_replace("/<!--LOGIN-->/i",$loginmsg,$html);

	//	メニュー
	include("./sub/menu.inc");
	$menu = menu($LOGDATA_DIR,$menu_file);
##	$html = eregi_replace("<!--MENU-->",$menu,$html);	//	メニュー
	$html = preg_replace("/<!--MENU-->/i",$menu,$html);	//	メニュー

	//	メイン内容
##	$html = eregi_replace("<!--MAIN-->",$main,$html);
	$html = preg_replace("/<!--MAIN-->/i",$main,$html);

	//	お勧め
	include("./sub/osusume.inc");
	$LIST = "";
	$osusume = osusume($LIST);
##	$html = eregi_replace("<!--OSUSUME-->",$osusume,$html);
	$html = preg_replace("/<!--OSUSUME-->/i",$osusume,$html);


	echo("$html");

	if ($db) { pg_close($db); }

$session_id = session_id();
//echo("<font color=\"#ffffff\">$session_id</font><br>\n");

	exit;

?>
