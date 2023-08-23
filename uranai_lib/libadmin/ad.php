<?php

class Ad {
	static public $period_not_over_cond =  "ad_date_end>0 AND ad_date_end<CURRENT_TIMESTAMP";

	// ids of displayed ads
	static private $uniq_history = array();

	/**
	 * IDから広告のデータを取得
	 *
	 * @param int $id
	 * @return array (広告のレコード）
	 */
	static function getById($id) {
		global $conn;

		$sql = "SELECT
			ad_id,
			ad_tag,
			ad_tag_mobile,
			ad_tag_Android,
			ad_tag_iOS
		FROM ad
		WHERE date_delete IS NULL
		AND NOT (".self::$period_not_over_cond.")
		AND ad_is_show = 1
		AND ad_id IN ( $id )
		";

		$rs = mysqli_query($conn, $sql);

		if($rs) {
			while($row = mysqli_fetch_assoc($rs)){
				$ad[] = $row;
			}
			return $ad;
		}
		return null;
	}

	static function getGroupById($id) {
		global $conn;

		$sql = "SELECT
			ad.ad_id,
			ad.ad_tag,
			ad.ad_tag_mobile,
			ad.ad_tag_Android,
			ad.ad_tag_iOS
		FROM ad_group ag
		INNER JOIN ad_group_ad aga ON ag.ad_group_id = aga.ad_group_id
		INNER JOIN ad ON aga.ad_id = ad.ad_id
		WHERE ad.date_delete IS NULL
		AND ag.date_delete IS NULL
		AND ag.ad_group_id = {$id}
		AND NOT ( ad.ad_date_end>0 AND ad.ad_date_end<CURRENT_TIMESTAMP)
		AND ad.ad_is_show = 1
		";

		$rs = mysqli_query($conn, $sql);

		if($rs) {
			while($row = mysqli_fetch_assoc($rs)){
				$ad[] = $row;
			}
			return $ad;
		}
		return null;
	}

	
	/**
	 * Creating a random Ad
	 * @author Azet
	 * @param bool $uniq_
	 * @return array (ad record)
	 */
	static function random($uniq_=false) {
		global $conn;
		$unique_cond = '';

		// 独特の条件
		if($uniq_) {
			if(count(self::$uniq_history)>0) {
				$ids = join(', ', self::$uniq_history);
				//pre($ids);
				$unique_cond = " AND ad_id NOT IN ($ids)";
			}
		}

		// random listing
		$sql = "SELECT
			ad_id,
			ad_name,
			ad_tag,
			ad_tag_mobile
		FROM ad
		WHERE date_delete IS NULL
		AND NOT (".self::$period_not_over_cond.")
		AND ad_is_show = 1
		$unique_cond
		ORDER BY count_display, rand()
		LIMIT 1
		";

		$rs = mysqli_query($conn, $sql);
		//pre($sql);
		if($rs) {
			$ad = mysqli_fetch_assoc($rs);
			self::$uniq_history[] = $ad['ad_id'];
			return $ad;
		}

		// error?
		return null;
	}


	/**
	 * TEST用の広告
	 *
	 * @author Azet
	 * @return array (ad record)
	 */
	static function test() {
		global $conn;

		// random listing
		$sql = "SELECT
			ad_id,
			ad_name,
			ad_tag,
			ad_tag_mobile
		FROM ad
		WHERE date_delete IS NULL
		AND ad_name='TEST'
		LIMIT 1
		";

		$rs = mysqli_query($conn, $sql);
		if($rs) {
			$ad = mysqli_fetch_assoc($rs);
			return $ad;
		}

		// error?
		return null;
	}


	/**
	 * counting ad displays
	 * @author Azet
	 * @param int $ad_id_
	 * @return bool
	 */
	static function countDisplay($ad_id_) {
		global $conn, $BOTS;

		// bot以外
		//print $_SERVER['REMOTE_HOST'];
		if(preg_match('/'.$BOTS['REMOTE_HOST'].'/', $_SERVER['REMOTE_HOST'])
			|| preg_match('/.'.$BOTS['HTTP_USER_AGENT'].'/', $_SERVER['HTTP_USER_AGENT'])) {
			return false;
		}

		// 更新 
		$sql = "UPDATE ad
			SET count_display = count_display + 1
			WHERE ad_id = $ad_id_
			LIMIT 1";
		//pre($sql);

		$ok = mysqli_query($conn, $sql);

		return $ok;
	}


	/**
	 * counting ad clicks
	 * @author Azet
	 * @param int $ad_id_
	 * @return bool
	 */
	static function countClick($ad_id_) {
		global $conn;

		$sql = "UPDATE ad
			SET count_click = count_click + 1
			WHERE ad_id = $ad_id_
			LIMIT 1";
		//pre($sql);

		$ok = mysqli_query($conn, $sql);

		return $ok;
	}

}

/**
 * 広告一覧
 * @author Azet
 * @param string $filter_:
 *  - active
 *  - off
 *  - over
 * @return array
 */
function ad_listing($filter_='active') {
	global $conn;
	$filter = "";
	$period_not_over_cond = Ad::$period_not_over_cond;

	// filter patterns
	if($filter_=='active') {
		$filter .= " AND ad_is_show=1";
		$filter .= " AND not($period_not_over_cond)";
	}

	if($filter_=='off') {
		$filter .= " AND ad_is_show=0";
	}

	if($filter_=='over') {
		$filter .= " AND $period_not_over_cond";
	}

	$sql = "SELECT
			ad_id,
			ad_is_show,
			ad_name,
			ad_date_begin,
			ad_date_end,
			count_display,
			count_click,
			IF($period_not_over_cond, 1, 0) as is_over
		FROM ad
		WHERE date_delete IS NULL";
	// append filters
	$sql .= $filter;
	$result = mysqli_query($conn, $sql);
	//pre($sql);

	while ($data = mysqli_fetch_assoc($result)) {
		/*if ($data["url"] == "") {
			$data["url"] = "複数のURLが登録されています。";
		}*/
		$ret[] = $data;
	}

	return array("status" => "OK", "db" => $ret, 'filter' => $filter_);
}


/**
 * 変更
 * @author Azet
 * @param int $id
 * @return array
 */
function ad_edit($id) {
	global $conn;

	$sql = "SELECT * FROM ad WHERE ad_id = '$id';";
	$result = mysqli_query($conn, $sql);
	$data = mysqli_fetch_assoc($result);
	//pre($data);

	return array("status" => "OK", "message" => "", "db" => $data);
}


/**
 * 広告確認
 * @author Azet
 * @param array $post
 * @return array [status, message, check, db]
 */
function ad_check($post) {
	$message = "";
	$field_errors = array();

	// NAME
	$post["ad_name"] = mb_convert_kana($post["ad_name"], 'asKH');
	if(!$post['ad_name']) {
		$status = 'ERR';
		$field_errors['ad_name'] = "広告名が入力されません。";
	}

	// date check
	if($_POST['ad_date_begin']) {
		if(!preg_match("/^(\d{4}-\d{2}-\d{2}) ([0-1][0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])$/", $_POST['ad_date_begin'])) {
			$status = 'ERR';
			$field_errors['ad_date_begin'] = "開始日時の値は間違っています。";
		}
	}
	if($_POST['ad_date_end']) {
		if(!preg_match("/^(\d{4}-\d{2}-\d{2}) ([0-1][0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])$/", $_POST['ad_date_end'])) {
			$status = 'ERR';
			$field_errors['ad_date_end'] = "開始日時の値は間違っています。";
		}
	}

	// code/tag check
	if(!$post['ad_tag']) {
		$status = 'ERR';
		$field_errors['ad_tag'] = "コメントが入力されていません。";
	}
	
	// code/tag check
	if(!$post['ad_tag_mobile']) {
		$status = 'ERR';
		$field_errors['ad_tag_mobile'] = "コメントが入力されていません。";
	}
	
	// code/tag check
	if(!$post['ad_tag_Android']) {
		$status = 'ERR';
		$field_errors['ad_tag_Android'] = "コメントが入力されていません。";
	}
	
	// code/tag check
	if(!$post['ad_tag_iOS']) {
		$status = 'ERR';
		$field_errors['ad_tag_iOS'] = "コメントが入力されていません。";
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
 * 広告削除
 * @author Azet
 * @param int $id
 * @return array
 */
function ad_delete($id) {
	return array("status" => "OK", "message" => "", "ad_id" => $id);
}


/**
 * 広告削除処理
 * @author Azet
 * @param int $id
 * @return array
 */
function ad_delete_do($id) {
	global $conn;

	$sql = "UPDATE ".DB_NAME.".ad SET date_delete=CURRENT_TIMESTAMP WHERE ad_id='{$id}';";
	$result = mysqli_query($conn, $sql);
	$status = "OK";
	$message = "広告を削除しました。";
	if (!$result) {
		if (DEBUG) {
		    $message = "ERROR<br>" . mysqli_error($conn);
		} else {
			$message = "広告の削除に失敗しました。";
		}
	}

	return array("status" => $status, "message" => $message);
}


/**
 * 広告保存
 * @author Azet
 * @param array $post
 * @return array
 */
function ad_save($post) {
	global $conn;

	$data = ad_check($post);
	if($data['status'] == 'OK') {
		//pre($data['db']);

		if($data['db']['ad_id']) {
			$sql = "UPDATE ".DB_NAME.".ad";
		}
		else {
			$sql = "INSERT INTO ".DB_NAME.".ad";
		}

		$sql .= " SET
			ad_is_show = '{$data['db']["ad_is_show"]}',
			ad_date_begin = '{$data['db']["ad_date_begin"]}',
			ad_date_end = '{$data['db']["ad_date_end"]}',
			ad_tag = '{$data['db']["ad_tag"]}',
			ad_tag_mobile = '{$data['db']["ad_tag_mobile"]}',
			ad_tag_Android = '{$data['db']["ad_tag_Android"]}',
			ad_tag_iOS = '{$data['db']["ad_tag_iOS"]}',
			ad_name = '{$data['db']["ad_name"]}',
			`comment` = '{$data['db']["comment"]}',
			date_delete = NULL,
			date_update = CURRENT_TIMESTAMP
		";
		
		if($data['db']['ad_id']) {
			$sql .= " WHERE ad_id = '{$data['db']['ad_id']}'";
		}

		//pre($sql);
		$result = mysqli_query($conn, $sql);

		if($result) {
			$status = 'OK';
			$message = "広告の情報を保存されました。";
		}
		else {
			$status = 'ERR';
			$message = "DBに保存した時に、エラーなりました。";
		}
	}
	else {
		$status = $data['status'];
		$message = $data["message"];
	}

	return array("status" => $status, "message" => $message);
}

//---------------------------------------------
//広告グループ
//---------------------------------------------
/**
 * 広告グループ一覧
 * 
 * @access public
 * @return void
 */
function ad_group_listing() {

	global $conn;

	$sql = "SELECT ad_group_id,ad_group_name FROM ad_group WHERE date_delete IS NULL";

	$result = mysqli_query($conn, $sql);
	while ($data = mysqli_fetch_assoc($result)) {
		$ret[] = $data;
	}

	return array("status" => "OK", "db" => $ret);
}

function grep_ad(){

	$res = array();

	// $cmd = "C:\cygwin64\bin\bash.exe --login  -c \"grep 'insert.*ad_group' -rn d:/workspace/uranai/uranai_lib/templates/user\" ";
	$cmd = "grep 'insert.*ad_group' -rn ".dirname(__FILE__)."/../templates/user ";
	$output = shell_exec($cmd);

	$lines = explode("\n", $output);
	$lines = array_map("htmlspecialchars", $lines);
	foreach($lines as $k => $l){
		$lines[$k] = preg_replace("/^.*user\/(.*.tpl):/", "$1:", $l);
	}
	array_pop($lines);

	foreach($lines as $k => $l){
		preg_match("/(.*tpl):(\d+):.*id=&quot;(\d+)&quot;/", $l, $m);
		$res[$m[3]][$m[1]][] = $m[2];
	}
	ksort($res);
	return $res;
}

/**
 * グループ変更
 * @author Azet
 * @param int $id
 * @return array
 */
function ad_group_edit($id) {
	global $conn;

	$sql = "SELECT a.ad_group_name, b.ad_id";
	$sql.= " FROM ad_group a";
	$sql.= " INNER JOIN ad_group_ad b ON a.ad_group_id = b.ad_group_id";
	$sql.= " WHERE a.ad_group_id = '".$id."'";

	$data = null;
	$result = mysqli_query($conn, $sql);

	$data['ad_group_id'] = $id;

	if($result) {
		//グループ名だけ取得
		$row = mysqli_fetch_assoc($result);
		$data['ad_group_name'] = $row['ad_group_name'];

		//ポインタもどす
		mysqli_data_seek($result, 0);

		while($row = mysqli_fetch_assoc($result)){
			$data['ad_ids'][] = $row['ad_id'];
		}
	}

	return array("status" => "OK", "message" => "", "db" => $data);
}

/**
 * 広告グループ確認
 * @author Azet
 * @param array $post
 * @return array [status, message, check, db]
 */
function ad_group_check($post) {
	$message = "";
	$field_errors = array();

	// NAME
	$post["ad_group_name"] = mb_convert_kana($post["ad_group_name"], 'asKH');
	if(!$post['ad_group_name']) {
		$status = 'ERR';
		$field_errors['ad_group_name'] = "広告グループ名が入力されていません。";
	}

	if(empty($post['ad_ids'])) {
		$status = 'ERR';
		$field_errors['ad_ids'] = "広告が選択されていません。";
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
 * 広告グループ保存
 * @author Azet
 * @param array $post
 * @return array
 */
function ad_group_save($post) {
	global $conn;

	$data = ad_group_check($post);
	if($data['status'] == 'OK') {

		//グループマスタ更新
		if(isset($data['db']['ad_group_id']) && !empty($data['db']['ad_group_id'])) {
			$sql = "UPDATE ".DB_NAME.".ad_group";
		}
		else {
			$sql = "INSERT INTO ".DB_NAME.".ad_group";
		}

		$sql .= " SET
			ad_group_name = '{$data['db']["ad_group_name"]}',
			date_delete = NULL,
			date_update = CURRENT_TIMESTAMP
		";
		
		if(isset($data['db']['ad_group_id']) && !empty($data['db']['ad_group_id'])) {
			$sql .= " WHERE ad_group_id = '{$data['db']['ad_group_id']}'";
		}
		$result = mysqli_query($conn, $sql);

		unset($sql);
		//グループ所属情報更新
		if(isset($data['db']['ad_group_id']) && !empty($data['db']['ad_group_id'])) {
			$ad_group_id = $data['db']["ad_group_id"];
		} else {
			$ad_group_id = mysqli_insert_id($conn);
		}

		$sql = "SELECT ad_id";
		$sql.= " FROM ad_group_ad";
		$sql.= " WHERE ad_group_id = '".$ad_group_id."'";

		$current_ad_ids = array();
		if($res = mysqli_query($conn, $sql)){
			while($row = mysqli_fetch_assoc($res)){
				$current_ad_ids[] = $row['ad_id'];
			}
		}
		unset($row, $res);

		foreach($data['db']['ad_ids'] as $ad_id){

			//1回ずつselectで存在チェック...
			$sql = "SELECT *";
			$sql.= " FROM ad_group_ad";
			$sql.= " WHERE 1";
			$sql.= "  AND ad_group_id = '".$ad_group_id."'";
			$sql.= "  AND ad_id = '".$ad_id."'";
			$rows = 0;
			if($rs = mysqli_query($conn, $sql)){
				$rows = mysqli_num_rows($rs);
			}

			//更新 or 作成
			unset($sql);
			if($rows > 0){
				$sql = "UPDATE ".DB_NAME.".ad_group_ad";
			} else {
				$sql = "INSERT INTO ".DB_NAME.".ad_group_ad";
			}

			$sql.= " SET";
			$sql.= " ad_group_id = '{$ad_group_id}',";
			$sql.= " ad_id = '{$ad_id}',";
			$sql.= " date_update = CURRENT_TIMESTAMP";

			if($rows > 0){
				$sql.= " WHERE ad_group_id = '{$ad_group_id}'";
				$sql.= " AND ad_id = '{$ad_id}'";
			}

			$result2 = mysqli_query($conn, $sql);
		}

		//登録済みだったがアンチェックされたものは削除
		if(is_array($current_ad_ids) && count($current_ad_ids) > 0){
			$deleted = array_diff($current_ad_ids, $data['db']['ad_ids']);
		}

		//関連物理削除
		if(is_array($deleted) && count($deleted) > 0){
			$sql = "DELETE FROM ".DB_NAME.".ad_group_ad";
			$sql.= " WHERE ad_id IN (".implode(",", $deleted).")";
			$result3 = mysqli_query($conn, $sql);
		}else{
			$result3 = true;
		}
		//---

		if($result && $result2 && $result3) {
			$status = 'OK';
			$message = "広告グループの情報が保存されました。";
		}
		else {
			$status = 'ERR';
			$message = "DBに保存した時に、エラーなりました。";
		}
	}
	else {
		$status = $data['status'];
		$message = $data["message"];
	}

	return array("status" => $status, "message" => $message);
}


/**
 * 広告グループ削除
 * @author Azet
 * @param int $id
 * @return array
 */
function ad_group_delete($id) {
	return array("status" => "OK", "message" => "", "ad_group_id" => $id);
}

/**
 * 広告グループ削除処理
 * @author Azet
 * @param int $id
 * @return array
 */
function ad_group_delete_do($id) {
	global $conn;

	//マスタ無効化
	$sql = "UPDATE ".DB_NAME.".ad_group SET date_delete=CURRENT_TIMESTAMP WHERE ad_group_id='{$id}';";
	$result = mysqli_query($conn, $sql);

	//関連物理削除
	$sql = "DELETE FROM ".DB_NAME.".ad_group_ad";
	$sql.= " WHERE ad_group_id='{$id}'";

	$result = mysqli_query($conn, $sql);
	$status = "OK";
	$message = "広告グループを削除しました。";
	if (!$result) {
		if (DEBUG) {
		    $message = "ERROR<br>" . mysqli_error($conn);
		} else {
			$message = "広告グループの削除に失敗しました。";
		}
	}

	return array("status" => $status, "message" => $message);
}

/**
 * 全広告IDと名前取得
 * 
 * @return array
 */
function get_all_ad_names(){

	global $conn;
	$data = null;

	$sql = "SELECT ad_id, ad_name";
	$sql.= " FROM ad ";
	$sql.= " WHERE date_delete IS NULL";

	$result = mysqli_query($conn, $sql);

	if($result) {
		while($row = mysqli_fetch_assoc($result)){
			$data[$row['ad_id']] = $row['ad_name'];
		}
	}
	return $data;
}

/**
 * 全広告IDと名前取得
 * 
 * @return array
 */
function get_ad_names($id_list){

	global $conn;
	$data = null;

	$sql = "SELECT ad_id, ad_name";
	$sql.= " FROM ad ";
	$sql.= " WHERE date_delete IS NULL";
	$sql.= " AND ad_id IN (".implode(",", $id_list).")";

	$result = mysqli_query($conn, $sql);

	if($result) {
		while($row = mysqli_fetch_assoc($result)){
			$data[$row['ad_id']] = $row['ad_name'];
		}
	}
	return $data;
}
