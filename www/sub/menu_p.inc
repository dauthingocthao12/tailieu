<?PHP
function menu() {
global $PHP_SELF,$conn_id,$dir;

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

	if ($dir == 1) {
		include "../sub/menu.inc";
		include "../../cone.inc";
	}
	else {
		include "./sub/menu.inc";
		include "../cone.inc";
	}

	if ($customer || $opt) {
		$buy = <<<WAKABA
      <a href="/cago.php?mode=check" title="買物かご" class="l_cart">買物かご</a><br>
WAKABA;
	}

	if ($idpass) {
		list($email,$pass,$check,$af_num) = explode("<>",$idpass);
		$sql  = "SELECT kojin_num, name_s FROM kojin" .
				" WHERE email='$email' AND pass='$pass' AND saku!='1' AND kojin_num<'100000' ORDER BY kojin_num;";
		if ($result = pg_query($conn_id,$sql)) {
			$list = pg_fetch_array($result);
			$kojin_num = $list['kojin_num'];
			$name_s = $list['name_s'];
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
		$login = <<<WAKABA
      <a href="/login.php?url=$burl_" class="l_login" title="ログイン">ログイン</a><br>
WAKABA;

		$m_list = <<<WAKABA
 / <a href="/member/" class="l_member" title="会員登録">会員登録</a><br>
 / <a href="/member/pass.php" class="l_pass" title="パスワード忘れ">パスワード忘れ</a><br>
$buy
WAKABA;
		unset($idpass);
		unset($_SESSION['idpass']);
		setcookie("idpass");
	}
	else {
		$ai_time = date("H");
		if ($ai_time >= 6 && $ai_time < 10) { $aisatu = "　おはようございます。"; }
		elseif ($ai_time >= 10 && $ai_time < 18) { $aisatu = "　こんにちは。"; }
		else { $aisatu = "　こんばんは。"; }

		$login = <<<WAKABA
      <font class="size10">{$name_s}様<br>
      $aisatu</font><br>
      <a href="/logout.php?url=$burl_" class="l_logout" title="ログアウト">ログアウト</a><br>
WAKABA;

		$m_list = <<<WAKABA
 / <a href="/kakunin.htm" class="l_send" title="商品発送状況">商品発送状況</a><br>
 / <a href="/member/henkou.php" class="l_update" title="会員登録変更">会員登録変更</a><br>
 / <a href="/member/dakai.php" class="l_dakai" title="会員脱会">会員脱会</a><br>
 / <a href="/rireki.php" class="l_history" title="お買い物履歴">お買い物履歴</a><br>
 / <a href="/point.php" class="l_point" title="ポイント確認">ポイント確認</a><br>
$buy
WAKABA;
	}

/*
	//	一時的処理
	if ($af_num) {
		$m_list .= <<<WAKABA
      <a href="/affiliate.php" class="l_affiliate" title="アフィリエイト">アフィリエイト</a><br>

WAKABA;
	}
*/

	$menu = menu0($login,$m_list);

	return $menu;

}

?>
