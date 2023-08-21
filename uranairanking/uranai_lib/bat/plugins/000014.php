<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000014 extends UranaiPlugin {

	function run($URL) {

		foreach ($URL as $key => $url) {
			$url = str_replace("(md)", date("md"), $url);
			$url = str_replace("(ymd)", date("ymd"), $url);
			$URL[$key] = $url;
		}

		$CONTENTS = $this->load($URL);

		$RESULT = array();
		foreach($CONTENTS as $star_num => $content) {
			// each site
			$content = mb_convert_encoding($content, "UTF-8", "SJIS");
			$LINES = explode("\n", $content);
			$rank_num = 0;

			$line_prev = "";	//add okabe 2016/04/04
			foreach ($LINES AS $line) {
				// rank
				//del okabe start 2016/04/04
				/*
				if (preg_match("/<img src=\".*12_point_(\d{1,2})\.gif\">.*12seiza_result_rank/", $line, $MATCHES)) {
					$rank_num = $MATCHES[1];
					$RESULT[$star_num] = $rank_num;
					break;
				}
				*/
				//del okabe end 2016/04/04

				//add okabe start 2016/04/04
				if (preg_match("/>位<\//", $line)) {
					if (preg_match("/>(\d{1,2})<\//", $line_prev, $MATCHES)) {
						$rank_num = intval($MATCHES[1]);
						if ($rank_num >= 1 && $rank_num <= 12) {
							$RESULT[$star_num] = $rank_num;
						}
					}
				}
				$line_prev = $line;
				//add okabe end 2016/04/04
			}	
		}

		return $RESULT;
	}
}
