<?php
function linkCalendar($my_today_offset_,$detail_star_ = NULL){
	//print_r($my_today_offset_);
	list($my_today_offset,$data_type) = $my_today_offset_;
	$detail_star = $detail_star_;
	//print($my_today_offset);
	$my_today_offset = date('Y-m-d',strtotime($my_today_offset));
	//print($my_today_offset);
	$day_name = Array("日","月","火","水","木","金","土");
	$month_days = Array(31,28,31,30,31,30,31,31,30,31,30,31);
	//指定された月の曜日番号と、月の日数を取得
	if(preg_match('/^([0-9]{4}-[0-9]{2})-([0-9]{2})$/',$my_today_offset,$match)){
		global $holidays;
		//$this_date = date('Y年n月d日',strtotime($match[0]));
		$this_date = date('Y年n月',strtotime($match[0])); // add kimura 2017/03/27 表示崩れ対応Y年n月だけ出力
		//	$prev_date = date('Ymd', strtotime($match[0] .' -1 month'));
		//	$next_date = date('Ymd', strtotime($match[0] .' +1 month'));
		//$total_day = $match[0];
		//$monthly = ltrim($match[2],"0");
		//$ym = $match[1].$match[2];
		$week_num = date('w',strtotime("first day of $match[1]"));
		$day_count = date('t',strtotime("first day of $match[1]"));
	}
	$tableline = ceil(($week_num + $day_count)/7);
	//print($tableline);
	$cell_count = $tableline * 7;
	for($i=0; $i<$cell_count; $i++){
		$monthlytable[$i] = "-";
	}
	$month_day = $day_count + $week_num;
	for($i=$week_num; $i<$month_day; $i++){
		$days = $days + 1;
		$day = sprintf("%02d",$days);
		$ymd = str_replace("-","","$match[1]").$day;
		$monthlytable[$i] = array($days,$ymd);
	}
	//print_r($monthlytable);
	//day[0]日付0なしday[1]年月日day[2]祝日
	foreach($monthlytable as $day){
		$current_class = '';
		//祝日処理
		$holidays_flg ="";
		$ho_year = substr($day[1],0,4);
		$ho_month = substr($day[1],4,2);
		$ho_day = substr($day[1],6,8);
		$ho_date = "$ho_year-$ho_month-$ho_day";
		
		if($holidays[$ho_date]){
			$holidays_flg = 'current_holidays';
		}
		if($day[1] <= date('Ymd') && $day[1] >= PREV_DATE && $day[0] !== "-"){
			if($day[1]==str_replace('-', '', $my_today_offset)) {
				$current_class = 'link_day_current';
			}
	//print($data_type."aaa");
			if($data_type !="" && $data_type !="money" && $day[1] < PREV_DATE_DTL){
				$day_list[] = "<span class=\"future_day $holidays_flg\">".$day[0]."</span>";
			}elseif($data_type =="money" && $day[1] < PREV_DATE_DTL_M){
				$day_list[] = "<span class=\"future_day $holidays_flg\">".$day[0]."</span>";
			}else{
				if(isset($detail_star_)){
						
					if($data_type == ""){
						$link = smarty_function_sitelink(array('mode' => "detail", 'd' => $day[1], 'star' => $detail_star_));
					}else{
						$link = smarty_function_sitelink(array('mode' => "detail", 'topic' => $data_type , 'd' => $day[1], 'star' => $detail_star_));
					}
					$day_list[] = "<span class=\"link_day $current_class $holidays_flg\"><a href=\"$link\">".$day[0]."</a></span>";
				}else{
					if($data_type == ""){
						$link = smarty_function_sitelink(array('mode' => "rank" ,'d' => $day[1]));
					}else{
						$link = smarty_function_sitelink(array('mode' => "rank" ,'topic' => $data_type , 'd' => $day[1]));
					}
					$day_list[] = "<span class=\"link_day $current_class $holidays_flg\"><a href=\"$link\">".$day[0]."</a></span>";
				}
			}
		}else{
			$day_list[] = "<span class=\"future_day $holidays_flg\">".$day[0]."</span>";
		}
	}
	//echo $data_type;
	$calendar_date = array($day_list,$day_name,$this_date,$detail_star);
	return $calendar_date;
}
?>
