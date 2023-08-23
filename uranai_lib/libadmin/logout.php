<?php

function logout_go() {
	$_SESSION = array();
	return array("message" => "ログアウトしました。");
}
