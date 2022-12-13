<?PHP
/*

	■アフィリエイトコントロールプログラム

	$member_table→kojinテーブル名
	afuser_table→アフィリエイトユーザー情報
	appoint_table→取得ポイント、アフィリエイト商品売買に関する情報
	application_table→アフィリエイトポイント変換に関する情報
	afrefere_table→アフィリエイト商品のクリック情報
	bank_table→ユーザーの振込先銀行情報

*/

//--------------------------------------//
//		アフィリエイト初期ページ		//
//--------------------------------------//
function nonmember_html(){
	//------------------//
	//	非会員初期ページ //
	//------------------//
	global	$aff_ritsu;	//	アフィリエイトポイント変換率

	$title = "ネイバーズスポーツ アフィリエイト参加登録(Affiliate)";

		$INPUTS['PERCENT'] = $aff_ritsu;		//	アフィリエイトポイント率

		//	html作成・置換
		$make_html = new read_html();
		$make_html->set_dir(TEMPLATE_DIR);
		$make_html->set_file("normal.htm");
		$make_html->set_rep_cmd($INPUTS);
		$make_html->set_del_cmd($DEL_INPUTS);
		$html = $make_html->replace();

		return array($title,$html);

}
function afmember_html($af_num){
	//-------------------------------//
	//	アフィリエイト会員初期ページ //
	//-------------------------------//
	global	$aff_ritsu,			//	アフィリエイトポイント変換率
			$afuser_table,		//	afuserテーブル名
			$appoint_table,		//	appointテーブル名
			$application_table,	//	applicationテーブル名
			$set_point_mon,		//	アフェリエイトポイント確定時期	2013/12/06現在	1ヶ月
			$change_point;		//	アフェリエイト返金可能ポイント	2013/12/06現在	5000

	$title = "ネイバーズスポーツ アフィリエイト参加登録(Affiliate)";

		$sql =  "SELECT SUM(point) AS all_point FROM $appoint_table" .
				" WHERE af_num='$af_num';";
//echo($sql."<br>");
		if ($result = pg_query(DB,$sql)) {
			$list = pg_fetch_array($result);
			//	(int)→型キャスト：変数の値を整数に変換する
			(int)$all_point = $list["all_point"];
		}

		//	獲得済みアフィリエイトポイント
		$order_time = mktime(0,0,0,date("m")-$set_point_mon,1,date("Y"));
		$order_day = date("Y-m-d",$order_time);
		$sql =  "SELECT SUM(point) AS dec_point FROM $appoint_table" .
				" WHERE af_num='$af_num' AND state='1'" .
				" AND send_day<'$order_day';";
			//	" WHERE af_num='$af_num' AND state='1' AND send_day<'$order_day';";
//echo($sql."<br>");
		if ($result = pg_query(DB,$sql)) {
			$list = pg_fetch_array($result);
			(int)$dec_point = $list["dec_point"];
		}

		//	変換（支払orポイント）済みポイント情報
		$sql =  "SELECT SUM(af_point) AS pay_point FROM $application_table" .
				" WHERE af_num='$af_num' AND state!='2';";
//echo($sql."<br>");
		if ($result = pg_query(DB,$sql)) {
			$list = pg_fetch_array($result);
			(int)$pay_point = $list["pay_point"];
		}

		//	確定アフィリエイトポイント
		$point = $dec_point - $pay_point;
		if ($point < 1) {
			$point = 0;
		}
		$point_ = number_format($point);

		//	獲得予定ポイント
		$indent_point = $all_point - $dec_point;
		$indent_point_ = number_format($indent_point);

		//	アフェリエイト返金可能ポイント
		$change_point_ = number_format($change_point);

		if ($point >= $change_point) {
			$DEL_INPUTS['NOTCONVERT'] = 1;	//	ポイント変換可能
		} else {
			$DEL_INPUTS['CONVERT'] = 1;		//	ポイント変換不可
		}

		$INPUTS['AFPOINT'] = $point_;				//	確定アフィリエイトポイント
		$INPUTS['INDENTPOINT'] = $indent_point_;	//	獲得予定アフィリエイトポイント
		$INPUTS['CHANGEPOINT'] = $change_point_;	//	アフェリエイト返金可能ポイント

		//	html作成・置換
		$make_html = new read_html();
		$make_html->set_dir(TEMPLATE_DIR);
		$make_html->set_file("normal_am.htm");
		$make_html->set_rep_cmd($INPUTS);
		$make_html->set_del_cmd($DEL_INPUTS);
		$html = $make_html->replace();

		return array($title,$html);

}
function member_html(){
	//-------------------------------------------//
	//	会員だが非アフィリエイト会員の初期ページ //
	//-------------------------------------------//
	global	$aff_ritsu;			//	アフィリエイトポイント変換率

	$title = "ネイバーズスポーツ アフィリエイト参加登録(Affiliate)";

		$INPUTS['PERCENT'] = $aff_ritsu;		//	アフィリエイトポイント率

		//	html作成・置換
		$make_html = new read_html();
		$make_html->set_dir(TEMPLATE_DIR);
		$make_html->set_file("normal_m.htm");
		$make_html->set_rep_cmd($INPUTS);
		$make_html->set_del_cmd($DEL_INPUTS);
		$html = $make_html->replace();

		return array($title,$html);

}

//-----------//
//	規約表示 //
//-----------//
function kiyaku() {

	$title = "ネイバーズスポーツアフィリエイト(Affiliate)規約";

	//	規約読み込み
	$k_file = "./include/kiyaku.txt";
	if (file_exists($k_file) && filesize($k_file) > 0) {
		$LIST = file($k_file);
		if ($LIST) {
			foreach ($LIST AS $VAL) {
				$kiyaku .= $VAL;
			}
		}
	}

	$DEL_INPUTS['KIYAKUDOUI'] = 1;		//	規約同意を削除

	$INPUTS['KIYAKUTEXT'] = $kiyaku;	//	規約文章

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("kiyaku.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return array($title,$html);

}
//-----------------------------------------//
//	アフィリエイト会員登録＆確認メール送信 //
//-----------------------------------------//
function af_regist(&$ERROR) {

	global	$PHP_SELF,		//	現在実行しているスクリプトのファイル名
			$afuser_table,	//	afuserテーブル名
			$member_table,	//	kojinテーブル名
			$m_footer,		//	メールフッター
			$admin_mail_a,	//	'affiliate@futboljersey.com'
			$admin_name;	//	'NEIGHBOURS SPORTS'

	$doui = $_POST["doui"];
	if (!$doui) {
		$ERROR[] = "規約に同意されておりません。";
	}

	//	ユーザー確認
	if (!$ERROR) {
		list($email,$pass,$check,$af_num) = explode("<>",$_SESSION['idpass']);
		$sql  = "SELECT kojin_num, name_s, name_n FROM $member_table" .
				" WHERE email='$email' AND saku!='1' AND kojin_num<'100000' LIMIT 1;";
		if ($result = pg_query(DB,$sql)) {
			$list = pg_fetch_array($result);
			$kojin_num = $list["kojin_num"];
			$name_s = $list["name_s"];
			$name_n = $list["name_n"];
			$name = "$name_s $name_n";
		}
		if ($kojin_num < 1) { $ERROR[] = "会員情報が確認出来ません。"; }
	}

	//	登録処理
	if (!$ERROR) {
		$sql  = "INSERT INTO $afuser_table" .
				" (kojin_num,regist)" .
				" VALUES('$kojin_num',now());";
		if (!$result = pg_query(DB,$sql)) {
			$ERROR[] = "アフィリエイト登録出来ませんでした。";
		} else {
			$sql  = "SELECT af_num FROM $afuser_table WHERE kojin_num='$kojin_num' AND state!='1' LIMIT 1;";
			if ($result = pg_query(DB,$sql)) {
				$list = pg_fetch_array($result);
				$af_num = $list["af_num"];
			}
		}
	}

	//	エラーがあったら処理を中断
	if($ERROR){
		return;
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

		$msr = <<<EOT
{$name}様がアフィリエイト会員登録して頂きました。

名前：{$name}
会員番号：{$kojin_num}
ID：{$af_num}
E-mail：{$email}
------------------------------------------------------
$host ($ip)

EOT;

		//	送信処理
		$send_email = $email;
		$send_name = $name;
		$get_email = $admin_mail_a;
		//$get_email = "検証アドレス";
		send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msr);

		//	会員
		//	件名
		$subject = "ネイバーズスポーツアフィリエイト会員登録完了";

		//	メッセージ
		$msr = <<<EOT
{$name}様、この度はネイバーズスポーツアフィリエイト会員登録して頂き有り難うございます。
ネイバーズスポーツアフィリエイト会員登録処理が完了致しました。

{$name}様のアフィリエイトの会員IDは、「{$af_num}」となります。
リンク作成時間違いない様にお願い致します。

それではこれからもよろしくお願い致します。

$m_footer

EOT;

		//	送信処理
		$send_email = $admin_mail_a;
		$send_name = $admin_name;
		$get_email = $email;
		//$get_email = "検証アドレス";
		send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msr);

	}

	//	エラーリセット
	if ($ERROR) {
		$ERROR = array();
	}

	header ("Location: ./template/thank_af_member.htm");
	exit;

}

//----------------------------//
// アフィリエイト規約同意画面 //
//----------------------------//
function re_defaults_html($ERROR) {

	global	$PHP_SELF;	//	現在実行しているスクリプトのファイル名

	$title = "ネイバーズスポーツ アフィリエイト参加登録(Affiliate)";

	//	すでに登録済みなら処理中止
	$check = regist_check();
	if ($check) {
		$html .= "<br />\n";
		$html .= "すでに登録されているか、登録出来ません。<br />\n";
		$html .= "<br />\n";
		$html .= "<form action=\"".$PHP_SELF."\" method=\"POST\">\n";
		$html .= "	<input type=\"submit\" value=\"アフィリエイトTOPに戻る\">\n";
		$html .= "</form>\n";
		return array($title,$html);
	}

	//	エラーチェック
	if($ERROR){
		$errors = error_html($ERROR);
	}

	//	規約読み込み
	$k_file = "./include/kiyaku.txt";
	if (file_exists($k_file) && filesize($k_file) > 0) {
		$LIST = file($k_file);
		if ($LIST) {
			foreach ($LIST AS $VAL) {
				$kiyaku .= $VAL;
			}
		}
	}

	$DEL_INPUTS['KIYAKU'] = 1;			//	通常の規約HTMLを削除

	$INPUTS['ERRORS'] = $errors;		//	エラーメッセージ
	$INPUTS['KIYAKUTEXT'] = $kiyaku;	//	規約テキスト

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("kiyaku.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return array($title,$html);

}
//----------------------//
// 登録済みかどうか確認 //
//----------------------//
function regist_check() {

	global	$member_table,	//	kojinテーブル名
			$afuser_table;	//	afuserテーブル名

	if ($_SESSION['idpass']) {
		$idpass = $_SESSION['idpass'];
	} elseif ($_COOKIE['idpass']) {
		$idpass = $_COOKIE['idpass'];
	}
	list($email,$pass,$check,$af_num)  = explode("<>",$idpass);

	if (!$af_num) {
		$sql  = "SELECT af.af_num FROM $member_table a, $afuser_table af" .
				" WHERE a.kojin_num=af.kojin_num AND a.email='$email';";
		if ($result = pg_query(DB,$sql)) {
			$af_num = pg_numrows($result);
		}
	}

	return $af_num;

}

//---------------------------------//
//	ポイント詳細ページ（年毎一覧） //
//---------------------------------//
function pc_defaults_html($af_num){

	global	$PHP_SELF,	//	現在実行しているスクリプトのファイル名
			$afrefere_table,	//	afrefereテーブル名
			$afuser_table,		//	afuserテーブル名
			$appoint_table,		//	appointテーブル名
			$application_table,	//	applicationテーブル名
			$set_point_mon;		//	アフェリエイトポイント確定時期	2013/12/06現在	1ヶ月

	$title = "ネイバーズスポーツ アフィリエイトポイント詳細";

	//	本年度
	$first_year = $last_year = date("Y");

	//	クリック数
	$sql  = "SELECT date_part('year',click_time) AS year, COUNT(*) AS click FROM $afrefere_table" .	//	update yoshizawa SUM(*)→COUNT(*) 2013/12/10
			" WHERE af_num='$af_num'" .
			" GROUP BY date_part('year',click_time);";
//echo($sql);
	if ($result = pg_query(DB,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$year = $list['year'];
			$click = $list['click'];
			$CLICK[$year] = $click;
		}
	}

	//	売上件数
	$sql =  "SELECT date_part('year',order_day) AS year, COUNT(*) AS count FROM $appoint_table" .
			" WHERE af_num='$af_num'" .
			" GROUP BY date_part('year',order_day);";
//echo($sql);
	if ($result = pg_query(DB,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$year = $list['year'];
			$count = $list['count'];
			$COUNT[$year] = $count;
		}
	}

	//	獲得済み＆獲得予定アフィリエイト総ポイント
	$sql =  "SELECT date_part('year',order_day) AS year, SUM(point) AS all_point FROM $appoint_table" .
			" WHERE af_num='$af_num' GROUP BY date_part('year',order_day);";
//echo($sql);
	if ($result = pg_query(DB,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$year = $list['year'];
			$all_point = $list['all_point'];
			$ALL[$year] = $all_point;
			//	登録初年度を代入
			if ($first_year > $year) { $first_year = $year; }
		}
	}

	//	獲得済みアフィリエイトポイント
	$order_time = mktime(0,0,0,date("m")-$set_point_mon,1,date("Y"));
	$order_day = date("Y-m-d",$order_time);
	$sql =  "SELECT date_part('year',order_day) AS year, SUM(point) AS dec_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND state='1'" .
			" AND send_day<'$order_day'" .
			" GROUP BY date_part('year',order_day);";
//echo($sql);
	if ($result = pg_query(DB,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$year = $list['year'];
			$dec_point = $list['dec_point'];
			$DEC[$year] = $dec_point;
		}
	}

	//	変換（支払orポイント）ポイント情報
	$sql =  "SELECT date_part('year',appli_day) AS year, SUM(af_point) AS pay_point FROM $application_table" .
			" WHERE af_num='$af_num' AND state!='2' GROUP BY date_part('year',appli_day);";
//echo($sql);
	if ($result = pg_query(DB,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$year = $list['year'];
			$pay_point = $list['pay_point'];
			$PAY[$year] = $pay_point;
		}
	}

	for($year=$last_year; $year>=$first_year; $year--) {
		//	クリック数
		$click = $CLICK[$year];
		if ($click < 1) { $click = 0; }

		//	売上件数
		$count = $COUNT[$year];
		if ($count < 1) { $count = 0; }

		//	獲得確定ポイント
		$point = $DEC[$year];
		if ($point < 1) { $point = 0; }

		//	獲得予定ポイント
		$indent_point = $ALL[$year] - $DEC[$year];

		//	変換ポイント
		$pay_point = $PAY[$year];

		//	有効ポイント	2013/12/19現在	未使用
//		$now_point = $point - $pay_point;
//		if ($now_point < 1) { $now_point = 0; }

		$click_ = number_format($click);
		$count_ = number_format($count);
		$point_ = number_format($point);
		$indent_point_ = number_format($indent_point);
		$pay_point_ = number_format($pay_point);
//		$now_point_ = number_format($now_point);

		$a_click += $click;
		$a_count += $count;
		$a_indent_point += $indent_point;
		$a_point += $point;
		$a_pay_point += $pay_point;
//		$a_now_point += $now_point;

		$html .= "	<div class=\"box-row\">\n";
		$html .= "		<div class=\"aff-point-content\" data-label=\"年\"><input type=\"submit\" class=\"btn-standard\" name=\"year\" value=\"".$year."\">年</div>\n";
		$html .= "		<div class=\"aff-point-content\" data-label=\"クリック数\">".$click_."件</div>\n";
		$html .= "		<div class=\"aff-point-content\" data-label=\"売上件数\">".$count_."件</div>\n";
		$html .= "		<div class=\"aff-point-content\" data-label=\"予定ポイント\">".$indent_point_."pt</div>\n";
		$html .= "		<div class=\"aff-point-content\" data-label=\"確定ポイント\">".$point_."pt</div>\n";
		$html .= "		<div class=\"aff-point-content\" data-label=\"変換ポイント\">".$pay_point_."pt</div>\n";
		$html .= "	</div>\n";
	}

	$a_click = number_format($a_click);
	$a_count = number_format($a_count);
	$a_indent_point = number_format($a_indent_point);
	$a_point = number_format($a_point);
	$a_pay_point = number_format($a_pay_point);
//	$a_now_point = number_format($a_now_point);

	$DEL_INPUTS['MONHIDDEN'] = 1;				//	月毎のhidden削除
	$DEL_INPUTS['PTRANSITIONS'] = 1;			//	月or日のページ遷移ボタン削除
	$DEL_INPUTS['PBACK'] = 1;					//	月or日の一覧に戻るボタン削除

	$INPUTS['SORT'] = "(年毎)";					//	ページタイトル
	$INPUTS['THDATE'] = "年";					//	テーブルタイトル
	$INPUTS['POINTLIST'] = $html;				//	詳細一覧

	$INPUTS['ACLICK'] = $a_click;				//	クリック数合計
	$INPUTS['ACOUNT'] = $a_count;				//	売上件数合計
	$INPUTS['AINDENTPOINT'] = $a_indent_point;	//	予定ポイント合計
	$INPUTS['APOINT'] = $a_point;				//	確定ポイント合計
	$INPUTS['APAYPOINT'] = $a_pay_point;		//	変換ポイント合計

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("p_check.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return array($title,$html);

}
//---------------------------------//
//	ポイント詳細ページ（月毎一覧） //
//---------------------------------//
function pc_mon_html($af_num){

	global	$PHP_SELF,			//	現在実行しているスクリプトのファイル名
			$afrefere_table,	//	afrefereテーブル名
			$afuser_table,		//	afuserテーブル名
			$appoint_table,		//	appointテーブル名
			$application_table,	//	applicationテーブル名
			$set_point_mon;		//	アフェリエイトポイント確定時期	2013/12/06現在	1ヶ月

	$title = "ネイバーズスポーツ アフィリエイトポイント詳細";

	$year = $_POST['year'];

	//	クリック数
	$sql  = "SELECT date_part('mon',click_time) AS mon, COUNT(*) AS click FROM $afrefere_table" .	//	update yoshizawa SUM(*)→COUNT(*) 2013/12/10
			" WHERE af_num='$af_num' AND date_part('year',click_time)='$year'" .
			" GROUP BY date_part('mon',click_time);";
	if ($result = pg_query(DB,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$mon = $list['mon'];
			$click = $list['click'];
			$CLICK[$mon] = $click;
		}
	}

	//	売上件数
	$sql =  "SELECT date_part('mon',order_day) AS mon, COUNT(*) AS count FROM $appoint_table" .
			" WHERE af_num='$af_num' AND date_part('year',order_day)='$year'" .
			" GROUP BY date_part('mon',order_day);";
	if ($result = pg_query(DB,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$mon = $list['mon'];
			$count = $list['count'];
			$COUNT[$mon] = $count;
		}
	}

	//	獲得済み＆獲得予定アフィリエイト総ポイント
	$sql =  "SELECT date_part('mon',order_day) AS mon, SUM(point) AS all_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND date_part('year',order_day)='$year'" .
			" GROUP BY date_part('mon',order_day);";
	if ($result = pg_query(DB,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$mon = $list['mon'];
			$all_point = $list['all_point'];

			$ALL[$mon] = $all_point;
		}
	}

	//	獲得済みアフィリエイトポイント
	$order_time = mktime(0,0,0,date("m")-$set_point_mon,1,date("Y"));
	$order_day = date("Y-m-d",$order_time);
	$sql =  "SELECT date_part('mon',order_day) AS mon, SUM(point) AS dec_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND state='1' AND date_part('year',order_day)='$year'" .
			" AND send_day<'$order_day'" .
			" GROUP BY date_part('mon',order_day);";
	if ($result = pg_query(DB,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$mon = $list['mon'];
			$dec_point = $list['dec_point'];
			$DEC[$mon] = $dec_point;
		}
	}

	//	変換（支払orポイント）ポイント情報
	$sql =  "SELECT date_part('mon',appli_day) AS mon, SUM(af_point) AS pay_point FROM $application_table" .
			" WHERE af_num='$af_num' AND state!='2' AND date_part('year',appli_day)='$year'" .
			" GROUP BY date_part('mon',appli_day);";
	if ($result = pg_query(DB,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$mon = $list['mon'];
			$pay_point = $list['pay_point'];
			$PAY[$mon] = $pay_point;
		}
	}

	//	初期年度
	$sql =  "SELECT MIN(date_part('year',order_day)) AS first_year FROM $appoint_table" .
			" WHERE af_num='$af_num';";
	if ($result = pg_query(DB,$sql)) {
		$list = pg_fetch_array($result);
		//	登録初年度を代入
		$first_year = $list['first_year'];
	}
	//	月毎一覧

	$year = $_POST['year'];

	if ($first_year < $year) {
	    $back_year = $year - 1;
		$first .= "		<form action=\"".$PHP_SELF."\" method=\"POST\">\n";
		$first .= "			<input type=\"hidden\" name=\"mode\" value=\"p_check\">\n";
		$first .= "			<input type=\"hidden\" name=\"action\" value=\"mon\">\n";
		$first .= "			<input type=\"hidden\" name=\"year\" value=\"".$back_year."\">\n";
		$first .= "			<input type=\"submit\" value=\"&lt;&lt;\">\n";
		$first .= "		</form>\n";
	}

	$now_year = date("Y");
	if ($now_year > $year) {
		$next_year = $year + 1;
		$next .= "		<form action=\"".$PHP_SELF."\" method=\"POST\">\n";
		$next .= "			<input type=\"hidden\" name=\"mode\" value=\"p_check\">\n";
		$next .= "			<input type=\"hidden\" name=\"action\" value=\"mon\">\n";
		$next .= "			<input type=\"hidden\" name=\"year\" value=\"".$next_year."\">\n";
		$next .= "			<input type=\"submit\" value=\"&gt;&gt;\">\n";
		$next .= "		</form>\n";
	}

	for($mon=1; $mon<=12; $mon++) {
		//	クリック数
		$click = $CLICK[$mon];
		if ($click < 1) { $click = 0; }

		//	売上件数
		$count = $COUNT[$mon];
		if ($count < 1) { $count = 0; }

		//	獲得確定ポイント
		$point = $DEC[$mon];
		if ($point < 1) { $point = 0; }

		//	獲得予定ポイント
		$indent_point = $ALL[$mon] - $DEC[$mon];

		//	変換ポイント
		$pay_point = $PAY[$mon];

		//	有効ポイント	2013/12/19現在	未使用
//		$now_point = $point - $pay_point;
//		if ($now_point < 1) { $now_point = 0; }

		$click_ = number_format($click);
		$count_ = number_format($count);
		$point_ = number_format($point);
		$indent_point_ = number_format($indent_point);
		$pay_point_ = number_format($pay_point);
//		$now_point_ = number_format($now_point);

		$a_click += $click;
		$a_count += $count;
		$a_indent_point += $indent_point;
		$a_point += $point;
		$a_pay_point += $pay_point;
//		$a_now_point += $now_point;

		$mon = sprintf('%02d',$mon);

		$html .= "<tr>\n";
		$html .= "	<td class=\"af_align\" data-label=\"月\" ><input type=\"submit\" class=\"btn-standard\" name=\"mon\" value=\"".$mon."\">月</td>\n";
		$html .= "	<td data-label=\"クリック数\">".$click_."件</td>\n";
		$html .= "	<td data-label=\"売上件数\">".$count_."件</td>\n";
		$html .= "	<td data-label=\"予定ポイント\">".$indent_point_."pt</td>\n";
		$html .= "	<td data-label=\"確定ポイント\">".$point_."pt</td>\n";
		$html .= "	<td data-label=\"変換ポイント\">".$pay_point_."pt</td>\n";
		$html .= "</tr>\n";
	}

	$a_click = number_format($a_click);
	$a_count = number_format($a_count);
	$a_indent_point = number_format($a_indent_point);
	$a_point = number_format($a_point);
	$a_pay_point = number_format($a_pay_point);
//	$a_now_point = number_format($a_now_point);

	$DEL_INPUTS['YEARHIDDEN'] = 1;				//	年毎のhidden削除
	$DEL_INPUTS['YBACK'] = 1;					//	「○○年の月一覧に戻る」

	$INPUTS['SORT'] = "(月毎)";					//	ページタイトル
	$INPUTS['FIRST'] = $first;					//	前月へページ遷移「<<」
	$INPUTS['YEAR'] = $year;					//	「ポイント獲得履歴 "○○"年」値
	$INPUTS['DATE'] = "年";						//	「ポイント獲得履歴 ○○"年"」単位
	$INPUTS['NEXT'] = $next;					//	次月ページ遷移「>>」
	$INPUTS['THDATE'] = "月";					//	テーブルタイトル
	$INPUTS['POINTLIST'] = $html;				//	詳細一覧

	$INPUTS['ACLICK'] = $a_click;				//	クリック数合計
	$INPUTS['ACOUNT'] = $a_count;				//	売上件数合計
	$INPUTS['AINDENTPOINT'] = $a_indent_point;	//	予定ポイント合計
	$INPUTS['APOINT'] = $a_point;				//	確定ポイント合計
	$INPUTS['APAYPOINT'] = $a_pay_point;		//	変換ポイント合計

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("p_check.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return array($title,$html);

}
//---------------------------------//
//	ポイント詳細ページ（日毎一覧） //
//---------------------------------//
function pc_day_html($af_num){

	global	$PHP_SELF,			//	現在実行しているスクリプトのファイル名
			$afrefere_table,	//	afrefereテーブル名
			$afuser_table,		//	afuserテーブル名
			$appoint_table,		//	appointテーブル名
			$application_table,	//	applicationテーブル名
			$set_point_mon;		//	アフェリエイトポイント確定時期	2013/12/06現在	1ヶ月

	$title = "ネイバーズスポーツ アフィリエイトポイント詳細";

	$year = $_POST['year'];
	$mon = $_POST['mon'];

	//	タイムスタンプを取得
	$dis_time = mktime(0,0,0,$mon,1,$year);

	//	クリック数
	$sql  = "SELECT date_part('day',click_time) AS day, COUNT(*) AS click FROM $afrefere_table" .	//	update yoshizawa SUM(*)→COUNT(*) 2013/12/10
			" WHERE af_num='$af_num' AND date_part('year',click_time)='$year' AND date_part('mon',click_time)='$mon'" .
			" GROUP BY date_part('day',click_time);";
	if ($result = pg_query(DB,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$day = $list['day'];
			$click = $list['click'];
			$CLICK[$day] = $click;
		}
	}

	//	売上件数
	$sql =  "SELECT date_part('day',order_day) AS day, COUNT(*) AS count FROM $appoint_table" .
			" WHERE af_num='$af_num' AND date_part('year',order_day)='$year' AND date_part('mon',order_day)='$mon'" .
			" GROUP BY date_part('day',order_day);";
	if ($result = pg_query(DB,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$day = $list['day'];
			$count = $list['count'];
			$COUNT[$day] = $count;
		}
	}

	//	獲得済み＆獲得予定アフィリエイト総ポイント
	$sql =  "SELECT date_part('day',order_day) AS day, SUM(point) AS all_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND date_part('year',order_day)='$year' AND date_part('mon',order_day)='$mon'" .
			" GROUP BY date_part('day',order_day);";
	if ($result = pg_query(DB,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$day = $list['day'];
			$all_point = $list['all_point'];
			$ALL[$day] = $all_point;
		}
	}

	//	獲得済みアフィリエイトポイント
	$order_time = mktime(0,0,0,date("m")-$set_point_mon,1,date("Y"));
	$order_day = date("Y-m-d",$order_time);
	$sql =  "SELECT date_part('day',order_day) AS day, SUM(point) AS dec_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND state='1'" .
			" AND date_part('year',order_day)='$year' AND date_part('mon',order_day)='$mon'" .
			" AND send_day<'$order_day'" .
			" GROUP BY date_part('day',order_day);";
	if ($result = pg_query(DB,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$day = $list['day'];
			$dec_point = $list['dec_point'];
			$DEC[$day] = $dec_point;
		}
	}

	//	変換（支払orポイント）ポイント情報
	$sql =  "SELECT date_part('day',appli_day) AS day, SUM(af_point) AS pay_point FROM $application_table" .
			" WHERE af_num='$af_num' AND state!='2'" .
			" AND date_part('year',appli_day)='$year' AND date_part('mon',appli_day)='$mon'" .
			" GROUP BY date_part('day',appli_day);";
	if ($result = pg_query(DB,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$day = $list['day'];
			$pay_point = $list['pay_point'];
			$PAY[$day] = $pay_point;
		}
	}

	//	初期年度
	$sql =  "SELECT MIN(order_day) AS first_day FROM $appoint_table" .
			" WHERE af_num='$af_num';";
	if ($result = pg_query(DB,$sql)) {
		$list = pg_fetch_array($result);
		$first_day = $list['first_day'];
		list($f_year,$f_mon,$f_day) = explode("-",$first_day);
		//	登録初年度のタイムスタンプを代入
		$first_time = mktime(0,0,0,$f_mon,1,$f_year);
	}

	$year = $_POST['year'];
	$mon = $_POST['mon'];

	//	タイムスタンプを取得
	$dis_time = mktime(0,0,0,$mon,1,$year);

	if ($first_time < $dis_time) {
		$back_mon = $mon - 1;
		if ($back_mon < 1) {
			$back_mon = 12;
			$back_year = $year - 1;
		} else {
			$back_year = $year;
		}
		$first .= "	<form action=\"".$PHP_SELF."\" method=\"POST\">\n";
		$first .= "		<input type=\"hidden\" name=\"mode\" value=\"p_check\">\n";
		$first .= "		<input type=\"hidden\" name=\"action\" value=\"day\">\n";
		$first .= "		<input type=\"hidden\" name=\"year\" value=\"".$back_year."\">\n";
		$first .= "		<input type=\"hidden\" name=\"mon\" value=\"".$back_mon."\">\n";
		$first .= "		<input type=\"submit\" value=\"&lt;&lt;\">\n";
		$first .= "	</form>\n";
	}

	$now_time = mktime(0,0,0,date("m"),1,date("Y"));
	if ($now_time > $dis_time) {
		$next_mon = $mon + 1;
		if ($next_mon > 12) {
			$next_mon = 1;
			$next_year = $year + 1;
		} else {
			$next_year = $year;
		}
		$next .= "	<form action=\"".$PHP_SELF."\" method=\"POST\">\n";
		$next .= "		<input type=\"hidden\" name=\"mode\" value=\"p_check\">\n";
		$next .= "		<input type=\"hidden\" name=\"action\" value=\"day\">\n";
		$next .= "		<input type=\"hidden\" name=\"year\" value=\"".$next_year."\">\n";
		$next .= "		<input type=\"hidden\" name=\"mon\" value=\"".$next_mon."\">\n";
		$next .= "		<input type=\"submit\" value=\"&gt;&gt;\">\n";
		$next .= "	</form>\n";
	}

	$last_day = date("t",$dis_time);
	for($day=1; $day<=$last_day; $day++) {
		//	クリック数
		$click = $CLICK[$day];
		if ($click < 1) { $click = 0; }

		//	売上件数
		$count = $COUNT[$day];
		if ($count < 1) { $count = 0; }

		//	獲得確定ポイント
		$point = $DEC[$day];
		if ($point < 1) { $point = 0; }

		//	獲得予定ポイント
		$indent_point = $ALL[$day] - $DEC[$day];

		//	変換ポイント
		$pay_point = $PAY[$day];

		//	有効ポイント	2013/12/19現在	未使用
//		$now_point = $point - $pay_point;
//		if ($now_point < 1) { $now_point = 0; }

		$click_ = number_format($click);
		$count_ = number_format($count);
		$point_ = number_format($point);
		$indent_point_ = number_format($indent_point);
		$pay_point_ = number_format($pay_point);
//		$now_point_ = number_format($now_point);

		$a_click += $click;
		$a_count += $count;
		$a_indent_point += $indent_point;
		$a_point += $point;
		$a_pay_point += $pay_point;
//		$a_now_point += $now_point;

		//	ひとケタの日数に0をつける 1→01
		$day = sprintf('%02d',$day);

		$html .= "<tr>\n";
		$html .= "	<td class=\"af_align\" data-label=\"日\">".$day."日</td>\n";
		$html .= "	<td data-label=\"クリック数\">".$click_."件</td>\n";
		$html .= "	<td data-label=\"売上件数\">".$count_."件</td>\n";
		$html .= "	<td data-label=\"予定ポイント\">".$indent_point_."pt</td>\n";
		$html .= "	<td data-label=\"確定ポイント\">".$point_."pt</td>\n";
		$html .= "	<td data-label=\"変換ポイント\">".$pay_point_."pt</td>\n";
		$html .= "</tr>\n";

	}

	$a_click = number_format($a_click);
	$a_count = number_format($a_count);
	$a_indent_point = number_format($a_indent_point);
	$a_point = number_format($a_point);
	$a_pay_point = number_format($a_pay_point);
//	$a_now_point = number_format($a_now_point);

	$DEL_INPUTS['YEARHIDDEN'] = 1;				//	年毎のhidden削除
	$DEL_INPUTS['MONHIDDEN'] = 1;				//	月毎のhidden削除

	$INPUTS['SORT'] = "(日毎)";					//	ページタイトル
	$INPUTS['FIRST'] = $first;					//	前日へページ遷移「<<」
	$INPUTS['YEAR'] = $year;					//	「ポイント獲得履歴 "○○"年」値
	$INPUTS['DATE'] = "年".$mon."月";			//	「ポイント獲得履歴 ○○"年"」単位
	$INPUTS['NEXT'] = $next;					//	次月ページ遷移「>>」
	$INPUTS['THDATE'] = "日";					//	テーブルタイトル
	$INPUTS['POINTLIST'] = $html;				//	詳細一覧

	$INPUTS['ACLICK'] = $a_click;				//	クリック数合計
	$INPUTS['ACOUNT'] = $a_count;				//	売上件数合計
	$INPUTS['AINDENTPOINT'] = $a_indent_point;	//	予定ポイント合計
	$INPUTS['APOINT'] = $a_point;				//	確定ポイント合計
	$INPUTS['APAYPOINT'] = $a_pay_point;		//	変換ポイント合計

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("p_check.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return array($title,$html);

}
//------------------------//
// ポイント変換申請ページ //
//------------------------//
function p_apply_html($af_num,$ERROR) {

	global	$PHP_SELF,			//	現在実行しているスクリプトのファイル名
			$set_point_mon,		//	アフェリエイトポイント確定時期	2013/12/06現在	1ヶ月
			$change_point,		//	アフェリエイト返金可能ポイント	2013/12/06現在	5000
			$CHENGE_TYPE_L,		//	変換タイプ
			$DEPOSIT_L;			//	振り込み先

	$title = "ネイバーズスポーツ アフィリエイトポイント変換申請";

	//	フラグ
	$_SESSION['APPLYCHECK'] = 1;	//add 2013/12/27

	if ($ERROR) {
		$error_html = error_html($ERROR);
		unset($ERROR);
	}

	//	変換可能ポイントを取得
	unset($point);
	$point = point_checks($af_num);
	$point_ = number_format($point);

	if ($_SESSION['BANK']) {
		$point = $_SESSION['BANK']['point'];
		$change_type = $_SESSION['BANK']['change_type'];
		$bank_name = $_SESSION['BANK']['bank_name'];
		$store_name = $_SESSION['BANK']['store_name'];
		$deposit = $_SESSION['BANK']['deposit'];
		$account_num = $_SESSION['BANK']['account_num'];
		$account_name = $_SESSION['BANK']['account_name'];
		unset($_SESSION['BANK']);
	}

	if ($change_type == 1) {
		$checked1 = "checked=\"checked\"";
		$display = "none";
		$hyouji = "on";
	} else {
		$checked2 = "checked=\"checked\"";
		$display = "block";
		$hyouji = "off";
	}

	$selected21 = $selected22 = "";
	if ($deposit == 2) { $selected22 = "selected"; } else { $selected21 = "selected"; }

	$change_point = number_format($change_point);

	//	※配列の[0]が空白なのでループ処理するとプルダウンに空欄ができてしまう。$CHENGE_TYPE_L(ラジオ)も同様
	//	変換種類ラジオボタン
	$chenge_type_html .= "<div class=\"box-row\">\n";
	$chenge_type_html .= "	<p class=\"item-name\">変換種類</p>\n";
	$chenge_type_html .= "	<div class=\"input-section\">\n";
	$chenge_type_html .= "		<input type=\"radio\" name=\"change_type\" value=\"1\" ".$checked1." onclick=\"hyouji('on');\">：".$CHENGE_TYPE_L[1]."<br />\n";
	$chenge_type_html .= "		<input type=\"radio\" name=\"change_type\" value=\"2\" ".$checked2." onclick=\"hyouji('off');\">：".$CHENGE_TYPE_L[2]."\n";
	$chenge_type_html .= "	</div>\n";
	$chenge_type_html .= "	</div>\n";


	//	振込先科目プルダウン
	$deposit_html .= "<div class=\"box-row\">\n";
	$deposit_html .= "<div class=\"item-name\">\n";
	$deposit_html .= "	<p class=\"af_subtitle\">振込先科目</p>\n";
	$deposit_html .= "</div>\n";
	$deposit_html .= "	<div class=\"input-section\">\n";
	$deposit_html .= "		<select name=\"deposit\">\n";
	$deposit_html .= "			<option value=\"1\" ".$selected21.">".$DEPOSIT_L[1]."</option>\n";
	$deposit_html .= "			<option value=\"2\" ".$selected22.">".$DEPOSIT_L[2]."</option>\n";
	$deposit_html .= "		</select>\n";
	$deposit_html .= "	</div>\n";
	$deposit_html .= "	</div>\n";

	$DEL_INPUTS['PAPPLICHANGE'] = 1;				//	テンプレのポイント変換変更ページ部分を削除
	$DEL_INPUTS['LISTBACK'] = 1;					//	テンプレのポイント変換変更ページ部分を削除

	$INPUTS['POINT'] = $point_;						//	変更可能ポイント
	$INPUTS['INPUTPOINT'] = $point;					//	ユーザー入力欄
	$INPUTS['CHANGEPOINT'] = $change_point;			//	返金可能ポイント
	$INPUTS['CHENGETYPEL'] = $chenge_type_html;		//	変換種類ラジオボタン
	$INPUTS['BANKNAME'] = $bank_name;				//	金融機関名
	$INPUTS['STORENAME'] = $store_name;				//	支店名
	$INPUTS['DEPOSIT'] = $deposit_html;				//	振込先科目プルダウン
	$INPUTS['ACCONUTNUM'] = $account_num;			//	振込先口座番号
	$INPUTS['ACCOUNTNAME'] = $account_name;			//	受取人名
	$INPUTS['DISPLAY'] = $display;					//	displayプロパティパラメーター
	$INPUTS['HYOUJI'] = $hyouji;					//	リセットボタンonclickパラメータ

	$INPUTS['ERRORMSG'] = $error_html;				//	エラーメッセージ

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("pap_defaults.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return array($title,$html);

}
//----------------------------//
// ポイント変換申請確認ページ //
//----------------------------//
function p_apply_check_html($af_num) {

	global	$PHP_SELF,		//	現在実行しているスクリプトのファイル名
			$CHENGE_TYPE_L,	//	変換タイプ
			$DEPOSIT_L;		//	振り込み先

	if ($_SESSION['BANK']) {
		$point = $_SESSION['BANK']['point'];
		$change_type = $_SESSION['BANK']['change_type'];
		$bank_name = $_SESSION['BANK']['bank_name'];
		$store_name = $_SESSION['BANK']['store_name'];
		$deposit = $_SESSION['BANK']['deposit'];
		$account_num = $_SESSION['BANK']['account_num'];
		$account_name = $_SESSION['BANK']['account_name'];
	}

	$point = number_format($point);

	//	獲得ポイント
	$point_ = point_checks($af_num);
	$point_ = number_format($point_);

	if ($change_type != 2) {
		$DEL_INPUTS['CHANGETYPE2'] = 1;						//	振込口座情報表示部分を削除
	}

	$DEL_INPUTS['NOTCANCEL'] = 1;							//	キャンセル取り消し部分を削除

	$INPUTS['MODE'] = "p_apply";							//	modeパラメーター
	$INPUTS['ACTION'] = "p_apply_regist";					//	actionパラメーター
	$INPUTS['STYPE'] = "申請";								//	submitボタン
	$INPUTS['INPUTPOINT'] = $point;							//	ユーザー入力ポイント
	$INPUTS['POINT'] = $point_;								//	変更可能ポイント
	$INPUTS['CHENGETYPEL'] = $CHENGE_TYPE_L[$change_type];	//	変換種類
	$INPUTS['BANKNAME'] = $bank_name;						//	金融機関名
	$INPUTS['STORENAME'] = $store_name;						//	支店名
	$INPUTS['DEPOSIT'] = $DEPOSIT_L[$deposit];				//	振込先科目
	$INPUTS['ACCONUTNUM'] = $account_num;					//	振込先口座番号
	$INPUTS['ACCOUNTNAME'] = $account_name;					//	受取人名

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("pap_check.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return array($title,$html);

}
//--------------------------//
// ポイント変換申請確認処理 //
//--------------------------//
function p_apply_check($af_num,&$ERROR){

	global	$PHP_SELF,		//	現在実行しているスクリプトのファイル名
			$set_point_mon,	//	アフェリエイトポイント確定時期	2013/12/06現在	1ヶ月
			$change_point;	//	アフェリエイト返金可能ポイント	2013/12/06現在	5000

	if($_SESSION['APPLYCHECK'] != 1){					//	add 2013/12/27
		header ("Location: $PHP_SELF?mode=p_apply");	//	add 2013/12/27
		exit;											//	add 2013/12/27
	}													//	add 2013/12/27

	$point = $_POST['point'];
#	$point = mb_convert_kana($point,"sn","EUC-JP");
	$point = mb_convert_kana($point,"sn","UTF-8");
	$point = trim($point);
	$change_type = $_POST['change_type'];
	$bank_name = $_POST['bank_name'];
#	$bank_name = mb_convert_kana($bank_name,"sCKV","EUC-JP");
	$bank_name = mb_convert_kana($bank_name,"sCKV","UTF-8");
	$bank_name = trim($bank_name);
	$store_name = $_POST['store_name'];
#	$store_name = mb_convert_kana($store_name,"sCKV","EUC-JP");
	$store_name = mb_convert_kana($store_name,"sCKV","UTF-8");
	$store_name = trim($store_name);
	$deposit = $_POST['deposit'];
	$account_num = $_POST['account_num'];
#	$account_num = mb_convert_kana($account_num,"sn","EUC-JP");
	$account_num = mb_convert_kana($account_num,"sn","UTF-8");
	$account_num = trim($account_num);
	$account_name = $_POST['account_name'];
#	$account_name = mb_convert_kana($account_name,"sCKV","EUC-JP");
	$account_name = mb_convert_kana($account_name,"sCKV","UTF-8");
	$account_name = trim($account_name);

	$change_point_ = number_format($change_point);

	if (!$point) {
		$ERROR[] = "変換するポイントが入力されておりません。";
	} elseif (!isNum($point)) {
		$ERROR[] = "数字以外の文字が含まれております。";
	} elseif ($point < $change_point) {
		$ERROR[] = "変換出来るポイントは、".$change_point_."pt以上です。";
	}

	if ($change_type == 2) {
		if (!$bank_name) {
			$ERROR[] = "金融機関名が入力されておりません。";
		} elseif (!isZenKkana($bank_name)) {
			$ERROR[] = "金融機関名に、カタカナ以外の文字が含まれております。";
		}
		if (!$store_name) {
			$ERROR[] = "支店名が入力されておりません。";
		} elseif (!isZenKkana($store_name)) {
			$ERROR[] = "支店名に、カタカナ以外の文字が含まれております。";
		}
		if (!$deposit) {
			$ERROR[] = "振込先科目が選択されておりません。";
		}
		if (!$account_num) {
			$ERROR[] = "振込先口座番号が入力されておりません。";
		} elseif (!isNum($account_num)) {
			$ERROR[] = "振込先口座番号に、数字以外の文字が含まれております。";
		}
		if (!$account_name) {
			$ERROR[] = "受取人名が入力されておりません。";
		} elseif (!isZenKkana($account_name)) {
			$ERROR[] = "受取人名に、カタカナ以外の文字が含まれております。";
		}
	} elseif ($change_type != 1) {
		$ERROR[] = "変換種類が確認出来ません。";
	}

	if (!$ERROR) {
		//	獲得ポイント
		$point_ = point_checks($af_num);
		if ($point_ < $point) {
			$ERROR[] = "入力されたポイントが獲得ポイントを越えております。";
		}
	}

	//	セッション埋め込み
	unset($_SESSION['BANK']);
	$_SESSION['BANK']['point'] = $point;
	$_SESSION['BANK']['change_type'] = $change_type;
	if ($change_type == 2) {
		$_SESSION['BANK']['bank_name'] = $bank_name;
		$_SESSION['BANK']['store_name'] = $store_name;
		$_SESSION['BANK']['deposit'] = $deposit;
		$_SESSION['BANK']['account_num'] = $account_num;
		$_SESSION['BANK']['account_name'] = $account_name;
	}

	if (!$_SESSION['BANK'] || !$af_num) {
		$ERROR[] = "入力された情報が確認できません。";
	}

}
//--------------------------------------//
// ポイント変換申請登録処理＆メール送信 //
//--------------------------------------//
function p_apply_regist($af_num,&$ERROR){

	global	$PHP_SELF,			//	現在実行しているスクリプトのファイル名
			$bank_table,		//	bankテーブル名
			$afuser_table,		//	afuserテーブル名
			$appoint_table,		//	appointテーブル名
			$application_table,	//	applicationテーブル名
			$member_table,		//	kojinテーブル名
			$set_point_mon,		//	アフェリエイトポイント確定時期	2013/12/06現在	1ヶ月
			$change_point, 		//	アフェリエイト返金可能ポイント	2013/12/06現在	5000
			$CHENGE_TYPE_L,		//	変換タイプ→array('','ネイバーズスポーツ割引ポイントに返還する。','ご指定の口座にお振り込みをする。');
			$CHENGE_TYPE2_L,	//	変換タイプ→array('','割引ポイント','口座にお振り込み');
			$DEPOSIT_L,			//	振込先
			$DIR_CPT,			//	tempディレクトリ
			$m_footer,			//	メールテンプレート
			$admin_mail_a,		//	affiliate@futboljersey.com
			$admin_name;		//	NEIGHBOURS SPORTS

	//	ロック：
	//	ポイント変換処理を行っている間はその他の申請を遮断する。
	//	申請が重複した時の二重登録を防止する。
	if (!$ERROR) {
		$lockdir = "$DIR_CPT/$af_num";
		lock($lockdir);
	}

	if ($_SESSION['BANK']) {
		$point = $_SESSION['BANK']['point'];
		$change_type = $_SESSION['BANK']['change_type'];
		$bank_name = $_SESSION['BANK']['bank_name'];
		$store_name = $_SESSION['BANK']['store_name'];
		$deposit = $_SESSION['BANK']['deposit'];
		$account_num = $_SESSION['BANK']['account_num'];
		$account_name = $_SESSION['BANK']['account_name'];
	}

	//	ポイント確認
	$point_ = point_checks($af_num);
	if ($point > $point_) {
		$ERROR[] = "変換するポイントをご確認下さい。";
		return;
	}

	//	振込先登録
	if (!$ERROR && $change_type == 2) {
		$sql  = "INSERT INTO $bank_table" .
				" (af_num,bank_name,store_name,deposit,account_num,account_name)" .
				" VALUES('$af_num','$bank_name','$store_name','$deposit','$account_num','$account_name');";
		if (!$result = pg_exec(DB,$sql)) {
			// $ERROR[] = "振込先情報を登録出来ませんでした。";
		} else {
			$sql  = "SELECT MAX(bank_num) AS max FROM $bank_table" .
					" WHERE af_num='$af_num' AND state='0';";
			if ($result = pg_query(DB,$sql)) {
				$list = pg_fetch_array($result);
				$bank_num = $list['max'];
			}
		}
	}
	if (!$bank_num) {
		$bank_num = 0;
	}

	//	支払いテーブル記録
	if (!$ERROR) {
		$sql  = "INSERT INTO $application_table" .
				" (af_num,change_type,bank_num,af_point,appli_day)" .
				" VALUES('$af_num','$change_type','$bank_num','$point',now());";
		if (!$result = pg_exec(DB,$sql)) {
			$ERROR[] = "変換情報を登録出来ませんでした。";
		} else {
			$sql  = "SELECT MAX(appli_num) AS max FROM $application_table" .
					" WHERE af_num='$af_num';";
			if ($result = pg_query(DB,$sql)) {
				$list = pg_fetch_array($result);
				$appli_num = $list['max'];
			}
		}
	}

	if($ERROR){
		return;
	} elseif (!$ERROR) {
	//	メール配信

		//	ユーザー情報抜き出し
		$sql  = "SELECT a.kojin_num, a.name_s, a.name_n, a.email" .
				" FROM $member_table a, $afuser_table b" .
				" WHERE a.kojin_num=b.kojin_num AND b.state!='1' AND b.af_num='$af_num' LIMIT 1;";
		if ($result = pg_query(DB,$sql)) {
			$list = pg_fetch_array($result);
			$kojin_num = $list['kojin_num'];
			$name_s = $list['name_s'];
			$name_n = $list['name_n'];
			$name = "$name_s $name_n";
			$email = $list['email'];
		}
		if (!$kojin_num || !$name_s || !$name_n || !$email) {
			$ERROR[] = "会員情報が確認出来ません。";
		}
	}

	if (!$ERROR) {
		unset($bank_msg);
		if ($change_type == 2) {
			$bank_msg = <<<WAKABA

お振り込み口座
金融機関名：{$bank_name}
支店名：{$store_name}
振込先科目：{$DEPOSIT_L[$deposit]}
振込先口座番号：{$account_num}
受取人名：{$account_name}

WAKABA;
		}
		$point = number_format($point);
		$appli_num_ = sprintf("%05d",$appli_num);

		//	受け取り
		//	件名
		$subject = "アフィリエイトポイント変換申請 (申請番号：{$appli_num_})";

		$ip = getenv("REMOTE_ADDR");
		$host = gethostbyaddr($ip);
		if (!$host) { $host = $ip; }

		$msr = <<<WAKABA
{$name}様がアフィリエイトポイント変換申請しました。

名前：{$name}
会員番号：{$kojin_num}
ID：{$af_num}
E-mail：{$email}

申請番号：{$appli_num_}
変換ポイント：{$point}pt
変換種類：{$CHENGE_TYPE2_L[$change_type]}
$bank_msg
------------------------------------------------------
$host ($ip)

WAKABA;
//echo('店舗受け取りメール=>'.$msr."<br />");

		//	送信処理
		$send_email = $email;
		$send_name = $name;
		$get_email = $admin_mail_a;
//$get_email = "検証用アドレス";
		send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msr);

		//	会員
		//	件名
		$subject = "アフィリエイトポイント変換申請確認";

		//	メッセージ
		$msr = <<<WAKABA
{$name}様、ネイバーズスポーツアフィリエイトポイント変換申請受付完了致しました。
受付内容は以下でよろしいでしょうか？
情報がおかしな場合、申請した記憶がない場合はお手数ですがご連絡お願い致します。

名前：{$name}
会員番号：{$kojin_num}
ID：{$af_num}
E-mail：{$email}

申請番号：{$appli_num_}
変換ポイント：{$point}pt
変換種類：{$CHENGE_TYPE2_L[$change_type]}
$bank_msg

それではこれからもよろしくお願い致します。

{$m_footer}

WAKABA;
//echo('ユーザー宛てメール=>'.$msr."<br />");

		//	送信処理
		$send_email = $admin_mail_a;
		$send_name = $admin_name;
		$get_email = $email;
//$get_email = "検証用アドレス";
		send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msr);

	}

	//	保持したデータを削除
	if (!$ERROR) {
		unset($_SESSION['BANK']);
		unset($_SESSION['APPLYCHECK']);	//	add 2013/12/27
	} else {
		if ($bank_num) {
			$sql  = "DELETE FROM $bank_table WHERE bank_num='$bank_num';";
			$result = pg_query(DB,$sql);
		}
		if ($appli_num) {
			$sql  = "DELETE FROM $application_table WHERE appli_num='$appli_num';";
			$result = pg_query(DB,$sql);
		}
	}

	//	ロック解除
	if (file_exists($lockdir)) {
		rmdir($lockdir);
	}

	header ("Location: ./template/thank_apply.htm");
	exit;

}

//--------------------------//
//	変換可能ポイントを取得	//
//--------------------------//
function point_checks($af_num) {

	global	$afuser_table,		//	afuserテーブル名
			$appoint_table,		//	appointテーブル名
			$application_table,	//	applicationテーブル名
			$set_point_mon;		//	アフェリエイトポイント確定時期	2013/12/06現在	1ヶ月

	//	確定ポイント(全て)
	$order_time = mktime(0,0,0,date("m")-$set_point_mon,1,date("Y"));
	$order_day = date("Y-m-d",$order_time);
	$sql =  "SELECT SUM(point) AS dec_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND state='1'" .
			" AND send_day<'$order_day';";
	if ($result = pg_query(DB,$sql)) {
		$list = pg_fetch_array($result);
		(int)$dec_point = $list['dec_point'];
	}
	//	支払い情報(全て)
	$sql =  "SELECT SUM(af_point) AS pay_point FROM $application_table" .
			" WHERE af_num='$af_num' AND state!='2';";
	if ($result = pg_query(DB,$sql)) {
		$list = pg_fetch_array($result);
		(int)$pay_point = $list['pay_point'];
	}
	//	獲得確定ポイント
	$point = $dec_point - $pay_point;
	if ($point < 1) { $point = 0; }

	return $point;
}
//------------------//
// カタカナチェック //
//------------------//
function isZenKkana($data) {

    //magic_quotes_gpcがONの時は、エスケープを解除する
    if (get_magic_quotes_gpc()) {
        $data = stripslashes($data);
    }

    $data= trim($data);
    $pat = "^[ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブプヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲン]+$";   
    if (mb_ereg_match($pat, $data)) {
        return true;
    } else {
        return false;
    }

}
//---------------//
//	数字チェック //
//---------------//

function isNum($data) {

    $pat = "/^[0-9]+$/";
    if (preg_match($pat, trim($data))) {
        return true;
    } else {
        return false;
    }

}
//--------//
// ロック //
//--------//
//	ポイント変換処理を行っている間はその他の申請を遮断する。
//	申請が重複した時の二重登録を防止する。
function lock($lockdir) {

	//	ファイルまたはディレクトリが存在するかどうか調べる
	if (file_exists($lockdir)) {
		//	現在時刻を取得
		$now = time();
		//	ファイルの統計情報を取得
		list($device,$inode,$remode,$num_of_link,$user_id,$group_id,$rdev,
		$size,$atime,$mtime,$ctime,$blocksize,$num_of_blocks) = stat($lockdir);

		//現在時刻 - 最終修正時間
		$sa = $now - $mtime;
		if ($sa >= 30) {
			if (file_exists($lockdir)) {
				//	ディレクトリを 削除
				rmdir($lockdir);
			}
		}
	}

	$flag = 0;
	for($i=1; $i<=5; $i++) {
		if (!file_exists($lockdir)) {
			//	ディレクトリを作成
			mkdir($lockdir,0777);
			//	ファイルのアクセス権限を0777に変更
			//	＠→エラー制御演算子：エラーがあってもメッセージを表示させない
			@chmod($lockdir,0777);
			$flag = 1;
			break;
		} else {
			//1秒間プログラムの実行を遅延させます
			sleep(1);
		}
	}

}

//----------------------//
// ポイント変換履歴一覧 //
//----------------------//
function p_update_html($af_num,$ERROR){

	global	$PHP_SELF,			//	現在実行しているスクリプトのファイル名
			$application_table,	//	applicationテーブル名
			$CHENGE_TYPE2_L;	//	変換種類

	$title = "ネイバーズスポーツ アフィリエイトポイント変換履歴";

	//フラグ作成
	$_SESSION['CHANGECHECK'] = 1;	//	add /2013/12/27

	//	エラー表示
	if($ERROR){
		$errors = error_html($ERROR);
	}

	unset($_SESSION['BANK']);

	//	一覧取得
	$sql  = "SELECT * FROM $application_table" .
			" WHERE af_num='$af_num' ORDER BY appli_num DESC;";
	if ($result = pg_query(DB,$sql)) {
		$count = pg_numrows($result);
	}

	//	変換履歴がなければメッセージ表示
	if ($count <= 0) {
		$html .= "<p>今現在、変換申請履歴はございません。<br /><br /></p>\n";
		$html .= "<form action=\"".$PHP_SELF."\" method=\"POST\">\n";
		$html .= "	<input type=\"submit\" value=\"アフィリエイトTOPに戻る\">\n";
		$html .= "</form>\n";
		return array($title,$html);
	}

	//	変換履歴一覧を作成
	while ($list = pg_fetch_array($result)) {
		$appli_num = $list['appli_num'];
		$af_num = $list['af_num'];
		$change_type = $list['change_type'];
		$bank_num = $list['bank_num'];
		$af_point = $list['af_point'];
		$appli_day = $list['appli_day'];
		$pay_price = $list['pay_price'];
		$pay_day = $list['pay_day'];
		$state = $list['state'];

		//	申請番号
		$appli_num = sprintf("%05d",$appli_num);

		//	申請日
		list($appli_day) = explode(" ",$appli_day);

		//	変換申請ポイント
		$af_point = number_format($af_point);

		//	変換ポイント・金額
		$pay_price = number_format($pay_price);

		//	$state == 1 → 変換手続き完了
		if ($state == 1) {
			list($pay_day) = explode(" ",$pay_day);
			$submit = $appli_num;
		//	$state == 2 → キャンセル
		} elseif ($state == 2) {
			$pay_day = "キャンセル";
			$pay_price = 0;
			$submit = $appli_num;
		//	その他		→ 変換手続き完了待ち
		} else {
			$pay_day = "手続き完了待ち";
			$pay_price = 0;
			$submit = "<input type=\"submit\" name=\"appli_num\" value=\"".$appli_num."\">";
		}

		if ($change_type == 1) {
			$pay_price .= "pt";
		} else {
			$pay_price .= "円";
		}

		$html .= "<tr>\n";
		$html .= "	<td data-label=\"申請番号\">".$submit."</td>\n";
		$html .= "	<td data-label=\"申請日\">".$appli_day."</td>\n";
		$html .= "	<td data-label=\"変換種類\">".$CHENGE_TYPE2_L[$change_type]."</td>\n";
		$html .= "	<td data-label=\"変換申請ポイント\">".$af_point."pt</td>\n";
		$html .= "	<td data-label=\"変換ポイント・金額\">".$pay_price."</td>\n";
		$html .= "	<td data-label=\"状態・変換日\">".$pay_day."</td>\n";
		$html .= "</tr>\n";
	}

	$INPUTS['ERRORMSG'] = $errors;					//	エラー表示
	$INPUTS['AFPLIST'] = $html;						//	詳細一覧
	$INPUTS['CHENGETYPE2L'] = $CHENGE_TYPE2_L["2"];	//	支払方法
	$INPUTS['MODE'] = $mode;						//	$modeパラメーター

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("pac_defaults.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return array($title,$html);

}
//----------------------------//
// ポイント変換変更内容の入力 //
//----------------------------//
function p_update_change_html($af_num,$ERROR){

	global	$PHP_SELF,			//	現在実行しているスクリプトのファイル名
			$application_table,	//	applicationテーブル
			$bank_table,		//	bankテーブル
			$change_point, 		//	アフェリエイト返金可能ポイント	2013/12/06現在	5000
			$CHENGE_TYPE_L,		//	変換タイプ
			$DEPOSIT_L;			//	振り込み先

	$title = "ネイバーズスポーツ アフィリエイトポイント変換履歴";

	$appli_num = $_POST['appli_num'];

	if($_SESSION['CANCEL'] && !$appli_num){
		$appli_num = $_SESSION['CANCEL']['appli_num'];
	}

	if (!$appli_num) {
		$ERROR[] = "変換申請番号が確認出来ません。";
	}

	//	獲得ポイント
	if (!$ERROR) {
		$now_point = point_checks($af_num);
	}

	//	基本情報読み込み
	if (!$ERROR) {
		$sql  = "SELECT * FROM $application_table" .
				" WHERE appli_num='$appli_num' AND af_num='$af_num' LIMIT 1;";
		if ($result = pg_query(DB,$sql)) {
			$list = pg_fetch_array($result);
			$change_type = $list['change_type'];
			$bank_num = $list['bank_num'];
			$point = $list['af_point'];
			$state = $list['state'];
		}
		if ($state > 0) {
			$appli_num = sprintf("%05d",$appli_num);
			$ERROR[] = "変換申請番号($appli_num)は、処理が完了しております。";
		}
	}

	//	口座チェック
	if (!$ERROR && $bank_num > 0) {
		$sql  = "SELECT * FROM $bank_table" .
				" WHERE bank_num='$bank_num' AND af_num='$af_num' AND state='0' LIMIT 1;";
		if ($result = pg_query(DB,$sql)) {
			$list = pg_fetch_array($result);
			$bank_num = $list['bank_num'];
			$bank_name = $list['bank_name'];
			$store_name = $list['store_name'];
			$deposit = $list['deposit'];
			$account_num = $list['account_num'];
			$account_name = $list['account_name'];
		}
		if (!$bank_num) { $ERROR[] = "お振り込み口座情報が確認出来ませんでした。"; }
	}

	if (!$ERROR) {
		//	変換可能獲得ポイント
		// var_dump($now_point);
		// var_dump($point);
		$total_point = $now_point + $point;
		//	セッション埋め込み
		$_SESSION['BANK']['appli_num'] = $appli_num;
		$_SESSION['BANK']['bank_num'] = $bank_num;
		$_SESSION['BANK']['total_point'] = $total_point;

		//	$_SESSION['BANK']をunsetするとキャンセル画面でappli_numとbank_numが使えないので別のセッション名を使用する
		$_SESSION['CANCEL']['appli_num'] = $appli_num;
		$_SESSION['CANCEL']['bank_num'] = $bank_num;

	} else {
		$point  = $_SESSION['BANK']['point'];
		$total_point = $_SESSION['BANK']['total_point'];
		$change_type = $_SESSION['BANK']['change_type'];
		$bank_num = $_SESSION['BANK']['bank_num'];
		$bank_name = $_SESSION['BANK']['bank_name'];
		$store_name = $_SESSION['BANK']['store_name'];
		$deposit = $_SESSION['BANK']['deposit'];
		$account_num = $_SESSION['BANK']['account_num'];
		$account_name = $_SESSION['BANK']['account_name'];
	}

	$total_point = number_format($total_point);

	if ($ERROR) {
		$error_html = error_html($ERROR);
		unset($ERROR);
	}

	if ($change_type == 1) {
		$checked1 = "checked=\"checked\"";
		$display = "none";
		$hyouji = "on";
	} else {
		$checked2 = "checked=\"checked\"";
		$display = "block";
		$hyouji = "off";
	}

	$selected21 = $selected22 = "";
	if ($deposit == 2) { $selected22 = "selected"; } else { $selected21 = "selected"; }

	$change_point = number_format($change_point);

	//	※配列の[0]が空白なのでループ処理するとプルダウンに空欄ができてしまう。$CHENGE_TYPE_L(ラジオ)も同様
	//	変換種類ラジオボタン

	$chenge_type_html .= "<div class=\"box-row\">\n";
	$chenge_type_html .= "	<p class=\"item-name\">変換種類</p>\n";
	$chenge_type_html .= "	<div class=\"input-section\">\n";
	$chenge_type_html .= "		<input type=\"radio\" name=\"change_type\" value=\"1\" ".$checked1." onclick=\"hyouji('on');\">：".$CHENGE_TYPE_L[1]."<br />\n";
	$chenge_type_html .= "		<input type=\"radio\" name=\"change_type\" value=\"2\" ".$checked2." onclick=\"hyouji('off');\">：".$CHENGE_TYPE_L[2]."\n";
	$chenge_type_html .= "	</div>\n";
	$chenge_type_html .= "	</div>\n";

	//	振込先科目プルダウン

	$deposit_html .= "<div class=\"box-row\">\n";
	$deposit_html .= "<div class=\"item-name\">\n";
	$deposit_html .= "	<p class=\"af_subtitle\">振込先科目</p>\n";
	$deposit_html .= "</div>\n";
	$deposit_html .= "	<div class=\"input-section\">\n";
	$deposit_html .= "		<select name=\"deposit\">\n";
	$deposit_html .= "			<option value=\"1\" ".$selected21.">".$DEPOSIT_L[1]."</option>\n";
	$deposit_html .= "			<option value=\"2\" ".$selected22.">".$DEPOSIT_L[2]."</option>\n";
	$deposit_html .= "		</select>\n";
	$deposit_html .= "	</div>\n";
	$deposit_html .= "	</div>\n";

	$DEL_INPUTS['PAPPLI'] = 1;						//	テンプレのポイント変換申請部分を削除

	$INPUTS['MODE'] = "p_update";					//	modeパラメーター
	$INPUTS['POINT'] = $total_point;				//	所有する変更可能ポイント
	$INPUTS['INPUTPOINT'] = $point;					//	変更可能ポイント
	$INPUTS['CHANGEPOINT'] = $change_point;			//	返金可能ポイント
	$INPUTS['CHENGETYPEL'] = $chenge_type_html;		//	変換種類ラジオボタン
	$INPUTS['BANKNAME'] = $bank_name;				//	金融機関名
	$INPUTS['STORENAME'] = $store_name;				//	支店名
	$INPUTS['DEPOSIT'] = $deposit_html;				//	振込先科目プルダウン
	$INPUTS['ACCONUTNUM'] = $account_num;			//	振込先口座番号
	$INPUTS['ACCOUNTNAME'] = $account_name;			//	受取人名
	$INPUTS['DISPLAY'] = $display;					//	displayプロパティパラメーター
	$INPUTS['HYOUJI'] = $hyouji;					//	リセットボタンonclickパラメータ

	$INPUTS['ERRORMSG'] = $error_html;				//	エラーメッセージ

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("pap_defaults.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return array($title,$html);

}
//--------------------------------//
// ポイント変換変更内容の確認処理 //
//--------------------------------//
function p_update_check($af_num,&$ERROR){

	global	$PHP_SELF,			//	現在実行しているスクリプトのファイル名
			$set_point_mon,		//	アフェリエイトポイント確定時期	2013/12/06現在	1ヶ月
			$change_point;		//	返金可能ポイント

	//	フラグ確認
	if($_SESSION['CHANGECHECK'] != 1){					//	add 2013/12/27
		header ("Location: $PHP_SELF?mode=p_update");	//	add 2013/12/27
		exit;											//	add 2013/12/27
	}													//	add 2013/12/27

	$point = $_POST['point'];
#	$point = mb_convert_kana($point,"sn","EUC-JP");
	$point = mb_convert_kana($point,"sn","UTF-8");
	$point = trim($point);
	$change_type = $_POST['change_type'];
	$bank_name = $_POST['bank_name'];
#	$bank_name = mb_convert_kana($bank_name,"sCKV","EUC-JP");
	$bank_name = mb_convert_kana($bank_name,"sCKV","UTF-8");
	$bank_name = trim($bank_name);
	$store_name = $_POST['store_name'];
#	$store_name = mb_convert_kana($store_name,"sCKV","EUC-JP");
	$store_name = mb_convert_kana($store_name,"sCKV","UTF-8");
	$store_name = trim($store_name);
	$deposit = $_POST['deposit'];
	$account_num = $_POST['account_num'];
#	$account_num = mb_convert_kana($account_num,"sn","EUC-JP");
	$account_num = mb_convert_kana($account_num,"sn","UTF-8");
	$account_num = trim($account_num);
	$account_name = $_POST['account_name'];
#	$account_name = mb_convert_kana($account_name,"sCKV","EUC-JP");
	$account_name = mb_convert_kana($account_name,"sCKV","UTF-8");
	$account_name = trim($account_name);

	$ERROR = array();
	$change_point_ = number_format($change_point);
	if (!$point) {
		$ERROR[] = "変換するポイントが入力されておりません。";
	} elseif (!isNum($point)) {
		$ERROR[] = "数字以外の文字が含まれております。";
	} elseif ($point < $change_point) {
		$ERROR[] = "変換出来るポイントは、".$change_point_."pt以上です。";
	}
	if ($change_type == 2) {
		if (!$bank_name) {
			$ERROR[] = "金融機関名が入力されておりません。";
		} elseif (!isZenKkana($bank_name)) {
			$ERROR[] = "金融機関名に、カタカナ以外の文字が含まれております。";
		}
		if (!$store_name) {
			$ERROR[] = "支店名が入力されておりません。";
		} elseif (!isZenKkana($store_name)) {
			$ERROR[] = "支店名に、カタカナ以外の文字が含まれております。";
		}
		if (!$deposit) {
			$ERROR[] = "振込先科目が選択されておりません。";
		}
		if (!$account_num) {
			$ERROR[] = "振込先口座番号が入力されておりません。";
		} elseif (!isNum($account_num)) {
			$ERROR[] = "振込先口座番号に、数字以外の文字が含まれております。";
		}
		if (!$account_name) {
			$ERROR[] = "受取人名が入力されておりません。";
		} elseif (!isZenKkana($account_name)) {
			$ERROR[] = "受取人名に、カタカナ以外の文字が含まれております。";
		}
	} elseif ($change_type != 1) {
		$ERROR[] = "変換種類が確認出来ません。";
	}

	if (!$ERROR) {
		//	獲得ポイント
		$point_ = point_checks($af_num);
		if ($point_ < $point) {
			var_dump($point_);
			var_dump($point);
			$ERROR[] = "入力されたポイントが獲得ポイントを越えております。";
		}
	}

	//	セッション埋め込み
	$_SESSION['BANK']['point'] = $point;
	$_SESSION['BANK']['change_type'] = $change_type;
	if ($change_type == 2) {
		$_SESSION['BANK']['bank_name'] = $bank_name;
		$_SESSION['BANK']['store_name'] = $store_name;
		$_SESSION['BANK']['deposit'] = $deposit;
		$_SESSION['BANK']['account_num'] = $account_num;
		$_SESSION['BANK']['account_name'] = $account_name;
	} else {
		unset($_SESSION['BANK']['bank_name']);
		unset($_SESSION['BANK']['store_name']);
		unset($_SESSION['BANK']['deposit']);
		unset($_SESSION['BANK']['account_num']);
		unset($_SESSION['BANK']['account_name']);
	}

	if (!$_SESSION['BANK'] || !$af_num) {
		$ERROR[] = "入力された情報が確認できません。";
	}

}
//----------------------------------//
// ポイント変換変更内容の確認ページ //
//----------------------------------//
function p_update_check_html(){

	global	$PHP_SELF,		//	現在実行しているスクリプトのファイル名
			$CHENGE_TYPE_L,	//	アフェリエイトポイント確定時期	2013/12/06現在	1ヶ月
			$DEPOSIT_L;     //	返金可能ポイント

	$title = "ネイバーズスポーツ アフィリエイトポイント変換履歴";

	if ($_SESSION['BANK']) {
		$appli_num = $_SESSION['BANK']['appli_num'];
		$point = $_SESSION['BANK']['point'];
		$total_point = $_SESSION['BANK']['total_point'];
		$change_type = $_SESSION['BANK']['change_type'];
		$bank_name = $_SESSION['BANK']['bank_name'];
		$store_name = $_SESSION['BANK']['store_name'];
		$deposit = $_SESSION['BANK']['deposit'];
		$account_num = $_SESSION['BANK']['account_num'];
		$account_name = $_SESSION['BANK']['account_name'];
	}

	$point = number_format($point);

	//	獲得ポイント
	$total_point = number_format($total_point);



	if ($change_type != 2) {
		$DEL_INPUTS['CHANGETYPE2'] = 1;						//	振込口座情報表示部分を削除
	}
	$DEL_INPUTS['NOTCANCEL'] = 1;							//	キャンセル取り消し部分を削除

	$INPUTS['MODE'] = "p_update";							//	modeパラメーター
	$INPUTS['ACTION'] = "p_update_regist";					//	actionパラメーター
	$INPUTS['STYPE'] = "更新";								//	submitのvalue
	$INPUTS['POINT'] = $total_point;						//	所有する変換可能ポイント
	$INPUTS['INPUTPOINT'] = $point;							//	ユーザー入力ポイント
	$INPUTS['CHENGETYPEL'] = $CHENGE_TYPE_L[$change_type];	//	変換種類
	$INPUTS['BANKNAME'] = $bank_name;						//	金融機関名
	$INPUTS['STORENAME'] = $store_name;						//	支店名
	$INPUTS['DEPOSIT'] = $DEPOSIT_L[$deposit];				//	振込先科目
	$INPUTS['ACCONUTNUM'] = $account_num;					//	振込先口座番号
	$INPUTS['ACCOUNTNAME'] = $account_name;					//	受取人名

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("pap_check.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return array($title,$html);

}
//----------------------------------------//
// ポイント変換変更内容の更新＆メール送信 //
//----------------------------------------//
function p_update_regist($af_num,&$ERROR){

	global	$PHP_SELF,			//	現在実行しているスクリプトのファイル名
			$bank_table,		//	bankテーブル
			$afuser_table,		//	afuserテーブル
			$appoint_table,		//	appointテーブル
			$application_table,	//	applicationテーブル
			$member_table,		//	kojinテーブル
			$set_point_mon,		//	アフェリエイトポイント確定時期	2013/12/06現在	1ヶ月
			$change_point,      //	返金可能ポイント
			$CHENGE_TYPE_L,		//	変換タイプ→array('','ネイバーズスポーツ割引ポイントに返還する。','ご指定の口座にお振り込みをする。');
			$CHENGE_TYPE2_L,	//	変換タイプ→array('','割引ポイント','口座にお振り込み');
			$DEPOSIT_L,         //	返金可能ポイント
			$DIR_CPT,			//	ポイント返還tempフォルダー
			$m_footer,			//	メールテンプレート
			$admin_mail_a,		//	affiliate@futboljersey.com
			$admin_name;		//	NEIGHBOURS SPORTS

	//	ロック：
	//	ポイント変換処理を行っている間はその他の申請を遮断する。
	//	申請が重複した時の二重登録を防止する。
	if (!$ERROR) {
		$lockdir = "$DIR_CPT/$af_num";
		lock($lockdir);
	}

	if ($_SESSION['BANK']) {
		$appli_num = $_SESSION['BANK']['appli_num'];
		$point = $_SESSION['BANK']['point'];
		$total_point = $_SESSION['BANK']['total_point'];
		$change_type = $_SESSION['BANK']['change_type'];
		$bank_num = $_SESSION['BANK']['bank_num'];
		$bank_name = $_SESSION['BANK']['bank_name'];
		$store_name = $_SESSION['BANK']['store_name'];
		$deposit = $_SESSION['BANK']['deposit'];
		$account_num = $_SESSION['BANK']['account_num'];
		$account_name = $_SESSION['BANK']['account_name'];
	}

	//	ポイント確認
	if ($point > $total_point) {
		$ERROR[] = "変換するポイントをご確認下さい。";
	}

	//	振込先更新
	if (!$ERROR) {
		if ($change_type == 2) {
			if ($bank_num > 0) {
				$sql  = "UPDATE $bank_table SET" .
						" bank_name='$bank_name'," .
						" store_name='$store_name'," .
						" deposit='$deposit'," .
						" account_num='$account_num'," .
						" account_name='$account_name'" .
						" WHERE bank_num='$bank_num';";
				if (!$result = pg_exec(DB,$sql)) { $ERROR[] = "振込先情報を更新出来ませんでした。"; }
			} else {
				$sql  = "INSERT INTO $bank_table" .
						" (af_num,bank_name,store_name,deposit,account_num,account_name)" .
						" VALUES('$af_num','$bank_name','$store_name','$deposit','$account_num','$account_name');";
				if (!$result = pg_exec(DB,$sql)) {
					$ERROR[] = "振込先情報を登録出来ませんでした。";
				} else {
					$sql  = "SELECT MAX(bank_num) AS max FROM $bank_table" .
							" WHERE af_num='$af_num' AND state='0';";
					if ($result = pg_query(DB,$sql)) {
						$list = pg_fetch_array($result);
						$bank_num = $list['max'];
					}
				}
			}
		} elseif ($bank_num > 0) {	//	口座情報削除
			$sql  = "UPDATE $bank_table SET" .
					" state='1'" .
					" WHERE bank_num='$bank_num';";
			if (!$result = pg_exec(DB,$sql)) { $ERROR[] = "振込先情報を削除出来ませんでした。"; }
			unset($_SESSION['BANK']['bank_num']);
			unset($bank_num);
		}
	}
	if (!$bank_num) { $bank_num = 0; }

	//	支払いテーブル更新
	if (!$ERROR) {
		$sql  = "UPDATE $application_table SET" .
				" change_type='$change_type'," .
				" bank_num='$bank_num'," .
				" af_point='$point'," .
				" appli_day=now()" .
				" WHERE appli_num='$appli_num';";
		if (!$result = pg_exec(DB,$sql)) {
			$ERROR[] = "変換情報を更新出来ませんでした。";
		}
	}

	//	メール配信
	if (!$ERROR) {	//	ユーザー情報抜き出し
		$sql  = "SELECT a.kojin_num, a.name_s, a.name_n, a.email" .
				" FROM $member_table a, $afuser_table b" .
				" WHERE a.kojin_num=b.kojin_num AND b.state!='1' AND b.af_num='$af_num' LIMIT 1;";
		if ($result = pg_query(DB,$sql)) {
			$list = pg_fetch_array($result);
			$kojin_num = $list['kojin_num'];
			$name_s = $list['name_s'];
			$name_n = $list['name_n'];
			$name = "$name_s $name_n";
			$email = $list['email'];
		}
		if (!$kojin_num || !$name_s || !$name_n || !$email) {
			$ERROR[] = "会員情報が確認出来ません。";
		}
	}


	if($ERROR){
		return;
	}else{
		unset($bank_msg);
		if ($change_type == 2) {
			$bank_msg = <<<WAKABA

お振り込み口座
金融機関名：{$bank_name}
支店名：{$store_name}
振込先科目：{$DEPOSIT_L[$deposit]}
振込先口座番号：{$account_num}
受取人名：{$account_name}

WAKABA;
		}
		$point = number_format($point);

		//	受け取り
		//	件名
		$subject = "アフィリエイトポイント変換変更申請 (申請番号：{$appli_num})";

		$ip = getenv("REMOTE_ADDR");
		$host = gethostbyaddr($ip);
		if (!$host) { $host = $ip; }

		$msr = <<<WAKABA
{$name}様がアフィリエイトポイント変換変更申請しました。

名前：{$name}
会員番号：{$kojin_num}
ID：{$af_num}
E-mail：{$email}

申請番号：{$appli_num}
変換ポイント：{$point}pt
変換種類：{$CHENGE_TYPE2_L[$change_type]}
{$bank_msg}
------------------------------------------------------
{$host} ($ip)

WAKABA;
//echo('店舗へ送信メール=>'.$msr."<br />");

		//	送信処理
		$send_email = $email;
		$send_name = "$name";
		$get_email = $admin_mail_a;
//$get_email = "検証用アドレス";
		send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msr);

		//	会員
		//	件名
		$subject = "アフィリエイトポイント変換変更申請確認";

		//	メッセージ
		$msr = <<<WAKABA
{$name}様、ネイバーズスポーツアフィリエイトポイント変換変更申請受付完了致しました。
受付内容は以下でよろしいでしょうか？
情報がおかしな場合、申請した記憶がない場合はお手数ですがご連絡お願い致します。

名前：{$name}
会員番号：{$kojin_num}
ID：{$af_num}
E-mail：{$email}

申請番号：{$appli_num}
変換ポイント：{$point}pt
変換種類：{$CHENGE_TYPE2_L[$change_type]}
$bank_msg

それではこれからもよろしくお願い致します。

{$m_footer}

WAKABA;

//echo('顧客へ確認メール=>'.$msr."<br />");
		//	送信処理
		$send_email = $admin_mail_a;
		$send_name = $admin_name;
		$get_email = $email;
//$get_email = "検証用アドレス";
		send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msr);

	}

	//	保持したデータを削除
	if (!$ERROR) {
		unset($_SESSION['BANK']);
		unset($_SESSION['CANCEL']);
		unset($_SESSION['CHANGECHECK']);	//	add 2013/12/27
	}

	//	ロック解除
	if (file_exists($lockdir)) {
		rmdir($lockdir);
	}

	header ("Location: ./template/thank_update.htm");
	exit;

}
//----------------------------------------//
// ポイント変換変更内容のキャンセルページ //
//----------------------------------------//
function p_cancel_html($af_num,&$ERROR){

	global	$PHP_SELF,			//	現在実行しているスクリプトのファイル名
			$application_table,	//	applicationテーブル名
			$bank_table,		//	bankテーブル名
			$CHENGE_TYPE_L, 	//	アフェリエイトポイント確定時期	2013/12/06現在	1ヶ月
			$DEPOSIT_L;     	//	返金可能ポイント

	$title = "ネイバーズスポーツ アフィリエイトポイント変換履歴";

	//	フラグ確認
	if($_SESSION['CHANGECHECK'] != 1){					//	add 2013/12/27
		header ("Location: $PHP_SELF?mode=p_update");	//	add 2013/12/27
		exit;											//	add 2013/12/27
	}													//	add 2013/12/27

	if($_SESSION['CANCEL']){
		$appli_num = $_SESSION['CANCEL']['appli_num'];
	}

	//	基本情報読み込み
	$sql  = "SELECT * FROM $application_table" .
			" WHERE appli_num='$appli_num' AND af_num='$af_num' LIMIT 1;";
	if ($result = pg_query(DB,$sql)) {
		$list = pg_fetch_array($result);
		$change_type = $list['change_type'];
		$bank_num = $list['bank_num'];
		$point = $list['af_point'];
		$state = $list['state'];
	}
	if ($state > 0) {
		$appli_num = sprintf("%05d",$appli_num);
		$ERROR[] = "変換申請番号($appli_num)は、処理が完了しております。";
	}

	//	口座チェック
	if (!$ERROR && $bank_num > 0) {
		$sql  = "SELECT * FROM $bank_table" .
				" WHERE bank_num='$bank_num' AND af_num='$af_num' AND state='0' LIMIT 1;";
		if ($result = pg_query(DB,$sql)) {
			$list = pg_fetch_array($result);
			$bank_num = $list['bank_num'];
			$bank_name = $list['bank_name'];
			$store_name = $list['store_name'];
			$deposit = $list['deposit'];
			$account_num = $list['account_num'];
			$account_name = $list['account_name'];
		}
		if (!$bank_num) { $ERROR[] = "お振り込み口座情報が確認出来ませんでした。"; }
	}

	if($ERROR){
		return;
	}

	$point = number_format($point);

	//	獲得ポイント
	if ($_SESSION['BANK']) {
		$total_point = $_SESSION['BANK']['total_point'];
	}
	$total_point = number_format($total_point);



	if ($change_type != 2) {
		$DEL_INPUTS['CHANGETYPE2'] = 1;						//	振込口座情報表示部分を削除
	}
	$DEL_INPUTS['MODIFY'] = 1;								//	「修正する」部分を削除

	$INPUTS['STYPE'] = "キャンセル確定";					//	submitのvalue
	$INPUTS['MODE'] = "p_update";							//	modeパラメーター
	$INPUTS['ACTION'] = "p_cancel_start";					//	actionパラメーター
	$INPUTS['POINT'] = $total_point;						//	所有する変換可能ポイント
	$INPUTS['INPUTPOINT'] = $point;							//	ユーザー入力ポイント
	$INPUTS['CHENGETYPEL'] = $CHENGE_TYPE_L[$change_type];	//	変換種類
	$INPUTS['BANKNAME'] = $bank_name;						//	金融機関名
	$INPUTS['STORENAME'] = $store_name;						//	支店名
	$INPUTS['DEPOSIT'] = $DEPOSIT_L[$deposit];				//	振込先科目
	$INPUTS['ACCONUTNUM'] = $account_num;					//	振込先口座番号
	$INPUTS['ACCOUNTNAME'] = $account_name;					//	受取人名

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("pap_check.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return array($title,$html);

}

//--------------------------------------//
// ポイント変換変更内容のキャンセル処理 //
//--------------------------------------//
function p_cancel($af_num,&$ERROR){

	global	$PHP_SELF,				//	現在実行しているスクリプトのファイル名
			$bank_table,            //	bankテーブル
			$afuser_table,          //	afuserテーブル
			$appoint_table,         //	appointテーブル
			$application_table,     //	applicationテーブル
			$member_table,          //	kojinテーブル
			$set_point_mon,         //	アフェリエイトポイント確定時期	2013/12/06現在	1ヶ月
			$change_point,			//	返金可能ポイント
			$CHENGE_TYPE_L,         //	変換タイプ→array('','ネイバーズスポーツ割引ポイントに返還する。','ご指定の口座にお振り込みをする。');
			$CHENGE_TYPE2_L,        //	変換タイプ→array('','割引ポイント','口座にお振り込み');
			$DEPOSIT_L,             //	返金可能ポイント
			$DIR_CPT,               //	ポイント返還tempフォルダー
			$m_footer,              //	メールテンプレート
			$admin_mail_a,          //	affiliate@futboljersey.com
			$admin_name;            //	NEIGHBOURS SPORTS

	//	ロック：
	//	ポイント変換処理を行っている間はその他の申請を遮断する。
	//	申請が重複した時の二重登録を防止する。
	if (!$ERROR) {
		$lockdir = "$DIR_CPT/$af_num";
		lock($lockdir);
	}

	if ($_SESSION['BANK']) {
		$appli_num = $_SESSION['CANCEL']['appli_num'];
		$bank_num = $_SESSION['CANCEL']['bank_num'];
	}

	//	振込先削除
	if (!$ERROR && $bank_num > 0) {
		$sql  = "UPDATE $bank_table SET" .
				" state='1'" .
				" WHERE bank_num='$bank_num';";
		if (!$result = pg_exec(DB,$sql)) { $ERROR[] = "振込先情報を削除出来ませんでした。"; }
	}

	//	支払いテーブル削除
	if (!$ERROR) {
		$sql  = "UPDATE $application_table SET" .
				" state='2'" .
				" WHERE appli_num='$appli_num';";
		if (!$result = pg_exec(DB,$sql)) { $ERROR[] = "変換情報を削除出来ませんでした。"; }
	}

	//	メール配信
	if (!$ERROR) {	//	ユーザー情報抜き出し
		$sql  = "SELECT a.kojin_num, a.name_s, a.name_n, a.email" .
				" FROM $member_table a, $afuser_table b" .
				" WHERE a.kojin_num=b.kojin_num AND b.state!='1' AND b.af_num='$af_num' LIMIT 1;";
		if ($result = pg_query(DB,$sql)) {
			$list = pg_fetch_array($result);
			$kojin_num = $list['kojin_num'];
			$name_s = $list['name_s'];
			$name_n = $list['name_n'];
			$name = "$name_s $name_n";
			$email = $list['email'];
		}
		if (!$kojin_num || !$name_s || !$name_n || !$email) {
			$ERROR[] = "会員情報が確認出来ません。";
		}
	}

	if($ERROR){
		return;
	}else{
		//	受け取り
		//	件名
		$subject = "アフィリエイトポイント変換申請キャンセル (申請番号：{$appli_num})";

		$ip = getenv("REMOTE_ADDR");
		$host = gethostbyaddr($ip);
		if (!$host) { $host = $ip; }

		$msr = <<<WAKABA
{$name}様がアフィリエイトポイント変換申請キャンセルしました。

名前：{$name}
会員番号：{$kojin_num}
ID：{$af_num}
E-mail：{$email}

キャンセル申請番号：{$appli_num}
------------------------------------------------------
{$host} ($ip)

WAKABA;
//echo('店舗へ送信メール=>'.$msr."<br />");
		//	送信処理
		$send_email = $email;
		$send_name = "$name";
		$get_email = $admin_mail_a;
//$get_email = "検証用アドレス";
		send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msr);

		//	会員
		//	件名
		$subject = "アフィリエイトポイント変換申請キャンセル確認";

		//	メッセージ
		$msr = <<<WAKABA
{$name}様、ネイバーズスポーツアフィリエイトポイント変換申請キャンセル受付完了致しました。
申請した記憶がない場合はお手数ですがご連絡お願い致します。

名前：{$name}
会員番号：{$kojin_num}
ID：{$af_num}
E-mail：{$email}

キャンセル申請番号：{$appli_num}

それではこれからもよろしくお願い致します。

{$m_footer}

WAKABA;
//echo('顧客へ確認メール=>'.$msr."<br />");
		//	送信処理
		$send_email = $admin_mail_a;
		$send_name = $admin_name;
		$get_email = $email;
//$get_email = "検証用アドレス";
		send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msr);

	}

	//	保持したデータを削除
	if (!$ERROR) {
		unset($_SESSION['BANK']);
		unset($_SESSION['CANCEL']);
		unset($_SESSION['CHANGECHECK']);	// add 2013/12/27
	}

	//	ロック解除
	if (file_exists($lockdir)) {
		rmdir($lockdir);
	}

	header ("Location: ./template/thank_update.htm");
	exit;

}



//----	解約ページなし、保留します	yoshizawa 2013/12/09	----//
//	解約ページ
//function cansel($mode,$action,$method) {
//}
//--------------------------------------------------------------//

?>
