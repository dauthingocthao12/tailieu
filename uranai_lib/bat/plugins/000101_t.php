<?php

class Zodiac000101 extends UranaiPlugin {

    /**
     * UranaiPlugin->getParentDataを参照ください
     */
	function run($CONTENTS) {
		mb_regex_encoding("UTF-8");

		$content = $CONTENTS[0]; //全体URLを使用する
		$LINES = explode("\n", $content);
		$star = self::$starKanji;
		
//	&& preg_match(preg_match('/alt="\d{1,2}位"/',$line)===1
		foreach($LINES as $line){
			$d=date("Y/m/d");//日付の習得
			$dateCheck=preg_match('~<span class="font_min">'.$d.'<\/span>~',$line)===1;
			if($dateCheck){
				$dateCheck=true;
				break;
			}			
		}

		foreach($LINES as $line){
			$rankCheck=false;
			$holoCheck=false;
			$rankCheck=preg_match('/alt="(\d{1,2})位"/',$line,$getRank)===1;
			$holoCheck=preg_match('/alt="(.*座)"/',$line,$getHolo)===1;
			if($rankCheck){
				$rank=$getRank[1];//順位(1～12)位
			}
			if($rank&&$holoCheck){
				$holo=$getHolo[1];//$holoに星座の値を代入 例:魚座
				//$holo_num=$kanjiToNum[$holo];//変数$holo_num　$kanijToNumの変数のkeyを$holo(魚座)
				//$RESULT[$holo_num]=$rank;
				$RESULT[$star[$holo]]=$rank;
			}
		}
		if(!$dateCheck){
			print $this->logDateError().PHP_EOL;
		}
		return $RESULT;
	}
    /**
     * UranaiPlugin->getParentDataTopicを参照ください
     */
	function topic_run($topicContents) {
	//return $this->getParentDataTopic($CONTENTS);
		$topic_array = array(
		"愛情運" => "love"
		,"仕事運" => "work"
		,"金銭運" => "money"
				);
		$star = self::$starKanji;
		foreach($topicContents as $topicContent) {

			$LINES = explode("\n", $topicContent);
			foreach($LINES as $line){
				$d=date("Y/n/d");//日付の習得
				$dateCheck=preg_match('~<span>'.$d.'<\/span>~u',$line)===1;
				if($dateCheck){
					$dateCheck=true;
					break;
				}
			}
			foreach($LINES as $line){
				$love_star_score=0;//初期化
				$holoCheck=false;//初期化
				$check_love_star=false;//初期化
				
				$holoCheck=preg_match('/<span class="starsign_headline">(.*)の運勢<\/span>/', $line, $tHolo);
				$check_love_star=preg_match('/(\S*運).*<span class="star">(.*)<\/span>/',$line,$get_love_star);//サイトの中の★の数を参照する
				if($check_love_star){
					$topic_type=$topic_array[$get_love_star[1]];//運勢を取得
					$cnt_lovestar=substr_count($get_love_star[2],'★');//参照したサイトの★の数を数える
					$love_star_score=$cnt_lovestar*20;//スコアを100を満点とし計算
				}
				if($holoCheck){
					$topic_holo=$tHolo[1];//星座の取得　水瓶座
				}
				if($topic_holo&&$check_love_star){
					$topic_holo_num=$star["$topic_holo"];//$kanjiToNumのキーに星座
					$RESULT["$topic_holo_num"]["$topic_type"] =$love_star_score;
								//$RESULT[$kanjiToNum[$topic_holo]]=array("love"=>$love_star_score);
				}
			}
			if(!$dateCheck){
				print $this->logDateError().PHP_EOL;
			}

		}
		return $RESULT;
	}
	
	function topic_load($URL){
		$t_url = array();
		$TOPIC = array( 'love' => 'type=love' ,'work' => 'type=work' ,'money' => 'type=money' );
//		var_dump ($TOPIC);
		foreach($URL as $key => $url){
			$U = explode("#", $url);
			foreach($TOPIC as $k => $topic){
				$t_key = $key."_".$k;
				//登録フォームに空白が入っているとURLが正しく読み込めないため。
				$url_nospase = rtrim($U[0]);
				$t_url[$t_key] = $url_nospase.$topic."#".$U[1];
				$t_url[$t_key] = $U[0]."/?".$topic."#".$U[1];
			}
		}
//		var_dump ($t_url);
		return parent::load($t_url);
	}
}
?>