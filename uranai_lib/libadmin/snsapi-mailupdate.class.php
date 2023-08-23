<?php

// LIBS
// - mail class
require_once dirname(__FILE__)."/mail.class.php";
// uranairanking
require_once dirname(__FILE__).'/uranairanking.class.php';

require dirname(__FILE__)."/../libadmin/account.ctrl.php";		//add okabe 2016/06/17
require_once(dirname(__FILE__) . "/../libadmin/common.php");	//add okabe 2016/06/17

require_once(dirname(__FILE__).'/../libs/Smarty.class.php');
$smarty = new Smarty;
$smarty->setTemplateDir(dirname(__FILE__)."/../templates/user/"); // Smarty はカレントディレクトリしか探さない
$smarty->setCompileDir(dirname(__FILE__)."/../templates_c/");

//======================================================================
class MailUpdateAPI extends SnsAPI {


	// 規定は本番モード (メールを送りません！)
	public $test_mode = false;
	// テスト用のユーザID
	public $test_user = 0;
	// 時間確認なし
	public $force_now = false;


	/**
	 * 送信
	 *
	 * @author Azet
	 * @paran string $msg_ 未使用
	 * @return bool
	 */
	function publish($msg_) {
		global $conn, $smarty, $holidays;

		// finding users
		// basic query
		//$sql = "SELECT `email`, `handlename`	//del okabe 2016/06/17
		$sql = "SELECT `email`, `handlename`, `birthday`
			FROM `users`
			WHERE `is_delete`=0 AND `notificationSw`=1";	//edit okabe 2016/06/20
		
		// one user test
		if($this->test_user) {
			$sql .= " AND `user_id`={$this->test_user}";
		}

		// in test mode, no hour/day check
		if(!$this->force_now) {
			// current hour
			$hour = date('H:00');
			$sql .= " AND `notificationHour`='$hour'";
			// week day check
			$week_day = date('w');	// 0=sunday
			$sql .= " AND `notification{$week_day}` = 1";
			// holidays check
			$today = date('Y-m-d');
			if($holidays[$today]) {
				$sql .= " AND `notificationHolidays` = 1";
			}

		}

		if($this->test_mode) {
			print PHP_EOL.$sql.PHP_EOL;
		}

		// ranking data 
		$today = date('Y-m-d');
		//print $today.PHP_EOL;
		$ranking = new UranaiRankingEx($today ,"");
		$RANKS = $ranking->getRanks();

		foreach($RANKS as $ranks){
			$R[$ranks[star_num]]["name"]= $ranks[name];
			$R[$ranks[star_num]]["integrated"]= $ranks[num];
		}

		$ranking_l = new UranaiRankingEx($today ,"love");
		$RANKS_L = $ranking_l->getRanks();
		foreach($RANKS_L as $ranks_l){
			$R[$ranks_l[star_num]]["love"]= $ranks_l[num];
		}

		$ranking_w = new UranaiRankingEx($today ,"work");
		$RANKS_W = $ranking_w->getRanks();
		foreach($RANKS_W as $ranks_w){
			$R[$ranks_w[star_num]]["work"]= $ranks_w[num];
		}
		
		$ranking_m = new UranaiRankingEx($today ,"money");
		$RANKS_M = $ranking_m->getRanks();
		foreach($RANKS_M as $ranks_m){
			$R[$ranks_m[star_num]]["money"]= $ranks_m[num];
		}

		$yesterday = date('Y-m-d',strtotime('-1 day'));
		//print $yesterday.PHP_EOL;
		$ranking_y = new UranaiRankingEx($yesterday ,"");
		$RANKS_Y = $ranking_y->getRanks();

		foreach($RANKS_Y as $ranks_y){
			$R_Y[$ranks_y[star_num]]["name"]= $ranks_y[name];
			$R_Y[$ranks_y[star_num]]["integrated"]= $ranks_y[num];
		}

		$ranking_y_l = new UranaiRankingEx($yesterday  ,"love");
		$RANKS_Y_L = $ranking_y_l->getRanks();
		foreach($RANKS_Y_L as $ranks_y_l){
			$R_Y[$ranks_y_l[star_num]]["love"]= $ranks_y_l[num];
		}

		$ranking_y_w = new UranaiRankingEx($yesterday  ,"work");
		$RANKS_Y_W = $ranking_y_w->getRanks();
		foreach($RANKS_Y_W as $ranks_y_w){
			$R_Y[$ranks_y_w[star_num]]["work"]= $ranks_y_w[num];
		}
		
		$ranking_y_m = new UranaiRankingEx($yesterday  ,"money");
		$RANKS_Y_M = $ranking_y_m->getRanks();
		foreach($RANKS_Y_M as $ranks_y_m){
			$R_Y[$ranks_y_m[star_num]]["money"]= $ranks_y_m[num];
		}

		$ranks=array();
//print_r( $R_Y);
		$rs = $conn->query($sql);
		$mails_count = 0;
		if($rs) {	//配信対象者が存在する場合に、一連の処理を実行する！

			for($i=1; $i<13; ++$i) {
				if($R_Y[$i]["integrated"] > $R[$i]["integrated"]){
					$R[$i]["integrated_rate"] = " ↑ ";
				}elseif($R_Y[$i]["integrated"] == $R[$i]["integrated"]){
					$R[$i]["integrated_rate"] = " － ";
				}else{
					$R[$i]["integrated_rate"] = " ↓ ";
				}

				if($R_Y[$i]["love"] > $R[$i]["love"]){
					$R[$i]["love_rate"] = " ↑ ";
				}elseif($R_Y[$i]["love"] == $R[$i]["love"]){
					$R[$i]["love_rate"] = " － ";
				}else{
					$R[$i]["love_rate"] = " ↓ ";
				}

				if($R_Y[$i]["work"] > $R[$i]["work"]){
					$R[$i]["work_rate"] = " ↑ ";
				}elseif($R_Y[$i]["work"] == $R[$i]["work"]){
					$R[$i]["work_rate"] = " － ";
				}else{
					$R[$i]["work_rate"] = " ↓ ";
				}
				
				if($R_Y[$i]["money"] > $R[$i]["money"]){
					$R[$i]["money_rate"] = " ↑ ";
				}elseif($R_Y[$i]["money"] == $R[$i]["money"]){
					$R[$i]["money_rate"] = " － ";
				}else{
					$R[$i]["money_rate"] = " ↓ ";
				}

				$str_val = $R[$i]['name'];
				$str_pad = "　　　　　";
				$mail_name = mb_substr($str_val.$str_pad,0,5,'UTF-8');

				$mail_str_num = str_pad($R[$i]['integrated'],2,' ',STR_PAD_LEFT); 

				$mail_str_num_love = str_pad($R[$i]['love'],2,' ',STR_PAD_LEFT); 

				$mail_str_num_work = str_pad($R[$i]['work'],2,' ',STR_PAD_LEFT); 
				
				$mail_str_num_money = str_pad($R[$i]['money'],2,' ',STR_PAD_LEFT); 

				$ranks[$i]['mail_line'] = "　{$mail_name}
　　　総合運 : {$R[$i]['integrated_rate']} {$mail_str_num}位
　　　恋愛運 : {$R[$i]['love_rate']} {$mail_str_num_love}位
　　　仕事運 : {$R[$i]['work_rate']} {$mail_str_num_work}位
　　　　金運 : {$R[$i]['money_rate']} {$mail_str_num_money}位";
			}

//print_r($ranks);

//			$smarty->assign('ranks', $ranks);
			//$body_common = $smarty->fetch('mailupdate-mail.tpl');	//del okabe 2016/06/17

			//add okabe start 2016/06/17
			//生まれ星座ごとの配信データ12種類を準備する 格納先は MAIL_TMP_FOLDER 下
			global $name;
			$weekday = array( "日", "月", "火", "水", "木", "金", "土" );
			$today_date = $today = date('Y年m月d日')."(".$weekday[date("w")].") ".$hour;
			$smarty->assign('today_date', $today_date);
			$bdays = MAIL_PAST_DAYS;		//n日前までのランクを取得記載
			for($i=1; $i<13; ++$i) {	//星座コードのループ
				//ランキング一覧
				$tmp_star_name = $name["star".$i];
				//星座別の数日間の変動
				$rankhistory = array();
				for($j=1; $j<=$bdays; ++$j) {
					$tmp_targetday = date("Y-m-d", strtotime("-".$j." day"));
					$weekday_name = $weekday[date("w", strtotime("-".$j." day"))];
					$tmp_dispday = date("m月d日", strtotime("-".$j." day"))."(".$weekday_name.")";

					$tmp_ranking = new UranaiRankingEx($tmp_targetday,"");
					$tmp_ranks = $tmp_ranking->getRanks();

					$tmp_ranking_l = new UranaiRankingEx($tmp_targetday,"love");
					$tmp_ranks_l = $tmp_ranking_l->getRanks();

					$tmp_ranking_w= new UranaiRankingEx($tmp_targetday,"work");
					$tmp_ranks_w = $tmp_ranking_w->getRanks();
					
					$tmp_ranking_m= new UranaiRankingEx($tmp_targetday,"money");
					$tmp_ranks_m = $tmp_ranking_m->getRanks();

					//該当星座のランクを調べる
					$juni = 0;
					$juni_l = 0;
					$k=0;
					while($k<12) {
						if ($tmp_ranks[$k]['name'] == $tmp_star_name) {
							$juni = str_pad($tmp_ranks[$k]['num'], 2,' ',STR_PAD_LEFT); 
						}
						if ($tmp_ranks_l[$k]['name'] == $tmp_star_name) {
							$juni_l = str_pad($tmp_ranks_l[$k]['num'], 2,' ',STR_PAD_LEFT); 
						}
						if ($tmp_ranks_w[$k]['name'] == $tmp_star_name) {
							$juni_w = str_pad($tmp_ranks_w[$k]['num'], 2,' ',STR_PAD_LEFT); 
						}
						if ($tmp_ranks_m[$k]['name'] == $tmp_star_name) {
							$juni_m = str_pad($tmp_ranks_m[$k]['num'], 2,' ',STR_PAD_LEFT); 
						}
						$k++;
					}
					$rankhistory[]['mail_line'] = $tmp_dispday."
　　　総合運: ".$juni."位
　　　恋愛運: ".$juni_l."位
　　　仕事運: ".$juni_w."位
　　　　金運: ".$juni_m."位
";
					$tmp_ranking = null;
				}
				$smarty->assign('ranks', $ranks[$i]);
				$smarty->assign('kikan', $bdays."日間");
				$smarty->assign('seiza_name', $tmp_star_name);
				$smarty->assign('rankhistory', $rankhistory);
				$body_common = $smarty->fetch('mailupdate-mail.tpl');	//del okabe 2016/06/17
				$body_common = str_replace("　".$tmp_star_name, "★".$tmp_star_name, $body_common);
				//作成した送信文をテンポラリディレクトリに格納する bat/tmp/下
				$file_path = MAIL_TMP_FOLDER.$i.".txt";
				file_put_contents($file_path, $body_common, LOCK_EX);
			}
			//add okabe end 2016/06/17

			while($user = $rs->fetch_object()) {
				++$mails_count;

				//add okabe start 2016/06/17
				//配信対象者の生まれ星座を誕生日から調べる
				$birthdayStar = getStarFromBirthday($user->birthday);
				//add okabe end 2016/06/17
				$body = "{$user->handlename}さん、こんにちは\n";
				//$body .= $body_common;	//del okabe 2016/06/17
				//add okabe start 2016/06/17
				$file_path = MAIL_TMP_FOLDER.$birthdayStar.".txt";
				$body .= file_get_contents($file_path);
				//add okabe end 2016/06/17

				$mail = new mail();
				$mail->set_encoding("utf-8");
				if(!$this->test_mode || preg_match("/azet\.jp/", $user->email)) {
					$ok = $mail->send(
						MAIL_SENDER_EMAIL,
						MAIL_SENDER_NAME,
						$user->email,
						MAIL_UPDATE_SUBJECT,
						$body);
				}

				// in test, we print out the first email content for debug
				if($this->test_mode && $mails_count==1) {
					print ">>>".PHP_EOL;
					print $body;
					print "<<<".PHP_EOL;
				}

				print "MailUpdate to {$user->email}".PHP_EOL;
			}
		}
		// count report
		$log_msg = "{$mails_count} メッセージを送りました";
		self::$log->add("MailUpdate API", $log_msg);

		// debug mode
		if($this->test_mode) {
			print $log_msg.PHP_EOL;
		}

		return true;
	}

}
