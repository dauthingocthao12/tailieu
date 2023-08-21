<?php

function AllSiteLinks(){
	global $conn;
	$sql="SELECT `site_id`,`site_name`, `etc_url`, `link_url`, `is_execute`, `site_furigana`
		FROM `site`
		WHERE is_delete = 0
		ORDER BY `site_furigana` COLLATE utf8_unicode_ci ;";
	$result = $conn->query($sql);
	$sitelinks=array();
	$count=0;
	while ($data = mysqli_fetch_assoc($result)) {
		$site_name = $data['site_name'];
		if($data['link_url'] != ""){
			$link_url = $data['link_url'];
			$site_id = $data['site_id'];
		}elseif($data['etc_url'] != ""){
			$link_url = $data['etc_url'];
			$site_id = $data['site_id'];
		}
		$flag= $data['is_execute'];
		if($flag == 1){
			$sitelinks[$site_name]['url']=$link_url;
			$sitelinks[$site_name]['site_id']=$site_id;
			$count++;
		}
	}
	$hfcou=ceil($count/2);

	return array($sitelinks,$hfcou);
}
