<?PHP
/*
 * 注意 (2017-01-17)
 *
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
 * 同時に修正されるファイル：
 *  - sub/loginmsg.php
 *  - sub/loginmsg_resp.php
 * ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
 */

//	ログインチェック
function loginmsg($burl) {
global $PHP_SELF,$db,$point_b,$point_c;

	if (!$burl) { $burl = $PHP_SELF; }

	if ($_SESSION['idpass']) {
		$idpass = $_SESSION['idpass'];
	}
	elseif ($_COOKIE['idpass']) {
		$idpass = $_COOKIE['idpass'];
	}

	if (!$idpass) {
		if ($_SESSION['cart_time']) {
			$cart_time = $_SESSION['cart_time'];
			$_SESSION['cart_time'] = $cart_time;
		} else {
			$_SESSION['cart_time'] = time();
		}
	}

	$check = 0;
	if ($idpass) {
		list($email,$pass,$memo) = explode("<>",$idpass);
		if ($memo != "out") {
			$sql =	"SELECT name_s, name_n, point FROM kojin" .
					" WHERE saku!='1' AND email='$email' AND pass='$pass' AND kojin_num BETWEEN 100001 AND 600000 LIMIT 1;";
			if ($result = pg_query($db,$sql)) {
				$list = pg_fetch_array($result);
				$name_s = $list['name_s'];
				$name_n = $list['name_n'];
				$point = $list['point'];
				if ($name_s) { $check = 1; }
			}
		}
	}

	$ai_time = date("H");
	if ($ai_time >= 6 && $ai_time < 10) { $aisatu = "おはようございます。"; }
	elseif ($ai_time >= 10 && $ai_time < 18) { $aisatu = "こんにちは。"; }
	else { $aisatu = "こんばんは。"; }

##	if ($_SESSION['customer'] && !eregi("cart.php",$PHP_SELF)) {
	if ($_SESSION['customer'] && !preg_match("/cart.php/i",$PHP_SELF)) {
		$burl_ = urlencode($burl);
		$cago = "<li><a href=\"/cart.php\" title=\"買い物かご\" class='btn btn-primary'>買い物かご</a></li>\n";
	}
	else { $cago = ""; }

	if ($check == 1) {
		// LOGINされている ===
		$name = "<B>" . $name_s . " " . $name_n . "</B>さん";
		$wari = $point / $point_c;
		$wari = floor($wari);
		$en = $wari * $point_b;

		$en = number_format($en);
		//$en = "ご利用可能金額 " . $en . "円";
		$en = "$en 円";

		$point = number_format($point);
		//$point = "現在の獲得ポイント(" . $point . "pt)";
		$point = "$point pt";

		$burl_ = urlencode($burl);
		$logout_msg = "<li><a href=\"/logout.php?url=$burl_\" title=\"ログアウト\" class='btn btn-primary'>ログアウト</a></li>\n";
		$hen_msg    = "<li><a href=\"/member.php\" title=\"会員登録変更\" class='btn btn-primary'>会員登録変更</a></li>\n";
##		if (eregi("member.php",$burl)) {
		if (preg_match("/member.php/i",$burl)) {
			$logout_msg = "";
			$hen_msg = "";
		}
##		elseif (eregi("cart.php?",$burl)) {
		elseif (preg_match("/cart.php?/i",$burl)) {
			$logout_msg = "";
			$hen_msg = "";
		}

		$html = <<<EOT
<div id="login-name">
	$aisatu$name
</div>
<ul id="login-menu">
	$cago
	$logout_msg
	$hen_msg
	<li><a href="/phistory.php" title="ご注文履歴" class='btn btn-primary'>ご注文履歴</a></li>
</ul>
EOT;

//		if ($memo == 1) {
//			setcookie("idpass",$idpass,time() + 60*60*24*30);
//		}
	}
	else {
		// LOGINされていない ===
		$burl_ = urlencode($burl);
		$name = "<B>ゲスト</B>さん";
		$login_msg = "<li><a href=\"/login.php?url=$burl_\" title=\"ログイン\" class='btn btn-primary'>ログイン</a></li>\n";
		$add_msg   = "<li><a href=\"/member.php\" title=\"会員登録\" class='btn btn-primary'>会員登録</a></li>\n";
##		if (eregi("login.php",$burl)) {
		if (preg_match("/login.php/i",$burl)) {
			$login_msg = "";
		}
##		elseif (eregi("member.php",$burl)) {
		elseif (preg_match("/member.php/i",$burl)) {
			$login_msg = "";
			$add_msg = "";
		}
##		elseif (eregi("cart.php?",$burl)) {
		elseif (preg_match("/cart.php?/i",$burl)) {
			$login_msg = "";
			$add_msg = "";
		}

		$html = <<<EOT
<div id="login-name">
$aisatu$name
</div>
<ul id="login-menu">
	$cago
	$login_msg
	$add_msg
	<li><a href="/conf.php" title="パスワード確認" class='btn btn-primary'>パスワード確認</a></li>
</ul>
EOT;

		unset($idpass);
		unset($_SESSION['idpass']);
		unset($_COOKIE['idpass']);
		setcookie("idpass");
	}

	return $html;

}
?>
