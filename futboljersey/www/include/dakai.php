<?PHP
/*

	■ネイバーズスポーツ会員脱会プログラム

*/
//------------------//
//	TOPページ表示	//
//------------------//
function dakai_html($ERROR){

	//	エラーがあったらメッセージ表示
	if ($ERROR) {
		$error_html = error_html($ERROR);
	}

	//	入力されたメールアドレスを入力欄に表示
	if($_SESSION['member']['email'] && $ERROR){
		$email = $_SESSION['member']['email'];
	}

	$INPUTS['ERRORMSG'] = $error_html;		//	エラーメッセージ
	$INPUTS['EMAIL'] = $email;				//	入力されたメールアドレス

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("dakai.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;

}

//------------------//
//	入力内容確認	//
//------------------//
function dakai_check(&$ERROR){
	global $conn_id; // mysql DB

	if ($_SESSION['idpass']) {
		$idpass = $_SESSION['idpass'];
	} elseif ($_COOKIE['idpass']) {
		$idpass = $_COOKIE['idpass'];
	}
	list($email_,$pass_,$check_,$af_num_) = explode("<>",$idpass);

	// パラメーターを取得
	$email = trim($_POST['email']);
	$pass = trim($_POST['pass']);
#	$email = mb_convert_kana($email,n,"EUC-JP");
	$email = mb_convert_kana($email,n,"UTF-8");
	$email = strtolower($email);
#	$pass = mb_convert_kana($pass,n,"EUC-JP");
	$pass = mb_convert_kana($pass,n,"UTF-8");
	$pass = strtolower($pass);

	// セッションにパラメーターを保持
	$_SESSION['member']['email'] = $email;

	//	エラーチェック
	if (!$email) {
		$ERROR[] = "メールアドレスが入力されておりません。";
	}
	if (!$pass) {
		$ERROR[] = "パスワードが入力されておりません。";
	}
	if($email && $pass){
		if ($email != $email_) {
			$ERROR[] = "ログイン中のE-mailアドレスと一致していません。";
		}
	}
	//	エラーがあったら中断してメッセージを表示
	if ($ERROR) {
		return;
	}

	//	会員登録データがあるか確認
	if (!$ERROR) {
		$sql  = "SELECT kojin_num, name_s, name_n, email, pass FROM kojin" .
				" WHERE email='".$email."' AND pass='".$pass."' AND saku='0' AND kojin_num<='100000';";
		$sql1 = mysqli_query($conn_id,$sql);
		$count = mysqli_num_rows($sql1);
		if ($count < 1) {
			$ERROR[] = "メールアドレス又はパスワードが間違っています。";
			mysqli_close($conn_id);
		}
	}

	//	エラーがあったら中断してメッセージを表示
	if ($ERROR) {
		return;
	}

	list($kojin_num,$name_s,$name_n,$email,$pass) = mysqli_fetch_array($sql1);

	return array($kojin_num,$name_s,$name_n,$email,$pass);

}

//----------------------//
//	脱会処理&メール送信	//
//----------------------//
function dakai_start($kojin_num,$name_s,$name_n,$email,$pass){

	global $afuser_table,	//	afuserテーブル名
		   $admin_name,		//	"NEIGHBOURS SPORTS"
		   $admin_mail_m,	//	店舗アドレス
		   $m_footer,		//	メールフッター
		   $conn_id; // mysql DB

	if ($_SESSION['idpass']) {
		$idpass = $_SESSION['idpass'];
	} elseif ($_COOKIE['idpass']) {
		$idpass = $_COOKIE['idpass'];
	}
	list($email_,$pass_,$check_,$af_num_) = explode("<>",$idpass);

	//	会員脱会SQL
	$sql =  "UPDATE kojin set " .
			"saku='1' " .
			"WHERE email='".$email."' " .
			"AND pass='".$pass."';";
	$sql1 = mysqli_query($conn_id,$sql);
	//	アフィリエイト会員退会SQL
	if ($af_num_ > 0) {
		$sql =  "UPDATE ".$afuser_table." SET " .
				"state='1' " .
				"WHERE af_num='".$af_num_."';";
		$sql1 = mysqli_query($conn_id,$sql);
	}

	// お客様送信メール
	$subject = "会員脱会のご連絡 - ネイバーズスポーツ -";

	$msr = <<<WAKABA
{$subject}
{$name_s} {$name_n} 様 
------------------------------------------------------

お世話になります。
会員脱会の手続きのご確認のメールを出させていただきました。
登録時のメールアドレス
　{$email}

ご利用有り難う御座いました。
またのご利用お待ちしております。

もし心当たりのない場合はご連絡下さい。

{$m_footer}
WAKABA;
//echo('$msr=>'.$msr."<br />\n");

	$send_email = $admin_mail_m;
	$send_name = $admin_name;
	$get_email = $email;
	//$get_email = "検証アドレス";
	send_email($send_email,$send_name,$mail_get,$get_email,$get_email,$subject,$msr);

	// 店舗受け取りメール
	$subject = "会員脱会されました。(No.".$kojin_num.") - ネイバーズスポーツ -";

	$ip = getenv("REMOTE_ADDR");
	$host = gethostbyaddr($ip);
	if (!$host) { $host = $ip; }

	$msr = <<<WAKABA
{$name_s} {$name_n} 様が {$subject}
------------------------------------------------------

会員番号
　No.{$kojin_num}

お名前
　{$name_s} {$name_n}

メールアドレス
　{$email}

パスワード
　{$pass}

------------------------------------------------------
{$host} ({$ip})
WAKABA;
//echo('$msr=>'.$msr."<br />\n");

	$send_email = $email;
	$send_name = $name_s." ".$name_n;
	$get_email = $admin_mail_m;
	//$get_email = "検証アドレス";
	send_email($send_email,$send_name,$mail_get,$get_email,$get_email,$subject,$msr);

	mysqli_close($conn_id);

	unset($_SESSION['member']);
	unset($idpass);
	unset($_SESSION['idpass']);

	header ("Location: ../template/thank_d.htm");
	exit();

}

?>