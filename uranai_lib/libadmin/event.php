<?php

class Event {
	
	//イベント期間
	private static $event_reriod_start = "2017-06-30 00:00:00";
	private static $event_reriod_end = "2017-07-14 23:59:59";
	
	//イベント告知期間
	private static $announcement_strt = "2017-06-26 00:00:00";
	private static $announcement_end = "2017-06-29 23:59:59";
	private static $DESIGN_NAME = "tanabata_ev_";
	
	
	private static function event_period_tdy() {
		return date("Y-m-d H:i:s");
	}
	
	
	static function getDesignName($mode) {	
		$design_name = array();
		
		if(self::event_period_tdy() >= self::$event_reriod_start && self::event_period_tdy() <= self::$event_reriod_end){
			$design_name["ev_name"] = self::$DESIGN_NAME;
			if($mode == "rank"){
				$design_name["class_name"] = "tanabata_top";
			}else{
				$design_name["class_name"] = "tanabata";
			}
			$design_name["ev_bg"] = "<div class=\"event_background\">";
			$design_name["ev_bg"] .= "<img class=\"evbg_default\" src=\"/user/img_re/back_sasa.png\" alt=\"笹の画像\">";
			$design_name["ev_bg"] .= "<img class=\"evbg_change\" src=\"/user/img_re/back_sasa2.png\" alt=\"笹の画像短冊付き\" style=\"display:none;\">";
			$design_name["ev_bg"] .= "</div>";
			$design_name["sns"] = "ON";
			$design_name['remaining'] = true;
		}else{
			// 規定
			$design_name["ev_name"] = "n-";
			$design_name["class_name"] = "designA";
			$design_name["ev_end_name"] = "tanabata_ev_";
		}
		
		return $design_name;
	}
	
	
	static function getAnnouncement() {
		$announcement = array();
		
		if(self::isActive()){
			list($end_day, $end_hour) = explode(" " ,self::$announcement_end);
			list($today,$today_hour) = explode( " ",self::event_period_tdy());
			$remaining = strtotime($end_day) - strtotime($today);
			
			$announcement["remaining"] = date("j",$remaining);
			$announcement["ev_name"] = self::$DESIGN_NAME;
			$link = smarty_function_sitelink(array('mode' => 'announcement'));
			$announcement["ev_banner"] = "<div class=\"event_banner_box\">";
			$announcement["ev_banner"] .= "<a href=\"".$link."\">";
			$announcement["ev_banner"] .= "<img class=\"hidden-xs hidden-sm\" src=\"/user/img_re/".self::$DESIGN_NAME."banner.png\" alt=\"開催予告/星空に願いをこめて\" width=\"100%\">";
			$announcement["ev_banner"] .= "<img class=\"visible-xs visible-sm\" src=\"/user/img_re/".self::$DESIGN_NAME."banner_sm.png\" alt=\"開催予告/星空に願いをこめて\" width=\"100%\">";
			$announcement["ev_banner"] .= "</a></div>";
		}
		
		return $announcement;
	}
	
	
	static function isActive() {
		return self::event_period_tdy() >= self::$announcement_strt && self::event_period_tdy() <= self::$announcement_end;
	}
}
