<?PHP
//	商品表示メインプログラム

	include("../cone.inc");
	include("./sub/setup.inc");
	include("./sub/array.inc");
	include("./sub/seo.class.php");

//	session_cache_limiter('public');
	session_start();
//	session_register("idpass","customer");

	$PHP_SELF = $_SERVER['PHP_SELF'];
	//	ログアウトチェック
##	if (eregi("out$",$PHP_SELF)) {
	if (preg_match("/out$/i",$PHP_SELF)) {
##		$PHP_SELF = eregi_replace("/out$","",$PHP_SELF);
		$PHP_SELF = preg_match("/\/out$/i","",$PHP_SELF);
##		$PHP_SELF = eregi_replace("out$","",$PHP_SELF);
		$PHP_SELF = preg_match("/out$/i","",$PHP_SELF);
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
		// seo対応 2017-01-17 
		OzzysSEO::addKeywordBreadcrumb("商品商法");

		include("./sub/syousai.inc");
		$syousai = syousai($VALUE,$CHECK);
		$main .= $syousai;
//echo("1<BR>\n");
	}
	elseif ($CHECK[n]) {	//	商品一覧
		include("./sub/goodsname.inc");
		// seo対応 2017-01-17 
		//OzzysSEO::addKeyword("商品一覧");

		$goodsname = goodsname($VALUE,$CHECK);
		$main .= $goodsname;
//echo("2<BR>\n");
	}
	elseif ($CHECK[m]) {
		if ($CHECK[s]) {	//	商品名一覧
			// seo対応 2017-01-17 
			//OzzysSEO::addKeyword("商品一覧");
			OzzysSEO::addKeywordBreadcrumb("商品一覧");

			include("./sub/goodslist.inc");
			$goodslist = goodslist($VALUE,$CHECK);
			include("./sub/goodsname.inc");
			$goodsname = goodsname($VALUE,$CHECK);
			$main .= $goodslist . $goodsname;
//echo("3<BR>\n");
		}
		elseif ($CHECK[main]) {	//	サブカテゴリー＆商品名一覧
			// seo対応 2017-01-18 
			//OzzysSEO::addKeyword("カテゴリー 一覧,商品名一覧");
			OzzysSEO::addKeywordBreadcrumb("カテゴリーと商品名一覧");

			include("./sub/sublist.inc");
			$sublist = sublist($VALUE,$CHECK);
			include("./sub/goodslist.inc");
			$goodslist = goodslist($VALUE,$CHECK);
			$main .= $sublist . $goodslist;
//echo("4<BR>\n");
		}
		else {	//	メインカテゴリー＆商品名一覧
			// seo対応 2017-01-17 
			//OzzysSEO::addKeyword("カテゴリー 一覧,商品名一覧");
			OzzysSEO::addKeywordBreadcrumb("カテゴリーと商品名一覧");

			include("./sub/mainlist.inc");
			$mainlist = mainlist($VALUE,$CHECK);
			include("./sub/goodslist.inc");
			$goodslist = goodslist($VALUE,$CHECK);
			$main .= $mainlist . $goodslist;
//echo("5<BR>\n");
		}
	}
	elseif ($CHECK[s]) {	//	メーカー名一覧＆商品名一覧
		// seo対応 2017-01-17 
		//OzzysSEO::addKeyword("メーカー名一覧,商品名一覧");
		OzzysSEO::addKeywordBreadcrumb("メーカー名と商品名一覧");

		include("./sub/makerlist.inc");
		$makerlist = makerlist($VALUE,$CHECK);
		include("./sub/goodslist.inc");
		$goodslist = goodslist($VALUE,$CHECK);
		$main .= $makerlist . $goodslist;
//echo("6<BR>\n");
	}
	elseif ($CHECK[main]) {	//	サブカテゴリー＆メーカー名一覧
		// seo対応 2017-01-17 
		//OzzysSEO::addKeyword("カテゴリー一覧,メーカー名一覧"); // seo keywords 追加
		OzzysSEO::addDescription("カテゴリーとメーカー名の一覧です。"); // seo description 追加

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
			// seo対応 2017-01-17 
			OzzysSEO::addKeyword("カテゴリー一覧"); // seo keywords 追加
			OzzysSEO::addDescription("カテゴリー一覧です。"); // seo description 追加

			include("./sub/mainlist.inc");
			$mainlist = mainlist($VALUE,$CHECK);
			$main .= $mainlist;
			$title = "カテゴリー 一覧";
//echo("8<BR>\n");
		}
##		elseif (!$CHECK && eregi("^$m_maker_file",$value)) {	//	メーカー一覧
		elseif (!$CHECK && preg_match("/^$m_maker_file/i",$value)) {	//	メーカー一覧
			// seo対応 2017-01-17 
			OzzysSEO::addKeyword("メーカー一覧"); // seo keywords 追加
			OzzysSEO::addDescription("メーカー一覧です。"); // seo description 追加

			include("./sub/makerlist.inc");
			$makerlist = makerlist($VALUE,$CHECK);
			$main .= $makerlist;
			$title = "メーカー 一覧";
//echo("9<BR>\n");
		}
		else {	//	メインカテゴリー＆メーカー一覧
			// seo対応 2017-01-18 
			OzzysSEO::addKeyword("カテゴリー一覧,メーカー名一覧"); // seo keywords 追加
			OzzysSEO::addDescription("カテゴリーとメーカー名の一覧です。"); // seo description 追加

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

	//	SEO (add 2017-01-17)>>>
	$html = str_replace("<!--SEO-KEYWORDS-->", OzzysSEO::makeMetaKeywords(), $html);
	$html = str_replace("<!--SEO-DESCRIPTION-->", OzzysSEO::makeMetaDescription(), $html);
	//	<<<

	//	ページタイトル
	$html = str_replace("<!--TITLE-->",$title,$html);

	//	ヘッド背景画
	include("./sub/headimg.inc");
	$headimg = headimg($HEADIMG);
	$html = str_replace("<!--HEADIMG-->",$headimg,$html);

	//	ヘッドメッセージ
	include("./sub/headmsg.inc");
	$headmsg = headmsg($LOGDATA_DIR,$headmsg_file);
	$html = str_replace("<!--HEADMSG-->",$headmsg,$html);

	//	ログインメッセージ
	include("./sub/loginmsg_resp.inc");
	$burl = "";
	$loginmsg = loginmsg($burl);
	$html = str_replace("<!--LOGIN-->",$loginmsg,$html);

	//	メニュー
	include("./sub/menu.inc");
	$menu = menu($LOGDATA_DIR, $menu_file_resp);
	$html = str_replace("<!--MENU-->",$menu,$html);	//	メニュー

	//	メイン内容
	$html = str_replace("<!--MAIN-->",$main,$html);

	//	お勧め
	include("./sub/osusume.inc");
	$LIST = "";
	$osusume = osusume($LIST, true);
	$html = str_replace("<!--OSUSUME-->",$osusume,$html);


	echo("$html");

	if ($db) { pg_close($db); }

$session_id = session_id();
//echo("<font color=\"#ffffff\">$session_id</font><br>\n");

	exit;

?>
