<?PHP
/*

	お問い合わせフォームプログラム


*/

//	設定項目

//	文字コード
define("ENCODING","utf-8");

//	返信メール送信者名
define("ADMIN_NAME","12星座占いランキング");

//	返信メールメールアドレス
define("ADMIN_EMAIL","info@uranairanking.jp");
//define("ADMIN_EMAIL","ookawara@azet.jp");
// C:\projects\uranai\uranai_lib\libadmin\contact.php←今ここ
// C:\projects\uranai\uranai_lib\templates\user\contact.tpl
// C:/projects/uranai/uranai_lib/templates/user/contact.tpl
//	テンプレートファイル名
define("TEMPLATE","../../user/contact.tpl");
//	送信後移行URL
define("THANK_URL","thank.html");

//	サイトURL
define("SITE_URL","https://uranairanking.jp/");

//	都道府県
$PREF_LIST = array("選択してください","北海道","青森県","岩手県","宮城県","秋田県","山形県","福島県","茨城県","栃木県","群馬県",
	"埼玉県","千葉県","東京都","神奈川県","新潟県","富山県","石川県","福井県","山梨県","長野県",
	"岐阜県","静岡県","愛知県","三重県","滋賀県","京都府","大阪府","兵庫県","奈良県","和歌山県",
	"鳥取県","島根県","岡山県","広島県","山口県","徳島県","香川県","愛媛県","高知県","福岡県",
	"佐賀県","長崎県","熊本県","大分県","宮崎県","鹿児島県","沖縄県");

//	注意事項1

//	実行プログラム

	session_start();

	// if ($_POST['mode']) {
	// 	define("MODE",$_POST['mode']);
	// }

	// //	処理
	// if (MODE == "check") {					//	入力データー確認処理
	// 	$ERROR = check_value();
	// } elseif (MODE == "send") {				//	メール送信処理
	// 	$ERROR = send_email();
	// } elseif (MODE == "リセット") {				//	メール送信処理
	// 	unset($_SESSION['VALUE']);
	// }

	// //	表示
	// if (!$ERROR && MODE == "check") {		//	確認ページ
	// 	$html = check_page();
	// } elseif (!$ERROR && MODE == "send") {	//	終了ページへ
	// 	$thank_url = THANK_URL;
	// 	header ("Location: $thank_url\n\n");
	// 	exit;
	// } else {								//	入力ページ
		$html = enter_page($ERROR);
	// }

	echo($html);

	exit;


//	入力ページ
function enter_page($ERROR) {
	global $PREF_LIST;

		if ($ERROR) {
			$INSERT_DATA[ERROR] = errors($ERROR);
		}

		if ($_SESSION['VALUE']) {
			foreach ($_SESSION['VALUE'] AS $key => $val) {
				if (!is_null($val)) {
					$$key = $val;
				}
			}
		}

		//	--	変更開始


	//	お名前 
	$INSERT_DATA['NAME'] = "<input type=\"text\" size=\"40\" name=\"name\" value=\"".$name."\">";

	//	フリガナ
	$INSERT_DATA['NAME2'] = "<input type=\"text\" size=\"40\" name=\"name2\" value=\"".$name2."\">";

	// //	郵便番号
	// $INSERT_DATA[ZIP] = "<input type=\"text\" size=\"12\" name=\"zip\" value=\"".$zip."\">";

	// //	都道府県
	// $PREF = "<select name=\"pref\">\n";
	// if (!$pref) { $pref = 0; }
	// foreach ($PREF_LIST AS $KEY => $VAL) {
	// 	if ($KEY == $pref) { $selected = "selected"; } else { $selected = ""; }
	// 	$PREF .= "<option value=\"{$KEY}\" $selected>{$VAL}</option>\n";
	// }
	// $PREF .= "</select>\n";
	// $INSERT_DATA[PREF] = $PREF;

	// //	市区町村
	// $INSERT_DATA[ADDRESS1] = "<input type=\"text\" size=\"50\" name=\"address1\" value=\"".$address1."\">";

	// //	番地
	// $INSERT_DATA[ADDRESS2] = "<input type=\"text\" size=\"50\" name=\"address2\" value=\"".$address2."\">";

	// //	建物名
	// $INSERT_DATA[ADDRESS3] = "<input type=\"text\" size=\"50\" name=\"address3\" value=\"".$address3."\">";

	//	ご連絡先電話番号
	$INSERT_DATA['TEL'] = "<input size=\"30\" type=\"text\" name=\"tel\" value=\"".$tel."\">";

	// //	FAX番号
	// $INSERT_DATA[FAX] = "<input size=\"30\" type=\"text\" name=\"fax\" value=\"".$fax."\">";

	//	E-mail1
	$INSERT_DATA['EMAIL1'] = "<input type=\"text\" size=\"50\" name=\"email\" value=\"".$email."\">";

	//	E-mail1コメント
	$INSERT_DATA['INDI'] = "<br />\n※受信可能なメールアドレスを必ず記入してください。";

	//	E-mail2
	$INSERT_DATA['EMAIL2'] = "<input type=\"text\" size=\"50\" name=\"check_email\" value=\"".$check_email."\">";

	//	お問い合わせ内容
	$INSERT_DATA['MESSAGE'] = "<textarea rows=\"10\" cols=\"50\" name=\"message\">".$message."</textarea>";


	//	送信ボタン
	$INSERT_DATA['SUBMIT'] = "<input type=\"submit\" value=\"確認\">　<input type=\"submit\" name=\"mode\" value=\"リセット\">";


	//	必須
	$INSERT_DATA['INDI'] = "<em>*</em>";

	//	表示メッセージ
	$ASK = "当サイトへのご相談・お問い合せは下記のフォームの所定項目をご入力のうえ送信してください。<br />\n";
	$ASK .= "<em>*</em>印は必須入力項目です。\n";
	$INSERT_DATA['ASK'] = $ASK;

	//	--	変更終了


	//	テンプレート読み込み
	$file = TEMPLATE;
	if (file_exists($file)) {
		echo('bbb');
		$html = file_get_contents($file);
	}

	//	置換
	foreach ($INSERT_DATA AS $key => $val) {
		$html = str_replace("<!--$key-->",$val,$html);
	}

	$form = "<form action=\"{$_SERVER['PHP_SELF']}\" method=\"POST\" id=\"mailform\">\n".
			"<input type=\"hidden\" name=\"mode\" value=\"check\">";
	$html = str_replace("<form>",$form,$html);

	return $html;

}


//	確認ページ
function check_page() {
global $PREF_LIST;

	if ($_SESSION['VALUE']) {
		foreach ($_SESSION['VALUE'] AS $key => $val) {
			if ($val != "") {
				$$key = $val;
			}
		}
	}

	//	--	変更開始

	//	お名前
	$INSERT_DATA[NAME] = $name;

	//	おフリガナ
	$INSERT_DATA[NAME2] = $name2;

	// //	郵便番号
	// if (!$zip) { $zip = "-----"; }
	// $INSERT_DATA[ZIP] = $zip;

	// //	都道府県
	// if (!$pref) {
	// 	$pref = "-----";
	// } else {
	// 	$pref = $PREF_LIST[$pref];
	// }
	// $INSERT_DATA[PREF] = $pref;

	// //	市区町村
	// if (!$address1) { $address1 = "-----"; }
	// $INSERT_DATA[ADDRESS1] = $address1;

	// //	番地
	// if (!$address2) { $address2 = "-----"; }
	// $INSERT_DATA[ADDRESS2] = $address2;

	// //	建物名
	// if (!$address3) { $address3 = "-----"; }
	// $INSERT_DATA[ADDRESS3] = $address3;

	//	ご連絡先電話番号
	if (!$tel) { $tel = "-----"; }
	$INSERT_DATA[TEL] = $tel;

	//	FAX番号
	if (!$fax) { $fax = "-----"; }
	$INSERT_DATA[FAX] = $fax;

	//	メールアドレス
	$INSERT_DATA[EMAIL1] = $email;

	//	メールアドレス
	$INSERT_DATA[EMAIL2] = $check_email;

	//お問い合わせ内容
	$INSERT_DATA[MESSAGE] = "<div style=\"display: block;width: 300px;\">".nl2br($message)."</div>";

	//	送信ボタン
	$INSERT_DATA[SUBMIT] = "<input type=\"submit\" value=\"送　信\">　<input type=\"submit\" name=\"mode\" value=\"戻　る\">";

	//	表示メッセージ
	$ASK = "以下の内容でよろしければ送信ボタンを押してください。<br />\n";
	$ASK .= "修正する場合は、戻るボタンを押してください。<br />\n";
	$INSERT_DATA[ASK] = $ASK;

	//	--	変更終了


	//	テンプレート読み込み
	$file = TEMPLATE;
	if (file_exists($file)) {
		$html = file_get_contents($file);
	}

	//	置換
	foreach ($INSERT_DATA AS $key => $val) {
		$html = str_replace("<!--$key-->",$val,$html);
	}

	$form = "<form action=\"{$_SERVER['PHP_SELF']}\" method=\"POST\" id=\"mailform\">\n".
			"<input type=\"hidden\" name=\"mode\" value=\"send\">";
	$html = str_replace("<form>",$form,$html);

	return $html;

}


//	入力データー確認処理
function check_value() {
global $PREF_LIST;

	if ($_POST) {
		unset($_SESSION['VALUE']);
		foreach ($_POST AS $key => $val) {
			if (!is_array($val)) {
				$val = mb_convert_kana($val,"asKV",ENCODING);
				$val = trim($val);
			}
			$$key = $val;
			$_SESSION['VALUE'][$key] = $val;
		}
	}

	//	--	変更開始
	//
	//	お名前
	if (!$name) { $ERROR[] = "お名前が入力されておりません。"; }
	//	お名前
	if (!$name2) { $ERROR[] = "フリガナが入力されておりません。"; }
	//	電話番号
//	if (!$tel) { $ERROR[] = "電話番号が入力されておりません。"; }
	//	都道府県
//	if (!$pref) { $ERROR[] = "都道府県が入力されておりません。"; }
	//	市区町村
//	if (!$address1) { $ERROR[] = "市区町村が入力されておりません。"; }
	//	番地
//	if (!$address1) { $ERROR[] = "番地が入力されておりません。"; }

	$email_flg = 0;
	//	メールアドレス
	if (!$email) {
		$ERROR[] = "メールアドレスが入力されておりません。";
		$email_flg = 1;
	} elseif (!preg_match("/^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)/",$email,$regs)) {
		$ERROR[] = "メールアドレスが不正です。";
		$email_flg = 1;
	}
	//	メールアドレス【確認用】
	if (!$check_email) {
		$ERROR[] = "メールアドレス【確認用】が入力されておりません。";
		$email_flg = 1;
	} elseif (!preg_match("/^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)/",$check_email,$regs)) {
		$ERROR[] = "メールアドレス【確認用】が不正です。";
		$email_flg = 1;
	}
	if ($email_flg != 1 && $email != $check_email) {
		$ERROR[] = "メールアドレスとメールアドレス【確認用】が一致しておりません。";
	}


	//	お問い合わせ内容
	if (!$message) { $ERROR[] = "お問い合わせ内容が入力されておりません。"; }


	//	--	変更終了

	return $ERROR;

}

//	メール送信処理
function send_email() {
global $PREF_LIST;

	if ($_SESSION['VALUE']) {
		foreach ($_SESSION['VALUE'] AS $key => $val) {
			if ($val != "") {
				$$key = $val;
			}
		}
	} else {
		$ERROR[] = "入力された情報が確認できません。";
	}

	//	受信用
	if (!$ERROR) {
		$ip = getenv("REMOTE_ADDR");
		$host = gethostbyaddr($ip);
		if (!$host){$host = $ip;}

		$subject = "[12星座占いランキング]{$name}様からお問い合わせ";

		$mail_msg = <<<EOT
$subject
----------------------------------------------------------------
お名前：{$name}
フリガナ：{$name2}
ご住所：
　{$zip}
　{$PREF_LIST[$pref]}{$address1}{$address2}
　{$address3}
ご連絡先電話番号：{$tel}
FAX番号：{$fax}
E-mail：{$email}
お問い合わせ内容：
{$message}
----------------------------------------------------------------
$host ($ip)
EOT;

		$newmail = new mail();
//		$newmail->set_language("");
		$newmail->set_encoding(ENCODING);
		$ERROR = $newmail->send($email,$name,ADMIN_EMAIL,$subject,$mail_msg,$cc_mail,$bcc_mail);
		//	send(送り主メールアドレス,送り主表示名,送信先メールアドレス,件名,本文,CCメールアドレス,BCCメールアドレス)
	}

	//	お客様送信用
	if (!$ERROR) {

		$subject = "{$name}様　お問い合わせありがとうございます[12星座占いランキング]";

		$mail_msg = <<<EOT
$subject

お問い合わせ内容は以下でよろしいでしょうか？
もし、お間違いなどございましたら、お手数ですがご連絡お願い致します。

----------------------------------------------------------------
お名前：{$name}
フリガナ：{$name2}
ご住所：
　{$zip}
　{$PREF_LIST[$pref]}{$address1}{$address2}
　{$address3}
ご連絡先電話番号：{$tel}
FAX番号：{$fax}
E-mail：{$email}
お問い合わせ内容：
{$message}
----------------------------------------------------------------

折り返しご連絡を差し上げますので、しばらくお待ち下さい。
今後とも当社を宜しくお願い致します。

■□―　12星座占いランキング 　―□■

　　　〒371-0801
　　　群馬県前橋市文京町3-8-2
　　　TEL 027-212-8080
　　　FAX 027-212-8085
　　　E-mail：info@nihonbs.co.jp

―――――――――――――――――――――――
EOT;

		$newmail = new mail();
//		$newmail->set_language("");
		$newmail->set_encoding(ENCODING);
		$ERROR = $newmail->send(ADMIN_EMAIL,ADMIN_NAME,$email,$subject,$mail_msg,$cc_mail,$bcc_mail);

	}

	if (!$ERROR) { unset($_SESSION['VALUE']); }

	return $ERROR;

}


//	エラー
function errors($ERROR) {
global $PREF_LIST;

	if ($ERROR) {
		foreach ($ERROR AS $val) {
			if (!$val) { continue; }
			$errors .= "・".$val."<br />\n";
		}
		if ($errors) {
			$errors = "<font color=\"#ff0000\"><b>エラー</b></font><br />\n$errors<br />\n";
		}
	}

	return $errors;
}


class mail {
	var $language = "ja";
	var $encoding = "Shift_JIS";

	function set_language($val) {
		$this->language = $val;
	}
	function set_encoding($val) {
		$this->encoding = $val;
	}

	function send($from_mail,$from_name,$to_mail,$subject,$msg,$cc_mail,$bcc_mail) {
		if ($this->encoding == "utf-8") { mb_language(uni); }
		else { mb_language($this->language); }
		mb_internal_encoding($this->encoding);

		if ($from_name) { $from_name = mail::base64_enc($from_name); }
		else { $from_name = $from_mail; }
		$additional_headers = "From: $from_name <$from_mail>\n";
		$additional_headers .= "Reply-To: $from_mail\n";
		if ($cc_mail) { $additional_headers .= "Cc: $cc_mail\n"; }
		if ($bcc_mail) { $additional_headers .= "Bcc: $bcc_mail\n"; }

		if (!mb_send_mail ($to_mail,$subject,$msg,$additional_headers , "-f$from_mail")) {
			$ERROR = "It failed in Mail Sending. (" . $to_mail .")";
		}
		return $ERROR;

	}
	function base64_enc($str) {
		$result = iconv($this->encoding, "ISO-2022-JP", $str).chr(27).'(B';
		$result = '=?ISO-2022-JP?B?'.base64_encode($result).'?=';
		return $result;
	}
}
?>
