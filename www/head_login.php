<?PHP
//	ヘッダ
	//	サブルーチンフォルダー
	$SUB_DIR = "./sub";

	include ("../cone.inc");
	include ("$SUB_DIR/menu.inc");
	include("$SUB_DIR/setup.inc");
	include("$SUB_DIR/array.inc");
	session_cache_limiter('nocache');
	session_start();
	$_SESSION["idpass"];
	$_SESSION["customer"];
	$_SESSION["opt"];
	$_SESSION["blurl"];

	$burl = $_SERVER['REQUEST_URI'];
	if ($_SESSION['blurl']) {
		$burl = $_SESSION['blurl'];
	}

	if ($_SESSION['idpass']) {
		$idpass = $_SESSION['idpass'];
	}
	elseif ($_COOKIE['idpass']) {
		$idpass = $_COOKIE['idpass'];
	}
	$customer = $_SESSION['customer'];
	$opt = $_SESSION['opt'];
	if ($idpass) {
		list($email,$pass,$check,$af_num) = explode("<>",$idpass);
		$sql  = "SELECT kojin_num, name_s FROM kojin" .
				" WHERE email='$email' AND pass='$pass' AND saku!='1' AND kojin_num<'100000' ORDER BY kojin_num;";
		if ($result = pg_query($conn_id,$sql)) {
			$list = pg_fetch_array($result);
			$kojin_num = $list['kojin_num'];
			$name_s = $list['name_s'];
#			$name_s = mb_convert_encoding($name_s,"utf-8","euc-jp");
		}
		if (!$kojin_num) {
			unset($idpass);
			unset($_SESSION['idpass']);
			setcookie("idpass");
			unset($af_num);
		}
		else {
			$sql = "SELECT af_num FROM afuser WHERE kojin_num='$kojin_num' AND state!='1';";
			if ($result = pg_query($conn_id,$sql)) {
				$list = pg_fetch_array($result);
				$af_num = $list['af_num'];
			}
			if ($af_num < 1) { unset($af_num); }
		}

		if ($kojin_num) {
			$idpass = "$email<>$pass<>$check<>$af_num<>";
			$_SESSION['idpass'] = $idpass;
			if ($check == 1) {
				setcookie("idpass",$idpass,time() + 60*60*24*30,"/",".futboljersey.com");
			}
		}
	}
	$burl_ = urlencode($burl);
	if (!$idpass) {
		$login 	= "<li>ようこそゲストさん</li>\n";

		$m_list  = "<a href=\"/login.php?url=$burl_\" class=\"l_login\" title=\"ログイン\">ログイン</a>\n";
		$m_list .= " / <a href=\"/member/\" class=\"l_member\" title=\"会員登録\">会員登録</a>\n";
		$m_list .= " / <a href=\"/member/pass.php\" class=\"l_pass\" title=\"パスワード忘れ\">パスワード忘れ</a>\n";
		$m_list .= "$buy\n";
		unset($idpass);
		unset($_SESSION['idpass']);
		setcookie("idpass");
	} else {

		$sql =  "select point from kojin" .
				" where email='$email' AND pass='$pass' AND kojin_num<='100001' AND saku='0'" .
				" ORDER BY kojin_num;";
		$sql1 = pg_exec($conn_id,$sql);
		$count = pg_numrows($sql1);

		if ($count >= 1) {
			list($point) = pg_fetch_array($sql1,0);
		}

		$point = number_format($point);

		$ai_time = date("H");
		if ($ai_time >= 6 && $ai_time < 10) { $aisatu = "　おはようございます。"; }
		elseif ($ai_time >= 10 && $ai_time < 18) { $aisatu = "　こんにちは。"; }
		else { $aisatu = "　こんばんは。"; }

#		$name_s = mb_convert_encoding($name_s,"euc-jp","auto");

//		$login .= "<li><font class=\"size10\">{$name_s}様 $aisatu</font></li>\n";	//	del ookawara 2009/10/13
		$login .= "<li>{$name_s}様 $aisatu</li>\n";	//	add ookawara 2009/10/13
		$login .= "<li>保有割引ポイントは、$point pt ( $point 円 )です。</li>\n";

		$m_list  = "<a href=\"/logout.php?url=$burl_ \" class=\"l_logout\" title=\"ログアウト\">ログアウト</a>\n";
		$m_list .= " / <a href=\"/kakunin.htm\" class=\"l_send\" title=\"商品発送状況\">商品発送状況</a>\n";
		$m_list .= " / <a href=\"/member/henkou.php\" class=\"l_update\" title=\"会員登録変更\">会員登録変更</a>\n";
		$m_list .= " / <a href=\"/member/dakai.php\" class=\"l_dakai\" title=\"会員脱会\">会員脱会</a>\n";
		$m_list .= " / <a href=\"/rireki.php\" class=\"l_history\" title=\"お買い物履歴\">お買い物履歴</a>\n";
//		$m_list .= "<a href=\"/point.php\" class=\"l_point\" title=\"ポイント確認\">ポイント確認</a>\n";
		$m_list .= "$buy\n";
	}

	$html = "<div id=\"login\"><div id=\"loginname\"><ul><!--LOGINNAME--></ul></div><div id=\"loginmenu\"><!--LOGINMENU--></div></div>\n";
##	$html = eregi_replace("<!--LOGINNAME-->",$login,$html);
	$html = preg_replace("/<!--LOGINNAME-->/i",$login,$html);
##	$html = eregi_replace("<!--LOGINMENU-->",$m_list,$html);
	$html = preg_replace("/<!--LOGINMENU-->i/",$m_list,$html);

	echo($html);

	exit;

?>
