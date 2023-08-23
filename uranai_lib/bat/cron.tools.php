<?php

class Log {

	private $file;

	function __construct() {
		$this->file = LOG_FOLDER . date('Ymd') . '.log';
		if(!file_exists($this->file)) {
			$fd = fopen($this->file, 'w');
			fclose($fd);
		}
		@chmod($this->file, 0777);
		//print "log file: ".$this->file.PHP_EOL;
	}

	function start() {
		$this->add('BEGIN', '=== Batch Start ===');
	}

	function stop() {
		$this->add('STOP', '=== Batch Stop ===');
	}

	function add($header_, $msg_) {
		$line = date('Y-m-d H:i:s').' | '.$header_.' | '.$msg_ ."\n";
		error_log($line, 3, $this->file);
	}

}


class UranaiPlugin {

	/** @var Log $log */
	static public $log = null;
	/** @var \mysqli $conn */
	static public $conn = null; // MySQL接続オブジェクト (MySQLi)


	// 一般的に下記の星座の書き方です
	static public $starDefault = array(
		'みずがめ座' => 1,
		'うお座' => 2,
		'おひつじ座' => 3,
		'おうし座' => 4,
		'ふたご座' => 5,
		'かに座' => 6,
		'しし座' => 7,
		'おとめ座' => 8,
		'てんびん座' => 9,
		'さそり座' => 10,
		'いて座' => 11,
		'やぎ座' => 12
	);

	static public $starKanji = array(
		'水瓶座' => 1,
		'魚座' => 2,
		'牡羊座' => 3,
		'牡牛座' => 4,
		'双子座' => 5,
		'蟹座' => 6,
		'獅子座' => 7,
		'乙女座' => 8,
		'天秤座' => 9,
		'蠍座' => 10,
		'射手座' => 11,
		'山羊座' => 12
	);

	// my SiteID
	public $id = 0;
	public $parent_id = 0;

	// cUrl規定
	static public $curlParamsDefault = array(
			CURLOPT_HEADER => 0,
			CURLOPT_URL => 'XXX',
			CURLOPT_FRESH_CONNECT => 1,
			CURLOPT_RETURNTRANSFER => 1,
			CURLOPT_FORBID_REUSE => 1,
			CURLOPT_TIMEOUT => 4,
			CURLOPT_USERAGENT => 'Mozilla/5.0 (Linux; Android 4.0.4; Galaxy Nexus Build/IMM76B) AppleWebKit/535.19 (KHTML, like Gecko) Chrome/18.0.1025.133 Mobile Safari/535.19'
		);
	// Curlを使用時に
	private $curlParams = null;

	// Pattern Use
	public $patternMode = false;

	// Test Mode
	public $testMode = false;

	// Force Backup Mode
	public $forceBackup = false;

	// Pattern Save
	private $patternMakeFolder;


	/**
	 * プラグインに利用される共有オブジェクト登録
	 *
	 * @author Azet
	 * @param Log $log_ ログ管理するインスタンス
	 */
	static function setLogObject($log_) {
		self::$log = $log_;
	}


	/**
	 * MySQLi接続オブジェクト登録
	 *
	 * @param mysqli $conn_
	 * @return void
	 */
	static function setConnObject($conn_) {
		self::$conn = $conn_;
	}


	/**
	 * 読み込むURLの形から、ファイル名に変換する
	 *
	 * clears :/?& characters
	 * @author Azet
	 * @param string $url_
	 * @return string
	 */
	static function convertUrlPatternFileName($url_) {
		return preg_replace('/[:\/\?\&]/', '', $url_);
	}

	static function convertUrlToBackupFileName($url_) {
		$url_data_reg = "/#(data-.*$)/";

		$ret = preg_replace($url_data_reg, '', $url_);
		$ret = preg_replace('/[:\/\?\&]/', '', $ret);

		return $ret;
	}

	/**
	 * 今日の日付の情報を出す
	 *
	 * @author Azet
	 * @return array[year => xx, month => yy, day => zz]
	 */
	static function getToday() {
		return array(
			'year' => date('Y'),
			'month' => date('n'),
			'day' => date('j')
		);
	}


	/**
	 * 今日の日付の情報を出す (ISO)
	 * クエリに便利
	 *
	 * @author Azet
	 * @return array[year => xx, month => yy, day => zz]
	 */
	public static function getTodayISO()
	{
		return date('Y-m-d'); // 例: 2018-01-01
	}


	function __construct($id_, $parent_id_) {
		// プラグインIDを登録
		$this->id = $id_;
		$this->parent_id = $parent_id_;
	}


	function logDateError() {
		$msg = 'ERR 更新日の確認できませんでした';
		self::$log->add("PLUGIN {$this->id}", $msg);
		return $msg;
	}


	/**
	 * パターンモードを設定
	 *
	 * @author Azet
	 */
	function setPatternMode() {
		$this->patternMode = true;
	}

	/**
	 * テストモードを設定
	 *
	 * @author Azet
	 */
	function setTestMode() {
		$this->testMode = true;
	}

	/**
	 * テストモードを設定
	 *
	 * @author Azet
	 */
	function forceBackup() {
		$this->forceBackup = true;
	}

	/**
	 * Curlを使う為に、設定登録する機能
	 *
	 * @author Azet
	 * @param array $params_
	 */
	function useCurl($params_) {
		$this->curlParams = $params_;
	}


	/**
	 * cUrlでデータをゲット
	 *
	 * @author Azet
	 * @param string $url_
	 * @return string
	 */
	function getWithCurl($url_) {
		$curlParams = $this->curlParams;

		// custom URL
		$curlParams[CURLOPT_URL] = $url_;

		$ch = curl_init();
		curl_setopt_array($ch, $curlParams);
		$result = curl_exec($ch);

		//if( ! $result = curl_exec($ch))
		//{
		//	return null;
		//}

		curl_close($ch);
		return $result;
	}


	/**
	 * HPのデータを読み込む
	 *
	 * @author Azet
	 * @param array $URL
	 * @return array
	 */
	function load($URL) {
		$CONTENTS = array();
		$fgc_options = stream_context_create(array('ssl' => array(
			'verify_peer'      => false,
			'verify_peer_name' => false
		)));
		if ($URL[0] == "") {
			// 12 links
			foreach ($URL as $key => $url) {
				if ($key < 1) { continue; }

				$url_nohash = preg_replace('/#data-.*$/', '', $url);
				self::$log->add("LOAD()", "$url_nohash 読み込み");
				if($this->curlParams && !$this->patternMode) {
					// パタンーモードでｃUrlを未使用
					$CONTENTS[$key] = $this->getWithCurl($url_nohash);
				}
				else {
					$CONTENTS[$key] = @file_get_contents($url_nohash, false, $fgc_options);
					/*
						Accept-Encodingを無視してgzip圧縮してレスポンスを返すサイトが存在するため、
						ヘッダ情報を確認してgzip形式の場合は解凍する。
					 */
					if ($this->isGzip($CONTENTS[$key])) {
						$CONTENTS[$key] = gzdecode($CONTENTS[$key]);
					}
				}

				// patternMode (local files) = no waiting
				if(!$this->patternMode) {
					sleep(1);
				}

				if ($CONTENTS[$key] === FALSE) {
					self::$log->add("LOAD()", "ERR ページが存在しないか、誤字があります。");
				}
				else {
					// URLの後にある#data-のkeyを使い、データを保存する
					$this->load_save($url, $CONTENTS[$key]);
				}
			}
		} else {
			// 1 main link
			$url_nohash = preg_replace('/#data-.*$/', '', $URL[0]);
			self::$log->add("LOAD()", "$url_nohash 読み込み");
			if($this->curlParams && !$this->patternMode) {
				// パタンーモードでｃUrlを未使用
				$CONTENTS[0] = $this->getWithCurl($url_nohash);
			}
			else {
				$CONTENTS[0] = @file_get_contents($url_nohash, false, $fgc_options);
				if ($this->isGzip($CONTENTS[0])) {
					$CONTENTS[0] = gzdecode($CONTENTS[0]);
				}
			}

			if ($CONTENTS[0] === FALSE) {
				self::$log->add("LOAD()", "ERR サイトのページとして存在しないか、誤字があります。");
			}
			else {
				// URLの後にある#data-のkeyを使い、データを保存する
				$this->load_save($URL[0], $CONTENTS[0]);
			}
		}

		return $CONTENTS;
	}


	/**
	 * ファイルを読み込む時に、パターンフォルダーに保存する
	 *
	 * @author Azet
	 * @param string $folderName_ れい: "ok", "err1" など
	 * @return bool
	 */
	function patternMake($folderName_) {

		// 存在確認
		if(file_exists($folderName_)) {
			self::$log->add('PATTERN_MAKE', 'ERR フォルダー '.$folderName_.' 存在しています');
			return false;
		}

		//作成: recursive
		$patternMakeFolderOk = @mkdir($folderName_, 0777, true);
		if(!$patternMakeFolderOk) {
			self::$log->add('PATTERN_MAKE', 'ERR フォルダー '.$folderName_.' を作成できませんでした');
			return false;
		}

		$this->patternMakeFolder = $folderName_;
		return true;
	}


	/**
	 * コンテンツを保存する
	 * @author Azet
	 * @param string $url_
	 * @param string $content_
	 */
	private function load_save($url_, $content_) {
		$url_data_reg = "/#(data-.*$)/";

		// pattern save
		if($this->patternMakeFolder) {
			//getting file name from URL
			$patternFileName = preg_replace($url_data_reg, '', $url_);
			// file name convert
			$patternFileName = $test_path.self::convertUrlPatternFileName($patternFileName);
			@file_put_contents($this->patternMakeFolder.'/'.$patternFileName, $content_);
		}

		$ok = preg_match($url_data_reg, $url_, $match);
		// saving file in case of plugin data error
		if($ok) {
			$datakey = $match[1];
			$savefile = DATA_SAVE_FOLDER.$datakey;
			//print "SAVING CONTENT OF $url_ TO $savefile".PHP_EOL;
			$ok = @file_put_contents($savefile, $content_);
			if(!$ok) {
				self::$log->add('LOAD_SAVE()', 'ERR データ保存はできませんでした。');
			}
		}
		else {
			self::$log->add('LOAD_SAVE()', 'ERR urlキーがありませんので、コンテンツの情報を保存できません！');
		}

		//バックアップフォルダにページ内容を保存する
		$is_dry_run = ($this->patternMode || $this->patternMakeFolder || $this->testMode);

		//(本番で、テスト実行以外（本実行）のとき または 強制オプションが付いているときはバックアップ
		if ((IS_SERVER && ($is_dry_run === false)) || $this->forceBackup) {
			$this->backup($url_, $content_);
		}
	}

	/**
	 * 取得したhtmlをファイルバックアップする
	 * 
	 * @param string $url_ 
	 * @param string $content_ 
	 * @access private
	 * @return boolean true:成功 | false:失敗
	 */
	private function backup($url_, $content_) {

		if (!$url_ || !$content_) {
			self::$log->add('BACKUP()', 'ERR URLまたは内容が空のため、バックアップは保存はできませんでした。');
			return false;
		}

		$site_dir = sprintf("%06s", $this->id);
		$today = date("Ymd");

		$dir = BACKUP_FOLDER.$today."/".$site_dir."/";
		$f = self::convertUrlToBackupFileName($url_);
		$backup_name = $dir.$f;

		if (!file_exists($dir)) {
  	  mkdir($dir, 0777, true);
		}
		$ok = @file_put_contents($backup_name, $content_);
		if(!$ok) {
			self::$log->add('BACKUP()', 'ERR ファイル作成に失敗したため、バックアップは保存はできませんでした。'.$backup_name);
		}
		system("zip -r ".BACKUP_FOLDER.$today);

		return $ok;
	}

	/**
	 * バックアップフォルダが存在するかチェックする
	 * 
	 * @param string $date_ymd 20191212など
	 * @access public
	 * @return boolean ファイルが存在?
	 */
	public static function backupExists($date_ymd){
		return file_exists(BACKUP_FOLDER.$date_ymd);
	}

	/**
	 * バックアップフォルダを圧縮する
	 *
	 * ---
	 * DIAGNOSTICS
	 * http://linux.math.tifr.res.in/manuals/man/zip.html
	 * 
	 * @param string $date_ymd 20191212など
	 * @access public
	 * @return integer zipコマンドリターンコード(0は成功)
	 */
	public static function archiveBackup($date_ymd){

		//invalid param
		if ($date_ymd == "") { return false; }
		system("~/bin/archivedir.sh ".BACKUP_FOLDER." ".$date_ymd, $ret1);

		//zip ok?
		if ($ret1 == 0) {
			self::$log->add("ZIP-BACKUP", "OK ".$date_ymd." 圧縮成功");
		} else {
			self::$log->add("ZIP-BACKUP", "ERR ".$date_ymd." 圧縮失敗");
		}

		//remove
		system("rm -r ".BACKUP_FOLDER.$date_ymd, $ret2);
		//rm ok?
		if ($ret2 == 0) {
			self::$log->add("DEL-BACKUP", "OK ".$date_ymd." 削除成功");
		} else {
			self::$log->add("DEL-BACKUP", "ERR ".$date_ymd." 削除失敗");
		}

		return (($ret1 + $ret2) == 0);
	}

	//URL形式データから取得に必要なURL情報を作成する
	//$get_type 取得形式　全体　個別など
	//$URL 取得に必要なURLを配列で返す
	static function get_Url_Type($get_type,$row,$data_file_key){
		$URL = array();
		if ($get_type == "1") {
			// single main URL
			$URL[0] = $row["url"].'#'.$data_file_key;
		} elseif ($get_type == "2") {
			// zoodiac URL
			for ($i = 1; $i <= 12; $i++) {
				$set_url_name = "star".$i."_url";
				$URL[$i] = $row[$set_url_name].'#'.$data_file_key.'_'.$i;
			}
		} elseif ($get_type == "3") {
			// other link URL
			$URL[0] = $row["etc_url"].'#'.$data_file_key;
			for ($i = 1; $i <= 12; $i++) {
				$set_url_name = "star".$i."_url";
				$URL[$i] = $row[$set_url_name].'#'.$data_file_key.'_'.$i;
			}
		} elseif ($get_type == "4") {
			for ($i = 1; $i <= 12; $i++) {
				$set_url_name = "star".$i."_url";
				$URL[$i] = $row[$set_url_name].'#'.$data_file_key.'_'.$i;
			}
		}
		return $URL;
	}

	//urlの日付情報を書き換える
	static function url_Date_Replace($URL){
		foreach ($URL as $key => $url){
				$url = str_replace("(md)", date("md"), $url);
				$url = str_replace("(ymd)", date("ymd"), $url);
				$url = str_replace("(m)", date("m"), $url);
				$url = str_replace("(Y)", date("Y"), $url);
				$url = str_replace("(M)", intval(date("m")), $url);
				$url = str_replace("(d)", intval(date("d")), $url);
				$url = str_replace("(dd)", date("d"), $url);

				$URL[$key] = $url;

		}
		return $URL;
	}

	//データチェックしてsql文を作る　総合運
	function make_Sql_data( $data ,$row){

		$sql = "INSERT INTO `log` (
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
		return array($sql,$date_error,$data_size);
	}

	//データチェックしてsql文を作る　運勢ごと
	function make_Topic_Sql_data( $topic_data ,$row,$data_size){
			$topic_sql = "INSERT INTO `topic_log` (
						`site_id`,
						`day`,
						`star`,
						`data_type`,
						`score`,
						`date_create`
					) VALUES ";

			//DBへの各運勢データ書き込み
			foreach ($topic_data as $key => $value) {
				++$data_size;
			// データ確認
				if(!preg_match("/[0-9]+/", $key)) {
					$data_error = true;
					break;
				}

				foreach ($value as $val) {
					if(!preg_match("/[0-9]+/", $val)) {
						$data_error = true;
						break;
					}
				}

				foreach ($value as $k=> $val) {

				if(is_null($val)){ continue;}

				$topic_sql .= "(
						'{$row["site_id"]}',
						CURRENT_DATE,
						'{$key}',
						'{$k}',
						'{$val}',
						CURRENT_TIMESTAMP
					),";
				}
			}
		return array($topic_sql,$date_error,$data_size);
	}

	//データチェックしてsql文を作る　ラッキーシンボルごと
	function makeLuckySymbolSqlData( $lucky_data ,$row,$data_size){
		$lucky_symbol_sql = "INSERT INTO `lucky_symbol_log` (
					`site_id`,
					`day`,
					`star`,
					`symbol_type`,
					`content`,
					`date_create`
				) VALUES ";

		//DBへの各ラッキーシンボルデータ書き込み
		foreach ($lucky_data as $key => $value) {
			$data_error = false;
			++$data_size;
		// データ確認
			if(!preg_match("/[0-9]+/", $key)) {
				$data_error = true;
				break;
			}

			foreach ($value as $val) {
				if(!preg_match("/[0-9]+/", $val)) {
					$data_error = true;
					break;
				}
			}

			foreach ($value as $k=> $val) {

				if(is_null($val)){ continue;}

				$lucky_symbol_sql .= "(
						'{$row["site_id"]}',
						CURRENT_DATE,
						'{$key}',
						'{$k}',
						'{$val}',
						CURRENT_TIMESTAMP
					),";
			}
		}
	return array($lucky_symbol_sql,$data_error,$data_size);
	}

	//DBへデータ保存
	//$sql 入力用に作成されたsql文
	//$conn データベース情報
	//$params テストモードかどうか判断するためのモードパラメーター
	function inport_Data_DB($sql,$conn,$params){
		if($params['test']) {
			//print "SAVE QUERY: $sql".PHP_EOL;
			self::$log->add("PLUGIN {$this->id} DB", 'TEST');
		}else {
			// OK：保存しましょう
			$ok = mysqli_query($conn, $sql);
			if(!$ok) {
				self::$log->add("PLUGIN {$this->id} DB", 'SAVE ERR');
			}
			else {
				self::$log->add("PLUGIN {$this->id} DB", 'SAVE OK');
				$save_ok = true;
			}
		}
		return $save_ok;
	}

	function get_Topic_Data_Main( $row , $params ,$topic_param_arr,$CONTENTS ,$TOPIC_URL ,$conn){
		//各運勢取得ロジック
		$topic_data = NULL;
		$data_size_t = 0;
		$plugin_file =$topic_param_arr['plugin_file'];
		include_once("plugins/$plugin_file");
		if(class_exists("{$topic_param_arr['plugin_call']}")) {
			$plugin = new $topic_param_arr["plugin_call"]($this->id, $this->parent_id);

			if($params['patternMake']) {
				$patternFolder = BAT_PATTERN_TEST_FOLDER.$topic_param_arr["plugin_id"].'/'.$params['patternMake'];
				$this->patternMake($patternFolder);
			}

			if($params['pattern']) {
				$this->setPatternMode();
			}
			$TOPIC_CONTENTS=array();
			//htmlの読み込み
			if(!$topic_param_arr["auto_flag"]){
				if($row["topic_get_type"] == 4){
					$TOPIC_CONTENTS = $plugin-> topic_load($TOPIC_URL);
				}else{
					$TOPIC_CONTENTS = $this -> load($TOPIC_URL);
				}
			}else{
				if( $row["get_type"] != $row["topic_get_type"]){
					if($row["topic_get_type"] == 4){
						$TOPIC_CONTENTS = $plugin-> topic_load($TOPIC_URL);
					}else{
						$TOPIC_CONTENTS = $this -> load($TOPIC_URL);
					}
				}else{
					$TOPIC_CONTENTS = $CONTENTS;
				}
			}
			$topic_data = $plugin->topic_run($TOPIC_CONTENTS);
		}else {
			$msg =  "ERR {$plugin_file} ファイルか、{$plugin_call} クラス がありません。";
			self::$log->add("PLUGIN {$this->id}",$msg);
			return;
		}
		// テストの時に、情報を出す
		if($params['test']) {
			print "DATA: ".print_r($topic_data, true).PHP_EOL;
		}

		//運勢ごと sql文の作成
		list($topic_sql,$date_error,$data_size_t) = $this -> make_Topic_Sql_data( $topic_data ,$row,$data_size_t);
		$topic_sql = rtrim($topic_sql, ',');
		//print $topic_sql;

		if($data_error || $data_size_t!=12) {
			// データエラーの場合は
			self::$log->add("PLUGIN {$this->id} TOPIC_DATA", 'ERR データの形式は間違っている:'.str_replace("\n", '', print_r($data, 1)));
		}else {
			// HTMLファイル削除 >>>
			$data_clear_cmd = 'rm '.DATA_SAVE_FOLDER.$data_file_key.'*';

			//print $data_clear_cmd.PHP_EOL;
			system($data_clear_cmd, $clear_ok);
			if($clear_ok>0) {
				//print "Clear command return: ".$clear_ok.PHP_EOL;
				self::$log->add("PLUGIN {$this->id} TOPIC_DATA", "ERR データファイルの削除ができませんでした: [$data_clear_cmd]");
			}
			// <<<
			// DBにデータを保存
			$topic_ok =$this -> inport_Data_DB($topic_sql,$conn,$params);
		}
		return $topic_ok;
	}

	function getLuckySymbol( $row , $params ,$topic_param_arr,$CONTENTS ,$TOPIC_URL ,$conn){
		//ラッキーシンボル取得ロジック　各運勢と同一ページに表示されているラッキーシンボル取得
		$lucky_symbol_data = NULL;
		$data_size_t = 0;
		$plugin_file =$topic_param_arr['plugin_file'];
		include_once("plugins/$plugin_file");
		if(class_exists("{$topic_param_arr['plugin_call']}")) {
			$plugin = new $topic_param_arr["plugin_call"]($this->id, $this->parent_id);

			if($params['patternMake']) {
				$patternFolder = BAT_PATTERN_TEST_FOLDER.$topic_param_arr["plugin_id"].'/'.$params['patternMake'];
				$this->patternMake($patternFolder);
			}

			if($params['pattern']) {
				$this->setPatternMode();
			}
			$TOPIC_CONTENTS=array();
			//htmlの読み込み
			if(!$topic_param_arr["auto_flag"]){
				if($row["topic_get_type"] == 4){
					$TOPIC_CONTENTS = $plugin-> topic_load($TOPIC_URL);
				}else{
					$TOPIC_CONTENTS = $this -> load($TOPIC_URL);
				}
			}else{
				if( $row["get_type"] != $row["topic_get_type"]){
					if($row["topic_get_type"] == 4){
						$TOPIC_CONTENTS = $plugin-> topic_load($TOPIC_URL);
					}else{
						$TOPIC_CONTENTS = $this -> load($TOPIC_URL);
					}
				}else{
					$TOPIC_CONTENTS = $CONTENTS;
				}
			}
			$lucky_symbol_data = $plugin->lucky_run($TOPIC_CONTENTS);
		}else {
			$msg =  "ERR {$plugin_file} ファイルか、{$plugin_call} クラス がありません。";
			self::$log->add("PLUGIN {$this->id}",$msg);
			return;
		}
		// テストの時に、情報を出す
		if($params['test']) {
			print "DATA: ".print_r($lucky_symbol_data, true).PHP_EOL;
		}

		//ラッキーシンボルごと sql文の作成
		list($lucky_symbol_sql,$date_error,$data_size_t) = $this -> makeLuckySymbolSqlData( $lucky_symbol_data ,$row,$data_size_t);
		$lucky_symbol_sql = rtrim($lucky_symbol_sql, ',');
		

		if($data_error || $data_size_t!=12) {
			// データエラーの場合は
			self::$log->add("PLUGIN {$this->id} TOPIC_DATA", 'ERR データの形式は間違っている:'.str_replace("\n", '', print_r($data, 1)));
		}else {
			// HTMLファイル削除 >>>
			$data_clear_cmd = 'rm '.DATA_SAVE_FOLDER.$data_file_key.'*';

			system($data_clear_cmd, $clear_ok);
			if($clear_ok>0) {
				self::$log->add("PLUGIN {$this->id} TOPIC_DATA", "ERR データファイルの削除ができませんでした: [$data_clear_cmd]");
			}
			// DBにデータを保存
			$lucky_symbol_ok =$this -> inport_Data_DB($lucky_symbol_sql,$conn,$params);
		}
		return $lucky_symbol_ok;
	}


	/**
     * サイトの親データをコピーする
     * CONTENTがなければ、ページがなくなりましたか？
	 * その場合は何もしない
	 *
     * @param array[string] $CONTENTS
     * @return array data[star] = rank;
     * 星座（インデックス） => 順位
     */
	public function getParentData($CONTENTS) {
		if($this->parent_id==0) {
			self::$log->add("PLUGIN {$this->id} getParentData()", "ERR No parent ID.");
			return null;
		}

		if(!is_array($CONTENTS) || array($CONTENTS)==0) {
			self::$log->add("PLUGIN {$this->id} getParentData()", "ERR No content from Site?");
			return null;
		}

		$data = array();
		// today
		$today = self::getTodayISO();

		// parent data query
		$sql = "SELECT * FROM log WHERE is_delete = 0 AND day = '$today' AND site_id = {$this->parent_id}";

		/** @var \mysqli_result $rs */
		$rs = self::$conn->query($sql);

		while($row = $rs->fetch_assoc()) {
			$star_id = $row['star'];
			$data[$star_id] = $row['rank'];
		}

		return $data;
	}


	/**
     * サイトの親データをコピーする
     * Topic版
	 * CONTENTがなければ、ページがなくなりましたか？
	 * その場合は何もしない
     *
     * @param array[string] $CONTENTS
     * @return array
     * array(
     *   1 => array('love' => X, 'money' => Y, 'work' => Z)
     *   ...
     * )
     * X, Y, Z は順位(スコア)
     */
	public function getParentDataTopic($CONTENTS) {
		if($this->parent_id==0) {
			self::$log->add("PLUGIN {$this->id} getParentDataTopic()", "ERR No parent ID.");
			return null;
		}

		if(!is_array($CONTENTS) || array($CONTENTS)==0) {
			self::$log->add("PLUGIN {$this->id} getParentDataTopic()", "ERR No content from Site?");
			return null;
		}

		$data = array();
		// today
		$today = self::getTodayISO();

		// parent data query
		$sql = "SELECT * FROM topic_log WHERE is_delete = 0 AND day = '$today' AND site_id = {$this->parent_id} ";

		/** @var \mysqli_result $rs */
		$rs = self::$conn->query($sql);
		$lines = array();
		while($row = $rs->fetch_assoc()) {
			$lines[] = $row;
		}

		// prepare all lines
		$data = array_reduce($lines, function($carry, $elem) {
			$star = $elem['star'];
			$type = $elem['data_type'];
			if(!is_array($carry[$star])) {
				$carry[$star] = array();
			}
			$carry[$star][$type] = $elem['score'];
			return $carry;
		}, array());

		return $data;
	}

	/**
	 * 文字列がgzip形式か判定します。
	 *
	 * @param string $contents 判定文字列
	 * @return boolean gzipファイルの場合true
	 */
	public function isGzip($contents) {
		$gzip_header = [0x1F, 0x8B, 0x08];
		$header = substr($contents, 0, count($gzip_header));
		return array_map(function ($c) {
			return ord($c);
		}, str_split($header)) === $gzip_header;
	}

	/**
	 *
	 * UranaiPlugin::runの返却値の配列をtsvに変換する
	 * ※たまに手動取得が必要な時の対応用! アプリケーションで使いません。
	 *
	 * @param array $result_array UranaiPlugin::runのの返却値
	 * @example
	 * プラグインクラスのrunメソッドの中で:
	 *  echo self::_resultsToTsv($RESULT);
	 * @return string
	 * みずがめ座	うお座	おひつじ座	おうし座	ふたご座	かに座	しし座	おとめ座	てんびん座	さそり座	いて座	やぎ座
	 * 12	6	7	1	2	3	11	4	8	10	9	5
	 *
	 */
	public static function _resultsToTsv($result_array) {
		$out = "";
		$result_array_ = $result_array;
		ksort($result_array_);
		$out .= rtrim(array_reduce(array_flip(self::$starDefault), function($csv_header, $star_name) {
			$csv_header .= $star_name . "\t";
			return $csv_header;
		}, ""), "\t") . "\n";
		foreach ($result_array_ as $result) {
			$out .= $result . "\t";
		}
		$out = rtrim($out, "\t")."\n";
		return $out;
	}

	/**
	 *
	 * UranaiPlugin::topic_runの返却値の配列をtsvに変換する
	 * ※たまに手動取得が必要な時の対応用! アプリケーションで使いません。
	 *
	 * @param array $result_array UranaiPlugin::topic_runのの返却値
	 * @example
	 * プラグインクラスのtopic_runメソッドの中で:
	 *  echo self::_topicResultsToTsv($TOPIC_RESULT);
	 *
	 * @return string
	 * 運勢|星座	みずがめ座	うお座	おひつじ座	おうし座	ふたご座	かに座	しし座	おとめ座	てんびん座	さそり座	いて座	やぎ座
	 * work	20	60	80	100	60	100	20	60	40	40	60	40
	 * interpersonal	40	60	80	80	60	80	60	80	60	20	80	60
	 * money	40	40	60	80	80	60	20	100	80	60	20	80
	 * outing	20	80	40	100	80	60	60	80	40	40	20	60
	 * love	20	80	60	80	80	60	40	60	80		40	60
	 * health	20	40	40	100	80	80	60	60	60	60	40	60
	 *
	 */
	public static function _topicResultsToTsv($result_array) {
		$out = "";
		if (empty($result_array)) {
			return $out;
		}
		$out = "運勢|星座\t";
		$out .= rtrim(array_reduce(array_flip(self::$starDefault), function($csv_header, $star_name) {
			$csv_header .= $star_name . "\t";
			return $csv_header;
		}, ""), "\t") . "\n";
		$result_array_ = $result_array;
		ksort($result_array_);
		$topics = array_keys(array_shift($result_array));
		foreach ($topics as $topic) {
			$out .= $topic . "\t";
			foreach ($result_array_ as $result) {
				$out .= (isset($result[$topic]) ? $result[$topic] : "") . "\t";
			}
			$out = rtrim($out, "\t");
			$out .= "\n";
		}
		return $out;
	}
}
