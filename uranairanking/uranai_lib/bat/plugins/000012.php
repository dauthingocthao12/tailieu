<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000012 extends UranaiPlugin {

	function run($URL) {

		$CONTENTS = $this->load($URL);
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		$pattern = "<h3>(.*?座)<\/h3>";

		// date check
		$date_ok = false;
		$now = self::getToday();
		$date_pattern = "/<h2><strong>0?{$now['month']}月0?{$now['day']}日<\/strong>の運勢ランキング<\/h2>/";
		$date_ok = preg_match($date_pattern, $content);
		if(!$date_ok) {
			print $this->logDateError().PHP_EOL;
			return null;
		}

		preg_match_all("/$pattern/", $content, $MATCHES);

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

		$RESULT = array();
		$i = 1;
		foreach ($MATCHES[1] as $key => $value) {
			$RESULT[$star[$value]] = $i;
			$i++;
		}

		return $RESULT;
	}
}
