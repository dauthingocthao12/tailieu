#!/usr/local/bin/php
<?php
/**
 * 星座詳細のみ取得するプログラム（ボタン起動のみ）
 * このスクリプトの使い方
 * ======================
 * $ php topic.php
 * ふつうの使い方
 *
 * パラメター：
 * --test
 * テスト用、DBに保存されていない、プラグインのデータを出力をする
 *
 * --now
 * DBに設定されているサイトの時間に関係なく、今すぐデータを読み込む
 *
 * [--test]のパラメターは必要です。
 *
 * --site <x>
 * 一つのサイトだけを起動する：<x>はIDです
 *
 * --pattern <y>
 * テストのパターン：/uranai_lib/bat/tests/<00000x>/<y>/httpkids.yahoo.co.jpfortune
 * テストのファイル名には、[:] [/] の文字禁止です
 *
 * --patternMake <y>
 * 読み込んだデータが、パターンに保存します。
 * <y>パターンフォルダーに保存させます。
 */


//======================================================================
// ___ _   _ ___ _____
//|_ _| \ | |_ _|_   _|
// | ||  \| || |  | |
// | || |\  || |  | |
//|___|_| \_|___| |_|
//
//======================================================================

include(dirname(__FILE__).'/../libadmin/config.php');
include(dirname(__FILE__).'/cron.tools.php');
include(dirname(__FILE__).'/../libadmin/snsapi.class.php');
include(dirname(__FILE__).'/../libadmin/snsapi-twitter.class.php');
include(dirname(__FILE__).'/../libadmin/snsapi-facebook.class.php');
include(dirname(__FILE__).'/../libadmin/uranairanking.class.php');
date_default_timezone_set('Asia/Tokyo');
set_time_limit(0);

### DEBUG
### $time_start = microtime(true);
$log = new Log();
$log->start();

// 新規プラグインクラス
UranaiPlugin::setLogObject($log);
UranaiPlugin::setConnObject($conn);
UranaiRanking::setLogObject($log);

mysqli_query($conn, "set names 'utf8'");

//======================================================================
//  ____ ____   ___  _   _
// / ___|  _ \ / _ \| \ | |
//| |   | |_) | | | |  \| |
//| |___|  _ <| |_| | |\  |
// \____|_| \_\\___/|_| \_|
//
// ____   _    ____      _    __  __ _____ _____ _____ ____  ____
//|  _ \ / \  |  _ \    / \  |  \/  | ____|_   _| ____|  _ \/ ___|
//| |_) / _ \ | |_) |  / _ \ | |\/| |  _|   | | |  _| | |_) \___ \
//|  __/ ___ \|  _ <  / ___ \| |  | | |___  | | | |___|  _ < ___) |
//|_| /_/   \_\_| \_\/_/   \_\_|  |_|_____| |_| |_____|_| \_\____/
//
//======================================================================

// テストの為に、パラメターがあれば、プラグインを使用する
$param_name = "";
$params = array(
	'test' => false,
	'now' => false,
	'site' => 0,

);
foreach($argv as $entry_) {

	// parameter names
	if($entry_ === '--site') {
		$param_name = $entry_;
		continue;
	}

	if($entry_ === '--pattern') {
		$param_name = $entry_;
		continue;
	}

	if($entry_ === '--patternMake') {
		$param_name = $entry_;
		continue;
	}

	// testモード
	if($entry_ === '--test') {
		$params['test'] = true;
		$log->add('INIT', "テストモード");
		continue;
	}

	// 今すぐモード
	if($entry_ === '--now') {
		$params['now'] = true;
		$log->add('INIT', "今すぐモード");
		continue;
	}

	// 上はキー名、下は値です
	// ======================

	// パラメター値
	if($param_name=='--site') {
		$params['site'] = $entry_;
		$log->add('INIT', "プラグイン $entry_ 使用");
		continue;
	}

	if($param_name=='--pattern') {
		$params['pattern'] = $entry_;
		$log->add('INIT', "テストパターン $entry_ 使用");
		continue;
	}

	if($param_name === '--patternMake') {
		$params['patternMake'] = $entry_;
		$log->add('INIT', "パターン $entry_ 使作");
		continue;
	}
}

//======================================================================
// _     ___   ____ ___ ____
//| |   / _ \ / ___|_ _/ ___|
//| |  | | | | |  _ | | |
//| |__| |_| | |_| || | |___
//|_____\___/ \____|___\____|
//======================================================================

// 設定時間ではなく、今すぐする
if($params['now']) {
	$time_clause = '1';
}
else {
	// Time check
	$time_clause = " (DATE_FORMAT(site_get_time, '%k') = DATE_FORMAT(now(), '%k'))";
	// 曜日の条件
	$week_day = date('w');
	$time_clause .= " AND `site_get_week{$week_day}` = 1";
}

// クエリのベース
$sql = "
SELECT *
FROM `site`
WHERE is_delete=0 AND
	$time_clause
";

// テスト以外ログがある場合は処理しない
if(!$params['test']){
	$sql .= "AND (site_topic = '1') AND site_id NOT IN (SELECT DISTINCT(site_id) FROM `topic_log` WHERE day = current_date)";
}
// specific site
if($params['site']) {
	$sql .= " AND site_id={$params['site']}";
}

// TEST
if($params['test']) {
	print $sql.PHP_EOL;
}

// プラグイン実行
// ======================================================================
$result = mysqli_query($conn, $sql);
$data_updated = 0;

if ($result->num_rows>0) {
	//$i = 1;
	//print_r($result);

	while ($row = mysqli_fetch_assoc($result)) {
		//print_r($row);

		# サイト ID を取得する
		$site_id = $row["site_id"];
		$parent_id = $row["parent_id"];

		# 指定時間を取得する
		$site_get_time = $row["site_get_time"];

		$log->add("SITE $site_id @ $site_get_time", "{$row['site_name']}のurlを読み込み");

		/**
		 * REFリンクタイプ
		 */
		// data file save key
		$data_file_key = "data-".date('Ymd-H')."_{$site_id}";
		//print $data_file_key.PHP_EOL;

		//各運勢用のURL読み込み
		if( $row["site_topic"] ){
			$TOPIC_URL = array();

			$TOPIC_URL = UranaiPlugin::get_Url_Type($row["topic_get_type"],$row,$data_file_key);
			$TOPIC_URL = UranaiPlugin::url_Date_Replace($TOPIC_URL);

		}

		// plugin file
		$plugin_id = sprintf("%06s", $site_id);
		$plugin_call = "Zodiac{$plugin_id}";
		$plugin_file = $plugin_id . "_t.php";	//テストデータ
//		$plugin_file = $plugin_id . ".php";	本番用データ

		//TESTとPATTERNのばあいは、ローカルファイルのテスト仕様
		if($params['test'] && $params['pattern']) {
			//test file(s)
			$test_path = BAT_PATTERN_TEST_FOLDER.$plugin_id.'/'.$params['pattern'].'/';
			foreach($TOPIC_URL as $t_k => $t_link) {
				$TOPIC_URL[$t_k] = $test_path.UranaiPlugin::convertUrlPatternFileName($t_link);
			}
		}

		if($params['test'] && $row["site_topic"]) {
			print "TOPIC_URLS: ".print_r($TOPIC_URL, true).PHP_EOL;
		}

		// plugin file
		@include("plugins/$plugin_file");
		//Plugin check
		$plugin = new $plugin_call($site_id, $parent_id);

		//各運勢取得ロジック
		if( $row["site_topic"] ){
			$auto_flag = FALSE;
			$topic_param_arr=array(
				"auto_flag" => $auto_flag,
				"plugin_call" => $plugin_call,
				"plugin_id" => $plugin_id,
				'plugin_file' => $plugin_file);
			$topic_ok = $plugin -> get_Topic_Data_Main( $row , $params ,$topic_param_arr,$CONTENTS ,$TOPIC_URL ,$conn);

		}else{
			$log->add("PLUGIN $site_id TOPIC_DATA", "運勢別の情報が無効です");
		}

		if( $topic_ok){++$data_updated;}

	}


	mysqli_free_result($result);

}else {
	// 時間に対して、プラグインを起動は必要ではなかった。
	$log->add('INIT', "この時間に、起動するプラグインがありません。");
}


// 集計
// ======================================================================
$today_arr = UranaiPlugin::getToday();
$today = $today_arr['month'].'月'.$today_arr['day'].'日';
$now = date("H:i");
$ranking = new UranaiRanking($today_arr['year'].'-'.$today_arr['month'].'-'.$today_arr['day']);
$ranking_arr = $ranking->getRanks();

if($data_updated || $params['test']) {

	$ranking->compileTopicLogs($today_arr['year'].'-'.$today_arr['month'].'-'.$today_arr['day'],'daily');
}
// <<<


// end mysql
// ======================================================================
mysqli_close($conn);

$log->stop();

print PHP_EOL;
### DEBUG
### $timelimit = microtime(true) - $time_start;
### echo "${timelimit} seconds\n";
