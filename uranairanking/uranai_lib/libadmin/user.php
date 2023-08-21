<?php

function user_listing($sort_column, $order) {
	global $conn;
	$data = array();

	$sql = "SELECT *
		FROM `users`
		WHERE `is_delete`=0";
	$sql.= " ORDER BY ".$sort_column." ".$order;
	$sql.=";";

	$rs = $conn->query($sql);
	if($rs) {
		while($row = $rs->fetch_assoc()) {
			$data[] = $row;
		}
	}

	return array("status" => "OK", "message" => "", "db" => $data);
}


function user_detail($user_id) {
	global $conn;
	$status = 'OK';
	$message = '';

	$sql = "SELECT *
		FROM `users`
		WHERE `user_id`='$user_id'";
	$rs = $conn->query($sql);
	if(!$rs) {
		$status = 'ERR';
		$message = 'ユーザを見つかりませんでした';
	}
	else {
		$data = $rs->fetch_assoc();
	}

	return array("status" => $status, "message" => $message, "user" => $data);
}


function user_delete($user_id) {
	return array("status" => "OK", "message" => "", "user_id" => $user_id);
}


function user_delete_do($user_id) {
	global $conn;
	$status = 'OK';
	$message = 'ユーザの削除ができました';

	// 削除処理
	$sql = "UPDATE `users` SET
		`date_delete`=NOW(),
		`who_delete`='admin',
		`is_delete`=1
		WHERE `user_id`='$user_id' LIMIT 1";
	$rs = $conn->query($sql);
	if(!$rs || $conn->affected_rows!=1) {
		$status = 'ERR';
		$message = 'ユーザの削除ができませんでした';
		$message .= '<BR>'.$sql;
	}

	return array("status" =>$status, "message" => $message, "user_id" => $user_id);
}
