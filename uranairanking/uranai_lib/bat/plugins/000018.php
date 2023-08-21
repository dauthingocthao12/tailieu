<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000018 extends UranaiPlugin {

	function run($URL) {

		foreach ($URL as $key => $url) {
			$url = str_replace("(md)", date("md"), $url);
			$url = str_replace("(ymd)", date("ymd"), $url);
			$URL[$key] = $url;
		}

		$CONTENTS = $this->load($URL);

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


		$RESULT = array();
		$content = $CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8", "SJIS");
		$LINES = explode("\n", $content);
		$chk_flg = 0;
		foreach ($LINES AS $key => $line) {
			if (count($RESULT) == 12) { break; }
			if ($chk_flg == 0 && preg_match("/<table width=\"750\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">/", $line, $MATCHES)) {
				$chk_flg = 1;
				continue;
			}
			if ($chk_flg == 1 && preg_match("/<img src=\"img\/rank_(\d{1,2})\.gif\" width=\"28\" height=\"28\">/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				$chk_flg = 2;
			} elseif ($chk_flg == 2 && preg_match("/<a href=\".*?\" class=\".*?\">(.*?座)<\/a>/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank_num;
				$chk_flg = 1;
			}
		}

		return $RESULT;
	}
}
