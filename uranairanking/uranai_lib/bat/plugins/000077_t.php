<?php
/*****************************************************************************/
/*   占いサイトプラグイン                                                    */
/*  夢占いの館 :https://yumeuranai-yakata.com/seiza/                         */
/*                                            作成者:山口  作成日：2017/1/20 */
/*												uodated: okabe 2017/06/19	 */
/*****************************************************************************/

/**
data[star] = rank;
星座（インデックス） => 順位
*/

class Zodiac000077 extends UranaiPlugin {

	function run($CONTENTS) {	// edited okabe 2017/06/19 $URL -> $CONTENTS
		/*サイトのhtml取得*/
		//$CONTENTS = $this->load($URL);	// del okabe 2017/06/19

		/*このプラグインは、０番のURLしか使用しない*/
		$content = $CONTENTS[0];

		/*結果代入用配列
			$RESULT[星座番号]=順位*/
		$RESULT = array();

		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		/*取得したhtmlを改行で分割し配列$LINESに代入*/
		$LINES = explode("\n", $content);
		
		/*星座名はディフォルト設定を使用*/
		$star = self::$starDefault;
		
		/*日付の表示が今日の日付と比較*/
		/*今日の日付を取得*/
		$now = self::getToday();
		/*サイトと同形式の日付データの作成*/
		$date_pattern = "/<h1>12星座占い<br>【{$now['year']}年0?{$now['month']}月0?{$now['day']}日】<\/h1>/";

		$date_check_ok = false;  //日付データのチェック状況変数
		$rank_num=0;	//順位代入用変数
		//判定
		foreach ($LINES AS $line) {
			/*12星座分のデータがリザルトにある時処理を抜ける*/
			if (count($RESULT) == 12) { break; }

			/*日付の判定*/
			
			if (!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}
			
			/*順位の取得*/
			if($date_check_ok && !$rank_num && preg_match("/<div class=\"rankno rank(\d{1,2})\">(\d{1,2})位<\/div>/", $line, $MATCHES)){
				$rank_num = $MATCHES[1];
				continue;
			}
			/*星座の取得*/
			if ($rank_num && preg_match("/<h2 class=\"name\">(.*?座)<\/h2>/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];

				/*星座ごとに結果を$RESULTに格納*/
				$RESULT[$star_num] = $rank_num;

				/*値のリセット*/
				$rank_num = 0;
			}


		}

		// エラーチェック
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		print_r ($RESULT);
		return $RESULT;
	}


	// add okabe start 2017/06/19
	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		// date check
		$now = self::getToday();
		//$star = self::$starDefault;
		// サイト毎に星座名のプラグイン個別設定
		$star = array();
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

		foreach($TOPIC_CONTENTS AS $key => $topic_content) {
			//$topic_content = mb_convert_encoding($topic_content, "UTF-8", "EUC-JP");		
			$TOPIC_LINES = explode("\n", $topic_content);
			$date_check = false;
			$star_num = 0;
			$skip_count_l = 0;
			$skip_count_m = 0;
			$skip_count_w = 0;

			foreach ($TOPIC_LINES as $topic_line) {
				//print $topic_line;
				// Date check
				if(!$date_check) {
					$date_check = preg_match("!\"date\">{$now['year']}年0?{$now['month']}月0?{$now['day']}日!", $topic_line);
				}
				// star
				if ($star_num == 0 && preg_match("!\"ttl\">(.*座)の運勢<\/h2>!", $topic_line, $MATCHES)) {
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
				}

				//love
				if ($skip_count_l == 0 && $date_check && $star_num > 0 && preg_match("!alt=\"恋愛運\"><\/h3>!", $topic_line, $MATCHES)){
					$skip_count_l = 1;
				} else {
					if ($skip_count_l > 0 && $skip_count_l < 4) {
						$skip_count_l ++;
						if ($skip_count_l == 3) {
							if (preg_match("!\"point\">(\d{1,3})点<\/span>!", $topic_line, $MATCHES)) {
								$love_num = $MATCHES[1];
								//echo $topic_line.", ".$love_num."\n";
							}
							$skip_count_l = 999;
						}
					}
				}

				//money
				if ($skip_count_m == 0 && $date_check && $star_num > 0 && preg_match("!alt=\"金運\"><\/h3>!", $topic_line, $MATCHES)){
					$skip_count_m = 1;
				} else {
					if ($skip_count_m > 0 && $skip_count_m < 4) {
						$skip_count_m ++;
						if ($skip_count_m == 3) {
							if (preg_match("!\"point\">(\d{1,3})点<\/span>!", $topic_line, $MATCHES)) {
								$money_num = $MATCHES[1];
								//echo $topic_line.", ".$money_num."\n";
							}
							$skip_count_m = 999;
						}
					}
				}

				//work
				if ($skip_count_w == 0 && $date_check && $star_num > 0 && preg_match("!alt=\"仕事運\"><\/h3>!", $topic_line, $MATCHES)){
					$skip_count_w = 1;
				} else {
					if ($skip_count_w > 0 && $skip_count_w < 4) {
						$skip_count_w ++;
						if ($skip_count_w == 3) {
							if (preg_match("!\"point\">(\d{1,3})点<\/span>!", $topic_line, $MATCHES)) {
								$work_num = $MATCHES[1];
								//echo $topic_line.", ".$work_num."\n";
							}
							$skip_count_w = 999;
						}
					}
				}

			}
			$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);
			if(!$date_check) {
				print $this->logDateError().PHP_EOL;
			}
		}

		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/19

}

