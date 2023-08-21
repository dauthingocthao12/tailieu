<?php
/**
 * @author Azet
 * @date 2016-02-24
 * @url http://top.tsite.jp/news/lifetrend/o/26954539/
 *        → http://free-fortune.jp/rank_fortune
 */
class Zodiac000040 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	function run($CONTENTS) {
		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$RESULT = array();

		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/consteDay\">{$now['month']}月{$now['day']}日（/";

        // サイト毎に星座名の設定
		// サイト毎に星座名のプラグイン個別設定
		$star["水瓶座"] = 1;
		$star["魚座"] = 2;
		$star["牡羊座"] = 3;
		$star["牡牛座"] = 4;
		$star["双子座"] = 5;
		$star["蟹座"] = 6;
		$star["獅子座"] = 7;
		$star["乙女座"] = 8;
		$star["天秤座"] = 9;
		$star["蠍座"] = 10;
		$star["射手座"] = 11;
		$star["山羊座"] = 12;

		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "EUC-JP");
		$LINES = explode("\n", $content);

		$rank_num = 1;	//順位をカウントしながら星座を検索
		$data_ptn = "/alt=\"(.*座)の今日の運勢\"/";

		//行ごとにパース
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) {
				break;
			}

			$flg = preg_match($data_ptn, $line, $MATCHES);	//各順位と星座名を取り出す
			if ($flg) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank_num;
				$rank_num++;
			}

		}

		return $RESULT;
	}

	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		$now = self::getToday();

		// star: custom
		$star["水瓶座"] = 1;
		$star["魚座"] = 2;
		$star["牡羊座"] = 3;
		$star["牡牛座"] = 4;
		$star["双子座"] = 5;
		$star["蟹座"] = 6;
		$star["獅子座"] = 7;
		$star["乙女座"] = 8;
		$star["天秤座"] = 9;
		$star["蠍座"] = 10;
		$star["射手座"] = 11;
		$star["山羊座"] = 12;

		foreach($TOPIC_CONTENTS as $topic_content){
			$TOPIC_LINES = explode("\n", $topic_content);

			$pattern = "/<div class=\"nichiun-result-todays-fortune-constellation-title-name\">(.*?座)<\/div>/";

			$date_pattern = "/\<meta property=\"og:description\" content=\"{$now['month']}月{$now['day']}日/";

			$date_check_ok = false;
			$love = 0;
			$money = 0;
			$work = 0;
			$current_row = "";

			foreach ($TOPIC_LINES as $topic_line) {

				if(!$date_check) {
					$date_check = preg_match($date_pattern , $topic_line);
					//print $date_check;
				 }

				if(preg_match($pattern, $topic_line, $MATCHES)) {
					$this_page_star = $MATCHES[1];					
					$star_num = $star["$this_page_star"];
				//	print $star_num;
				 }

				if (preg_match("/<span>恋愛運<\/span>/", $topic_line)) {
					$current_row = "love";
				}

				if ($current_row == "love" && $date_check && $star_num && preg_match("/<ol>(.*★.*)<\/ol>/", $topic_line, $MATCHES)){
					$love_content = $MATCHES[1];
					$love = substr_count($love_content , '★');
					$love_num = ($love * 20);
					$current_row = "";
				}

				if (preg_match("/<span>仕事運<\/span>/", $topic_line)) {
					$current_row = "work";
				}

				if ($current_row == "work" && $date_check && $star_num && preg_match("/<ol>(.*★.*)<\/ol>/", $topic_line, $MATCHES)){
					$work_content = $MATCHES[1];
					$work = substr_count($work_content , '★');
					$work_num = ($work * 20);
					$current_row = "";
				}

				if (preg_match("/<span>金財運<\/span>/", $topic_line)) {
					$current_row = "money";
				}

				if ($current_row == "money" && $date_check && $star_num && preg_match("/<ol>(.*★.*)<\/ol>/", $topic_line, $MATCHES)){
					$money_content = $MATCHES[1];
					$money = substr_count($money_content , '★');
					$money_num = ($money * 20);
					$current_row = "";
				}

			}
			$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);

			if(!$date_check){
				print $this->logDateError().PHP_EOL;
			}

		}
		return $TOPIC_RESULT;
	}
}
