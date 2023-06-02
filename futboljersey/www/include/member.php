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

//	会員概要
function member_summary($ERROR){

	$html = "";

	$email = $_SESSION['member']['email'];

	if ($ERROR) {
		$error_html = error_html($ERROR);
	}

	$INPUTS['ERRORMSG'] = $error_html;		//	エラーメッセージ
	$INPUTS['EMAIL'] = $email;				//	メールエラーメッセージ

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("member.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;

}



//	メールエラーチェック
function email_check(&$ERROR){
	global $conn_id; // mysql DB

	$_SESSION['member'] = array();

	//	メールアドレス取得
	$email = $_POST['email'];
#	$email = mb_convert_kana($email, "as", "EUC-JP");
	$email = mb_convert_kana($email, "as", "UTF-8");
	$email = trim($email);

	$_SESSION['member']['email'] = $email;



	//	メールエラーチェック
	if ($email == "") {
		$ERROR[] = "メールアドレスが入力されておりません。";
	}
	if ($email && !preg_match("/^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)/",$email,$regs)) {
		$ERROR[] = "メールアドレスが不正です。";
	}
	if ($email) {
		$sql =  "SELECT email FROM kojin WHERE email='".$email."';";
		$sql1 = mysqli_query($conn_id,$sql);
		$count = mysqli_num_rows($sql1);
		if ($count >= 1) {
			$ERROR[] = "すでに入力されたメールアドレスは登録されております。";
		}
	}

}



//	入力フォームエラーチェック
function member_check(&$ERROR){
	global $conn_id; // mysql DB

	$_SESSION['member'] = array();

	//	ユーザー情報取得
	$name_s = $_POST['name_s'];								//	姓
#	$name_s = mb_convert_kana($name_s, "asKV", "EUC-JP");
	$name_s = mb_convert_kana($name_s, "asKV", "UTF-8");
	$name_s = trim($name_s);
	$name_n = $_POST['name_n'];								//	名
#	$name_n = mb_convert_kana($name_n, "asKV", "EUC-JP");
	$name_n = mb_convert_kana($name_n, "asKV", "UTF-8");
	$name_n = trim($name_n);
	$kana_s = $_POST['kana_s'];								//	姓：ふりがな
#	$kana_s = mb_convert_kana($kana_s, "ascHV", "EUC-JP");
	$kana_s = mb_convert_kana($kana_s, "ascHV", "UTF-8");
	$kana_s = trim($kana_s);
	$kana_n = $_POST['kana_n'];								//	名：ふりがな
#	$kana_n = mb_convert_kana($kana_n, "ascHV", "EUC-JP");
	$kana_n = mb_convert_kana($kana_n, "ascHV", "UTF-8");
	$kana_n = trim($kana_n);
	$zip1 = $_POST['zip1'];									//	郵便番号1
#	$zip1 = mb_convert_kana($zip1, "as", "EUC-JP");
	$zip1 = mb_convert_kana($zip1, "as", "UTF-8");
	$zip2 = $_POST['zip2'];									//	郵便番号2
#	$zip2 = mb_convert_kana($zip2, "as", "EUC-JP");
	$zip2 = mb_convert_kana($zip2, "as", "UTF-8");
	$prf = $_POST['prf'];									//	都道府県
	$city = $_POST['city'];									//	市区町村名
#	$city = mb_convert_kana($city, "asKV", "EUC-JP");
	$city = mb_convert_kana($city, "asKV", "UTF-8");
	$add1 = $_POST['add1'];									//	所番地
#	$add1 = mb_convert_kana($add1, "asKV", "EUC-JP");
	$add1 = mb_convert_kana($add1, "asKV", "UTF-8");
	$add2 = $_POST['add2'];									//	マンション名など
#	$add2 = mb_convert_kana($add2, "asKV", "EUC-JP");
	$add2 = mb_convert_kana($add2, "asKV", "UTF-8");
	$tel1 = $_POST['tel1'];									//	電話番号1
#	$tel1 = mb_convert_kana($tel1, "as", "EUC-JP");
	$tel1 = mb_convert_kana($tel1, "as", "UTF-8");
	$tel2 = $_POST['tel2'];									//	電話番号2
#	$tel2 = mb_convert_kana($tel2, "as", "EUC-JP");
	$tel2 = mb_convert_kana($tel2, "as", "UTF-8");
	$tel3 = $_POST['tel3'];									//	電話番号3
#	$tel3 = mb_convert_kana($tel3, "as", "EUC-JP");
	$tel3 = mb_convert_kana($tel3, "as", "UTF-8");
	$fax1 = $_POST['fax1'];									//	FAX1
#	$fax1 = mb_convert_kana($fax1, "as", "EUC-JP");
	$fax1 = mb_convert_kana($fax1, "as", "UTF-8");
	$fax2 = $_POST['fax2'];									//	FAX2
#	$fax2 = mb_convert_kana($fax2, "as", "EUC-JP");
	$fax2 = mb_convert_kana($fax2, "as", "UTF-8");
	$fax3 = $_POST['fax3'];									//	FAX3
#	$fax3 = mb_convert_kana($fax3, "as", "EUC-JP");
	$fax3 = mb_convert_kana($fax3, "as", "UTF-8");
	$email = $_POST['email'];								//	メールアドレス
#	$email = mb_convert_kana($email, "as", "EUC-JP");
	$email = mb_convert_kana($email, "as", "UTF-8");
	$email = trim($email);
	$pass1 = $_POST['pass1'];								//	パスワード
	$pass2 = $_POST['pass2'];								//	確認パスワード
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
	if (!preg_match("/^[0-9]+$/",$tel1)|| !preg_match("/^[0-9]+$/",$tel2)|| !preg_match("/^[0-9]+$/",$tel3)){
		$ERROR[] = "電話番号が不正です。（半角数学で入力お願いします）";
	}
	if ($email == "") {
		$ERROR[] = "メールアドレスが入力されておりません。";
	}
	if ($email && !preg_match("/^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)/",$email,$regs)) {
		$ERROR[] = "メールアドレスが不正です。";
	}

	if ($email) {
		$sql =  "SELECT email FROM ".T_KOJIN." as kojin".
				" WHERE email='".$email."';";
		$sql1 = mysqli_query($conn_id,$sql);
		$count = mysqli_num_rows($sql1);
		if ($count >= 1) {
			$ERROR[] = "すでに入力されたメールアドレスは登録されております。";
		}
	}

	if (!$pass1) {
		$ERROR[] = "パスワードが入力されておりません。";
	}
	if ($pass1 && !preg_match("/^[A-Za-z0-9]{6,8}$/", $pass1)){
		$ERROR[] = "パスワードが不正です。";
	}
	// $pass1_l = strlen($pass1);	//	strlen→文字列の長さを得る
	// if ($pass1 && $pass1_l < 6 || $pass1_l > 8) {
	// 	$ERROR[] = "パスワードが不正です。";
	// }
	if (!$pass2) {
		$ERROR[] = "確認パスワードが入力されておりません。";
	}
	$pass2_l = strlen($pass2);
	if ($pass2 && $pass2_l < 6 || $pass2_l > 8) {
		$ERROR[] = "確認パスワードが不正です。";
	}
	if ($pass1 && $pass2 && $pass1 != $pass2) {
		$ERROR[] = "パスワードと確認用パスワードが一致しておりません。";
	} elseif ($pass1 && $pass2 && $pass1 == $pass2) {
		$pass = $pass1;
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
	$_SESSION['member']['meruma'] = $meruma;

}



//	ユーザー情報入力
function member_form($ERROR){

	global $PRF_N;

	$html = "";

	if ($ERROR) {
		$error_html = error_html($ERROR);
	}

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


	$DEL_INPUTS['TOUROKUDEL'] = 1;			//	「登録する」・「修正する」ボタン削除
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

	$INPUTS['ERRORMSG'] = $error_html;		//	エラーメッセージ

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("member_form.htm");
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

	if($meruma == 1){			//	メルマガ購読表示
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
	$INPUTS['NAMESNN'] = $name_s."&nbsp;".$name_n;				//	名 + 姓
	$INPUTS['KANASN'] = $kana_s;					//	姓：ふりがな
	$INPUTS['KANANN'] = $kana_n;					//	名：ふりがな
	$INPUTS['KANASNN'] = $kana_s."&nbsp;".$kana_n;  //	名 + 姓 :ふりがな
	$INPUTS['PRFN'] = $PRF_N[$prf];					//	都道府県
	$INPUTS['CITYN'] = $city;						//	市区町村名
	$INPUTS['ADD1N'] = $add1;						//	所番地
	$INPUTS['ADD2N'] = $add2;						//	マンション名など
	$INPUTS['EMAILN'] = $email;						//	メールアドレス
	$INPUTS['PASSN'] = "*********";						//	パスワード
	$INPUTS['MERUMAN'] = $meruma;					//	メルマガ

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("member_form.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;

}




//	登録実行
function member_touroku(){

	global $PRF_N , $admin_name , $admin_mail_m , $m_footer, 
			$conn_id; // mysql DB;

	if($_POST['modes'] != "touroku"){
		echo("お客様情報が確認できません。");
		exit();
	}

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
		$email = $_SESSION['member']['email'];
		$meruma = $_SESSION['member']['meruma'];

	}



	//	DB登録
	if ($_POST['modes'] == "touroku") {
		$sql =  "SELECT kojin_num FROM ".T_KOJIN.
				" WHERE kojin_num<='100000'".
				" ORDER BY kojin_num desc;";	//	降順に並べ替え
		$sql1 = mysqli_query($conn_id,$sql);
		$check = mysqli_num_rows($sql1);
		if ($check >= 1) {
			$list = mysqli_fetch_array($sql1);
			$kojin_num = $list['kojin_num'];
			$kojin_num = $kojin_num + 1;
		}else {
			$kojin_num = 1;
		}

		//	新規登録ポイントプレゼント
		$point = 0;
		$flag = 1;
		if (MEM_REG_START && MEM_REG_END) {
			//	開始期間
			list($s_year,$s_mon,$s_day) = explode("/",MEM_REG_START);
			$s_time = mktime(0,0,0,$s_mon,$s_day,$s_year);

			//	終了期間
			list($f_year,$f_mon,$f_day) = explode("/",MEM_REG_END);
			$f_time = mktime(0,0,0,$f_mon,$f_day+1,$f_year);

			//	今の時間
			$n_time = time();
			if ($s_time > $n_time || $n_time > $f_time) {
				$flag = 0;
			}
		}
		if (MEM_REG_POINT > 0 && $flag == 1) {
			$point = MEM_REG_POINT;
		}

		$sql = "INSERT INTO kojin VALUES (".
					"'".$kojin_num."',".
					"'".$name_s."',".
					"'".$name_n."',".
					"'".$kana_s."',".
					"'".$kana_n."',".
					"'".$zip1."',".
					"'".$zip2."',".
					"'".$prf."',".
					"'".$city."',".
					"'".$add1."',".
					"'".$add2."',".
					"'".$tel1."',".
					"'".$tel2."',".
					"'".$tel3."',".
					"'".$fax1."',".
					"'".$fax2."',".
					"'".$fax3."',".
					"'".$email."',".
					"'".$pass."',".
					"'".$meruma."',".
					"'".$point."',".
					"'0',".
					"'0',".
					"'0',".
					"'0',".
					"'0');";

	}
	if($sql){
		mysqli_query($conn_id,$sql);
	}



	if($meruma == 1){
		$meruma = "購読する";
	}
	if($meruma == 2){
		$meruma = "購読しない";
	}

// お客様送信用
	$subject = "会員登録確認 - ネイバーズスポーツ -";

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
　{*********}

メールマガジン
　{$meruma}

{$m_footer}
EOT;


	$send_email = $admin_mail_m;
	$send_name = $admin_name;
	$get_email = $email;
	send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msg);





// 受け取り用
	$sql  = "SELECT kojin_num FROM ".T_KOJIN.
			" WHERE email='".$email."'".
			" AND pass='".$pass."'".
			" AND saku!='1'".
			" AND kojin_num<'100000'".
			" ORDER BY kojin_num;";

	if ($result = mysqli_query($conn_id, $sql)) {		//	SQLの実行

		$list = mysqli_fetch_array($result);	//	行を配列として取得する
		$kojin_num = $list['kojin_num'];
		$men = " ( No.".$kojin_num." )";
		$subject = "会員登録されました。".$men." - ネイバーズスポーツ -";

	}

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
　{*********}

メールマガジン
　{$meruma}

------------------------------------------------------
{$host} ({$ip})
EOT;

	$send_email = $email;
	$send_name = $name_s .$name_n;
	$get_email = $admin_mail_m;
	send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msg);

	unset($_SESSION['member']);

	header ("Location: ../template/thank_n.htm");
	exit();

}

?>