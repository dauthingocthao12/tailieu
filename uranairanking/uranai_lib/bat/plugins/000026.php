<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000026 extends UranaiPlugin {

	function run($URL) {
		$RESULT = array();

		// CURL設定, 規定でいいです
		$this->useCurl(self::$curlParamsDefault);

		// データ読み込み
		$CONTENT = $this->load($URL);
		$content = $CONTENT[0];
		//file_put_contents("tmp.html", $content);

		$star = self::$starDefault;

		$rank_num = null;
		$date_check_ok = false;
		// date check
		$day = date("j");
		$month = date("n");
		//print "$month / $day".PHP_EOL;
		$date_reg = "/{$month}<span>月<\/span>{$day}<span>日\(.\)<\/span>/u";
		//print $date_reg.PHP_EOL;

		foreach(explode("\n", $content) as $line) {
			if (count($RESULT) == 12) { break; }

			// date check: 一回だけ
			if(!$date_check_ok) $date_check_ok = preg_match($date_reg, $line);

			// step 1 - rank
			if($date_check_ok && !$rank_num) {
				$rank_found = preg_match("/<span class=\"rank\d\d\">(\d+)位<\/span>/", $line, $match);
				if($rank_found && $match[1]>=1 && $match[1]<=12) {
					$rank_num = $match[1];
					continue;
				}
			}

			// step 2 - star
			if($date_check_ok && $rank_num) {
				$star_found = preg_match("/<span>(.+座)<\/span>/", $line, $match);
				if($star_found && $match[1]) {
					$star_name = $match[1];
					$star_num = $star[$star_name];
					$RESULT[$star_num] = $rank_num;
					$rank_num = null;
				}
			}
		}


		return $RESULT;
	}
}
