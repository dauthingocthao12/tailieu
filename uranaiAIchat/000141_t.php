<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000141 extends UranaiPlugin {

	$CON_DB_GET_DATA = NULL;

	function run($CONTENTS) {

		$RESULT_ALL = array();
		$RESULT = array();

		$RESULT_ALL = con_db_get_data();
		$RESULT = RESULT_ALL["run"];

		return $RESULT;
	}

	function topic_run($TOPIC_CONTENTS) {

		$TOPIC_RESULT_ALL = array();
		$TOPIC_RESULT = array();

		$TOPIC_RESULT_ALL = con_db_get_data();
		$TOPIC_RESULT = TOPIC_RESULT_ALL["topic_run"];

		return $TOPIC_RESULT;
	}

	function con_db_get_data() {

		$date_check = false;

		if (!is_null($this->CON_DB_GET_DATA)) {	//CON_DB_GET_DATAの値が空っぽだったら
			return $this->CON_DB_GET_DATA;
		}

		//	DB接続情報


		//	data取得

		$RESULT[$star_num] = $rank_num;

		$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);

		$this->CON_DB_GET_DATA["run"] = $RESULT;
		$this->CON_DB_GET_DATA["topic_run"] = $TOPIC_RESULT;


		if(!$date_check) {
			print $this->logDateError().PHP_EOL;
		}

		return $this->CON_DB_GET_DATA;
	}
}
