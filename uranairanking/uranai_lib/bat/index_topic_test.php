#!/usr/local/bin/php
<?php
/**
 * このスクリプトの使い方
 * ======================
 * $ php index.php
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
// random ranking in twitts
include(dirname(__FILE__).'/../libadmin/uranairanking.class.php');
//include(dirname(__FILE__).'/../libadmin/graph_data.php');	//add okabe 2016/08/23	一時使用中止
require_once dirname(__FILE__) . "/../libadmin/custom_message.class.php" ;

# このプログラムは CRON が実行することを前提としている。

date_default_timezone_set('Asia/Tokyo');
set_time_limit(0);

### DEBUG
### $time_start = microtime(true);
$log = new Log();
$log->start();

// 新規プラグインクラス
UranaiPlugin::setLogObject($log);
UranaiPlugin::setConnObject($conn);
TwitterAPI::setLogObject($log);
FacebookAPI::setLogObject($log);
UranaiRanking::setLogObject($log);

# データベースが使えないならばランキングの処理ができない
//$conn = mysqli_connect(DB_SERVER, DB_USER, DB_PASS, DB_NAME);
//if (!$conn) {
//	$log->add('INIT/DB', "データベースが使えないとランキングの処理ができません。");
//	$log->stop();
//	exit(1);
//}
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
	'pattern' => '',
	'force-backup' => false,
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

	// バックアップ強制モード
	if($entry_ === '--force-backup') {
		$params['force-backup'] = true;
		$log->add('INIT', "バックアップ強制モード");
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

// テスト以外、ログがある場合は処理しない
if(!$params['test']) {
	$sql .= " AND (is_execute = '1')
		AND site_id NOT IN (SELECT DISTINCT(site_id) FROM `log` WHERE day = current_date)";
}

// specific site
if($params['site']) {
	$sql .= " AND site_id={$params['site']}";
}

// TEST
if($params['test']) {
	print $sql.PHP_EOL;
}

// 前日のバックアップがあればzipする
// ======================================================================
$yesterday_ymd = date('Ymd', strtotime("-1 days")); //昨日

//zipする
if (UranaiPlugin::backupExists($yesterday_ymd)) { //昨日バックアップがあれば
	if (UranaiPlugin::archiveBackup($yesterday_ymd)) { //圧縮して元は消す
		$log->add("ARCHIVE-BACKUP", "OK バックアップ圧縮 ".$yesterday_ymd." 正常終了");
	} else {
		$log->add("ARCHIVE-BACKUP", "ERR バックアップ圧縮 ".$yesterday_ymd." 処理中にエラーがありました");
	}
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

		// reset data
		$data = null;

		# サイト ID を取得する
		$site_id = $row['site_id'];
		$parent_id = $row['parent_id'];

		# 指定時間を取得する
		$site_get_time = $row["site_get_time"];

		$log->add("SITE $site_id @ $site_get_time", "{$row['site_name']}のurlを読み込み");

		/**
		 * REFリンクタイプ
		 */

		// data file save key
		$data_file_key = "data-".date('Ymd-H')."_{$site_id}";
		//print $data_file_key.PHP_EOL;

		//全体運のURLの読み込み　htmlデータの取得
		$URL = array();

		$URL = UranaiPlugin::get_Url_Type($row["get_type"],$row,$data_file_key);

		$URL = UranaiPlugin::url_Date_Replace($URL);
		//各運勢用のURL読み込み　htmlデータの取得
		if( $row["site_topic"] ){
			$TOPIC_URL = array();

			if( $row["get_type"] != $row["topic_get_type"]){
				$TOPIC_URL = UranaiPlugin::get_Url_Type($row["topic_get_type"],$row,$data_file_key);
				$TOPIC_URL = UranaiPlugin::url_Date_Replace($TOPIC_URL);
			}
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
			foreach($URL as $k => $link) {
				$URL[$k] = $test_path.UranaiPlugin::convertUrlPatternFileName($link);
			}
			foreach($TOPIC_URL as $t_k => $t_link) {
				$TOPIC_URL[$t_k] = $test_path.UranaiPlugin::convertUrlPatternFileName($t_link);
			}
		}

		if($params['test']) {
			print "URLS: ".print_r($URL, true).PHP_EOL;
		}
		if($params['test'] && $row["site_topic"]) {
			print "TOPIC_URLS: ".print_r($TOPIC_URL, true).PHP_EOL;
		}


		// plugin file
		@include("plugins/$plugin_file");
		//Plugin check
		if(class_exists($plugin_call)) {
			$plugin = new $plugin_call($site_id, $parent_id);

			if($params['patternMake']) {
				$patternFolder = BAT_PATTERN_TEST_FOLDER.$plugin_id.'/'.$params['patternMake'];
				$plugin->patternMake($patternFolder);
			}

			if($params['pattern']) {
				$plugin->setPatternMode();
			}

			if($params['test']) {
				$plugin->setTestMode();
			}

			if($params['force-backup']) {
				$plugin->forceBackup();
			}

			$CONTENTS = array();
			$CONTENTS = $plugin -> load($URL);
			$data = $plugin -> run($CONTENTS);

		}else {
			$msg =  "ERR {$plugin_file} ファイルか、{$plugin_call} クラス がありません。";
			$log->add("PLUGIN $site_id",$msg);
			continue;
		}

		# 親プログラムはデータベースに順位を記録する。内容がNULLなら記録しない。ログも残す。
		if (!$data) {
			$log->add("PLUGIN $site_id DATA", "ERR プラグインからの情報の提供がないため、続行できません");
			//次のプラグインを起動する
			continue;
		}else {
			$log->add("PLUGIN $site_id DATA", "OK サイトのファイルの読み込みはサクセスでした");
		}

		// テストの時に、情報を出す
		if($params['test']) {
			print "DATA: ".print_r($data, true).PHP_EOL;
		}

		//総合運 sql文の作成
		list($sql,$date_error,$data_size) = $plugin -> make_Sql_data( $data ,$row);

		$sql = rtrim($sql, ',');
		// TEST
		//print $sql;

		// data check (content and size)
		if($data_error || $data_size!=12) {
			// データエラーの場合は
			$log->add("PLUGIN $site_id DATA", 'ERR データの形式は間違っている:'.str_replace("\n", '', print_r($data, 1)));
		}else {
			if( !$row["site_topic"] ){
				// HTMLファイル削除 >>>
				$data_clear_cmd = 'rm '.DATA_SAVE_FOLDER.$data_file_key.'*';

				//print $data_clear_cmd.PHP_EOL;
				system($data_clear_cmd, $clear_ok);
				if($clear_ok>0) {
					//print "Clear command return: ".$clear_ok.PHP_EOL;
					$log->add("PLUGIN $site_id TOPIC_DATA", "ERR データファイルの削除ができませんでした: [$data_clear_cmd]");
				}
				// <<<
			}
			// DBにデータを保存
			$save_ok = $plugin -> inport_Data_DB($sql,$conn,$params);
		}

		//各運勢取得ロジック
		if( $row["site_topic"] ){
			$auto_flag = TRUE;
			$topic_param_arr = array(
				'auto_flag' => $auto_flag,
				'plugin_call' => $plugin_call,
				'plugin_id' => $plugin_id,
				'plugin_file' => $plugin_file
			);
			$topic_ok = $plugin -> get_Topic_Data_Main( $row , $params ,$topic_param_arr,$CONTENTS ,$TOPIC_URL ,$conn);

		}else{
			$log->add("PLUGIN $site_id TOPIC_DATA", "運勢別の情報が無効です");
		}


		if($site_topic){
			if( $save_ok && $topic_ok){++$data_updated;}
		}else{
		if( $save_ok ){++$data_updated;}
		}

		//ラッキーシンボル取得
		if( $row["site_topic"] && $row["site_topic"] == 2){
			$auto_flag = TRUE;
			$lucky_param_arr = array(
				'auto_flag' => $auto_flag,
				'plugin_call' => $plugin_call,
				'plugin_id' => $plugin_id,
				'plugin_file' => $plugin_file
			);
			//TODO: トピックの情報がすでにあったら再利用する（通信が増えるのを防ごう） -> 引数3
			$topic_ok = $plugin -> getLuckySymbol($row, $params, $lucky_param_arr , $CONTENTS, $TOPIC_URL,$conn);

		}else{
			$log->add("PLUGIN $site_id TOPIC_DATA", "ラッキーシンボル取得はOFFです");
		}


		if($site_topic){
			if( $save_ok && $topic_ok){++$data_updated;}
		}else{
		if( $save_ok ){++$data_updated;}
		}

	}

	mysqli_free_result($result);

}else {
	// 時間に対して、プラグインを起動は必要ではなかった。
	$log->add('INIT', "この時間に、起動するプラグインがありません。");
}


// 集計
// ======================================================================
//add okabe start 2016/07/14
$today_arr = UranaiPlugin::getToday();
// add simon 2016-09-01 >>>
if($data_updated || $params['test']) {
	// edit simon 2017-03-29
	//$ranking->compileLogsDaily($today_arr);
	UranaiRankingEX::compileLogs($today_arr['year'].'-'.$today_arr['month'].'-'.$today_arr['day'],'daily');//add kimura 2017/04/10
	UranaiRankingEX::compileTopicLogs($today_arr['year'].'-'.$today_arr['month'].'-'.$today_arr['day'],'daily');//add yamaguchi 2017/06/08
}
// <<<
$type = UranaiRankingEx::randomDataType();
$ranking = new UranaiRankingEX($today_arr['year'].'-'.$today_arr['month'].'-'.$today_arr['day'],$type['en']);
$ranking_arr = $ranking->getRanks();

// SNSプラグイン用の共有メッセージ
$weekday = array("日", "月", "火", "水", "木", "金", "土");
$w = $weekday[date("w", strtotime($today_arr['year'].'-'.$today_arr['month'].'-'.$today_arr['day']))];
$today = $today_arr['month'].'月'.$today_arr['day'].'日('.$w.')';
$hour = date("H");

$rand_rank = rand(0, 11);
$random_ranking = "ちなみに「{$ranking_arr[$rand_rank]['name']}」の「{$type['jp']}」は{$ranking_arr[$rand_rank]['num']}位。";
$msg_base = "{$today}の12星座占いランキングが{$hour}時に更新されました！";

// SNSプラグインは本番でしか実行しません！ simon 2016-09-01
if( IS_SERVER && $data_updated>0 ) { // edit simon 2016-09-01

	//カスタムメッセージ	
	$custom_message = new CustomMessage($conn);
	$custom_message->loadMessages([
		 "TWEET"
		,"TOOT"
	]);

	// Twitter >>>
	$twitter = new TwitterAPI();
	$twitter->test_mode = $params['test'];
	// message
	$twitmsg = $custom_message->of("TWEET");
	$twitmsg.= "\n";
	$twitmsg.= "【{$today}】\n";
	$twitmsg.= "占いランキングが{$hour}時に更新されたよ😍\n";
	$twitmsg.= "\n";
	$twitmsg.= "現在の #{$ranking_arr[$rand_rank]['name']} ".$star_emojis[$ranking_arr[$rand_rank]['star_num']]." は・・・\n";
	$twitmsg.= "\n";
	$twitmsg.= "💖 #{$type['jp']} {$ranking_arr[$rand_rank]['num']}位 💖\n";
	$twitmsg.= "\n";
	$twitmsg.= "詳しくは%sをチェック❗\n";
	$twitmsg.= "#星座占い #星占い #占い #12星座";

	// send
	// $ok = $twitter->publish(sprintf(
	// 	$twitmsg,"https://uranairanking.jp/?utm_source=Tw.b&utm_medium=Tw.b&utm_campaign=Tw.b&utm_id=Tw.b"
	// ));
	// <<<

	// facebook >>>
	// $facebookmsg = "{$msg_base}\n{$random_ranking}";
	// $facebook = new FacebookAPI();
	// $facebook->test_mode = $params['test'];
	// send
	//$ok = $facebook->publish($facebookmsg); //API更新のため一時コメント 2018/07/31 kimura
	// <<<

}


// SNSプラグインテスト (開発サーバ用)>>>
//if($params['test'] && !IS_SERVER) { // 手動でテスト
//if($data_updated>0 && !IS_SERVER) { // 自動（クロン）テスト
//echo "{$msg_base}\n{$random_ranking}\n".PROD_SITE_ROOT_URL."\n#星座占い #ランキング";
//$test_msg ="{$msg_base}\n{$random_ranking}\n".PROD_SITE_ROOT_URL."\n#星座占い #ランキング";
//mail("yamaguchi@azet.jp","SNStestMessage",$test_msg);
//}
// <<<

// 一時使用中止 2016/08/24
////add okabe start 2016/08/23	グラフ用データ作成
//list($rs, $dt) = make_graph_data();
//if (!$rs) {
//	$log->add("Create Graph Data", 'SAVE OK');
//} else {
//	$log->add("Create Graph Data", 'SAVE ERR '.$rs);
//}
//add okabe end 2016/08/23


// end mysql
// ======================================================================
mysqli_close($conn);

$log->stop();

print PHP_EOL;
### DEBUG
### $timelimit = microtime(true) - $time_start;
### echo "${timelimit} seconds\n";
