<?PHP
//	メール配信サブルーチン
function send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msg) {

	//	文字コード宣言
	mb_language("Japanese");
#	mb_internal_encoding("EUC-JP");
	mb_internal_encoding("UTF-8");
	$send_name = header_base64_encode($send_name);
	$from = "From: ".$send_name." <".$send_email.">\nReply-To: ".$send_email."\n";
	if ($mail_bcc == 1) {
		$from .= "Bcc: ".$bcc_email."\n";
	}

	//	文字化け対策
	$subject = "　" . $subject;

	mb_send_mail ( $get_email, $subject, $msg , $from , "-f$send_email");

}
function header_base64_encode($str) {

#	$result = iconv("EUC-JP", "ISO-2022-JP", $str).chr(27).'(B';	//iconv 文字列を指定した文字エンコーディングに変換する
	$result = iconv("UTF-8", "ISO-2022-JP", $str).chr(27).'(B';	//iconv 文字列を指定した文字エンコーディングに変換する
	$result = '=?ISO-2022-JP?B?'.base64_encode($result).'?=';

	return $result;

}



//	メール＆パスワード入力
function member_henkou($ERROR) {

	$html = "";

	$email = $_SESSION['member']['email'];

	if ($ERROR) {
		$error_html = error_html($ERROR);
	}

	$INPUTS['ERRORMSG'] = $error_html;		//	エラーメッセージ
	$INPUTS['EMAIL'] = $email;				//	入力メールアドレス

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("henkou.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;

}



//	エラーチェック＆ユーザー情報取得
function email_pass_check(&$ERROR) {
	global $conn_id; // mysql DB

	$_SESSION['member'] = array();

	//	メール＆パスワード取得
	$email = $_POST['email'];
#	$email = mb_convert_kana($email, "as", "EUC-JP");
	$email = mb_convert_kana($email, "as", "UTF-8");
	$email = trim($email);
	$pass = $_POST['pass'];

	$_SESSION['member']['email'] = $email;



	//	エラーチェック
	if ($email == "") {
		$ERROR[] = "メールアドレスが入力されておりません。";
	}
	if ($email && !preg_match("/^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)/",$email,$regs)) {
		$ERROR[] = "メールアドレスが不正です。";
	}
	if (!$pass) {
		$ERROR[] = "パスワードが入力されておりません。";
	}
	$pass_l = strlen($pass);	//	strlen→文字列の長さを得る
	if ($pass && $pass_l < 6 || $pass_l > 8) {
		$ERROR[] = "パスワードが不正です。";
	}

	$idpass = $_SESSION['idpass'];
	list($id_email,$id_pass) = explode("<>",$idpass);
	if(!$ERROR){
		if($id_email != $email){
			$ERROR[] = "ログイン中のメールアドレスと一致していません。";
		}
		if($id_pass != $pass){
			$ERROR[] = "ログイン中のパスワードと一致していません。";
		}
	}



	if(!$ERROR){
		$kojin_num = 0;
		$sql  = "SELECT * FROM ".T_KOJIN.
				" WHERE email='".$email."'".
				" AND pass='".$pass."'".
				" AND saku!='1'".
				" AND kojin_num<'100000'".
				" ORDER BY kojin_num;";

		if ($result = mysqli_query($conn_id, $sql)) {
			$list = mysqli_fetch_array($result);
			$kojin_num = $list['kojin_num'];

			if ($kojin_num < 1) {
				$ERROR[] = "登録されていないか、入力された情報が間違っています。";
			}

			if(!$ERROR){

				//	ユーザー情報取得
				$name_s = $list['name_s'];								//	姓
				$name_n = $list['name_n'];								//	名
				$kana_s = $list['kana_s'];								//	姓：ふりがな
				$kana_n = $list['kana_n'];								//	名：ふりがな
				$zip1 = $list['zip1'];									//	郵便番号1
				$zip2 = $list['zip2'];									//	郵便番号2
				$prf = $list['prf'];									//	都道府県
				$city = $list['city'];									//	市区町村名
				$add1 = $list['add1'];									//	所番地
				$add2 = $list['add2'];									//	マンション名など
				$tel1 = $list['tel1'];									//	電話番号1
				$tel2 = $list['tel2'];									//	電話番号2
				$tel3 = $list['tel3'];									//	電話番号3
				$fax1 = $list['fax1'];									//	FAX1
				$fax2 = $list['fax2'];									//	FAX2
				$fax3 = $list['fax3'];									//	FAX3
				$email = $list['email'];								//	メールアドレス
				$pass = $list['pass'];									//	パスワード
				$meruma = $list['meruma'];								//	メルマガラジオボタン
				$point = $list['point'];								//	ポイント

				$_SESSION['member']['name_s'] = $name_s;
				$_SESSION['member']['name_n'] = $name_n;
				$_SESSION['member']['kana_s'] = $kana_s;
				$_SESSION['member']['kana_n'] = $kana_n;
				$_SESSION['member']['zip1'] = $zip1;
				$_SESSION['member']['zip2'] = $zip2;
				$_SESSION['member']['prf'] = $prf;
				$_SESSION['member']['city'] = $city;
				$_SESSION['member']['add1'] = $add1;
				$_SESSION['member']['add2'] = $add2;
				$_SESSION['member']['tel1'] = $tel1;
				$_SESSION['member']['tel2'] = $tel2;
				$_SESSION['member']['tel3'] = $tel3;
				$_SESSION['member']['fax1'] = $fax1;
				$_SESSION['member']['fax2'] = $fax2;
				$_SESSION['member']['fax3'] = $fax3;
				$_SESSION['member']['email'] = $email;
				$_SESSION['member']['pass'] = $pass;
				$_SESSION['member']['meruma'] = $meruma;
				$_SESSION['member']['point'] = $point;

				$_SESSION['member']['kojin_num'] = $kojin_num;

			}
		}
	}

}



//	入力フォームエラーチェック
function member_check(&$ERROR){
	global $conn_id; // mysql DB

	//	ユーザー情報取得
	$name_s = $_POST['name_s'];								//	姓
#	$name_s = mb_convert_kana($name_s, "asKV", "EUC-JP");
	$name_s = mb_convert_kana($name_s, "asKV","UTF-8");
	$name_s = trim($name_s);
	$name_n = $_POST['name_n'];								//	名
#	$name_n = mb_convert_kana($name_n, "asKV", "EUC-JP");
	$name_n = mb_convert_kana($name_n, "asKV","UTF-8");
	$name_n = trim($name_n);
	$kana_s = $_POST['kana_s'];								//	姓：ふりがな
#	$kana_s = mb_convert_kana($kana_s, "ascHV", "EUC-JP");
	$kana_s = mb_convert_kana($kana_s, "ascHV","UTF-8");
	$kana_s = trim($kana_s);
	$kana_n = $_POST['kana_n'];								//	名：ふりがな
#	$kana_n = mb_convert_kana($kana_n, "ascHV", "EUC-JP");
	$kana_n = mb_convert_kana($kana_n, "ascHV","UTF-8");
	$kana_n = trim($kana_n)	;
	$zip1 = $_POST['zip1'];									//	郵便番号1
#	$zip1 = mb_convert_kana($zip1, "as", "EUC-JP");
	$zip1 = mb_convert_kana($zip1, "as","UTF-8");
	$zip2 = $_POST['zip2'];									//	郵便番号2
#	$zip2 = mb_convert_kana($zip2, "as", "EUC-JP");
	$zip2 = mb_convert_kana($zip2, "as","UTF-8");
	$prf = $_POST['prf'];									//	都道府県
	$city = $_POST['city'];									//	市区町村名
#	$city = mb_convert_kana($city, "asKV", "EUC-JP");
	$city = mb_convert_kana($city, "asKV","UTF-8");
	$add1 = $_POST['add1'];									//	所番地
#	$add1 = mb_convert_kana($add1, "asKV", "EUC-JP");
	$add1 = mb_convert_kana($add1, "asKV","UTF-8");
	$add2 = $_POST['add2'];									//	マンション名など
#	$add2 = mb_convert_kana($add2, "asKV", "EUC-JP");
	$add2 = mb_convert_kana($add2, "asKV","UTF-8");
	$tel1 = $_POST['tel1'];									//	電話番号1
#	$tel1 = mb_convert_kana($tel1, "as", "EUC-JP");
	$tel1 = mb_convert_kana($tel1, "as","UTF-8");
	$tel2 = $_POST['tel2'];									//	電話番号2
#	$tel2 = mb_convert_kana($tel2, "as", "EUC-JP");
	$tel2 = mb_convert_kana($tel2, "as","UTF-8");
	$tel3 = $_POST['tel3'];									//	電話番号3
#	$tel3 = mb_convert_kana($tel3, "as", "EUC-JP");
	$tel3 = mb_convert_kana($tel3, "as","UTF-8");
	$fax1 = $_POST['fax1'];									//	FAX1
#	$fax1 = mb_convert_kana($fax1, "as", "EUC-JP");
	$fax1 = mb_convert_kana($fax1, "as","UTF-8");
	$fax2 = $_POST['fax2'];									//	FAX2
#	$fax2 = mb_convert_kana($fax2, "as", "EUC-JP");
	$fax2 = mb_convert_kana($fax2, "as","UTF-8");
	$fax3 = $_POST['fax3'];									//	FAX3
#	$fax3 = mb_convert_kana($fax3, "as", "EUC-JP");
	$fax3 = mb_convert_kana($fax3, "as","UTF-8");
	$email = $_POST['email'];								//	メールアドレス
#	$email = mb_convert_kana($email, "as", "EUC-JP");
	$email = mb_convert_kana($email, "as","UTF-8");
	$email = trim($email);
	$pass = $_SESSION['member']['pass'];									//	現在のパスワード
	$pass1 = $_POST['pass1'];								//	変更後のパスワード
	$pass2 = $_POST['pass2'];								//	変更後の確認パスワード
	$meruma = $_POST['meruma'];								//	メルマガラジオボタン

	//	ユーザー情報エラーチェック
	if ($name_s == "") {
		$ERROR[] = "漢字氏名（姓）が入力されておりません。";
	}
	if ($name_n == "") {
		$ERROR[] = "漢字氏名（名）が入力されておりません。";
	}
	if ($kana_s == "") {
		$ERROR[] = "ふりがな氏名（姓）が入力されておりません。";
	}
	if ($kana_n == "") {
		$ERROR[] = "ふりがな氏名（名）が入力されておりません。";
	}
	if ($zip1 == "" || $zip2 == ""){
		$ERROR[] = "郵便番号が入力されておりません。";
	}
	if (!preg_match("/^[0-9]+$/",$zip1)|| !preg_match("/^[0-9]+$/",$zip2)){
		$ERROR[] = "郵便番号が不正です。（半角数学で入力お願いします）";
	}
	if ($prf == "") {
		$ERROR[] = "都道府県名が選択されておりません。";
	}
	if ($city == "") {
		$ERROR[] = "市区町村名が入力されておりません。";
	}
	if ($add1 == "") {
		$ERROR[] = "所番地が入力されておりません。";
	}
	if ($tel1 == "" || $tel2 == "" || $tel3 == "" ){
		$ERROR[] = "電話番号が入力されておりません。";
	}
	if (!preg_match("/^[0-9]*$/",$tel1)|| !preg_match("/^[0-9]*$/",$tel2)|| !preg_match("/^[0-9]*$/",$tel3)){
		$ERROR[] = "電話番号が不正です。（半角数学で入力お願いします）";
	}
	if ($email == "") {
		$ERROR[] = "メールアドレスが入力されておりません。";
	}
	if ($email && !preg_match("/^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)/",$email,$regs)) {
		$ERROR[] = "メールアドレスが不正です。";
	}


	$idpass = $_SESSION['idpass'];
	list($user_email, $user_pass, $check, $af_num_xx, $name_s_xx, $point_xx) = explode("<>", $idpass);
	if ($email) {
		$sql =  "SELECT email FROM ".T_KOJIN.
				" WHERE email='".$email."';";
		$sql1 = mysqli_query($conn_id,$sql);
		$count = mysqli_num_rows($sql1);
		if ($count >= 1 && $email != $user_email) {
				$ERROR[] = "すでに入力されたメールアドレスは登録されております。";
		}
	}


	if (!$pass) {
		$ERROR[] = "現在のパスワードが入力されておりません。";
	}
	$pass_l = strlen($pass);
	if ($pass && $pass_l < 6 || $pass_l > 8) {
		$ERROR[] = "現在のパスワードが不正です。";
	}
	if ($pass1 || $pass2) {

		if (!$pass1) {
			$ERROR[] = "変更後のパスワードが入力されておりません。";
		}
		if (!$pass2) {
			$ERROR[] = "確認パスワードが入力されておりません。";
		}
		$pass1_l = strlen($pass1);
		if ($pass1 && $pass1_l < 6 || $pass1_l > 8) {
			$ERROR[] = "変更後のパスワードが不正です。";
		}
		$pass2_l = strlen($pass2);
		if ($pass2 && $pass2_l < 6 || $pass2_l > 8) {
			$ERROR[] = "確認パスワードが不正です。";
		}
		if ($pass1 && $pass2 && $pass1 != $pass2) {
			$ERROR[] = "変更後のパスワードと確認用パスワードが一致しておりません。";
		} elseif ($pass1 && $pass2 && $pass1 == $pass2) {
			$new_pass = $pass1;
		}
	}

	if ($meruma == "") {
		$ERROR[] = "メールマガジン購読が選択されておりません。";
	}

	$_SESSION['member']['name_s'] = $name_s;
	$_SESSION['member']['name_n'] = $name_n;
	$_SESSION['member']['kana_s'] = $kana_s;
	$_SESSION['member']['kana_n'] = $kana_n;
	$_SESSION['member']['zip1'] = $zip1;
	$_SESSION['member']['zip2'] = $zip2;
	$_SESSION['member']['prf'] = $prf;
	$_SESSION['member']['city'] = $city;
	$_SESSION['member']['add1'] = $add1;
	$_SESSION['member']['add2'] = $add2;
	$_SESSION['member']['tel1'] = $tel1;
	$_SESSION['member']['tel2'] = $tel2;
	$_SESSION['member']['tel3'] = $tel3;
	$_SESSION['member']['fax1'] = $fax1;
	$_SESSION['member']['fax2'] = $fax2;
	$_SESSION['member']['fax3'] = $fax3;
	$_SESSION['member']['email'] = $email;
	$_SESSION['member']['pass'] = $pass;
	$_SESSION['member']['new_pass'] = $new_pass;
	$_SESSION['member']['meruma'] = $meruma;

}



//	変更情報入力フォーム
function member_form($ERROR) {

	global $PRF_N;

	$html = "";

	if ($_POST['modes'] != "") {

		$name_s = $_SESSION['member']['name_s'];
		$name_n = $_SESSION['member']['name_n'];
		$kana_s = $_SESSION['member']['kana_s'];
		$kana_n = $_SESSION['member']['kana_n'];
		$zip1 = $_SESSION['member']['zip1'];
		$zip2 = $_SESSION['member']['zip2'];
		$prf = $_SESSION['member']['prf'];
		$city = $_SESSION['member']['city'];
		$add1 = $_SESSION['member']['add1'];
		$add2 = $_SESSION['member']['add2'];
		$tel1 = $_SESSION['member']['tel1'];
		$tel2 = $_SESSION['member']['tel2'];
		$tel3 = $_SESSION['member']['tel3'];
		$fax1 = $_SESSION['member']['fax1'];
		$fax2 = $_SESSION['member']['fax2'];
		$fax3 = $_SESSION['member']['fax3'];
		$email = $_SESSION['member']['email'];
		$pass = $_SESSION['member']['pass'];
		$meruma = $_SESSION['member']['meruma'];

	} else {

		unset($_SESSION['member']);

	}



	//	都道府県プルダウン
	$prf_html .= "	<select class=\"input-full-length\" name=\"prf\">";
	$prf_html .= "		<option value=\"\">選択して下さい。</option>\n";
		for ($i = 1; $i <= 47; $i++) {
			if ($i == $prf) {
				$selected = "selected";
			} else {
				$selected = "";
			}
			$prf_html .= "		<option value=\"".$i."\"".$selected.">".$PRF_N[$i]."</option>\n";
		}
	$prf_html .= "	</select>";

	//	メルマガラジオ
	$checked = "checked=\"checked\"";
	if ($meruma == 1){
		$INPUTS['MERUMA1'] = $checked;	//	メルマガ購読する
	} elseif ($meruma == 2){
		$INPUTS['MERUMA2'] = $checked;	//	メルマガ購読しない
	}



	if ($ERROR) {
		$error_html = error_html($ERROR);
	}

	$DEL_INPUTS['TOUROKUDEL'] = 1;			//	入力内容確認ページ削除
	$DEL_INPUTS['TYUUIKAKUNINDEL'] = 1;					//　注意ブロック

	$INPUTS['NAMES'] = $name_s;				//	姓
	$INPUTS['NAMEN'] = $name_n;				//	名
	$INPUTS['KANAS'] = $kana_s;				//	姓：ふりがな
	$INPUTS['KANAN'] = $kana_n;				//	名：ふりがな
	$INPUTS['ZIP1'] = $zip1;				//	郵便番号1
	$INPUTS['ZIP2'] = $zip2;				//	郵便番号2
	$INPUTS['PRF'] = $prf_html;				//	都道府県
	$INPUTS['CITY'] = $city;				//	市区町村名
	$INPUTS['ADD1'] = $add1;				//	所番地
	$INPUTS['ADD2'] = $add2;				//	マンション名など
	$INPUTS['TEL1'] = $tel1;				//	電話番号1
	$INPUTS['TEL2'] = $tel2;				//	電話番号2
	$INPUTS['TEL3'] = $tel3;				//	電話番号3
	$INPUTS['FAX1'] = $fax1;				//	FAX1
	$INPUTS['FAX2'] = $fax2;				//	FAX2
	$INPUTS['FAX3'] = $fax3;				//	FAX3
	$INPUTS['EMAIL'] = $email;				//	メール
	$INPUTS['PASSL'] = "*********";				//	パスワード

	$INPUTS['ERRORMSG'] = $error_html;		//	エラーメッセージ

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("henkou_form.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;

}



//	確認ページ
function member_kakunin(){

	global $PRF_N;

	$html = "";

	if ($_SESSION['member']) {

		$name_s = $_SESSION['member']['name_s'];
		$name_n = $_SESSION['member']['name_n'];
		$kana_s = $_SESSION['member']['kana_s'];
		$kana_n = $_SESSION['member']['kana_n'];
		$zip1 = $_SESSION['member']['zip1'];
		$zip2 = $_SESSION['member']['zip2'];
		$prf = $_SESSION['member']['prf'];
		$city = $_SESSION['member']['city'];
		$add1 = $_SESSION['member']['add1'];
		$add2 = $_SESSION['member']['add2'];
		$tel1 = $_SESSION['member']['tel1'];
		$tel2 = $_SESSION['member']['tel2'];
		$tel3 = $_SESSION['member']['tel3'];
		$fax1 = $_SESSION['member']['fax1'];
		$fax2 = $_SESSION['member']['fax2'];
		$fax3 = $_SESSION['member']['fax3'];
		$pass = $_SESSION['member']['pass'];
		$new_pass = $_SESSION['member']['new_pass'];
		if($new_pass){
			$pass = $new_pass;
		}
		$email = $_SESSION['member']['email'];
		$meruma = $_SESSION['member']['meruma'];

	}

	if($zip1 && $zip2){
		$zipn = "〒".$zip1."-".$zip2;
	}
	$INPUTS['ZIPN'] = $zipn;	//	郵便番号表示

	if($tel1 && $tel2 && $tel3){
		$teln = $tel1."-".$tel2."-".$tel3;
	}
	$INPUTS['TELN'] = $teln;	//	電話番号表示

	if($fax1 && $fax2 && $fax3){
		$faxn = $fax1."-".$fax2."-".$fax3;
	}
	$INPUTS['FAXN'] = $faxn;	//	FAX番号表示

	if($meruma == 1){			//	メルマガ表示
		$meruma = "購読する";
	}
	if($meruma == 2){
		$meruma = "購読しない";
	}

	//	入力フォーム部分削除
	$DEL_INPUTS['NAMEDEL'] = 1;
	$DEL_INPUTS['KANADEL'] = 1;
	$DEL_INPUTS['ZIPDEL'] = 1;
	$DEL_INPUTS['CITYDEL'] = 1;
	$DEL_INPUTS['ADD1DEL'] = 1;
	$DEL_INPUTS['ADD2DEL'] = 1;
	$DEL_INPUTS['TELDEL'] = 1;
	$DEL_INPUTS['FAXDEL'] = 1;
	$DEL_INPUTS['EMAILDEL'] = 1;
	$DEL_INPUTS['PASSDEL'] = 1;
	$DEL_INPUTS['MSRDEL'] = 1;
	$DEL_INPUTS['ORDERBUTTONDEL'] = 1;
	$DEL_INPUTS['TYUUIDEL'] = 1;

	$INPUTS['NAMESN'] = $name_s;					//	姓
	$INPUTS['NAMENN'] = $name_n;					//	名
	$INPUTS['NAMESNN'] = $name_s."&nbsp;".$name_n;	//	名 + 姓
	$INPUTS['KANASN'] = $kana_s;					//	姓：ふりがな
	$INPUTS['KANANN'] = $kana_n;					//	名：ふりがな
	$INPUTS['KANASNN'] = $kana_s."&nbsp;".$kana_n;	//	名 + 姓 ：ふりがな
	$INPUTS['ZIP1N'] = $zip1;						//	郵便番号1
	$INPUTS['ZIP2N'] = $zip2;						//	郵便番号2
	$INPUTS['PRFN'] = $PRF_N[$prf];					//	都道府県
	$INPUTS['CITYN'] = $city;						//	市区町村名
	$INPUTS['ADD1N'] = $add1;						//	所番地
	$INPUTS['ADD2N'] = $add2;						//	マンション名など
	$INPUTS['TEL1N'] = $tel1;						//	電話番号1
	$INPUTS['TEL2N'] = $tel2;						//	電話番号2
	$INPUTS['TEL3N'] = $tel3;						//	電話番号3
	$INPUTS['FAX1N'] = $fax1;						//	FAX1
	$INPUTS['FAX2N'] = $fax2;						//	FAX2
	$INPUTS['FAX3N'] = $fax3;						//	FAX3
	$INPUTS['EMAILN'] = $email;						//	メールアドレス
	$INPUTS['PASSN'] = "*********";						//	パスワード
	$INPUTS['MERUMAN'] = $meruma;					//	メルマガ

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("henkou_form.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;

}



//	登録内容変更
function member_touroku(){

	global $PRF_N , $admin_name , $admin_mail_m , $m_footer,
			$conn_id; // mysql DB

	if($_POST['modes'] != "henkou"){
		echo("お客様情報が確認できません。");
		exit();
	}

	if ($_SESSION['member']) {

		$kojin_num = $_SESSION['member']['kojin_num'];
		$name_s = $_SESSION['member']['name_s'];
		$name_n = $_SESSION['member']['name_n'];
		$kana_s = $_SESSION['member']['kana_s'];
		$kana_n = $_SESSION['member']['kana_n'];
		$zip1 = $_SESSION['member']['zip1'];
		$zip2 = $_SESSION['member']['zip2'];
		$prf = $_SESSION['member']['prf'];
		$city = $_SESSION['member']['city'];
		$add1 = $_SESSION['member']['add1'];
		$add2 = $_SESSION['member']['add2'];
		$tel1 = $_SESSION['member']['tel1'];
		$tel2 = $_SESSION['member']['tel2'];
		$tel3 = $_SESSION['member']['tel3'];
		$fax1 = $_SESSION['member']['fax1'];
		$fax2 = $_SESSION['member']['fax2'];
		$fax3 = $_SESSION['member']['fax3'];
		$pass = $_SESSION['member']['pass'];
		$new_pass = $_SESSION['member']['new_pass'];
		if($new_pass){
			$pass = $new_pass;
		}
		$email = $_SESSION['member']['email'];
		$meruma = $_SESSION['member']['meruma'];
		$point = $_SESSION['member']['point'];

	}



	//	DB登録
	if ($_POST['modes'] == "henkou" && $kojin_num) {

		$sql =  "UPDATE ".T_KOJIN." SET" .
				" name_s='".$name_s."',".
				" name_n='".$name_n."',".
				" kana_s='".$kana_s."',".
				" kana_n='".$kana_n."',".
				" zip1='".$zip1."',".
				" zip2='".$zip2."',".
				" prf='".$prf."',".
				" city='".$city."',".
				" add1='".$add1."',".
				" add2='".$add2."',".
				" tel1='".$tel1."',".
				" tel2='".$tel2."',".
				" tel3='".$tel3."',".
				" fax1='".$fax1."',".
				" fax2='".$fax2."',".
				" fax3='".$fax3."',".
				" email='".$email."',".
				" pass='".$pass."',".
				" meruma='".$meruma."'".
				" WHERE kojin_num='".$kojin_num."';";

		if($sql){
			mysqli_query($conn_id,$sql);
		}
	}


	if($meruma == 1){
		$meruma = "購読する";
	}
	if($meruma == 2){
		$meruma = "購読しない";
	}

// お客様送信用
	$subject = "会員登録変更確認 - ネイバーズスポーツ -";

$msg = <<<EOT
{$subject}
{$name_s} 様 登録内容は以下でよろしいでしょうか？
もし間違いがある場合は、お手数ですがホームページの
会員登録変更で修正お願い致します。
------------------------------------------------------
漢字氏名
　{$name_s} {$name_n}

ふりがな
　{$kana_s} {$kana_n}

住所
　〒{$zip1}-{$zip2}
　{$PRF_N[$prf]} {$city} {$add1} {$add2}

電話番号
　{$tel1} - {$tel2} - {$tel3}

FAX番号
　{$fax1} - {$fax2} - {$fax3}

メールアドレス
　{$email}

パスワード
　***********

メールマガジン
　{$meruma}

{$m_footer}
EOT;

	//	メール送信
	$send_email = $admin_mail_m;
	$send_name = $admin_name;
	$get_email = $email;
	send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msg);





// 受け取り用
	$men = " ( No.".$kojin_num." )";
	$subject = "会員登録変更されました。".$men." - ネイバーズスポーツ -";

	$ip = getenv("REMOTE_ADDR");
	$host = gethostbyaddr($ip);
	if (!$host) {
		$host = $ip;
	}

$msg = <<<EOT
{$name_s} {$name_n} 様が {$subject}
------------------------------------------------------
漢字氏名
　{$name_s} {$name_n}

ふりがな
　{$kana_s} {$kana_n}

住所
　〒{$zip1}-{$zip2}
　{$PRF_N[$prf]} {$city} {$add1} {$add2}

電話番号
　{$tel1} - {$tel2} - {$tel3}

FAX番号
　{$fax1} - {$fax2} - {$fax3}

メールアドレス
　{$email}

パスワード
　*********

メールマガジン
　{$meruma}

------------------------------------------------------
{$host} ({$ip})
EOT;

	//	メール送信
	$send_email = $email;
	$send_name = $name_s .$name_n;
	$get_email = $admin_mail_m;
	send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msg);



	//	$_SESSION['idpass']を変更後のデータに書き換え
	$idpass = $_SESSION['idpass'];
	list($id_email, $id_pass, $check, $af_num_xx, $name_s_xx, $point_xx) = explode("<>", $idpass);
	$idpass = $email."<>".$pass."<>".$check."<>".$af_num."<>".$name_s."<>".$point_xx."<>";
	$_SESSION['idpass'] = $idpass;



	unset($_SESSION['member']);

	header ("Location: ../template/thank_h.htm");
	exit();

}

?>