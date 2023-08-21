<?php

/**
 * サイトデータからどのリンクを使えばいいか判断する機能
 *
 * @author Azet
 * @param array $data_ [link_url, etc_url] キーが必要！
 * @return string
 */
function site_link_decide($data_)
{
	$link_url = '';

	if ($data_['link_url'] != "") {
		// link_urlはメインリンクデータ
		$link_url = $data_['link_url'];
	} elseif ($data_['etc_url'] != "") {
		// etc_urlは他のリンクデータ（メインリンクがない時に）
		$link_url = $data_['etc_url'];
	}

	return $link_url;
}


/**
 * サイトへのリンクが日付に対して有効期限内かどうかチェックする
 *
 * @param string $day (20170530)
 * @param string $stmt (Dateintervalオブジェクトの間隔パラメータ 0Dなど)
 * @param string $limit (18:00:00)
 * @return bool
 */

function is_show($day, $stmt, $limit)
{

	if ($stmt == "") {
		$stmt = "0D";
	}

	$prefix = "P";
	$date = new DateTime(); // 現在日
	$date->sub(new DateInterval($prefix . $stmt));

	#	$date->add(new DateInterval($prefix . '1D'));
	$limit_prev_date = $date->format('Ymd'); // 参照限界日

	if ($limit) { //時間設定有りの場合
		$hour = $date->format('H:i:s'); //今現在の時間
		if ($day == $limit_prev_date && $hour < $limit) { //現在時刻が限界時間内だったら
			return TRUE;
		} else {
			return FALSE;
		}
	}

	if ($day >= $limit_prev_date) {	// 変数は順序関係を保っている
		return TRUE;	// 表示する
	}
	return FALSE;
}

/**
 * debug用
 * 値を出す機能
 * @author Azet
 * @param mixed $var_
 */
function pre($var_)
{
	print "<pre>\n";
	if (is_string($var_)) {
		print htmlspecialchars($var_);
	} else {
		print_r($var_);
	}
	print "</pre>\n";
}

/**
 * find a star number from a date (ISO)
 *
 * @author Azet
 * @param string $bd_
 * @return string "" if not found
 */
function getStarFromBirthday($bd_)
{
	global $star_dates;
	$found = "";

	$bd_noyear = substr($bd_, 5);
	foreach ($star_dates as $k => $v) {
		//print "{$v['from']} <= $user_date_noyear <= {$v['to']}";
		if ($k == '12') {
			// irregular pattern
			if (
				$v['from'] <= $bd_noyear && $bd_noyear <= '12-31' ||
				'01-01' <= $bd_noyear && $bd_noyear <= $v['to']
			) {
				$found = $k;
				break;
			}
		} else {
			if ($v['from'] <= $bd_noyear && $bd_noyear <= $v['to']) {
				$found = $k;
				break;
			}
		}
	}

	return $found;
}


/**
 * 前日の日付の計算
 *
 * @author Azet
 * @param string $date_ (例： 2017-03-17)
 * @return string (例： 2017-03-16)
 */
function previous_date($date_)
{
	return date('Ymd', strtotime($date_ . ' -1 day'));
}


/**
 * 次日の日付の計算
 *
 * @author Azet
 * @param string $date_ (例： 2017-03-17)
 * @return string (例： 2017-03-18)
 */
function next_date($date_)
{
	$next_date = date('Ymd', strtotime($date_ . ' +1 day'));

	if ($next_date > date("Ymd")) {
		$next_date = "";
	}

	return $next_date;
}
/**
 * 前月の日付の計算
 *
 * @author Azet
 * @param string $date_ (例： 2017-03-17)
 * @return string (例： 2017-02-01)
 */
function previous_month_date($date_, $data_type_ = NULL)
{
	//print ($data_type_.aaaa);
	$date = date('Y-m-d', strtotime("$date_"));
	preg_match('/^([0-9]{4}-[0-9]{2})-[0-9]{2}$/', $date, $match);
	$back_day = date('Ymd', strtotime("first day of $match[1] -1 month"));
	$min_month = substr(PREV_DATE_DTL, 0, 8);
	$min_month = date('Ymd', strtotime("first day of $min_month -1 month"));
	$min_month_m = substr(PREV_DATE_DTL_M, 0, 8);
	$min_month_m = date('Ymd', strtotime("first day of $min_month_m -1 month"));
	//print $min_month.$back_day;
	if ($data_type_ == 'money' && $back_day < PREV_DATE_DTL_M) {
		if ($back_day == $min_month_m) {
			$back_day = "";
		} else {
			$back_day = PREV_DATE_DTL_M;
		}
	} elseif ($data_type_ && $back_day < PREV_DATE_DTL) {
		if ($back_day == $min_month) {
			$back_day = "";
		} else {
			$back_day = PREV_DATE_DTL;
		}
	}

	if ($back_day < PREV_DATE) {
		$back_day = "";
	}
	//print($back_day);
	return $back_day;
}


/**
 * 次月の日付の計算
 *
 * @author Azet
 * @param string $date_ (例： 2017-03-17)
 * @return string (例： 2017-04-01)
 */
function next_month_date($date_)
{
	$date = date('Y-m-d', strtotime("$date_"));
	preg_match('/^([0-9]{4}-[0-9]{2})-[0-9]{2}$/', $date, $match);
	$next_date = date('Ymd', strtotime("first day of $match[1] +1 month"));
	if ($next_date > date("Ym01")) {
		$next_date = "";
	}
	return $next_date;
}


/*
	星座別メッセージ取得関数
	add 2016/07/09
*/

// 相互ランク
function sougolink_confirmed_sites()
{
	require_once dirname(__FILE__) . '/sougoulink-listing_action.php';
	$list = getConfirmedSites();

	return $list;
}



// function testSendMail(){
// 	$sougo_mails =  __DIR__ . "/../data/100mail.txt";
// 	$string = "";
// 	$myfile = fopen($sougo_mails, "r") or die("Unable to open file!");
// 	$result = "";
// 	while (($site_info = fgets($myfile)) !== false) {
// 		$values = "(";
// 		$temp_arr = explode("https://",$site_info);
// 		foreach($temp_arr as $key => $value){
// 			$value = trim($value," ");
// 			if($key == 1){
// 				$values .= "'https://" . $value . "'),";
// 				break;
// 			}
// 			$values .= "'" . $value . "',";
// 		}
// 		$result .= $values;

// 	}
// 	fclose($myfile);

// 	// var_dump($result);

// }


function getSeizaDetailMessage($star)
{
	global $msg_data_file;

	$file_name = DATA_FOLDER . $msg_data_file[0];	//固定ファイル

	$filep = fopen($file_name, "r");
	$result = "";

	if ($filep) {
		$dummy = fgets($filep);
		while (substr($dummy, 0, 2) == "//") {
			$dummy = fgets($filep);
		}
		$max_num = trim($dummy);
		if (intval($max_num) > 0) {
			$cnt = 0;
			while ($cnt < $max_num && $line = fgets($filep)) {
				$line_ary = explode("|", $line);
				if (count($line_ary) > 1) {
					if (intval($line_ary[0]) == $star) {
						$result = $line_ary[1];
						$cnt = 999;
					}
				}
				$cnt++;
			}
		}
		fclose($filep);
	}

	return $result;
}

/**
 * 日付パラメータから表示用日付にフォーマットする
 * @param integer $date 日付パラメータ (20170905 ...)
 * @return string $date_print 表示用日付(207年9月5日(火) ...)
 */
function formatDateJpn($date)
{
	$date_print = "";

	$weekday = array("日", "月", "火", "水", "木", "金", "土");
	$w = $weekday[date("w", strtotime($date))];
	if ($date == date('Ymd')) {
		$date_print = "今日";
	} else {
		$date_print = date('Y年n月j日', strtotime($date)) . "($w)";
	}

	return $date_print;
}

function getBestRankOfStar($all_ranks)
{
	$ranks_exist = array_keys($all_ranks);
	$best_rank = $ranks_exist[0];
	return $best_rank;
}

function getRanksExist($all_ranks)
{
	return array_keys($all_ranks);
}


/**
 * 開発用のファンクション
 */
function debug($data_)
{
	// init (if needed)
	if (!isset($GLOBALS['debug'])) {
		$GLOBALS['debug'] = array();
	}

	// append message
	if (is_array($data_) || is_object($data_)) {
		$msg = print_r($data_, true);
	} else {
		$msg = $data_;
	}

	$GLOBALS['debug'][] = $msg;
}


/**
 * アプリによって下のメニューが必要です（古いバーションの場合）
 * 
 * @param  string $app_os_      iOS または Android
 * @param  string $app_version_ 例： 1.1 又は 1.3.0
 * @return boolean               メニューが必要だったら、trueになります
 */
function app_botton_menu_check($app_os_, $app_version_)
{
	$needed = true;

	if ($app_os_ == 'iOS') {
		$needed = !app_version_greater_than("1.1", $app_version_);
	}
	// Androidの場合はHTMLのメニューを使い続きます。（アプリ内でしたのナビゲーションがGoogleに禁止されているため）
	// else if($app_os_ == 'Android') {
	// 	$needed = !app_version_greater_than("1.3.0", $app_version_);
	// }

	return $needed;
}


/**
 * アプリのバージョンの比較するファンクション
 * $base_より、$compare_が上だったら、trueです
 * 
 * @param  string $base_    基本バージョン
 * @param  string $compare_ 比較をしたいバージョン
 * @return bool           $base_ < $compare_
 */
function app_version_greater_than($base_, $compare_)
{
	$base_arr = explode('.', $base_);
	$compare_arr = explode('.', $compare_);

	// make arrays the same length
	if (count($base_arr) > count($compare_arr)) {
		$compare_arr = array_pad($compare_arr, count($base_arr), 0);
	} else if (count($compare_arr) > count($base_arr)) {
		$base_arr = array_pad($base_arr, count($compare_arr), 0);
	}
	// print_r($base_arr);
	// print_r($compare_arr);

	$loop_counter = 0;
	$loop_max = count($base_arr);
	$higher_count = 0;
	foreach ($base_arr as $pos => $base_number) {
		++$loop_counter;
		if ($compare_arr[$pos] < $base_number) {
			$higher_count -= ($loop_max - $loop_counter);
		} elseif ($compare_arr[$pos] > $base_number) {
			++$higher_count;
		}
	}

	return $higher_count > 0;
}

function is_local()
{
	global $AFFILIATE_OFF;
	return (!IS_SERVER || in_array($_SERVER['REMOTE_ADDR'], $AFFILIATE_OFF['IP']) || $_SERVER['REMOTE_ADDR'] == "127.0.0.1");
}


/**
 * SEO用日付文言を返す
 * 
 * @param string $page_date yyyymmdd
 * @return string 今日 | 2019年9月5日(木)
 */
function get_seo_date_string($page_date)
{

	$weekday = array("日", "月", "火", "水", "木", "金", "土");
	$w = $weekday[date("w", strtotime($page_date))];
	$date_print = date('Y年n月j日', strtotime($page_date)) . "($w)";

	return ($page_date == date("Ymd")) ? "今日" : $date_print;
}
/*
 * 上部・下部の遷移メニューリンク作成
 * 
 * $topic_Jp_name 日本語と英語のメニュー組み合わせ配列
 * $date 選択している日付
 * $mode 開いているページのmode
 */
function make_menu_link($topic_Jp_name, $date, $mode, $en_star, $data_type)
{
	$link_basic = array();
	foreach ($topic_Jp_name as $en_type => $jp_type) {
		if ($en_type != 'defolt') {
			$link_basic["$en_type"] .= "/" . $en_type . "/";
		} else {
			$link_basic["$en_type"] .= "/";
		}
		if ($date != date('Ymd') && $mode != 'ranking-past') {
			$link_basic["$en_type"] .= $date . "/";
		}
		if ($mode == 'ranking-past') {
			$link_basic["$en_type"] .= $mode;
		}
		if ($mode == 'detail') {
			$link_basic["$en_type"] .= $en_star;
		}
		if ($link_basic["$en_type"] == "/") {
			$link_basic["$en_type"] = 'rank';
		} else {
			$link_basic["$en_type"] = ltrim($link_basic["$en_type"], "/");
			$link_basic["$en_type"] = rtrim($link_basic["$en_type"], "/");
		}
		if ($date < PREV_DATE_DTL) {
			if ($en_type != 'defolt') {
				$link_basic["$en_type"] = "404";
			}
		}
		if ($date < PREV_DATE_DTL_M) {
			if ($en_type == 'money') {
				$link_basic["$en_type"] = "404";
			}
		}
		if ($date < PREV_DATE && $en_type == 'defolt') {
			$link_basic["$en_type"] = "404";
		}
	}
	if ($data_type == 'love' || $data_type == 'work') {
		$link_basic['ranking_past'] .= $data_type . "/ranking-past";
	} else {
		$link_basic['ranking_past'] .= "ranking-past";
	}
	$link_basic['digest'] = "digest/2023";
	//	print_r($link_basic);
	return $link_basic;
}

function trim_msg($msg, $length)
{
	return mb_substr($msg, 0, $length) . "...";
}