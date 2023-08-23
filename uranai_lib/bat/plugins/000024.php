<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000024 extends UranaiPlugin {

	function run($URL) {

		foreach ($URL as $key => $url) {
			$url = str_replace("(md)", date("md"), $url);
			$url = str_replace("(ymd)", date("ymd"), $url);
			$url = str_replace("(Y)", date("Y"), $url);
			$url = str_replace("(M)", intval(date("m")), $url);
			$url = str_replace("(d)", intval(date("d")), $url);
			$URL[$key] = $url;
		}

		$CONTENTS = $this->load($URL);

		$now = self::getToday();
		$date_pattern = "!<h1 id=\"scope_head_fortune\">{$now['month']}月{$now['day']}日の.*</h1>!";

		$RESULT = array();
		foreach ($CONTENTS AS $key => $content) {
			//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");		
			$LINES = explode("\n", $content);
			$rank_num = 0;
			$date_check_ok = false;

			foreach ($LINES as $line) {
				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);
					continue;
				}

				if ($date_check_ok && preg_match("/HOROSCOPE RANKING<span>No.(\d{1,2})<\/span>/", $line, $MATCHES)) {
					$RESULT[$key] = $MATCHES[1];
					break;
				}
			}

			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}

		}

		return $RESULT;
	}
}
