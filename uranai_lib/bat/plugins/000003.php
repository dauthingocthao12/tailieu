<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000003 extends UranaiPlugin {

	function run($URL) {

		foreach ($URL as $key => $url) {
			$url = str_replace("(md)", date("md"), $url);
			$url = str_replace("(ymd)", date("ymd"), $url);
			$URL[$key] = $url;
		}

		$CONTENTS = $this->load($URL);

		$RESULT = array();
		foreach ($CONTENTS AS $key => $content) {
			$content = mb_convert_encoding($content, "UTF-8", "SJIS");
			$LINES = explode("\n", $content);
			$chk_flg = 0;
			foreach ($LINES as $line) {
				if (preg_match("/<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" class=\"obi-brown\">/", $line, $MATCHES)) {
					$chk_flg = 1;
					continue;
				}
				if ($chk_flg == 1 && preg_match("/(\d{1,2})位<\/h2><\/td>/", $line, $MATCHES)) {
					$RESULT[$key] = $MATCHES[1];
					break;
				}
			}
		}

		return $RESULT;
	}
}
