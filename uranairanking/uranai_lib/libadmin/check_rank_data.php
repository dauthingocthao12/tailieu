<?php

function site_check() {
	//アクティブサイト一覧の前前日から前日までのデータを出す。(まだ取得できていないサイトもチェックできるように)
	global $conn;
	$data = array();
	$date1 = date('Y-m-d',strtotime('-1 day'));
	$date2 = date('Y-m-d',strtotime('-2 day'));

	$sql = make_diff_sql($date1,$date2);
	

	$rs = $conn->query($sql);
	$site_id = "";
	$star = "";
	$rank = "";
	$return_array = array();
	$error = "";
	if($rs) {
		while($row = $rs->fetch_assoc()) {
			if(!$site_id && !$star){
				$site_id = $row['site_id'];
				$CHECK[$row['site_id']][$row['day']][$row['star']] = $row['rank']; 
			}elseif($site_id != $row['site_id']){
				
				if(!isset($CHECK[$site_id][$date1]) || !isset($CHECK[$site_id][$date2])){
					$error .= '<br><br>site_id'.$site_id.'の値が不足しています。別の日に再度試してください<br>';
					$error .= make_diff_sql($date1,$date2,$site_id);
				}elseif($CHECK[$site_id][$date1] !== $CHECK[$site_id][$date2]){
					
				}else{
					$error .= '<br><br>site_id'.$site_id.'の値が全く同じ様です。確認してください。<br>';
					$error .= make_diff_sql($date1,$date2,$site_id);
				}
				$CHECK[$row['site_id']][$row['day']][$row['star']] = $row['rank']; 
				unset($CHECK[$site_id]);
				$site_id = $row['site_id'];
				$star = $row['star'];
				$rank = $row['rank'];
				
			}elseif($site_id == $row['site_id']){
				$CHECK[$row['site_id']][$row['day']][$row['star']] = $row['rank']; 
			}
			
			
			
		}
		if(!isset($CHECK[$site_id][$date1]) || !isset($CHECK[$site_id][$date2])){
			$error .= '<br><br>site_id'.$site_id.'の値が不足しています。別の日に再度試してください<br>';
			$error .= make_diff_sql($date1,$date2,$site_id);
		}elseif($CHECK[$site_id][$date1] !== $CHECK[$site_id][$date2]){

		}else{
			$error .= '<br><br>site_id'.$site_id.'の値が全く同じ様です。確認してください。<br>';
			$error .= make_diff_sql($date1,$date2,$site_id);
		}
		unset($CHECK[$site_id]);
	}
	if(!$error){
		$error = '怪しげなサイトはありません。';
	}
	$return_array['data'] = $error;
	$return_array['message'] = "";
	$return_array['title'] = "errorチェック";
	return $return_array;

}
function make_diff_sql($date1,$date2,$site_id = NULL){
	$where = "";
	if($site_id){
		$where = " AND site_id ='".$site_id."'";
	}
	$sql .= "SELECT *";
	$sql .= " FROM `log`";
	$sql .= " WHERE `is_delete`=0";
	$sql .= " AND day BETWEEN '".$date2."' AND '".$date1."'";
	$sql .= $where;
	$sql .= " ORDER BY site_id,star ";
	
	return $sql;
}

