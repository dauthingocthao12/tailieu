<?php

// Load the Google API PHP Client Library.
require_once __DIR__ . '/vendor/autoload.php';

//いろいろアクセスデータ見る用
class Analysis{

	const GA_PROFILE = 113769931;

	private $ga;
	private $ga_profile;

	private $date_start;
	private $date_end;

	public function __construct($date_start, $date_end){
		$this->date_start = $date_start;
		$this->date_end = $date_end;
		$this->ga = $this->initializeAnalytics();
		$this->ga_profile = self::GA_PROFILE;
		// $this->ga_profile = $this->getFirstProfileId($this->ga);
	}

	public function registed_user(){
		global $conn;

		$user_count = 0;

		$sql = "SELECT COUNT(*) AS count";
		$sql.= " FROM `users`";
		$sql.= " WHERE 1";
		$sql.= " AND is_delete = 0";
		$sql.= " AND DATE(date_create) BETWEEN '".$this->date_start."' AND '".$this->date_end."'";

		$rs = $conn->query($sql);
		if($rs) {
			$row = $rs->fetch_assoc();
			$user_count = $row['count'];
		}
		return $user_count;
	}

	public function all_user(){
		global $conn;

		$user_count = 0;

		$sql = "SELECT COUNT(*) AS count";
		$sql.= " FROM `users`";
		$sql.= " WHERE 1";
		$sql.= " AND is_delete = 0";

		$rs = $conn->query($sql);
		if($rs) {
			$row = $rs->fetch_assoc();
			$user_count = $row['count'];
		}
		return $user_count;
	}

	public function deleted_user(){
		global $conn;

		$user_count = 0;

		$sql = "SELECT COUNT(*) AS count";
		$sql.= " FROM `users`";
		$sql.= " WHERE 1";
		$sql.= " AND is_delete = 1";
		$sql.= " AND DATE(date_create) BETWEEN '".$this->date_start."' AND '".$this->date_end."'";

		$rs = $conn->query($sql);
		if($rs) {
			$row = $rs->fetch_assoc();
			$user_count = $row['count'];
		}
		return $user_count;
	}

	public function plugins(){
		global $conn;

		$count = 0;

		$sql = "SELECT COUNT(*) AS count";
		$sql.= " FROM `site`";
		$sql.= " WHERE 1";
		$sql.= " AND is_execute = 1";
		$sql.= " AND is_delete = 0";

		$rs = $conn->query($sql);
		if($rs) {
			$row = $rs->fetch_assoc();
			$count = $row['count'];
		}
		return $count;
	}

	public function recieved_mail(){
		$mails = array();

		$d_start = strtotime($this->date_start);
		$d_end = strtotime($this->date_end);

		$f = @file("/home/uranairank/uranairank00001/log/ansmail.log");
		// $f = @file("D:\ansmail.log"); //Windows local
		if(is_array($f) && count($f) > 0){
			foreach($f as $line){
				$columns = explode(" ", $line);
				$d = $columns[0];
				//期間内メールだけ取得
				if((strtotime($d) >= $d_start) && strtotime($d) <= $d_end){
					$mails[] = htmlspecialchars($line);
				}
			}
		}
		return $mails;
	}

	public function get_ga_data($metrics, $option = array()){

		$results = $this->ga->data_ga->get(
			'ga:' . $this->ga_profile,
			$this->date_start,
			$this->date_end,
			$metrics,
			$option
		);
		$rows = $results->getRows();
		if (empty($rows)) {
			return null;
		}
		return $rows;
	}

	public static function get_weekday_session($session_data_arr){

		//平日セッション
		$weekday_session = 0;

		// print_r($session_data_arr);
		foreach($session_data_arr as $sess){
			$w = date("w", strtotime($sess[0]));
			//日曜でも土曜でもなければ加算
			if($w != 0 && $w != 6){
				$weekday_session+= $sess[1];
			}
		}
		return $weekday_session;
	}

	public static function sum_referrer_result($rows){
		$ret = 0;
		if (empty($rows)) { return $ret; }
		foreach($rows as $val_arr){
			foreach($val_arr as $key => $val){
				if($key == 1){
					$ret+= $val;
				}
			}
		}
		return $ret;
	}

	private function initializeAnalytics(){
		// Creates and returns the Analytics Reporting service object.

		// Use the developers console and download your service account
		// credentials in JSON format. Place them in this directory or
		// change the key file location if necessary.
		$KEY_FILE_LOCATION = __DIR__ . '/service-account-credentials.json';

		// Create and configure a new client object.
		$client = new Google_Client();
		$client->setApplicationName("Hello Analytics Reporting");
		$client->setAuthConfig($KEY_FILE_LOCATION);
		$client->setScopes(['https://www.googleapis.com/auth/analytics.readonly']);
		$analytics = new Google_Service_Analytics($client);

		return $analytics;
	}

	private function getFirstProfileId($analytics) {
		// Get the user's first view (profile) ID.

		// Get the list of accounts for the authorized user.
		$accounts = $analytics->management_accounts->listManagementAccounts();

		if (count($accounts->getItems()) > 0) {
			$items = $accounts->getItems();
			$firstAccountId = $items[0]->getId();

			// Get the list of properties for the authorized user.
			$properties = $analytics->management_webproperties
				->listManagementWebproperties($firstAccountId);

			if (count($properties->getItems()) > 0) {
				$items = $properties->getItems();
				$firstPropertyId = $items[0]->getId();

				// Get the list of views (profiles) for the authorized user.
				$profiles = $analytics->management_profiles
					->listManagementProfiles($firstAccountId, $firstPropertyId);

				if (count($profiles->getItems()) > 0) {
					$items = $profiles->getItems();

					// Return the first view (profile) ID.
					return $items[0]->getId();

				} else {
					throw new Exception('No views (profiles) found for this user.');
				}
			} else {
				throw new Exception('No properties found for this user.');
			}
		} else {
			throw new Exception('No accounts found for this user.');
		}
	}
	
}
