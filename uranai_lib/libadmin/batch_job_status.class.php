<?php
/**
 *
 * NOTE:
 *  dbコネクションはglobal変数を使う
 */
class BatchJobStatus {

	private $site_status = [];
	// private $log_file;
	// public function __construct ($log_file) {
	// 	$this->log_file = $log_file;
	// 	return $this;
	// }

	/**
	 * 取得状況サマリーをDBから取得
	 * 
	 * @return BatchJobStatus
	 */
	public function load() {
		global $conn;

		$day = date("Y-m-d");

		$sql = "SELECT
			'{$day}' AS 'date',
			s.site_id,
			s.site_name,
			s.site_get_time,
			s.is_execute,
			s.site_topic,
			COUNT(DISTINCT l.site_id) AS 'log_exists',
			COUNT(tl.log_id) AS 'topic_log_exists',
			GROUP_CONCAT(tl.data_type) AS 'topics'
			FROM site s
			LEFT JOIN log l ON s.site_id = l.site_id
				AND l.`day` = '{$day}'
				AND l.is_delete = 0
			LEFT JOIN topic_log tl ON s.site_id = tl.site_id
				AND tl.`day` = '{$day}'
				AND tl.is_delete = 0
			WHERE 1
				-- AND s.is_execute = 1 -- 総合運の実行フラグOFFでも取る
				AND s.is_delete = 0
			GROUP BY s.site_id";

		//*cron設定が変わったらココをいじらないといけない
		$interval_per_hour = date("H:05:00"); //毎時05分
			
		if($rs = mysqli_query($conn, $sql)) {
			while($row = mysqli_fetch_assoc($rs)){

				$is_active = $row['is_execute'] || $row['site_topic']; //有効か

				//バッチが実行済みか否か
				//実行時刻を過ぎている かつ 総合運取得がON 又は トピック運取得がON
				$batch_done = ($row['site_get_time']<= $interval_per_hour) && $is_active;
				
				//バッチ結果が成功か否か
				//バッチが実行済み かつ 総合運がONならlogが出来ている, トピック運がONならtopic_logができている
				$is_success = ($batch_done && 
					(
						($row['is_execute'] && $row['log_exists'] == 1) ||
						($row['site_topic'] && $row['topic_log_exists'] == 1)
					));


				// echo "<pre>"; print_r($row); echo "</pre>";
				$this->site_status[] = [
					'date' => $row['date'], //日
					'site_id' => $row['site_id'], //サイトID
					'site_name' => $row['site_name'], //サイト名
					'site_get_time' => $row['site_get_time'], //取得時間
					'is_execute' => $row['is_execute'], //総合運が有効か
					'site_topic' => $row['site_topic'], //トピック運取得が有効か
					'is_active' => $is_active, //有効か
					'log_exists' => $row['log_exists'] > 0, //総合運ログがあるか
					'topic_log_exists' => $row['topic_log_exists'] > 0, //トピック運ログが有るか
					'topics' => $row['topics'],
					'batch_done' => $batch_done,
					'is_success' => $is_success
				];
				// echo "<pre>"; echo var_export($summary,true); echo "</pre>";
			}
		}
		// else {
		// 	echo mysqli_error($conn);
		// }

		return $this;
	}

	/**
	 * 取得状況を得る
	 * 
	 * @return array
	 */
	public function getSiteStatus() {
		return $this->site_status;
	}

	/**
	 * プラグイン数を数える
	 * 
	 * @return int
	 */
	public function getPluginCount() {
		return count($this->site_status);
	}

	/**
	 * フィルター関数で絞り込んだプラグイン数を数える
	 * 
	 * @param Callable $filter_fn 
	 * @return int
	 */
	private function getPluginCountByFilter($filter_fn) {
		return count(array_filter($this->site_status, $filter_fn));
	}
	
	/**
	 * 稼働中プラグイン数を数える
	 * 
	 * @return int
	 */
	public function getActivePluginCountAll() {
		return $this->getPluginCountByFilter(function($v) {
			return $v['is_active'];
		});
	}

	/**
	 * 稼働中プラグイン数(総合運)を数える
	 * 
	 * @return int
	 */
	public function getActivePluginCountSougo() {
		return $this->getPluginCountByFilter(function($v) {
			return $v['is_active'] && $v['is_execute'];
		});
	}

	/**
	 * 稼働中プラグイン数(トピック運)を数える
	 * 
	 * @return int
	 */
	public function getActivePluginCountTopic() {
		return $this->getPluginCountByFilter(function($v) {
			return $v['is_active'] && $v['site_topic'];
		});
	}

	/**
	 * 実行成功プラグイン数を数える
	 * 
	 * @return int
	 */
	public function getSuccessCount() {
		return $this->getPluginCountByFilter(function($v) {
			return $v['is_active'] && $v['is_success'];
		});
	}

	/**
	 * 実行失敗プラグイン数を数える
	 * 
	 * @return int
	 */
	public function getFailCount() {
		return $this->getPluginCountByFilter(function($v) {
			return !$v['is_success'] && $v['batch_done'] == 1;
		});
	}

	/**
	 * 未実行プラグイン数を数える
	 * 
	 * @return int
	 */
	public function getPendingCount() {
		return $this->getPluginCountByFilter(function($v) {
			return $v['is_active'] && $v['batch_done'] != 1;
		});
	}

}


