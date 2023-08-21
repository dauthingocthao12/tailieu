<?php

class YearNav{//$tab に英語の月かtotalが入る初期値の時は02など月が入る
	/**
	 * 月・日の基本情報を取得
	 *
	 * @author Azet
	 * @param string $year_past (例：2016) $data_type_ (例：love) 
	 * @return [start] => 1451574000 [end_month] => 1498873135 [end_year] => 1470015535 [year] => 2016 [month] => 01 ) Array ( [start] => 1451574000 [end_month] => 1498873135 [end_year] => 1470015535 [year] => 2016 [month] => 01 ) Array ( [start] => 1451574000 [end_month] => 1498873135 [end_year] => 1470015535 [year] => 2016 [month] => 01
	 */
	static function GetBasic($year_past,$data_type_){		
		if($data_type_ == ""){
			$start = strtotime(PREV_DATE);//サイトが集計を開始した日付　 20160101
		}else{
			if($data_type_ == 'money'){
				$first_day = date("Y-m-01",strtotime(PREV_DATE_DTL_M));//恋愛運等開始日　 20170623
			}else{
				$first_day = date("Y-m-01",strtotime(PREV_DATE_DTL));//恋愛運等開始日　 20170623
			}
			//print $first_day;
			$start = strtotime("$first_day +1 month");//恋愛運表示月　2017-07-01
		}
		$basic = array();
		$basic['start'] = $start;
		$basic['end_month'] = strtotime('first day of last month');//先月の1日
		$basic['end_year'] = strtotime('first day of last year');//去年の1日
		$basic['year'] = date('Y',$start);
		$basic['month'] = date('m',$start);
		return $basic;
	}
	
	/**
	 * 月間のリンク一覧を取得
	 *
	 * @author Azet
	 * @param string $year_past (例：2016) $data_type_ (例：love) 
	 * @return [2017] => Array ( [01] => ranking2017/january [02] => ranking2017/february …)
	 */
	static function Get_Month_Link($year_past,$data_type_){
		$basic = self::GetBasic($year_past,$data_type_);
		global $month_past_formatB;//　英語　月の名前
		$dateList = array();
			$start = $basic['start'];
			$year = $basic['year'];
			$month = $basic['month'];
			while($start <= $basic['end_month']){
				if($data_type_ == ""){
					$linkM = 'ranking' . $year . '/' . $month_past_formatB[$month];
					
				}else{
					$linkM = $data_type_ .'/ranking' . $year . '/' . $month_past_formatB[$month];
				
				}
				$dateList[$year][$month] = $linkM;
				$start = strtotime('+1 month',$start);
				$month = sprintf("%02d",$month + 1);
				if($month > 12){
				$month = sprintf("%02d",1);
				$year = $year+1;
				}
			}
			return $dateList;

	}
	/**
	 * 年間のリンク一覧を取得
	 *
	 * @author Azet
	 * @param string $year_past (例：2016) $data_type_ (例：love) 
	 * @return [2017] => Array ( [2016] => ranking2016/total )
	 */
	static function Get_Year_Link($start,$data_type_){
		$basic = self::GetBasic($start,$data_type_);
		$dateList = array();
		$start = $basic['start'];
		$year = $basic['year'];
			while($start <= $basic['end_year']){
				if($data_type_ == ""){
					$linkY = 'ranking' . $year . '/' . 'total';
					
				}else{
					$linkY = $data_type_ .'/ranking' . $year . '/' . 'total';
				}
				$dateList[$year] = $linkY;
				$year = $year + 1;
				$start = strtotime('+1 year',$start);
			}
			return $dateList;
	}
}
	
class SelectFortune{
	
		/**
		 * 年間のタブリンクを取得
		 *
		 * @author Azet
		 * @param string $year_past (例：2016) $data_type_ (例：love) 
		 * @return love/ranking2016/total
		 */
		static function Tab_Link_Year($year_past,$data_type_){
		$basic = YearNav::GetBasic($year_past,$data_type_);
		$link = array();
		$last_y = date('Y',$basic['end_year']);
		$start_y = date('Y',$basic['start']);
			if($year_past == date('Y')){
			$year_past = date('Y',strtotime("-1 year"));
			}
				if($data_type_ == ""){
					$link = 'ranking' . $year_past . '/' . 'total';
					
				}else{
					$link = $data_type_ .'/ranking' . $year_past . '/' . 'total';
				}
			return $link;
		
		}
		/**
		 * 月間のタブリンクを取得
		 *
		 * @author Azet
		 * @param string $year_past (例：2016) $data_type_ (例：love) 
		 * @return love/ranking2017/july
		 */
	static function Tab_Link_Month($year_past,$data_type_){
		$link = array();
		if($data_type_ == 'money'){
			$start_year = date("Y",strtotime(PREV_DATE_DTL_M));
		}else{
			$start_year = date("Y",strtotime(PREV_DATE_DTL));
		}
		if($data_type_ == ""){
			$link = 'ranking' . $year_past . '/' . 'january';

		}else{
			if($year_past < $start_year){
				$year_past = $start_year;
				if($data_type_ == 'money'){
					$month_name = 'october';
				}else{
					$month_name = 'july';
				}
				$link = $data_type_ .'/ranking' . $year_past . '/' . $month_name;
			
			}else{
			$link = $data_type_ .'/ranking' . $year_past . '/' . 'january';
			}
		}
		return $link;
		
	}

		/**
		 * 月間の他の星座リンクを取得
		 *
		 * @author Azet
		 * @param string $year_past (例：2016) $month_past_ (例：07) $data_type_ (例：love) 
		 * @return Array ( [defolt] => ranking2017/july [tab] => love/ranking2016/total )
		 */
	static function Select_Fortune_Month($year_past,$month_past_,$data_type_){
		$basic = YearNav::GetBasic($year_past,$data_type_);
		global $month_past_formatB;//　英語　月の名前
		global $DATA_TYPE;
		$link = array();
		$fortune = $DATA_TYPE;
		unset($fortune[4]); //healthはリリースしていないので外します。
		
		$month = date('m',$basic['end_month']);
		foreach($fortune as $k => $f){
			$type = $f;
			if($f == $data_type_){
					continue;
			}
			if($f == ""){
				$type = "defolt";
			}
			if($f == ""){
					$link[$type] .= 'ranking' . $year_past . '/' . $month_past_formatB[$month_past_];
					
			}else{
					$link[$type] .= $f .'/ranking' . $year_past . '/' . $month_past_formatB[$month_past_];
					
			}
				
		}
		$link['tab'] .= self::Tab_Link_Year($year_past,$data_type_);
		return $link;

	}
		/**
		 * 年間の他の星座リンクを取得
		 *
		 * @author Azet
		 * @param string $year_past (例：2016) $month_past_ (例：07) $data_type_ (例：love) 
		 * @return Array ( [defolt] => ranking2016/total [tab] => love/ranking2017/july )
		 */
	static function Select_Fortune_Year($year_past,$month_past,$data_type_){
		$basic = YearNav::GetBasic($year_past,$data_type_);

		global $month_past_formatB;//　英語　月の名前
		global $DATA_TYPE;

		$link = array();
		$fortune = $DATA_TYPE;
		unset($fortune[4]); //healthはリリースしていないので外します。
		$month = date('m',$basic['end_month']);
		
		$money_year =  date('Y',strtotime(DB_DATE_DTL_M));
		
		foreach($fortune as $k => $f){
			$type = $f;
			if($f == $data_type_){
					continue;
			}
			if($f == ""){
				$type = "defolt";
			}
			if($f == ""){
				$link[$type] .= 'ranking' . $year_past . '/' . 'total';
				
			}else{
				if($f == 'money' && $money_year > $year_past){
				}else{
					$link[$type] .= $f .'/ranking' . $year_past . '/' . 'total';
				}
			}
		}
		$link['tab'] .= self::Tab_Link_Month($year_past,$data_type_);
		return $link;

	}
	

}
