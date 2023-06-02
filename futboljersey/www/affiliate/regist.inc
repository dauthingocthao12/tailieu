<?PHP
//	アフィリエイト会員登録

//	アフィリエイト初期画面
function re_defaults($mode,$action,$method) {
global $PHP_SELF;

	if ($_SESSION['ERROR']) {
		$errors = ERROR($_SESSION['ERROR']);

		$errors = <<<WAKABA
      <br>
      <table border="0" width="600" cellspacing="1" cellpadding="10">
        <tbody>
          <tr>
            <td>$errors</td>
          </tr>
        </tbody>
      </table>

WAKABA;
		unset($_SESSION['ERROR']);
	}

	//	規約読み込み
	$k_file = "./affiliate/kiyaku.txt";
	if (file_exists($k_file) && filesize($k_file) > 0) {
		$LIST = file($k_file);
		if ($LIST) {
			foreach ($LIST AS $VAL) {
				$kiyaku .= $VAL;
			}
		}
	}

	$html = <<<WAKABA
      <form action="$PHP_SELF" method="POST">
      <input type="hidden" name="mode" value="regist">
      <input type="hidden" name="action" value="regist">
$errors
      <br>
      アフィリエイトに参加する場合以下の規約をお読みください。<BR>
      <table border="0" width="600" cellspacing="1" cellpadding="10" bgcolor="#666666">
        <tbody>
          <tr bgcolor="#ffffff">
            <td>
$kiyaku
            </td>
          </tr>
        </tbody>
      </table>
      <br>
      上記の規約に同意出来るならば同意するにチェックをし、登録ボタンを押してください。<br>
      <br>
      <input type="checkbox" name="doui" value="1">：規約に同意する。<br>
      <input type="submit" value="登録">
      </form>

WAKABA;

	//	登録削除チェック
	$check = regist_check();
	if ($check) { unset($html); }

	return $html;

}



//	登録完了ページ
function af_re_end($mode,$method) {

	$html = <<<WAKABA
      <br>
      <table border="0" width="600" cellspacing="1" cellpadding="10" bgcolor="#666666">
        <tbody>
          <tr bgcolor="#ffffff">
            <td>
登録完了致しました。<br>
登録確認のメールを送信致しましたのでご確認下さい。<br>
確認のメールが届かない場合はお手数ですがお問合せからご連絡お願い致します。<br>
<br>
<p align="center"><a href="/affiliate.php">アフィリエイトTOP</a></p>
            </td>
          </tr>
        </tbody>
      </table>

WAKABA;

	return $html;

}



//	登録処理
function af_regist($mode,$action,$method) {
global $PHP_SELF,$conn_id,$afuser_table,$member_table,$m_footer,
		$admin_mail_a,$admin_name;

	$doui = $_POST['doui'];
	if (!$doui) { $ERROR[] = "規約に同意されておりません。"; }

	//	ユーザー確認
	if (!$ERROR) {
		list($email,$pass,$check,$af_num) = explode("<>",$_SESSION['idpass']);
		$sql  = "SELECT kojin_num, name_s, name_n FROM $member_table" .
				" WHERE email='$email' AND saku!='1' AND kojin_num<'100000' LIMIT 1;";
		if ($result = mysqli_query($conn_id,$sql)) {
			$list = mysqli_fetch_array($result);
			$kojin_num = $list['kojin_num'];
			$name_s = $list['name_s'];
			$name_n = $list['name_n'];
			$name = "$name_s $name_n";
		}
		if ($kojin_num < 1) { $ERROR[] = "会員情報が確認出来ません。"; }
	}

	//	登録処理
	if (!$ERROR) {
		$sql  = "INSERT INTO $afuser_table" .
				" (kojin_num,regist)" .
				" VALUES('$kojin_num',now());";
		if (!$result = mysqli_query($conn_id,$sql)) { $ERROR[] = "アフィリエイト登録出来ませんでした。"; }
		else {
			$sql  = "SELECT af_num FROM $afuser_table WHERE kojin_num='$kojin_num' AND state!='1' LIMIT 1;";
			if ($result = mysqli_query($conn_id,$sql)) {
				$list = mysqli_fetch_array($result);
				$af_num = $list['af_num'];
			}
		}
	}

	//	セッション、クッキー埋め込み
	if (!$ERROR && $kojin_num) {
		$idpass = "$email<>$pass<>$check<>$af_num<>";
		$_SESSION['idpass'] = $idpass;
		if ($check == 1) {
			setcookie("idpass",$idpass,time() + 60*60*24*30);
		}
	}

	//	登録メール送信
	if (!$ERROR) {
		//	受け取り
		//	件名
		$subject = "アフィリエイト会員登録 (ID：{$af_num})";

		$ip = getenv("REMOTE_ADDR");
		$host = gethostbyaddr($ip);
		if (!$host) { $host = $ip; }

		$msr = <<<WAKABA
{$name}様がフィリエイト会員登録して頂きました。

名前：{$name}
会員番号：{$kojin_num}
ID：{$af_num}
E-mail：{$email}
------------------------------------------------------
$host ($ip)

WAKABA;

		//	送信処理
		$send_email = $email;
		$send_name = "$name";
		$get_email = $admin_mail_a;
		send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msr);

		//	会員
		//	件名
		$subject = "ネイバーズスポーツアフィリエイト会員登録完了";

		//	メッセージ
		$msr = <<<WAKABA
{$name}様、この度はネイバーズスポーツアフィリエイト会員登録して頂き有り難うございます。
ネイバーズスポーツアフィリエイト会員登録処理が完了致しました。

{$name}様のアフィリエイトの会員IDは、「{$af_num}」となります。
リンク作成時間違いない様にお願い致します。

それではこれからもよろしくお願い致します。

$m_footer

WAKABA;

		//	送信処理
		$send_email = $admin_mail_a;
		$send_name = $admin_name;
		$get_email = $email;
		send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msr);

	}

	if ($ERROR) {
		$_SESSION['ERROR'] = $ERROR;
		$actions = "";
	}
	else {
		$actions = "&action=end";
	}

	header ("Location: $PHP_SELF?mode=regist$actions\n\n");


	exit;

}



//	登録済みか確認
function regist_check() {
global $PHP_SELF,$conn_id,$member_table,$afuser_table;

	if ($_SESSION['idpass']) {
		$idpass = $_SESSION['idpass'];
	}
	elseif ($_COOKIE['idpass']) {
		$idpass = $_COOKIE['idpass'];
	}
	list($email,$pass,$check,$af_num)  = explode("<>",$idpass);

	if (!$af_num) {
		$sql  = "SELECT af.af_num FROM $member_table a, $afuser_table af" .
				" WHERE a.kojin_num=af.kojin_num AND a.email='$email';";
		if ($result = mysqli_query($conn_id,$sql)) {
			$af_num = mysqli_num_rows($result);
		}
	}

	return $af_num;

}



//	登録不可メッセージ
function not_regist($mode,$action,$method) {

	$html = <<<WAKABA
<br>
すでに登録されているか、登録出来ません。<br>
<br>

WAKABA;

	return $html;

}
?>
