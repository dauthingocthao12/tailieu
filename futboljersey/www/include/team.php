<?PHP
//	メール配信サブルーチン
function send_email($send_email, $send_name, $mail_bcc, $bcc_email, $get_email, $subject, $msg)
{

	//	文字コード宣言
	mb_language("Japanese");
	#	mb_internal_encoding("EUC-JP");
	mb_internal_encoding("UTF-8");
	$send_name = header_base64_encode($send_name);
	$from = "From: " . $send_name . " <" . $send_email . ">\nReply-To: " . $send_email . "\n";
	if ($mail_bcc == 1) {
		$from .= "Bcc: " . $bcc_email . "\n";
	}

	//	文字化け対策
	$subject = "　" . $subject;

	mb_send_mail($get_email, $subject, $msg, $from, "-f$send_email");
}
function header_base64_encode($str)
{

	#	$result = iconv("EUC-JP", "ISO-2022-JP", $str).chr(27).'(B';	//iconv 文字列を指定した文字エンコーディングに変換する
	$result = iconv("UTF-8", "ISO-2022-JP", $str) . chr(27) . '(B';	//iconv 文字列を指定した文字エンコーディングに変換する
	$result = '=?ISO-2022-JP?B?' . base64_encode($result) . '?=';

	return $result;
}



//	未注文状態
function not_team_order()
{

	$html = "";

	$DEL_INPUTS['ORDERKAKUNIN'] = 1;
	$DEL_INPUTS['TYUUIKAKUNINDEL'] = 1;					//　注意ブロック
	$DEL_INPUTS['TYUUIDEL'] = 1;					//　注意ブロック

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("team.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;
}



//	チームオーダー入力画面
function team_order($customer, $ERROR)
{

	global $PRF_N , $SEBAN_N , $SEBAN_P_N , $SENAME_N , $SENAME_P_N , $MUNEBAN_N , $MUNEBAN_P_N , $PANT_N , $PANT_P_N , $BACH_N , $BACH_P_N,
			$conn_id; // mysql DB

	$html = "";

	$INPUTS = array();
	$DEL_INPUTS = array();

	if ($ERROR) {
		$error_html = error_html($ERROR);
	}

	//	ログイン状態ならDB接続、登録データをフォームに入れる
	$idpass = $_SESSION['idpass'];
	if ($idpass) {
		list($email_,$pass,$check,$af_num) = explode("<>",$idpass);
		$sql =  "SELECT * FROM ".T_KOJIN.
				" WHERE email='".$email_."'".
				" AND pass='".$pass."'".
				" AND saku!='1'".
				" AND kojin_num<'100000';";
		$sql1 = mysqli_query($conn_id,$sql);
		$count = mysqli_num_rows($sql1);
		if ($count >= 1) {
			list($kojin_num,$name_s,$name_n,$kana_s,$kana_n,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$n,$n,$point,$saku,$kei1,$kei2,$kei3,$email2) = mysqli_fetch_array($sql1);
		}
	}

	if ($_POST['modes'] != "") {

		$seban_l = $_SESSION['team_order']['seban_l'];
		$sename_l = $_SESSION['team_order']['sename_l'];
		$muneban_l = $_SESSION['team_order']['muneban_l'];
		$pant_l = $_SESSION['team_order']['pant_l'];
		$bach_l = $_SESSION['team_order']['bach_l'];

		//$p_set = $_SESSION['team_order']['p_set'];
		//$s_set = $_SESSION['team_order']['s_set'];

		$name_s = $_SESSION['team_order']['name_s'];
		$name_n = $_SESSION['team_order']['name_n'];
		$kana_s = $_SESSION['team_order']['kana_s'];
		$kana_n = $_SESSION['team_order']['kana_n'];
		$zip1 = $_SESSION['team_order']['zip1'];
		$zip2 = $_SESSION['team_order']['zip2'];
		$prf = $_SESSION['team_order']['prf'];
		$city = $_SESSION['team_order']['city'];
		$add1 = $_SESSION['team_order']['add1'];
		$add2 = $_SESSION['team_order']['add2'];
		$tel1 = $_SESSION['team_order']['tel1'];
		$tel2 = $_SESSION['team_order']['tel2'];
		$tel3 = $_SESSION['team_order']['tel3'];
		$fax1 = $_SESSION['team_order']['fax1'];
		$fax2 = $_SESSION['team_order']['fax2'];
		$fax3 = $_SESSION['team_order']['fax3'];
		$email = $_SESSION['team_order']['email'];
		$msr = $_SESSION['team_order']['msr'];
	} else {

		unset($_SESSION['team_order']);
	}



	$DEL_INPUTS['NOTORDER'] = 1;	//	「※チームオーダーする商品を選択してきて下さい」非表示
	$DEL_INPUTS['CHECK'] = 1;
	$DEL_INPUTS['OMITUMORI'] = 1;
	$DEL_INPUTS['TYUUIKAKUNINDEL'] = 1;					//　注意ブロック

	/*
	//	パンツ&ソックスセット	注文プログラム
	if($p_set == 1){
		$checked_p="checked=checked";
	}
	$p_set_html.="<tr>\n";
	$p_set_html.="	<th>パンツ</th>\n";
	$p_set_html.="</tr>\n";
	$p_set_html.="<tr>\n";
	$p_set_html.="	<td>\n";
	$p_set_html.="		<input type=\"checkbox\" name=\"p_set\" value=\"1\" ".$checked_p.">：パンツをセットで申し込む場合はチェックを入れて下さい。\n";
	$p_set_html.="	</td>\n";
	$p_set_html.="</tr>\n";

	if($s_set == 1){
		$checked_s="checked=\"checked\"";
	}
	$s_set_html.="<tr>\n";
	$s_set_html.="	<th>ソックス</th>\n";
	$s_set_html.="</tr>\n";
	$s_set_html.="<tr>\n";
	$s_set_html.="	<td>\n";
	$s_set_html.="		<input type=\"checkbox\" name=\"s_set\" value=\"1\" ".$checked_s.">：ソックスをセットで申し込む場合はチェックを入れて下さい。\n";
	$s_set_html.="	</td>\n";
	$s_set_html.="</tr>\n";

	$INPUTS['PANTSET'] = $p_set_html;		//	パンツセット
	$INPUTS['SOCKSSET'] = $s_set_html;		//	ソックスセット
*/

	//	背番号プルダウン
	$max = count($SEBAN_N);
	$seban_l_html .= "	<select name=\"seban_l\" class=\"input-full-length\">\n";
	$seban_l_html .= "		<option value=\"0\">無し</option>\n";
	for ($i = 1; $i < $max; $i++) {
		if ($seban_l == $i) {
			$selected = "selected";
		} else {
			$selected = "";
		}
		$price_t = number_format($SEBAN_P_N[$i]);
		$seban_l_html .= "		<option value=\"" . $i . "\" " . $selected . ">" . $SEBAN_N[$i] . "：" . $price_t . "円/1文字</option>\n";
	}
	$seban_l_html .= "	</select>\n";



	//	背ネームプルダウン
	$max = count($SENAME_N);
	$sename_l_html .= "	<select name=\"sename_l\" class=\"input-full-length\">\n";
	$sename_l_html .= "		<option value=\"0\">無し</option>\n";
	for ($i = 1; $i < $max; $i++) {
		if ($sename_l == $i) {
			$selected = "selected";
		} else {
			$selected = "";
		}
		$price_t = number_format($SENAME_P_N[$i]);
		$sename_l_html .= "		<option value=\"" . $i . "\" " . $selected . ">" . $SENAME_N[$i] . "：" . $price_t . "円/1文字</option>\n";
	}
	$sename_l_html .= "	</select>\n";



	//	胸番号プルダウン
	$max = count($MUNEBAN_N);
	$muneban_l_html .= "	<select name=\"muneban_l\" class=\"input-full-length\">\n";
	$muneban_l_html .= "		<option value=\"0\">無し</option>\n";
	for ($i = 1; $i < $max; $i++) {
		if ($muneban_l == $i) {
			$selected = "selected";
		} else {
			$selected = "";
		}
		$price_t = number_format($MUNEBAN_P_N[$i]);
		$muneban_l_html .= "		<option value=\"" . $i . "\" " . $selected . ">" . $MUNEBAN_N[$i] . "：" . $price_t . "円/1文字</option>\n";
	}
	$muneban_l_html .= "	</select>\n";



	//	パンツ番号プルダウン
	$max = count($PANT_N);
	$pant_l_html .= "	<select name=\"pant_l\" class=\"input-full-length\">\n";
	$pant_l_html .= "		<option value=\"0\">無し</option>\n";
	for ($i = 1; $i < $max; $i++) {
		if ($pant_l == $i) {
			$selected = "selected";
		} else {
			$selected = "";
		}
		$price_t = number_format($PANT_P_N[$i]);
		$pant_l_html .= "		<option value=\"" . $i . "\" " . $selected . ">" . $PANT_N[$i] . "：" . $price_t . "円/1文字</option>\n";
	}
	$pant_l_html .= "	</select>\n";



	//	バッジプルダウン
	$max = count($BACH_N);
	$bach_l_html .= "	<select name=\"bach_l\">\n";
	$bach_l_html .= "		<option value=\"0\">無し</option>\n";
	for ($i = 1; $i < $max; $i++) {
		if ($bach_l == $i) {
			$selected = "selected";
		} else {
			$selected = "";
		}
		$price_t = number_format($BACH_P_N[$i]);
		$bach_l_html .= "		<option value=\"" . $i . "\" " . $selected . ">" . $BACH_N[$i] . "：" . $price_t . "円/1文字</option>\n";
	}
	$bach_l_html .= "	</select>\n";



	//	チームオーダー各詳細入力
	$result = explode("<>", rtrim($customer, "<>"));
	// echo '<pre>'; var_export( $result ); echo '</pre>'; // DBG

	foreach ($result as $val) {
		// 各商品：

		list($hinban, $title, $kakaku, $buy_n) = explode("::", $val);

		// 商品グループ
		$html .= "<div class=\"box-outline primary-color\">\n";
		$html .= "	<div class=\"box-grid primary-color box-form\">\n";

		if ($val) {

			$html .= "		<div class=\"team-order-goods\">\n";
			$html .= "			<dl>\n";
			$html .= "				<span>\n";
			$html .= "					<dt>商品名</dt>\n";
			$html .= "					<dd>" . $title . "</dd>\n";
			$html .= "				</span>\n";
			$html .= "				<span>\n";
			$html .= "					<dt>商品番号</dt>\n";
			$html .= "					<dd>" . $hinban . "</dd>\n";
			$html .= "				</span>\n";
			$html .= "				<span>\n";
			$html .= "					<dt>数量</dt>\n";
			$html .= "					<dd>" . $buy_n . "</dd>\n";
			$html .= "				</span>\n";
			$html .= "			</dl>\n";
			$html .= "		</div>\n";
		}

		for ($i = 1; $i <= $buy_n; $i++) {
			// 商品に対して、各マーキングの着名:

			$hinban_h = preg_replace("/-/", "_", $hinban);
			$names_ban = $hinban_h . "_ban_" . $i;
			$names_name = $hinban_h . "_name_" . $i;

			$check_names_ban = $_POST[$names_ban];
			#			$check_names_ban = mb_convert_kana($check_names_ban, "n", "EUC-JP");
			$check_names_ban = mb_convert_kana($check_names_ban, "n", "UTF-8");
			$check_names_ban = trim($check_names_ban);

			$check_names_name = $_POST[$names_name];
			#			$check_names_name = mb_convert_kana($check_names_name, "asKV", "EUC-JP");
			$check_names_name = mb_convert_kana($check_names_name, "asKV", "UTF-8");
			$check_names_name = trim($check_names_name);

			$check_names_ban = $_SESSION['team_order'][$names_ban];
			$check_names_name = $_SESSION['team_order'][$names_name];

			$html .= "		<div class=\"team-order-title box-row\">\n";
			$html .= "			<span class=\"first-item\">" . $i . "着目</span>\n";
			$html .= "			<div class=\"names-on-back\">\n";
			$html .= "				<div>\n";
			$html .= "					<label>番号：</label>";
			$html .= "						<input class=\"input-length-3-letters\" type=\"text\" maxlength=\"3\" name=\"" . $names_ban . "\" value=\"" . $check_names_ban . "\">\n";
			$html .= "				</div>\n";
			$html .= "				<div class=\"input-name-back\">\n";
			$html .= "					<label>背ネーム：</label>";
			$html .= "						<input type=\"text\" maxlength=\"20\" name=\"" . $names_name . "\" value=\"" . $check_names_name . "\">\n";
			$html .= "				</div>\n";
			$html .= "			</div>\n";
			$html .= "		</div>\n";
		}

		$html .= "	</div>\n"; // end box-grid
		$html .= "</div>\n"; // end box-outline

		// end 商品グループ
	}

	//	都道府県プルダウン
	$prf_html .= "	<select name=\"prf\" class=\"input-full-length\">";
	$prf_html .= "		<option value=\"\">選択して下さい。</option>\n";
	for ($i = 1; $i <= 47; $i++) {
		if ($i == $prf) {
			$selected = "selected";
		} else {
			$selected = "";
		}

		$prf_html .= "		<option value=\"" . $i . "\"" . $selected . ">" . $PRF_N[$i] . "</option>\n";
	}
	$prf_html .= "	</select>";



	$INPUTS['SEBANL'] = $seban_l_html;		//	背番号タイプ
	$INPUTS['SENAMEL'] = $sename_l_html;	//	背ネームタイプ
	$INPUTS['MUNEBANL'] = $muneban_l_html;	//	胸番号タイプ
	$INPUTS['PANTL'] = $pant_l_html;		//	パンツ番号タイプ
	$INPUTS['BACHL'] = $bach_l_html;		//	バッヂタイプ

	$INPUTS['NAMES'] = $name_s;				//	姓
	$INPUTS['NAMEN'] = $name_n;				//	名
	$INPUTS['NAMESNN'] =$name_s."&nbsp;".$name_n;//	姓　名
	$INPUTS['KANAS'] = $kana_s;				//	姓：ふりがな
	$INPUTS['KANAN'] = $kana_n;				//	名：ふりがな
	$INPUTS['KANASNN'] =$kana_s."&nbsp;".$kana_n;//  姓　名 ふりがな
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
	$INPUTS['EMAIL'] = $email;				//	メールアドレス
	$INPUTS['MSR'] = $msr;					//	ご意見ご要望

	$INPUTS['ERRORMSG'] = $error_html;		//	エラーメッセージ
	$INPUTS['GOODSLIST'] = $html;			//	オーダー内容入力html

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("team.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;
}



//	エラーチェック
function team_order_check($customer, &$ERROR)
{

	//	チームオーダー情報取得
	$seban_l = $_POST['seban_l'];							//	背番号タイプ
	$sename_l = $_POST['sename_l'];							//	背ネームタイプ
	$muneban_l = $_POST['muneban_l'];						//	胸番号タイプ
	$pant_l = $_POST['pant_l'];								//	パンツ番号タイプ
	$bach_l = $_POST['bach_l'];								//	バッヂタイプ

	$_SESSION['team_order']['seban_l'] = $seban_l;
	$_SESSION['team_order']['sename_l'] = $sename_l;
	$_SESSION['team_order']['muneban_l'] = $muneban_l;
	$_SESSION['team_order']['pant_l'] = $pant_l;
	$_SESSION['team_order']['bach_l'] = $bach_l;

	/*
	//	パンツ＆ソックスセット情報取得
	$p_set = $_POST['p_set'];							//	パンツセット
	$s_set = $_POST['s_set'];							//	ソックスセット

	$_SESSION['team_order']['p_set'] = $p_set;
	$_SESSION['team_order']['s_set'] = $s_set;
*/

	//	ユーザー情報取得
	$name_s = $_POST['name_s'];								//	姓
	#	$name_s = mb_convert_kana($name_s, "asKV", "EUC-JP");
	$name_s = mb_convert_kana($name_s, "asKV", "UTF-8");
	$name_n = $_POST['name_n'];								//	名
	#	$name_n = mb_convert_kana($name_n, "asKV", "EUC-JP");
	$name_n = mb_convert_kana($name_n, "asKV", "UTF-8");
	$kana_s = $_POST['kana_s'];								//	姓：ふりがな
	#	$kana_s = mb_convert_kana($kana_s, "ascHV", "EUC-JP");
	$kana_s = mb_convert_kana($kana_s, "ascHV", "UTF-8");
	$kana_n = $_POST['kana_n'];								//	名：ふりがな
	#	$kana_n = mb_convert_kana($kana_n, "ascHV", "EUC-JP");
	$kana_n = mb_convert_kana($kana_n, "ascHV", "UTF-8");
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
	$msr = $_POST['msr'];									//	ご意見ご要望
	#	$msr = mb_convert_kana($msr, "asKV", "EUC-JP");
	$msr = mb_convert_kana($msr, "asKV", "UTF-8");

	$_SESSION['team_order']['name_s'] = $name_s;
	$_SESSION['team_order']['name_n'] = $name_n;
	$_SESSION['team_order']['kana_s'] = $kana_s;
	$_SESSION['team_order']['kana_n'] = $kana_n;
	$_SESSION['team_order']['zip1'] = $zip1;
	$_SESSION['team_order']['zip2'] = $zip2;
	$_SESSION['team_order']['prf'] = $prf;
	$_SESSION['team_order']['city'] = $city;
	$_SESSION['team_order']['add1'] = $add1;
	$_SESSION['team_order']['add2'] = $add2;
	$_SESSION['team_order']['tel1'] = $tel1;
	$_SESSION['team_order']['tel2'] = $tel2;
	$_SESSION['team_order']['tel3'] = $tel3;
	$_SESSION['team_order']['fax1'] = $fax1;
	$_SESSION['team_order']['fax2'] = $fax2;
	$_SESSION['team_order']['fax3'] = $fax3;
	$_SESSION['team_order']['email'] = $email;
	$_SESSION['team_order']['msr'] = $msr;



	//	チームオーダー各商品の番号＆ネーム情報取得、エラーチェック
	$result = explode("<>", $customer);
	$flag_sn = 0;
	foreach ($result as $val) {

		list($hinban, $title, $kakaku, $buy_n) = explode("::", $val);

		for ($i = 1; $i <= $buy_n; $i++) {

			$hinban_h = preg_replace("/-/", "_", $hinban);
			$names_ban = $hinban_h . "_ban_" . $i;
			$names_name = $hinban_h . "_name_" . $i;

			$check_names_ban = $_POST[$names_ban];
			#			$check_names_ban = mb_convert_kana($check_names_ban, "n", "EUC-JP");
			$check_names_ban = mb_convert_kana($check_names_ban, "n", "UTF-8");
			$check_names_ban = trim($check_names_ban);

			$check_names_name = $_POST[$names_name];
			#			$check_names_name = mb_convert_kana($check_names_name, "asKV", "EUC-JP");
			$check_names_name = mb_convert_kana($check_names_name, "asKV", "UTF-8");
			$check_names_name = trim($check_names_name);

			$_SESSION['team_order'][$names_ban] = $check_names_ban;
			$_SESSION['team_order'][$names_name] = $check_names_name;

			if (preg_match("/[^0-9]/", $check_names_ban)) {
				$ERROR[] = "商品番号：" . $hinban . " の" . $i . "着目の番号が不正です。";
			} elseif ($check_names_ban == "") {
				//$ERROR[] = "商品番号：".$hinban." の".$i."着目の番号が入力されておりません。";
			}
			if ($check_names_ban != "") {
				$flag_sb = 1;
			}

			if (preg_match("/[^A-Za-z0-9,-_.']/", $check_names_name)) {
				$ERROR[] = "商品番号：" . $hinban . " の" . $i . "着目の背ネームが不正です。";
			} elseif ($check_names_name == "") {
				//$ERROR[] = "商品番号：".$hinban." の".$i."着目の背ネームが入力されておりません。";
			}
			if ($check_names_name != "") {
				$flag_sn = 1;
			}
		}
	}

	//	チームオーダーエラーチェック
	if ($flag_sb == 1 && !$seban_l) {
		$ERROR[] = "背番号タイプが選択されておりません。";
	}
	if ($flag_sn == 1 && !$sename_l) {
		$ERROR[] = "背ネームタイプが選択されておりません。";
	}

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
	if ($zip1 == "" || $zip2 == "") {
		$ERROR[] = "郵便番号が入力されておりません。";
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
	if ($tel1 == "" || $tel2 == "" || $tel3 == "") {
		$ERROR[] = "電話番号が入力されておりません。";
	}
	if ($email == "") {
		$ERROR[] = "メールアドレスが入力されておりません。";
	}

	if ($email && !preg_match("/^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)/", $email, $regs)) {
		$ERROR[] = "メールアドレスが不正です。";
	}
}



//	御見積商品確認
function team_order_confirmation($customer)
{

	global $PRF_N, $SEBAN_N, $SENAME_N, $MUNEBAN_N, $PANT_N, $BACH_N;

	$html = "";

	if ($_SESSION['team_order']) {

		$seban_l = $_SESSION['team_order']['seban_l'];
		$sename_l = $_SESSION['team_order']['sename_l'];
		$muneban_l = $_SESSION['team_order']['muneban_l'];
		$pant_l = $_SESSION['team_order']['pant_l'];
		$bach_l = $_SESSION['team_order']['bach_l'];

		//$p_set = $_SESSION['team_order']['p_set'];	//	パンツセット
		//$s_set = $_SESSION['team_order']['s_set'];	//	ソックスセット

		$name_s = $_SESSION['team_order']['name_s'];
		$name_n = $_SESSION['team_order']['name_n'];
		$kana_s = $_SESSION['team_order']['kana_s'];
		$kana_n = $_SESSION['team_order']['kana_n'];
		$zip1 = $_SESSION['team_order']['zip1'];
		$zip2 = $_SESSION['team_order']['zip2'];
		$prf = $_SESSION['team_order']['prf'];
		$city = $_SESSION['team_order']['city'];
		$add1 = $_SESSION['team_order']['add1'];
		$add2 = $_SESSION['team_order']['add2'];
		$tel1 = $_SESSION['team_order']['tel1'];
		$tel2 = $_SESSION['team_order']['tel2'];
		$tel3 = $_SESSION['team_order']['tel3'];
		$fax1 = $_SESSION['team_order']['fax1'];
		$fax2 = $_SESSION['team_order']['fax2'];
		$fax3 = $_SESSION['team_order']['fax3'];
		$email = $_SESSION['team_order']['email'];
		$msr = $_SESSION['team_order']['msr'];
		$msr = nl2br($msr);
	}

	//	入力フォームページ削除
	$DEL_INPUTS['NOTORDER'] = 1;
	$DEL_INPUTS['ENTER'] = 1;
	$DEL_INPUTS['KAKUNIN'] = 1;
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
	$DEL_INPUTS['MSRDEL'] = 1;

	$DEL_INPUTS['TYUUIDEL'] = 1;



	//	チームオーダー詳細表示
	$result = explode("<>", rtrim($customer, "<>"));
	foreach ($result as $val) {
		list($hinban, $title, $kakaku, $buy_n) = explode("::", $val);

		// 商品グループ
		$goods_kakunin_html .= "<div class=\"box-outline primary-color\">\n";
		$goods_kakunin_html .= "	<div class=\"box-grid primary-color box-form no-gap\">\n";


		if ($val) {

			$goods_kakunin_html .= "	<div class=\"team-order-goods\">\n";
			$goods_kakunin_html .= "		<dl>\n";
			$goods_kakunin_html .= "			<span>\n";
			$goods_kakunin_html .= "				<dt>商品名</dt>\n";
			$goods_kakunin_html .= "				<dd>" . $title . "</dd>\n";
			$goods_kakunin_html .= "			</span>\n";
			$goods_kakunin_html .= "			<span>\n";
			$goods_kakunin_html .= "				<dt>商品番号</dt>\n";
			$goods_kakunin_html .= "				<dd>" . $hinban . "</dd>\n";
			$goods_kakunin_html .= "			</span>\n";
			$goods_kakunin_html .= "			<span>\n";
			$goods_kakunin_html .= "				<dt>数量</dt>\n";
			$goods_kakunin_html .= "				<dd>" . $buy_n . "</dd>\n";
			$goods_kakunin_html .= "			</span>\n";
			$goods_kakunin_html .= "		</dl>\n";
			$goods_kakunin_html .= "	</div>\n";
		}

		for ($i = 1; $i <= $buy_n; $i++) {

			$hinban_h = preg_replace("/-/", "_", $hinban);
			$names_ban = $hinban_h . "_ban_" . $i;
			$names_name = $hinban_h . "_name_" . $i;

			$check_names_ban = $_SESSION['team_order'][$names_ban];
			$check_names_name = $_SESSION['team_order'][$names_name];

			if ($seban_l != 0) {
				$se_n = "背番号";
			}
			if ($muneban_l != 0) {
				$mune_n = "胸番号";
			}
			if ($se_n || $mune_n) {
				$number_order = "		" . $se_n . "&nbsp;" . $mune_n . ": [" . $check_names_ban . "]\n";
			}
			if ($sename_l != 0) {
				$name_order = "		背ネーム: [" . $check_names_name . "]\n";
			}

			// $goods_kakunin_html .= "<tr>\n";
			// $goods_kakunin_html .= "	<th class=\"team_order_title\">" . $i . "着目</th>\n";
			// $goods_kakunin_html .= "	<td>\n";
			// $goods_kakunin_html .= $number_order;
			// $goods_kakunin_html .= $name_order;
			// $goods_kakunin_html .= "	</td>\n";
			// $goods_kakunin_html .= "</tr>\n";

			$goods_kakunin_html .= "	<div class=\"team-order-title\">\n";
			$goods_kakunin_html .= "			<span class=\"first-item\">" . $i . "着目</span>\n";
			$goods_kakunin_html .= "		<div class=\"names-confirmation-area\">\n";
			$goods_kakunin_html .= 				$number_order;
			$goods_kakunin_html .= 				$name_order;
			$goods_kakunin_html .= "		</div>\n";
			$goods_kakunin_html .= "	</div>\n";
		}

		$goods_kakunin_html .= "	</div>\n"; // end box-grid
		$goods_kakunin_html .= "</div>\n"; // end box-outline

		// end 商品グループ
	}
	/*
	if($p_set == 1){
		$p_set_check .= "<tr>\n";
		$p_set_check .= "	<th>\n";
		$p_set_check .= "		パンツ\n";
		$p_set_check .= "	</th>\n";
		$p_set_check .= "</tr>\n";
		$p_set_check .= "<tr>\n";
		$p_set_check .= "	<td>\n";
		$p_set_check .= "		パンツセットでお見積り";
		$p_set_check .= "	</td>\n";
		$p_set_check .= "</tr>\n";
	}
	if($s_set == 1){
		$p_set_check .= "<tr>\n";
		$p_set_check .= "	<th>\n";
		$p_set_check .= "		ソックス\n";
		$p_set_check .= "	</th>\n";
		$p_set_check .= "</tr>\n";
		$s_set_check .= "<tr>\n";
		$s_set_check .= "	<td>\n";
		$s_set_check .= "		ソックスセットでお見積り";
		$s_set_check .= "	</td>\n";
		$s_set_check .= "</tr>\n";
	}
*/



	if ($zip1 && $zip2) {
		$zipn = "〒" . $zip1 . "-" . $zip2;
	}
	$INPUTS['ZIPN'] = $zipn;	//	郵便番号表示

	if ($tel1 && $tel2 && $tel3) {
		$teln = $tel1 . "-" . $tel2 . "-" . $tel3;
	}
	$INPUTS['TELN'] = $teln;	//	電話番号表示

	if ($fax1 && $fax2 && $fax3) {
		$faxn = $fax1 . "-" . $fax2 . "-" . $fax3;
	}
	//2022/12/16 追加 uenishi
	else {
		$faxn = "FAX番号：--";
	}
	$INPUTS['FAXN'] = $faxn;	//	FAX番号表示

	$INPUTS['SEBANN'] = $SEBAN_N[$seban_l];			//	背番号タイプ
	$INPUTS['SENAMEN'] = $SENAME_N[$sename_l];		//	背ネームタイプ
	$INPUTS['MUNEBANN'] = $MUNEBAN_N[$muneban_l];	//	胸番号タイプ
	$INPUTS['PANTN'] = $PANT_N[$pant_l];			//	パンツ番号タイプ
	$INPUTS['BACHN'] = $BACH_N[$bach_l];			//	バッヂタイプ

	//$INPUTS['PANTSETN'] = $p_set_check;			//	パンツセット
	//$INPUTS['SOCKSSETN'] = $s_set_check;			//	ソックスセット

	$INPUTS['NAMESN'] = $name_s;					//	姓
	$INPUTS['NAMENN'] = $name_n;					//	名
	$INPUTS['KANASN'] = $kana_s;					//	姓：ふりがな
	$INPUTS['KANANN'] = $kana_n;					//	名：ふりがな
	$INPUTS['PRFN'] = $PRF_N[$prf];					//	都道府県
	$INPUTS['CITYN'] = $city;						//	市区町村名
	$INPUTS['ADD1N'] = $add1;						//	所番地
	$INPUTS['ADD2N'] = $add2;						//	マンション名など
	$INPUTS['EMAILN'] = $email;						//	メールアドレス
	$INPUTS['MSRN'] = $msr;							//	ご意見ご要望

	$INPUTS['ORDERKAKUNIN'] = $html;				//	オーダー内容確認html
	$INPUTS['GOODSKAKUNIN'] = $goods_kakunin_html;	//	各商品入力確認html

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("team.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;
}



//	御見積（メール送信）
function team_order_send($customer, &$ERROR)
{

	if (!is_array($_SESSION['team_order'])) {
		$ERROR[] = "御見積内容が確認できませんでした。";
		return;
	}

	global $PRF_N , $SEBAN_N , $SENAME_N , $MUNEBAN_N , $PANT_N , $BACH_N , $admin_name , $admin_mail_t , $m_footer,
			$conn_id; // mysql DB

	if ($_SESSION['team_order']) {

		$seban_l = $_SESSION['team_order']['seban_l'];
		$sename_l = $_SESSION['team_order']['sename_l'];
		$muneban_l = $_SESSION['team_order']['muneban_l'];
		$pant_l = $_SESSION['team_order']['pant_l'];
		$bach_l = $_SESSION['team_order']['bach_l'];

		//$p_set = $_SESSION['team_order']['p_set'];
		//$s_set = $_SESSION['team_order']['s_set'];

		$name_s = $_SESSION['team_order']['name_s'];
		$name_n = $_SESSION['team_order']['name_n'];
		$kana_s = $_SESSION['team_order']['kana_s'];
		$kana_n = $_SESSION['team_order']['kana_n'];
		$zip1 = $_SESSION['team_order']['zip1'];
		$zip2 = $_SESSION['team_order']['zip2'];
		$prf = $_SESSION['team_order']['prf'];
		$city = $_SESSION['team_order']['city'];
		$add1 = $_SESSION['team_order']['add1'];
		$add2 = $_SESSION['team_order']['add2'];
		$tel1 = $_SESSION['team_order']['tel1'];
		$tel2 = $_SESSION['team_order']['tel2'];
		$tel3 = $_SESSION['team_order']['tel3'];
		$fax1 = $_SESSION['team_order']['fax1'];
		$fax2 = $_SESSION['team_order']['fax2'];
		$fax3 = $_SESSION['team_order']['fax3'];
		$email = $_SESSION['team_order']['email'];
		$msr = $_SESSION['team_order']['msr'];
	}



	$result = explode("<>", $customer);
	foreach ($result as $val) {

		list($hinban, $title, $kakaku, $buy_n) = explode("::", $val);

		if ($val) {

			$msg_g .= "****************************************************\n";
			$msg_g .= "商品番号：" . $hinban . " \n";
			$msg_g .= "商 品 名：" . $title . " \n";
			$msg_g .= "数	量：" . $buy_n . " \n";
			$msg_g .= "----------------------------------------------------\n";
		}

		for ($i = 1; $i <= $buy_n; $i++) {

			$hinban_h = preg_replace("/-/", "_", $hinban);	//	add yoshizawa 2014/01/08
			$names_ban = $hinban_h . "_ban_" . $i;				//	add yoshizawa 2014/01/08
			$names_name = $hinban_h . "_name_" . $i;			//	add yoshizawa 2014/01/08

			$check_names_ban = $_SESSION['team_order'][$names_ban];
			$check_names_name = $_SESSION['team_order'][$names_name];

			if ($seban_l != 0) {
				$se_n = "背番号";
			}
			if ($muneban_l != 0) {
				$mune_n = "胸番号";
			}
			if ($se_n || $mune_n) {
				$number_order = "		" . $se_n . $mune_n . ": [" . $check_names_ban . "]\n";
			}
			if ($sename_l != 0) {
				$name_order = "		背ネーム: [" . $check_names_name . "]\n";
			}

			$msg_g .= $i . "着目\n";
			$msg_g .= $number_order;
			$msg_g .= $name_order;
			$msg_g .= "\n";
		}
	}



	//	マーキングタイプを変数に代入
	$msg_t = "";
	if ($seban_l) {
		$msg_t .= "背番号タイプ：" . $SEBAN_N[$seban_l] . " \n";
	}
	if ($sename_l) {
		$msg_t .= "背ネームタイプ：" . $SENAME_N[$sename_l] . " \n";
	}
	if ($muneban_l) {
		$msg_t .= "胸番号タイプ：" . $MUNEBAN_N[$muneban_l] . " \n";
	}
	if ($pant_l) {
		$msg_t .= "パンツ番号タイプ：" . $PANT_N[$pant_l] . " \n";
	}
	if ($bach_l) {
		$msg_t .= "バッジタイプ：" . $BACH_N[$bach_l] . " \n";
	}



	//	マーキングタイプ表示テキスト構築
	if ($msg_t) {
		$msg_t_text .= "マーキングタイプ\n";
		$msg_t_text .= $msg_t;
	}


	/*
	if ($p_set == 1) {
		$set .= "****************************************************\n";
		$set .= "パンツセットでお見積もり \n";
	}
	if ($s_set == 1) {
		$set .= "****************************************************\n";
		$set .= "ソックスセットでお見積もり \n";
	}
*/



	//	メール作業
	list($user_email, $pass) = explode("<>", $_SESSION['idpass']);

	if ($user_email) {

		$sql  = "SELECT kojin_num FROM " . T_KOJIN .
			" WHERE email='" . $user_email . "'" .
			" AND pass='" . $pass . "'" .
			" AND saku!='1'" .
			" AND kojin_num<'100000'" .
			" ORDER BY kojin_num;";

		$sql1 = mysqli_query($conn_id, $sql);		//	SQLの実行
		$check = mysqli_num_rows($sql1);	//	mysqli_num_rows 行数を返す

		if ($check <= 0) {
			$idpass = "";
		} else {
			$list = mysqli_fetch_array($sql1);	//	mysqli_fetch_array 行を配列として取得する
			$kojin_num = $list['kojin_num'];
			$men = " ( No." . $kojin_num . " )";
		}
	} else {
		$men = "(ゲストさん)";
	}



	//	ホスト名を取得
	$ip = getenv("REMOTE_ADDR");	//	getenv 環境変数の値を取得する
	$host = gethostbyaddr($ip);		//	gethostbyaddr IPアドレスからホスト名を取得する
	if (!$host) {
		$host = $ip;
	}

	$team_num = date("ymdHis");		//	日付の取得

	//	店舗用確認メール送信
	$subject = "御見積番号[ " . $team_num . " ] " . $name_s . " " . $name_n . " 様" . $men . " からの御見積依頼です。";

	$msg = <<<EOT
{$subject}
########################################################

御見積内容

{$msg_t}
{$msg_g}
{$set}
------------------------------------------------------
連絡先

お名前： {$name_s} {$name_n}

ふりがな： {$kana_s} {$kana_n}

住所
〒{$zip1} - {$zip2}
{$PRF_N[$prf]} {$city}
{$add1} {$add2}

電話番号： {$tel1} - {$tel2} - {$tel3}

FAX番号： {$fax1} - {$fax2} - {$fax3}

E-mail： {$email}

------------------------------------------------------
ご意見ご要望など
{$msr}

------------------------------------------------------
{$host} ({$ip})
EOT;
	//echo('店舗宛て=>\n'.$msg.'<br />\n');

	$send_email = $email;
	$send_name = $name_s . $name_n;
	$get_email = $admin_mail_t;
	//$get_email = "検証アドレス";
	send_email($send_email, $send_name, $mail_bcc, $bcc_email, $get_email, $subject, $msg);





	//	お客様に確認メール送信
	$subject = "御見積依頼ありがとうございます。  - ネイバーズスポーツ -";

	$msg = <<<EOT
{$subject}
{$name_s} {$name_n}様 御見積内容は以下でよろしいでしょうか？
もし間違いがある場合は、お手数ですがご連絡お願いします。
########################################################

御見積内容

{$msg_t}
{$msg_g}
{$set}
------------------------------------------------------
連絡先

お名前： {$name_s} {$name_n}

ふりがな： {$kana_s} {$kana_n}

住所
〒{$zip1} - {$zip2}
{$PRF_N[$prf]} {$city}
{$add1} {$add2}

電話番号： {$tel1} - {$tel2} - {$tel3}

FAX番号： {$fax1} - {$fax2} - {$fax3}

E-mail： {$email}

------------------------------------------------------
ご意見ご要望など
{$msr}

------------------------------------------------------

{$m_footer}
EOT;
	//echo('お客宛て=>\n'.$msg.'<br />\n');

	$send_email = $admin_mail_t;
	$send_name = $admin_name;
	$get_email = $email;
	//$get_email = "検証アドレス";
	send_email($send_email, $send_name, $mail_bcc, $bcc_email, $get_email, $subject, $msg);



	//	セッション解除
	unset($_SESSION['team_order']);
	unset($_SESSION['customer']);

	header("Location: ./template/thank.htm");
	exit();
}
