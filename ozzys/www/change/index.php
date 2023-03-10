<?PHP
//---------------------------------------
//	Ozzy's商品管理プログラム
//
//	2006/2/3	Ver.1.00
//	有限会社　アゼット　大河原
//---------------------------------------

	//	管理ID、パスワード
	$ADMIN_ID = "ozzys";
	$ADMIN_PASS = "K11nyk.9";

	include ("../sub/setup.inc");
	include ("../../cone.inc");
	include ("pass_check.inc");

	session_start();
##	session_register("midpass");

	$PHP_SELF = $_SERVER['PHP_SELF'];

	//	管理者用カテゴリー
	$MAIN_L = array('選択して下さい','入荷情報','お勧め商品','お知らせ');

	$main = $_POST['main'];
	$b_main = $_POST['b_main'];
	$sub = $_POST['sub'];
	if ($main != $b_main) {
		$sub = 0;
		unset($_SESSION['selects']);
		unset($_SESSION['search']);
	}

	if (!$ERROR) {
		list($ERROR,$main) = pass_check($main,$ADMIN_ID,$ADMIN_PASS);
		if (!$_SESSION['midpass']) {
			$htm .= login($ERROR,$msg);
		}
		else {
			$htm .= main($main,$sub);
		}
		unset($ERROR);
	}

	$html  = headers($main,$sub);
	$html .= $htm;
	$html .= footer();

//	$html = mb_convert_encoding($html,"SJIS","UTF-8");
	echo ($html);
	ob_start("mb_output_handler");

	if ($db) { pg_close($db); }

	exit;



//-----------------------------------------------



//	ログイン
function login($ERROR,$msg) {
global $PHP_SELF;

	if ($ERROR) {
		$errors = ERROR($ERROR);
		if ($_POST['id']) { $id = $_POST['id']; }
		if ($_POST['pass']) { $pass = $_POST['pass']; }
	}

	$html = <<<WAKABA
<CENTER>
<TABLE border="0" cellpadding="2" cellspacing="1" width="350">
  <TBODY>
    <TR>
      <TD>
$msg
$errors
      </TD>
    </TR>
  </TBODY>
</TABLE>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="login">
<TABLE border="0" bgcolor="#666666" cellpadding="2" cellspacing="1" width="350">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TH colspan="2">Ozzy's商品管理入室画面</TH>
    </TR>
    <TR bgcolor="#FFFFFF">
      <TD colspan="2"></TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD colspan="2">IDとパスワードを入力し入室ボタンを押して下さい。</TD>
    </TR>
    <TR bgcolor="#FFFFFF">
      <TD colspan="2"></TD>
    </TR>
    <TR>
      <TD width="150" align="center" bgcolor="#cccccc">ID</TD>
      <TD width="250" bgcolor="#ffffff"><INPUT size="26" type="text" name="id" value="$id"></TD>
    </TR>
    <TR bgcolor="#FFFFFF">
      <TD colspan="2"></TD>
    </TR>
    <TR>
      <TD align="center" bgcolor="#cccccc">パスワード</TD>
      <TD bgcolor="#ffffff"><INPUT size="20" type="password" name="pass" value="$pass"></TD>
    </TR>
    <TR bgcolor="#FFFFFF">
      <TD colspan="2"></TD>
    </TR>
    <TR bgcolor="#FFFFFF">
      <TD colspan="2" align="center">
      <INPUT type="submit" value="入室">　<INPUT type="reset">
      </TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>
</CENTER>

WAKABA;

	return $html;

}



//	セレクト部分
function main($main,$sub) {
global $PHP_SELF,$MAIN_L,$SUB_L;

	if ($_SESSION['midpass']) {
		list($id,$pass) = explode("<>",$_SESSION['midpass']);
	}

	$html = <<<WAKABA
<TABLE border="0" width="100%">
  <TBODY>
    <TR>
      <FORM action="$PHP_SELF" method="POST">
      <INPUT type="hidden" name="b_main" value="$main">
      <TD nowrap>
      メインカテゴリー<BR>
      <SELECT name="main" OnChange="submit()">

WAKABA;

	$max = count($MAIN_L);
	for($i=0; $i<$max; $i++) {
		if ($main == $i) { $selected = "selected"; } else { $selected = ""; }
		$ml_name = $MAIN_L[$i];
		$html .= "        <OPTION value=\"$i\" $selected>$ml_name</OPTION>\n";
	}

	$html .= <<<WAKABA
      </SELECT>
      </TD>

WAKABA;

//	if ($main > 0 && $SUB_L[$main]) {
//		list($htm,$sub) = select($main,$sub);
//		$html .= $htm;
//	}

	$html .= <<<WAKABA
      <TD valign="bottom" nowrap>
      <INPUT type="submit" value="決定">
      </TD>
      <TD width="100%"></TD>
     </FORM>
    </TR>
  </TBODY>
</TABLE>
<HR>
</FORM>

WAKABA;

	if ($main > 0) { $html .= sagyou($main,$sub); }

	return $html;

}



//	サブカテゴリー選択
function select($main,$sub) {
global $SUB_L;

	$html .= <<<WAKABA
      <TD nowrap>
      作業項目<BR>
      <SELECT name="sub">

WAKABA;

	$max = count($SUB_L[$main]);
	if (!$sub && $max == 2) { $sub = 1; }
	for($i=0; $i<$max; $i++) {
		if ($sub == $i) { $selected = "selected"; } else { $selected = ""; }
		$k_name = $SUB_L[$main][$i];
		if (!$k_name) { continue; }
		$html .= "        <OPTION value=\"$i\" $selected>$k_name</OPTION>\n";
	}

	$html .= <<<WAKABA
      </SELECT>
      </TD>

WAKABA;

	return array($html,$sub);

}



//	作業部分
function sagyou($main,$sub) {
global $MAIN_L,$SUB_L;

	if ($main > 0) {
		if ($sub > 0) { $k_name = " (" . $SUB_L[$main][$sub] . ")"; }
		$html = "<B>" . $MAIN_L[$main] . "$k_name</B><BR>\n";
	}

	if ($main == 1) {
		include "./new_goods.inc";
		$html .= new_goods($main,$sub);
	}
	elseif ($main == 2) {
		include "./osusume_goods.inc";
		$html .= osusume_goods($main,$sub);
	}
	elseif ($main == 3) {
		include "./oshirase_goods.inc";
		$html .= oshirase_goods($main,$sub);
	}

//	if (!$sub) { $html .= "<BR>\n作業項目を選択してください。"; }

	return $html;

}



//	管理画面用ヘッダ
function headers($main,$sub) {
global $PHP_SELF;

	if ($_SESSION['midpass']) {
		$logout = "<INPUT type=\"submit\" value=\"ログアウト\">";
	}
	else { $logout = ""; }

	if ($main == "logout") { $name = ""; }

	$html = <<<WAKABA
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=UTF-8">
<META http-equiv="Content-Style-Type" content="text/css">
<TITLE>新着・お勧め商品管理</TITLE>
<STYLE type="text/css">
<!--
BODY,TABLE{
  font-size : 14px;
}
H1{
  font-size : 20px;
}
.list{
  font-size : 12px;
}
.copyright{
  font-size : 10px;
}
-->
</STYLE>
</HEAD>
<BODY>
<TABLE border="0" width="100%">
  <TBODY>
    <TR>
      <TD><FONT SIZE="+2"><B>新着・お勧め商品管理</B></FONT></TD>
      <TD align="center">
      <FORM action="$PHP_SELF" method="POST">
      <INPUT type="hidden" name="main" value="logout">
      $logout
      </TD></FORM>
    </TR>
    <TR bgcolor="#FFFFFF">
      <TD colspan="2"><HR></TD>
    </TR>
  </TBODY>
</TABLE>

WAKABA;

	return $html;

}



//	管理画面用フッタ
function footer() {
global $copyright;

	$html = <<<WAKABA
</BODY>
</HTML>

WAKABA;

	return $html;

}



//	エラー処理
function ERROR($ERROR) {

	foreach ($ERROR AS $val) {
		if ($val != "") { $error .= "・$val<BR>\n"; }
	}

	$errors = <<<WAKABA
<BR>
<FONT color="#FF0000"><B>エラー</B></FONT><BR>
$error
WAKABA;

	return $errors;

}



//	ロック
function lockfile($lockdir) {

	//	デッドロックチェック
	if (file_exists($lockdir)) {
		$now = time();
		$list = stat($lockdir);
		$mtime = $list[mtime];
		$sa = $now - $mtime;
		if ($sa > 30) {
			rmdir($lockdir);
		}
	}

	//	ロック設定
	$lock_check = 0;
	for($i=0; $i<5; $i++) {
		if (!file_exists($lockdir)) {
			mkdir($lockdir,0777);
			chmod($lockdir,0777);
			$lock_check = 1;
			break;
		}
		else {
			sleep(1);
		}
	}

	return $lock_check;

}



//	アンロック
function unlockfile($lockdir) {

	if (file_exists($lockdir)) {
		rmdir($lockdir);
	}

}
?>
