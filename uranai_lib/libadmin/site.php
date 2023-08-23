<?php
# db_test は データベース名に直すこと

/**
20150925 サーバのテーブルにカラム（future_days, past_days）を追加
*/
function site_listing() {
	global $conn;

	$sql = 'SELECT
		s1.site_id,
		s1.parent_id,
		s2.site_name as parent_name,
		s1.site_name,
		s1.url,
		s1.site_get_time,
		s1.is_execute,
		s1.site_topic,
		s1.topic_get_type,
		s1.site_id NOT IN (
			select distinct(site_id) from log where `is_delete`=0 AND `day` = CURRENT_DATE
		) AND s1.is_execute=1 AND s1.site_get_time<now() as batch,
		s1.site_id NOT IN (
			select distinct(site_id) from log_test where `is_delete`=0 AND `day` = CURRENT_DATE
		) AND s1.is_execute=1 AND s1.site_get_time<now() as batch_test,
		s1.site_id NOT IN (
			select distinct(site_id) from topic_log where `is_delete`=0 AND `day` = CURRENT_DATE
		) AND s1.is_execute=1 AND s1.site_topic =1 AND s1.site_get_time<now() as batch_topic
		FROM site as s1
		LEFT JOIN site as s2 ON s1.parent_id>0 AND s2.site_id=s1.parent_id
		WHERE s1.is_delete=0';
	//print $sql;
	$result = mysqli_query($conn, $sql);

	while ($data = mysqli_fetch_assoc($result)) {
		if ($data["url"] == "") {
			$data["url"] = "複数のURLが登録されています。";
		}
		$ret[] = $data;
	}

	return array("status" => "OK", "message" => "", "db" => $ret);
}


/**
 * site用のフォーム
 *
 * @author Azet
 * @return array
 */
function site_input() {
	return array("status" => "OK", "message" => "");
}


/**
 * サイト情報確認
 *
 * @author Azet
 * @param array $post
 */
function site_check($post) {

	$post["site_name"] = mb_convert_kana($post["site_name"], 'asKH');

	// ERROR test
	//$status = "ERR";

	$field_errors = array();

	if (!$post["site_name"]) {
		$status = "ERR";
		$field_errors['site_name'] = "サイト名が入力されていません。";
	}

	if ($post["parent_id"]) {
	   	if(!is_numeric($post['parent_id'])) {
			$status = "ERR";
			$field_errors['parent_id'] = "親IDは、半角数字で入力して下さい。";
		}
	}
	else {
		$post['parent_id'] = 0;
	}


	if (!$post["site_furigana"]) {
		$status = "ERR";
		$field_errors['site_furigana'] = "サイト名のフリガナが入力されていません。";
	}
	else {
		// 全角
		$furigana = mb_convert_kana($post['site_furigana'], 'KV', 'utf-8');
		if(!preg_match(KATAKANA_REG_MASK, $furigana)) {
			$status = "ERR";
			$field_errors['site_furigana'] = "サイト名のフリガナを<b>カタカナ</b>で入力してください。<br>句読点を使えません。";
		}
		else {
			$post['site_furigana'] = $furigana;
		}
	}

	if (!$post["site_get_time"]) {
		$status = "ERR";
		$field_errors["site_get_time"] = "取得時刻が入力されていません。";
	} elseif (!preg_match("/^([0-1][0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])$/", $post["site_get_time"])) {
		$status = "ERR";
		$field_errors["site_get_time"] = "取得時刻が異常です。";
	}

	//曜日チェックをはずすとmysqlエラー Incorrect integer value: '' for column 'site_get_weekXXX'となる件の対応
	//値がなくても0とする -> uranai_lib/libadmin/site.php L:270付近 -> site_saveではフィールドに値ありきの前提のためこっち側で対応
	$post['site_get_week0'] = $post['site_get_week0'] ?: 0;
	$post['site_get_week1'] = $post['site_get_week1'] ?: 0;
	$post['site_get_week2'] = $post['site_get_week2'] ?: 0;
	$post['site_get_week3'] = $post['site_get_week3'] ?: 0;
	$post['site_get_week4'] = $post['site_get_week4'] ?: 0;
	$post['site_get_week5'] = $post['site_get_week5'] ?: 0;
	$post['site_get_week6'] = $post['site_get_week6'] ?: 0;

	if(!$post['get_type']) {
		$status = 'ERR';
		$field_errors['get_type'] = "URLタイプが選択されていません。";
	}

	if ($post["get_type"] == 1) {
		// main URL
		if($post['url'] == '') {
			$status = 'ERR';
			$field_errors['url'] = "サイトのURLが入力されていません。";
		}
	}

	if ($post["get_type"] == 2) {
		//星座URL
		for ($i = 12; $i >= 1; $i--) {
			$star_url_field = "star${i}_url";
			if ($post[$star_url_field] == "") {
				$status = "ERR";
				$field_errors[$star_url_field] = "この星座のURLが入力されていません。";
			}
		}
	}

	if ($post["get_type"] == 3) {
		if($post['etc_url'] == '') {
			$status = 'ERR';
			$field_errors['etc_url'] = "その他URLの入力が入力されていません。";
		}
	}

	//全体URL / 星座ごと / その他 / 運勢ごとのラジオボタンのフォーム:
	// 未使用のようだが、INT型fieldに空文字列をいれようとするとmysqlのエラーになるため対応 >>>
	// 未選択のままきたら数値を入れてあげる
	if (!isset($post["topic_get_type"])) { //運勢データ取得URL
		$post["topic_get_type"] = 0;
	}
	 
	if (!isset($post["link_get_type"])) { //総合
		$post["link_get_type"] = 0;
	}
	 
	if (!isset($post["love_link_get_type"])) { //恋愛
		$post["love_link_get_type"] = 0;
	}
	// 何故か金運から先はこのデータを持たない(?) <<<


	// リンクURL
	// ===========================================
	// 後で使用！ (prepared for later! let it be!)
	// ===========================================
	//if($post['link_url'] == '') {
	//	$status = 'ERR';
	//	$field_errors['link_url'] = "リンクURLの入力が入力されていません。";
	//}

	// week check
	$check_count = 0;
	for($i=0; $i<=6; ++$i) {
		$field_name = 'site_get_week'.$i;
		$check_count += $post[$field_name];
	}
	if($check_count==0) {
		$status = 'ERR';
		$field_errors['site_get_week'] = "一つの日を選択してください。";
	}

	// past and future
	if($post['past_days']==='') {
		$status = 'ERR';
		$field_errors['past_days'] = '値が必要です。';
	}
	if($post['future_days']==='') {
		$status = 'ERR';
		$field_errors['future_days'] = '値が必要です。';
	}

	//limit_timeのチェック

	if(!preg_match('/^(2[0-4]|[0-1][0-9]):[0-5][0-9]:[0-5][0-9]$|^$/', $post['limit_time'])) {
		$status = 'ERR';
		$field_errors['limit_time'] = '時刻の値はHH:MM:SSまたはNULL（入力なし）で指定してください。';
	}

	// ========== final check ==========
	if($status==='ERR') {
		$message = "フォームのデータが間違っています。";
	}
	else {
		$status = 'OK';
	}

	$check = array(
		'error' => "登録が不可能です。下記のフィールドのデータを直してください。",
		'field_errors' => $field_errors
	);

	return array(
		"status" => $status,
		"message" => $message,
		"check" => $check,
		"db" => $post
	);
}


/**
 * サイトの情報を保存する
 * @author Azet
 * @param array $post
 */
function site_save($post) {
	global $conn;

	//$data = check_values($_POST, "サイトの情報を更新しました。");
	$data = site_check($post);
	if($data['status'] == 'OK') {
		//pre($data['db']);

		if($data['db']['site_id']) {
			$sql = "UPDATE ".DB_NAME.".site";
			$sql2 = "UPDATE ".DB_NAME.".link_url";
			$sql3 = "UPDATE ".DB_NAME.".link_love_url";
		}
		else {
			$sql = "INSERT INTO ".DB_NAME.".site";
			$sql2 = "INSERT INTO ".DB_NAME.".link_url";
			$sql3 = "INSERT INTO ".DB_NAME.".link_love_url";
		}

		$data['db']['limit_time'] = $data['db']['limit_time'] ? "\"" . $data['db']['limit_time']  . "\"" : "NULL";
		//limit_timeは空白をNULLでインサートするため値があるときのみクォートで囲む 2017/05/31 kimura


		$sql .= " SET
			parent_id = {$data['db']["parent_id"]},
			site_name = '{$data['db']["site_name"]}',
			site_furigana = '{$data['db']["site_furigana"]}',
			past_days = '{$data['db']["past_days"]}',
			future_days = '{$data['db']["future_days"]}',
			future_flag = '{$data['db']["future_flag"]}',
			updated_url = '{$data['db']["updated_url"]}',
			link_url = '{$data['db']["link_url"]}',
			limit_time = {$data['db']["limit_time"]},
			sp_link_url = '{$data['db']["sp_link_url"]}',
			get_type = '{$data['db']["get_type"]}',
			site_get_time = '{$data['db']["site_get_time"]}',
			site_get_week0 = '{$data['db']['site_get_week0']}',
			site_get_week1 = '{$data['db']['site_get_week1']}',
			site_get_week2 = '{$data['db']['site_get_week2']}',
			site_get_week3 = '{$data['db']['site_get_week3']}',
			site_get_week4 = '{$data['db']['site_get_week4']}',
			site_get_week5 = '{$data['db']['site_get_week5']}',
			site_get_week6 = '{$data['db']['site_get_week6']}',
			url = '{$data['db']["url"]}',
			sp_url = '{$data['db']["sp_url"]}',
			star1_url = '{$data['db']["star1_url"]}',
			star2_url = '{$data['db']["star2_url"]}',
			star3_url = '{$data['db']["star3_url"]}',
			star4_url = '{$data['db']["star4_url"]}',
			star5_url = '{$data['db']["star5_url"]}',
			star6_url = '{$data['db']["star6_url"]}',
			star7_url = '{$data['db']["star7_url"]}',
			star8_url = '{$data['db']["star8_url"]}',
			star9_url = '{$data['db']["star9_url"]}',
			star10_url = '{$data['db']["star10_url"]}',
			star11_url = '{$data['db']["star11_url"]}',
			star12_url = '{$data['db']["star12_url"]}',
			sp_star1_url = '{$data['db']["sp_star1_url"]}',
			sp_star2_url = '{$data['db']["sp_star2_url"]}',
			sp_star3_url = '{$data['db']["sp_star3_url"]}',
			sp_star4_url = '{$data['db']["sp_star4_url"]}',
			sp_star5_url = '{$data['db']["sp_star5_url"]}',
			sp_star6_url = '{$data['db']["sp_star6_url"]}',
			sp_star7_url = '{$data['db']["sp_star7_url"]}',
			sp_star8_url = '{$data['db']["sp_star8_url"]}',
			sp_star9_url = '{$data['db']["sp_star9_url"]}',
			sp_star10_url = '{$data['db']["sp_star10_url"]}',
			sp_star11_url = '{$data['db']["sp_star11_url"]}',
			sp_star12_url = '{$data['db']["sp_star12_url"]}',
			etc_url = '{$data['db']["etc_url"]}',
			`comment` = '{$data['db']["comment"]}',
			is_execute = '{$data['db']["is_execute"]}',
			site_topic = '{$data['db']["site_topic"]}',
			topic_get_type = '{$data['db']["topic_get_type"]}',
			is_delete = 0,
			date_delete = NULL,
			date_update = CURRENT_TIMESTAMP
		";
		if($data['db']['site_id']) {
			$sql .= " WHERE site_id = '{$data['db']['site_id']}'";
		}
		$result = mysqli_query($conn, $sql);  // $conn->query($sql) と同じ
		$line_id ="";
		if(!$data['db']['site_id']) {
			$line_id = $conn->insert_id; // 追加されたレコードのID
		}
		$sql2 .= " SET ";
		if(!$data['db']['site_id']) {
				$sql2 .= " site_id = $line_id,";
		}
		$sql2 .= "
			all_link_url = '{$data['db']["all_link_url"]}',
			all_sp_link_url = '{$data['db']["all_sp_link_url"]}',
			link_get_type = '{$data['db']["link_get_type"]}',
			star1_link_url = '{$data['db']["star1_link_url"]}',
			star2_link_url = '{$data['db']["star2_link_url"]}',
			star3_link_url = '{$data['db']["star3_link_url"]}',
			star4_link_url = '{$data['db']["star4_link_url"]}',
			star5_link_url = '{$data['db']["star5_link_url"]}',
			star6_link_url = '{$data['db']["star6_link_url"]}',
			star7_link_url = '{$data['db']["star7_link_url"]}',
			star8_link_url = '{$data['db']["star8_link_url"]}',
			star9_link_url = '{$data['db']["star9_link_url"]}',
			star10_link_url = '{$data['db']["star10_link_url"]}',
			star11_link_url = '{$data['db']["star11_link_url"]}',
			star12_link_url = '{$data['db']["star12_link_url"]}',
			sp_star1_link_url = '{$data['db']["sp_star1_link_url"]}',
			sp_star2_link_url = '{$data['db']["sp_star2_link_url"]}',
			sp_star3_link_url = '{$data['db']["sp_star3_link_url"]}',
			sp_star4_link_url = '{$data['db']["sp_star4_link_url"]}',
			sp_star5_link_url = '{$data['db']["sp_star5_link_url"]}',
			sp_star6_link_url = '{$data['db']["sp_star6_link_url"]}',
			sp_star7_link_url = '{$data['db']["sp_star7_link_url"]}',
			sp_star8_link_url = '{$data['db']["sp_star8_link_url"]}',
			sp_star9_link_url = '{$data['db']["sp_star9_link_url"]}',
			sp_star10_link_url = '{$data['db']["sp_star10_link_url"]}',
			sp_star11_link_url = '{$data['db']["sp_star11_link_url"]}',
			sp_star12_link_url = '{$data['db']["sp_star12_link_url"]}'
		";
		$sql3 .= " SET ";
			if(!$data['db']['site_id']) {
				$sql3 .= " site_id = $line_id,";
			}
			$sql3 .= "link_love_url = '{$data['db']["link_love_url"]}',
			sp_link_love_url = '{$data['db']["sp_link_love_url"]}',
			love_link_get_type = '{$data['db']["love_link_get_type"]}',
			star1_link_love_url = '{$data['db']["star1_link_love_url"]}',
			star2_link_love_url = '{$data['db']["star2_link_love_url"]}',
			star3_link_love_url = '{$data['db']["star3_link_love_url"]}',
			star4_link_love_url = '{$data['db']["star4_link_love_url"]}',
			star5_link_love_url = '{$data['db']["star5_link_love_url"]}',
			star6_link_love_url = '{$data['db']["star6_link_love_url"]}',
			star7_link_love_url = '{$data['db']["star7_link_love_url"]}',
			star8_link_love_url = '{$data['db']["star8_link_love_url"]}',
			star9_link_love_url = '{$data['db']["star9_link_love_url"]}',
			star10_link_love_url = '{$data['db']["star10_link_love_url"]}',
			star11_link_love_url = '{$data['db']["star11_link_love_url"]}',
			star12_link_love_url = '{$data['db']["star12_link_love_url"]}',
			sp_star1_link_love_url = '{$data['db']["sp_star1_link_love_url"]}',
			sp_star2_link_love_url = '{$data['db']["sp_star2_link_love_url"]}',
			sp_star3_link_love_url = '{$data['db']["sp_star3_link_love_url"]}',
			sp_star4_link_love_url = '{$data['db']["sp_star4_link_love_url"]}',
			sp_star5_link_love_url = '{$data['db']["sp_star5_link_love_url"]}',
			sp_star6_link_love_url = '{$data['db']["sp_star6_link_love_url"]}',
			sp_star7_link_love_url = '{$data['db']["sp_star7_link_love_url"]}',
			sp_star8_link_love_url = '{$data['db']["sp_star8_link_love_url"]}',
			sp_star9_link_love_url = '{$data['db']["sp_star9_link_love_url"]}',
			sp_star10_link_love_url = '{$data['db']["sp_star10_link_love_url"]}',
			sp_star11_link_love_url = '{$data['db']["sp_star11_link_love_url"]}',
			sp_star12_link_love_url = '{$data['db']["sp_star12_link_love_url"]}'
		";

		if($data['db']['site_id']) {
			$sql2 .= " WHERE site_id = '{$data['db']['site_id']}'";
			$sql3 .= " WHERE site_id = '{$data['db']['site_id']}'";
		}
		//pre($sql);
		//pre($sql2);
		//pre($sql3);
		$result2 = mysqli_query($conn, $sql2);
		$result3 = mysqli_query($conn, $sql3);

		if($result) {
			$status = 'OK';
			$message = "サイトの情報を保存されました。";

		}
		else {
			$status = 'ERR';
			$message = "DBに保存した時に、エラーなりました。";
			$message.= "(エラー：" . mysqli_error($conn) . ")";
		}
		if($result2) {
			$status2 = 'OK';
			$message2 = "リンクURL(総合)の情報を保存されました。";
		}
		else {
			$status2 = 'ERR';
			$message2 = "リンクURL(総合)の情報を保存した時に、エラーなりました。";
			$message2.= "(エラー：" . mysqli_error($conn) . ")";
		}
		if($result3) {
			$status3 = 'OK';
			$message3 = "リンクURL(恋愛)の情報を保存されました。";
		}
		else {
			$status3 = 'ERR';
			$message3 = "リンクURL(恋愛)の情報を保存した時に、エラーなりました。";
			$message3.= "(エラー：" . mysqli_error($conn) . ")";
		}
		if($status == 'OK' && $status2 == 'OK' && $status3 == 'OK'){
			$status ='OK';
		}else{
			$status ='ERR';
		}
	}
	else {
		$status = $data['status'];
		$message = $data["message"];
	}

	return array("status" => $status, "message" => $message.$message2.$message3);
}


/**
 * siteを削除確認画面
 *
 * @author Azet
 * @param int $id
 * @return array
 */
function site_delete($id) {
	return array("status" => "OK", "message" => "", "site_id" => $id);
}


/**
 * siteを削除処理
 *
 * @author Azet
 * @param int $id
 * @return array
 */
function site_delete_do($id) {
	global $conn;

	$sql = "UPDATE ".DB_NAME.".site SET is_delete=1, date_delete=CURRENT_TIMESTAMP WHERE site_id='{$id}';";
	$result = mysqli_query($conn, $sql);
	$status = "OK";
	$message = "サイトを削除しました。";
	if (!$result) {
		if (DEBUG) {
		    $message = "ERROR<br>" . mysqli_error($conn);
		} else {
			$message = "サイトの削除に失敗しました。";
		}
	}

	return array("status" => $status, "message" => $message);
}


/**
 * サイト情報を読み込み
 *
 * @author Azet
 * @param int $id
 * @return array (dbはサイトの情報)
 */
function site_edit($id) {
	global $conn;
	$sql = "SELECT
		*,
		sd.description AS site_description,
		sd.presentation AS site_presentation,
		sd.visible AS site_detail_visible
		FROM site AS s
		JOIN link_url AS url ON s.site_id = url.site_id
		LEFT JOIN site_details sd ON s.site_id = sd.site_id
		JOIN link_love_url as lurl ON s.site_id = lurl.site_id
		WHERE s.site_id = '$id'";
	$result = mysqli_query($conn, $sql);
	//print $sql;
	// TODO サイトを見つからない場合は？
	$data = mysqli_fetch_assoc($result);
	//print_r($data);
	return array("status" => "OK", "message" => "", "db" => $data);
}


/**
 * cronを起動する
 * @author Azet
 * @param int $id_
 * @return array
 */
function batch_run($id_) {

	// どの環境でも使えるコマンド
//	$cmd = PHP_BIN.' '.dirname(__FILE__).'/../bat/index.php --site '.$id_.' --now';
	$cmd = PHP_BIN.' '.dirname(__FILE__).'/../bat/index_topic_test.php --site '.$id_.' --now';
	exec($cmd, $output);

	$data = array(
		'status' => 'OK'
		//,'message' => 'バチを起動できました。'	//del okabe 2016/04/04
		,'message' => 'データ取得バッチを実行しました。'	//add okabe 2016/04/04
		,'command' => $cmd
		,'output' => print_r($output, true)
	);

	return $data;
}


// 未使用？ならば削除しよう！ シモン 2018-10-01
//testバッチ用コマンド
function batch_run_test($id_) {

	// どの環境でも使えるコマンド
	$cmd = dirname(__FILE__).'/../bat/index.php --site '.$id_.' --now';
	//$cmd = PHP_BIN.' '.dirname(__FILE__).'/../bat/index_topic_test.php --site '.$id_.' --now';
	exec($cmd, $output);

	$data = array(
		'status' => 'OK'
		//,'message' => 'バチを起動できました。'	//del okabe 2016/04/04
		,'message' => 'データ取得バッチを実行しました。'	//add okabe 2016/04/04
		,'command' => $cmd
		,'output' => print_r($output, true)
	);

	return $data;
}


function batch_Topic_run($id_) {

	//$cmd = dirname(__FILE__).'/../bat/index.php --site '.$id_.' --now';
	// どの環境でも使えるコマンド
	$cmd = PHP_BIN.' '.dirname(__FILE__).'/../bat/topic.php --site '.$id_.' --now';
	exec($cmd, $output);

	$data = array(
		'status' => 'OK'
		//,'message' => 'バチを起動できました。'	//del okabe 2016/04/04
		,'message' => 'データ取得バッチを実行しました。'	//add okabe 2016/04/04
		,'command' => $cmd
		,'output' => print_r($output, true)
	);

	return $data;
}
