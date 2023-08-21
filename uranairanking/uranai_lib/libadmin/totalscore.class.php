<?php
class TotalScore{
	public function GetMonthly($year,$month){
		global $conn;
		global $en_num_star;
		global $jpn_num_star;
		$ranking = array();
		$sql = "SELECT star AS num,rank,day FROM score_monthly
							WHERE YEAR(day) = '" .  $year . "' " .
							"AND  MONTH(day) = '" . $month . "' " .
							"AND is_delete = 0";
		$rs = $conn->query($sql);	
		while($data = $rs->fetch_assoc()){
			$data['en_name'] = $en_num_star[$data['num']];// 英語名
			$data['name'] = $jpn_num_star[$data['num']]; //日本語名
			$ranking[] = $data;
		}
		return $ranking;
	}
	
	public function GetYearly($year){
		global $conn;
		global $en_num_star;
		global $jpn_num_star;
		$ranking = array();
		$sql = "SELECT star AS num,rank,day FROM score_yearly
							WHERE YEAR(day) = '" .  $year . "' " .
							"AND is_delete = 0";
		$rs = $conn->query($sql);	
		while($data = $rs->fetch_assoc()){
			$data['en_name'] = $en_num_star[$data['num']];// 英語名
			$data['name'] = $jpn_num_star[$data['num']]; //日本語名
			$ranking[] = $data;
		}
		return $ranking;
	}
}