#!/usr/local/bin/php
<?php
/**
 * =======================================
 * =======================================
 * 未使用？ならば削除しよう！ シモン 2018-10-01
 * =======================================
 * =======================================
 *
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
include(dirname(__FILE__).'/../libadmin/snsapi-mastodon.class.php');
// random ranking in twitts
include(dirname(__FILE__).'/../libadmin/uranairanking.class.php');
//include(dirname(__FILE__).'/../libadmin/graph_data.php');	//add okabe 2016/08/23	一時使用中止

# このプログラムは CRON が実行することを前提としている。

date_default_timezone_set('Asia/Tokyo');
set_time_limit(0);

### DEBUG
### $time_start = microtime(true);
$log = new Log();
$log->start();

// 新規プラグインクラス
UranaiPlugin::setLogObject($log);
TwitterAPI::setLogObject($log);
FacebookAPI::setLogObject($log);
MastodonAPI::setLogObject($log);
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
	'pattern' => ''
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

// テスト以外、ログがある場合は勝利しない
if(!$params['test']) {
	$sql .= " AND (is_execute = '1')
		AND site_id NOT IN (SELECT DISTINCT(site_id) FROM `log_test` WHERE day = current_date)";
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

		// reset data
		$data = null;

		# サイト ID を取得する
		$site_id = $row["site_id"];

		# 指定時間を取得する
		$site_get_time = $row["site_get_time"];

		$log->add("SITE $site_id @ $site_get_time", "{$row['site_name']}のurlを読み込み");

		/**
		 * REFリンクタイプ
		 */

		// data file save key
		$data_file_key = "data-".date('Ymd-H')."_{$site_id}";
		//print $data_file_key.PHP_EOL;

		$URL = array();
		if ($row["get_type"] == "1") {
			// single main URL
			$URL[0] = $row["url"].'#'.$data_file_key;
		} elseif ($row["get_type"] == "2") {
			// zoodiac URL
			for ($i = 1; $i <= 12; $i++) {
				$set_url_name = "star".$i."_url";
				$URL[$i] = $row[$set_url_name].'#'.$data_file_key.'_'.$i;
			}
		} elseif ($row["get_type"] == "3") {
			// other link URL
			$URL[0] = $row["etc_url"].'#'.$data_file_key;
			for ($i = 1; $i <= 12; $i++) {
				$set_url_name = "star".$i."_url";
				$URL[$i] = $row[$set_url_name].'#'.$data_file_key.'_'.$i;
			}
		}

		// plugin file
		$plugin_id = sprintf("%06s", $site_id);
		$plugin_call = "Zodiac{$plugin_id}";
		$plugin_file = $plugin_id . ".php";

		//TESTとPATTERNのばあいは、ロカルファイルのテスト仕様
		if($params['test'] && $params['pattern']) {
			//test file(s)
			$test_path = BAT_PATTERN_TEST_FOLDER.$plugin_id.'/'.$params['pattern'].'/';
			foreach($URL as $k => $link) {
				$URL[$k] = $test_path.UranaiPlugin::convertUrlPatternFileName($link);
			}
		}

		if($params['test']) {
			print "URLS: ".print_r($URL, true).PHP_EOL;
		}

		// plugin file
		@include("plugins/$plugin_file");
		//Plugin check
		if(class_exists($plugin_call)) {
			$plugin = new $plugin_call($site_id);

			if($params['patternMake']) {
				$patternFolder = BAT_PATTERN_TEST_FOLDER.$plugin_id.'/'.$params['patternMake'];
				$plugin->patternMake($patternFolder);
			}

			if($params['pattern']) {
				$plugin->setPatternMode();
			}
			$data = $plugin->run($URL);
		}
		else {
			$msg =  "ERR {$plugin_file} ファイルか、{$plugin_call} クラス がありません。";
			$log->add("PLUGIN $site_id",$msg);
			continue;
		}

		# 親プログラムはデータベースに順位を記録する。内容がNULLなら記録しない。ログも残す。
		if (!$data) {
			$log->add("PLUGIN $site_id DATA", "ERR プラグインからの情報の提供がないため、続行できません");
			//次のプラグインを起動する
			continue;
		}
		else {
			$log->add("PLUGIN $site_id DATA", "OK サイトのファイルの読み込みはサクセスでした");
		}

		// テストの時に、情報を出す
		if($params['test']) {
			print "DATA: ".print_r($data, true).PHP_EOL;
		}

		$sql = "INSERT INTO `log_test` (
					`site_id`,
					`day`,
					`star`,
					`rank`,
					`date_create`
				) VALUES ";

		// TEST error data
		//$data[' '] = 'error data';

		$data_error = false;
		$data_size = 0;
		foreach ($data as $key => $value) {
			++$data_size;
			// データ確認
			if(!preg_match("/[0-9]+/", $key) || !preg_match("/[0-9]+/", $value)) {
				$data_error = true;
				break;
			}

			//add okabe start 2016/04/04 データの順位に1～12以外の数値が含まれていないかチェック
			if ($value < 1 || $value > 12) {
				$data_error = true;
				break;
			}
			//add okabe end 2016/04/04

			// query build up
			$sql .= "(
					'{$row["site_id"]}',
					CURRENT_DATE,
					'{$key}',
					'{$value}',
					CURRENT_TIMESTAMP
				),";
		}
		$sql = rtrim($sql, ',');
		// TEST
		//print $sql;

		// data check (content and size)
		if($data_error || $data_size!=12) {
			// データエラーの場合は
			$log->add("PLUGIN $site_id DATA", 'ERR データの形式は間違っている:'.str_replace("\n", '', print_r($data, 1)));
		}
		else {
			// HTMLファイル削除 >>>
			$data_clear_cmd = 'rm '.DATA_SAVE_FOLDER.$data_file_key.'*';

			//print $data_clear_cmd.PHP_EOL;
			system($data_clear_cmd, $clear_ok);
			if($clear_ok>0) {
				//print "Clear command return: ".$clear_ok.PHP_EOL;
				$log->add("PLUGIN $site_id DATA", "ERR データファイルの削除ができませんでした: [$data_clear_cmd]");
			}
			// <<<

			// DBにデータを保存
			if($params['test']) {
				//print "SAVE QUERY: $sql".PHP_EOL;
				$log->add("PLUGIN $site_id DB", 'TEST');
			}
			else {
				// OK：保存しましょう
				$ok = mysqli_query($conn, $sql);
				if(!$ok) {
					$log->add("PLUGIN $site_id DB", 'SAVE ERR');
				}
				else {
					$log->add("PLUGIN $site_id DB", 'SAVE OK');
					++$data_updated;
				}
			}
		}
	}
	mysqli_free_result($result);

}
else {
	// 時間に対して、プラグインを起動は必要ではなかった。
	$log->add('INIT', "この時間に、起動するプラグインがありません。");
}


// 集計
// ======================================================================
//add okabe start 2016/07/14
$today_arr = UranaiPlugin::getToday();
$today = $today_arr['month'].'月'.$today_arr['day'].'日';
$now = date("H:i");
if(date("H") == "00"){
	$type =array( 'en' => '', 'jp' => '総合運');
}else{
	$type = UranaiRankingEx::randomDataType();

}
// add simon 2016-09-01 >>>
if($data_updated || $params['test']) {
	// edit simon 2017-03-29
	//$ranking->compileLogsDaily($today_arr);
	//UranaiRankingEX::compileLogs($today_arr['year'].'-'.$today_arr['month'].'-'.$today_arr['day'],'daily');//add kimura 2017/04/10
}
// <<<

$ranking = new UranaiRankingEX($today_arr['year'].'-'.$today_arr['month'].'-'.$today_arr['day'],$type['en']);
$ranking_arr = $ranking->getRanks();


// SNSプラグイン用の共有メッセージ
$rand_rank = rand(0, 11);
$random_ranking = "ちなみに「{$ranking_arr[$rand_rank]['name']}」の「{$type['jp']}」は{$ranking_arr[$rand_rank]['num']}位。";
$msg_base = "{$today}の12星座占いランキングが{$now}に更新されました！";

/*
// SNSプラグインは本番でしか実行しません！ simon 2016-09-01
if( IS_SERVER && $data_updated>0 ) { // edit simon 2016-09-01


	// Twitter >>>
	$twitter = new TwitterAPI();
	$twitter->test_mode = $params['test'];
	// message
	$twitmsg = "{$msg_base}\n{$random_ranking}\n".PROD_SITE_ROOT_URL."\n#星座占い #ランキング";
	// send
	$ok = $twitter->publish($twitmsg);
	// <<<

	// facebook >>>
	$facebookmsg = "{$msg_base}\n{$random_ranking}";
	$facebook = new FacebookAPI();
	$facebook->test_mode = $params['test'];
	// send
	$ok = $facebook->publish($facebookmsg);
	// <<<

	// mastodon >>>
	$mastodonmsg = "{$msg_base}\n{$random_ranking}\n".PROD_SITE_ROOT_URL."\n#占い #星座 #ランキング";
	$mastodon = new MastodonAPI();
	$mastodon->test_mode = $params['test'];
	// send
	$ok = $mastodon->publish($mastodonmsg,'public');
	// <<<

}
*/
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
