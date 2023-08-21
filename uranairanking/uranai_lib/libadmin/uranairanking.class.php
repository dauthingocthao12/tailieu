<?php

class UranaiRanking {

	protected $ranks = array();
	protected $date = null;
	static public $log;
	protected $updown_rank = array();//add hirose 2017/03/09


	/**
	 * ログ用インスタンスを登録
	 *
	 * @author Azet
	 * @param Log $log_
	 */
	static public function setLogObject($log_) {
		self::$log = $log_;
	}

	/**
	 * logのsite_idを計算する機能
	 *
	 * @author Azet
	 * @param string $date_ (例：2016-03-02)
	 * @return int
	 */
	static function countLogsSites($date_) {
		global $conn;
		$num = 0;

		$sql = "SELECT star, count(*)
			FROM `log`
			WHERE is_delete=0
			AND`day` = {$date_}
			GROUP BY star";
		$result2 = $conn->query($sql);
		if($result2) {
			$num = $result2->fetch_assoc();
		}

		return $num["count(*)"];
	}

	/**
	 * topic_logのsite_idを計算する機能	add yamaguchi 2017/06/08
	 *
	 * @author Azet
	 * @param string $date_ (例：2016-03-02)
	 * @return int
	 */
	static function countTopicLogsSites($date_,$data_type) {
		global $conn;
		$num = 0;
		$sql = "SELECT star, count(*)
			FROM `topic_log`
			WHERE is_delete=0
			AND `data_type` = '{$data_type}'
			AND `day` = $date_
			GROUP BY star";
		$result2 = $conn->query($sql);
		if($result2) {
			$num = $result2->fetch_assoc();
		}
		return $num["count(*)"];
	}

	/**
	 * ランキングオブジェクトを作成
	 *
	 * @author Azet
	 * @param string $date_ (Y-m-d)
	 */
	function __construct($date_,$data_type = NULL) {
		global $conn, $name, $en_star, $num_star;

		// saving date of logs
		$this->date = $date_;

		$sql = "
				SELECT
					star, sum(rank)
				FROM
					`log` l
				JOIN `site` s
					ON s.is_delete=0 AND s.parent_id=0 AND s.site_id=l.site_id
				WHERE l.is_delete=0
					AND l.`day`='{$date_}'
					GROUP BY l.star
					ORDER BY SUM(rank) ASC, l.star ASC
			";
		// TEST
		//print $sql;

		$result = mysqli_query($conn, $sql);

		// add okabe start 2016/06/08
		// １日前のランキングも取得し、上下変動を調べる
		$date_ = str_replace("-","",$date_);	//add okabe 2016/06/17★

		$tmp_prevday = substr($date_, 0, 4)."-".substr($date_, 4, 2)."-".substr($date_, 6, 2);
		$prevday = date('Ymd', strtotime($tmp_prevday." -1 day"));
		$sql2 = "
				SELECT
					star, sum(rank)
				FROM
					`log` l
				JOIN `site` s
					ON s.is_delete=0 AND s.parent_id=0 AND s.site_id=l.site_id
				WHERE l.is_delete=0
					AND l.`day`='{$prevday}'
					GROUP BY l.star
					ORDER BY SUM(rank) ASC, l.star ASC
			";
		// TEST
		//print $sql2;

		$result2 = mysqli_query($conn, $sql2);
		$updown_info = array();
		$rank = 0;
		$last_value = 0;
		$incr = 1;
		while ($data = mysqli_fetch_assoc($result2)) {
			$star = $name["star{$data["star"]}"];
			$value = $data["sum(rank)"];
			if ($value != $last_value && $last_value !== null) {
				$rank += $incr;
				$incr = 1;
			} else {
				$incr++;
			}
			$last_value = $value;	//add okabe 2016/06/22
			$updown_info[$star] = $rank;
		}
		// add okabe end 2016/06/08

		$rank = 0;
		$last_value = 0;
		$incr = 1;
		while ($data = mysqli_fetch_assoc($result)) {
			$star = $name["star{$data["star"]}"];
			//print $data['star'];

			$value = $data["sum(rank)"];

			if ($value != $last_value && $last_value !== null) {
				$rank += $incr;
				$incr = 1;
			} else {
				$incr++;
			}
			$last_value = $value;	//add okabe 2016/06/22

			// add okabe start 2016/06/08
			// $updown_info[$star] には前日の順位が入っている, $rank は今日の順位
			if ($updown_info[$star] == $rank) {
				//$updown_mark = "<span class=\"rank-equal\">=</span>".$updown_info[$star];
				$updown_mark = "<i class=\"fa fa-minus fa-updown-deco fa-lg\" aria-hidden=\"true\"></i>";
				$updown_mail = "－";
				$updown_rank = "変化はありませんでした";//add hirose 2017/03/09
			} else if ($updown_info[$star] > $rank) {
				//$updown_mark = "<span class=\"rank-down\">↑</span>".$updown_info[$star];
				$updown_mark = "<i class=\"fa fa-arrow-up fa-updown-deco fa-lg\" aria-hidden=\"true\"></i>";
				$updown_mail = "↑";
				// （未使用）アップ・ダウンの単語が反対かもしれない。?
				$updown_rank = "$updown_info[$star]" - "$rank".位アップしました; //add hirose 2017/03/09
			} else {
				//$updown_mark = "<span class=\"rank-up\">↓</span>".$updown_info[$star];
				$updown_mark = "<i class=\"fa fa-arrow-down fa-updown-deco fa-lg\" aria-hidden=\"true\"></i>";
				$updown_mail = "↓";
				// （未使用）アップ・ダウンの単語が反対かもしれない。?
				$updown_rank = "$rank" - "$updown_info[$star]".位ダウンしました;//add hirose 2017/03/09
			}
			// add okabe end 2016/06/08

			$this->ranks[] = array(
				"num" => $rank,
				"name" => $star,
				"en_name" => $en_star["$star"],
				"star_num" => $num_star[$star]
				,"updown" => $updown_mark		// add okabe start 2016/06/08
				,"updown_mail" => $updown_mail		// add okabe start 2016/06/17
				,"points" => $value		//add okabe 2016/07/12
//,"message" => $this->getSeizaMessage($num_star[$star], $points)	//add okabe 2016/07/09	一旦非公開 2016/07/11
			);
			//add hirose start 2017/03/09
			$this->updown_rank[] = array(
			"$num_star[$star]" => $updown_rank,
			);//add hirose end 2017/03/09

			$last_value = $value;
		}
	}


/*
	その日の星座別メッセージ取得関数
	add 2016/07/09
*/
	function getSeizaMessage($star, $points) {
		global $msg_data_file;
		$md5sub = hexdec( substr(md5($points), -4) ) % 10;

		$file_name = DATA_FOLDER.$msg_data_file[$star];

		$filep = fopen($file_name, "r");
		$msg = "undefined:".$md5sub."*".$file_name;
		if($filep){
			$dummy = fgets($filep);
			while(substr($dummy, 0, 2) == "//") {
				$dummy = fgets($filep);
			}
			$max_num = trim($dummy);
			if (intval($max_num) > 0) {
				$cnt = 0;
				while($cnt < $max_num && $line = fgets($filep)) {
					if ($cnt == $md5sub) {
						$msg = $line;
						$cnt = 999;
					}
					$cnt++;
				}
			}
			fclose($filep);
		}
		$result = $msg;

		return $result;
	}


	/**
	 * ランキングを出す
	 *
	 * @author Azet
	 * @return array
	 *
	 * リターンの例:
	 * Array
	 * (
	 * 	[0] => Array
	 * 	  (
	 * 	  	[num] => 1
	 * 	  	[name] => ふたご座
	 * 	  	[en_name] => gemini
	 * 	  	[star_num] => 5
	 * 	  )
	 * 	...
	 * 	)
	 */
	function getRanks() {
		return $this->ranks;
	}

	/********アップダウンの順位差異を出す******* add hirose start 2017/03/09*/
	function getUpdown(){
		return $this->updown_rank;
	}
	/* add hirose end*/

	/**
	 * 星座番号によってランキングを出す
	 *
	 * @author Azet
	 * @param int $num_
	 * @return array
	 *  ie: Array(
	 * 	  	[num] => 1
	 * 	  	[name] => ふたご座
	 * 	  	[en_name] => gemini
	 * 	  	[star_num] => 5
	 * 	  )
	 */
	function getRankForStar($num_) {
		$rank = array();
		
		for($k=0; $k<count($this->ranks); ++$k) {
			if($this->ranks[$k]['star_num']==$num_) {
				$rank = $this->ranks[$k];
				break;
			}
		};
		// one record only
		return $rank;
	}


	/**
	 *
	 * @param int $star_
	 * @param int $rank_
	 * @return array
	 */
	function getDetailsForStarRank($star_, $data_type=null, $rank_=null) {
		global $conn, $name, $en_star;
		$logs = array();

		$now = date("H:i:s"); // 現在時刻

		$sql = "SELECT";
		$sql .= " log.day, site.site_id, site.site_name, site.past_days, site.future_days , site.future_flag ,";
		$sql .= " site.limit_time , log.star,";
		if($data_type == ""){
			$sql .= " log.rank";
		}else{
			$sql .= " log.score";
		}
		$sql .= ",";
		$sql .= " url,".
			" sp_url,".
			" site.link_url,".
			" site.sp_link_url,".
			" site.star".$star_."_url,".
			" site.sp_star".$star_."_url,";

		if($data_type == ""){
			$sql .= " lu.all_link_url,".
				" lu.all_sp_link_url,".
				" lu.star".$star_."_link_url,".
				" lu.sp_star".$star_."_link_url";
		}else{
			$sql .= " lvu.link_love_url,".
				" lvu.sp_link_love_url,".
				" lvu.star".$star_."_link_love_url,".
				" lvu.sp_star".$star_."_link_love_url";
		}
		$sql .= " FROM `site` site";

		if($data_type == ""){
			$sql .= " INNER JOIN `log` log";
			$sql .= " LEFT JOIN `link_url` lu ON lu.site_id = site.site_id";
		}else{
			$sql .= " INNER JOIN `topic_log` log";
			$sql .= " LEFT JOIN `link_love_url` lvu ON lvu.site_id = site.site_id";
		}
		$sql .= " WHERE log.`is_delete`=0 AND log.`day`='{$this->date}' AND site.`is_delete`=0";
		$sql .= " AND site.site_id=log.site_id";
		$sql .= " AND log.`star`='{$star_}'";

		if($data_type != "") {
			$sql .= " AND log.`data_type` = '{$data_type}'";
		}
		if($rank_) {
			$sql .= " AND log.`rank`={$rank_}";
		}

		if($data_type == ""){
			$sql .= " ORDER BY log.rank,site.site_furigana";
		}else{
			$sql .= " ORDER BY log.score DESC,site.site_furigana";
		}

		$result = mysqli_query($conn, $sql);

		if($result) {
			while ($data = mysqli_fetch_assoc($result)) {
				$site_id = $data['site_id'];
				$site_url = "";
				$site_name = $data["site_name"];
				$date = str_replace('-', '', $this->date);

				$is_sphone = $this->isMobile();
				//---モバイル---
				if($is_sphone || preg_match("/Azet .*\s?.* App/",$_SERVER['HTTP_USER_AGENT'])) {//モバイル端末か有料か無料のアプリ){
					//---総合運---
					if($data_type == ""){
						if($data["sp_star".$star_."_link_url"] != ""){
							$site_url = $data["sp_star".$star_."_link_url"]; // モバイル 星座別 総合運
						}else if($data["all_sp_link_url"] != ""){
							$site_url = $data["all_sp_link_url"]; // モバイル 一覧 総合運
						}else if($data["star".$star_."_link_url"] != ""){
							$site_url = $data["star".$star_."_link_url"]; // PC 星座別 総合運
						}else if($data["all_link_url"] != ""){
							$site_url = $data["all_link_url"]; // PC 一覧 総合運
						}
					//---{トピック}運---
					}else{
						if($data["sp_star".$star_."_link_".$data_type."_url"] != ""){ 
							$site_url = $data["sp_star".$star_."_link_".$data_type."_url"]; // モバイル 星座別 別運勢
						}else if($data["sp_link_".$data_type."_url"] != ""){
							$site_url = $data["sp_link_".$data_type."_url"]; // モバイル 一覧 別運勢
						}else if($data["star".$star_."_link_".$data_type."_url"] != ""){ 
							$site_url = $data["star".$star_."_link_".$data_type."_url"]; // PC 星座別 別運勢
						}else if($data["link_".$data_type."_url"] != ""){
							$site_url = $data["link_".$data_type."_url"]; // PC 一覧 別運勢
						}
					}
				//---PC---
				}else{
					//---総合運---
					if($data_type == ""){
						if($data["star".$star_."_link_url"] != ""){
							$site_url = $data["star".$star_."_link_url"]; // PC 星座別 総合運
						}else if($data["all_link_url"] != ""){
							$site_url = $data["all_link_url"]; // PC 一覧 総合運
						}
					//---{トピック}運---
					}else{
						if($data["star".$star_."_link_".$data_type."_url"] != ""){ 
							$site_url = $data["star".$star_."_link_".$data_type."_url"]; // PC 星座別 別運勢
						}else if($data["link_".$data_type."_url"] != ""){
							$site_url = $data["link_".$data_type."_url"]; // PC 一覧 別運勢
						}
					}
				}

				//各運勢用URLがなければプラグイン用に使用しているURLを表示する
				if($site_url == ""){
					//---プラグインの取得用URL---
					if($data["sp_star".$star_."_url"] != ""){
						$site_url = $data["sp_star".$star_."_url"]; // モバイル 星座別 取得用
					}else if($data["sp_url"] != ""){
						$site_url = $data["sp_url"]; // モバイル 一覧 取得用
					}else if($data["sp_link_url"] != ""){
						$site_url = $data["sp_link_url"]; // モバイル リンクURL
					}else if($data["star".$star_."_url"] != ""){
						$site_url = $data["star".$star_."_url"]; // PC 星座別 取得用
					}else if($data["url"]){
						$site_url = $data["url"]; // PC 一覧 取得用
					}else if($data["link_url"] != ""){
						$site_url = $data["link_url"]; // PC リンクURL
					}else{
						$site_url = $data["etc_url"]; //その他URL
					}
				}

				//情報更新時刻を過ぎていたら※印
				if($date == date("Ymd")){
					if($data["limit_time"]){
						$limit_time = date("H:i:s",strtotime($data["limit_time"]));
						if($now >= $limit_time ){
							$site_name = "※".$site_name;
						}
					}
				}else if($data["past_days"] == "0D"){
					$site_name = "※".$site_name;
				}
				//過去のデータがあるが、データが存在する最古の日より前の日付を閲覧中は最新情報を表示
				if($data["past_days"] != "0D"){
					$datetime_obj = new DateTime();
					$datetime_obj->sub(new DateInterval("P".$data["past_days"]));
					$date_exist_start = $datetime_obj->format('Ymd');
					if($date < $date_exist_start){
						$date = date("Ymd"); //今日の日付
						$site_name = "※".$site_name; //※つける
					}
				}

				$site_url = str_replace("(md)", date("md", strtotime($date)), $site_url);
				$site_url = str_replace("(ymd)", date("ymd", strtotime($date)), $site_url);
				$site_url = str_replace("(Y)", date("Y", strtotime($date)), $site_url);
				$site_url = str_replace("(M)", intval(date("m", strtotime($date))), $site_url);
				$site_url = str_replace("(m)", date("m", strtotime($date)), $site_url);
				$site_url = str_replace("(d)", intval(date("d", strtotime($date))), $site_url);
				$site_url = str_replace("(dd)", date("d", strtotime($date)), $site_url);
				$site_url = str_replace("(yesterday)",date("d",strtotime("-1 day",strtotime($date))),$site_url);

				if($data_type == ""){
					$rank = $data['rank'];
				}else{
					for($i = 0;$i <= 100; $i += 10){
						if($i <= $data['score'] && $data['score'] < ($i +10)){
							$rank = $i;
						}
					}
				}

				$rankings = array(
					"num" => $rank	// num is actually "rank" badly named
					,"site" => $site_name
					,"site_url" => $site_url
					,"site_id" => $site_id
				);

				$logs[$rank][] = $rankings;
			}
		}
		//pre($logs);
		return $logs;
	}
	/**
	 * log of one specific rank at specific date (for details)
	 *
	 * @author Azet
	 * @param int $star_
	 * @param int $rank_
	 * @see libadmin/common.php is_show()
	 * @return array
	 */
	 
	function getDetailsForStarRank_OLD($star_,$data_type=null, $rank_=null) {
		global $conn, $name, $en_star;
		$logs = array();

		// int like date
		$date = str_replace('-', '', $this->date);

		$sql = "SELECT";
		$sql .= " log.day, site.site_name, site.site_furigana, site.past_days, site.future_days , site.limit_time , log.star,";
		if($data_type == ""){
			$sql .= " log.rank";
		}else{
			$sql .= " log.score";
		}
		$sql .= " , site.url, site.sp_url, site.get_type, site.etc_url, site.link_url";
		$sql .= " , site.star1_url, site.star2_url, site.star3_url, site.star4_url, site.star5_url, site.star6_url";
		$sql .= " , site.star7_url, site.star8_url, site.star9_url, site.star10_url, site.star11_url, site.star12_url";
		$sql .= " , site.sp_star1_url, site.sp_star2_url, site.sp_star3_url, site.sp_star4_url, site.sp_star5_url, site.sp_star6_url";
		$sql .= " , site.sp_star7_url, site.sp_star8_url, site.sp_star9_url, site.sp_star10_url, site.sp_star11_url, site.sp_star12_url";
		if($data_type == ""){
			$sql .= " FROM `site` site, `log` log";
		}else{
			$sql .= " FROM `site` site, `topic_log` log";
		}
		$sql .= " WHERE log.`is_delete`=0 AND log.`day`='{$this->date}' AND site.`is_delete`=0";
		$sql .= " AND site.site_id=log.site_id";
		$sql .= " AND log.`star`='{$star_}'";
		if($data_type != "") {
			$sql .= " AND log.`data_type` = '{$data_type}'";
		}
		if($rank_) {
			$sql .= " AND log.`rank`={$rank_}";
		}
		if($data_type == ""){
			$sql .= " ORDER BY log.rank ,site.site_furigana";
		}else{
			$sql .= " ORDER BY log.score DESC ,site.site_furigana ";
		}
		//echo "$sql";
		$result = mysqli_query($conn, $sql);
		if($result) {
			while ($data = mysqli_fetch_assoc($result)) {
				$star = $data["star"];

				// URL処理 >>>
				$is_sphone = self::isMobile();
				if ($is_sphone) {
					if ($data["sp_url"] != "") {
						$site_url = $data["sp_url"];
					} elseif ($data["url"] != "") {
						$site_url = $data["url"];
					} elseif ($data["link_url"] != "") {
						$site_url = $data["link_url"];
					} else {
						$site_url = "sp_star".$star."_url";
						$site_url = $data[$site_url];
						if ($site_url == "") {
							$site_url = "star".$star."_url";
							$site_url = $data[$site_url];
						}						
					}
				} else { // PC
					$url = $data["url"];
					if ($url != "") {
						$site_url = $url;
					} elseif ($data["link_url"] != "") {
						$site_url = $data["link_url"];
					} else {
						$site_url = "star".$star."_url";
						$site_url = $data[$site_url];
					}
				}

				if (is_show($date, $data["past_days"] , $data["limit_time"]) == FALSE) {
					$site_name = "※" . $data["site_name"];
					$site_url = str_replace("(md)", date("md"), $site_url);
					$site_url = str_replace("(ymd)", date("ymd"), $site_url);
					$site_url = str_replace("(Y)", date("Y"), $site_url);
					$site_url = str_replace("(M)", intval(date("m")), $site_url);
					$site_url = str_replace("(m)", date("m"), $site_url);
					$site_url = str_replace("(d)", intval(date("d")), $site_url);
					$site_url = str_replace("(dd)", date("d"), $site_url);
					$site_url = str_replace("(today)", "today", $site_url);
				}
				else {
					$site_name = $data["site_name"];
					$site_url = str_replace("(md)", date("md", strtotime($date)), $site_url);
					$site_url = str_replace("(ymd)", date("ymd", strtotime($date)), $site_url);
					$site_url = str_replace("(Y)", date("Y", strtotime($date)), $site_url);
					$site_url = str_replace("(M)", intval(date("m", strtotime($date))), $site_url);
					$site_url = str_replace("(m)", date("m", strtotime($date)), $site_url);
					$site_url = str_replace("(d)", intval(date("d", strtotime($date))), $site_url);
					$site_url = str_replace("(dd)", date("d", strtotime($date)), $site_url);
					if ($date < date("Ymd")) {
						$site_url = str_replace("(today)", "yesterday", $site_url);
					} else {
						$site_url = str_replace("(today)", "today", $site_url);
					}
				}
				// <<<
				if($data_type == ""){
					$rank = $data['rank'];
				}else{
					for($i = 0;$i <= 100; $i += 10){
						if($i <= $data['score'] && $data['score'] < ($i +10)){
							$rank = $i;
						}
					}
				}
	
				$rankings = array(
					"num" => $rank	// num is actually "rank" badly named
					,"site" => $site_name
					,"name" => $name["star$star"]
					,"en_name" => $en_star["{$name["star$star"]}"]
					,"star_num" => $star
					,"url" => $url
					,"site_url" => $site_url
				);
				$logs[$rank][] = $rankings;
				// <<<
			}
		}
		//print_r($logs);

		return $logs;
	}

	/**
	 * 最も早い時間に稼働する取得プラグインの時間を調べる
	 * 
	 * @return string $time ( 00:00:00 ...)
	 */
	function checkEarliestPluginTime(){
		global $conn;
		$time = "";

		$sql = "SELECT ".
					 "MIN(`site_get_time`) AS min_time ".
					 "FROM `site` ".
					 "WHERE `is_delete` = '0' ".
					 "AND is_execute = '1' ".
					 ";";

		$result = mysqli_query($conn, $sql);

		if($result) {
			while ($data = mysqli_fetch_assoc($result)) {
				$time = $data["min_time"];
			}
		}
		return $time;
	}

	/**
	 * logテーブルに当日のレコードが1件でも存在するか調べる
	 *
	 * @param string $data_type 運勢タイプ
	 * @return bool $is_log レコードが存在するか。1件でもあればtrue、0件ならfalse。
	 */
	function isTodaysLog( $data_type = null ){
		global $conn;
		$is_log = false;
		$where_type = "";

		$sql = "SELECT * ";
		if($data_type == ""){
			$sql .= "FROM `log` ";
		}else{
			$sql .= "FROM `topic_log` ";
			$where_type = "AND `data_type` = '{$data_type}' ";
		}
		$sql .= "WHERE `day` = DATE( NOW() ) ".
						"AND `is_delete` = '0' ".
						$where_type.
						";";

		$result = mysqli_query($conn, $sql);

		if($result) {
			$row = $result->num_rows;
			if($row > 0){
				$is_log = true;
			}
		}
		return $is_log;
	}

	/**
	 * 有料アプリ
	 * 順位別サイトURL一覧取得
	 * 更新時間が過ぎていたら※印をつける
	 *
	 * @author Azet
	 * @param string $data_type
	 * @param int $star_
	 * @param int $rank_
	 * @return array
	 */
	 
	function getAppDetailsForStarRankAjax($star_,$data_type=null, $rank_=null) {
		global $conn, $name, $en_star;
		$logs = array();

		$now = date("H:i:s"); // 現在時刻

		$sql = "SELECT";
		$sql .= " log.day, site.site_name, site.past_days, site.future_days , site.future_flag ,";
		$sql .= " site.limit_time , log.star,";
		if($data_type == ""){
			$sql .= " log.rank";
		}else{
			$sql .= " log.score";
		}
		$sql .= ",";
		$sql .= " url,".
						" sp_url,".
						" site.link_url,".
						" site.sp_link_url,".
						" site.star".$star_."_url,".
						" site.sp_star".$star_."_url,";
		
		if($data_type == ""){
			$sql .= " lu.all_link_url,".
							" lu.all_sp_link_url,".
							" lu.star".$star_."_link_url,".
							" lu.sp_star".$star_."_link_url";
		}else{
			$sql .= " lvu.link_love_url,".
							" lvu.sp_link_love_url,".
							" lvu.star".$star_."_link_love_url,".
							" lvu.sp_star".$star_."_link_love_url";
		}
		$sql .= " FROM `site` site";
		
		if($data_type == ""){
			$sql .= " INNER JOIN `log` log";
			$sql .= " LEFT JOIN `link_url` lu ON lu.site_id = site.site_id";
		}else{
			$sql .= " INNER JOIN `topic_log` log";
			$sql .= " LEFT JOIN `link_love_url` lvu ON lvu.site_id = site.site_id";
		}
		$sql .= " WHERE log.`is_delete`=0 AND log.`day`='{$this->date}' AND site.`is_delete`=0";
		$sql .= " AND site.site_id=log.site_id";
		$sql .= " AND log.`star`='{$star_}'";

		if($data_type != "") {
			$sql .= " AND log.`data_type` = '{$data_type}'";
		}
		if($rank_) {
			$sql .= " AND log.`rank`={$rank_}";
		}

		if($data_type == ""){
			$sql .= " ORDER BY log.rank";
		}else{
			$sql .= " ORDER BY log.score DESC";
		}

		$result = mysqli_query($conn, $sql);
		//echo htmlspecialchars($sql);
		//pre($conn->error);

		if($result) {
			while ($data = mysqli_fetch_assoc($result)) {
				$site_url = "";
				$site_name = $data["site_name"];
				$date = str_replace('-', '', $this->date);
				// URL処理 >>>
				//---総合運---
				if($data_type == ""){
					if($data["sp_star".$star_."_link_url"] != ""){
						$site_url = $data["sp_star".$star_."_link_url"]; // モバイル 星座別 総合運
					}else if($data["all_sp_link_url"] != ""){
						$site_url = $data["all_sp_link_url"]; // モバイル 一覧 総合運
					}else if($data["star".$star_."_link_url"] != ""){
						$site_url = $data["star".$star_."_link_url"]; // PC 星座別 総合運
					}else if($data["all_link_url"] != ""){
						$site_url = $data["all_link_url"]; // PC 一覧 総合運
					}
				//---{トピック}運---
				}else{
					if($data["sp_star".$star_."_link_".$data_type."_url"] != ""){ 
						$site_url = $data["sp_star".$star_."_link_".$data_type."_url"]; // モバイル 星座別 別運勢
					}else if($data["sp_link_".$data_type."_url"] != ""){
						$site_url = $data["sp_link_".$data_type."_url"]; // モバイル 一覧 別運勢
					}else if($data["star".$star_."_link_".$data_type."_url"] != ""){ 
						$site_url = $data["star".$star_."_link_".$data_type."_url"]; // PC 星座別 別運勢
					}else if($data["link_".$data_type."_url"] != ""){
						$site_url = $data["link_".$data_type."_url"]; // PC 一覧 別運勢
					}
				}
				//各運勢用URLがなければプラグイン用に使用しているURLを表示する
				if($site_url == ""){
					//---プラグインの取得用URL---
					if($data["sp_star".$star_."_url"] != ""){
						$site_url = $data["sp_star".$star_."_url"]; // モバイル 星座別 取得用
					}else if($data["sp_url"] != ""){
						$site_url = $data["sp_url"]; // モバイル 一覧 取得用
					}else if($data["sp_link_url"] != ""){
						$site_url = $data["sp_link_url"]; // モバイル リンクURL
					}else if($data["star".$star_."_url"] != ""){
						$site_url = $data["star".$star_."_url"]; // PC 星座別 取得用
					}else if($data["url"]){
						$site_url = $data["url"]; // PC 一覧 取得用
					}else if($data["link_url"] != ""){
						$site_url = $data["link_url"]; // PC リンクURL
					}else{
						$site_url = $data["etc_url"]; //その他URL
					}
				}

				//情報更新時刻を過ぎていたら※印
				if($date == date("Ymd")){
					if($data["limit_time"]){
						$limit_time = date("H:i:s",strtotime($data["limit_time"]));
						if($now >= $limit_time ){
							$site_name = "※".$site_name;
						}
					}
				}else if($data["past_days"] == "0D"){
					$site_name = "※".$site_name;
				}
				//過去のデータがあるが、データが存在する最古の日より前の日付を閲覧中は最新情報を表示
				if($data["past_days"] != "0D"){
					$datetime_obj = new DateTime();
					$datetime_obj->sub(new DateInterval("P".$data["past_days"]));
					$date_exist_start = $datetime_obj->format('Ymd');
					if($date < $date_exist_start){
						$date = date("Ymd"); //今日の日付
						$site_name = "※".$site_name; //※つける
					}
				}

				$site_url = str_replace("(md)", date("md", strtotime($date)), $site_url);
				$site_url = str_replace("(ymd)", date("ymd", strtotime($date)), $site_url);
				$site_url = str_replace("(Y)", date("Y", strtotime($date)), $site_url);
				$site_url = str_replace("(M)", intval(date("m", strtotime($date))), $site_url);
				$site_url = str_replace("(m)", date("m", strtotime($date)), $site_url);
				$site_url = str_replace("(d)", intval(date("d", strtotime($date))), $site_url);
				$site_url = str_replace("(dd)", date("d", strtotime($date)), $site_url);
				$site_url = str_replace("(yesterday)",date("d",strtotime("-1 day",strtotime($date))),$site_url);

				if($data_type == ""){
					$rank = $data['rank'];
				}else{
					for($i = 0;$i <= 100; $i += 10){
						if($i <= $data['score'] && $data['score'] < ($i +10)){
							$rank = $i;
						}
					}
				}
	
				$rankings = array(
					"num" => $rank	// num is actually "rank" badly named
					,"site" => $site_name
					,"site_url" => $site_url
				);

				$logs[$rank][] = $rankings;
			}
		}
		//pre($logs);
		return $logs;
	}

	/**
	 * 有料アプリ
	 * 順位別サイトURL一覧取得
	 * 最新情報へのリンク以外は表示しない
	 *
	 * @author Azet
	 * @param string $data_type
	 * @param int $star_
	 * @param int $rank_
	 * @return array
	 */
	 
	function getAppDetailsForStarRank($star_,$data_type=null, $rank_=null) {
		global $conn, $name, $en_star;
		$logs = array();

		$date = str_replace('-', '', $this->date);
		$now = date("H:i:s"); // 現在時刻

		$sql = "SELECT";
		$sql .= " log.day, site.site_name, site.past_days, site.future_days , site.future_flag ,";
		$sql .= " site.limit_time , log.star,";
		if($data_type == ""){
			$sql .= " log.rank";
		}else{
			$sql .= " log.score";
		}
		$sql .= ",";
		$sql .= " url,".
						" sp_url,".
						" site.link_url,".
						" site.sp_link_url,".
						" site.star".$star_."_url,".
						" site.sp_star".$star_."_url,";
		
		if($data_type == ""){
			$sql .= " lu.all_link_url,".
							" lu.all_sp_link_url,".
							" lu.star".$star_."_link_url,".
							" lu.sp_star".$star_."_link_url";
		}else{
			$sql .= " lvu.link_love_url,".
							" lvu.sp_link_love_url,".
							" lvu.star".$star_."_link_love_url,".
							" lvu.sp_star".$star_."_link_love_url";
		}
		$sql .= " FROM `site` site";
		
		if($data_type == ""){
			$sql .= " INNER JOIN `log` log";
			$sql .= " LEFT JOIN `link_url` lu ON lu.site_id = site.site_id";
		}else{
			$sql .= " INNER JOIN `topic_log` log";
			$sql .= " LEFT JOIN `link_love_url` lvu ON lvu.site_id = site.site_id";
		}
		$sql .= " WHERE log.`is_delete`=0 AND log.`day`='{$this->date}' AND site.`is_delete`=0";
		$sql .= " AND site.site_id=log.site_id";
		$sql .= " AND log.`star`='{$star_}'";

		if($data_type != "") {
			$sql .= " AND log.`data_type` = '{$data_type}'";
		}
		if($rank_) {
			$sql .= " AND log.`rank`={$rank_}";
		}

		if($data_type == ""){
			$sql .= " ORDER BY log.rank";
		}else{
			$sql .= " ORDER BY log.score DESC";
		}

		$result = mysqli_query($conn, $sql);
		//echo htmlspecialchars($sql);
		//pre($conn->error);

		if($result) {
			while ($data = mysqli_fetch_assoc($result)) {
				$site_url = "";
				$site_name = $data["site_name"];
				// URL処理 >>>
				//---総合運---
				if($data_type == ""){
					if($data["sp_star".$star_."_link_url"] != ""){
						$site_url = $data["sp_star".$star_."_link_url"]; // モバイル 星座別 総合運
					}else if($data["all_sp_link_url"] != ""){
						$site_url = $data["all_sp_link_url"]; // モバイル 一覧 総合運
					}else if($data["star".$star_."_link_url"] != ""){
						$site_url = $data["star".$star_."_link_url"]; // PC 星座別 総合運
					}else if($data["all_link_url"] != ""){
						$site_url = $data["all_link_url"]; // PC 一覧 総合運
					}
				//---{トピック}運---
				}else{
					if($data["sp_star".$star_."_link_".$data_type."_url"] != ""){ 
						$site_url = $data["sp_star".$star_."_link_".$data_type."_url"]; // モバイル 星座別 別運勢
					}else if($data["sp_link_".$data_type."_url"] != ""){
						$site_url = $data["sp_link_".$data_type."_url"]; // モバイル 一覧 別運勢
					}else if($data["star".$star_."_link_".$data_type."_url"] != ""){ 
						$site_url = $data["star".$star_."_link_".$data_type."_url"]; // PC 星座別 別運勢
					}else if($data["link_".$data_type."_url"] != ""){
						$site_url = $data["link_".$data_type."_url"]; // PC 一覧 別運勢
					}
				}
				//各運勢用URLがなければプラグイン用に使用しているURLを表示する
				if($site_url == ""){
					//---プラグインの取得用URL---
					if($data["sp_star".$star_."_url"] != ""){
						$site_url = $data["sp_star".$star_."_url"]; // モバイル 星座別 取得用
					}else if($data["sp_url"] != ""){
						$site_url = $data["sp_url"]; // モバイル 一覧 取得用
					}else if($data["sp_link_url"] != ""){
						$site_url = $data["sp_link_url"]; // モバイル リンクURL
					}else if($data["star".$star_."_url"] != ""){
						$site_url = $data["star".$star_."_url"]; // PC 星座別 取得用
					}else if($data["url"]){
						$site_url = $data["url"]; // PC 一覧 取得用
					}else if($data["link_url"] != ""){
						$site_url = $data["link_url"]; // PC リンクURL
					}else{
						$site_url = $data["etc_url"]; //その他URL
					}
				}
				
				//情報更新時刻
				if($data["limit_time"]){
					$limit_time = date("H:i:s",strtotime($data["limit_time"]));
				}
				$future_flag = $data["future_flag"];

				//過去のデータがないサイト
				if($data["past_days"] == "0D"){
					if($future_flag == "1" && $now >= $limit_time ){ continue; } //当日中更新で、更新時刻を過ぎたら表示しない
					if($date < date("Ymd")){ continue; } //今日より過去の日付を閲覧中は表示しない
				}
				//過去のデータがあるが、データが存在する最古の日より前の日付を閲覧中は表示しない
				if($data["past_days"] != "0D"){
					$datetime_obj = new DateTime();
					$datetime_obj->sub(new DateInterval("P".$data["past_days"]));
					$date_exist_start = $datetime_obj->format('Ymd');
					if($date < $date_exist_start){
						continue;
					}
				}
				//過去のデータがあるサイトは日付パラメータによってURLを変更し表示
				$site_url = str_replace("(md)", date("md", strtotime($date)), $site_url);
				$site_url = str_replace("(ymd)", date("ymd", strtotime($date)), $site_url);
				$site_url = str_replace("(Y)", date("Y", strtotime($date)), $site_url);
				$site_url = str_replace("(M)", intval(date("m", strtotime($date))), $site_url);
				$site_url = str_replace("(m)", date("m", strtotime($date)), $site_url);
				$site_url = str_replace("(d)", intval(date("d", strtotime($date))), $site_url);
				$site_url = str_replace("(dd)", date("d", strtotime($date)), $site_url);
				$site_url = str_replace("(yesterday)",date("d",strtotime("-1 day",strtotime($date))),$site_url);
				
				if($data_type == ""){
					$rank = $data['rank'];
				}else{
					for($i = 0;$i <= 100; $i += 10){
						if($i <= $data['score'] && $data['score'] < ($i +10)){
							$rank = $i;
						}
					}
				}
	
				$rankings = array(
					"num" => $rank	// num is actually "rank" badly named
					,"site" => $site_name
					,"site_url" => $site_url
				);

				$logs[$rank][] = $rankings;
			}
		}
		//pre($logs);
		return $logs;
	}
	/**
	 * user agent 判断
	 *
	 * @author Azet
	 * @return bool
	 */
	function isMobile() {
		/*>>>*/
		$is_sphone = false;
		$ua = $_SERVER['HTTP_USER_AGENT'];
		if (
			(strpos($ua, 'iPhone') !== false)
		 || (strpos($ua, 'iPod') !== false)
		 || (strpos($ua, 'Android') !== false)
		 || (strpos($ua, 'iPad') !== false)
		 || (strpos($ua, 'Windows Phone') !== false)
		 || (strpos($ua, 'Blackberry') !== false)
		 || (strpos($ua, 'Symbian') !== false)
		) {
			$is_sphone = true;
		}

		return $is_sphone;
		/*<<<*/
	}

	//add hirose start 2017/03/13
	function first_rank($day,$data_type){
		global $conn;

		$first_count = array();
		$first_str_con = array();

		if($data_type ==""){
		$sql = "SELECT l.star, count(l.site_id) as first_count FROM `log` l WHERE l.is_delete=0 AND l.`day`='{$day}'AND l.rank = 1 GROUP BY l.star";
		}else{
		$sql = "SELECT t.star, count(t.site_id) as first_count FROM `topic_log` t WHERE t.is_delete=0 AND t.`day`='{$day}'AND t.score = 100 AND t.`data_type`='".$data_type."' GROUP BY t.star";
		}
		// TEST
		//print $sql;

		$result = mysqli_query($conn,$sql);
		if($result){
			 while($first_rank_count = mysqli_fetch_assoc($result)){
				
				$first_count[] = $first_rank_count;
			}
		}
		foreach($first_count as $index => $list){
				$str_con = array($list['star'] => $list['first_count']);
				$first_str_con = $first_str_con + $str_con;
		}
		return $first_str_con;
	}
	//add hirose end 2017/03/13

	/**
	 * daily情報からランキングの計算
	 *
	 * @author Azet
	 * @param mysqli_result_set $rs (12件、星座毎のつまり)
	 * @return array (12件、星座毎のランキングが追加されたデータ)
	 */
	static private function calculateRank($rs){
		$ranking = array();
		$rank = 0;
		$last_score = 0;
		$count = 0;
		while($data = $rs->fetch_assoc()){
			++$count;
			$score = $data['score'];

			if ($score > $last_score) {
				$rank = $count;
			}

			$data['rank'] = $rank;
			$ranking[] = $data;

			$last_score = $score;
		}

		// TEST
		//print_r($ranking);

		return $ranking;
	}
	/**
	 * Topic_daily情報からランキングの計算 add yamaguchi 2017/06/08
	 *
	 * @author Azet
	 * @param mysqli_result_set $rs (12件、星座毎のつまり)
	 * @return array (12件、星座毎のランキングが追加されたデータ)
	 */
	static private function calculateTopicRank($rs){
		$ranking = array();
		$rank = 1;
		$last_score = 0;
		$count = 0;
		while($data = $rs->fetch_assoc()){
			++$count;
			$score = $data['score'];

			if ($score < $last_score) {
				$rank = $count;
			}

			$data['rank'] = $rank;
			$ranking[] = $data;

			$last_score = $score;
		}

		// TEST
		//print_r($ranking);

		return $ranking;
	}
		
	/**
	* logテーブルから日間ランキングを集計する
	*	@param str $date_ ("2016-01-01" ...)
	* @return array
	*	Array ( 
	*	[0] => Array ( [score] => 110 [star_num] => 4 [num] => 1 [en_name] => taurus [name] => おうし座 ) 
	*	[1] => ...
	*	)
	**/
	static public function getDailyRanksFromLog($date_){
		global $conn;
		$ranks = array();
		$sql = "
				SELECT
					star, sum(rank) AS score
				FROM
					`log` l
				JOIN `site` s
					ON s.is_delete=0 AND s.parent_id=0 AND s.site_id=l.site_id
				WHERE l.is_delete=0
					AND l.`day`='{$date_}'
					GROUP BY l.star
					ORDER BY SUM(rank) ASC, l.star ASC
			";
		$rs = $conn->query($sql);
		if($rs){
			$ranks = self::calculateRank($rs);
		}
		$ranks = self::FormatRankArray($ranks);
		return $ranks;
	}

	/**
	* topic_logテーブルから日間ランキングを集計する	add yamaguchi 2017/06/08
	*	@param str $date_ ("2016-01-01" ...)
	* @return array
	*	Array ( 
	*	[0] => Array ( [score] => 110 [star_num] => 4 [num] => 1 [en_name] => taurus [name] => おうし座 ) 
	*	[1] => ...
	*	)
	**/
	static public function getTopicDailyRanksFromLog($date_,$data_type){
		global $conn;
		$ranks = array();
		$sql = "
				SELECT
					star,data_type, sum(score) AS score
				FROM
					`topic_log` tl
				JOIN `site` s
					ON s.is_delete=0 AND s.parent_id=0 AND s.site_id=tl.site_id
				WHERE tl.is_delete=0
					AND tl.`data_type`='{$data_type}'
					AND tl.`day`='{$date_}'
					GROUP BY tl.star
					ORDER BY sum(score) DESC, tl.star DESC
			";
		$rs = $conn->query($sql);
		if($rs){
			$ranks = self::calculateTopicRank($rs);
		}
		$ranks = self::FormatTopicRankArray($ranks);
		return $ranks;
	}

	/**
	*	期間ごとのランキングを取得する
	*	@param str $date_ ("2016-01-01" ...)
	*	@param str $interval_ ('weekly','monthly','yearly')
	* @return array
	*
	**/
	static public function getEachRanks($date_,$interval_){
		global $conn;
		$date = explode('-',$date_);
		$year = $date[0]; 
		$month = $date[1];
		$day = $date[2];
		$ranks = array();
		$sql = "SELECT SUM(star1_score) AS `1`,SUM(star2_score) AS `2`,
						SUM(star3_score) AS `3`,SUM(star4_score) AS `4`,
						SUM(star5_score) AS `5`,SUM(star6_score) AS `6`,
						SUM(star7_score) AS `7`,SUM(star8_score) AS `8`,
						SUM(star9_score) AS `9`,SUM(star10_score) AS `10`,
						SUM(star11_score) AS `11`,SUM(star12_score) AS `12`
						FROM star_ranks_daily WHERE is_delete = 0 ";
		switch($interval_){
			case 'daily' :
				$sql .= 'AND day = "' . $date_ . '"';
				break;
			case 'weekly' :
				$sql .= 'AND day >= "' . $date_ . '" - INTERVAL 7 DAY AND day < "' . $date_ . '"';
				break;
			case 'monthly' :
				$sql .= 'AND MONTH(day) = "' . $month . '" AND YEAR(day) = "' . $year. '"';
				break;
			case 'yearly' :
				$sql .= 'AND YEAR(day) ="' . $year . '"';
		}
		//echo $sql . PHP_EOL;
		$rs = mysqli_query($conn, $sql);
		if($rs){
			$data = mysqli_fetch_assoc($rs);
			if($data['1'] == 0){//1位のスコアが0だったらnullを返す
				return NULL;
			}
			asort($data,SORT_NUMERIC);//点数でソート　キー(星座ナンバー)=>得点 は維持
			$rank = 0;
			$last_score = 0;
			$count = 0;
			foreach($data as $star => $val){
				$count++;
				$score = $val;
				if ($score > $last_score) {
					$rank = $count;
				}
					$ranks[] = array(
						'star' => $star,
						'score'=> $val,
						'rank' => $rank
					);
				$last_score = $score;
			}
			$ranks = self::FormatRankArray($ranks);
			return $ranks;
		}
	}
	
	/**
	*	期間ごとのTopicランキングを取得する
	*	@param str $date_ ("2016-01-01" ...)
	*	@param str $interval_ ('weekly','monthly','yearly')
	* @return array
	*
	**/
	static public function getTopicEachRanks($date_,$interval_,$data_type){
		global $conn;
		$date = explode('-',$date_);
		$year = $date[0]; 
		$month = $date[1];
		$day = $date[2];
		$ranks = array();
		//print "***";
		$sql = "SELECT SUM(star1_score) AS `1`,SUM(star2_score) AS `2`,
						SUM(star3_score) AS `3`,SUM(star4_score) AS `4`,
						SUM(star5_score) AS `5`,SUM(star6_score) AS `6`,
						SUM(star7_score) AS `7`,SUM(star8_score) AS `8`,
						SUM(star9_score) AS `9`,SUM(star10_score) AS `10`,
						SUM(star11_score) AS `11`,SUM(star12_score) AS `12`
						FROM topic_ranks_daily WHERE is_delete = 0 
						AND `data_type` = '" . $data_type . "' ";
		switch($interval_){
			case 'daily' :
				$sql .= 'AND day = "' . $date_ . '"';
				break;
			case 'weekly' :
				$sql .= 'AND day >= "' . $date_ . '" - INTERVAL 7 DAY AND day < "' . $date_ . '"';
				break;
			case 'monthly' :
				$sql .= 'AND MONTH(day) = "' . $month . '" AND YEAR(day) = "' . $year. '"';
				break;
			case 'yearly' :
				$sql .= 'AND YEAR(day) ="' . $year . '"';
		}
		//echo $sql . PHP_EOL;
		$rs = mysqli_query($conn, $sql);
		if($rs){
			$data = mysqli_fetch_assoc($rs);
			if($data['1'] == 0){//1位のスコアが0だったらnullを返す
				return NULL;
			}
			arsort($data,SORT_NUMERIC);//点数でソート　キー(星座ナンバー)=>得点 は維持
			//print_r($data);
			$rank = 1;
			$last_score = 0;
			$count = 0;
			foreach($data as $star => $val){
				++$count;
				$score = $val;
				if ($score < $last_score) {
					$rank = $count;
				}
					$ranks[] = array(
						'star' => $star,
						'score'=> $val,
						'rank' => $rank
					);
				$last_score = $score;
			}
			$ranks = self::FormatTopicRankArray($ranks);
			return $ranks;
		}
	}
	
	/*
	* 星座、スコア、順位のキーを持っている配列を表示用の形に加工する
	* @params array $ranking_array
	* Array(
	* [0] => Array([star] => 4[score] => 110[rank] => 1)
	* [1] => ...
	* )
	*	@ return array
	* Array(
	* [0] => Array([score] => 110 [star_num] => 4 [num] => 1 [en_name] => taurus [star_name] => おうし座)
	* [1] => ...
	* )
	*/
	static public function FormatRankArray($ranking_array){
		global $en_num_star , $jpn_num_star;
		for($i=0; $i<12; $i++){
			$star = $ranking_array[$i]['star'];
			$ranking_array[$i]['star_num'] = $ranking_array[$i]['star'];
			unset($ranking_array[$i]['star']);
			$ranking_array[$i]['num'] = $ranking_array[$i]['rank'];
			unset($ranking_array[$i]['rank']);
			$ranking_array[$i]['en_name'] = $en_num_star[$star];
			$ranking_array[$i]['name'] = $jpn_num_star[$star];
		}
		return $ranking_array;
	}

	/*
	* 運勢別の星座、スコア、順位のキーを持っている配列を表示用の形に加工する	add yamaguchi 2017/06/08
	* @params array $ranking_array
	* Array(
	* [0] => Array([star] => 4[score] => 110[rank] => 1)
	* [1] => ...
	* )
	*	@ return array
	* Array(
	* [0] => Array([score] => 110 [star_num] => 4 [num] => 1 [en_name] => taurus [star_name] => おうし座)
	* [1] => ...
	* )
	*/
	static public function FormatTopicRankArray($ranking_array){
		global $en_num_star , $jpn_num_star;
		for($i=0; $i<12; $i++){
			$star = $ranking_array[$i]['star'];
			$ranking_array[$i]['star_num'] = $ranking_array[$i]['star'];
			unset($ranking_array[$i]['star']);
			$ranking_array[$i]['num'] = $ranking_array[$i]['rank'];
			unset($ranking_array[$i]['rank']);
			$ranking_array[$i]['en_name'] = $en_num_star[$star];
			$ranking_array[$i]['name'] = $jpn_num_star[$star];
		}
		return $ranking_array;
	}

	/**
	* 一行のレコードにデータを作成する
	* @param str $date ('2016-01-01'...)
	* @param str $interval ('daily','weekly','monthly','yearly')
	* @return mysqli_resultオブジェクト
	*
	**/
	static public function compileLogs($date_,$interval_) {
		global $conn;
		if($interval_ == 'daily'){
			$ranking = self::getDailyRanksFromLog($date_);
		}else{
			$ranking = self::getEachRanks($date_,$interval_);
		}
		$dateUnix = strtotime($date_);
		switch($interval_){
			case 'daily' :
				$siteCount = self::countLogsSites("'" . $date_ . "'"); //その日のサイト数
				$dayColumn = $date_;
				break;
			case 'weekly' :
				$scDate = date('Y-m-d',strtotime('-1 day',$dateUnix));//週末時点のサイト数
				$siteCount = self::countLogsSites("'" . $scDate . "'");
				$dayColumn = date('Y-m-d',strtotime('-7 day',$dateUnix));//dayカラムの値には先週の月曜日の値が入ります。
				break;
			case 'monthly' :
				$scDate = date('Y-m-d',strtotime('last day of last month'));//先月の月末時点のサイト数
				$siteCount = self::countLogsSites("'" . $scDate . "'");
				$dayColumn = $date_;
				break;
			case 'yearly' :
				$scDate = strtotime('+11 month',$dateUnix);
				$scDate = date('Y-m-d',strtotime('last day of',$scDate));
				$siteCount = self::countLogsSites("'" . $scDate . "'"); //12月末時点のサイト数
				$dayColumn = $date_;
		}
		//テーブルにすでに作成されたレコードがあるかチェックする
		$result = mysqli_query($conn,'SELECT * FROM star_ranks_' . $interval_ .' WHERE day ="' . $dayColumn . '" AND is_delete=0');
		//クエリの結果行数
		$rows = mysqli_num_rows($result);
		//レコードが未作成の場合 INSERTする
		if(!$rows){
			$sql = 'INSERT INTO star_ranks_' . $interval_ . ' (day,site_count,';
			for($i=0; $i<12; $i++){
				$sql .= 'star' . $ranking[$i]['star_num'] . '_rank,';
				$sql .= 'star' . $ranking[$i]['star_num'] . '_score,';
			}
			$sql .= 'date_create)';
			$sql .= 'VALUES ("' . $dayColumn . '","' . $siteCount . '",';
			for($i=0; $i<12; $i++){
				$sql .= '"' . $ranking[$i]['num'] . '",';
				$sql .= '"' . $ranking[$i]['score'] . '",';
			}
			$sql .= 'CURRENT_TIMESTAMP)';	
			$msg = 'データがインサートされました。';
		}else{//レコードがすでにある場合 UPDATEする
			$sql = 'UPDATE star_ranks_' . $interval_ . ' SET site_count ="' . $siteCount . '",';
			for($i=0; $i<12; $i++){
				$sql .= 'star' . $ranking[$i]['star_num'] . '_rank ="' . $ranking[$i]['num'] . '",';
				$sql .= 'star' . $ranking[$i]['star_num'] . '_score ="' . $ranking[$i]['score'] . '",';
			}
			$sql .= 'date_update = CURRENT_TIMESTAMP WHERE day = "' . $dayColumn . '"';
			$msg = 'データがアップデートされました。';
		}
		//echo $sql . PHP_EOL;
		$ok = mysqli_query($conn,$sql);
		$log_title = 'star_ranks_' . $interval_ . 'テーブルの' .$msg . '(dayカラム：' . $dayColumn .')';
		if($ok){
			self::$log->add($log_title, 'SAVE OK');
		}else{
			$log_title = 'star_ranks_' . $interval_ . 'テーブルの更新クエリに失敗しました';
			self::$log->add($log_title, 'SAVE ERR');
		}
		return $ok;
	}

	/**
	* 一行のレコードにtopicデータを作成する	add yamaguchi 2017/06/08
	* @param str $date ('2016-01-01'...)
	* @param str $interval ('daily','weekly','monthly','yearly')
	* @return mysqli_resultオブジェクト
	*
	**/
	static public function compileTopicLogs($date_,$interval_) {
		global $conn ;
		$data=array();
		$DATA_TYPE=array();
		$sql=" SELECT DISTINCT data_type FROM `topic_log` tl
			JOIN `site` s ON s.is_delete=0 AND s.site_id=tl.site_id
			WHERE tl.is_delete=0 AND s.parent_id=0 AND `day`='{$date_}';";
		$i=0;
		$rs = $conn->query($sql);
		
		while($data = mysqli_fetch_assoc($rs)){
			$DATA_TYPE[$i] = $data['data_type'];
			$i++;
		}
		print_r($DATA_TYPE);
		foreach($DATA_TYPE as $data_type){
			if($interval_ == 'daily'){
				$ranking = self::getTopicDailyRanksFromLog($date_,$data_type);
				//print_r($ranking);
			}else{
				$ranking = self::getTopicEachRanks($date_,$interval_,$data_type);
				//print_r($ranking);
			}
			$dateUnix = strtotime($date_);
			switch($interval_){
				case 'daily' :
					$siteCount = self::countTopicLogsSites("'" . $date_ . "'",$data_type); //その日のサイト数
					$dayColumn = $date_;
					break;
				case 'weekly' :
					$scDate = date('Y-m-d',strtotime('-1 day',$dateUnix));//週末時点のサイト数
					$siteCount = self::countTopicLogsSites("'" . $scDate . "'",$data_type);
					$dayColumn = date('Y-m-d',strtotime('-7 day',$dateUnix));//dayカラムの値には先週の月曜日の値が入ります。
					break;
				case 'monthly' :
					$scDate = date('Y-m-d',strtotime('last day of last month'));//先月の月末時点のサイト数
					$siteCount = self::countTopicLogsSites("'" . $scDate . "'",$data_type);
					$dayColumn = $date_;
					break;
				case 'yearly' :
					$scDate = strtotime('+11 month',$dateUnix);
					$scDate = date('Y-m-d',strtotime('last day of',$scDate));
					$siteCount = self::countTopicLogsSites("'" . $scDate . "'",$data_type); //12月末時点のサイト数
					$dayColumn = $date_;
			}
			//テーブルにすでに作成されたレコードがあるかチェックする
			$result = mysqli_query($conn,'SELECT * FROM topic_ranks_' . $interval_ .' WHERE day ="' . $dayColumn . '" AND is_delete=0 AND data_type="'.$data_type.'"');
			//クエリの結果行数
			$rows = mysqli_num_rows($result);
			//レコードが未作成の場合 INSERTする
			if(!$rows){
				$sql = 'INSERT INTO topic_ranks_' . $interval_ . ' (day,data_type,site_count,';
				for($i=0; $i<12; $i++){
					$sql .= 'star' . $ranking[$i]['star_num'] . '_rank,';
					$sql .= 'star' . $ranking[$i]['star_num'] . '_score,';
				}
				$sql .= 'date_create)';
				$sql .= 'VALUES ("' . $dayColumn . '","'.$data_type. '","' . $siteCount . '",';
				for($i=0; $i<12; $i++){
					$sql .= '"' . $ranking[$i]['num'] . '",';
					$sql .= '"' . $ranking[$i]['score'] . '",';
				}
				$sql .= 'CURRENT_TIMESTAMP)';	
				$msg = 'データがインサートされました。';
			}else{//レコードがすでにある場合 UPDATEする
				$sql = 'UPDATE topic_ranks_' . $interval_ . ' SET site_count ="' . $siteCount . '",';
				for($i=0; $i<12; $i++){
					$sql .= 'star' . $ranking[$i]['star_num'] . '_rank ="' . $ranking[$i]['num'] . '",';
					$sql .= 'star' . $ranking[$i]['star_num'] . '_score ="' . $ranking[$i]['score'] . '",';
				}
				$sql .= 'date_update = CURRENT_TIMESTAMP WHERE day = "' . $dayColumn . '" AND data_type="' .$data_type .'"';
				$msg = 'データがアップデートされました。';
			}
			echo $sql . PHP_EOL;
			$ok = mysqli_query($conn,$sql);
			$log_title = 'topic_ranks_' . $interval_ . 'テーブルの'.$data_type .$msg . '(dayカラム：' . $dayColumn .')';
			if($ok){
				self::$log->add($log_title, 'SAVE OK');
			}else{
				$log_title = 'topic_ranks_' . $interval_ . 'テーブルの更新クエリに失敗しました';
				if ($siteCount == 0) {
					$log_title .= '(data_type:' . $data_type . 'の件数がありませんでした。)';
				}
				self::$log->add($log_title, 'SAVE ERR');
			}
		}
		return $ok;
	}
	
	/*
	* 年間ランキングの配列を取得する（star_ranks_yearlyテーブルから）
	* @param int $year_ $data_type ex)love
	* @return ランキングの配列（順位昇順）
	* Array ( [0] => Array ( [score] => 116176 [star_num] => 8 [num] => 1 [en_name] => virgo [name] => おとめ座 ) ...
	*/
	static function getYearly($year_,$data_type_ = NULL){
		global $conn;
		$ranks = array();
		if($data_type_ == ""){
			$sql = 'SELECT * FROM star_ranks_yearly WHERE YEAR(day) =' . $year_;
		}else{
			$sql = 'SELECT * FROM topic_ranks_monthly WHERE YEAR(day) =' . $year_ . ' AND data_type = "'.$data_type_.'"';
		}
		$rs = mysqli_query($conn,$sql);
		if($rs){
			$data = mysqli_fetch_assoc($rs);
			for($i = 1; $i < 13; $i++){//$iは星座番号
				$ranks[] = array(
					'star' => $i,
					'score' => $data['star' . $i . '_score'],
					'rank' => $data['star' . $i . '_rank']
				);
			}
			foreach($ranks as $key=>$value){
				$tmp_ranks[$key] = $value['rank']; 	 
			}
			array_multisort($tmp_ranks,SORT_ASC,$ranks);
			$ranks = self::FormatRankArray($ranks);
			return $ranks;
		}else{
			self::$log->add('getYearlyの実行でエラーが発生しました', 'SAVE ERR');
		}
	}
	/*
	* 月間ランキングの配列を取得する（star_ranks_monthlyテーブルから）
	* @param int $year_, int $month_
	* @return ランキングの配列（順位昇順）
	* Array ( [0] => Array ( [score] => 116176 [star_num] => 8 [num] => 1 [en_name] => virgo [name] => おとめ座 ) ...
	*/
	static function getMonthly($year_,$month_,$data_type_ = NULL){
		global $conn;
		$ranks = array();
		if($data_type_ == ""){
			$sql = 'SELECT * FROM star_ranks_monthly WHERE YEAR(day) =' . $year_ . ' AND MONTH(day) =' . $month_;
		}else{
			$sql = 'SELECT * FROM topic_ranks_monthly WHERE YEAR(day) =' . $year_ . ' AND MONTH(day) =' . $month_ . ' AND data_type = "'.$data_type_.'"';
			//print("$sql");exit;
		}
		$rs = mysqli_query($conn,$sql);
		if($rs){
			$data = mysqli_fetch_assoc($rs);
			for($i = 1; $i < 13; $i++){//$iは星座番号
				$ranks[] = array(
					'star' => $i,
					'score' => $data['star' . $i . '_score'],
					'rank' => $data['star' . $i . '_rank']
				);
			}
			foreach($ranks as $key=>$value){
				$tmp_ranks[$key] = $value['rank']; 	 
			}
			array_multisort($tmp_ranks,SORT_ASC,$ranks);
			$ranks = self::FormatRankArray($ranks);
			return $ranks;
		}else{
			self::$log->add('getMonthlyの実行でエラーが発生しました', 'SAVE ERR');
		}
	}
	/*
	* グラフ生成
	* 更新時刻にはdate_updateカラムの時刻を表示します。
	* @param $date_ (20160101 ...)
	* @param int $star_ (星座番号 1,2,...12)
	* return html(スクリプトタグ)
	*/
	function outputGraph($date_,$star_,$data_type_){
		global $conn;
		$date = date('Y-m-d',strtotime($date_));
		$sql = 'SELECT MONTH(day),DAY(day),star' . $star_ . '_rank AS rank,
						date_create,HOUR(date_create) AS crhour,
						date_update,HOUR(date_update) AS uphour';
		if($data_type_ == ""){
			$sql .= '	FROM star_ranks_daily';
		}else{
			$sql .= '	FROM topic_ranks_daily';
		}
		$sql .= '		WHERE is_delete = 0';
		if($data_type_ != ""){
			$sql .= '	AND data_type = "' . $data_type_ . '"';
		}
		$sql .= '		AND day > "' . $date . '" - INTERVAL 7 DAY
						AND day <="' . $date . '"
						ORDER BY DAY
						LIMIT 7';
		$result = mysqli_query($conn,$sql);

		$counter = 1;
		while($row = $result->fetch_assoc()){
			$days .= '\'' . $row['MONTH(day)'] . '/' . $row['DAY(day)'] . '\'';
			$scores .= '\'' . $row['rank'] . '\'';
			if($counter == 7){
				break; //7回目はカンマを加えず終わる。
			}
			$scores .= ',';
			$days .= ',';
			$counter++;
		}

		$update = date('n月d日') . " ";
		$update .=  date('G').'時現在';

		if($row['date_create']){
			$update = '(' . date('n月d日',strtotime($row['date_create'])) . ' ';
			$update .= $row['crhour'] .'時現在)';
		}
		// if($row['date_update'] != '0000-00-00 00:00:00' && $row['date_update']){ // del khanh 2022/02/18 MySQL5.7エラーの修正
		if($row['date_update'] != '' && $row['date_update']){ // add khanh 2022/02/18 MySQL5.7エラーの修正
 			$update = '(' . date('n月d日',strtotime($row['date_update'])) . ' ';
			$update .= $row['uphour'] .'時現在)';
		}
		if($data_type_ == "love"){
		$graph_data = <<<EOM
		<script src="/user/js/vendor/chartist.min.js"></script>
		<link href="/user/css/vendor/chartist.min.css" rel="stylesheet" type="text/css" />
		<!--[if IE 9]>
		<script src="/user/js/vendor/matchMedia.js"></script>
		<![endif]-->
		<script type="text/javascript">
		<!--
			 function init() {
				 var options = {
					 fullWidth: true,
						 chartPadding: {
						    right: 40
						 },
						 lineSmooth: Chartist.Interpolation.none({}),
					 axisY: {
						 labelInterpolationFnc: function(value) {
							 return -value;
						 },
						 labelOffset: {
							x: -3,
							y: 4
						 },
						 type: Chartist.FixedScaleAxis,
						 low: -12,
						 high: -1,
						 fullWidth: true,
						 ticks: [-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12],
						 onlyInteger: true
					 },
				 };
				 new Chartist.Line('#detail-graph-1', {
					 labels: [{$days}],
					 series: [[{$scores}]]
				 }
				 , options
				 ).on('data', function(context) {
					context.data.series = context.data.series.map(function(series) {
						return series.map(function(value) {
							return -value;
						});
					});
				}).on('draw', function(data) {
					// custom shape >>>
					// source for shape: http://www.smiffysplace.com/stars.html
					if(data.type === 'point') {
						// we are creating a new path svg element that draws a triangle around the point coordinates
						var triangle = new Chartist.Svg('path', {
							d: ["M",
								data.x + 9.3732,
								data.y + -2.2116,
								"C",
								data.x + 9.3732,
								data.y + -5.4084,
								data.x + 6.7816,
								data.y + -8,
								data.x + 3.5848,
								data.y + -8,
								"c",
								-1.5012,
								0,
								-2.8696,
								0.572,
								-3.8984,
								1.5096,
								"C",
								data.x + -1.3424,
								data.y + -7.428,
								data.x + -2.71,
								data.y + -8,
								data.x + -4.2116,
								data.y + -8,
								"C",
								data.x + -7.4084,
								data.y + -8,
								data.x + -10,
								data.y + -5.4084,
								data.x + -10,
								data.y + -2.2116,
								"c",
								0,
								2.052,
								1.0788,
								3.7608,
								2.6792,
								4.882,
								"l",
								-0.0016,
								0.0004,
								"l",
								0.0264,
								0.0148,
								"c",
								0.1964,
								0.1244,
								6.9824,
								4.5528,
								6.9824,
								4.5528,
								"s",
								6.7856,
								-4.4284,
								6.9832,
								-4.5528,
								"l",
								0.0256,
								-0.0148,
								"l",
								-0.0016,
								-0.0004,
								"C",
								data.x + 8.294,
								data.y + 1.5492,
								data.x + 9.3732,
								data.y + -0.1596,
								data.x + 9.3732,
								data.y + -2.2116,
								"z"].join(' '),
							style: 'fill-opacity: 1'
						}, 'ct-area');

						// with data.element we get the chartist svg wrapper and we can replace the original point drawn by chartist with our newly created triangle
						data.element.replace(triangle);
					}
					// <<<
				});
			 };
			var sample = setTimeout( function(){ init(); }, 200);
		-->
		</script>
			<div id="detail-graph-1" class="ct-chart ct-octave detail_graph_panel"></div>
			<div class="detail_default_graph_datetime">
				{$update}<br/>
			</div>
			<div class="detail_default_graph_desc">
				<span style="font-size: 13px;">
				※"前日"以前を表示しても、グラフは本日までの１週間分の表示になります。
				</span>
			</div>
EOM;
		}elseif($data_type_ == "work"){
		$graph_data = <<<EOM
		<script src="/user/js/vendor/chartist.min.js"></script>
		<link href="/user/css/vendor/chartist.min.css" rel="stylesheet" type="text/css" />
		<!--[if IE 9]>
		<script src="/user/js/vendor/matchMedia.js"></script>
		<![endif]-->
		<script type="text/javascript">
		<!--
			 function init() {
				 var options = {
					 fullWidth: true,
						 chartPadding: {
						    right: 40
						 },
						 lineSmooth: Chartist.Interpolation.none({}),
					 axisY: {
						 labelInterpolationFnc: function(value) {
							 return -value;
						 },
						 labelOffset: {
							x: 0,
							y: 4
						 },
						 type: Chartist.FixedScaleAxis,
						 low: -12,
						 high: -1,
						 fullWidth: true,
						 ticks: [-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12],
						 onlyInteger: true
					 },
				 };
				 new Chartist.Line('#detail-graph-1', {
					 labels: [{$days}],
					 series: [[{$scores}]]
				 }
				 , options
				 ).on('data', function(context) {
					context.data.series = context.data.series.map(function(series) {
						return series.map(function(value) {
							return -value;
						});
					});
				}).on('draw', function(data) {
					// custom shape >>>
					// source for shape: http://www.smiffysplace.com/stars.html
					if(data.type === 'point') {
						// we are creating a new path svg element that draws a triangle around the point coordinates
						var triangle = new Chartist.Svg('path', {
							d: ["M",
								data.x + 16.005,
								data.y + -5.653,
								"c",
								-0.961,
								-2.961,
								-4.141,
								-4.58,
								-7.101,
								-3.618,
								"c",
								0.044,
								0.014,
								-0.087,
								0.03,
								-0.132,
								0.046,
								"c",
								0.002,
								-0.046,
								0.004,
								-0.094,
								0.004,
								-0.139,
								"C",
								data.x + 8.777,
								data.y + -12.477,
								data.x + 6.253,
								data.y + -15,
								data.x + 3.142,
								data.y + -15,
								"c",
								-3.113,
								0,
								-5.636,
								2.523,
								-5.636,
								5.636,
								"c",
								0,
								0.045,
								0.003,
								0.093,
								0.003,
								0.139,
								"C",
								data.x + -2.534,
								data.y + -9.241,
								data.x + -2.579,
								data.y + -9.258,
								data.x + -2.622,
								data.y + -9.271,
								"C",
								data.x + -5.582,
								data.y + -10.233,
								data.x + -8.76,
								data.y + -8.614,
								data.x + -9.723,
								data.y + -5.654,
								"c",
								-0.961,
								2.96,
								0.658,
								6.14,
								3.619,
								7.102,
								"c",
								0.044,
								0.015,
								0.091,
								0.026,
								0.137,
								0.04,
								"c",
								-0.028,
								0.037,
								-0.062,
								0.073,
								-0.088,
								0.11,
								"c",
								-1.83,
								2.518,
								-1.271,
								6.042,
								1.246,
								7.871,
								"c",
								2.518,
								1.83,
								6.043,
								1.271,
								7.871,
								-1.246,
								"c",
								0.027,
								-0.038,
								0.053,
								-0.082,
								0.08,
								-0.121,
								"c",
								0.026,
								0.039,
								0.051,
								0.083,
								0.078,
								0.121,
								"c",
								1.83,
								2.518,
								5.354,
								3.076,
								7.871,
								1.247,
								"c",
								2.519,
								-1.83,
								3.076,
								-5.354,
								1.248,
								-7.872,
								"c",
								-0.027,
								-0.037,
								-0.061,
								-0.072,
								-0.088,
								-0.109,
								"c",
								0.045,
								-0.014,
								0.092,
								-0.026,
								0.137,
								-0.041,
								"C",
								data.x + 15.347,
								data.y + 0.485,
								data.x + 16.968,
								data.y + -2.693,
								data.x + 16.005,
								data.y + -5.653,
								"z"].join(' '),
							style: 'fill-opacity: 1'
						}, 'ct-area');

						// with data.element we get the chartist svg wrapper and we can replace the original point drawn by chartist with our newly created triangle
						data.element.replace(triangle);
					}
					// <<<
				});
			 };
			var sample = setTimeout( function(){ init(); }, 200);
		-->
		</script>
			<div id="detail-graph-1" class="ct-chart ct-octave detail_graph_panel"></div>
			<div class="detail_default_graph_datetime">
				{$update}<br/>
			</div>
			<div class="detail_default_graph_desc">
				<span style="font-size: 13px;">
				※"前日"以前を表示しても、グラフは本日までの１週間分の表示になります。
				</span>
			</div>
EOM;

		}elseif($data_type_ == "money"){
		$graph_data = <<<EOM
		<script src="/user/js/vendor/chartist.min.js"></script>
		<link href="/user/css/vendor/chartist.min.css" rel="stylesheet" type="text/css" />
		<!--[if IE 9]>
		<script src="/user/js/vendor/matchMedia.js"></script>
		<![endif]-->
		<script type="text/javascript">
		<!--
			 function init() {
				 var options = {
					 fullWidth: true,
						 chartPadding: {
						    right: 40
						 },
						 lineSmooth: Chartist.Interpolation.none({}),
					 axisY: {
						 labelInterpolationFnc: function(value) {
							 return -value;
						 },
						 labelOffset: {
							x: 0,
							y: 4
						 },
						 type: Chartist.FixedScaleAxis,
						 low: -12,
						 high: -1,
						 fullWidth: true,
						 ticks: [-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12],
						 onlyInteger: true
					 },
				 };
				 new Chartist.Line('#detail-graph-1', {
					 labels: [{$days}],
					 series: [[{$scores}]]
				 }
				 , options
				 ).on('data', function(context) {
					context.data.series = context.data.series.map(function(series) {
						return series.map(function(value) {
							return -value;
						});
					});
				}).on('draw', function(data) {
					// custom shape >>>
					// source for shape: http://www.smiffysplace.com/stars.html
					if(data.type === 'point') {
						pointStyle: 'circle'
					}
					// <<<
				});
			 };
			var sample = setTimeout( function(){ init(); }, 200);
		-->
		</script>
			<div id="detail-graph-1" class="ct-chart ct-octave detail_graph_panel"></div>
			<div class="detail_default_graph_datetime">
				{$update}<br/>
			</div>
			<div class="detail_default_graph_desc">
				<span style="font-size: 13px;">
				※"前日"以前を表示しても、グラフは本日までの１週間分の表示になります。
				</span>
			</div>
EOM;

		}else{
		$graph_data = <<<EOM
		<script src="/user/js/vendor/chartist.min.js"></script>
		<link href="/user/css/vendor/chartist.min.css" rel="stylesheet" type="text/css" />
		<!--[if IE 9]>
		<script src="/user/js/vendor/matchMedia.js"></script>
		<![endif]-->
		<script type="text/javascript">
		<!--
			 function init() {
				 var options = {
					 fullWidth: true,
						 chartPadding: {
						    right: 40
						 },
						 lineSmooth: Chartist.Interpolation.none({}),
					 axisY: {
						 labelInterpolationFnc: function(value) {
							 return -value;
						 },
						 labelOffset: {
							x: 0,
							y: 4
						 },
						 type: Chartist.FixedScaleAxis,
						 low: -12,
						 high: -1,
						 fullWidth: true,
						 ticks: [-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12],
						 onlyInteger: true
					 },
				 };
				 new Chartist.Line('#detail-graph-1', {
					 labels: [{$days}],
					 series: [[{$scores}]]
				 }
				 , options
				 ).on('data', function(context) {
					context.data.series = context.data.series.map(function(series) {
						return series.map(function(value) {
							return -value;
						});
					});
				}).on('draw', function(data) {
					// custom shape >>>
					// source for shape: http://www.smiffysplace.com/stars.html
					if(data.type === 'point') {
						// we are creating a new path svg element that draws a triangle around the point coordinates
						var triangle = new Chartist.Svg('path', {
							d: ["M",
								data.x,
								data.y + 5,
								"L",
								data.x + 5.878,
								data.y + 8.090,
								"L",
								data.x + 4.755,
								data.y + 1.545,
								"L",
								data.x + 9.511,
								data.y + -3.090,
								"L",
								data.x + 2.939,
								data.y + -4.045,
								"L",
								data.x + 0.000,
								data.y + -10.000,
								"L",
								data.x + -2.939,
								data.y + -4.045,
								"L",
								data.x + -9.511,
								data.y + -3.090,
								"L",
								data.x + -4.755,
								data.y + 1.545,
								"L",
								data.x + -5.878,
								data.y + 8.090,
								"L",
								data.x,
								data.y + 5,
								"Z"].join(' '),
							style: 'fill-opacity: 1'
						}, 'ct-area');

						// with data.element we get the chartist svg wrapper and we can replace the original point drawn by chartist with our newly created triangle
						data.element.replace(triangle);
					}
					// <<<
				});
			 };
			var sample = setTimeout( function(){ init(); }, 200);
		-->
		</script>
			<div id="detail-graph-1" class="ct-chart ct-octave detail_graph_panel"></div>
			<div class="detail_default_graph_datetime">
				{$update}<br/>
			</div>
			<div class="detail_default_graph_desc">
				<span style="font-size: 13px;">
				※"前日"以前を表示しても、グラフは本日までの１週間分の表示になります。
				</span>
			</div>
EOM;
		}



	return $graph_data;
	}
}

/*
* 一行のレコード(star_ranks_XXXテーブル)を使用するバージョン
* 当日のランキングは親のgetEachRanksを使用する
*/
class UranaiRankingEx extends UranaiRanking{
	
	static $topics_en = array("","love","work","money");
	static $topics_jp = array("総合運","恋愛運","仕事運","金運");
	
	function __construct($date_ ,$data_type){
		global $conn;
		$this->date = $date_;
		/*-------------- 当日のランキング --------------*/
		if($data_type == ""){
			$this->ranks = parent::getEachRanks(date('Y-m-d',strtotime($date_)),'daily');
			$sql_table_type = "star";
		}else{
			$this->ranks = parent::getTopicEachRanks(date('Y-m-d',strtotime($date_)),'daily',$data_type);
			$sql_table_type = "topic";
		}
		/*--------------　前日との差 --------------*/
		$date = date('Y-m-d',strtotime($date_));
		$pdate = date('Y-m-d',strtotime($date_ . ' -1 day'));
		$sql = 'SELECT';
		for($i=1; $i<13; $i++){
			$sql .= '(star' . $i . '_rank - (SELECT star' . $i . '_rank FROM '.$sql_table_type.'_ranks_daily WHERE day = "' . $pdate . '"';
			if($data_type != ""){
					$sql .= 'AND data_type = "'.$data_type.'"';
			}
			$sql .=')) AS "' . $i . '"';
			if($i == 12){break;}
			$sql .= ',';
		}
		$sql .= ' FROM '.$sql_table_type.'_ranks_daily WHERE day = "' . $date . '"';
		if($data_type != ""){
				$sql .= 'AND data_type = "'.$data_type.'"';
		}
		$result = mysqli_query($conn,$sql);
		if($result){
			$resultCount = mysqli_num_rows($result);
		}
		if($result && $resultCount > 0){
			$data = mysqli_fetch_assoc($result);
			foreach($data as $key => $val){
				if($val == 0){
					$this->updown_rank[] = array($key => '変化はありませんでした');
					$this->updown_rank['mark'][$key] = "―";
				}else if($val < 0){
					$this->updown_rank[] =  array($key => abs($val) . '位アップしました');
					$this->updown_rank['mark'][$key] = "↑";
				}else{
					$this->updown_rank[] = array($key => $val . '位ダウンしました');
					$this->updown_rank['mark'][$key] = "↓";
				}
			}
		}
	}
	
	/*
	* データタイプをランダムで出力
	* @return 
	*/
	static function randomDataType(){
		$type_id = array_rand(self::$topics_en, 1);
		$d_type['en'] = self::$topics_en[$type_id];
		$d_type['jp'] = self::$topics_jp[$type_id];
		return $d_type;
	}
	
	/*
	* データタイプの種類を取得する
	* @param int $id_
	* @return love
	*/
	static function getDataTypeName($id_) {
		$d_type['jp'] = self::$topics_jp[$id_];
		return $d_type;
	}
}
